%define version 0.7.1

Summary: java wrapper for the GNU-readline library
Copyright: GNU Lesser General Public License
Group: Development/Java
Name: libreadline-java
Prefix: /usr
Provides: java_readline
Packager: Henner Zeller <H.Zeller@acm.org>
Release: 1
Source: java_readline-%{version}-src.tar.gz
URL: http://www.bablokb.de/java/readline.html
Version: %{version}
Buildroot: /tmp/javareadline-build
BuildRequires: readline-devel

%description
This is a Java-wrapper for the GNU-Readline using the 
Java Native Interface (JNI). It allows to write console Java applications
that provide commandline editing functions like history and TAB-completion.

%prep
%setup -q -n java_readline-%{version}

%build
make T_LIBS="JavaReadline" world apidoc

%install
rm -fr $RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT/%{prefix}/lib
mkdir -p $RPM_BUILD_ROOT/%{prefix}/share/java
cp java_readline.jar $RPM_BUILD_ROOT/%{prefix}/share/java/%{name}.jar
cp libJavaReadline.so $RPM_BUILD_ROOT/%{prefix}/lib

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,-,root)
%{prefix}/lib/libJavaReadline.so
%{prefix}/share/java/%{name}.jar
%doc ChangeLog README README.1st VERSION api
