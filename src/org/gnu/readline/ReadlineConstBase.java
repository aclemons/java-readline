/**************************************************************************
/* ReadlineConstBase.java -- The base class for constants for Readline
/*                           variables
/*
/* Java Wrapper Copyright (c) 1998-2003 by Bernhard Bablok (mail@bablokb.de)
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

/**
   This class is the base class for constants for Readline variables.
   The constants are used to access internal variables of the
   underlying implementation.

 @version $Revision$
 @author  $Author$
*/

class ReadlineConstBase {
  
  /**
     Numeric number of readline-variable. In the native implementation,
     this number serves as an index into an array of variables. This
     number can only be set once during initialization.
  */

  private int iNumber;

  /**
     Supporting implementations (not every implementation supports all
     variables).
  */

  private ReadlineLibrary[] iSupport;

  /**
     Constructor. The constructor is protected, since the class should not
     be used outside of the package.
  */

  protected ReadlineConstBase(int number, ReadlineLibrary[] support) {
    iNumber = number;
    iSupport = support;
  }

  /**
     Query number.

     @return number of readline-variable.
  */

  protected int getNumber() {
    return iNumber;
  }

  /**
     Check if this readline-variable is supported.
     
     @return true if supported
  */

  protected boolean isSupported(ReadlineLibrary lib) {
    for (int i=0; i < iSupport.length; ++i) {
      if (iSupport[i] == lib)
	return true;
    }
    return false;
  }
}

