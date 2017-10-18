// *****************************************************************************
//  lib.rs -- implementation of the Java wrapper of GNU readline.
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
// *****************************************************************************

use std::ffi::CString;
use std::ffi::CStr;
use std::ptr;

extern crate errno;
use errno::errno;

pub extern crate jni;
use jni::JNIEnv;
use jni::objects::{JClass, JString, JObject};
use jni::sys::JNI_FALSE;
use jni::sys::JNI_TRUE;
use jni::sys::jboolean;
use jni::sys::jint;
use jni::sys::jstring;

pub extern crate libc;
use libc::c_char;
use libc::c_int;
use libc::c_void;

#[cfg(feature = "getline")]
extern "C" {
    static mut rl_readline_name: *const c_char;
}

extern "C" {
    pub fn add_history(line: *const c_char);
    pub fn clear_history();
    pub fn readline(prompt: *const c_char) -> *const c_char;
    pub fn using_history();
}

#[cfg(not(feature = "getline"))]
extern "C" fn java_completer(text: *const c_char, state: c_int) -> *const c_char {
    ptr::null_mut()
}

#[cfg(not(feature = "getline"))]
static mut java_completer_target: Option<*const JObject> = None;

#[cfg(not(feature = "getline"))]
#[repr(C)]
pub struct HIST_ENTRY {
    pub line: *const c_char,
    pub timestamp: *const c_char,
    pub data: *mut c_void,
}

#[cfg(not(feature = "getline"))]
extern "C" {
    pub fn rl_initialize();
    pub fn history_get(offset: c_int) -> *const HIST_ENTRY;
    pub fn write_history(file_name: *const c_char) -> c_int;
    pub fn read_history(file_name: *const c_char) -> c_int;

    static mut rl_completion_entry_function: *const c_void;
}

#[cfg(feature = "readline")]
extern "C" {
    pub fn history_list() -> *const HIST_ENTRY;
    pub fn rl_cleanup_after_signal();
    pub fn rl_free_line_state();
    pub fn rl_parse_and_bind(param: *const c_char) -> c_int;
    pub fn rl_read_init_file(file: *const c_char) -> c_int;

    static mut rl_readline_version: *const c_int;
    static mut rl_gnu_readline_p: *const c_int;
    static mut rl_readline_state: *const c_int;
    static mut rl_editing_mode: *const c_int;
    static mut rl_insert_mode: *const c_int;
    static mut rl_point: *const c_int;
    static mut rl_end: *const c_int;
    static mut rl_mark: *const c_int;
    static mut rl_done: *const c_int;
    static mut rl_pending_input: *const c_int;
    static mut rl_dispatching: *const c_int;
    static mut rl_explicit_arg: *const c_int;
    static mut rl_numeric_arg: *const c_int;
    static mut rl_erase_empty_line: *const c_int;
    static mut rl_already_prompted: *const c_int;
    static mut rl_num_chars_to_read: *const c_int;
    static mut rl_catch_signals: *const c_int;
    static mut rl_catch_sigwinch: *const c_int;
    static mut rl_filename_completion_desired: *const c_int;
    static mut rl_filename_quoting_desired: *const c_int;
    static mut rl_attempted_completion_over: *const c_int;
    static mut rl_completion_type: *const c_int;
    static mut rl_completion_append_character: *const c_int;
    static mut rl_completion_suppress_append: *const c_int;
    static mut rl_completion_query_items: *const c_int;
    static mut rl_completion_mark_symlink_dirs: *const c_int;
    static mut rl_ignore_completion_duplicates: *const c_int;
    static mut rl_inhibit_completion: *const c_int;
    static mut history_base: *const c_int;
    static mut history_length: *const c_int;
    static mut history_max_entries: *const c_int;
    static mut history_quotes_inhibit_expansion: *const c_int;

    static mut rl_library_version: *const c_char; // const
    static mut rl_readline_name: *const c_char; // const
    static mut rl_prompt: *const c_char;
    static mut rl_line_buffer: *const c_char;
    static mut rl_terminal_name: *const c_char; // const
    static mut rl_executing_macro: *const c_char;
    static mut rl_basic_word_break_characters: *const c_char; // const
    static mut rl_completer_word_break_characters: *const c_char; // const
    static mut rl_completer_quote_characters: *const c_char; // const
    static mut rl_basic_quote_characters: *const c_char; // const
    static mut rl_filename_quote_characters: *const c_char; // const
    static mut rl_special_prefixes: *const c_char; // const
    static mut history_word_delimiters: *const c_char;
    static mut history_no_expand_chars: *const c_char;
    static mut history_search_delimiter_chars: *const c_char;
}

#[cfg(feature = "readline")]
macro_rules! global_integer_internals {
    ($x:expr)  => (match $x {
            0  => Ok(&mut rl_readline_version),
            1  => Ok(&mut rl_gnu_readline_p),
            2  => Ok(&mut rl_readline_state),
            3  => Ok(&mut rl_editing_mode),
            4  => Ok(&mut rl_insert_mode),
            5  => Ok(&mut rl_point),
            6  => Ok(&mut rl_end),
            7  => Ok(&mut rl_mark),
            8  => Ok(&mut rl_done),
            9  => Ok(&mut rl_pending_input),
            10 => Ok(&mut rl_dispatching),
            11 => Ok(&mut rl_explicit_arg),
            12 => Ok(&mut rl_numeric_arg),
            13 => Ok(&mut rl_erase_empty_line),
            14 => Ok(&mut rl_already_prompted),
            15 => Ok(&mut rl_num_chars_to_read),
            16 => Ok(&mut rl_catch_signals),
            17 => Ok(&mut rl_catch_sigwinch),
            18 => Ok(&mut rl_filename_completion_desired),
            19 => Ok(&mut rl_filename_quoting_desired),
            20 => Ok(&mut rl_attempted_completion_over),
            21 => Ok(&mut rl_completion_type),
            22 => Ok(&mut rl_completion_append_character),
            23 => Ok(&mut rl_completion_suppress_append),
            24 => Ok(&mut rl_completion_query_items),
            25 => Ok(&mut rl_completion_mark_symlink_dirs),
            26 => Ok(&mut rl_ignore_completion_duplicates),
            27 => Ok(&mut rl_inhibit_completion),
            28 => Ok(&mut history_base),
            29 => Ok(&mut history_length),
            30 => Ok(&mut history_max_entries),
            31 => Ok(&mut history_quotes_inhibit_expansion),
             _ => Err(())
    })
}

#[cfg(feature = "readline")]
macro_rules! global_string_internals {
    ($x:expr)  => (match $x {
            0  => Ok(&mut rl_library_version),
            1  => Ok(&mut rl_readline_name),
            2  => Ok(&mut rl_prompt),
            3  => Ok(&mut rl_line_buffer),
            4  => Ok(&mut rl_terminal_name),
            5  => Ok(&mut rl_executing_macro),
            6  => Ok(&mut rl_basic_word_break_characters),
            7  => Ok(&mut rl_completer_word_break_characters),
            8  => Ok(&mut rl_completer_quote_characters),
            9  => Ok(&mut rl_basic_quote_characters),
            10 => Ok(&mut rl_filename_quote_characters),
            11 => Ok(&mut rl_special_prefixes),
            12 => Ok(&mut history_word_delimiters),
            13 => Ok(&mut history_no_expand_chars),
            14 => Ok(&mut history_search_delimiter_chars),
             _ => Err(())
    })
}

#[cfg(feature = "getline")]
#[link(name = "getline")]
extern "C" {}

#[cfg(feature = "editline")]
#[link(name = "edit")]
extern "C" {}

// -------------------------------------------------------------------------- //
// Initialise readline and history. Set application name.                     //
// -------------------------------------------------------------------------- //
#[allow(non_snake_case)]
#[no_mangle]
pub extern "C" fn Java_org_gnu_readline_Readline_initReadlineImpl(
    env: JNIEnv,
    class: JClass,
    app_name: JString,
) {
    Java_org_gnu_readline_Readline_setVarStringImpl(env, class, 1, app_name);

    if cfg!(feature = "readline") {
        // don't install signal handlers in JNI code.
        let ptr_to_readline_value = unsafe { &mut rl_catch_signals };
        *ptr_to_readline_value = &0;
    }

    if cfg!(not(feature = "getline")) {
        unsafe { rl_initialize() }
    }

    unsafe { using_history() }
}

// -------------------------------------------------------------------------- //
// Reset readline's internal states and terminal.                             //
// -------------------------------------------------------------------------- //
#[allow(non_snake_case)]
#[no_mangle]
#[cfg(not(feature = "getline"))]
pub extern "C" fn Java_org_gnu_readline_Readline_cleanupReadlineImpl(_env: JNIEnv, _class: JClass) {
    if cfg!(feature = "readline") {
        unsafe {
            rl_free_line_state();
            rl_cleanup_after_signal();
        }
    }
}

// -------------------------------------------------------------------------- //
// Report, if we have a terminal                                              //
// -------------------------------------------------------------------------- //
#[allow(non_snake_case)]
#[no_mangle]
#[cfg(not(feature = "getline"))]
pub extern "C" fn Java_org_gnu_readline_Readline_hasTerminalImpl(
    _env: JNIEnv,
    _class: JClass,
) -> jboolean {
    let istty: bool = unsafe { libc::isatty(libc::STDIN_FILENO as i32) } != 0;

    return if istty { JNI_TRUE } else { JNI_FALSE };
}

// -------------------------------------------------------------------------- //
// Add line to history                                                        //
// -------------------------------------------------------------------------- //
#[allow(non_snake_case)]
#[no_mangle]
pub extern "C" fn Java_org_gnu_readline_Readline_addToHistoryImpl(
    env: JNIEnv,
    _class: JClass,
    line: JString,
) {
    if line.is_null() {
        return;
    }

    let r_str: String = env.get_string(line).expect("invalid value string").into();
    let value_to_set = CString::new(r_str).unwrap().into_raw();

    unsafe { add_history(value_to_set) }
}

// -------------------------------------------------------------------------- //
// Prompt for input. "Main" readline function.          .                     //
// -------------------------------------------------------------------------- //
#[allow(non_snake_case)]
#[no_mangle]
pub extern "C" fn Java_org_gnu_readline_Readline_readlineImpl(
    env: JNIEnv,
    _class: JClass,
    java_prompt: JString,
) -> jstring {

    let prompt = if java_prompt.is_null() {
        ptr::null_mut()
    } else {
        let r_str: String = env.get_string(java_prompt)
            .expect("invalid value string")
            .into();
        CString::new(r_str).unwrap().into_raw()
    };

    let ptr_to_line: *const c_char = unsafe { readline(prompt) };

    if ptr_to_line.is_null() {
        env.throw_new("java/io/EOFException", "").ok();

        return ptr::null_mut();
    }

    let value = unsafe { CStr::from_ptr(ptr_to_line).to_str().unwrap() };

    let output = env.new_string(value).expect("Couldn't create java string!");

    output.into_inner()
}

// -------------------------------------------------------------------------- //
// Get current history buffer                                                 //
// -------------------------------------------------------------------------- //
#[allow(non_snake_case)]
#[no_mangle]
#[cfg(not(feature = "getline"))]
pub extern "C" fn Java_org_gnu_readline_Readline_getHistoryImpl(
    _env: JNIEnv,
    _class: JClass,
    _coll: JObject,
) {

}

// -------------------------------------------------------------------------- //
// Clear the current history buffer                                           //
// -------------------------------------------------------------------------- //
#[allow(non_snake_case)]
#[no_mangle]
pub extern "C" fn Java_org_gnu_readline_Readline_clearHistoryImpl(_env: JNIEnv, _class: JClass) {
    unsafe { clear_history() }
}

// -------------------------------------------------------------------------- //
// Get nth entry from history file                                            //
// -------------------------------------------------------------------------- //
#[allow(non_snake_case)]
#[no_mangle]
#[cfg(not(feature = "getline"))]
pub extern "C" fn Java_org_gnu_readline_Readline_getHistoryLineImpl(
    env: JNIEnv,
    _class: JClass,
    i: jint,
) -> jstring {
    let entry = unsafe { history_get(i + 1) };

    if entry.is_null() {
        return ptr::null_mut();
    }

    let var = unsafe { (*entry).line };

    if var.is_null() {
        return ptr::null_mut();
    }

    let value = unsafe { CStr::from_ptr(var).to_str().unwrap() };

    let output = env.new_string(value).expect("Couldn't create java string!");

    output.into_inner()
}

// -------------------------------------------------------------------------- //
// Get the size of the history buffer.                                        //
// -------------------------------------------------------------------------- //
#[allow(non_snake_case)]
#[no_mangle]
#[cfg(not(feature = "getline"))]
pub extern "C" fn Java_org_gnu_readline_Readline_getHistorySizeImpl(
    env: JNIEnv,
    class: JClass,
) -> jint {
    Java_org_gnu_readline_Readline_getVarIntImpl(env, class, 29)
}

// -------------------------------------------------------------------------- //
// Read keybindings from file.                                                //
// -------------------------------------------------------------------------- //
#[allow(non_snake_case)]
#[no_mangle]
#[cfg(feature = "readline")]
pub extern "C" fn Java_org_gnu_readline_Readline_readInitFileImpl(
    env: JNIEnv,
    _class: JClass,
    file_name: JString,
) {
    let init_result: c_int = if file_name.is_null() {
        unsafe { rl_read_init_file(ptr::null()) }
    } else {
        let rust_file_name: String = env.get_string(file_name)
            .expect("Couldn't get java string!")
            .into();

        let c_file_name = CString::new(rust_file_name).unwrap();
        let ptr = c_file_name.into_raw();

        unsafe { rl_read_init_file(ptr) }
    };

    if init_result != 0 {
        let message = unsafe { CStr::from_ptr(libc::strerror(errno().0)).to_str().unwrap() };

        env.throw(("java/io/IOException", message)).ok();
    }
}

// -------------------------------------------------------------------------- //
// Read keybinding from string.                                               //
// -------------------------------------------------------------------------- //
#[allow(non_snake_case)]
#[no_mangle]
#[cfg(feature = "readline")]
pub extern "C" fn Java_org_gnu_readline_Readline_parseAndBindImpl(
    env: JNIEnv,
    _class: JClass,
    param: JString,
) -> jboolean {

    if param.is_null() {
        return JNI_TRUE;
    }

    let rust_param: String = env.get_string(param)
        .expect("Couldn't get java string!")
        .into();

    let c_param = CString::new(rust_param).unwrap();
    let ptr = c_param.into_raw();

    let result = unsafe { rl_parse_and_bind(ptr) };

    if result == 0 {
        return JNI_TRUE;
    }

    JNI_FALSE
}

// -------------------------------------------------------------------------- //
// Read history file                                                          //
// -------------------------------------------------------------------------- //
#[allow(non_snake_case)]
#[no_mangle]
#[cfg(not(feature = "getline"))]
pub extern "C" fn Java_org_gnu_readline_Readline_readHistoryFileImpl(
    env: JNIEnv,
    _class: JClass,
    file_name: JString,
) {

    let result = if file_name.is_null() {
        unsafe { read_history(ptr::null()) }
    } else {
        let rust_file_name: String = env.get_string(file_name)
            .expect("Couldn't get java string!")
            .into();

        let c_file_name = CString::new(rust_file_name).unwrap();
        let ptr = c_file_name.into_raw();

        unsafe { read_history(ptr) }
    };

    if result != 0 {
        let message = unsafe { CStr::from_ptr(libc::strerror(errno().0)).to_str().unwrap() };

        env.throw(("java/io/IOException", message)).ok();
    }
}

// -------------------------------------------------------------------------- //
// Write history file                                                         //
// -------------------------------------------------------------------------- //
#[allow(non_snake_case)]
#[no_mangle]
#[cfg(not(feature = "getline"))]
pub extern "C" fn Java_org_gnu_readline_Readline_writeHistoryFileImpl(
    env: JNIEnv,
    _class: JClass,
    file_name: JString,
) {

    if file_name.is_null() {
        return;
    }

    let rust_file_name: String = env.get_string(file_name)
        .expect("Couldn't get java string!")
        .into();

    let c_file_name = CString::new(rust_file_name).unwrap();
    let ptr = c_file_name.into_raw();

    unsafe { write_history(ptr) };
}

// -------------------------------------------------------------------------- //
// Install completer object                                                   //
// -------------------------------------------------------------------------- //
#[allow(non_snake_case)]
#[no_mangle]
#[cfg(not(feature = "getline"))]
pub extern "C" fn Java_org_gnu_readline_Readline_setCompleterImpl(
    env: JNIEnv,
    _class: JClass,
    object: JObject,
) {

    if object.is_null() {
        unsafe { rl_completion_entry_function = ptr::null_mut() }
    } else {
        // java_completer_target = Some(&object);

        // let class = env.get_object_class(object).expect("Couldn't get java class!");
    }
}

// -------------------------------------------------------------------------- //
// Returns rl_line_buffer                                                     //
// -------------------------------------------------------------------------- //
#[allow(non_snake_case)]
#[no_mangle]
#[cfg(not(feature = "getline"))]
pub extern "C" fn Java_org_gnu_readline_Readline_getLineBufferImpl(
    env: JNIEnv,
    class: JClass,
) -> jstring {
    Java_org_gnu_readline_Readline_getVarStringImpl(env, class, 3)
}

// -------------------------------------------------------------------------- //
// Returns rl_completer_word_break_characters                                 //
// -------------------------------------------------------------------------- //
#[allow(non_snake_case)]
#[no_mangle]
#[cfg(not(feature = "getline"))]
pub extern "C" fn Java_org_gnu_readline_Readline_getWordBreakCharactersImpl(
    env: JNIEnv,
    class: JClass,
) -> jstring {
    Java_org_gnu_readline_Readline_getVarStringImpl(env, class, 7)
}

// -------------------------------------------------------------------------- //
// Sets rl_completer_word_break_characters                                    //
// You should not use this function, since rl_completer_word_break_characters //
// is const char* !!!                                                         //
// -------------------------------------------------------------------------- //
#[allow(non_snake_case)]
#[no_mangle]
#[cfg(not(feature = "getline"))]
pub extern "C" fn Java_org_gnu_readline_Readline_setWordBreakCharactersImpl(
    env: JNIEnv,
    class: JClass,
    word_break_chars: JString,
) {

    Java_org_gnu_readline_Readline_setVarStringImpl(env, class, 7, word_break_chars);
}

// -------------------------------------------------------------------------- //
// Sets an internal integer variable                                          //
// -------------------------------------------------------------------------- //
#[allow(non_snake_case)]
#[no_mangle]
pub extern "C" fn Java_org_gnu_readline_Readline_setVarIntImpl(
    env: JNIEnv,
    _class: JClass,
    index: jint,
    value: jint,
) -> jint {
    let variable_reference = unsafe { global_integer_internals!(index) };

    if variable_reference.is_err() {
        env.throw_new(
            "java/lang/IllegalArgumentException",
            format!("Unknown readline integer index {}", index),
        ).ok();

        return -1;
    }

    let ptr_to_readline_value = variable_reference.unwrap();

    let old_readline_value = if ptr_to_readline_value.is_null() {
        -1
    } else {
        unsafe { **ptr_to_readline_value }
    };

    *ptr_to_readline_value = &value;

    return old_readline_value;
}

// -------------------------------------------------------------------------- //
// Queries an internal integer variable                                       //
// -------------------------------------------------------------------------- //
#[allow(non_snake_case)]
#[no_mangle]
pub extern "C" fn Java_org_gnu_readline_Readline_getVarIntImpl(
    env: JNIEnv,
    _class: JClass,
    index: jint,
) -> jint {
    let variable_reference = unsafe { global_integer_internals!(index) };

    if variable_reference.is_err() {
        env.throw_new(
            "java/lang/IllegalArgumentException",
            format!("Unknown readline integer index {}", index),
        ).ok();

        return -1;
    }

    let ptr_to_readline_value = variable_reference.unwrap();

    if ptr_to_readline_value.is_null() {
        return -1;
    }

    let ptr = *ptr_to_readline_value;

    if ptr.is_null() { -1 } else { ptr as i32 }
}

// -------------------------------------------------------------------------- //
// Sets an internal string variable                                           //
// -------------------------------------------------------------------------- //
#[allow(non_snake_case)]
#[no_mangle]
pub extern "C" fn Java_org_gnu_readline_Readline_setVarStringImpl(
    env: JNIEnv,
    _class: JClass,
    index: jint,
    value: JString,
) -> jstring {
    let variable_reference = unsafe { global_string_internals!(index) };

    if variable_reference.is_err() {
        env.throw_new(
            "java/lang/IllegalArgumentException",
            format!("Unknown readline string index {}", index),
        ).ok();

        return ptr::null_mut();
    }

    let ptr_to_readline_value = variable_reference.unwrap();

    let old_readline_value = if ptr_to_readline_value.is_null() {
        ptr::null_mut()
    } else {
        let old_value = unsafe { CStr::from_ptr(*ptr_to_readline_value).to_str().unwrap() };

        let output = env.new_string(old_value).expect(
            "Couldn't create java string!",
        );

        output.into_inner()
    };

    if value.is_null() {
        *ptr_to_readline_value = ptr::null();
    } else {
        let r_str: String = env.get_string(value).expect("invalid value string").into();

        let value_to_set = CString::new(r_str).unwrap().into_raw();

        *ptr_to_readline_value = value_to_set;
    }

    old_readline_value
}

// -------------------------------------------------------------------------- //
// Queries an internal string variable                                        //
// -------------------------------------------------------------------------- //
#[allow(non_snake_case)]
#[no_mangle]
pub extern "C" fn Java_org_gnu_readline_Readline_getVarStringImpl(
    env: JNIEnv,
    _class: JClass,
    index: jint,
) -> jstring {
    let variable_reference = unsafe { global_string_internals!(index) };

    if variable_reference.is_err() {
        env.throw_new(
            "java/lang/IllegalArgumentException",
            format!("Unknown readline string index {}", index),
        ).ok();

        return ptr::null_mut();
    }

    let ptr_to_readline_value = variable_reference.unwrap();

    if ptr_to_readline_value.is_null() {
        return ptr::null_mut();
    }

    let value = unsafe { CStr::from_ptr(*ptr_to_readline_value).to_str().unwrap() };

    let output = env.new_string(value).expect("Couldn't create java string!");

    output.into_inner()
}
