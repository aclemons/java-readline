#**************************************************************************
#* Makefile for libJavaReadline.so -- load library for JNI wrapper of
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
# Makefile for JNI-library libJavaReadline.so
#
# $Author$
# $Revision$
#

.PHONY: all clean

SO := $(BUILDDIR)/libJavaReadline.so

all: $(SO)

$(SO): org_gnu_readline_Readline.o
	gcc -shared $(LIBPATH) $(LIBS) org_gnu_readline_Readline.o -o $(SO)

org_gnu_readline_Readline.o: org_gnu_readline_Readline.c \
                                                   org_gnu_readline_Readline.h
	$(CC) $(INCLUDES) $(CPPFLAGS) $(CFLAGS) -c org_gnu_readline_Readline.c

org_gnu_readline_Readline.h: $(BUILDDIR)/org/gnu/readline/Readline.class
	javah -jni org.gnu.readline.Readline
	touch org_gnu_readline_Readline.h

clean:
	-rm org_gnu_readline_Readline.h org_gnu_readline_Readline.o