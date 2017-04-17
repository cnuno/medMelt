/*
Author: Robert Pierucci
Progress Log
03/23/2017
I added level structure

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
 */

#include "headers.h"
#include "robertP.h"
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <iostream>
#include </usr/include/AL/alut.h>
#define GRAVITY .1
#define MAX_PARTICLE 50
#define x_rnd() (rand() % 11) - 5 
#define y_rnd() (rand() % 11)  

extern Screen* scrn;
using namespace std;

extern void resetMain(Game *game); 
extern ALuint alSource;
extern ALuint alBuffer;
extern int titledrop;
extern float pitch;
extern Ppmimage *dyna;
extern Ppmimage *dyna1; 
extern Ppmimage *dyna2; 
extern Ppmimage *medMeltTitle; 
extern Ppmimage *pillIcon; 
extern Ppmimage *playIcon; 
extern Ppmimage *optionsIcon; 
extern Ppmimage *exitIcon; 
extern Ppmimage *dyna; 
extern Ppmimage *dyna1; 
extern Ppmimage *resumeIcon; 
extern Ppmimage *resumeSelIcon; 
extern Ppmimage *quitIcon; 
extern Ppmimage *quitSelIcon;
extern Ppmimage *fieldIcon;
extern Ppmimage *staryIcon;
extern GLuint silTitle; 
extern GLuint silPill; 
extern GLuint silPlay; 
extern GLuint silOptions; 
extern GLuint silOptionsSel; 
extern GLuint silExit; 
extern GLuint medMeltTexture; 
extern GLuint pillTexture; 
extern GLuint playTexture; 
extern GLuint optionsTexture; 
extern GLuint exitTexture; 
extern GLuint resumeTexture; 
extern GLuint resumeSelTexture; 
extern GLuint quitTexture; 
extern GLuint quitSelTexture;
extern GLuint fieldTexture;
extern GLuint staryTexture;
extern unsigned char *buildAlphaData(Ppmimage *img);
extern ALuint alSource;  
extern ALuint alBuffer;

extern Ppmimage *etIcon;
extern GLuint etTexture; 

Game::~Game()
{
	system("rm -Rf images/");
	system("rm -Rf audio/");
}

void loadSound(const char path[20])
{
	alutInit(0, NULL);
	if (alGetError() != AL_NO_ERROR) {
		printf("ERROR: alutInit()\n");
	}
	alGetError();
	float vec[6] = {0.0f,0.0f,1.0f, 0.0f,1.0f,0.0f};
	alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
	alListenerfv(AL_ORIENTATION, vec);
	alListenerf(AL_GAIN, 1.0f);
	alBuffer = alutCreateBufferFromFile(path);
	alGenSources(1, &alSource);
	if (alGetError() != AL_NO_ERROR) {
		printf("ERROR: setting source\n");
	}
}

//playSound parameters
//1st: float pitch
//2nd: bool - true or false for looping
void playSound(float pitch, bool loop)
{
	alSourcei(alSource, AL_BUFFER, alBuffer);
	alSourcef(alSource, AL_GAIN, 1.0f);
	alSourcef(alSource, AL_PITCH, pitch);
	if (loop == true) {
		alSourcei(alSource, AL_LOOPING, AL_TRUE);
	} else {
		alSourcei(alSource, AL_LOOPING, AL_FALSE);
	}
	alSourcePlay(alSource);
}

void endSound() {
	alDeleteSources(1, &alSource);
	alDeleteBuffers(1, &alBuffer);
}

void closeSoundDevice() {
	ALCcontext *Context = alcGetCurrentContext();
	ALCdevice *Device = alcGetContextsDevice(Context);
	alcMakeContextCurrent(NULL);
	alcDestroyContext(Context);
	alcCloseDevice(Device);
	return;
}

/*
   process images in images/numbers in the function below
   1. create variables in medMelt.cpp
   2. extern those variables at top of this file
   3. follow procedure below
 */

void loadImages()
{
	//Need to figure out how to delete /images/ and /audio/ if
	//game crashes

	//extract .tar.gz
	system("tar -xvzf resources.tar.gz");
	system("clear");

	//====LOAD IMAGES========
	system("convert ./images/title.png ./images/title.ppm");
	system("convert ./images/pill.png ./images/pill.ppm");
	system("convert ./images/play_icon.png ./images/play.ppm");
	system("convert ./images/option_icon.png ./images/options.ppm");
	system("convert ./images/exit_icon.png ./images/exit.ppm");
	system("convert ./images/resume.png ./images/resume.ppm");
	system("convert ./images/quit.png ./images/quit.ppm");
	system("convert ./images/resume_select.png ./images/resume_select.ppm");
	system("convert ./images/quit_select.png ./images/quit_select.ppm");
	system("convert ./images/starynight.png ./images/stary.ppm");
	system("convert ./images/field.png ./images/field.ppm");
	system("convert ./images/et.png ./images/et.ppm");
	medMeltTitle = ppm6GetImage("./images/title.ppm");
	pillIcon = ppm6GetImage("./images/pill.ppm");
	playIcon = ppm6GetImage("./images/play.ppm");
	optionsIcon = ppm6GetImage("./images/options.ppm");
	exitIcon = ppm6GetImage("./images/exit.ppm");
	resumeIcon = ppm6GetImage("./images/resume.ppm");
	quitIcon = ppm6GetImage("./images/quit.ppm");
	quitSelIcon = ppm6GetImage("./images/quit_select.ppm");
	resumeSelIcon = ppm6GetImage("./images/resume_select.ppm");
	fieldIcon = ppm6GetImage("./images/field.ppm");
	staryIcon = ppm6GetImage("./images/stary.ppm");
	etIcon = ppm6GetImage("./images/et.ppm");

	dyna = playIcon;
	dyna1 = resumeIcon;
	dyna2 = fieldIcon;
	//Texture
	glGenTextures(1, &medMeltTexture);
	glGenTextures(1, &pillTexture);
	glGenTextures(1, &playTexture);
	glGenTextures(1, &optionsTexture);
	glGenTextures(1, &exitTexture);
	glGenTextures(1, &resumeTexture);
	glGenTextures(1, &quitTexture);
	glGenTextures(1, &resumeSelTexture);
	glGenTextures(1, &quitSelTexture);
	glGenTextures(1, &fieldTexture);
	glGenTextures(1, &staryTexture);
	glGenTextures(1, &etTexture);

	//Title=============================================================================
	int w = medMeltTitle->width;
	int h = medMeltTitle->height;
	//
	glBindTexture(GL_TEXTURE_2D, medMeltTexture);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, medMeltTitle->data);
	//silhouette
	glBindTexture(GL_TEXTURE_2D, silTitle);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	//
	unsigned char *silData = buildAlphaData(medMeltTitle);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE,silData);
	free(silData);

	//Pill Icon=========================================================================
	glBindTexture(GL_TEXTURE_2D, pillTexture);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);

	w = pillIcon->width; 
	h = pillIcon->height; 

	unsigned char *pilData = buildAlphaData(pillIcon);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE,pilData);
	free(pilData);

	//Play Icon=========================================================================
	glBindTexture(GL_TEXTURE_2D, playTexture);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);

	w = playIcon->width; 
	h = playIcon->height; 

	unsigned char *plData = buildAlphaData(playIcon);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE,plData);
	free(plData);

	//Resume Icon========================================================================
	glBindTexture(GL_TEXTURE_2D, resumeTexture);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);

	w = resumeIcon->width; 
	h = resumeIcon->height; 

	unsigned char *reData = buildAlphaData(resumeIcon);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, reData);
	free(reData);

	//Resume Select Icon=================================================================
	glBindTexture(GL_TEXTURE_2D, resumeSelTexture);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);

	w = resumeSelIcon->width; 
	h = resumeSelIcon->height; 

	unsigned char *reSelData = buildAlphaData(resumeSelIcon);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, reSelData);
	free(reSelData);

	//Quit Icon=========================================================================
	glBindTexture(GL_TEXTURE_2D, quitTexture);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);

	w = quitIcon->width; 
	h = quitIcon->height; 

	unsigned char *qtData = buildAlphaData(quitIcon);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE,qtData);
	free(qtData);

	//Quit Select Icon=======++========================================================
	glBindTexture(GL_TEXTURE_2D, quitSelTexture);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);

	w = quitSelIcon->width; 
	h = quitSelIcon->height; 

	unsigned char *qtSelData = buildAlphaData(quitSelIcon);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE,qtSelData);
	free(qtSelData);

	//Options Icon=========================================================================
	glBindTexture(GL_TEXTURE_2D, optionsTexture);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);

	w = optionsIcon->width; 
	h = optionsIcon->height; 

	unsigned char *opData = buildAlphaData(optionsIcon);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE,opData);
	free(opData);

	//Exit Icon=========================================================================
	glBindTexture(GL_TEXTURE_2D, exitTexture);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);

	w = exitIcon->width; 
	h = exitIcon->height; 

	unsigned char *exData = buildAlphaData(exitIcon);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE,exData);
	free(exData);

	//Field Icon===================================================================
	glBindTexture(GL_TEXTURE_2D, fieldTexture);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);

	w = fieldIcon->width; 
	h = fieldIcon->height; 

	unsigned char *fieldData = buildAlphaData(fieldIcon);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE,fieldData);
	free(fieldData);

	//Starynight Icon===========================================================
	glBindTexture(GL_TEXTURE_2D, staryTexture);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);

	w = staryIcon->width; 
	h = staryIcon->height; 

	unsigned char *staryData = buildAlphaData(staryIcon);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE,staryData);
	free(staryData);

	//ET Icon===========================================================
	glBindTexture(GL_TEXTURE_2D, etTexture);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);

	w = etIcon->width; 
	h = etIcon->height; 

	unsigned char *etData = buildAlphaData(etIcon);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE,etData);
	free(etData);

}

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

		/*
		   if (game->level.player[i].status.lifeCount <= 5) {
		//render # of icons to indicate life count
		} else {
		//else render one icon " x [Number of lives]"
		}
		 */

		//if player health < 50%, display white numbers
		//else display transparent value percentage up to 200%
		//0%	white
		//100%	33% transparancey red over white
		//150%	66% transparancey red over white
		//200%	solid Red
	}
}

//when main menu is called after quitting, the following function resets 
//all parameters.
void resetMain(Game *game) 
{
	endSound();
	loadSound("./audio/test.wav");
	game->mainMenu.titleVel.y = rnd() * 0.5 - 0.25;
	game->mainMenu.titleBox.center.y = game->WINDOW_HEIGHT + game->WINDOW_HEIGHT/3;
	game->mainMenu.titleBox.center.x = game->WINDOW_WIDTH/2;
	titledrop = 0;
	pitch = 3.0f;
	game->render = MAINMENU;
}

//incorporate this in zack's level handler functions
void Level::deathCheck(Player *player)
{
	if (player->body.center.x < (-scrn->width/4 * 3)) {
		//call death animation on the left edge @ player->body.center.y
		//if lives available, respawn
		//Pass (x,y) to render particles at
		deathAnimation(-scrn->width/2, player->body.center.x);
		player->status.lifeState = DEAD;
	}
	else if (player->body.center.x 
			> (scrn->width + scrn->width/4 * 3)) {
		//call death animation on the right edge @ player->body.center.y
		//if lives available, respawn
		//Pass (x,y) to render particles at
		deathAnimation(scrn->width + scrn->width/2,
				player->body.center.y);
		player->status.lifeState = DEAD;
	}
	else if (player->body.center.y < (-scrn->height/4 * 3)) {
		//call death animation on the Top edge @ player->body.center.x
		//if lives available, respawn
		//Pass (x,y) to render particles at
		deathAnimation(player->body.center.x, 
				-scrn->height/2);
		player->status.lifeState = DEAD;
	}
	else if (player->body.center.y 
			> (scrn->height + scrn->height/4 * 3)) {
		//call death animation on the top edge @ player->body.center.x
		//if lives available, respawn
		//Pass (x,y) to render particles at
		deathAnimation(player->body.center.x, 
				scrn->height + scrn->height/2);
		player->status.lifeState = DEAD;
	}

	//Respawn Handling
	if (player->status.lifeState == DEAD 
			&& player->status.lifeCount > 0) {
		//There are lives available
		respawn(player);
	}
	else if (player->status.lifeState == DEAD 
			&& player->status.lifeCount < 0) {
		player->body.center.x = -scrn->width;
		player->body.center.y = -scrn->height;
		player->delta.x = 0.0;
		player->delta.y = 0.0;
		//No lives left
		//Draw a red 'X' over status box
	}
}

//50 particles to work with
void deathAnimation(int x, int y) {
}

void deathPhysics() {
	//conduct physics for particles here
}

void deathRender() {
	//render particles here
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
