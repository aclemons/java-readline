#!/usr/bin/env bats
#
# BATS test case for java-readline
#
# Copyright (c) 2017 by Andrew Clemons (andrew.clemons@gmail.com)
#
# This program is free software; you can redistribute it and or modify
# it under the terms of the GNU Library General Public License as published
# by  the Free Software Foundation; either version 2 of the License or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Library General Public License for more details.
#
# You should have received a copy of the GNU Library General Public License
# along with this program; see the file COPYING.LIB.  If not, write to
# the Free Software Foundation Inc., 59 Temple Place - Suite 330,
# Boston, MA  02111-1307 USA

unset LC_CTYPE
export LANG=en_US.UTF-8

# not perfect, but good enough for running through the makefile with travis or
# directly from emacs
if [ -f test.bats ] ; then
  # running from test folder
  LIBDIR="$(pwd)/../.."
  LIBJAVAREADLINE="$LIBDIR/libJavaReadline.so"
  LIBJAVAEDITLINE="$LIBDIR/libJavaEditline.so"
else
  # running from root
  LIBDIR="$(pwd)"
  LIBJAVAREADLINE="$LIBDIR/libJavaReadline.so"
  LIBJAVAEDITLINE="$LIBDIR/libJavaEditline.so"
fi

setup() {
  rm -f .bats_rlhistory
}

is_readline() {
  [ -f "$LIBJAVAREADLINE" ]
}

is_editline() {
  [ -f "$LIBJAVAEDITLINE" ]
}

has_iso8859_15() {
  locale -a | grep de_DE@euro > /dev/null 2>&1
}

assert_var() {
  [ "$(LANG="$4" "$JAVA_HOME/bin/java" -Dfile.encoding="$5" -Djava.library.path="$LIBDIR" -cp "$LIBDIR/libreadline-java.jar" test.BatsTestVar "$1" "$2" "$3" | sed -n '1p')" = "$3" ]
}

assert_prompt() {
  [ "$(printf "\n" | LANG="$3" "$JAVA_HOME/bin/java" -Dfile.encoding="$4" -Djava.library.path="$LIBDIR" -cp "$LIBDIR/libreadline-java.jar" test.BatsTestReader "$1" "$2" | sed -n '1p')" = "$2" ]
}

assert_input() {
  sed_arg="2p"

  if [ "$1" = "Editline" ] ; then
    sed_arg="1p" # editline does not echo the prompt without a tty
  fi

  [ "$(printf "%s\n" "$2" | LANG="$3" "$JAVA_HOME/bin/java" -Dfile.encoding="$4" -Djava.library.path="$LIBDIR" -cp "$LIBDIR/libreadline-java.jar" test.BatsTestReader "$1" | sed -n "$sed_arg")" = "$2" ]
}

assert_history() {
  sed_arg="3p"

  if [ "$1" = "Editline" ] ; then
    sed_arg="2p" # editline does not echo the prompt without a tty
  fi

  output="$(printf "%s\n" "$2" | LANG="$3" "$JAVA_HOME/bin/java" -Dfile.encoding="$4" -Djava.library.path="$LIBDIR" -cp "$LIBDIR/libreadline-java.jar" test.BatsTestReader "$1" | sed -n "$sed_arg")"

  [ "$output" = "$2" ]
}

@test "GnuReadline handles simple ascii prompt" {
  if ! is_readline ; then
    skip "java-readline not built with GnuReadline support"
  fi

  assert_prompt "GnuReadline" "bats>" "en_US.UTF-8" "UTF-8"
}

@test "PureJava handles simple ascii prompt" {
  assert_prompt "PureJava" "bats>" "en_US.UTF-8" "UTF-8"
}

@test "GnuReadline handles simple ascii input" {
  if ! is_readline ; then
    skip "java-readline not built with GnuReadline support"
  fi

  assert_input "GnuReadline" "Hello World" "en_US.UTF-8" "UTF-8"
}

@test "Editline handles simple ascii input" {
  if ! is_editline ; then
    skip "java-readline not built with Editline support"
  fi

  assert_input "Editline" "Hello World" "en_US.UTF-8" "UTF-8"
}

@test "PureJava handles simple ascii input" {
  assert_input "PureJava" "Hello World" "en_US.UTF-8" "UTF-8"
}

@test "GnuReadline handles simple ascii history" {
  if ! is_readline ; then
    skip "java-readline not built with GnuReadline support" "UTF-8"
  fi

  assert_history "GnuReadline" "Hello World" "en_US.UTF-8" "UTF-8"
}

@test "Editline handles simple ascii history" {
  if ! is_editline ; then
    skip "java-readline not built with Editline support"
  fi

  assert_history "Editline" "Hello World" "en_US.UTF-8" "UTF-8"
}

@test "GnuReadline handles simple latin1 prompt" {
  if ! is_readline ; then
    skip "java-readline not built with GnuReadline support"
  fi

  if ! has_iso8859_15 ; then
    skip "system does not have a de_DE@euro locale"
  fi

  assert_prompt "GnuReadline" "$(printf "%b" "\xe4tzend>")" "de_DE@euro" "ISO-8859-15"
}

@test "PureJava handles simple latin1 prompt" {
  if ! has_iso8859_15 ; then
    skip "system does not have a de_DE@euro locale"
  fi

  assert_prompt "PureJava" "$(printf "%b" "\xe4tzend>")" "de_DE@euro" "ISO-8859-15"
}

@test "GnuReadline handles simple latin1 input" {
  if ! is_readline ; then
    skip "java-readline not built with GnuReadline support"
  fi

  if ! has_iso8859_15 ; then
    skip "system does not have a de_DE@euro locale"
  fi

  assert_input "GnuReadline" "$(printf "%b" "Hello World\xe4")" "de_DE@euro" "ISO-8859-15"
}

@test "Editline handles simple latin1 input" {
  if ! is_editline ; then
    skip "java-readline not built with Editline support"
  fi

  if ! has_iso8859_15 ; then
    skip "system does not have a de_DE@euro locale"
  fi

  assert_input "Editline" "$(printf "%b" "Hello World\xe4")" "de_DE@euro" "ISO-8859-15"
}

@test "PureJava handles simple latin1 input" {
  if ! has_iso8859_15 ; then
    skip "system does not have a de_DE@euro locale"
  fi

  assert_input "PureJava" "$(printf "%b" "Hello World\xe4")" "de_DE@euro" "ISO-8859-15"
}

@test "GnuReadline handles simple latin1 history" {
  if ! is_readline ; then
    skip "java-readline not built with GnuReadline support"
  fi

  if ! has_iso8859_15 ; then
    skip "system does not have a de_DE@euro locale"
  fi

  assert_history "GnuReadline" "$(printf "%b" "Hello World\xe4")" "de_DE@euro" "ISO-8859-15"
}

@test "Editline handles simple latin1 history" {
  if ! is_editline ; then
    skip "java-readline not built with Editline support"
  fi

  if ! has_iso8859_15 ; then
    skip "system does not have a de_DE@euro locale"
  fi

  skip 'Editline mangles history when not used with a UTF-8 locale'

  assert_history "Editline" "$(printf "%b" "Hello World\xe4")" "de_DE@euro" "ISO-8859-15"
}

@test "GnuReadline handles simple utf8 prompt" {
  if ! is_readline ; then
    skip "java-readline not built with GnuReadline support"
  fi

  assert_prompt "GnuReadline" "$(printf "%b" "\xc3\xa4tzend>")" "en_US.UTF-8" "UTF-8"
}

@test "PureJava handles simple utf8 prompt" {
  assert_prompt "PureJava" "$(printf "%b" "\xc3\xa4tzend>")" "en_US.UTF-8" "UTF-8"
}

@test "GnuReadline handles simple utf8 input" {
  if ! is_readline ; then
    skip "java-readline not built with GnuReadline support"
  fi

  assert_input "GnuReadline" "$(printf "%b" "Hello World\xc3\xa4")" "en_US.UTF-8" "UTF-8"
}

@test "Editline handles simple utf8 input" {
  if ! is_editline ; then
    skip "java-readline not built with Editline support"
  fi

  assert_input "Editline" "$(printf "%b" "Hello World\xc3\xa4")" "en_US.UTF-8" "UTF-8"
}

@test "PureJava handles simple utf8 input" {
  assert_input "PureJava" "$(printf "%b" "Hello World\xc3\xa4")" "en_US.UTF-8" "UTF-8"
}

@test "GnuReadline handles simple utf8 history" {
  if ! is_readline ; then
    skip "java-readline not built with GnuReadline support"
  fi

  assert_history "GnuReadline" "$(printf "%b" "Hello World\xc3\xa4")" "en_US.UTF-8" "UTF-8"
}

@test "Editline handles simple utf8 history" {
  if ! is_editline ; then
    skip "java-readline not built with Editline support"
  fi

  assert_history "Editline" "$(printf "%b" "Hello World\xc3\xa4")" "en_US.UTF-8" "UTF-8"
}

@test "GnuReadline handles utf8 outside BMP prompt" {
  if ! is_readline ; then
    skip "java-readline not built with GnuReadline support"
  fi

  assert_prompt "GnuReadline" "$(printf "%b" "\xf0\x9f\x8c\x8b>")" "en_US.UTF-8" "UTF-8"
}

@test "PureJava handles utf8 outside BMP prompt" {
  assert_prompt "PureJava" "$(printf "%b" "\xf0\x9f\x8c\x8b>")" "en_US.UTF-8" "UTF-8"
}

@test "GnuReadline handles utf8 outside BMP input" {
  if ! is_readline ; then
    skip "java-readline not built with GnuReadline support"
  fi

  assert_input "GnuReadline" "$(printf "%b" "Hello World\xf0\x9f\x8c\x8b")" "en_US.UTF-8" "UTF-8"
}

@test "Editline handles utf8 outside BMP input" {
  if ! is_editline ; then
    skip "java-readline not built with Editline support"
  fi

  assert_input "Editline" "$(printf "%b" "Hello World\xf0\x9f\x8c\x8b")" "en_US.UTF-8" "UTF-8"
}

@test "PureJava handles utf8 outside BMP input" {
  assert_input "PureJava" "$(printf "%b" "Hello World\xf0\x9f\x8c\x8b")" "en_US.UTF-8" "UTF-8"
}

@test "GnuReadline handles utf8 outside BMP history" {
  if ! is_readline ; then
    skip "java-readline not built with GnuReadline support"
  fi

  assert_history "GnuReadline" "$(printf "%b" "Hello World\xf0\x9f\x8c\x8b")" "en_US.UTF-8" "UTF-8"
}

@test "Editline handles utf8 outside BMP history" {
  if ! is_editline ; then
    skip "java-readline not built with Editline support"
  fi

  assert_history "Editline" "$(printf "%b" "Hello World\xf0\x9f\x8c\x8b")" "en_US.UTF-8" "UTF-8"
}

@test "GnuReadline handles ascii var" {
  if ! is_readline ; then
    skip "java-readline not built with GnuReadline support"
  fi

  assert_var "GnuReadline" "RL_LINE_BUFFER" "a" "en_US.UTF-8" "UTF-8"
}

@test "Editline handles ascii var" {
  if ! is_editline ; then
    skip "java-readline not built with Editline support"
  fi

  assert_var "Editline" "RL_LINE_BUFFER" "a" "en_US.UTF-8" "UTF-8"
}

@test "GnuReadline handles utf8 var" {
  if ! is_readline ; then
    skip "java-readline not built with GnuReadline support"
  fi

  assert_var "GnuReadline" "RL_LINE_BUFFER" "$(printf "%b" "Hello World\xc3\xa4")" "en_US.UTF-8" "UTF-8"
}

@test "Editline handles utf8 var" {
  if ! is_editline ; then
    skip "java-readline not built with Editline support"
  fi

  assert_var "Editline" "RL_LINE_BUFFER" "$(printf "%b" "Hello World\xc3\xa4")" "en_US.UTF-8" "UTF-8"
}

@test "GnuReadline handles utf8 outside the bmp var" {
  if ! is_readline ; then
    skip "java-readline not built with GnuReadline support"
  fi

  assert_var "GnuReadline" "RL_LINE_BUFFER" "$(printf "%b" "Hello World\xf0\x9f\x8c\x8b")" "en_US.UTF-8" "UTF-8"
}

@test "Editline handles utf8 outside the bmp var" {
  if ! is_editline ; then
    skip "java-readline not built with Editline support"
  fi

  assert_var "Editline" "RL_LINE_BUFFER" "$(printf "%b" "Hello World\xf0\x9f\x8c\x8b")" "en_US.UTF-8" "UTF-8"
}
