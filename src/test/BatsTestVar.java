/**************************************************************************
/* BatsTestReader.java -- set/get var program for testing Java wrapper of GNU
/* readline with BATS
/* Java Wrapper Copyright (c) 2017 by Andrew Clemons (andrew.clemons@gmail.com)
/*
/* This sample program is placed into the public domain and can be
/* used or modified without any restriction.
/*
/* This program is distributed in the hope that it will be useful, but
/* WITHOUT ANY WARRANTY; without even the implied warranty of
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
/**************************************************************************/

package test;

import java.lang.reflect.Method;

import org.gnu.readline.Readline;
import org.gnu.readline.ReadlineLibrary;

/**
 * BatsTestVar.java
 *
 * Sets a var and then gets the value, printing to stdout.
 *
 * @author Andrew Clemons <andrew.clemons@gmail.com>
 */
public final class BatsTestVar {

  /**
   * Main entry point.
   * @param args
   * @throws Exception
   */
  public static void main(final String[] args) throws Exception {
    final ReadlineLibrary library = ReadlineLibrary.byName(args[0]);

    if (library == null) {
      throw new IllegalArgumentException("Unknown readline library: " + args[0]);
    }

    Readline.load(library);
    Readline.initReadline("BatsTestVar");

    final String variable = args[1];
    final String value = args[2];

    final Object readlineConstant = Readline.class.getField(variable).get(Readline.class);
    final Class readlineConstStringClass = Readline.class.getClassLoader()
           .loadClass("org.gnu.readline.ReadlineConstString");

    final Method setVarMethod = Readline.class.getMethod("setVar", new Class[] { readlineConstStringClass, String.class });
    final Method getVarMethod = Readline.class.getMethod("getVar", new Class[] { readlineConstStringClass });

    setVarMethod.invoke(Readline.class, new Object[] { readlineConstant, value });

    System.out.println(getVarMethod.invoke(Readline.class, new Object[] { readlineConstant }));

    Readline.cleanup();

    System.exit(0);
  }
}
