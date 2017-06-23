#**************************************************************************
#* Java Wrapper Copyright (c) 2017 by Andrew Clemons (andrew.clemons@gmail.com)
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

java_home = get_option('JAVA_HOME')

if java_home == ''
  error('JAVA_HOME not set')
endif

javac = join_paths(java_home, 'bin', 'javac')

java_sources = [
  'org/gnu/readline/Readline.java',
  'org/gnu/readline/ReadlineCompleter.java',
  'org/gnu/readline/ReadlineConstBase.java',
  'org/gnu/readline/ReadlineConstInt.java',
  'org/gnu/readline/ReadlineConstString.java',
  'org/gnu/readline/ReadlineLibrary.java',
  'org/gnu/readline/ReadlineReader.java',
  'test/BatsTestReader.java',
  'test/BatsTestVar.java',
  'test/ReadlineTest.java',
  'test/TestCompleter.java',
]

classes_target = custom_target(
  'classes',
  input : java_sources,
  output : 'classes.touched',
  command : [
    javac,
    '-target',
    '1.5',
    '-source',
    '1.5',
    '-d',
    '@OUTDIR@',
    '@INPUT@'
  ],
  install : false,
  capture : true,
)

jar_binary = join_paths(java_home, 'bin', 'jar')

jar_target = custom_target(
  'jar',
  input : classes_target,
  output : 'libreadline-java.jar',
  command : [
    jar_binary,
    'cvfm',
    '@OUTPUT@',
    join_paths(meson.current_build_dir(), '..', '..', 'etc', 'manifest.stub'),
    '-C',
    meson.current_build_dir(),
    '.'
  ],
  install : true,
  install_dir : 'share/java',
)

javadoc_binary = join_paths(java_home, 'bin', 'javadoc')

readline_library_name = 'The Java-Readline Library'
readline_url = 'http://github.com/aclemons/java-readline'

jar_target = custom_target(
  'docs',
  input : jar_target,
  output : 'docs.touched',
  command : [
    javadoc_binary,
    '-sourcepath',
    meson.current_source_dir(),
    '-d',
    '@OUTDIR@',
    '-windowtitle',
    readline_library_name,
    '-doctitle',
    '@0@, Version @1@'.format(readline_library_name, meson.project_version()),
    '-footer',
    '<strong>@0@, Version @1@</version>'.format(readline_library_name, meson.project_version()),
    '-header',
    '<strong>@0@, Version @1@</version>'.format(readline_library_name, meson.project_version()),
    '-bottom',
    'Released under the LGPL, (c) Bernhard Bablok, Henner Zeller 1998-2002, Andrew Clemons 2017<br>Homepage: <a href="@0@">@1@</a>'.format(readline_url, readline_url),
    '-version',
    '-author',
    'org.gnu.readline',
    '-Xdoclint:none',
    'test'
  ],
  install : false,
)

subdir('native')
