%define version 0.7.3

Summary: java wrapper for the GNU-readline library
Copyright: GNU Lesser General Public License
Group: Development/Java
Name: libreadline-java
Prefix: /usr
Provides: java_readline
Packager: Henner Zeller <H.Zeller@acm.org>
Release: 1
Source: %{name}-%{version}-src.tar.gz
URL: http://www.bablokb.de/java/readline.html
Version: %{version}
Buildroot: /tmp/javareadline-build
BuildRequires: readline-devel

%description
This is a Java-wrapper for the GNU-Readline using the 
Java Native Interface (JNI). It allows to write console Java applications
that provide commandline editing functions like history and TAB-completion.

%prep
%setup -q

%build

%install
rm -fr $RPM_BUILD_ROOT
make DESTDIR=$RPM_BUILD_ROOT T_LIBS="JavaReadline" install

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)
%{prefix}/lib/libJavaReadline.so
%{prefix}/share/java/%{name}.jar
%doc NEWS ChangeLog README README.1st VERSION api
