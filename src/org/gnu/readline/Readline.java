/**************************************************************************
/* Readline.java -- Java wrapper of GNU readline
/*
/* Java Wrapper Copyright (c) 1998-2001 by Bernhard Bablok (mail@bablokb.de)
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
 uses native method calls if available, otherwise it defaults to normal
 I/O using a BufferedReader. 

 <p>A typical implementation could look like:
<pre>
try {
  Readline.load(ReadlineLibrary.GnuReadline);
} catch (Exception e) {
}
Readline.initReadline("myapp");

while (true) {
  try {
    line = Readline.readline("myprompt> ");
    if (line == null)
       System.out.println("no input");
    else
       processLine();
  } catch (EOFException e) {
    break;
  } catch (Exception e) {
    doSomething();
  }
}
</pre>

 <p>Note that the fallback solution does not throw an EOFException, you
 therefore need some other means of deciding when to stop.

 @version $Revision$
 @author  $Author$
*/

public class Readline {
  
  /**
     The currently defined ReadlineCompleter.
  */

  private static ReadlineCompleter iCompleter = null;


  /**
     The currently implementing backing library.
  */

  private static ReadlineLibrary iLib = ReadlineLibrary.PureJava;

  /**
     The BufferedReader for the fallback solution.
  */

  private static BufferedReader iReader = null;

  /**
     Configuration flag: throw an UnsupportedOperationException, if true.
     This value defaults to false.
  */

  private static boolean iThrowException = false;

  /////////////////////////////////////////////////////////////////////////////

  /**
     Load an implementing backing library. This method might throw an
     UnsatisfiedLinkError in case the native libary is not found in the
     library path. If you want to have portable program, just catch and 
     ignore that error. JavaReadline will then just use the pure Java fallback
     solution.

     @param lib An object (constant) of type ReadlineLibrary
     @see org.gnu.readline.ReadlineLibrary
  */

  public static final void load(ReadlineLibrary lib) {
    if (lib == iLib)     // ok, since objects are immutable
      return;
    if (lib == ReadlineLibrary.PureJava) {
      iLib = lib;
      return;
    }
    System.loadLibrary(lib.getName()); // might throw UnsatisfiedLinkError
    iLib = lib;
  }

  /////////////////////////////////////////////////////////////////////////////

  /**
     Initialize the GNU-Readline library. This will also set the
     application name, which can be used in the initialization files of
     Readline (usually /etc/inputrc and ~/.inputrc) to define application
     specific keys. See the file ReadlineTest.java int the test subdir of
     the distribution for an example.

     @param applicationName Name of application in initialization file
  */

  public static void initReadline(String applicationName) {
    if (iLib != ReadlineLibrary.PureJava)
      initReadlineImpl(applicationName);
  }

  /////////////////////////////////////////////////////////////////////////////

  /**
     Display a prompt on standard output and read a string from
     standard input. This method returns 'null', if there has
     been an empty input (user pressed just [RETURN]) and throws
     an EOFException on end-of-file (C-d).

     @param prompt Prompt to display
     @return The string the user entered or 'null' if there was no input.
     @throws EOFException on end-of-file, i.e. CTRL-d input.
  */

  public static String readline(String prompt) throws EOFException, 
                                    IOException, UnsupportedEncodingException {
    if (iLib != ReadlineLibrary.PureJava)
      return readlineImpl(prompt);
    else {
      System.out.print(prompt);
      if (iReader == null)
        iReader = new BufferedReader(new InputStreamReader(System.in));
      String line = iReader.readLine();
      if (line == null)
        throw new EOFException("EOF");
      return (line.length() == 0) ? null : line;
    }
  }

  /////////////////////////////////////////////////////////////////////////////

  /**
     Read keybindings and variable assignments from a file. This method is a
     wrapper to rl_read_init_file(char *filename). Throws IOException if 
     something goes wrong.

     @param filename Name of file to read bindings from
     @return void
  */

  public static void readInitFile(String filename) throws IOException {
    if (iLib == ReadlineLibrary.GnuReadline)
      readInitFileImpl(filename);
    else if (iThrowException)
      throw new UnsupportedOperationException();
  }

  /////////////////////////////////////////////////////////////////////////////

  /**
     Parse argument string as if it had been read from `inputrc' file
     and perform key bindings and variable assignments found.

     @param line Simulated line from inputrc file
     @return boolean False in case of error
  */

  public static boolean parseAndBind(String line) {
    if (iLib == ReadlineLibrary.GnuReadline)
      return parseAndBindImpl(line);
    else if (iThrowException)
      throw new UnsupportedOperationException();
    else
      return true;
  }

  /////////////////////////////////////////////////////////////////////////////

  /**
     Reads a history file into memory
       @param filename Name of history file to read
     */

  public static void readHistoryFile(String filename)
                              throws EOFException, UnsupportedEncodingException {
    if (iLib != ReadlineLibrary.PureJava)
      readHistoryFileImpl(filename);
    else if (iThrowException)
      throw new UnsupportedOperationException();
  }


  /////////////////////////////////////////////////////////////////////////////

  /**
     Writes a history file to disc

     @param filename Name of history file to write
  */

  public static void writeHistoryFile(String filename)
                              throws EOFException, UnsupportedEncodingException {
    if (iLib != ReadlineLibrary.PureJava)
      writeHistoryFileImpl(filename);
    else if (iThrowException)
      throw new UnsupportedOperationException();
  }


  /////////////////////////////////////////////////////////////////////////////

  /**
     Set your completer function.

     @param rlc An object implementing the ReadlineCompleter interface
  */
    
  public static void setCompleter(ReadlineCompleter rlc) {
    if (iLib != ReadlineLibrary.PureJava) {
      iCompleter = rlc;
      setCompleterImpl(rlc);
    } else if (iThrowException)
      throw new UnsupportedOperationException();
  }

  /////////////////////////////////////////////////////////////////////////////

  /**
     Query current completer function.

     @return Current ReadlineCompleter object
  */
    
  public static ReadlineCompleter getCompleter() {
    return iCompleter;
  }

  /////////////////////////////////////////////////////////////////////////////

  /**
     Query word break characters.
  */
    
  public static String getWordBreakCharacters() {
    if (iLib != ReadlineLibrary.PureJava)
      return getWordBreakCharactersImpl();
    else if (iThrowException)
      throw new UnsupportedOperationException();
    else
      return null;
  }

  /////////////////////////////////////////////////////////////////////////////

  /**
     Set word break characters.

     @param wordBreakCharacters A string of word break characters
  */
    
  public static void 
    setWordBreakCharacters(String wordBreakCharacters)
                              throws UnsupportedEncodingException {
    if (iLib != ReadlineLibrary.PureJava)
      setWordBreakCharactersImpl(wordBreakCharacters);
    else if (iThrowException)
      throw new UnsupportedOperationException();
  }

  /////////////////////////////////////////////////////////////////////////////

  /**
     Configure behavior in case an unsupported method is called. If argument
     is true, unsupported methods throw an UnsupportedOperationException.

     @param flag configuration flag
  */

  public static void setThrowExceptionOnUnsupportedMethod(boolean flag) {
    iThrowException = flag;
  }

  /////////////////////////////////////////////////////////////////////////////

  /**
     Query behavior in case an unsupported method is called.

     @return configuration flag
  */

  public static boolean getThrowExceptionOnUnsupportedMethod() {
    return iThrowException;
  }

  /////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////

  /**
     Native implementation of initReadline()

     @see org.gnu.readline.Readline#initReadline(String applicationName)
  */

  private native static void initReadlineImpl(String applicationName);

  /////////////////////////////////////////////////////////////////////////////

  /**
     Native implementation of readline()

     @see org.gnu.readline.Readline#readline(String prompt)
  */

  private native static String readlineImpl(String prompt)
                              throws EOFException, UnsupportedEncodingException;

  /////////////////////////////////////////////////////////////////////////////

  /**
     Native implementation of readInitFile(String filename)

     @see org.gnu.readline.Readline#readInitFile(String filename)
  */

  private native static void readInitFileImpl(String filename)
                                                            throws IOException;

  /////////////////////////////////////////////////////////////////////////////

  /**
     Native implementation of parseAndBind(String line)

     @see org.gnu.readline.Readline#parseAndBind(String line)
  */

  private native static boolean parseAndBindImpl(String line);

  /////////////////////////////////////////////////////////////////////////////

  /**
     Native implementation of readHistoryFile(String filename)

     @see org.gnu.readline.Readline#readHistoryFile(String filename)
  */

  private native static void readHistoryFileImpl(String filename)
                              throws EOFException, UnsupportedEncodingException;

  /////////////////////////////////////////////////////////////////////////////

  /**
     Native implementation of writeHistoryFile(String filename)

     @see org.gnu.readline.Readline#writeHistoryFile(String filename)
  */

  private native static void writeHistoryFileImpl(String filename)
                              throws EOFException, UnsupportedEncodingException;

  /////////////////////////////////////////////////////////////////////////////

  /**
     Native implementation of setCompleter(ReadlineCompleter rlc)

     @see org.gnu.readline.Readline#setCompleter(ReadlineCompleter rlc)
  */
    
  private native static void setCompleterImpl(ReadlineCompleter rlc);
    
  /////////////////////////////////////////////////////////////////////////////

  /**
     Native implementation of getWordBreakCharacters()

     @see org.gnu.readline.Readline#getWordBreakCharacters()
  */

  private native static String getWordBreakCharactersImpl();

  /////////////////////////////////////////////////////////////////////////////

  /**
     Native implementation of setWordBreakCharacters()

     @see 
   org.gnu.readline.Readline#setWordBreakCharacters(String wordBreakCharacters)
  */

  private native static void 
    setWordBreakCharactersImpl(String wordBreakCharacters)
                              throws UnsupportedEncodingException;
}
