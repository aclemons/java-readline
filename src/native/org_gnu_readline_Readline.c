/**************************************************************************
 * gnu_readline_Readline.c -- implementation of the Java wrapper
 * of GNU readline.
 *
 * Java Wrapper Copyright (c) 1998-2001 by Bernhard Bablok (mail@bablokb.de)
 *
 * This program is free software; you can redistribute it and or modify
 * it under the terms of the GNU Library General Public License as published
 * by  the Free Software Foundation; either version 2 of the License or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this program; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation Inc., 59 Temple Place - Suite 330,
 * Boston, MA  02111-1307 USA
 **************************************************************************/

/*
 * This file implements the native method interface for class
 * gnu.readline.Readline
 *
 * $Revision$
 * $Author$
 */

#ifdef WIN32
#include <windows.h>
#endif
#include "org_gnu_readline_Readline.h"

#ifdef JavaReadline
#include <readline/readline.h>
#include <readline/history.h>
#endif

#ifdef JavaEditline
#include <editline/readline.h>
#endif

#ifdef JavaGetline
#include "getline.h"
#endif

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
#ifndef WIN32
#include <unistd.h>
#endif

/* -------------------------------------------------------------------------- */
/* Internal status variables of the backing implementation. The functions     */
/* [sg]etVar\(String\|Int\)Impl() use these arrays. Since the functions use   */
/* index-based access, it is important that the variables are in sync with    */
/* the constants in Readline.java.                                            */
/* Also, some of the variables are actually read-only. You should check the   */
/* documentation/source before using setVar                                   */
/*                                                                            */
/* TODO: redirect all variables marked as const in globalStringInternals to   */
/* static buffers, so that free() works in setVarStringImpl                   */
/* -------------------------------------------------------------------------- */

static int undefinedInternalInt = 0;
static char* undefinedInternalString = NULL;
static char undefinedInternalChar = '0';

#ifdef JavaReadline
static int* globalIntegerInternals[] = {
  &rl_readline_version,
  &rl_gnu_readline_p,
  &rl_readline_state,
  &rl_editing_mode,
  &rl_insert_mode,
  &rl_point,
  &rl_end,
  &rl_mark,
  &rl_done,
  &rl_pending_input,
  &rl_dispatching,
  &rl_explicit_arg,
  &rl_numeric_arg,
  &rl_erase_empty_line,
  &rl_already_prompted,
  &rl_num_chars_to_read,
  &rl_catch_signals,
  &rl_catch_sigwinch,
  &rl_filename_completion_desired,
  &rl_filename_quoting_desired,
  &rl_attempted_completion_over,
  &rl_completion_type,
  &rl_completion_append_character,
  &rl_completion_suppress_append,
  &rl_completion_query_items,
  &rl_completion_mark_symlink_dirs,
  &rl_ignore_completion_duplicates,
  &rl_inhibit_completion,

  &history_base,
  &history_length,
  &history_max_entries,
  &history_quotes_inhibit_expansion,        /* index: 31 */
  NULL
};

static char** globalStringInternals[] = {
 /* const */ &rl_library_version,
 /* const */ &rl_readline_name,
 &rl_prompt,
 &rl_line_buffer,
 /* const */ &rl_terminal_name,
 &rl_executing_macro,
 /* const */ &rl_basic_word_break_characters,
 /* const */ &rl_completer_word_break_characters,
 /* const */ &rl_completer_quote_characters,
 /* const */ &rl_basic_quote_characters,
 /* const */ &rl_filename_quote_characters,
 /* const */ &rl_special_prefixes,

 &history_word_delimiters,
 &history_no_expand_chars,
 &history_search_delimiter_chars,        /* index: 14 */
 NULL
};

/* unused: needs [sg]etVarCharImpl */

static char* globalCharInternals[] = {
 &history_expansion_char,
 &history_subst_char,
 &history_comment_char,
 NULL
};
#endif

#ifdef JavaEditline
static int* globalIntegerInternals[] = {
  &undefinedInternalInt, /*  &rl_readline_version, */
  &undefinedInternalInt, /*  &rl_gnu_readline_p, */
  &undefinedInternalInt, /*  &rl_readline_state, */
  &undefinedInternalInt, /*  &rl_editing_mode, */
  &undefinedInternalInt, /*  &rl_insert_mode, */
  &undefinedInternalInt, /*  &rl_point, */
  &undefinedInternalInt, /*  &rl_end, */
  &undefinedInternalInt, /*  &rl_mark, */
  &undefinedInternalInt, /*  &rl_done, */
  &undefinedInternalInt, /*  &rl_pending_input, */
  &undefinedInternalInt, /*  &rl_dispatching, */
  &undefinedInternalInt, /*  &rl_explicit_arg, */
  &undefinedInternalInt, /*  &rl_numeric_arg, */
  &undefinedInternalInt, /*  &rl_erase_empty_line, */
  &undefinedInternalInt, /*  &rl_already_prompted, */
  &undefinedInternalInt, /*  &rl_num_chars_to_read, */
  &undefinedInternalInt, /*  &rl_catch_signals, */
  &undefinedInternalInt, /*  &rl_catch_sigwinch, */
  &undefinedInternalInt, /*  &rl_filename_completion_desired, */
  &undefinedInternalInt, /*  &rl_filename_quoting_desired, */
  &undefinedInternalInt, /*  &rl_attempted_completion_over, */
  &undefinedInternalInt, /*  &rl_completion_type, */
  &undefinedInternalInt, /*  &rl_completion_append_character, */
  &undefinedInternalInt, /*  &rl_completion_suppress_append, */
  &undefinedInternalInt, /*  &rl_completion_query_items, */
  &undefinedInternalInt, /*  &rl_completion_mark_symlink_dirs, */
  &undefinedInternalInt, /*  &rl_ignore_completion_duplicates, */
  &undefinedInternalInt, /*  &rl_inhibit_completion, */

  &undefinedInternalInt, /*  &history_base, */
  &undefinedInternalInt, /*  &history_length, */
  &undefinedInternalInt, /*  &history_max_entries, */
  &undefinedInternalInt, /*  &history_quotes_inhibit_expansion, */
  NULL
};

static char** globalStringInternals[] = {
  &undefinedInternalString, /* const  &rl_library_version, */
  &undefinedInternalString, /* const  &rl_readline_name, */
  &undefinedInternalString, /*  &rl_prompt, */
  &undefinedInternalString, /*  &rl_line_buffer, */
  &undefinedInternalString, /* const  &rl_terminal_name, */
  &undefinedInternalString, /*  &rl_executing_macro, */
  &undefinedInternalString, /* const  &rl_basic_word_break_characters, */
  &undefinedInternalString, /* const  &rl_completer_word_break_characters, */
  &undefinedInternalString, /* const  &rl_completer_quote_characters, */
  &undefinedInternalString, /* const  &rl_basic_quote_characters, */
  &undefinedInternalString, /* const  &rl_filename_quote_characters, */
  &undefinedInternalString, /* const  &rl_special_prefixes, */

  &undefinedInternalString, /*  &history_word_delimiters, */
  &undefinedInternalString, /*  &history_no_expand_chars, */
  &undefinedInternalString, /*  &history_search_delimiter_chars */
};

/* unused: needs [sg]etVarCharImpl */

static char* globalCharInternals[] = {
  &undefinedInternalChar, /*  &history_expansion_char, */
  &undefinedInternalChar, /*  &history_subst_char, */
  &undefinedInternalChar, /*  &history_comment_char */
};
#endif

#ifdef JavaGetline
static int* globalIntegerInternals[] = {
  &undefinedInternalInt, /*  &rl_readline_version, */
  &undefinedInternalInt, /*  &rl_gnu_readline_p, */
  &undefinedInternalInt, /*  &rl_readline_state, */
  &undefinedInternalInt, /*  &rl_editing_mode, */
  &undefinedInternalInt, /*  &rl_insert_mode, */
  &undefinedInternalInt, /*  &rl_point, */
  &undefinedInternalInt, /*  &rl_end, */
  &undefinedInternalInt, /*  &rl_mark, */
  &undefinedInternalInt, /*  &rl_done, */
  &undefinedInternalInt, /*  &rl_pending_input, */
  &undefinedInternalInt, /*  &rl_dispatching, */
  &undefinedInternalInt, /*  &rl_explicit_arg, */
  &undefinedInternalInt, /*  &rl_numeric_arg, */
  &undefinedInternalInt, /*  &rl_erase_empty_line, */
  &undefinedInternalInt, /*  &rl_already_prompted, */
  &undefinedInternalInt, /*  &rl_num_chars_to_read, */
  &undefinedInternalInt, /*  &rl_catch_signals, */
  &undefinedInternalInt, /*  &rl_catch_sigwinch, */
  &undefinedInternalInt, /*  &rl_filename_completion_desired, */
  &undefinedInternalInt, /*  &rl_filename_quoting_desired, */
  &undefinedInternalInt, /*  &rl_attempted_completion_over, */
  &undefinedInternalInt, /*  &rl_completion_type, */
  &undefinedInternalInt, /*  &rl_completion_append_character, */
  &undefinedInternalInt, /*  &rl_completion_suppress_append, */
  &undefinedInternalInt, /*  &rl_completion_query_items, */
  &undefinedInternalInt, /*  &rl_completion_mark_symlink_dirs, */
  &undefinedInternalInt, /*  &rl_ignore_completion_duplicates, */
  &undefinedInternalInt, /*  &rl_inhibit_completion, */

  &undefinedInternalInt, /*  &history_base, */
  &undefinedInternalInt, /*  &history_length, */
  &undefinedInternalInt, /*  &history_max_entries, */
  &undefinedInternalInt, /*  &history_quotes_inhibit_expansion, */
  NULL
};

static char** globalStringInternals[] = {
  &undefinedInternalString, /* const  &rl_library_version, */
  &undefinedInternalString, /* const  &rl_readline_name, */
  &undefinedInternalString, /*  &rl_prompt, */
  &undefinedInternalString, /*  &rl_line_buffer, */
  &undefinedInternalString, /* const  &rl_terminal_name, */
  &undefinedInternalString, /*  &rl_executing_macro, */
  &undefinedInternalString, /* const  &rl_basic_word_break_characters, */
  &undefinedInternalString, /* const  &rl_completer_word_break_characters, */
  &undefinedInternalString, /* const  &rl_completer_quote_characters, */
  &undefinedInternalString, /* const  &rl_basic_quote_characters, */
  &undefinedInternalString, /* const  &rl_filename_quote_characters, */
  &undefinedInternalString, /* const  &rl_special_prefixes, */

  &undefinedInternalString, /*  &history_word_delimiters, */
  &undefinedInternalString, /*  &history_no_expand_chars, */
  &undefinedInternalString, /*  &history_search_delimiter_chars */
};

/* unused: needs [sg]etVarCharImpl */

static char* globalCharInternals[] = {
  &undefinedInternalChar, /*  &history_expansion_char, */
  &undefinedInternalChar, /*  &history_subst_char, */
  &undefinedInternalChar, /*  &history_comment_char */
};
#endif

/* -------------------------------------------------------------------------- */
/* Global buffer. The buffer is allocated when needed and grows in steps of   */
/* 1024. It is never freed, but this is not a problem since in realistic      */
/* environments it should never be larger than a few KB. This strategy will   */
/* prevent constant malloc/free-cycles and keeps the code clean.              */
/*                                                                            */
/* TODO: buffer is changed by utf82ucs and ucs2utf8, but this is not really   */
/*       transparent. Add some additional comments.                           */
/*       Change the variable name buffer to globalBuffer.                     */
/* -------------------------------------------------------------------------- */

static char*  buffer = NULL;
static size_t bufLength = 0;

static char*  word_break_buffer = NULL;

static char* utf2ucs(const char *utf8);
static char* ucs2utf(const char *ucs);
static int   allocBuffer(size_t n);

static jobject   jniObject;
static jmethodID jniMethodId;
static jclass    jniClass;
static JNIEnv*   jniEnv;

/* -------------------------------------------------------------------------- */
/* Initialize readline and history. Set application name.                     */
/* -------------------------------------------------------------------------- */

JNIEXPORT void JNICALL Java_org_gnu_readline_Readline_initReadlineImpl
                              (JNIEnv *env, jclass theClass, jstring jappName) {
   const char *appName;
   jboolean is_copy;

   appName = (*env)->GetStringUTFChars(env,jappName,&is_copy);
   if (appName && strlen(appName))
     rl_readline_name = strdup(appName);
   else
     rl_readline_name = strdup("JAVA");
   if (is_copy == JNI_TRUE)
    (*env)->ReleaseStringUTFChars(env, jappName, appName);
#ifdef JavaReadline
   rl_catch_signals = 0; // don't install signal handlers in JNI code.
#endif
#ifndef JavaGetline
   rl_initialize();
#endif
   using_history();
}

/* -------------------------------------------------------------------------- */
/* Reset readline's internal states and terminal.
/* -------------------------------------------------------------------------- */

#ifndef JavaGetline
JNIEXPORT void JNICALL Java_org_gnu_readline_Readline_cleanupReadlineImpl
                              (JNIEnv *env, jclass theClass) {
#ifdef JavaReadline
    rl_free_line_state();
    rl_cleanup_after_signal();
#endif
}
#endif


/* -------------------------------------------------------------------------- */
/* Report, if we have a terminal
/* -------------------------------------------------------------------------- */

#ifndef JavaGetline
JNIEXPORT jboolean JNICALL Java_org_gnu_readline_Readline_hasTerminalImpl
                              (JNIEnv *env, jclass theClass) {
    return (jboolean) (isatty( STDIN_FILENO ) ? JNI_TRUE : JNI_FALSE);
}
#endif

/* -------------------------------------------------------------------------- */
/* Add line to history                                                        */
/* -------------------------------------------------------------------------- */

JNIEXPORT void JNICALL Java_org_gnu_readline_Readline_addToHistoryImpl
                               (JNIEnv *env, jclass theClass, jstring jline) {

  const char *line;
  jboolean is_copy;

  line = (*env)->GetStringUTFChars(env,jline,&is_copy);
  if (!utf2ucs(line)) {
    jclass newExcCls;
    if (is_copy == JNI_TRUE)
      (*env)->ReleaseStringUTFChars(env, jline, line);
    newExcCls = (*env)->FindClass(env,"java/io/UnsupportedEncodingException");
    if (newExcCls != NULL)
      (*env)->ThrowNew(env,newExcCls,"");
    return;
  }

  if (is_copy == JNI_TRUE)
    (*env)->ReleaseStringUTFChars(env, jline, line);

  add_history(buffer);
  return;
}

/* -------------------------------------------------------------------------- */
/* Prompt for input. "Main" readline function.          .                     */
/* -------------------------------------------------------------------------- */

JNIEXPORT jstring JNICALL Java_org_gnu_readline_Readline_readlineImpl
                               (JNIEnv *env, jclass theClass, jstring jprompt) {

  const char *prompt;
  char *input;
  jboolean is_copy;

  /* retrieve prompt argument and convert to ucs ---------------------------- */

  prompt = (*env)->GetStringUTFChars(env,jprompt,&is_copy);
  if (!utf2ucs(prompt)) {
    jclass newExcCls;
    if (is_copy == JNI_TRUE)
      (*env)->ReleaseStringUTFChars(env, jprompt, prompt);
    newExcCls = (*env)->FindClass(env,"java/io/UnsupportedEncodingException");
    if (newExcCls != NULL)
      (*env)->ThrowNew(env,newExcCls,"");
    return NULL;
  }
  if (is_copy == JNI_TRUE)
    (*env)->ReleaseStringUTFChars(env, jprompt, prompt);

  /* use gnu-readline, convert string to utf8 ------------------------------- */

  input = readline(buffer);
  if (input == NULL) {
    jclass     newExcCls;
    newExcCls = (*env)->FindClass(env,"java/io/EOFException");
    if (newExcCls != NULL)
      (*env)->ThrowNew(env,newExcCls,"");
    return NULL;
  } else if (*input) {
    ucs2utf(input);
    return (*env)->NewStringUTF(env,buffer);
  } else
    return NULL;
}

/* -------------------------------------------------------------------------- */
/* Get current history buffer                                                 */
/* -------------------------------------------------------------------------- */

#ifndef JavaGetline
JNIEXPORT void JNICALL Java_org_gnu_readline_Readline_getHistoryImpl
                                (JNIEnv *env, jclass theClass, jobject jcoll) {
  jclass cls;
  jmethodID mid;
  jstring jline;
#ifdef JavaReadline
  HIST_ENTRY **hist;
#endif
#ifdef JavaEditline
  HIST_ENTRY *histSingle;
  int pos;
#endif

  cls = (*env)->GetObjectClass(env,jcoll);
  mid = (*env)->GetMethodID(env,cls,"add","(Ljava/lang/Object;)Z");

#ifdef JavaReadline
  hist = history_list();
  if (hist != NULL) {
    while (*hist != NULL) {
      jline = (*env)->NewStringUTF(env,(*hist)->line);
      (*env)->CallBooleanMethod(env,jcoll,mid,jline);
      hist++;
    }
  }
#endif

#ifdef JavaEditline
  for (pos = 0; pos < history_length; pos++) {
    histSingle = history_get(pos + 1);
    if (histSingle) {
      ucs2utf(histSingle->line);
      jline = (*env)->NewStringUTF(env,buffer);
      (*env)->CallBooleanMethod(env,jcoll,mid,jline);
    }
  }
#endif
}
#endif

/* -------------------------------------------------------------------------- */
/* Clear the current history buffer                                           */
/* -------------------------------------------------------------------------- */

JNIEXPORT void JNICALL Java_org_gnu_readline_Readline_clearHistoryImpl
                                               (JNIEnv *env, jclass theClass) {
  clear_history();
}

/* -------------------------------------------------------------------------- */
/* Get nth entry from history file                                            */
/* -------------------------------------------------------------------------- */

#ifndef JavaGetline
JNIEXPORT jstring JNICALL Java_org_gnu_readline_Readline_getHistoryLineImpl
                                       (JNIEnv *env, jclass theClass, jint i) {
  HIST_ENTRY *hist = NULL;

  if ((hist = history_get ((int) (i + 1))) != NULL) {
    ucs2utf(hist->line);
    return (*env)->NewStringUTF(env,buffer);
  }
  return NULL;
}
#endif

/* -------------------------------------------------------------------------- */
/* Get the size of the history buffer.                                        */
/* -------------------------------------------------------------------------- */

#ifndef JavaGetline
JNIEXPORT jint JNICALL Java_org_gnu_readline_Readline_getHistorySizeImpl
                                               (JNIEnv *env, jclass theClass) {
  return (jint) history_length;
}
#endif

/* -------------------------------------------------------------------------- */
/* Read keybindings from file.                                                */
/* -------------------------------------------------------------------------- */

#ifdef JavaReadline
JNIEXPORT void JNICALL Java_org_gnu_readline_Readline_readInitFileImpl
                            (JNIEnv *env, jclass theClass, jstring jfilename) {
  const char *filename;
  jboolean is_copy;

  /* retrieve filename argument and convert to ucs -------------------------- */

  filename = (*env)->GetStringUTFChars(env,jfilename,&is_copy);
  if (!utf2ucs(filename)) {
    jclass newExcCls;
    if (is_copy == JNI_TRUE)
      (*env)->ReleaseStringUTFChars(env,jfilename,filename);
    newExcCls = (*env)->FindClass(env,"java/io/UnsupportedEncodingException");
    if (newExcCls != NULL)
      (*env)->ThrowNew(env,newExcCls,"");
    return;
  }
  if (is_copy == JNI_TRUE)
    (*env)->ReleaseStringUTFChars(env,jfilename,filename);

  /* pass to readline function ---------------------------------------------- */

  if (rl_read_init_file(buffer)) {
    jclass newExcCls;
    newExcCls = (*env)->FindClass(env,"java/io/IOException");
    if (newExcCls != NULL)
      (*env)->ThrowNew(env,newExcCls,strerror(errno));
    return;
  }
}
#endif

/* -------------------------------------------------------------------------- */
/* Read keybinding from string.                                               */
/* -------------------------------------------------------------------------- */

#ifdef JavaReadline
JNIEXPORT jboolean JNICALL Java_org_gnu_readline_Readline_parseAndBindImpl
                                (JNIEnv *env, jclass theClass, jstring jline) {
  const char *line;
  jboolean is_copy;

  /* retrieve line argument and convert to ucs -------------------------- */

  line = (*env)->GetStringUTFChars(env,jline,&is_copy);
  if (!utf2ucs(line)) {
    jclass newExcCls;
    if (is_copy == JNI_TRUE)
      (*env)->ReleaseStringUTFChars(env,jline,line);
    newExcCls = (*env)->FindClass(env,"java/io/UnsupportedEncodingException");
    if (newExcCls != NULL)
      (*env)->ThrowNew(env,newExcCls,"");
    return (jboolean) JNI_FALSE;
  }
  if (is_copy == JNI_TRUE)
    (*env)->ReleaseStringUTFChars(env,jline,line);

  /* pass to readline function ---------------------------------------------- */

  if (rl_parse_and_bind(buffer))
    return (jboolean) JNI_FALSE;
  else
    return (jboolean) JNI_TRUE;
}
#endif

/* -------------------------------------------------------------------------- */
/* Read history file                                                          */
/* -------------------------------------------------------------------------- */

#ifndef JavaGetline
JNIEXPORT void JNICALL Java_org_gnu_readline_Readline_readHistoryFileImpl
                            (JNIEnv *env, jclass theClass, jstring jfilename) {
  const char *filename;
  jboolean is_copy;
    
  /* retrieve filename argument and convert to ucs -------------------------- */

  filename = (*env)->GetStringUTFChars(env,jfilename,&is_copy);
  if (!utf2ucs(filename)) {
    jclass newExcCls;
    if (is_copy == JNI_TRUE)
      (*env)->ReleaseStringUTFChars(env,jfilename,filename);
    newExcCls = (*env)->FindClass(env,"java/io/UnsupportedEncodingException");
    if (newExcCls != NULL)
      (*env)->ThrowNew(env,newExcCls,"");
    return;
  }
  if (is_copy == JNI_TRUE)
    (*env)->ReleaseStringUTFChars(env,jfilename,filename);
  
  /* pass to history function ----------------------------------------------- */

  read_history(buffer);
}
#endif

/* -------------------------------------------------------------------------- */
/* Write history file                                                         */
/* -------------------------------------------------------------------------- */

#ifndef JavaGetline
JNIEXPORT void JNICALL Java_org_gnu_readline_Readline_writeHistoryFileImpl
                            (JNIEnv *env, jclass theClass, jstring jfilename) {
  const char *filename;
  jboolean is_copy;
  
  /* retrieve filename argument and convert to ucs -------------------------- */

  filename = (*env)->GetStringUTFChars(env,jfilename,&is_copy);
  if (!utf2ucs(filename)) {
    jclass newExcCls;
    if (is_copy == JNI_TRUE)
      (*env)->ReleaseStringUTFChars(env,jfilename,filename);
    newExcCls = (*env)->FindClass(env,"java/io/UnsupportedEncodingException");
    if (newExcCls != NULL)
      (*env)->ThrowNew(env,newExcCls,"");
    return;
  }
  if (is_copy == JNI_TRUE)
    (*env)->ReleaseStringUTFChars(env,jfilename,filename);
  
  /* pass to history function ----------------------------------------------- */

  write_history(buffer);
}
#endif


/* -------------------------------------------------------------------------- */
/* Completer function, visible to the readline library                        */
/* -------------------------------------------------------------------------- */

#ifndef JavaGetline
const char *java_completer(char *text, int state) {
  jstring jtext;
  jstring completion;
  const char *line;
  jboolean is_copy;
  
  jtext = (*jniEnv)->NewStringUTF(jniEnv,text);

  if (jniMethodId == 0) {
    return;
  }

  completion = (*jniEnv)->CallObjectMethod(jniEnv, jniObject,
					   jniMethodId, jtext, state);
  if (!completion) {
    return ((const char *)NULL);
  }

  line = (*jniEnv)->GetStringUTFChars(jniEnv,completion,&is_copy);
  return line;
}
#endif
  
/* -------------------------------------------------------------------------- */
/* Install completer object                                                   */
/* -------------------------------------------------------------------------- */

#ifndef JavaGetline
JNIEXPORT void JNICALL Java_org_gnu_readline_Readline_setCompleterImpl
                                      (JNIEnv *env, jclass class, jobject obj) {
  
/*   completer_cls = (*env)->GetObjectClass(env, compl); */
/*   completer_cls = (*env)->NewGlobalRef(env, completer_cls); */

/*   completer = (*env)->GetMethodID(env, completer_cls, */
/* 			      "completer", */
/* 			      "(Ljava/lang/String;I)Ljava/lang/String;"); */

/*  (*env)->CallVoidMethod(env, completer_cls, completer, "test", 1); */
  if (obj != NULL) {
    jniEnv    = env;
    jniObject = obj;
    jniClass = (*jniEnv)->GetObjectClass(jniEnv, jniObject);
    jniClass = (*env)->NewGlobalRef(env, jniClass);
    jniObject = (*env)->NewGlobalRef(env, jniObject);
    jniMethodId = (*jniEnv)->GetMethodID(jniEnv, jniClass, "completer",
				"(Ljava/lang/String;I)Ljava/lang/String;");
    if (jniMethodId == 0) {
      rl_completion_entry_function = NULL;
      return;
    }
#ifdef JavaEditline
    rl_completion_entry_function = (CPFunction *) java_completer;
#else
    rl_completion_entry_function = (rl_compentry_func_t *) java_completer;
#endif
  }
  else {
    rl_completion_entry_function = NULL;
  }
}
#endif

/* -------------------------------------------------------------------------- */
/* Returns rl_line_buffer
/* -------------------------------------------------------------------------- */

#ifndef JavaGetline
JNIEXPORT jstring JNICALL 
             Java_org_gnu_readline_Readline_getLineBufferImpl
                                                 (JNIEnv * env, jclass class) {
  jniEnv = env;
  return (*jniEnv)->NewStringUTF(jniEnv, rl_line_buffer);
}
#endif

/* -------------------------------------------------------------------------- */
/* Returns rl_completer_word_break_characters                                 */
/* -------------------------------------------------------------------------- */

#ifndef JavaGetline
JNIEXPORT jstring JNICALL 
             Java_org_gnu_readline_Readline_getWordBreakCharactersImpl
                                                 (JNIEnv * env, jclass class) {
  jstring word_break_characters;

  jniEnv = env;
  if (rl_completer_word_break_characters == 0) {
    word_break_characters =
        (*jniEnv)->NewStringUTF(jniEnv, rl_basic_word_break_characters);
  
  } else {
    word_break_characters =
        (*jniEnv)->NewStringUTF(jniEnv, rl_completer_word_break_characters);
  }

  return word_break_characters;
}
#endif

/* -------------------------------------------------------------------------- */
/* Sets rl_completer_word_break_characters                                    */
/* You should not use this function, since rl_completer_word_break_characters */
/* is const char* !!!                                                         */
/* -------------------------------------------------------------------------- */

#ifndef JavaGetline
JNIEXPORT void JNICALL 
                    Java_org_gnu_readline_Readline_setWordBreakCharactersImpl
                      (JNIEnv * env, jclass class, jstring jword_break_chars) {
  const char * word_break_chars;
  jboolean is_copy;
  
  word_break_chars = (*env)->GetStringUTFChars(env,jword_break_chars,&is_copy);
  if (!utf2ucs(word_break_chars)) {
    jclass newExcCls;
    if (is_copy == JNI_TRUE)
      (*env)->ReleaseStringUTFChars(env,jword_break_chars,word_break_chars);
    newExcCls = (*env)->FindClass(env,"java/io/UnsupportedEncodingException");
    if (newExcCls != NULL)
      (*env)->ThrowNew(env,newExcCls,"");
    return;
  }
  if (is_copy == JNI_TRUE)
    (*env)->ReleaseStringUTFChars(env,jword_break_chars,word_break_chars);
  
  if (word_break_buffer)
    free(word_break_buffer);
  word_break_buffer = strdup(buffer);
  if (!word_break_buffer) {
    jclass newExcCls;
    newExcCls = (*env)->FindClass(env,"java/lang/OutOfMemoryError");
    if (newExcCls != NULL)
      (*env)->ThrowNew(env,newExcCls,"");
    return;    
  }
  rl_completer_word_break_characters = word_break_buffer;
}
#endif

/* -------------------------------------------------------------------------- */
/* Sets an internal integer variable                                          */
/* -------------------------------------------------------------------------- */

JNIEXPORT jint JNICALL 
  Java_org_gnu_readline_Readline_setVarIntImpl(JNIEnv *env, jclass class,
                                                     jint jindex, jint jvalue) {
  int oldValue;
  oldValue = *(globalIntegerInternals[(int) jindex]);
  *(globalIntegerInternals[(int) jindex]) = (int) jvalue;
  return (jint) oldValue;
}

/* -------------------------------------------------------------------------- */
/* Queries an internal integer variable                                       */
/* -------------------------------------------------------------------------- */

JNIEXPORT jint JNICALL 
  Java_org_gnu_readline_Readline_getVarIntImpl(JNIEnv *env, jclass class,
                                                                   jint jindex) {
  return (jint) *(globalIntegerInternals[(int) jindex]);
}

/* -------------------------------------------------------------------------- */
/* Sets an internal string variable                                           */
/* -------------------------------------------------------------------------- */

JNIEXPORT jstring JNICALL 
  Java_org_gnu_readline_Readline_setVarStringImpl(JNIEnv *env, jclass class,
                                                  jint jindex, jstring jvalue) {
  char *oldValue;
  char **value;
  const char *newValue;
  jboolean is_copy;

  /* save old value */

  oldValue = strdup(*(globalStringInternals[(int) jindex]));
  if (!oldValue) {
    jclass newExcCls;
    newExcCls = (*env)->FindClass(env,"java/lang/OutOfMemoryError");
    if (newExcCls != NULL)
      (*env)->ThrowNew(env,newExcCls,"");
    return;    
  }

  /* read new value from argument */

  newValue = (*env)->GetStringUTFChars(env,jvalue,&is_copy);
  if (!utf2ucs(newValue)) {
    jclass newExcCls;
    if (is_copy == JNI_TRUE)
      (*env)->ReleaseStringUTFChars(env,jvalue,newValue);
    newExcCls = (*env)->FindClass(env,"java/io/UnsupportedEncodingException");
    if (newExcCls != NULL)
      (*env)->ThrowNew(env,newExcCls,"");
    return;
  }
  if (is_copy == JNI_TRUE)
    (*env)->ReleaseStringUTFChars(env,jvalue,newValue);
  
  /* set new value */

  value = globalStringInternals[(int) jindex];
  // TODO: currently a memory-leak, but otherwise it crashes
  // free(*value);
  *value = strdup(buffer);
  
  /* return old value */
  
  ucs2utf(oldValue);
  free(oldValue);
  return (*env)->NewStringUTF(env,buffer);
}

/* -------------------------------------------------------------------------- */
/* Queries an internal string variable                                        */
/* -------------------------------------------------------------------------- */

JNIEXPORT jstring JNICALL 
  Java_org_gnu_readline_Readline_getVarStringImpl(JNIEnv *env, jclass class,
                                                                  jint jindex) {
  char *value;
  value = *(globalStringInternals[(int) jindex]);
  if (value) {
    ucs2utf(value);
    return (*env)->NewStringUTF(env,buffer);
  }
  return NULL;
}

/* -------------------------------------------------------------------------- */
/* Convert utf8-string to ucs1-string                   .                     */
/* -------------------------------------------------------------------------- */

char* utf2ucs(const char *utf8) {
  const char *pin;
  char *pout, *ucs;
  unsigned char current, next;
  int i;
  size_t n;

  n = strlen(utf8);
  if (2*n > bufLength) {
    if (allocBuffer(2*n))
      return NULL;
  }

  for (i=0,pin=utf8,pout=buffer; i<bufLength && *pin; i++,pin++,pout++) {
    current = *pin;
    if (current >= 0xE0) {                   /* we support only two-byte utf8 */
      return NULL;
    } else if ((current & 0x80) == 0)        /* one-byte utf8                 */
      *pout = current;
    else {                                   /* two-byte utf8                 */
      next = *(++pin);
      if (next >= 0xC0) {                    /* illegal coding                */
	return NULL;
      }
      *pout = ((current & 3) << 6) +         /* first two bits of first byte  */
	(next & 63);                         /* last six bits of second byte  */
    }
  }
  if (i<bufLength)
    *pout = '\0';
  return buffer;
}

/* -------------------------------------------------------------------------- */
/* Convert ucs1-string to utf8-string                   .                     */
/* -------------------------------------------------------------------------- */

char* ucs2utf(const char *ucs) {
  const char *pin;
  char *pout;
  unsigned char current;
  int i;
  size_t n;

  n = strlen(ucs);
  if (2*n > bufLength) {
    if (allocBuffer(2*n))
      return NULL;
  }

  for (i=0,pin=ucs,pout=buffer; i<bufLength && *pin; i++,pin++,pout++) {
    current = *pin;
    if (current < 0x80)                      /* one-byte utf8                 */
      *pout = current;
    else {                                   /* two-byte utf8                 */
      *pout = 0xC0 + (current>>6);           /* first two bits                */
      pout++, i++;                           /* examine second byte           */
      if (i>=bufLength) {                    /* cannot convert last byte      */
	*(--pout) = '\0';
	return buffer;
      }
      *pout = 0x80 + (current & 63);         /* last six bits                 */
    }
  }
  if (i<bufLength)
    *pout = '\0';
  return buffer;
}

/* -------------------------------------------------------------------------- */
/* Allocate a buffer of at least the given size         .                     */
/* -------------------------------------------------------------------------- */

int allocBuffer(size_t newSize) {
  assert(newSize >= bufLength);
  newSize = (newSize/1024 + 1) * 1024;
  buffer = realloc(buffer,newSize);
  if (buffer == NULL)
    return 1;
  bufLength = newSize;
  return 0;
}

#ifdef WIN32
int WINAPI
readline_init(HANDLE h, DWORD reason, void *foo) {
  return 1;
}
#endif
