# Makefile for jam

CC = cc
TARGET = -o jam0
OPTIM = -O

# Special flavors - uncomment appropriate lines

# Solaris 2.0 seem not to have any identifying cpp symbol
#CFLAGS = -Dsolaris

# AIX needs -lbsd, and has no identifying cpp symbol
#LINKLIBS = -lbsd
#CFLAGS = -DAIX

# NT (with Microsoft compiler)
# Use FATFS if building on a DOS FAT file system
#Lib = $(MSVCNT)/lib
#Include = $(MSVCNT)/include
#CC = cl /nologo
#CFLAGS = -I $(Include) -DNT 
#TARGET = /Fejam0
#LINKLIBS = $(Lib)/kernel32.lib $(Lib)/libc.lib

# Use FATFS if building on a DOS FAT file system
#CFLAGS = $(CFLAGS) -DFATFS

install: jam0
	jam0 -f Jambase install

jam0:
	$(CC) $(TARGET) $(OPTIM) $(CFLAGS) *.c $(LINKLIBS)
