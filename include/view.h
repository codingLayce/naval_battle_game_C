#ifndef VIEW_H_GUARD
#define VIEW_H_GUARD

#include <ncurses.h>


void init_view ();
void end_view ();

void create_centered_pop_up (char *content);

#endif
