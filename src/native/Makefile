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
#* the Free Software Foundation Inc., 59 Temple Place - Suite 330,
#* Boston, MA  02111-1307 USA
#***************************************************************************
#
# Makefile for JNI-library libJava*.so
#
# $Author$
# $Revision$
#

export 

.PHONY: org_gnu_readline_Readline.o

ROOTDIR=../..
BUILDDIR=$(ROOTDIR)/build

ifeq (MSC,$(WIN32))
INCLUDES = -I "c:/Programme/DevStudio/VC/include" \
			-I $(JAVAINCLUDE) -I $(JAVANATINC)
LIB = "c:/Programme/DevStudio/VC/lib"
CC = cl
OBJ_EXT := obj
LIB_PRE := 
LIB_EXT := dll
CFLAGS=-DWIN32=$(WIN32) -D__IBMC__
else
INCLUDES          = -I $(JAVAINCLUDE) -I $(JAVANATINC)
LIBPATH           = -L/usr/lib/termcap 
JavaReadline_LIBS = -lreadline -ltermcap -lhistory
JavaEditline_LIBS = -ledit -ltermcap
ifeq (cygwin,$(WIN32))
JavaGetline_LIBS = -lcygwin
endif
CC = gcc
OBJ_EXT := o
LIB_PRE := lib
LIB_EXT := so
CFLAGS=-fPIC -DPOSIX
endif


OBJECTS := org_gnu_readline_Readline.$(OBJ_EXT)
ifeq ($(TG),JavaGetline)
OBJECTS += getline.$(OBJ_EXT)
endif

all: $(T_LIBS) 

lib: $(ROOTDIR)/$(LIB_PRE)$(TG).$(LIB_EXT)

JavaReadline:
	make TG=$@ lib

JavaEditline:
	make TG=$@ lib

JavaGetline:
	make TG=$@ lib

$(ROOTDIR)/$(LIB_PRE)$(TG).$(LIB_EXT): $(OBJECTS)
ifeq (MSC,$(WIN32))
	$(CC) $(INCLUDES) -LD $(OBJECTS) -Fe$@
else
ifeq (CYGWIN,$(WIN32))
	$(CC) -s -W1,--base-file,$(ROOTDIR)/lib$(TG).base -o $@ \
		$(OBJECTS) -W1,-e,_readline_init@12
	dlltool --base-file $(ROOTDIR)/lib$(TG).base \
		--def $(ROOTDIR)/lib$(TG).def \
		--output-exp $(ROOTDIR)/lib$(TG).exp \
		--dllname $(ROOTDIR)/lib$(TG).dll
	$(CC) -s -W1,--base-file $(ROOTDIR)/lib$(TG).base,$(ROOTDIR)/lib$(TG).exp \
		-o $@ $(OBJECTS) -W1,-e,_readline_init@12
	dlltool --base-file $(ROOTDIR)/lib$(TG).base \
		--def $(ROOTDIR)/lib$(TG).def \
		--output-exp $(ROOTDIR)/lib$(TG).exp \
		--dllname $(ROOTDIR)/lib$(TG).dll
	$(CC) -W1,$(ROOTDIR)/lib$(TG).exp -o $@ \
		$(OBJECTS) -W1,-e,_readline_init@12
else
	$(CC) -shared $(OBJECTS) $(LIBPATH) $($(TG)_LIBS) -o $@
endif
endif

getline.$(OBJ_EXT): getline.c
	$(CC) $(INCLUDES) $(CPPFLAGS) $(CFLAGS) -D$(TG) -c getline.c

org_gnu_readline_Readline.$(OBJ_EXT): org_gnu_readline_Readline.h \
	                                      org_gnu_readline_Readline.c
	$(CC) $(INCLUDES) $(CPPFLAGS) $(CFLAGS) -D$(TG) \
	                                   -c org_gnu_readline_Readline.c

org_gnu_readline_Readline.h: $(BUILDDIR)/org/gnu/readline/Readline.class
	javah -classpath $(BUILDDIR) -jni org.gnu.readline.Readline
	touch org_gnu_readline_Readline.h

clean:
	-rm -f org_gnu_readline_Readline.h \
		org_gnu_readline_Readline.$(OBJ_EXT) getline.$(OBJ_EXT)
