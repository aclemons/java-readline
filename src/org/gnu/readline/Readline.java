/**************************************************************************
/* Readline.java -- Java wrapper of GNU readline
/*
/* Java Wrapper Copyright (c) 1998 by Bernhard Bablok (bablokb@gmx.de)
/*
/* This program is free software; you can redistribute it and/or modify
/* it under the terms of the GNU Library General Public License as published
/* by  the Free Software Foundation; either version 2 of the License or
/* (at your option) any later version.
/*
/* This program is distributed in the hope that it will be useful, but
/* WITHOUT ANY WARRANTY; without even the implied warranty of
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
/* GNU Library General Public License for more details.
/*
/* You should have received a copy of the GNU Library General Public License
/* along with this program; see the file COPYING.LIB.  If not, write to
/* the Free Software Foundation Inc., 59 Temple Place - Suite 330,
/* Boston, MA  02111-1307 USA
/**************************************************************************/
 

package org.gnu.readline;

import java.io.*;

/**
 This class implements basic functionality of the GNU-readline interface. It
 uses native method calls and might therefore not be portable to other 
 platforms.

 @version $Revision$
 @author  $Author$
*/

public class Readline {

  static {
    System.loadLibrary("JavaReadline");
  }

  /**
     Initialize the GNU-Readline library. This will also set the
     application name, which can be used in the initialization files of
     Readline (usually /etc/inputrc and ~/.inputrc) to define application
     specific keys. See the file ReadlineTest.java int the test subdir of
     the distribution for an example.

     @param applicationName Name of application in initialization file
  */

  public native static void initReadline(String applicationName);

  /**
     Display a prompt on standard output and read a string from
     standard input. This method throws an EOFException on end-of-file (C-d).

     @param prompt Prompt to display
     @return The string the user entered
  */

  public native static String readline(String prompt)
                              throws EOFException, UnsupportedEncodingException;

  /**
     Read keybindings and variable assignments from a file. This method is a
     wrapper to rl_read_init_file(char *filename). Throws IOException if 
     something goes wrong.

     @param filename Name of file to read bindings from
     @return void
  */

  public native static void readInitFile(String filename) throws IOException;

  /**
     Parse argument string as if it had been read from `inputrc' file
     and perform key bindings and variable assignments found.

     @param line Simulated line from inputrc file
     @return boolean False in case of error
  */

  public native static boolean parseAndBind(String line);
}
