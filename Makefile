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

export 

# what to build   -------------------------------------------------------------

T_LIBS    = JavaReadline

# java-compiler flavor   ------------------------------------------------------

## normal javac
JAVAC = javac
JC_FLAGS = 

## with jikes
#JAVAC = jikes
#JC_FLAGS = -O +E

# installation directories   --------------------------------------------------

PREFIX    = /usr
BINLIBDIR = $(PREFIX)/lib
DOCDIR    = $(PREFIX)/doc
JAVALIBDIR= $(PREFIX)/share/java

# OS-specific stuff   ---------------------------------------------------------

# Operating system/compiler dependent, default is LINUX.
# Note that both CYGWIN and MSWIN use the cygwin-environment,
# but the latter uses a native MS-compiler.

OS_FLAVOR = LINUX
#OS_FLAVOR = CYGWIN
#OS_FLAVOR = MSWIN
#OS_FLAVOR = MAC

# Linux

ifeq (LINUX,$(OS_FLAVOR))
JAVAINCLUDE = $(JAVA_HOME)/include
JAVANATINC  = $(JAVA_HOME)/include/linux
endif

# Cygwin (builds fine, but does not work)

ifeq (CYGWIN,$(OS_FLAVOR))
DRIVE_C := /cygdrive/c
JAVA_HOME := c:/j2sdk1.4.0
JAVAINCLUDE = $(JAVA_HOME)/include
JAVANATINC := $(JAVA_HOME)/include/win32
PATH:=$(subst c:,$(DRIVE_C),$(JAVA_HOME))/bin:$(PATH)
endif

# Visual C++ (only suppports getline)

ifeq (MSWIN,$(OS_FLAVOR))
DRIVE_C := /cygdrive/c
JAVA_HOME := c:/j2sdk1.4.0
JAVAINCLUDE = $(JAVA_HOME)/include
JAVANATINC := $(JAVA_HOME)/include/win32
PATH:=$(subst c:,$(DRIVE_C),$(JAVA_HOME))/bin:$(PATH)
PATH:=$(DRIVE_C)/Programme/DevStudio/VC/bin:$(DRIVE_C)/Programme/DevStudio/SharedIDE/bin/:$(PATH)
T_LIBS = JavaGetline
ARGS   = Getline
endif

# MAC
ifeq (MAC,$(OS_FLAVOR))
JAVAINCLUDE = /System/Library/Frameworks/JavaVM.framework/Headers
JAVANATINC  =
endif

# some constants, you should not need to change these variables   -------------

TARGET    = libreadline-java
README    = README README.1st
NEWS      = NEWS
CHANGELOG = ChangeLog
LICENSE   = COPYING.LIB
TODO      = TODO
NAME      = The Java-Readline Library
HOMEPAGE  = http://java-readline.sourceforge.net/
COPYRIGHT = Released under the LGPL, (c) Bernhard Bablok, Henner Zeller 1998-2002
WTITLE    = "$(NAME)"
DTITLE    = "$(NAME), Version $(VERSION)"
DBOTTOM   = "$(COPYRIGHT)<br>Homepage: <a href="$(HOMEPAGE)">$(HOMEPAGE)</a>"
DHEADER   = "<strong>$(NAME), Version $(VERSION)</strong>"
DFOOTER   = "<strong>$(NAME), Version $(VERSION)</strong>"
BIN_ADD   = $(README) $(NEWS) $(TODO) $(CHANGELOG) $(LICENSE) \
             $(JAR) *.so $(APIDIR)
SRC_ADD   = $(README) $(NEWS) $(TODO) $(CHANGELOG) $(LICENSE) \
             Makefile VERSION $(SUBDIRS) contrib src etc
MF_STUB   = etc/manifest.stub

VERSION         = `cat VERSION`
JAR             = $(TARGET).jar
APIDIR          = ./api
BUILDDIR        = ./build
RPM_BASE        = `pwd`/$(BUILDDIR)/

# targets, finally ;-)   ------------------------------------------------------

world : jar build-native

jar: build-java
	cd $(BUILDDIR) ; jar -cvmf ../$(MF_STUB) ../$(JAR) *

$(JAR):
	cd $(BUILDDIR) ; jar -cvmf ../$(MF_STUB) ../$(JAR) *

build-java: $(BUILDDIR)
	cd src ; $(MAKE) JAVAC="$(JAVAC)" JC_FLAGS="$(JC_FLAGS)" \
		OS_FLAVOR=$(OS_FLAVOR) java

build-native: 
	cd src; $(MAKE) T_LIBS="$(T_LIBS)" JAVAINCLUDE="$(JAVAINCLUDE)" \
		        OS_FLAVOR=$(OS_FLAVOR) JAVANATINC="$(JAVANATINC)" native

apidoc: $(APIDIR)
	javadoc -sourcepath src -d $(APIDIR) -windowtitle $(WTITLE) \
                -doctitle $(DTITLE) -footer $(DFOOTER) -header $(DHEADER) \
                -bottom $(DBOTTOM) \
                -version -author org.gnu.readline test

install: jar build-native apidoc
	install -D $(JAR)    $(DESTDIR)$(JAVALIBDIR)/$(JAR)
	install -d $(DESTDIR)$(BINLIBDIR)
	install  *.so        $(DESTDIR)$(BINLIBDIR)
	install -d $(DESTDIR)$(DOCDIR)/$(TARGET)-$(VERSION)
	cp -r api $(DESTDIR)$(DOCDIR)/$(TARGET)-$(VERSION)

bin-dist: jar build-native apidoc
	mkdir -p "$(TARGET)-$(VERSION)"
	-cp -r $(BIN_ADD) "$(TARGET)-$(VERSION)"
	tar -czf $(TARGET)-$(VERSION)-bin.tar.gz --exclude "CVS" "$(TARGET)-$(VERSION)"
	rm -rf "$(TARGET)-$(VERSION)"

src-dist: clean
	mkdir -p "$(TARGET)-$(VERSION)"
	-cp -r $(SRC_ADD) "$(TARGET)-$(VERSION)"
	tar -czf $(TARGET)-$(VERSION)-src.tar.gz --exclude "CVS" "$(TARGET)-$(VERSION)"
	rm -rf "$(TARGET)-$(VERSION)"

$(APIDIR):
	mkdir $(APIDIR)

$(BUILDDIR):
	mkdir $(BUILDDIR)

$(METADIR): 
	mkdir $(METADIR)

rpm: src-dist
	mkdir -p $(RPM_BASE)/SPECS $(RPM_BASE)/SOURCES $(RPM_BASE)/BUILD \
	      $(RPM_BASE)/SRPMS $(RPM_BASE)/RPMS
	cp etc/libreadline-java.spec $(RPM_BASE)/SPECS
	cp $(TARGET)-$(VERSION)-src.tar.gz $(RPM_BASE)/SOURCES
	rpm --define _topdir$(RPM_BASE) -ba $(RPM_BASE)/SPECS/libreadline-java.spec

test: $(JAR) build-native
	java  -Djava.library.path=. -jar $(JAR) src/test/tinputrc $(ARGS)

clean:
	$(MAKE) -C src/native clean
	-rm -fr `find . -name "*.o" -o -name "*~"` \
		$(JAR) $(TARGET)-*.tar.*z* $(APIDIR) \
		$(BUILDDIR) *.so *.rpm .rltest_history
