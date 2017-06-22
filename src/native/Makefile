#**************************************************************************
#* Makefile for libJava*.so -- load library for JNI wrapper of
#* of GNU readline
#*
#* Copyright (c) 1987-1998 Free Software Foundation, Inc.
#* Java Wrapper Copyright (c) 1998-2001 by Bernhard Bablok (mail@bablokb.de)
#*
#* This program is free software; you can redistribute it and/or modify
#* it under the terms of the GNU Library General Public License as published
#* by  the Free Software Foundation; either version 2 of the License or
#* (at your option) any later version.
#*
#* This program is distributed in the hope that it will be useful, but
#* WITHOUT ANY WARRANTY; without even the implied warranty of
#* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#* GNU Library General Public License for more details.
#*
#* You should have received a copy of the GNU Library General Public License
#* along with this program; see the file COPYING.LIB.  If not, write to
#* the Free Software Foundation Inc., 51 Franklin Street, Fifth Floor,
#* Boston, MA  02110-1301 USA
#***************************************************************************
#
# Makefile for JNI-library libJava*.so
#
# $Author$
# $Revision$
#

export

.PHONY: org_gnu_readline_Readline.o

CC ?= gcc

# OS-specific stuff   ---------------------------------------------------------

ifeq (LINUX,$(OS_FLAVOR))
OBJ_EXT := o
LIB_PRE := lib
LIB_EXT := so
NATIVE_CFLAGS := -fpic -DPOSIX $(CFLAGS)
LD_FLAGS = -shared

INCLUDES          = -I $(JAVAINCLUDE) -I $(JAVANATINC)
LIBPATH           = -L/usr/lib/termcap

JavaReadline_LIBS = -lreadline -ltermcap -lhistory
JavaEditline_LIBS = -ledit -ltermcap
JavaGetline_LIBS  =
endif

ifeq (CYGWIN,$(OS_FLAVOR))
OBJ_EXT := o
LIB_PRE :=
LIB_EXT := dll
NATIVE_CFLAGS := -DWIN32 $(CFLAGS)
LD_FLAGS = -shared

INCLUDES          = -I $(JAVAINCLUDE) -I $(JAVANATINC)
LIBPATH           = -L/usr/lib/termcap

JavaReadline_LIBS = -lreadline -ltermcap -lhistory
JavaEditline_LIBS = -ledit -ltermcap
JavaGetline_LIBS  = -ltermcap
endif

ifeq (MSWIN,$(OS_FLAVOR))
OBJ_EXT := obj
LIB_PRE :=
LIB_EXT := dll
NATIVE_CFLAGS := -DWIN32 $(CFLAGS)
LD_FLAGS = -LD

INCLUDES          = -I "c:/Programme/DevStudio/VC/include" \
			-I $(JAVAINCLUDE) -I $(JAVANATINC)
LIBPATH           =
LIB               = "c:/Programme/DevStudio/VC/lib"
JavaReadline_LIBS =
JavaEditline_LIBS =
JavaGetline_LIBS  =
endif

ifeq (MAC,$(OS_FLAVOR))
OBJ_EXT := o
LIB_PRE := lib
LIB_EXT := jnilib
NATIVE_CFLAGS := -fPIC -DPOSIX $(CFLAGS)
LD_FLAGS = -dynamiclib -framework JavaVM

INCLUDES          = -I/sw/include -I $(JAVAINCLUDE) -I $(JAVANATINC)
LIBPATH           = -L/usr/lib -L/sw/lib

JavaReadline_LIBS = -lreadline -ltermcap -lhistory
JavaEditline_LIBS = -ledit -ltermcap
JavaGetline_LIBS  =
endif

# some constants, you should not need to change these variables   -------------

ROOTDIR=../..
BUILDDIR=$(ROOTDIR)/build
OBJECTS := org_gnu_readline_Readline.$(OBJ_EXT)
ifeq ($(TG),JavaGetline)
OBJECTS += getline.$(OBJ_EXT)
endif

# targets, finally ;-)   ------------------------------------------------------

all: $(T_LIBS)

lib: $(ROOTDIR)/$(LIB_PRE)$(TG).$(LIB_EXT)

JavaReadline:
	$(MAKE) TG=$@ lib

JavaEditline:
	$(MAKE) TG=$@ lib

JavaGetline:
	$(MAKE) TG=$@ lib

$(ROOTDIR)/$(LIB_PRE)$(TG).$(LIB_EXT): $(OBJECTS)
ifeq (MSWIN,$(OS_FLAVOR))
	$(CC) $(LD_FLAGS) $(OBJECTS) $(LIBPATH) $($(TG)_LIBS) -Fe$@
endif
ifeq (LINUX,$(OS_FLAVOR))
	$(CC) $(LD_FLAGS) $(OBJECTS) $(LIBPATH) $($(TG)_LIBS) -o $@
endif
ifeq (MAC,$(OS_FLAVOR))
	$(CC) $(LD_FLAGS) $(OBJECTS) $(LIBPATH) $($(TG)_LIBS) -o $@
endif

getline.$(OBJ_EXT): getline.c
	$(CC) $(INCLUDES) $(CPPFLAGS) $(NATIVE_CFLAGS) -D$(TG) -c getline.c

org_gnu_readline_Readline.$(OBJ_EXT): org_gnu_readline_Readline.h \
																				org_gnu_readline_Readline.c
	$(CC) $(INCLUDES) $(CPPFLAGS) $(NATIVE_CFLAGS) -D$(TG) \
																		 -c org_gnu_readline_Readline.c

org_gnu_readline_Readline.h: $(BUILDDIR)/org/gnu/readline/Readline.class
	$(JAVA_HOME)/bin/javah -classpath $(BUILDDIR) -jni org.gnu.readline.Readline
	touch org_gnu_readline_Readline.h

clean:
	-rm -f org_gnu_readline_Readline.h \
		org_gnu_readline_Readline.$(OBJ_EXT) getline.$(OBJ_EXT)
