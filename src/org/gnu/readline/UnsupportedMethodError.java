/**************************************************************************
/* UnsupportedMethodError.java -- Error class for cases in which methods
/* are not supported by native implementations
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

/**
 Error class for cases in which methods are not supported by native
 implementations.
/*

 @version $Revision$
 @author  $Author$
*/

public class UnsupportedMethodError extends Error {

  /////////////////////////////////////////////////////////////////////////////

  /**
     Default constructor.
  */

  public UnsupportedMethodError() {
    super();
  }

  /////////////////////////////////////////////////////////////////////////////

  /**
     Constructor with message argument.

     @param msg Detail about error.
  */

  public UnsupportedMethodError(String msg) {
    super(msg);
  }
}
