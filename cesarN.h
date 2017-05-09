//Author: Cesar Nuno

#ifndef _CESAR_
#define _CESAR_

void cesarN();
void cpause(Game *game);
void render_pause(Game *game);
void menu(Game *game);
void render_menu(Game *game);
void movement_menu(Game *game);
void menu_move(Joystick *joystick, Game *game);
void pause_move(Joystick *joystick);
void toggle_pause(Joystick *joystick);
void levelsel(Game *game);
void render_levelsel(Game *game);
void levelsel_move(Joystick *joystick, Game *game);
void cesar_func();
int cesar_count();

#endif
