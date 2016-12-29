@echo off
SETLOCAL
REM Calculate depot path from current directory
del /F /Q %TEMP%\p4.* >%TEMP%\nul 2>&1
SET p4.cwd.depot.env=>%TEMP%\nul 2>&1
SET p4.cwd.client.env=>%TEMP%\nul 2>&1
SET p4.cwd.local.env=>%TEMP%\nul 2>&1
SET p4.change.env=>%TEMP%\nul 2>&1
SET p4.opened.env=>%TEMP%\nul 2>&1
p4 -z tag where %cd% 2>%TEMP%\p4.cmd.stderr | sed -n -e "s/^...[ \t]*depotFile[ \t]*\(.*\)/p4.cwd.depot \1/p" -e "s/^...[ \t]*clientFile[ \t]*\(.*\)/p4.cwd.client \1/p" -e "s/^...[ \t]*path[ \t]*\(.*\)/p4.cwd.local \1/p" >%TEMP%\p4.cmd.stdout 2>%TEMP%\sed.stderr
if ERRORLEVEL 2 goto P4ERROR
SET p4.cmd.error=%ERRORLEVEL%
IF EXIST %TEMP%\sed.stderr type %TEMP%\sed.stderr >> %TEMP%\p4.cmd.stderr
FOR /F "usebackq" %%i IN (%TEMP%\p4.cmd.stderr) do SET p4.cmd.error=3
IF %p4.cmd.error% GTR 2 goto P4ERROR
FOR /F "usebackq tokens=1,*" %%i IN (%TEMP%\p4.cmd.stdout) do SET %%i.env=%%j
REM Revert any pending edits from this client
p4 revert %p4.cwd.depot.env%/... >%TEMP%\nul 2>&1
REM sync property file
p4 sync -f %p4.cwd.depot.env%/p4.build.properties >%TEMP%\p4.cmd.stdout 2>%TEMP%\p4.cmd.stderr
if ERRORLEVEL 2 goto P4ERROR
REM check out property file
p4 edit %p4.cwd.depot.env%/p4.build.properties >%TEMP%\p4.cmd.stdout 2>%TEMP%\p4.cmd.stderr
if ERRORLEVEL 2 goto P4ERROR
SET p4.cmd.error=%ERRORLEVEL%
FOR /F "usebackq" %%i IN (%TEMP%\p4.cmd.stderr) do SET p4.cmd.error=3
IF %p4.cmd.error% GTR 2 goto P4ERROR
p4 changes -m 1 %p4.cwd.depot.env%/... | cut -f 2 -d " " >%TEMP%\p4.change.env 2>%TEMP%\p4.cmd.stderr
if ERRORLEVEL 2 goto P4ERROR
SET p4.cmd.error=%ERRORLEVEL%
FOR /F "usebackq" %%i IN (%TEMP%\p4.cmd.stderr) do SET p4.cmd.error=3
IF %p4.cmd.error% GTR 2 goto P4ERROR
SET /P p4.change.env=<%TEMP%\p4.change.env
REM modify property file with new change number
sed "s/\([ \t]*p4.prop.verpatchlevel[ \t]*=\)[ \t]*\(.*\)$/\1%p4.change.env%/" %p4.cwd.local.env%\p4.build.properties > %TEMP%\p4.build.properties
del /F /Q %p4.cwd.local.env%\p4.build.properties >%TEMP%\nul 2>&1
copy /Y %TEMP%\p4.build.properties %p4.cwd.local.env%\p4.build.properties >%TEMP%\nul 2>&1
REM Revert if unchanged
p4 revert -a %p4.cwd.depot.env%/... >%TEMP%\nul 2>&1
GOTO ANT
:P4ERROR
TYPE %TEMP%\p4.cmd.stderr >&2
GOTO END
:END
ENDLOCAL
GOTO :EOF
:ANT
ENDLOCAL
ant p4.nightly
