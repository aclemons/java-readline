/**************************************************************************
/* BatsTestReader.java -- Echo program for testing Java wrapper of GNU
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

import java.io.File;
import java.io.IOException;

import org.gnu.readline.Readline;
import org.gnu.readline.ReadlineLibrary;

/**
 * BatsTestReader.java
 *
 * Prompts for input, then echos back what is reads.
 *
 * @author Andrew Clemons <andrew.clemons@gmail.com>
 */
public final class BatsTestReader {

  /**
   * Main entry point.
   * @param args
   * @throws IOException
   */
  public static void main(final String[] args) throws IOException {
    final ReadlineLibrary library = ReadlineLibrary.byName(args[0]);

    if (library == null) {
      throw new IllegalArgumentException("Unknown readline library: " + args[0]);
    }

    Readline.load(library);
    Readline.initReadline("BatsTestReader");

    final String prompt;
    if (args.length > 1) {
      prompt = args[1];
    } else {
      prompt = "bats> ";
    }

    final String line = Readline.readline(prompt);

    System.out.println(line);

    if (library == ReadlineLibrary.GnuReadline || library == ReadlineLibrary.Editline) {
      final File history = new File(".bats_rlhistory");
      Readline.writeHistoryFile(history.getName());

      System.out.println(Readline.getHistoryLine(0));
    }

    Readline.cleanup();

    System.exit(0);
  }
}
