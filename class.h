#include <iostream>
#include "joystick.h"

#ifndef _STRUCT_

#define _STRUCT_
#define MAX_PLAT 4
#define MAX_PLAYER 4
#define GRAVITY .1
#define rnd() (float)rand() / (float)RAND_MAX
#define MAX_PILLS 1000
#define MAX_PARTICLE 50
#define MAX_STAR 50

enum State {MAINMENU, PAUSE, LEVELSEL, ERICK, ROBERT, ZACK, FIELD, STARYNIGHT, BAKERSFIELD};
enum PlayerState {ALIVE, DEAD};
enum Collision {ENEMY, PLATFORM, PROJECTILE};
enum PlayerAction {EMPTY, PASSIVE, GROUNDPOUND, DASH, ATTACK, DEFEND, JUMP, MOVE};
enum Direction {LEFT, UP, RIGHT, DOWN};
enum TimeOfDay {MORNING, NOON, NIGHT};




class Vec {
	public:
		float x, y, z;
};

class Shape {
	public:
		float width, height;
		float radius;
		Vec center;
		Collision collision;
};

class Stat {
	public:
		bool initDeath;
		int health;
		int lifeCount;
		PlayerState lifeState;
};

class Gpad {
	public:
		char location[MAX_PLAYER][15];
		Joystick *joystick[MAX_PLAYER];
		Gpad();
};

class Color {
	public:
		int pink[3];
		int blue[3];
		int green[3];
		int red[3];
		int bloodred[3];
		int darkblue[3];
		int darkgreen[3];
		int hotpink[3];
		Color();
};

class Particle {
	public:
		Shape s;
		Vec velocity;
		int color[3];
};

class Level;
class Player {
	public:
		Stat status;
		Joystick *joystick = NULL;
		Shape body;
		int color[3];
		Vec delta;
		bool onGround;
		float dashStrength;
		PlayerAction action;
		Direction direction;
		int currentContact;
		int jumpCount;
		int JUMP_MAX;
		void collision(Shape platform[]);
		void check_controller(Player *player, Joystick *joystick);
		void render();
		struct timespec timeStart, timeCurrent;
		double timeSpan;
		Particle bloodStream[MAX_PARTICLE];
		double timeDiff(struct timespec *start, struct timespec *end);
		Player();
};

//inheritance test
class Kerby:public Player {
	public:
		Shape outline;
		Shape body;
		Shape shade;
		Shape eye;
		int color[3];
		float dashStrength;
		void render();
};

class StatDisplay {
	public:
		Shape quadrant[4];
		void render();
		StatDisplay();
};

class Pill {
	public:
		Vec center; 
		Vec velocity; 
		Shape body[2];
		Shape end[2];
		Pill();
};

class Menu {
	public:
		Shape titleBox;
		Shape pillBox;
		Shape playBox;
		Shape optionBox;
		Shape exitBox;
		Shape selector;
		Vec titleVel;
};

class pMenu {
	public:
		Shape rbox;
		Shape obox;
		Shape qbox;
		Shape selector;
};

class lMenu {
	public:
		Shape level1;
		Shape level2;
		Shape selector;
		Shape textBox;
};

class Test {
	public:
		int pill_count;
		Pill pill[MAX_PILLS];
		Player player1;
		Shape floor;  
		int xScale;
		int yScale;
};


class Level {
	public:
		Player player[MAX_PLAYER];
		Shape platform[MAX_PLAT];
		Gpad controller;
		StatDisplay statDisplay;

		//To handle shading
		int widthPartition;
		int heightPartition;
		void physics(Player *player);
		void deathCheck(Player *player);
		void respawn(Player *player);
		Level();
};

class Starynight_Level:public Level {
	public:
		float starP[MAX_STAR];
		void erick_init();	
		void render();
		Starynight_Level();
		Shape moon; 	
		Shape etBox;
		void physics (Player *player) 
		{
			Level::physics(player);
		}
		void deathCheck (Player *player) 
		{
			Level::deathCheck(player);
		}
		void respawn(Player *player)
		{
			Level::respawn(player);
		}
};

class Field_Level: public Level {
	public:
		void render();
		void erick_init();
		Field_Level();
		void physics(Player *player) 
		{
			Level::physics(player);
		}
		void deathCheck (Player *player) 
		{
			Level::deathCheck(player);
		}
};

class Bakersfield_Level: public Level {
	public:
		TimeOfDay tod;
		Shape sun;
		Shape moon;
		Shape gas;
		Shape oil;
		void render();
		Bakersfield_Level();
		void physics(Player *player) 
		{
			Level::physics(player);
		}
		void deathCheck (Player *player) 
		{
			Level::deathCheck(player);
		}
};

class Game {
	public:
		int WINDOW_WIDTH, WINDOW_HEIGHT;
		float WINDOW_RATIO;
		int mouse[2];
		Menu mainMenu;
		pMenu pauseMenu;
		lMenu levelMenu;
		State render;

		//ericks environment
		Test level1;

		Field_Level level2;
		Starynight_Level level3;	
		Bakersfield_Level level4;
		State prevState;
		Game();
		~Game();
};

#endif
