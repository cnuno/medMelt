//Author: Robert Pierucci

#ifndef _ROBERT_
#define _ROBERT_

void loadSound(const char path[20]);
void playSound(float pitch, bool loop);
void endSound();
void closeSoundDevice();
//void deathAnimation(int x, int y);
void deathPhysics();
void deathRender();
void respawn(Player *player);
#endif
