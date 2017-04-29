//Author: Erick Ortiz

/*
   Progress Log
   04/18/2017 - 04/21/2017
   I developed a new "DISCO" level with dynamic features. I encountered a lot
   of issues in establishing the coordinates for the triangle lined sky. The
   sky fades in and fades out. Once it fades out, the background randomizes.
   The mathematics had me boggled for days. So, I toubleshooted this issue
   and rendered the coordinates onto the screen without the lines. This 
   helped me solve the issues I encountered. 

   04/17/2017
   Worked with robert to figure out timing for respawns. 

   April 16th, 2017 - part 2
   I am attempting to create inheritance from the player class. In this
   attempt, I realized that this may alter the way physics is conducted for
   different players (once they are defined. That means that I may need to
   make exclusive physics for each player type. 

   April 16th, 2017 - part 1
   I added a class for predefined character colors. This will enable the level
   to assign a predefined color to each round character. I made the lines of 
   my source code < 80 characters long.	

   April 14th, 2017
   After 4 hours of work I fixed the issue mentions on 04/11. Physics are now 
   working with all platforms in our game! Now I need to develop player 
   collision. This should be relatively similar to handling platform 
   collision. Now, we need	to fix the movement of the player. In order to 
   move, the player needs to constantly press left or right. I need to develop 
   a toggle variable for left and right movement (in the air and the ground).

   April 11th, 2017	
   Big issue that may be distorting my understanding of the physics. When the 
   player is on a platform, or on the ground, player->onGround = false. I may 
   need to	redefine Player::collision() function. 

   April 9th, 2017
   I spent 2 hours making a library of numbers for Robert to utilize as a 
   meausure of health. I made the numbers 0-9 in both red and white. I tried 
   to reduce the size of the entire library. Each .png file is approximately 
   150x150 pixels.

   April 8th, 2017
   I spent 4 hours attempting to redefine the physics for our game. I did make
   some progress. However, it is not yet ready for implementation.

   March 19th, 2017
   I spent 4 hours researching various methods of obtaining input from an Xbox
   360 controller and a PS3 I was not able to successfully implement this
   feature. I began by installing the drivers for the input device on my
   machine. This would	ensure that the device is communicating with the
   computer. From there, I	expiremented with the button features in XLIB. The
   signal was not intercepted through my implementation.

   March 17th, 2017
   I spent 8 hours diagramming a potential camera system for Medical Meltdown.
   This system aimed to magnify the screen and upscale the resolution. The 
   center	point of the camera was to be centered between the furthest left,
   furthest right, highest player, and lowest player.
 */

#include "headers.h"

using namespace std;

#define MAX_PILLS 1000
#define x_rnd() (rand() % 11) - 5 
#define y_rnd() (rand() % 11)  
#define color_rnd() (rand() % 6) 
#define coor() ((float)rand() / (float)RAND_MAX) - 0.5
#define rnd() (float)rand() / (float)RAND_MAX
#define change() (rand() % 3) + 1

extern Screen* scrn;
extern void resetMain(Game *game);
extern Game game;

bool bTemp = true;
void Player::check_controller(Player *player, Joystick *joystick)
{
	bool conditionA, conditionB, conditionC, conditionD;
	JoystickEvent event;

	if (!joystick->isFound() && bTemp)
	{
		printf("Attention: Game Pad is not connected\n");
		printf("Multiplayer disabled\n");
		bTemp ^= 1;
	}

				int h = 0;
	if (joystick->sample(&event) && event.isButton() && event.value == 1) {
		switch (event.number) {
			//Attack
			case 0:
				player->atk ^= 1;
				player->attack();
				break;
			case 1:
			case 3:
				//JUMP
				conditionA = player->jumpCount < player->JUMP_MAX;
				conditionB = player->action != GROUNDPOUND;
				conditionC = player->action != DASH;
				if (conditionA && conditionB && conditionC) {
					player->delta.y = 7.0f;
					player->jumpCount++;
				}
				break;
			case 5:
				//SPECIAL MOVE: DASH
				conditionA = player->action != DASH; 
				conditionB = player->action != GROUNDPOUND;
				if (conditionA || conditionB) {
					player->action = DASH;

					conditionC = player->onGround;
					conditionD = player->delta.x == 0.0f;

					if (conditionC && conditionD) {
						player->delta.y = 0.3f;
					} else {
						player->delta.y = 0.9f;
					}

					if (player->direction == RIGHT) {
						player->delta.x = player->dashStrength;
					} else if (player->direction == LEFT) {
						player->delta.x = -player->dashStrength;
					}
				}
				break;
			case 7:
				//PAUSE
				toggle_pause(joystick);
				break;
			case 12:
				//MOVE RIGHT
				if (player->onMovingPlat) {
					if (player->direction == LEFT && player->onGround) {
						player->direction = RIGHT;
						player->action = PASSIVE;
					} else {
						if (player->platFW == true) {
							player->direction = RIGHT;
							player->action = MOVE;
							player->delta.x = 10.0f; 
							if (player->onGround) { 
								player->delta.y = 1.25f;
							}
						} else {
							player->direction = RIGHT;
							player->action = MOVE;
							player->delta.x = 5.0f; 
							if (player->onGround) { 
								player->delta.y = 1.25f;
							}
						}
					}
				} else {
					if (player->direction == LEFT && player->onGround) {
						player->direction = RIGHT;
						player->action = PASSIVE;
					} else {
						player->direction = RIGHT;
						player->action = MOVE;
						player->delta.x = 5.0f; 
						if (player->onGround) { 
							player->delta.y = 1.25f;
						}
					}
				}
				break;
			case 11:
				//MOVE LEFT
				if (player->onMovingPlat) {
					if (player->direction == RIGHT && player->onGround) {
						player->direction = LEFT;
						player->action = PASSIVE;
					} else {
						player->direction = LEFT;
						player->action = MOVE;
						player->delta.x = -10.0f; 
						if (player->onGround) { 
							player->delta.y = 1.25f; 
						}
					}
					break;
				} else {
					if (player->direction == RIGHT && player->onGround) {
						player->direction = LEFT;
						player->action = PASSIVE;
					} else {
						player->direction = LEFT;
						player->action = MOVE;
						player->delta.x = -5.0f; 
						if (player->onGround) { 
							player->delta.y = 1.25f; 
						}
					}
					break;
				}
			case 14:
				//SPECIAL MOVE: GROUND POUND
				player->action = GROUNDPOUND;
				player->delta.x = 0.0f; 
#ifdef USE_OPENAL_SOUND
				if (player->status.lifeState == ALIVE) {
					play_sound(3, 1.0f, false);
				}
#endif
				player->delta.y = -20.0f; 
				break;
			default: 
				break;
		}
	}
}

//Constructor of colors for player definition
Color::Color() {
	//RGB assignment
	pink[0] = 255;
	pink[1] = 150;
	pink[2] = 150;
	blue[0] = 150;
	blue[1] = 150;
	blue[2] = 255;
	green[0] = 150;
	green[1] = 255;
	green[2] = 150;
	red[0] = 255;
	red[1] = 75;
	red[2] = 75;
	bloodred[0] = 166;
	bloodred[1] = 16;
	bloodred[2] = 30;
	darkgreen[0] = 75;
	darkgreen[1] = 255;
	darkgreen[2] = 75;
	darkblue[0] = 75;
	darkblue[1] = 75;
	darkblue[2] = 255;

	//NEON
	neon[0][0] = 57;
	neon[0][1] = 255;
	neon[0][2] = 20;
	neon[1][0] = 243;
	neon[1][1] = 243;
	neon[1][2] = 21;
	neon[2][0] = 255;
	neon[2][1] = 0;
	neon[2][2] = 146;
	neon[3][0] = 34;
	neon[3][1] = 141;
	neon[3][2] = 255;
	neon[4][0] = 186;
	neon[4][1] = 1;
	neon[4][2] = 255;
	neon[5][0] = 255;
	neon[5][1] = 202;
	neon[5][2] = 27;
}

//Idea to potential toggle a movement state
//move until button is released
void toggle_move(Player *player, Direction dir)
{
	if (dir == LEFT) {
		player->delta.x = -5.0f; 
		if (player->onGround) 
			player->delta.y = 1.25f; 
	} else {
		player->delta.x = 5.0f; 
		if (player->onGround) 
			player->delta.y = 1.25f; 
	}
}

//Player constructor
Player::Player() 
{
	JUMP_MAX = 4;
	body.radius = 50; 
	body.width = 100;
	weapon.width = 1.5 * body.width; 
	weapon.height = 20;
	body.height = body.width;
	body.center.y = 4*scrn->height/5;
	delta.x = 0; 
	delta.y = rnd() * 0.5 - 0.25;
	jumpCount = 0;
	action = PASSIVE;
	dashStrength = 10.0f;

	Color colors;
	for (int i = 0; i < MAX_PARTICLE; i++) {
		//set dimensions of particles
		bloodStream[i].s.width = 4;
		bloodStream[i].s.height = bloodStream[i].s.width;

		//set color of particles
		for (int j = 0; j < 3; j++) {
			bloodStream[i].color[j] = colors.bloodred[j];
		}
	}
	status.initDeath = false;
}

//Game Pad constructor
Gpad::Gpad() 
{
	joystick[0] = new Joystick("/dev/input/js0");
	joystick[1] = new Joystick("/dev/input/js1");
	joystick[2] = new Joystick("/dev/input/js2");
	joystick[3] = new Joystick("/dev/input/js3");
}

void Player::render()
{
	if (status.lifeState == ALIVE) {
		float radius = body.radius;
		float x = body.center.x;
		float y = body.center.y;
		int triangleNum = 25;
		float twicePI = 2.0 * M_PI;

		if (jumpCount == 2) 
			radius = body.radius - 5;
		else if (jumpCount == 3) 
			radius = body.radius - 10;
		else if (jumpCount == 4) 
			radius = body.radius - 15;

		//render weapon
		glPushMatrix();
		glBegin(GL_QUADS);

		if (atk) {
			//outline
			glColor3f(0,0,0);
			glVertex2i(weapon.center.x - weapon.width/2 - 5,
					weapon.center.y - weapon.height/2 - 5);
			glVertex2i(weapon.center.x - weapon.width/2 - 5,
					weapon.center.y + weapon.height/2 + 5);
			glVertex2i(weapon.center.x + weapon.width/2 + 5,
					weapon.center.y + weapon.height/2 + 5);
			glVertex2i(weapon.center.x + weapon.width/2 + 5,
					weapon.center.y - weapon.height/2 - 5);

			//arm
			glColor3ub(color[0],color[1],color[2]);
			glVertex2i(weapon.center.x - weapon.width/2,
					weapon.center.y - weapon.height/2);
			glVertex2i(weapon.center.x - weapon.width/2,
					weapon.center.y + weapon.height/2);
			glVertex2i(weapon.center.x + weapon.width/2,
					weapon.center.y + weapon.height/2);
			glVertex2i(weapon.center.x + weapon.width/2,
					weapon.center.y - weapon.height/2);

			glEnd();
		}

		//black circle
		glPushMatrix();
		glColor4ub(0,0,0,255);
		glEnable (GL_BLEND); 
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBegin(GL_TRIANGLE_FAN);
		glVertex2f(x,y);
		for (int i = 0; i < 120; i++) {
			glVertex2f(x + (radius * cos(i * twicePI / triangleNum)),
					y + (radius * sin(i * twicePI / triangleNum)));
		}
		glEnd();

		//Large Color Circle
		radius -= 3;
		glPushMatrix();
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBegin(GL_TRIANGLE_FAN);
		glColor4ub(color[0],color[1],color[2],255);
		glVertex2f(x,y);
		for (int i = 0; i < 120; i++) {
			glVertex2f(x + (radius * cos(i * twicePI / triangleNum)),
					y + (radius * sin(i * twicePI / triangleNum)));
		}
		glEnd();

		for (int i = 0; i < 3; i++) { 
			if (color[i] == 150) { 
				color[i] += 30;
			} else if (color[i] == 75) {
				color[i] += 55;
			}
		}

		//smaller color circle
		radius = radius - radius/4;
		glPushMatrix();
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBegin(GL_TRIANGLE_FAN);
		int x1;
		int y1 = y + sqrt(radius);

		float OldMax, OldMin, OldValue;
		float NewMax, NewMin, offset;

		//if character is in the middle of the level
		if (body.center.x == scrn->width/2) {
			x1 = x;
		} else if (body.center.x < scrn->width/2) {
			//take the width resolution and scale the entire thing down to 7
			//controls shading of round character
			OldValue = x;
			OldMin = 0;
			OldMax = scrn->width/2;
			NewMin = 0;
			NewMax = sqrt(radius - 10);
			offset = (OldValue - OldMin) * (NewMax - NewMin);
			offset /= (OldMax - OldMin);
			offset += NewMin;
			offset -= sqrt(radius - 10);
			offset = abs(offset);
			x1 = x + offset; 
		} else {
			OldValue = x;
			OldMin = scrn->width/2;
			OldMax = scrn->width;
			NewMin = 0;
			NewMax = sqrt(radius);
			offset = (OldValue - OldMin) * (NewMax - NewMin);
			offset /= OldMax - OldMin; 
			offset += NewMin;
			x1 = x - offset; 
		}

		glColor3ub(color[0],color[1],color[2]);
		glVertex2f(x1,y1);
		for(int i = 0; i < 120; i++) {
			glVertex2f(x1 + (radius * cos(i * twicePI / triangleNum)),
					y1 + (radius * sin(i * twicePI / triangleNum)));
		}
		glEnd();

		//(Eye or Dot) Position
		int y2,x2;
		radius = 6;
		glPushMatrix();
		glColor4f(0.0,0.0,0.0, 1.0);
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBegin(GL_TRIANGLE_FAN);
		if (direction == RIGHT) {
			x2 = x + 3.5 * radius;
			y2 = y + 2 * radius;
		} else {
			x2 = x - 3.5 * radius;
			y2 = y + 2 * radius;
		}
		glVertex2f(x2,y2);
		for(int i = 0; i < 120; i++) {
			glVertex2f(x2 + (radius * cos(i * twicePI / triangleNum)),
					y2 + (radius * sin(i * twicePI / triangleNum)));
		}
		glEnd();
		glDisable(GL_BLEND);

		//reset color
		for (int i = 0; i < 3; i++) { 
			if (color[i] == 180) { 
				color[i] -= 30;
			} else if (color[i] == 130) {
				color[i] -= 55;
			}
		}
	} 
}

//Pill Constructor
Pill::Pill() {
	for (int i = 0; i < 2; i++) {
		body[i].width = 25;
		body[i].height = 25;
		end[i].radius = body[i].height - 0.5;
	}
}

void Level::physics(Player *p) 
{
	int temp = 0;
	int scale = 12;
	float reduction = 0.0;
	p->delta.y -= GRAVITY; 
	p->body.center.y += p->delta.y;
	p->body.center.x += p->delta.x;
	p->weapon.center.x = p->body.center.x;
	p->weapon.center.y = p->body.center.y;

	p->collision(platform); 
	//if (player->onGround && player->delta.y <= 0.0) {
	if (p->onGround) {
		//temp designate which platform the player is colliding with
		temp = p->currentContact;

		if (p->delta.y <= 0.0) {
			p->jumpCount = 0;
			p->body.center.y = platform[temp].center.y 
				+ platform[temp].height/2 + p->body.width/2;
			p->delta.y *= -0.2f;
			p->body.center.y += p->delta.y;
			p->body.center.x += p->delta.x;
			p->weapon.center.x = p->body.center.x;
			p->weapon.center.y = p->body.center.y;
		}

		//Player Moving Right 
		if (p->delta.x > 0.0) {
			//momentum reduction
			reduction = p->delta.x/scale; 
			if (p->delta.x > 0.05) { 
				p->delta.x -= 2 * reduction; 
			}
			else { 
				p->delta.x = 0.0f; 
			}
		}	

		//Player Moving Left 
		if (p->delta.x < 0.0) {
			//momentum reduction
			reduction = -p->delta.x / scale; 
			if (p->delta.x < -0.05) { 
				p->delta.x += 2 * reduction; 
			}
			else {
				p->delta.x = 0.0f; 
			}
		}

		//player not moving
		if (p->delta.x == 0.0) 
			p->action = PASSIVE;
	} 

	for (int i = 0; i < MAX_PLAYER; i++) {
		if (i == p->id) {
			//skip itself
			i++;
		} else {
			//is this character colliding with another?
			p->collision(player[i]);

			//if so, where?
			if (p->collide == TP || p->collide == BOT) {
				p->delta.y *= -0.8;
			} else if (p->collide == LFT || p->collide == RGHT) {
				p->delta.x *= -0.8;
			} else {
				p->collide = NONE;
			}	
		}
	}
}

void Player::collision(Player p)
{
	bool condition1, condition2, condition3, condition4;
	//TOP - This player is under
	condition1 = body.center.y + body.height/2 == p.body.center.y - p.body.height/2;
	condition2 = body.center.x + body.width/2 <= p.body.center.x - p.body.width/2;
	condition3 = body.center.x - body.width/2 >= p.body.center.x + p.body.width/2;

	if (condition1 && condition2 && condition3) {
		collide = TP;
		//playerContact
	}

	//BOTTOM - this player is over
	condition1 = body.center.y - body.height/2 == p.body.center.y + p.body.height/2;
	condition2 = body.center.x + body.width/2 <= p.body.center.x - p.body.width/2;
	condition3 = body.center.x - body.width/2 >= p.body.center.x + p.body.width/2;
	condition4 = delta.y <= 0.0;
	if (condition1 && condition2 && condition3 && condition4) {
		collide = BOT;
	}

	//LEFT - this player is to the right

	//RIGHT - this player is to the left
}

void Player::collision(Shape platform[])
{
	bool condition1, condition2, condition3, condition4, condition5;

	for (int unsigned i = 0; i < MAX_PLAT;i++) {
		//bottom of player collide with floor
		condition1 = (body.center.y - body.height/2) 
			<= (platform[i].center.y + platform[i].height/2);
		//right edge of player to right of left most spot
		condition2 = (body.center.x + body.width/2) 
			>= (platform[i].center.x - platform[i].width/2);
		//left edge of player to left of right most spot
		condition3 = (body.center.x - body.width/2) 
			<= (platform[i].center.x + platform[i].width/2);
		//bottom of player is above the bottom of the platform
		condition4 = (body.center.y - body.height/2) 
			> (platform[i].center.y - platform[i].height/2);
		//player is falling
		condition5 = delta.y <= 0.0;

		if (condition1 && condition2 && condition3 && condition4 
				&& condition5) {
			onGround = true;
			currentContact = i;
			i = MAX_PLAT + 1; //break
		}
		else {  
			onGround = false;
		}
	}
}

void erick(Game *game) 
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	erick_movement(game);
	render_erick(game);
}

void erick_movement(Game *game)
{
	for (int i = 0; i < game->level1.pill_count; i++) {
		game->level1.pill[i].velocity.y -= GRAVITY;
		game->level1.pill[i].center.y += game->level1.pill[i].velocity.y;
		for (int j = 0; j < 2; j++) {
			game->level1.pill[i].body[j].center.y = 
				game->level1.pill[i].center.y;
			game->level1.pill[i].end[j].center.y = 
				game->level1.pill[i].center.y;
		}
		if (game->level1.pill[i].center.y < -10.0) {
			game->level1.pill[i] = 
				game->level1.pill[--game->level1.pill_count];
			--game->level1.pill_count;
		}
	}
}

void makePill(Game *game, int x, int y)
{
	bool flag = true;
	if (game->level1.pill_count >= MAX_PILLS)
		return;

	//position of pill
	Pill *p = &game->level1.pill[game->level1.pill_count];
	p->center.x = x;
	p->center.y = y;
	for (int i = 0; i < 2; i++) {
		if (flag) {
			p->body[i].center.x = p->center.x - p->body[i].width;
			p->body[i].center.y = p->center.y;
			p->end[i].center.x = p->body[i].center.x - p->body[i].width;
			p->end[i].center.y = p->center.y;
		} else {
			p->body[i].center.x = p->center.x + p->body[i].width;
			p->body[i].center.y = p->center.y;
			p->end[i].center.x = p->body[i].center.x + p->body[i].width;
			p->end[i].center.y = p->center.y;
		}
		flag = false;
	}

	p->velocity.y = rnd() * 0.5 - 0.25;
	game->level1.pill_count++;
}

void render_erick(Game *game)
{
	bool flag = true;
	float A = 400;
	float B = 400; 
	float C = 600; 
	float D = 1500;

	glColor3ub(120,10,25);
	glPushMatrix();
	glBegin(GL_QUADS);
	glVertex2i(A,B); 
	glVertex2i(A,C); 
	glVertex2i(D,C); 
	glVertex2i(D,B); 
	glEnd();
	glPopMatrix();

	//Render Pill
	float radius = 0;
	for (int i = 0; i < game->level1.pill_count; i++) {
		for (int j = 0; j < 2; j++) {
			Vec *c = &game->level1.pill[i].body[j].center;
			Vec *cap = &game->level1.pill[i].end[j].center;
			int w = game->level1.pill[i].body[j].width;
			int h = game->level1.pill[i].body[j].height;
			float x = cap->x;
			int triangleNum = 90;
			float twicePI = 2.0 * M_PI;

			if (flag) {
				glColor3ub(255,255,255);
				glBegin(GL_QUADS);
				glVertex2i(c->x-w, c->y-h);
				glVertex2i(c->x-w, c->y+h);
				glVertex2i(c->x+w, c->y+h);
				glVertex2i(c->x+w, c->y-h);
				glEnd();
				glPopMatrix();

				radius = game->level1.pill[i].end[j].radius;
				glPushMatrix();
				glBegin(GL_TRIANGLE_FAN);
				glVertex2f(x, game->level1.pill[i].end[j].center.y);
				for(int k = 0; k < 120; k++) {
					glVertex2f(x + (radius * cos(k * twicePI / triangleNum)),
							game->level1.pill[i].end[j].center.y 
							+ (radius * sin(k * twicePI / triangleNum)));
				}
				glEnd();
				glPopMatrix();

			} else {
				glColor3ub(100,6,2);
				glBegin(GL_QUADS);
				glVertex2i(c->x-w, c->y-h);
				glVertex2i(c->x-w, c->y+h);
				glVertex2i(c->x+w, c->y+h);
				glVertex2i(c->x+w, c->y-h);
				glEnd();
				glPopMatrix();

				radius = game->level1.pill[i].end[j].radius;
				glPushMatrix();
				glBegin(GL_TRIANGLE_FAN);
				glVertex2f(x, game->level1.pill[i].end[j].center.y);
				for(int k = 0; k < 120; k++) {
					glVertex2f(x + (radius * cos(k * twicePI / triangleNum)),
							game->level1.pill[i].end[j].center.y 
							+ (radius * sin(k * twicePI / triangleNum)));
				}
				glEnd();
				glPopMatrix();
			}
			flag = false;
		}
		flag = true;
	}
}

void Disco_Level::init_triangle_sky()
{
	srand(time(NULL));
	int yPartition = 9;
	int xPartition = 16;
	float xRange = 2 * scrn->width;
	float yRange = 2 * scrn->height;
	float xStart = -scrn->width/2;
	float yStart = scrn->height + scrn->height/2;
	float yManip = scrn->height + scrn->height/2;
	float xJump = xRange / xPartition;
	float yJump = yRange / yPartition;
	int sample = 0;
	Color colors;

	//Define verticies
	coor[0].x = xStart;
	coor[0].y = yStart;
	sample = color_rnd();

	for (int j = 0; j < 3; j++) {
		for (int k = 0; k < 3; k++) {
			coor[0].color[j][k] = colors.neon[sample][k];
		}	
	}

	//every (x,y) on screen
	for (int i = 1; i < (xPartition + 1) * (yPartition + 1); i++) {
		//traves accross the screen
		coor[i].x = coor[i-1].x + xJump;
		coor[i].y = yManip;

		//Assign line color
		for (int j = 0; j < 3; j++) {
			sample = color_rnd();
			for (int k = 0; k < 3; k++) {
				coor[i].color[j][k] = colors.neon[sample][k];
			}	
		}	

		//restart x coordinate
		if (i % (xPartition + 1) == 0) {
			yManip -= yJump;
			coor[i].x = xStart;
			coor[i].y = yManip;
		}
	}
}

//Disco Constructor
Disco_Level::Disco_Level() 
{
	fw = true;
	erick_init();
	//floor characteristics
	platform[0].width = scrn->width;
	platform[0].height = scrn->height/20; 
	platform[0].center.x = scrn->width/2.2;
	platform[0].center.y = platform[0].height/2 + 150; 
	//void loadImages();

	//moving platform
	platform[1].width = scrn->width/4;
	platform[1].height = scrn->height/20; 
	platform[1].center.x = scrn->width/2;
	platform[1].center.y = 5*scrn->height/8; 

	//shade handling
	widthPartition = scrn->width/7;
	heightPartition = scrn->height/7;

	init_triangle_sky();
}

void Disco_Level::movingPlat(Shape *p) {
	//if forward flag, increment
	//else, decrement
	if (fw) {
		p->center.x += 5;
	} else if (fw == false) {
		p->center.x -= 5;
	} else {
		p->center.y -= 5;
	}

	//set flag, check range
	if (p->center.x <=0) {
		fw = true;
	} else if (p->center.x >=scrn->width) {
		fw = false; 
	}
}

//player physics for platform
void Disco_Level::movingPlatformPlayer(Player *p)
{
	if (p->onGround && p->currentContact == 1 && p->delta.x == 0.0) {
		float dist = 0;
		float offset = 5;

		p->onMovingPlat = true;
		p->platFW = game.level4.fw;

		if (game.level4.fw == false) {
			dist -= offset;
		} else {
			dist += offset;
		}

		//platform is to the left
		if (game.level4.platform[1].center.x < game.level4.player[0].body.center.x) {
			dist += abs(p->body.center.x) - abs(game.level4.platform[1].center.x);
		}

		p->body.center.x = game.level4.platform[1].center.x + dist;
	} else {
		p->onMovingPlat = false;
	}
}

void ericksTimer(Player *p)
{
	if (p->atk) {
		p->atkCounter++;
		if (p->atkCounter == 1) {
			p->atkCounter = 0;
			p->atk ^= 1;
		}
	}
}

bool alterCoor = false;
bool firstContact = false;
bool forward = true;
void disco(Game *game)
{

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f );
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (alterCoor == true) {
		game->level4.randomizeCoor();
		alterCoor = false;
	}

	game->level4.renderSky();
	game->level4.render();
	game->level4.movingPlat(&game->level4.platform[1]);


	for (int i = 0; i < MAX_PLAYER; i++) {
		//counts frames - only allows for arm to display for one frame
		ericksTimer(&game->level4.player[i]);
		game->level4.movingPlatformPlayer(&game->level4.player[i]);
		game->level4.physics(&game->level4.player[i]);
		game->level4.player[i].check_controller(&game->level4.player[i], 
				game->level4.controller.joystick[i]);
		game->level4.deathCheck(&game->level4.player[i]);
		if (game->level4.player[i].status.lifeState == ALIVE) {
			game->level4.player[i].render();
		} else {
			game->level4.player[i].deathPhysics();
			game->level4.player[i].deathRender();
		}
		game->level4.statDisplay.render();

		if (game->level4.player[i].status.lifeState == ALIVE) 
		{
			game->level4.player[i].render();
		} 
		else if (game->level4.player[i].status.lifeState == DEAD 
				&& game->level4.player[i].status.initDeath == false) 
		{
			clock_gettime(CLOCK_REALTIME, &game->level4.player[i].timeStart);
			game->level4.player[i].status.initDeath = true;
		}
		else if (game->level4.player[i].status.lifeState == DEAD 
				&& game->level4.player[i].status.initDeath == true) 
		{
			clock_gettime(CLOCK_REALTIME, &game->level4.player[i].timeCurrent);
			game->level4.player[i].timeSpan = 
				game->level4.player[i].timeDiff(&game->level4.player[i].timeStart, 
						&game->level4.player[i].timeCurrent);
			if (game->level4.player[i].timeSpan >= 5 
					&& game->level4.player[i].status.lifeCount > 0)
			{
				game->level4.player[i].status.initDeath = false;
				game->level4.respawn(&game->level4.player[i]);
			}
			else if (game->level4.player[i].status.lifeCount < 0) {
				game->level4.player[i].delta.x = 0.0;
				game->level4.player[i].delta.y = 0.0;
			}
		}	
	}

	game->level4.statDisplay.render();
}

int ct = 0;
bool begining = true;
float opacity = 0;
void Disco_Level::render()	
{
	glDisable(GL_BLEND);
	Color colors;
	glLineWidth(1);
	glColor3ub(255,255,255);
	glBegin(GL_LINES);
	glVertex2f(-scrn->width/2,-scrn->height/2);
	glVertex2f(-scrn->width/2,1.5*scrn->height);
	glEnd();

	glBegin(GL_LINES);
	glVertex2f(-scrn->width/2,1.5*scrn->height);
	glVertex2f(1.5*scrn->width,1.5*scrn->height);
	glEnd();

	glBegin(GL_LINES);
	glVertex2f(1.5*scrn->width,1.5*scrn->height);
	glVertex2f(1.5*scrn->width,-scrn->height/2);
	glEnd();

	glBegin(GL_LINES);
	glVertex2f(1.5*scrn->width,-scrn->height/2);
	glVertex2f(-scrn->width/2,-scrn->height/2);
	glEnd();

	//floor
	glColor3ub(0,0,0); 
	glPushMatrix(); 
	glBegin(GL_QUADS); 
	glVertex2i(platform[0].center.x - platform[0].width/2 - 5, 
			platform[0].center.y - platform[0].height/2 - 5);
	glVertex2i(platform[0].center.x - platform[0].width/2 - 5, 
			platform[0].center.y + platform[0].height/2 + 5);
	glVertex2i(platform[0].center.x + platform[0].width/2 + 5, 
			platform[0].center.y + platform[0].height/2 + 5);
	glVertex2i(platform[0].center.x + platform[0].width/2 + 5, 
			platform[0].center.y - platform[0].height/2 - 5);
	glEnd();

	//floor outline
	glColor3ub(colors.neon[1][0],colors.neon[1][1],colors.neon[1][2]); 
	glPushMatrix(); 
	glBegin(GL_LINES); 
	glVertex2i(platform[0].center.x - platform[0].width/2 - 5, 
			platform[0].center.y - platform[0].height/2 - 5);
	glVertex2i(platform[0].center.x - platform[0].width/2 - 5, 
			platform[0].center.y + platform[0].height/2 + 5);
	glVertex2i(platform[0].center.x - platform[0].width/2 - 5, 
			platform[0].center.y + platform[0].height/2 + 5);
	glVertex2i(platform[0].center.x + platform[0].width/2 + 5, 
			platform[0].center.y + platform[0].height/2 + 5);
	glVertex2i(platform[0].center.x + platform[0].width/2 + 5, 
			platform[0].center.y + platform[0].height/2 + 5);
	glVertex2i(platform[0].center.x + platform[0].width/2 + 5, 
			platform[0].center.y - platform[0].height/2 - 5);
	glVertex2i(platform[0].center.x + platform[0].width/2 + 5, 
			platform[0].center.y - platform[0].height/2 - 5);
	glVertex2i(platform[0].center.x - platform[0].width/2 - 5, 
			platform[0].center.y - platform[0].height/2 - 5);
	glEnd();

	//moving platform
	glColor3ub(0,0,0); 
	glPushMatrix(); 
	glBegin(GL_QUADS); 
	glVertex2i(platform[1].center.x - platform[1].width/2 - 5, 
			platform[1].center.y - platform[1].height/2 - 5);
	glVertex2i(platform[1].center.x - platform[1].width/2 - 5, 
			platform[1].center.y + platform[1].height/2 + 5);
	glVertex2i(platform[1].center.x + platform[1].width/2 + 5, 
			platform[1].center.y + platform[1].height/2 + 5);
	glVertex2i(platform[1].center.x + platform[1].width/2 + 5, 
			platform[1].center.y - platform[1].height/2 - 5);
	glEnd();

	//floor outline
	glColor3ub(colors.neon[1][0],colors.neon[1][1],colors.neon[1][2]); 
	glPushMatrix(); 
	glBegin(GL_LINES); 
	glVertex2i(platform[1].center.x - platform[1].width/2 - 5, 
			platform[1].center.y - platform[1].height/2 - 5);
	glVertex2i(platform[1].center.x - platform[1].width/2 - 5, 
			platform[1].center.y + platform[1].height/2 + 5);
	glVertex2i(platform[1].center.x - platform[1].width/2 - 5, 
			platform[1].center.y + platform[1].height/2 + 5);
	glVertex2i(platform[1].center.x + platform[1].width/2 + 5, 
			platform[1].center.y + platform[1].height/2 + 5);
	glVertex2i(platform[1].center.x + platform[1].width/2 + 5, 
			platform[1].center.y + platform[1].height/2 + 5);
	glVertex2i(platform[1].center.x + platform[1].width/2 + 5, 
			platform[1].center.y - platform[1].height/2 - 5);
	glVertex2i(platform[1].center.x + platform[1].width/2 + 5, 
			platform[1].center.y - platform[1].height/2 - 5);
	glVertex2i(platform[1].center.x - platform[1].width/2 - 5, 
			platform[1].center.y - platform[1].height/2 - 5);
	glEnd();

}

void Disco_Level::renderSky() {
	//TriSky
	int processed = 0;
	ct = 0;
	int x,x1,x2,x3,y,y1,y2,y3;
	glLineWidth(3);
	for (int i = 0; i < 17 * 9; i++) {
		bool flag = true;

		if (processed == (16 + ct) && begining == true) {
			processed = 0;
			flag ^= 1;
			begining ^= 1;
			ct++;
		} else if (processed == 16) {
			processed = 0;
			flag ^= 1;
		}

		x = coor[i].x;
		y = coor[i].y;

		//coordinate to the right
		x1 = coor[i + 1].x;
		y1 = coor[i + 1].y;

		//coordinate below
		x2 = coor[i + 17].x;
		y2 = coor[i + 17].y;

		//coordinate right and below
		x3 = coor[i + 18].x;
		y3 = coor[i + 18].y;

		if (flag) {
			glEnable(GL_ALPHA_TEST);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glColor4ub(coor[i].color[0][0],coor[i].color[0][1],
					coor[i].color[0][2], opacity);
			glBegin(GL_LINES);
			glVertex2f(x,y);
			glVertex2f(x1,y1);
			glEnd();
			glDisable(GL_ALPHA_TEST);

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glColor4ub(coor[i].color[1][0],coor[i].color[1][1],
					coor[i].color[1][2], opacity);
			glBegin(GL_LINES);
			glVertex2f(x,y);
			glVertex2f(x2,y2);
			glEnd();
			glDisable(GL_ALPHA_TEST);

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glColor4ub(coor[i].color[2][0],coor[i].color[2][1],
					coor[i].color[2][2], opacity);
			glBegin(GL_LINES);
			glVertex2f(x,y);
			glVertex2f(x3,y3);
			glEnd();
			glDisable(GL_ALPHA_TEST);
			glDisable(GL_BLEND);	
		}
		glColor3ub(255,0,0);
		glBegin(GL_POINTS);
		glVertex2f(x,y);
		glEnd();

		if (flag == true) {
			processed++;
		}
	}	
	if (opacity >= 255) {
		forward ^= 1;
	}
	else if (opacity < 0) {
		//randomize when this condition is true
		alterCoor = true;
		forward ^= 1;
	}

	if (forward) {
		opacity += 0.75;
	} else {
		opacity -= 0.75;
	}
}

void Disco_Level::randomizeCoor() {
	srand(time(NULL));
	int yPartition = 9;
	int xPartition = 16;
	float xRange = 2 * scrn->width;
	float yRange = 2 * scrn->height;
	float xStart = -scrn->width/2;
	float yStart = scrn->height + scrn->height/2;
	float yManip = scrn->height + scrn->height/2;
	float xJump = xRange / xPartition;
	float yJump = yRange / yPartition;
	int sample = 0;
	Color colors;

	//Define first  vertex
	coor[0].x = xStart;
	coor[0].y = yStart;
	sample = color_rnd();

	//assign 3 colors per vertex
	for (int j = 0; j < 3; j++) {
		for (int k = 0; k < 3; k++) {
			coor[0].color[j][k] = colors.neon[sample][k];
		}	
	}

	//for every (x,y) on screen
	int temp = change();
	for (int i = 1; i < (xPartition + 1) * (yPartition + 1); i++) {
		//travse accross the screen
		if (temp % 2 != 0) {
			if(change() % 1 == 0) {
				coor[i].x ^= 1;
				coor[i].x = coor[i-1].x + xJump * coor();
				coor[i].y ^= 1;
				coor[i].y = yManip + yManip * coor();
			}
			else {
				coor[i].x = coor[i-1].x + xJump;
				coor[i].y = yManip;
			}
		} else {
			if(temp % 1 == 0) {
				coor[i].x = coor[i-1].x + xJump * coor();
				coor[i].y = yManip + yManip * coor();
			} 
			else if(temp % 2 == 0) {
				coor[i].x = coor[i-1].x + xJump;
				coor[i].y = yManip + pow(coor(), 4) * 100;
			}
			else {
				coor[i].x = coor[i-1].x + xJump;
				coor[i].y = yManip;
			}
		}

		//Assign line color
		for (int j = 0; j < 3; j++) {
			sample = color_rnd();
			for (int k = 0; k < 3; k++) {
				coor[i].color[j][k] = colors.neon[sample][k];
			}	
		}	

		//restart x coordinate
		if (i % (xPartition + 1) == 0) {
			yManip -= yJump;
			coor[i].x = xStart;
			coor[i].y = yManip;
		}
	}
}

void Field_Level::erick_init()
{
	//life assignment, health assignment
	//Temporary color assignment until character select is up and running
	Color colors;
	int evenCount = 0;
	int oddCount = 0;

	for (int i = 0; i < MAX_PLAYER; i++) {
		player[i].index = i;
		player[i].multiplier = 0.0;
		player[i].status.health = 0;
		player[i].status.lifeCount = 5;
		player[i].status.lifeState = ALIVE;
		player[i].id = i;
		player[i].collide = NONE;

		if (i % 2 == 0) {
			if (evenCount % 2 == 0) {
				player[i].body.center.y = scrn->height/5;
			} else {
				player[i].body.center.y = 3 * scrn->height/5;
			}
			player[i].body.center.x = scrn->width/5;
			player[i].direction = RIGHT;
			evenCount++;
		} else {
			if (oddCount % 2 == 0) {
				player[i].body.center.y = scrn->height/5;
			} else {
				player[i].body.center.y = 3 * scrn->height/5;
			}
			player[i].body.center.x = 4 * scrn->width/5;
			player[i].direction = LEFT;
			oddCount++;
		}
		player[i].weapon.center.x = player[i].body.center.x;
		player[i].weapon.center.y = player[i].body.center.y;
		switch(i) {
			case 0:
				for (int j = 0; j < 3; j++)
					player[i].color[j] = colors.red[j];
				break;
			case 1:
				for (int j = 0; j < 3; j++)
					player[i].color[j] = colors.pink[j];
				break;
			case 2:
				for (int j = 0; j < 3; j++)
					player[i].color[j] = colors.darkblue[j];
				break;
			case 3:
				for (int j = 0; j < 3; j++)
					player[i].color[j] = colors.darkgreen[j];
				break;
			default:
				break;
		}
	}
}

void Starynight_Level::erick_init()
{
	Color colors;
	int evenCount = 0;
	int oddCount = 0;
	for (int i = 0; i < MAX_PLAYER; i++) {
		player[i].index = i;
		player[i].multiplier = 0.0;
		player[i].status.health = 0;
		player[i].status.lifeCount = 5;
		player[i].id = i;
		player[i].collide = NONE;

		if (i % 2 == 0) {
			if (evenCount % 2 == 0) {
				player[i].body.center.y = scrn->height/5;
			} else {
				//void loadImages();
				//void loadImages();
				player[i].body.center.y = 3 * scrn->height/5;
			}
			player[i].body.center.x = scrn->width/5;
			player[i].direction = RIGHT;
			evenCount++;
		} else {
			if (oddCount % 2 == 0) {
				player[i].body.center.y = scrn->height/5;
			} else {
				player[i].body.center.y = 3 * scrn->height/5;
			}
			player[i].body.center.x = 4 * scrn->width/5;
			player[i].direction = LEFT;
			oddCount++;
		}
		player[i].weapon.center.x = player[i].body.center.x;
		player[i].weapon.center.y = player[i].body.center.y;

		switch(i) {
			case 0:
				for (int j = 0; j < 3; j++)
					player[i].color[j] = colors.red[j];
				break;
			case 1:
				for (int j = 0; j < 3; j++)
					player[i].color[j] = colors.pink[j];
				break;
			case 2:
				for (int j = 0; j < 3; j++)
					player[i].color[j] = colors.blue[j];
				break;
			case 3:
				for (int j = 0; j < 3; j++)
					player[i].color[j] = colors.green[j];
				break;
			default:
				break;
		}
	}
}

void Disco_Level::erick_init()
{
	Color colors;
	int evenCount = 0;
	int oddCount = 0;
	for (int i = 0; i < MAX_PLAYER; i++) {
		player[i].index = i;
		player[i].id = i;
		player[i].multiplier = 0.0;
		player[i].status.health = 0;
		player[i].status.lifeCount = 5;
		player[i].collide = NONE;
		player[i].atk = false;

		if (i % 2 == 0) {
			if (evenCount % 2 == 0) {
				player[i].body.center.y = scrn->height/5;
			} else {
				player[i].body.center.y = 3 * scrn->height/5;
			}
			player[i].body.center.x = scrn->width/5;
			player[i].direction = RIGHT;
			evenCount++;
		} else {
			if (oddCount % 2 == 0) {
				player[i].body.center.y = scrn->height/5;
			} else {
				player[i].body.center.y = 3 * scrn->height/5;
			}
			player[i].body.center.x = 4 * scrn->width/5;
			player[i].direction = LEFT;
			oddCount++;
		}
		player[i].weapon.center.x = player[i].body.center.x;
		player[i].weapon.center.y = player[i].body.center.y;

		switch(i) {
			case 0:
				for (int j = 0; j < 3; j++)
					player[i].color[j] = colors.red[j];
				break;
			case 1:
				for (int j = 0; j < 3; j++)
					player[i].color[j] = colors.pink[j];
				break;
			case 2:
				for (int j = 0; j < 3; j++)
					player[i].color[j] = colors.blue[j];
				break;
			case 3:
				for (int j = 0; j < 3; j++)
					player[i].color[j] = colors.green[j];
				break;
			default:
				break;
		}
	}
}
