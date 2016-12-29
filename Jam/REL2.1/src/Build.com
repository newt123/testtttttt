! Bootstrap build script for Jam
$ cc command.c
$ cc compile.c
$ cc expand.c
$ cc execvms.c
$ cc filevms.c
$ cc glob.c
$ cc hash.c
$ cc headers.c
$ cc lists.c
$ cc make.c
$ cc make1.c
$ cc newstr.c
$ cc option.c
$ cc parse.c
$ cc regexp.c
$ cc rules.c
$ cc scan.c
$ cc search.c
$ cc timestamp.c
$ cc variable.c
$ cc jam.c
$ cc jamgram.c
$ link/exe=jam.exe command.obj, compile.obj, execvms.obj, expand.obj, -
    filevms.obj, glob.obj, hash.obj, headers.obj, lists.obj, make.obj, -
    make1.obj, newstr.obj, option.obj, parse.obj, regexp.obj, rules.obj, -
    scan.obj, search.obj, timestamp.obj, variable.obj, jam.obj, jamgram.obj
$ write sys$output "If that was successful, define a symbol for jam:"
$ write sys$output ""
$ write sys$output "    jam :== $xxxx:jam.exe -f xxxx:jambase"
$ write sys$output ""
$ write sys$output "and then run jam here."
