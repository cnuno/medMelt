//Author: Robert Pierucci

#ifndef _ROBERT_
#define _ROBERT_

#ifdef USE_OPENAL_SOUND
void initialize_sounds();
void play_sound(int track, float pitch, bool loop);
void cleanup_sounds();
#endif
void manual_launch();
void deathPhysics();
void deathRender();
void respawn(Player *player);
#endif
