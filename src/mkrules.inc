#**************************************************************************
#* Makefile for libJavaReadline.so -- load library for JNI wrapper
#* of GNU readline
#*
#* Copyright (c) 1987-1998 Free Software Foundation, Inc.
#* Java Wrapper Copyright (c) 1998-2001 by Bernhard Bablok (mail@bablokb.de)
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
# $Author$
# $Revision$
#

SUBDIRS := $(shell find . -type d -not -name "." -not -name "CVS" -maxdepth 1)
PACKDIR := $(CLASSDIR)/$(shell echo `pwd` | sed -e's!.*/src/!!')

.PHONY: all classes subdirs clean $(SUBDIRS)

JAVA_SOURCES := $(wildcard *.java)
JAVA_TARGETS := $(JAVA_SOURCES:%.java=$(PACKDIR)/%.class)

GIF_SOURCES := $(wildcard *.gif)
GIF_TARGETS := $(GIF_SOURCES:%=$(PACKDIR)/%)

all: $(PACKDIR) subdirs $(JAVA_TARGETS) $(GIF_TARGETS)

subdirs: $(SUBDIRS)

$(PACKDIR):
	mkdir $(PACKDIR)

$(SUBDIRS):
	$(MAKE) -C $@


$(PACKDIR)/%.class: %.java
	$(JAVAC) $(JAVAC_OPT) $< -d $(CLASSDIR)

$(PACKDIR)/%.gif: %.gif
	cp -vu $< $(PACKDIR)

clean:
