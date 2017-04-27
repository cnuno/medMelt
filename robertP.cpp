/*
Author: Robert Pierucci
Progress Log
04/09/2017
I added an area on the screen of each level. We will display the health, life 
count, and unique identifier here. Since we changed the coordinates of the 
screen [bottom left is (-scrn->width/2, -scrn->height/2) the top right is
(scrn->width * 3/2, scrn->height * 3/2). At first, the box was solid with no
transparency. After playing the game and flying around the bottom of the 
screen. The boxes were made transparent. This allowed the players to keep 
track of their position.

04/10/2017
I am now handling death and respawning. For now, the level assigns 5 lives.
I am trying to figure out respawning. I am trying to develop a death animation
as well. I need to collaborate with cesar to seeif he can make an additional
menu for character select that allows the users to define the number of lives
each player has.

04/16/2017
Today I figured out how to make the characters respawn. The death functions
are called in each level handler.

04/17/2017
Added timeDiff function to handle player respawns. Next up will work on revamping 
sound code and moving texture definitions out of my file.

4/21/2017
Today we rendered the weapon, character status boxes, and added the attack 
collision detection. We worked for 7.75 hours.

4/22-4/23/2017
Tonight I isolated the sound and added ifdef statements around all sound stuff. I 
also edited the makefile to create an executable with the sound and one without
the sound. I refactored the sound code that will load all sound files used in
the game at one time for ease of access. I also added songs to the levels as well
as sounds to the players.
 */

#include "headers.h"
#include "robertP.h"
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <iostream>
#ifdef USE_OPENAL_SOUND
#include </usr/include/AL/alut.h>
#endif
#define GRAVITY .1
#define MAX_PARTICLE 50
#define x_rnd() (rand() % 11) - 5 
#define y_rnd() (rand() % 11)  

extern Screen* scrn;
using namespace std;

extern void resetMain(Game *game); 
#ifdef USE_OPENAL_SOUND
#define TOTALSOUNDS 6
extern ALuint alSource[TOTALSOUNDS];
extern ALuint alBuffer[TOTALSOUNDS];
extern int titledrop;
extern float pitch;
#endif
extern Ppmimage *etIcon;
extern GLuint etTexture;
extern Game game;
char st[4][15] = {{"Player 1"},{"Player 2"},{"Player 3"}, {"Player 4"}}; 
Game::~Game()
{
    system("rm -Rf images/");
    system("rm -Rf audio/");
}

//Sound Code
#ifdef USE_OPENAL_SOUND

void initialize_sounds ()
{
    alutInit(0, NULL);
    if (alGetError() != AL_NO_ERROR) {
        cout << "OPENAL ERROR" << endl;
        return;
    }
    alGetError();
    float vec[6] = {0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f};
    alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
    alListenerfv(AL_ORIENTATION, vec);
    alListenerf(AL_GAIN, 1.0f);

    //add sound sources here
    alBuffer[0] = alutCreateBufferFromFile("./audio/test.wav");
    alBuffer[1] = alutCreateBufferFromFile("./audio/disco.wav");
    alBuffer[2] = alutCreateBufferFromFile("./audio/sword.wav");
    alBuffer[3] = alutCreateBufferFromFile("./audio/pound.wav");
    alBuffer[4] = alutCreateBufferFromFile("./audio/field.wav");
    alBuffer[5] = alutCreateBufferFromFile("./audio/starnight.wav");

    alGenSources(TOTALSOUNDS, alSource);

    //link buffers here
    alSourcei(alSource[0], AL_BUFFER, alBuffer[0]);
    alSourcei(alSource[1], AL_BUFFER, alBuffer[1]);	
    alSourcei(alSource[2], AL_BUFFER, alBuffer[2]);
    alSourcei(alSource[3], AL_BUFFER, alBuffer[3]);
    alSourcei(alSource[4], AL_BUFFER, alBuffer[4]);
    alSourcei(alSource[5], AL_BUFFER, alBuffer[5]);


}

void play_sound (int track, float pitch, bool loop)
{
    alSourcef(alSource[track], AL_GAIN, 1.0f);
    alSourcef(alSource[track], AL_PITCH, pitch);
    alSourcei(alSource[track], AL_LOOPING, loop);
    if (alGetError() != AL_NO_ERROR) {
        cout << "OPENAL ERROR" << endl;
        return;
    }
    alSourcePlay(alSource[track]);
}

void cleanup_sounds () {
    for (int i = 0; i < TOTALSOUNDS; i++) {
        alDeleteSources(1, &alSource[i]);
    }
    for (int i = 0; i < TOTALSOUNDS; i++) {
        alDeleteBuffers(1, &alBuffer[i]);
    }
    ALCcontext *Context = alcGetCurrentContext();
    ALCdevice *Device = alcGetContextsDevice(Context);
    alcMakeContextCurrent(NULL);
    alcDestroyContext(Context);
    alcCloseDevice(Device);
    return;
}

#endif

//Constructor to display player stats such as health
StatDisplay::StatDisplay()
{
    int bufferSize = 150;
    int partitionSize = scrn->width/4;
    int distance = 0;

    //define the x coordinate for each box
    quadrant[0].center.x = -partitionSize/2;
    quadrant[3].center.x = scrn->width + partitionSize/2;
    distance = quadrant[3].center.x + abs(quadrant[0].center.x);
    quadrant[1].center.x = quadrant[0].center.x + distance/3;
    quadrant[2].center.x = quadrant[3].center.x - distance/3;

    //uniformly define y coordinate for each box, width and height too
    for (int i = 0; i < 4; i++) {
        quadrant[i].center.y = -scrn->height/6;
        quadrant[i].width = partitionSize - (2 * bufferSize);
        quadrant[i].height = quadrant[i].width;
    }
}

//need to pass an instance of each level. render(Level *level) does not work
void StatDisplay::render() 
{
    //need to access player health, lifecount and display information here
    int coorA, coorB, coorC, coorD;
    int borderSize = 5;

    //each i corresponds to each player
    for (int i = 0; i < 4; i++) {
        //Acquire coordinates

        coorA = quadrant[i].center.x - quadrant[i].width;
        coorB = quadrant[i].center.x + quadrant[i].width;
        coorC = quadrant[i].center.y - quadrant[i].height;
        coorD = quadrant[i].center.y + quadrant[i].height;

        //Black Border
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4ub(0,0,0,100); 
        glPushMatrix(); 
        glBegin(GL_QUADS); 
        glVertex2i(coorA - borderSize, coorC - borderSize);
        glVertex2i(coorA - borderSize, coorD + borderSize);
        glVertex2i(coorB + borderSize, coorD + borderSize);
        glVertex2i(coorB + borderSize, coorC - borderSize);
        glEnd(); 
        glDisable(GL_BLEND);

        //BackGround
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4ub(155,155,255,100); 
        glPushMatrix(); 
        glBegin(GL_QUADS); 
        glVertex2i(coorA, coorC);
        glVertex2i(coorA, coorD);
        glVertex2i(coorB, coorD);
        glVertex2i(coorB, coorC);
        glEnd(); 
        glDisable(GL_BLEND);

        switch (game.render) {
            case STARYNIGHT: 
                game.level3.player[i].boxRender(quadrant[i].center.x, 
                        quadrant[i].center.y, quadrant[i].width);
                break;
            case FIELD:
                game.level2.player[i].boxRender(quadrant[i].center.x, 
                        quadrant[i].center.y, quadrant[i].width);
                break;
            case DISCO:
                game.level4.player[i].boxRender(quadrant[i].center.x, 
                        quadrant[i].center.y, quadrant[i].width);
                break;
            case MAINMENU:
                break;
            case PAUSE:
                break;
            case ERICK:
                break;
            case LEVELSEL:
                break;
            case ROBERT:
                break;
            case ZACK:
                break;
            default:
                break;
        }

        //render # of icons to indicate life count
        //else render one icon " x [Number of lives]"


        //if player health < 50%, display white numbers
        //else display transparent value percentage up to 200%
        //0%	white
        //100%	33% transparancey red over white
        //150%	66% transparancey red over white
        //200%	solid Red
    }
}

void Player::boxRender(int centx, int centy, int width)
{

    float radius = width -75;
    float x = centx - 50;
    float y = centy - 50;
    int triangleNum = 25;
    float twicePI = 2.0 * M_PI;
    Rect r; 
    glEnable(GL_TEXTURE_2D);
    r.bot = y + width;
    r.left = x + 42;
    r.center = x; 
    char temp[15]; 
    for (int i=0; i<15; i++) {
        temp[i] = st[index][i]; 
    }
    ggprint40(&r,0,0x0ff0000,"%s", temp); 
    r.bot = y - width/2;
    r.left = x + width;
    r.center = x; 
    switch (game.render) {
        case STARYNIGHT:
            ggprint40(&r, 0, 0x0ff0000, "%i", 
                    game.level3.player[index].status.lifeCount);
            break;
        case FIELD:
            ggprint40(&r, 0, 0x0ff0000, "%i", 
                    game.level2.player[index].status.lifeCount);
            break;
        case DISCO:
            ggprint40(&r, 0, 0x0ff0000, "%i", 
                    game.level4.player[index].status.lifeCount);
            break;
        case MAINMENU:
            break;
        case PAUSE:
            break;
        case ERICK:
            break;
        case LEVELSEL:
            break;
        case ROBERT:
            break;
        case ZACK:
            break;
        default:
            break;
    }
    r.bot = y + width/2;
    r.left = x + width;
    r.center = x; 
    switch (game.render) {
        case STARYNIGHT:
            ggprint40(&r, 0, 0x0ff0000, "%1.1f", 
                    game.level3.player[index].multiplier*10);
            break;
        case FIELD:
            ggprint40(&r, 0, 0x0ff0000, "%1.1f", 
                    game.level2.player[index].multiplier*10);
            break;
        case DISCO:
            ggprint40(&r, 0, 0x0ff0000, "%1.1f", 
                    game.level4.player[index].multiplier*10);
            break;
        case MAINMENU:
            break;
        case PAUSE:
            break;
        case ERICK:
            break;
        case LEVELSEL:
            break;
        case ROBERT:
            break;
        case ZACK:
            break;
        default:
            break;
    }

    glDisable(GL_TEXTURE_2D);

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
    x1 = x;
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
    glColor3ub(color[0],color[1],color[2]);
    glVertex2f(x1,y1);
    for (int i = 0; i < 120; i++) {
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
    x2 = x + 3.5 * radius;
    y2 = y + 2 * radius;
    glVertex2f(x2,y2);
    for (int i = 0; i < 120; i++) {
        glVertex2f(x2 + (radius * cos(i * twicePI / triangleNum)),
                y2 + (radius * sin(i * twicePI / triangleNum)));
    }
    glEnd();
    glDisable(GL_BLEND);


}

//when main menu is called after quitting, the following function resets 
//all parameters.
void resetMain(Game *game) 
{
#ifdef USE_OPENAL_SOUND
    //    endSound();
    //    loadSound("./audio/test.wav");
#endif
    game->mainMenu.titleVel.y = rnd() * 0.5 - 0.25;
    game->mainMenu.titleBox.center.y = 
        game->WINDOW_HEIGHT + game->WINDOW_HEIGHT/3;
    game->mainMenu.titleBox.center.x = game->WINDOW_WIDTH/2;
#ifdef USE_OPENAL_SOUND
    for (int i = 0; i < TOTALSOUNDS; i++) {
        alSourceStop(alSource[i]);
    }
    titledrop = 0;
    pitch = 3.0f;
#endif
    game->render = MAINMENU;
}

//incorporate this in zack's level handler functions
void Level::deathCheck(Player *player)
{
    if ((player->body.center.x < (-scrn->width/4 * 3)) && player->status.lifeState == ALIVE) {
        //call death animation on the left edge @ player->body.center.y
        //if lives available, respawn
        //Pass (x,y) to render particles at
        player->deathInit(-scrn->width/2, player->body.center.x);
        player->multiplier=0;
        player->status.lifeState = DEAD;
    }
    else if (player->body.center.x 
            > ((scrn->width + scrn->width/4 * 3)) && player->status.lifeState == ALIVE) {
        //call death animation on the right edge @ player->body.center.y
        //if lives available, respawn
        //Pass (x,y) to render particles at
        player->deathInit(scrn->width + scrn->width/2,
        		player->body.center.y);
        player->multiplier=0;
        player->status.lifeState = DEAD;
    }
    else if ((player->body.center.y < (-scrn->height/4 * 3)) && player->status.lifeState == ALIVE) {
        //call death animation on the Top edge @ player->body.center.x
        //if lives available, respawn
        //Pass (x,y) to render particles at
        player->deathInit(player->body.center.x, 
        		-scrn->height/2);
        player->multiplier=0;
        player->status.lifeState = DEAD;
    }
    else if ((player->body.center.y 
            > (scrn->height + scrn->height/4 * 3)) && player->status.lifeState == ALIVE) {
        //call death animation on the top edge @ player->body.center.x
        //if lives available, respawn
        //Pass (x,y) to render particles at
        player->deathInit(player->body.center.x, 
        		scrn->height + scrn->height/2);
        player->multiplier=0;
        player->status.lifeState = DEAD;
    }
}

//50 particles to work with
void Player::deathInit(int x, int y) {
    Color colors;
    for (int i = 0; i < 50; i++) {
	for (int j = 0; j < 3; j++) {
		bloodStream[i].color[j] = colors.bloodred[j];
	}
	bloodStream[i].s.center.x = x;
	bloodStream[i].s.center.y = y;
	bloodStream[i].velocity.x = -delta.x + 20 * ((float)rand() / (float)RAND_MAX);
	bloodStream[i].velocity.y = -delta.y + 20 * ((float)rand() / (float)RAND_MAX);; 
    }
}

void Player::deathPhysics() {
    for (int i = 0; i < 50; i++) {
	bloodStream[i].velocity.y -= GRAVITY * 3;
	bloodStream[i].s.center.x += bloodStream[i].velocity.x;
	bloodStream[i].s.center.y += bloodStream[i].velocity.y;
	cout << bloodStream[i].s.center.y << ", " << bloodStream[i].s.center.x << endl;
    }
}

void Player::deathRender() {
    for (int i = 0; i < 50; i++) {
	glPushMatrix();
	Vec *c = &bloodStream[i].s.center;
	glTranslatef(bloodStream[i].s.center.x, bloodStream[i].s.center.y, bloodStream[i].s.center.z);
	int w = 10;
	int h = 10;
	glBegin(GL_QUADS);
	glColor3ub(0, 0, 0);
		glVertex2i(c->x-w-3, c->y-h-3);
		glVertex2i(c->x-w-3, c->y+h+3);
		glVertex2i(c->x+w+3, c->y+h+3);
		glVertex2i(c->x+w+3, c->y-h-3);

	glColor3ub(bloodStream[i].color[0], bloodStream[i].color[1], bloodStream[i].color[2]);
		glVertex2i(c->x-w, c->y-h);
		glVertex2i(c->x-w, c->y+h);
		glVertex2i(c->x+w, c->y+h);
		glVertex2i(c->x+w, c->y-h);
	glEnd();
	glPopMatrix();
    }
}

void Level::respawn(Player *player)
{
    //wait 5 seconds

    player->status.lifeState = ALIVE;
    player->action = PASSIVE;
    --player->status.lifeCount;
    player->body.center.x = scrn->width/2;
    player->body.center.y = scrn->height;
    player->delta.x = 0.0;
    player->delta.y = 0.0;
    player->jumpCount = 0;
    player->status.health = 0;
}

double Player::timeDiff(struct timespec *start, struct timespec *end)
{
    double oobillion = 1.0/1e9;
    return (double)(end->tv_sec - start->tv_sec ) +
        (double)(end->tv_nsec - start->tv_nsec) * oobillion;
}

void Player::attack()
{
    if (direction == LEFT) {
        weapon.center.x -= weapon.width/2;
    }
    else if(direction == RIGHT) {
        weapon.center.x += weapon.width/2;
    }
    if (game.render == FIELD) {
        game.level2.Lattack(index);
    } else if (game.render == STARYNIGHT) {
        game.level3.Lattack(index);
    } else if (game.render == DISCO) {
        game.level4.Lattack(index);
    }

}

void Level::Lattack(int index)
{
    for (int i=0; i<4; i++) {
        if (player[index].index == i) {}
        else {
            if (player[index].direction == LEFT) {
                //if (player[index].weapon.center.x - i
                //player[index].weapon.width/2 
                //        <= player[i].body.center.x + player[i].body.width) {
                //    player[i].body.center.x -= 50;
                //}
                float distance = pow((player[index].weapon.center.x - 
                            player[index].weapon.width/2) - 
                        player[i].body.center.x, 2) + 
                    pow((player[index].weapon.center.y - 
                                player[index].weapon.height/2) - 
                            player[i].body.center.y, 2);

                distance = sqrt(distance);
                if (distance < player[i].body.radius){
                    //player[i].body.center.x -= player[i].multiplier * 50;
                    player[i].delta.x -= player[i].multiplier*5;
                    player[i].delta.y += player[i].multiplier*2;
                    player[i].multiplier += 0.25;
                }
            }
            else if (player[index].direction == RIGHT) {
                //if (player[index].weapon.center.x + 
                //player[index].weapon.width/2
                //	   >= player[i].body.center.x - player[i].body.width) {
                //player[i].body.center.x += 50;
                //}
                float distance = pow((player[index].weapon.center.x + 
                            player[index].weapon.width/2) - 
                        player[i].body.center.x,2) +
                    pow((player[index].weapon.center.y + 
                                player[index].weapon.height/2) - 
                            player[i].body.center.y, 2);
                distance = sqrt(distance);
                if (distance < player[i].body.radius) {
                    //player[i].body.center.x += player[i].multiplier * 50;
                    player[i].delta.x += player[i].multiplier*5;
                    player[i].delta.y += player[i].multiplier*2;
                    player[i].multiplier += 0.25;
                }
            }
        }
    }

}





