#**************************************************************************
#* Makefile for libJavaReadline.so -- load library for JNI wrapper of
#* of GNU readline
#*
#* Copyright (c) 1987-1998 Free Software Foundation, Inc.
#* Java Wrapper Copyright (c) 1998-1999 by Bernhard Bablok (bablokb@gmx.de)
#*
#* This program is free software; you can redistribute it and#or modify
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

JAVAC = jikes
JPATH = /usr/lib/java
JTARGETDIR = $(HOME)/classes          # must be in CLASSPATH (javah!!)
JAVAINCLUDE = $(JPATH)/include
JAVANATINC = $(JPATH)/include/linux
INCLUDES= -I $(JAVAINCLUDE) -I $(JAVANATINC)
LIBPATH = -L/usr/lib/termcap
LIBS = -lreadline -ltermcap -lhistory

all: libJavaReadline.so

libJavaReadline.so: org_gnu_readline_Readline.o
	gcc -shared -o libJavaReadline.so $(LIBPATH) \
                                          org_gnu_readline_Readline.o $(LIBS)

org_gnu_readline_Readline.o: org_gnu_readline_Readline.c \
                                                   org_gnu_readline_Readline.h
	$(CC) $(INCLUDES) $(CPPFLAGS) $(CFLAGS) -c org_gnu_readline_Readline.c

org_gnu_readline_Readline.h: ../Readline.java
	$(JAVAC) -d $(JTARGETDIR) ../Readline.java
	javah -jni org.gnu.readline.Readline
	touch org_gnu_readline_Readline.h
