//Author: Erick Ortiz

/*
Progress Log
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

#define rnd() (float)rand() / (float)RAND_MAX
#define MAX_PILLS 1000
#define x_rnd() (rand() % 11) - 5 
#define y_rnd() (rand() % 11)  

extern Screen* scrn;
extern void resetMain(Game *game);
extern Game game;

void Player::check_controller(Player *player, Joystick *joystick)
{
    bool conditionA, conditionB, conditionC, conditionD;
    JoystickEvent event;

    bool temp = true;
    if (!joystick->isFound() && temp)
    {
	printf("Attention: Game Pad is not connected\n");
	printf("Multiplayer disabled\n");
	temp = false;
    }

    if (joystick->sample(&event) && event.isButton() && event.value == 1) {
	switch (event.number) {
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
		    usleep(300000);

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
		break;
	    case 7:
		//PAUSE
		toggle_pause(joystick);
		break;
	    case 12:
		//MOVE RIGHT
		if (player->action != DASH) {
		    player->direction = RIGHT;
		    player->action = MOVE;
		    player->delta.x = 5.0f; 
		}
		if (player->onGround) 
		    player->delta.y = 1.25f;
		break;
	    case 11:
		//MOVE LEFT
		if (player->action != DASH) {
		    player->direction = LEFT;
		    player->action = MOVE;
		    player->delta.x = -5.0f; 
		}
		if (player->onGround) 
		    player->delta.y = 1.25f; 
		break;
	    case 14:
		//SPECIAL MOVE: GROUND POUND
		player->action = GROUNDPOUND;
		player->delta.x = 0.0f; 
		usleep(300000);
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
//Must rework, each level must assign a center(x,y)
Player::Player() 
{
    JUMP_MAX = 4;
    body.radius = 50; 
    body.width = 100; 
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


}

//Game Pad constructor
Gpad::Gpad() 
{
    joystick[0] = new Joystick("/dev/input/js0");
    joystick[1] = new Joystick("/dev/input/js1");
    joystick[2] = new Joystick("/dev/input/js2");
    joystick[3] = new Joystick("/dev/input/js3");
}

//Render Function for Player "Kirby"
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

	//black circle
	glPushMatrix();
	glColor4ub(0,0,0,255);
	glEnable (GL_BLEND); 
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x,y);
	for(int i = 0; i < 120; i++) {
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
	for(int i = 0; i < 120; i++) {
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
    else if (status.lifeState == DEAD) {
	//DEATH ANIMATION
	glColor3ub(0,255,0); 
	glPushMatrix(); 
	glBegin(GL_QUADS); 
	glVertex2i(scrn->width/2,scrn->height/2);
	glVertex2i(scrn->width/3, scrn->height);
	glVertex2i(0,0);
	glVertex2i(50,200);
	glEnd(); 
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

void makePoundParticle(Game *game, int x, int y)
{
    /*
       if (game->level2.partCount >= MAX_PARTICLE)
       return;

    //initialize particle
    Particle *p = &game->level2.particle[game->level2.partCount];
    p->s.center.x = x;
    p->s.center.y = y;
    p->velocity.y = y_rnd();
    p->velocity.x = x_rnd();
    game->level2.partCount++;
    */
}

void Level::physics(Player *player) 
{
    int temp = 0;
    int scale = 12;
    float reduction = 0.0;
    player->delta.y -= GRAVITY; 
    player->body.center.y += player->delta.y;
    player->body.center.x += player->delta.x;

    player->collision(platform); 
    //if (player->onGround && player->delta.y <= 0.0) {
    if (player->onGround) {
	//temp designate which platform the player is colliding with
	temp = player->currentContact;

	if (player->delta.y <= 0.0) {
	    player->jumpCount = 0;
	    player->body.center.y = platform[temp].center.y 
		+ platform[temp].height/2 + player->body.width/2;
	    player->delta.y *= -0.2f;
	    player->body.center.y += player->delta.y;
	    player->body.center.x += player->delta.x;
	}

	//Player Moving Right 
	if (player->delta.x > 0.0) {
	    //momentum reduction
	    reduction = player->delta.x/scale; 
	    if (player->delta.x > 0.05)  
		player->delta.x -= 2 * reduction; 
	    else 
		player->delta.x = 0.0f; 
	}	

	//Player Moving Left 
	if (player->delta.x < 0.0) {
	    //momentum reduction
	    reduction = -player->delta.x / scale; 
	    if (player->delta.x < -0.05)  
		player->delta.x += 2 * reduction; 
	    else 
		player->delta.x = 0.0f; 
	}

	//player not moving
	if (player->delta.x == 0.0) 
	    player->action = PASSIVE;
    } 
    //else if (player->onGround && player->delta.y > 0.0) {
    //}
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
	condition4 = (body.center.y - body.height/2) 
	    > (platform[i].center.y - platform[i].height/2);
	condition5 = delta.y <= 0.0;

	if (condition1 && condition2 && condition3 && condition4 && condition5) {
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

void init_triangle_sky(Game *game)
{
    //Define verticies
    for (int i = 0; i < 5; i++) {
	for (int j = 0; j < 5; j++) {
	}
    }
}

void triangle_sky(Game *game)
{
}

void render_triangle_sky(Game *game)
{
}

Bakersfield_Level::Bakersfield_Level() 
{
    //floor characteristics
    platform[0].center.x = scrn->width/2;
    platform[0].width = scrn->width-250;
    platform[0].height = scrn->height/20; 
    platform[0].center.y = platform[0].height/2 + 150; 

    //oil
    oil.width = 2 * scrn->width;
    oil.height = scrn->height/6;
    oil.center.x = scrn->width/2;
    oil.center.y = -scrn->width/6;

    //moon
    moon.center.x = scrn->width; 
    moon.center.y = scrn->height; 
    moon.radius = scrn->height/4; 

    //sun
    sun.center.x = scrn->width; 
    sun.center.y = scrn->height; 
    sun.radius = scrn->height/3; 

    //Toxic Gas Cloud
    gas.center.x = scrn->width/2;
    gas.center.y = scrn->height/2;
    gas.width = scrn->width * 2;
    gas.height = scrn->height * 2;

    //shade handling
    widthPartition = scrn->width/7;
    heightPartition = scrn->height/7;

    //life assignment, health assignment
    for (int i = 0; i < MAX_PLAYER; i++) {
	player[i].status.health = 0;
	player[i].status.lifeCount = 5;
    }
}

void bakersfield(Game *game)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    game->level4.render();

    for (int i = 0; i < MAX_PLAYER; i++) {
	game->level4.physics(&game->level4.player[i]);
	game->level4.player[i].check_controller(&game->level4.player[i], 
		game->level4.controller.joystick[i]);
	game->level4.player[i].render();
    }
    game->level4.statDisplay.render();
}

void Bakersfield_Level::render() 
{
    //Render of background
    glClear(GL_COLOR_BUFFER_BIT); 
    glColor3ub(80, 60, 0); 
    glPushMatrix(); 
    glBegin(GL_QUADS); 
    glVertex2i(-scrn->width/2,-scrn->height/2);
    glColor3ub(85,60,35); 
    glVertex2i(-scrn->width/2, 1.5*scrn->height);
    glColor3ub(80, 60, 0); 
    glVertex2i(1.5*scrn->width,1.5*scrn->height);
    glColor3ub(80, 30, 0); 
    glVertex2i(1.5*scrn->width,-scrn->height/2);
    glEnd(); 
    // End of Background

    //Render black line on floor
    glColor3ub(0,0,0); 
    glPushMatrix(); 
    glBegin(GL_QUADS); 
    glVertex2i(platform[0].center.x - platform[0].width/2 - 2, 
	    platform[0].center.y - platform[0].height/2 - 2);
    glVertex2i(platform[0].center.x - platform[0].width/2 - 2, 
	    platform[0].center.y + platform[0].height/2 + 2);
    glVertex2i(platform[0].center.x + platform[0].width/2 + 2, 
	    platform[0].center.y + platform[0].height/2 + 2);
    glVertex2i(platform[0].center.x + platform[0].width/2 + 2, 
	    platform[0].center.y - platform[0].height/2 - 2);
    glEnd(); 
    // End of blfloor

    //Render of floor
    glColor3ub(99,160,0); 
    glPushMatrix(); 
    glBegin(GL_QUADS); 
    glVertex2i(platform[0].center.x - platform[0].width/2, 
	    platform[0].center.y - platform[0].height/2);
    glVertex2i(platform[0].center.x - platform[0].width/2, 
	    platform[0].center.y + platform[0].height/2);
    glColor3ub(120,210,0); 
    glVertex2i(platform[0].center.x + platform[0].width/2, 
	    platform[0].center.y + platform[0].height/2);
    glVertex2i(platform[0].center.x + platform[0].width/2, 
	    platform[0].center.y - platform[0].height/2);
    glEnd(); 
    // End of floor
}

/*
   void Level::physics(Player *player) 
   {
   float reduction = 0.0;

   for (int unsigned i = 0; i < MAX_PLAT;i++) {
   player->collision(&platform[i]); 
   if (player->onGround) {
   player->jumpCount = 0;

   if ((player->body.center.y <= platform[i].center.y - platform[i].height/2)) {
//nothing
} else if ((player->body.center.y > platform[i].center.y - platform[i].height/2)) {
player->body.center.y = (platform[i].center.y + platform[i].height/2) + (player->body.height/2);
player->delta.y = -player->delta.y * 0.2f; 
}

//Player Moving Right 
if (player->delta.x > 0) {
//momentum reduction
reduction = player->delta.x/15; 
if (player->delta.x > 0.05)  
player->delta.x -= reduction; 
else 
player->delta.x = 0.0f; 
}

//Player Moving Left 
if (player->delta.x < 0) {
//momentum reduction
reduction = -player->delta.x / 15; 
if (player->delta.x < -0.05)  
player->delta.x += reduction; 
else 
player->delta.x = 0.0f; 
}

//player not moving
if (player->delta.x == 0) 
player->action = PASSIVE;
}
//player in the air
else {
player->delta.y -= GRAVITY; 
player->body.center.y += player->delta.y;
player->body.center.x += player->delta.x;
}
}
}
*/

/*
   void Level::physics(Player *player) 
   {
   float reduction = 0.0;
   player->delta.y -= GRAVITY;
   player->body.center.y += player->delta.y;
   player->body.center.x += player->delta.x;

   if (player->onGround == true) {
   player->jumpCount = 0;
   }

   for (int unsigned i = 0; i < MAX_PLAT;i++) {
//obtain whether player is on platform
player->collision(&platform[i]);

if (player->onGround) {
player->body.center.y = platform[i].center.y + platform[i].height/2 + player->body.height;
if (player->delta.x == 0.0 && player->delta.y == 0.0) {
player->action = PASSIVE;
}
//moving left
else if (player->delta.x < 0.0) {
reduction = -player->delta.x / 15; 
if (player->delta.x < -0.05)  
player->delta.x += reduction; 
else 
player->delta.x = 0.0f; 
} 
//moving right
else if (player->delta.x > 0.0) {
reduction = player->delta.x/15; 
if (player->delta.x > 0.05)  
player->delta.x -= reduction; 
else 
player->delta.x = 0.0f; 
}
}

if ((player->body.center.y <= platform[i].center.y + platform[i].height/2) && player->delta.y < 0.0) {
player->body.center.y = (platform[i].center.y + platform[i].height/2) + (player->body.height/2);
player->delta.y = -player->delta.y * 0.2f; 
}
}
}
*/

//void Player::collision(Shape *platform[])
/*
   void Player::collision(Shape *platform)
//void Player::collision(Shape *platform[])
{
bool condition1, condition2, condition3, condition4;

//for (int unsigned i = 0; i < MAX_PLAT;i++) {
//bottom of player collide with floor
condition1 = (body.center.y - body.height/2) <= (platform->center.y + platform->height/2);
//right edge of player to right of left most spot
condition2 = (body.center.x + body.width/2) >= (platform->center.x - platform->width/2);
//left edge of player to left of right most spot
condition3 = (body.center.x - body.width/2) <= (platform->center.x + platform->width/2);
condition4 = body.center.y >= 0.0;	

if (condition1 && condition2 && condition3 && condition4) 
onGround = true;
else  
onGround = false;
//}
}
*/
void Field_Level::erick_init()
{
    //==================ERICK's CODE====================
    //life assignment, health assignment
    //Temporary color assignment until character select is up and running
    Color colors;
    int evenCount = 0;
    int oddCount = 0;

    for (int i = 0; i < MAX_PLAYER; i++) {
	player[i].status.health = 0;
	player[i].status.lifeCount = 5;
	player[i].status.lifeState = ALIVE;

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
    //================END ERICK's CODE====================
}

void Starynight_Level::erick_init()
{
    //==================ERICK's CODE====================
    //life assignment, health assignment
    //Temporary color assignment until character select is up and running


    Color colors;
    int evenCount = 0;
    int oddCount = 0;
    for (int i = 0; i < MAX_PLAYER; i++) {
	player[i].status.health = 0;
	player[i].status.lifeCount = 5;
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
    //================END ERICK's CODE====================
}
