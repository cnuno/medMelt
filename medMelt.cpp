/*
Progress Log
	April 16th,2017
	Today we worked as a team to finish the respawn functions. We added color
	to each individual player. Now we need to pass these parameters from our
	(not yet done) character selection screen. We also fixed a slight bug
	in Erick's physics functions (including the collision). We edited our code
   	to fall in allignment with K&R style. 

	April 7th, 2017
	Today we worked as a team to accomplsh the task of getting our pause menu
	working. We researched how to clear just one image but we learned that it 
	is better to just redraw the image. We achieved our goal and got the pause
	menu to render transparently. We also worked on geting our controller to 
	connect without issues. Also we logged all the controller values to 
	controller.log. We spent aproximately 4 hours and 30 minutes. 
*/

#include "headers.h"
using namespace std;

#define GRAVITY .1
#define rnd() (float)rand() / (float)RAND_MAX
Display *dpy;
Window win;
GLXContext glc;
Display* disp = XOpenDisplay(NULL);
Screen* scrn = DefaultScreenOfDisplay(disp);
Ppmimage *medMeltTitle = NULL;
Ppmimage *pillIcon = NULL;
Ppmimage *playIcon = NULL;
Ppmimage *optionsIcon = NULL;
Ppmimage *exitIcon = NULL;
Ppmimage *dyna = NULL;
Ppmimage *dyna1 = NULL;
Ppmimage *dyna2 = NULL;
Ppmimage *resumeIcon = NULL;
Ppmimage *resumeSelIcon = NULL;
Ppmimage *quitIcon = NULL;
Ppmimage *quitSelIcon = NULL;
Ppmimage *fieldIcon = NULL;
Ppmimage *staryIcon = NULL;
Ppmimage *etIcon = NULL;
Ppmimage *octIcon = NULL;
Ppmimage *troll2 = NULL;
Ppmimage *troll3 = NULL;
Ppmimage *troll4 = NULL;
GLuint silTitle;
GLuint silPill;
GLuint silPlay;
GLuint silOptions;
GLuint silOptionsSel;
GLuint silExit;
GLuint medMeltTexture;
GLuint pillTexture;
GLuint playTexture;
GLuint optionsTexture;
GLuint exitTexture;
GLuint resumeTexture;
GLuint resumeSelTexture;
GLuint quitTexture;
GLuint quitSelTexture;
GLuint fieldTexture;
GLuint staryTexture;
GLuint etTexture;
GLuint octTexture;
GLuint troll2Tex;
GLuint troll3Tex;
GLuint troll4Tex;

#ifdef USE_OPENAL_SOUND
//Sound variables
#define SOUNDCOUNT 20
ALuint alSource[SOUNDCOUNT];
ALuint alBuffer[SOUNDCOUNT];
#endif

#ifdef USE_OPENAL_SOUND
int titledrop = 0;
float pitch = 1.0f;
#endif
int done=0;

Joystick joystick1("/dev/input/js0");

void setTitle(void);
void loadImages();
unsigned char *buildAlphaData(Ppmimage *img);
void check_mouse(XEvent *e, Game *game);
//int check_keys(XEvent *e, Game *game);
void init_opengl();
void initXWindows();
void cleanupXWindows();
void setTitle();
void render_switch(Game *game);
void menu_move(Joystick *joystick, Game *game);
void resetMain(Game *game);

Game::Game() {
	WINDOW_WIDTH = scrn->width;
	WINDOW_HEIGHT = scrn->height;

	//Not sure what do
	WINDOW_RATIO = WINDOW_WIDTH / WINDOW_HEIGHT;
	level1.xScale = floor(WINDOW_WIDTH / WINDOW_RATIO * 50);
	level1.yScale = floor(WINDOW_HEIGHT / WINDOW_RATIO * 50);
	//IDK
	level1.pill_count = 0;

	mouse[0] = 0;
	mouse[1] = 0;
	render = MAINMENU;
	mainMenu.titleVel.y = rnd() * 0.5 - 0.25;
	mainMenu.titleBox.center.x = WINDOW_WIDTH/2;
	mainMenu.titleBox.center.y = WINDOW_HEIGHT + WINDOW_HEIGHT/3;
	mainMenu.pillBox.center.x = WINDOW_WIDTH/2;
	mainMenu.pillBox.center.y = WINDOW_HEIGHT - WINDOW_HEIGHT/3.5;
	mainMenu.playBox.center.x = WINDOW_WIDTH/2;
	mainMenu.playBox.center.y = WINDOW_HEIGHT/3;
	mainMenu.optionBox.center.x = WINDOW_WIDTH/2;
	mainMenu.optionBox.center.y = WINDOW_HEIGHT/4.2;
	mainMenu.exitBox.center.x = WINDOW_WIDTH/2;
	mainMenu.exitBox.center.y = WINDOW_HEIGHT/7;
	mainMenu.selector.center.x = mainMenu.playBox.center.x;
	mainMenu.selector.center.y = mainMenu.playBox.center.y;
	pauseMenu.rbox.center.x = WINDOW_WIDTH/2;
	pauseMenu.rbox.center.y = WINDOW_HEIGHT - WINDOW_HEIGHT/6;
	pauseMenu.obox.center.x = WINDOW_WIDTH/2;
	pauseMenu.obox.center.y = WINDOW_HEIGHT - WINDOW_HEIGHT/3;
	pauseMenu.qbox.center.x = WINDOW_WIDTH/2;
	pauseMenu.qbox.center.y = WINDOW_HEIGHT - WINDOW_HEIGHT/1.5;
	pauseMenu.selector.center.x = pauseMenu.rbox.center.x;
	pauseMenu.selector.center.y = pauseMenu.rbox.center.y;
	levelMenu.level1.center.x = WINDOW_WIDTH/4;
	levelMenu.level1.center.y = WINDOW_HEIGHT/2;
	levelMenu.level2.center.x = 3 * (WINDOW_WIDTH/4);
	levelMenu.level2.center.y = WINDOW_HEIGHT/2;
	levelMenu.selector.center.x = levelMenu.level1.center.x;
	levelMenu.selector.center.y = levelMenu.level1.center.y;
	levelMenu.textBox.center.x = WINDOW_WIDTH/2;
	levelMenu.textBox.center.y = 3 * (WINDOW_HEIGHT/4);
	prevState = MAINMENU;
};

Game game;

int main() 
{
	initXWindows();
	init_opengl();
	srand(time(NULL));
#ifdef USE_OPENAL_SOUND
	initialize_sounds();
#endif
	//start animation
	while (!done) {
		while (XPending(dpy)) {
			XEvent e;
			XNextEvent(dpy, &e);
			check_mouse(&e, &game);
		    check_keys(&e, &game);
		}

		if (game.render==MAINMENU || game.render==OVER) {
			menu_move(&joystick1, &game);
		}
		else if (game.render==LEVELSEL) {
			levelsel_move(&joystick1, &game);
		}
		if (endcheck()){
		    game.prevState = game.render;
		    game.render = OVER;
		}
		render_switch(&game); 
		glXSwapBuffers(dpy, win);
	}
#ifdef USE_OPENAL_SOUND
	cleanup_sounds();
#endif
	cleanupXWindows();
	return 0;
}

void render_switch(Game *game)
{
	switch(game->render)
	{
		case MAINMENU:
			menu(game);
			break;
		case PAUSE:
			cpause(game);
			break;
		case ERICK:
			erick(game);
			break;
		case LEVELSEL:
			levelsel(game);
			break;
		case ROBERT:
			break;
		case ZACK:
			break;
		case FIELD:
			field(game); 
			break;
		case STARYNIGHT:
			starynight(game);
			break;
		case DISCO:
			disco(game); 
			break;
		case OVER:
			end_game();
		default:
			break;
	}
}

void init_opengl(void)
{
	//OpenGL initialization
	glViewport(0, 0, game.WINDOW_WIDTH, game.WINDOW_HEIGHT);
	//Initialize matrices
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	//Set 2D mode (no perspective)
	glOrtho(-game.WINDOW_WIDTH/2, 1.5 * game.WINDOW_WIDTH, 
			-game.WINDOW_HEIGHT/2, 1.5*game.WINDOW_HEIGHT, -1, 1);
	//glOrtho(0, game.WINDOW_WIDTH, 0, game.WINDOW_HEIGHT, -1, 1);
	//Set the screen background color
	glClearColor(0.1, 0.1, 0.1, 0.0);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	initialize_fonts();

	loadImages();
}

void initXWindows(void)
{
	//do not change
	GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
	int w=game.WINDOW_WIDTH, h=game.WINDOW_HEIGHT;
	dpy = XOpenDisplay(NULL);
	if (dpy == NULL) {
		std::cout << "\n\tcannot connect to X server\n" << std::endl;
		exit(EXIT_FAILURE);
	}
	Window root = DefaultRootWindow(dpy);
	XVisualInfo *vi = glXChooseVisual(dpy, 0, att);
	if (vi == NULL) {
		std::cout << "\n\tno appropriate visual found\n" << std::endl;
		exit(EXIT_FAILURE);
	} 
	Colormap cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
	XSetWindowAttributes swa;
	swa.colormap = cmap;
	swa.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask |
		ButtonPress | ButtonReleaseMask | PointerMotionMask |
		StructureNotifyMask | SubstructureNotifyMask;
	win = XCreateWindow(dpy, root, 0, 0, w, h, 0, vi->depth,
			InputOutput, vi->visual, CWColormap | CWEventMask, &swa);
	setTitle();
	glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
	glXMakeCurrent(dpy, win, glc);
}

void cleanupXWindows(void)
{
	XDestroyWindow(dpy, win);
	XCloseDisplay(dpy);
}

void setTitle(void)
{
	//Set the window title bar.
	XMapWindow(dpy, win);
	XStoreName(dpy, win, "MEDICAL MELTDOWN");
}

//Author: Gordon Griesel
unsigned char *buildAlphaData(Ppmimage *img)
{
	int i;
	int a,b,c;
	unsigned char *newdata, *ptr;
	unsigned char *data = (unsigned char *)img->data;
	newdata = (unsigned char *)malloc(img->width * img->height * 4);
	ptr = newdata;
	for (i=0; i<img->width * img->height * 3; i+=3) {
		a = *(data+0);
		b = *(data+1);
		c = *(data+2);
		*(ptr+0) = a;
		*(ptr+1) = b;
		*(ptr+2) = c;
		*(ptr+3) = (a|b|c);
		ptr += 4;
		data += 3;
	}
	return newdata;
}
//end

void check_mouse(XEvent *e, Game *game)

{
	static int savex = 0;
	static int savey = 0;

	if (e->type == ButtonRelease) {
		return;
	}
	if (e->type == ButtonPress) {
		if (e->xbutton.button==1) {
			//left button
			return;
		}
		if (e->xbutton.button==3) {
			//Right button was pressed
			return;
		}
	}

	//Did the mouse move?
	if (savex != e->xbutton.x || savey != e->xbutton.y) {
		savex = e->xbutton.x;
		savey = e->xbutton.y;
		int y = game->WINDOW_HEIGHT - e->xbutton.y;

		//Store mouse position
		game->mouse[0] = savex;
		game->mouse[1] = y;
	}
}
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
	system("convert ./images/octopus.png ./images/octopus.ppm");
	system("convert ./images/troll2.png ./images/troll2.ppm");
	system("convert ./images/troll3.png ./images/troll3.ppm");
	system("convert ./images/troll4.png ./images/troll4.ppm");
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
	octIcon = ppm6GetImage("./images/octopus.ppm");
	troll2 = ppm6GetImage("./images/troll2.ppm");
	troll3 = ppm6GetImage("./images/troll3.ppm");
	troll4 = ppm6GetImage("./images/troll4.ppm");

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
	glGenTextures(1, &octTexture);
	glGenTextures(1, &troll2Tex);
	glGenTextures(1, &troll3Tex);
	glGenTextures(1, &troll4Tex);

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

	//Octopus Icon===========================================================
	glBindTexture(GL_TEXTURE_2D, octTexture);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);

	w = octIcon->width; 
	h = octIcon->height; 

	unsigned char *octData = buildAlphaData(octIcon);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE,octData);
	free(octData);
}
/*int check_keys(XEvent *e, Game *game)
{
	//Was there input from the keyboard?
	if (e->type == KeyPress) {
		int key = XLookupKeysym(&e->xkey, 0);

		//You may check keys here.
		switch (key) {
			case XK_b:
				game->render = STARYNIGHT; 
				break;
			case XK_c:
				game->render = FIELD; 
				break;
			case XK_v:
				game->render = BAKERSFIELD; 
				break;
			case XK_Escape:
				return 1;
			default:
				break;
		}
	}
	
	   case XK_Left:
	   case XK_a:
	   switch (game->render) {
	   case ZACK:
	   if (game->level2.player1.action != DASH) {
	   game->level2.player1.direction = LEFT;
	   game->level2.player1.action = MOVE;
	   game->level2.player1.delta.x = -5.0f; 
	   if (game->level2.player1.collision(&game->level2.platform[0])) {
	   game->level2.player1.delta.y = 1.25f; 
	   }
	   }
	   break;
	   case ROBERT:
	   break;
	   default:
	   break;
	   }
	   break;
	   case XK_b:
	//load Starynight
	game->render = STARYNIGHT; 
	break;
	case XK_c:
	game->render = CESAR;
	break;
	case XK_Right:
	case XK_d:
	switch (game->render) {
	case ZACK:
	if (game->level2.player1.action != DASH) {
	game->level2.player1.direction = RIGHT;
	if (game->level2.player1.direction != LEFT) {
	game->level2.player1.action = MOVE;
	game->level2.player1.delta.x = 5.0f;   
	}
	if (game->level2.player1.collision(&game->level2.platform[0])) {
	game->level2.player1.delta.y = 1.25f; 
	}
	break;
	}
	default:
	break;
	}
	break;
	case XK_e:
	if (game->render != ERICK) 
	game->render = ERICK;
	break;
	case XK_f:
	break;
	case XK_g:
	break;
	case XK_h:
	break;
	case XK_i:
	switch (game->render) {
	case ZACK:
	if (game->level2.player2.jumpCount < game->level2.player2.JUMP_MAX && game->level2.player2.action != GROUNDPOUND && game->level2.player2.action != DASH) {
	game->level2.player2.delta.y = 5.0f;
	game->level2.player2.jumpCount++;
	}	
	break;
	default :
	break;
	}

	break;
	case XK_j:
	switch (game->render) {
case ZACK:
if (game->level2.player2.action != DASH) {
	game->level2.player2.direction = LEFT;
	game->level2.player2.action = MOVE;
	game->level2.player2.delta.x = -5.0f; 
	if (game->level2.player2.collision(&game->level2.platform[0])) {
		game->level2.player2.delta.y = 1.25f; 
	}
}
break;
default:
break;
}
break;
break;
case XK_k:
switch (game->render) {
	case ZACK:
		//if in the air
		if (game->level2.player2.action != MOVE && game->level2.player2.action != GROUNDPOUND && game->level2.player2.action != DASH && !game->level2.player2.collision(&game->level2.platform[0])) {
			game->level2.player2.action = GROUNDPOUND;
			game->level2.player2.delta.x = 0.0f; 
			usleep(300000);
			game->level2.player2.delta.y = -20.0f; 
		}
		break;
	default:
		break;
}

break;
case XK_l:
switch (game->render) {
	case ZACK:
		if (game->level2.player2.action != DASH) {
			game->level2.player2.direction = RIGHT;
			game->level2.player2.action = MOVE;
			game->level2.player2.delta.x = 5.0f;   
			if (game->level2.player2.collision(&game->level2.platform[0])) {
				game->level2.player2.delta.y = 1.25f; 
			}
			break;
		}
	default:
		break;
}
break;
case XK_m:
//reinitialize title and sounds
if (game->render != MAINMENU) {
	resetMain(game);
}
break;
case XK_n:
break;
case XK_o:
//Make pills
makePill(game, game->mouse[0], game->mouse[1]);
break;
case XK_p:
if (game->render != PAUSE && game->render != MAINMENU) { 
	game->prevState = game->render;
	game->render = PAUSE;
}
else if (game->render == PAUSE) { 
	game->render = game->prevState;
}	   
break;
case XK_q:
break;
case XK_r:
game->render = ROBERT;
break;
case XK_Down:
case XK_s:
switch (game->render) {
	case MAINMENU:
		if (dyna == playIcon) {
			dyna = optionsIcon;
			game->mainMenu.selector.center.x = game->mainMenu.optionBox.center.x;
			game->mainMenu.selector.center.y = game->mainMenu.optionBox.center.y;
		}
		else if (dyna == optionsIcon) {
			dyna = exitIcon;
			game->mainMenu.selector.center.x = game->mainMenu.exitBox.center.x;
			game->mainMenu.selector.center.y = game->mainMenu.exitBox.center.y;
		}
		else if (dyna == exitIcon) {
			dyna = playIcon;
			game->mainMenu.selector.center.x = game->mainMenu.playBox.center.x;
			game->mainMenu.selector.center.y = game->mainMenu.playBox.center.y;
		}
		break;

	case PAUSE:
		if (dyna1 == resumeIcon) {
			dyna1 = optionsIcon;
			game->pauseMenu.selector.center.x = game->pauseMenu.obox.center.x;
			game->pauseMenu.selector.center.y = game->pauseMenu.obox.center.y;
		}
		else if (dyna1 == optionsIcon) {
			dyna1 = quitIcon;
			game->pauseMenu.selector.center.x = game->pauseMenu.qbox.center.x;
			game->pauseMenu.selector.center.y = game->pauseMenu.qbox.center.y;
		}
		else if (dyna1 == quitIcon) {
			dyna1 = resumeIcon;
			game->pauseMenu.selector.center.x = game->pauseMenu.rbox.center.x;
			game->pauseMenu.selector.center.y = game->pauseMenu.rbox.center.y;
		}
		break;

	case ZACK:
		//if in the air
		if (!game->level2.player1.collision(&game->level2.platform[0]) && game->level2.player1.action != MOVE && game->level2.player1.action != GROUNDPOUND && game->level2.player1.action != DASH) {
			game->level2.player1.action = GROUNDPOUND;
			game->level2.player1.delta.x = 0.0f; 
			usleep(300000);
			game->level2.player1.delta.y = -20.0f; 
		}
		break;
	default:
		break;
}
break;
case XK_t:
break;
case XK_u:
break;
case XK_v:
break;
case XK_Up:
case XK_w:
switch (game->render) {
	case ZACK:
		if (game->level2.player1.jumpCount < game->level2.player1.JUMP_MAX && game->level2.player1.action != GROUNDPOUND && game->level2.player1.action != DASH) {
			game->level2.player1.delta.y = 5.0f;
			game->level2.player1.jumpCount++;
		}	
		break;
	case MAINMENU:
		if (dyna == playIcon) {
			dyna = exitIcon;
			game->mainMenu.selector.center.x = game->mainMenu.exitBox.center.x;
			game->mainMenu.selector.center.y = game->mainMenu.exitBox.center.y;
		}
		else if (dyna == optionsIcon) {
			dyna = playIcon;
			game->mainMenu.selector.center.x = game->mainMenu.playBox.center.x;
			game->mainMenu.selector.center.y = game->mainMenu.playBox.center.y;
		}
		else if (dyna == exitIcon) {
			dyna = optionsIcon;
			game->mainMenu.selector.center.x = game->mainMenu.optionBox.center.x;
			game->mainMenu.selector.center.y = game->mainMenu.optionBox.center.y;
		}
		break;
	case PAUSE:
		if (dyna1 == resumeIcon) {
			dyna1 = quitIcon;
			game->pauseMenu.selector.center.x = game->pauseMenu.qbox.center.x;
			game->pauseMenu.selector.center.y = game->pauseMenu.qbox.center.y;
		}
		else if (dyna1 == optionsIcon) {
			dyna1 = resumeIcon;
			game->pauseMenu.selector.center.x = game->pauseMenu.rbox.center.x;
			game->pauseMenu.selector.center.y = game->pauseMenu.rbox.center.y;
		}
		else if (dyna1 == quitIcon) {
			dyna1 = optionsIcon;
			game->pauseMenu.selector.center.x = game->pauseMenu.obox.center.x;
			game->pauseMenu.selector.center.y = game->pauseMenu.obox.center.y;
		}
		break;
	default :
		break;
}
break;
case XK_x:
switch (game->render) {
	case ZACK:
		if (game->level2.player1.action != DASH) {
			usleep(300000);
			if (game->level2.player1.collision(&game->level2.platform[0]) && game->level2.player1.delta.x == 0.0f) {
				game->level2.player1.delta.y = 0.0f;
			} else {
				game->level2.player1.delta.y = 0.9f;
			}

			if (game->level2.player1.direction == RIGHT) {
				game->level2.player1.delta.x = game->level2.player1.dashStrength;
			} else if (game->level2.player1.direction == LEFT) {
				game->level2.player1.delta.x = -game->level2.player1.dashStrength;
			}
			game->level2.player1.action = DASH;
		}
		break;
	default:
		break;
}
break;
case XK_y:
break;
case XK_z:
game->render = ZACK;
break;
case XK_Return:
switch (game->render) {
	case MAINMENU:
		if (dyna == playIcon) {		    
			loadSound("./audio/testsong.wav");
			playSound(1.0f, false);
			game->render = FIELD;
		}
		else if (dyna == optionsIcon) {
		}
		else if (dyna == exitIcon) {
			return 1;
		}
		break;
	case PAUSE:
		if (dyna1 == resumeIcon) {
			game->render = game->prevState;
		}
		else if (dyna1 == optionsIcon) {
		}
		else if (dyna1 == quitIcon) {
			resetMain(game);
		}
	default:
		break;
}
break;
case XK_Escape:
//if (game->render == MAINMENU) 
return 1;
break;
default:
break;
}
}

return 0;
}
*/
