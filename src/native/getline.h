#ifndef GETLINE_H
#define GETLINE_H

/* unix systems can #define POSIX to use termios, otherwise 
 * the bsd or sysv interface will be used 
 */

/* readline compatibility stuff */

extern char* rl_readline_name;                  /* unused by getline   */
#define add_history(buffer) gl_histadd(buffer)
#define readline(buffer)    getline(buffer)
#define clear_history()     hist_init()
#define using_history()     hist_init()

#if __STDC__ > 0
#include <stddef.h>

typedef size_t (*gl_strwidth_proc)(char *);

char           *getline(char *);		/* read a line of input */
void            gl_setwidth(int);		/* specify width of screen */
void            gl_histadd(char *);		/* adds entries to hist */
void		gl_strwidth(gl_strwidth_proc);	/* to bind gl_strlen */
void            hist_init();

extern int 	(*gl_in_hook)(char *);
extern int 	(*gl_out_hook)(char *);
extern int	(*gl_tab_hook)(char *, int, int *);

#else	/* not __STDC__ */

char           *getline();	
void            gl_setwidth();
void            gl_histadd();
void		gl_strwidth();

extern int 	(*gl_in_hook)();
extern int 	(*gl_out_hook)();
extern int	(*gl_tab_hook)();

#endif /* __STDC__ */

#endif /* GETLINE_H */
