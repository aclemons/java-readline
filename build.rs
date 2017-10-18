// *****************************************************************************
//  build.rs -- implementation of the Java wrapper of GNU readline.
//
//  Java Wrapper Copyright (c) 2017 by Andrew Clemons (andrew.clemons@gmail.com)
//
//  This program is free software; you can redistribute it and or modify
//  it under the terms of the GNU Library General Public License as published
//  by  the Free Software Foundation; either version 2 of the License or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful, but
//  WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Library General Public License for more details.
//
//  You should have received a copy of the GNU Library General Public License
//  along with this program; see the file COPYING.LIB.  If not, write to
//  the Free Software Foundation Inc., 51 Franklin Street, Fifth Floor,
//  Boston, MA  02110-1301 USA
//******************************************************************************

fn main() {
    if cfg!(getline) {
        println!("cargo:rustc-flags=-l termcap");
    } else if cfg!(editline) {
        println!("cargo:rustc-flags=-l edit -l termcap");
    } else {
        println!("cargo:rustc-flags=-l readline -l termcap -l history");
    }
}
