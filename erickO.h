#ifndef _ERICKO_
#define _ERICKO_

void erick(Game * game);
void erick_movement(Game *game);
void render_erick(Game *game);
void makePill(Game *game, int x, int y);
void epause(Game *game);
void render_pause(Game *game);
void init_terrain(Game *game);
void terrain(Game *game);
void render_terrain(Game *game);
void init_triangle_sky(Game *game);
void triangle_sky(Game *game);
void render_triangle_sky(Game *game);
int noise(int x);
bool collision_detect(Game *game, int x);
void toggle_move(Player *player, Direction dir);
void disco(Game *game);

#endif
