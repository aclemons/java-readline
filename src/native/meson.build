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

javah = join_paths(java_home, 'bin', 'javah')

readline_header = custom_target(
  'gen-native',
  input : jar_target,
  output : 'org_gnu_readline_Readline.h',
  command : [
    'javah',
    '-d',
    meson.current_source_dir(),
    '-cp',
    join_paths(meson.current_build_dir(), '..', 'libreadline-java.jar'),
    '-jni',
    'org.gnu.readline.Readline'
  ],
  install : false,
)

sources = ['org_gnu_readline_Readline.c', readline_header]

jni_incdirs = [join_paths(java_home, 'include')]

if host_machine.system() == 'linux'
 jni_incdirs += [join_paths(java_home, 'include', 'linux')]
elif host_machine.system() == 'freebsd'
  jni_incdirs += [join_paths(java_home, 'include', 'freebsd')]
endif

jni_incdir = include_directories(jni_incdirs)

if get_option('enable-editline')
  c_compiler = meson.get_compiler('c')
  edit = c_compiler.find_library('edit', required: true)

  shared_library(
    'JavaEditline',
    sources,
    dependencies : [edit],
    include_directories : jni_incdir,
    install : true,
    c_args : ['-DJavaEditline'],
  )
endif

if get_option('enable-getline')
  shared_library(
    'JavaGetline',
    sources + ['getline.c', 'getline.h'],
    include_directories : jni_incdir,
    install : true,
    c_args : ['-DJavaGetline'],
  )
endif

if get_option('enable-readline')
  c_compiler = meson.get_compiler('c')
  readline = c_compiler.find_library('readline', required: true)
  history = c_compiler.find_library('history', required: true)

  shared_library(
    'JavaReadline',
    sources,
    dependencies : [readline, history],
    include_directories : jni_incdir,
    install : true,
    c_args : ['-DJavaReadline'],
  )
endif
