#**************************************************************************
#* Makefile for libJavaReadline.so -- load library for JNI wrapper
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
# Toplevel Makefile for Java-Readline
#
# $Author$
# $Revision$
#

TARGET    := java_readline
README    := README
CHANGELOG := ChangeLog
LICENSE   := COPYING.LIB
TODO      := TODO
NAME      := The Java-Readline Library
HOMEPAGE  := http://www.bablokb.de/java/readline.html
COPYRIGHT := Released under the LGPL, (c) Bernhard Bablok 1998-2001
WTITLE    := "$(NAME)"
DTITLE     = "$(NAME), Version $(VERSION)"
DBOTTOM   := "$(COPYRIGHT)<br>Homepage: <a href="$(HOMEPAGE)">$(HOMEPAGE)</a>"
DHEADER    = "<strong>$(NAME), Version $(VERSION)</strong>"
DFOOTER    = "<strong>$(NAME), Version $(VERSION)</strong>"
PACKROOT  := 
SUBDIRS    = src etc
PACKAGES  := test org.gnu.readline
BIN_ADD   := libJavaReadline.so
MF_STUB   := etc/manifest.stub

# native stuff

export JAVAINCLUDE := $(JAVA_HOME)/include
export JAVANATINC  := $(JAVA_HOME)/include/linux
export INCLUDES    := -I $(JAVAINCLUDE) -I $(JAVANATINC)
export LIBPATH     := -L/usr/lib/termcap
export LIBS        := -lreadline -ltermcap -lhistory

VERSION         := $(shell cat VERSION)
export ROOTDIR  := $(shell pwd)
export BUILDDIR := $(shell pwd)/build
export METADIR  := $(BUILDDIR)/META-INF
export CLASSDIR := $(BUILDDIR)
export JAR      := $(TARGET).jar
APIDIR          := ./api

export JAVAC := jikes
export CLASSPATH := $(BUILDDIR):$(JAVA_HOME)/jre/lib/rt.jar
export JAVAC_OPT := -O +E

.PHONY: src-dist bin-dist test apidoc jar subdirs $(SUBDIRS)

jar: subdirs
	$(MAKE) $(JAR)

$(JAR): $(MF_STUB) $(shell find build -type f)
ifeq ($(strip $(MF_STUB)),) 
	jar -cvf $(JAR) -C $(BUILDDIR)/ .
else
	jar -cvfm $(JAR) $(MF_STUB) -C $(BUILDDIR)/ .
endif

apidoc: $(APIDIR)
	javadoc -sourcepath src -d $(APIDIR) -windowtitle $(WTITLE) \
                -doctitle $(DTITLE) -footer $(DFOOTER) -header $(DHEADER) \
                -bottom $(DBOTTOM) \
                -version -author -private $(patsubst %,$(PACKROOT)%,$(PACKAGES))

bin-dist: jar
	tar -czf $(TARGET)-$(VERSION)-bin.tgz --exclude "CVS" \
            $(README) $(TODO) $(CHANGELOG) $(LICENSE) $(JAR) $(BIN_ADD)
	-rm -fr $(TARGET)-$(VERSION)
	mkdir $(TARGET)-$(VERSION)
	(cd $(TARGET)-$(VERSION); tar -xzf ../$(TARGET)-$(VERSION)-bin.tgz)
	tar -czf $(TARGET)-$(VERSION)-bin.tgz $(TARGET)-$(VERSION)
	-rm -fr $(TARGET)-$(VERSION)

src-dist: clean
	tar -czf $(TARGET)-$(VERSION)-src.tgz --exclude "CVS" \
            $(README) VERSION $(TODO) $(CHANGELOG) Makefile \
            $(LICENSE) $(SUBDIRS) $(SRC_ADD)
	-rm -fr $(TARGET)-$(VERSION)
	mkdir $(TARGET)-$(VERSION)
	(cd $(TARGET)-$(VERSION); tar -xzf ../$(TARGET)-$(VERSION)-src.tgz)
	tar -czf $(TARGET)-$(VERSION)-src.tgz $(TARGET)-$(VERSION)
	rm -fr $(TARGET)-$(VERSION)

subdirs: $(BUILDDIR) $(METADIR) $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

$(APIDIR):
	mkdir $(APIDIR)

$(BUILDDIR):
	mkdir $(BUILDDIR)

$(METADIR): 
	mkdir $(METADIR)

test: jar
	LD_LIBRARY_PATH=$(ROOTDIR) java -jar $(JAR) src/test/tinputrc
clean:
	-rm -fr $(JAR) $(TARGET)-*.tgz $(APIDIR) $(BUILDDIR)
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir clean; \
	done
