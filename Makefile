#**************************************************************************
#* Makefile for libJavaReadline.so -- load library for JNI wrapper of
#* of GNU readline
#*
#* Copyright (c) 1987-1998 Free Software Foundation, Inc.
#* Java Wrapper Copyright (c) 1998 by Bernhard Bablok (bablokb@gmx.de)
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
# Toplevel Makefile for Java-Readline
#
# $Author$
# $Revision$
#

.PHONY : all class native test

JAVAC = jikes
JTARGETDIR = $(HOME)/classes

all: class native test

class: Readline.class
	$(JAVAC) -d $(JTARGETDIR) Readline.java
	cp $(JTARGETDIR)/org/gnu/readline/Readline.class .

native:
	$(MAKE) -C native

test:
	$(MAKE) -C test 

