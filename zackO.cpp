//Author: Zachary Obeid

/*
Date 04/14/2017
Today i implemented new image into my Starynight level. I also worked on 
getting check keyes working so that you may play the game using the keyboard.
You will not be able to play yet as i have not implemented the keys for the
characters. You may use the keyes to get into the levels however. 

Also Robert, Cesar and I figured out why are game is such a large file. We 
found out that our game is actually only 6.5mb. The issues is that when 
using github we automatically create a hidden .git folder which is 24mb. That 
is the source of why have such a large file. We are trying to look for 
solutions to solve this issue.

I worked at total of 5 hours. 

4/21/17
Added timer to my starynight level.Also Cesar, Robert, and I helped with 
creating the methodology of the physics for attacking. We aided robert in 
looking back at how to understand the collision from our own waterfall levels
and built that into our attacks.  Also we tested the functinality of the 
attacks and the game by playing 3-way- multiplayer.  

worked a total of around 7-8 hours

*/

#include "headers.h"
#include "zackO.h"

using namespace std;
#define GRAVITY .1
#define x_rnd() (rand() % 11) - 5 
#define y_rnd() (rand() % 11)  
#define rnd() (float)rand() / (float)RAND_MAX
#define MAX_STAR 50
#define MAX_PARTICLE 50
#define MAX_PLAYER 4
extern Screen* scrn;
extern Joystick joystick1;
extern Joystick joystick2;
extern Ppmimage *dyna; 
extern Ppmimage *dyna1; 
extern Ppmimage *dyna2;
extern Ppmimage *resumeIcon;
extern Ppmimage *resumeSelIcon;
extern Ppmimage *quitIcon;
extern Ppmimage *quitSelIcon;
extern Ppmimage *fieldIcon;
extern Ppmimage *staryIcon;
extern Ppmimage *medMeltTitle;
extern Ppmimage *pillIcon;
extern Ppmimage *playIcon;
extern Ppmimage *optionsIcon;
extern Ppmimage *exitIcon;
extern Ppmimage *etIcon;
extern GLuint etTexture;
extern float A,B,C,D;
extern int done; 
extern void resetMain(Game *game);
extern Game game;
extern void ericksTimer(Player * player);

Level::Level()
{
    /*
       Color colors;
       for (int i = 0; i < MAX_PARTICLE; i++) {
    //set dimensions of particles
    //void loadImages();
    bloodStream[i].s.width = 4;
    bloodStream[i].s.height = bloodStream[i].s.width;

    //set color of particles
    for (int j = 0; j < 3; j++) {
    bloodStream[i].color[j] = colors.bloodred[j];
    }
    }
     */
}

Field_Level::Field_Level() 
{
    Level();

    //floor
    platform[0].center.x = scrn->width/2;
    // For Level2 

    platform[0].width = scrn->width-250;
    platform[0].height = scrn->height/20; 
    platform[0].center.y = platform[0].height/2 + 150; 

    //handling shading
    widthPartition = scrn->width/7;
    heightPartition = scrn->height/7;
    erick_init();
}

void field(Game *game)
{
    //Oriinal Field 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    game->level2.render();

    for (int i = 0; i < MAX_PLAYER; i++) {
        game->level2.physics(&game->level2.player[i]);
        game->level2.player[i].check_controller(&game->level2.player[i], 
                game->level2.controller.joystick[i]);
        game->level2.deathCheck(&game->level2.player[i]);

        if (game->level2.player[i].status.lifeState == ALIVE) {
            game->level2.player[i].render();
        } else if (game->level2.player[i].status.lifeState == DEAD && 
                game->level2.player[i].status.initDeath == false) {
            clock_gettime(CLOCK_REALTIME, &game->level2.player[i].timeStart);
            game->level2.player[i].status.initDeath = true;
        } else if (game->level2.player[i].status.lifeState == 
                DEAD && game->level2.player[i].status.initDeath == true) {
            clock_gettime(CLOCK_REALTIME, &game->level2.player[i].timeCurrent);
            //game->level2.player[i].deathRender();
            game->level2.player[i].timeSpan = 
                game->level2.player[i].timeDiff(
                        &game->level2.player[i].timeStart, 
                        &game->level2.player[i].timeCurrent);
            if (game->level2.player[i].timeSpan >= 5 && 
                    game->level2.player[i].status.lifeCount > 0) {
                game->level2.player[i].status.initDeath = false;
                game->level2.respawn(&game->level2.player[i]);
            } else if (game->level2.player[i].status.lifeCount < 0) {
                game->level2.player[i].delta.x = 0.0;
                game->level2.player[i].delta.y = 0.0;
                //No lives left
                //Draw a red 'X' over status box
            }
        }	

        /*
        //No lives left
        //Draw a red 'X' over status box
         */
    }

    game->level2.statDisplay.render();
}

void Field_Level::render()
{
    //Render of background
    glClear(GL_COLOR_BUFFER_BIT); 
    glColor3ub(0,125,255); 
    glPushMatrix(); 
    glBegin(GL_QUADS); 
    glVertex2i(-scrn->width/2,-scrn->height/2);
    glColor3ub(0,162,255); 
    glVertex2i(-scrn->width/2, 1.5*scrn->height);
    glColor3ub(0,50,255); 
    glVertex2i(1.5*scrn->width,1.5*scrn->height);
    glColor3ub(0,162,255); 
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

Starynight_Level::Starynight_Level() 
{
    srand(time(NULL));
    Level();

    //floor
    platform[0].center.x = scrn->width/2;
    platform[0].width = scrn->width-250;
    platform[0].height = scrn->height/20; 
    platform[0].center.y = platform[0].height/2 + 150; 

    //Platforms
    for (int i=1; i<MAX_PLAT; i++ ) {
        if ( i % 2 == 0 ) {
            platform[i].center.x = scrn->width/2;
            platform[i].center.y = scrn->height-scrn->height/2 + 
                (scrn->height/6 );
        } else {
            platform[i].center.x = scrn->width/6 + 
                ((scrn->width - scrn->width/6) *(i-1))/2.5;
            platform[i].center.y = scrn->height-scrn->height/2;
        }
        platform[i].width = 250;
        platform[i].height = 50;
    }

    //handling shading
    widthPartition = scrn->width/7;
    heightPartition = scrn->height/7;

    //Randomizing Stars 
    for (int i=0; i<MAX_STAR; i++) {
        starP[i]=rnd(); 
    }

    moon.center.x = scrn->width; 
    moon.center.y = scrn->height; 
    moon.radius = scrn->height/2; 

    etBox.center.x = moon.center.x;
    etBox.center.y = moon.center.y;
    erick_init();	
}

void starynight(Game *game)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    game->level3.render();
    game->level3.etMove();

    for (int i = 0; i < MAX_PLAYER; i++) {
        ericksTimer(&game->level3.player[i]);
        game->level3.physics(&game->level3.player[i]);
        game->level3.player[i].check_controller(&game->level3.player[i], 
                game->level3.controller.joystick[i]);
        game->level3.deathCheck(&game->level3.player[i]);


        if (game->level3.player[i].status.lifeState == ALIVE) {
            game->level3.player[i].render();
        } else {
            game->level3.player[i].deathPhysics();
            game->level3.player[i].deathRender();
        }

        if (game->level3.player[i].status.lifeState == ALIVE) {
            game->level3.player[i].render();
        } else if (game->level3.player[i].status.lifeState == DEAD && 
                game->level3.player[i].status.initDeath == false) {
            clock_gettime(CLOCK_REALTIME, &game->level3.player[i].timeStart);
            game->level3.player[i].status.initDeath = true;
        } else if (game->level3.player[i].status.lifeState == DEAD && 
                game->level3.player[i].status.initDeath == true) {
            clock_gettime(CLOCK_REALTIME, &game->level3.player[i].timeCurrent);
            game->level3.player[i].timeSpan = 
                game->level3.player[i].timeDiff(
                        &game->level3.player[i].timeStart, 
                        &game->level3.player[i].timeCurrent);
            if (game->level3.player[i].timeSpan >= 5 && 
                    game->level3.player[i].status.lifeCount > 0) {
                game->level3.player[i].status.initDeath = false;
                game->level3.respawn(&game->level3.player[i]);
            } else if (game->level3.player[i].status.lifeCount < 0) {
                game->level3.player[i].delta.x = 0.0;
                game->level3.player[i].delta.y = 0.0;
                //No lives left
                //Draw a red 'X' over status box
            }
        }	
    }
    game->level3.statDisplay.render();
}

void Starynight_Level::render()
{
    //Render of background
    glClear(GL_COLOR_BUFFER_BIT); 
    glColor3ub(72,48,120); 
    glPushMatrix(); 
    glBegin(GL_QUADS); 
    glVertex2i(-scrn->width/2,-scrn->height/2);
    glColor3ub(0,24,72); 
    glVertex2i(-scrn->width/2, 1.5*scrn->height);
    glColor3ub(72,48,120); 
    glVertex2i(1.5*scrn->width,1.5*scrn->height);
    glVertex2i(1.5*scrn->width,-scrn->height/2);
    glEnd(); 
    // End of Background

    //Render Stars
    int vertical=0; 
    int horizontal=0; 
    int sideL=5;
    for (int i=0; i<7; i++) {
        for (int j=0; j<7; j++) {
            vertical=((int)2*scrn->height/7)*(i+starP[(i+1)*(j+1)]); 
            horizontal=((int)2*scrn->width/7)*(j+starP[(i+1)*(j+1)]);
            vertical -= scrn->height/2;
            horizontal -= scrn->width/2;
            glPushMatrix();
            glColor3ub(254,0+rand()%(255-0),215);
            glBegin(GL_QUADS);
            glVertex2i(horizontal,vertical);
            glVertex2i(horizontal+sideL,vertical);
            glVertex2i(horizontal+sideL,vertical+sideL);
            glVertex2i(horizontal,vertical+sideL);
        } 
        glEnd(); 
        glPopMatrix();
    }
    //End of Stars


    //Start of Moon 
    float x,y,radius; 
    x = moon.center.x; 
    y = moon.center.y;
    radius = moon.radius;
    glPushMatrix();
    glColor3ub(225,225,214);
    int triangleAmount = 100; 
    float twicePi = 2.0f * M_PI; 
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y); // center of circle
    for (int i = 0; i <= triangleAmount;i++) { 
        glVertex2f(
                x + (radius * cos(i *  twicePi / triangleAmount)), 
                y + (radius * sin(i * twicePi / triangleAmount))
                );
    }
    glEnd();
    glPopMatrix();
    // End of Moon 

    //render et
    A = etBox.center.x - etIcon->width/4;
    B = etBox.center.y - etIcon->height/4;
    C = etBox.center.y + etIcon->height/4;
    D = etBox.center.x + etIcon->width/4;

    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D,etTexture);
    glEnable(GL_ALPHA_TEST);
    glEnable(GL_TEXTURE_2D);
    glAlphaFunc(GL_GREATER, 0.0f);
    glColor4ub(255,255,255,255);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex2i(A,B);
    glTexCoord2f(0.0f, 0.0f); glVertex2i(A,C);
    glTexCoord2f(1.0f, 0.0f); glVertex2i(D,C);
    glTexCoord2f(1.0f, 1.0f); glVertex2i(D,B);
    glEnd();
    glPopMatrix();
    glDisable(GL_ALPHA_TEST);
    glDisable(GL_TEXTURE_2D);

    // end of et

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

    for (int i = 1; i < MAX_PLAT; i++ ) {
        // black line for platforms

        glColor3ub(0,0,0); 
        glPushMatrix(); 
        glBegin(GL_QUADS); 
        glVertex2i(platform[i].center.x - platform[i].width/2 - 2, 
                platform[i].center.y - platform[i].height/2 -2);
        glVertex2i(platform[i].center.x - platform[i].width/2 -2, 
                platform[i].center.y + platform[i].height/2 +2);
        glVertex2i(platform[i].center.x + platform[i].width/2 + 2, 
                platform[i].center.y + platform[i].height/2 + 2);
        glVertex2i(platform[i].center.x + platform[i].width/2 + 2, 
                platform[i].center.y - platform[i].height/2 - 2);
        glEnd(); 
        // end of black line 

        //Render Platforms 
        glColor3ub(125,160,255); 
        glPushMatrix(); 
        glBegin(GL_QUADS); 
        glVertex2i(platform[i].center.x - platform[i].width/2, 
                platform[i].center.y - platform[i].height/2);
        glVertex2i(platform[i].center.x - platform[i].width/2, 
                platform[i].center.y + platform[i].height/2);
        glColor3ub(120,210,0); 
        glVertex2i(platform[i].center.x + platform[i].width/2, 
                platform[i].center.y + platform[i].height/2);
        glVertex2i(platform[i].center.x + platform[i].width/2, 
                platform[i].center.y - platform[i].height/2);
        glEnd(); 
        // End of Platforms 
    }

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
//Check Keys for Gordon
void check_keys(XEvent *e, Game *game) 
{
    bool conditionA, conditionB, conditionC;
    if (e->type == KeyPress) {
        int key = XLookupKeysym(&e->xkey,0);

        switch (key) {
            default:
                break;
            case XK_Escape:
                done = 1; 
                break;
            case XK_m:
                manual_launch();
                break;
            case XK_c:
                switch (game->render) {
                    case STARYNIGHT:
                        game->level3.player[0].ispressed = !game->level3.player[0].ispressed;
                        break;
                    case DISCO:
                        game->level4.player[0].ispressed = !game->level4.player[0].ispressed;
                        break;
                    default : 
                        break;
                }
                break;
            case XK_w:
                switch (game->render) {
                    case MAINMENU:
                        if (dyna == playIcon) {
                            dyna = exitIcon;
                            game->mainMenu.selector.center.x = 
                                game->mainMenu.exitBox.center.x;
                            game->mainMenu.selector.center.y = 
                                game->mainMenu.exitBox.center.y;
                        } else if (dyna == optionsIcon) {
                            dyna = playIcon;
                            game->mainMenu.selector.center.x = 
                                game->mainMenu.playBox.center.x;
                            game->mainMenu.selector.center.y = 
                                game->mainMenu.playBox.center.y;
                        } else if (dyna == exitIcon) {
                            dyna = optionsIcon;
                            game->mainMenu.selector.center.x = 
                                game->mainMenu.optionBox.center.x;
                            game->mainMenu.selector.center.y = 
                                game->mainMenu.optionBox.center.y;
                        }
                        break;
                    case PAUSE:
                        if (dyna1 == resumeIcon) {
                            dyna1 = quitIcon;
                            game->pauseMenu.selector.center.x = 
                                game->pauseMenu.qbox.center.x;
                            game->pauseMenu.selector.center.y = 
                                game->pauseMenu.qbox.center.y;
                        } else if (dyna1 == optionsIcon) {
                            dyna1 = resumeIcon;
                            game->pauseMenu.selector.center.x = 
                                game->pauseMenu.rbox.center.x;
                            game->pauseMenu.selector.center.y = 
                                game->pauseMenu.rbox.center.y;
                        } else if (dyna1 == quitIcon) {
                            dyna1 = optionsIcon;
                            game->pauseMenu.selector.center.x = 
                                game->pauseMenu.obox.center.x;
                            game->pauseMenu.selector.center.y = 
                                game->pauseMenu.obox.center.y;
                        }
                        break;
                    case STARYNIGHT:
                        conditionA= game->level3.player[0].jumpCount < 
                            game->level3.player[0].JUMP_MAX;
                        conditionB= game->level3.player[0].action != 
                            GROUNDPOUND;
                        conditionC= game->level3.player[0].action != DASH;
                        if (conditionA && conditionB && conditionC) {
                            game->level3.player[0].delta.y = 7.0f; 
                            game->level3.player[0].jumpCount++; 
                        }
                        //void loadImages();
                        break;
                    case FIELD:
                        conditionA= game->level2.player[0].jumpCount < 
                            game->level2.player[0].JUMP_MAX;
                        conditionB= game->level2.player[0].action != 
                            GROUNDPOUND;
                        conditionC= game->level2.player[0].action != DASH;
                        if (conditionA && conditionB && conditionC) {
                            game->level2.player[0].delta.y = 7.0f; 
                            game->level2.player[0].jumpCount++; 
                        }
                        break;
                    case DISCO:
                        conditionA= game->level4.player[0].jumpCount < 
                            game->level4.player[0].JUMP_MAX;
                        conditionB= game->level4.player[0].action != 
                            GROUNDPOUND;
                        conditionC= game->level4.player[0].action != DASH;
                        if (conditionA && conditionB && conditionC) {
                            game->level4.player[0].delta.y = 7.0f; 
                            game->level4.player[0].jumpCount++; 
                        }
                        break;
                    default : 
                        break;

                }
                break;
            case XK_r: 
                if (game->render == OVER) {
                    resetMain(game);
                    if (game->prevState == FIELD)
                        game->level2.erick_init();
                    if (game->prevState == STARYNIGHT)
                        game->level3.erick_init();
                    if (game->prevState == DISCO)
                        game->level4.erick_init();
                    game->render = MAINMENU;
                }
                break;	
            case XK_s:
                switch (game->render) {
                    case MAINMENU:
                        if (dyna == playIcon) {
                            dyna = optionsIcon;
                            game->mainMenu.selector.center.x = 
                                game->mainMenu.optionBox.center.x;
                            game->mainMenu.selector.center.y = 
                                game->mainMenu.optionBox.center.y;
                        } else if (dyna == optionsIcon) {
                            dyna = exitIcon;
                            game->mainMenu.selector.center.x = 
                                game->mainMenu.exitBox.center.x;
                            game->mainMenu.selector.center.y = 
                                game->mainMenu.exitBox.center.y;
                        } else if (dyna == exitIcon) {
                            dyna = playIcon;
                            game->mainMenu.selector.center.x = 
                                game->mainMenu.playBox.center.x;
                            game->mainMenu.selector.center.y = 
                                game->mainMenu.playBox.center.y;
                        }
                        break;
                    case PAUSE:
                        if (dyna1 == resumeIcon) {
                            dyna1 = optionsIcon;
                            game->pauseMenu.selector.center.x = 
                                game->pauseMenu.obox.center.x;
                            game->pauseMenu.selector.center.y = 
                                game->pauseMenu.obox.center.y;
                        } else if (dyna1 == optionsIcon) {
                            dyna1 = quitIcon;
                            game->pauseMenu.selector.center.x = 
                                game->pauseMenu.qbox.center.x;
                            game->pauseMenu.selector.center.y = 
                                game->pauseMenu.qbox.center.y;
                        } else if (dyna1 == quitIcon) {
                            dyna1 = resumeIcon;
                            game->pauseMenu.selector.center.x = 
                                game->pauseMenu.rbox.center.x;
                            game->pauseMenu.selector.center.y = 
                                game->pauseMenu.rbox.center.y;
                        }
                        break;
                    case STARYNIGHT:
                        game->level3.player[0].action = GROUNDPOUND;
                        game->level3.player[0].delta.x = 0.0f;
#ifdef USE_OPENAL_SOUND
                        if (game->level3.player[0].status.lifeState == ALIVE) {
                            play_sound(3, 1.0f, false);
                        }
#endif
                        //usleep(300000);
                        game->level3.player[0].delta.y = -20.0f;
                        break;
                    case FIELD:
                        game->level2.player[0].action = GROUNDPOUND;
                        game->level2.player[0].delta.x = 0.0f;
#ifdef USE_OPENAL_SOUND
                        if (game->level2.player[0].status.lifeState == ALIVE) {
                            play_sound(3, 1.0f, false);
                        }
#endif
                        //usleep(300000);
                        game->level2.player[0].delta.y = -20.0f;
                        break;
                    case DISCO:
                        game->level4.player[0].action = GROUNDPOUND;
                        game->level4.player[0].delta.x = 0.0f;
#ifdef USE_OPENAL_SOUND
                        if (game->level4.player[0].status.lifeState == ALIVE) {
                            play_sound(3, 1.0f, false);
                        }
#endif
                        //usleep(300000);
                        game->level4.player[0].delta.y = -20.0f;
                        break;
                    default : 
                        break;
                }
                break;
            case XK_j:
                switch (game->render) {
                    case STARYNIGHT:
                        game->level3.player[0].attack(); 
                        break;
                    case FIELD:
                        game->level2.player[0].attack(); 
                        break;
                    case DISCO:
                        game->level4.player[0].attack(); 
                        break;
                    default:
                        break;
                };
                break;
            case XK_Return:
                switch (game->render) {
                    case MAINMENU:
                        if (dyna == playIcon) { 
                            game->render = LEVELSEL;
                        } else if (dyna == optionsIcon) {
                        } else if (dyna == exitIcon) {
                            done = 1;
                            return;
                        }
                        break;
                    case PAUSE:
                        if (dyna1 == resumeIcon) {
                            game->render = game->prevState;
                        } else if (dyna1 == optionsIcon) {
                        } else if (dyna1 == quitIcon) {
                            resetMain(game);
                        }
                    case LEVELSEL:
                        if (dyna2 == fieldIcon) {
#ifdef USE_OPENAL_SOUND
                            play_sound(4, 1.0f, true);
#endif
                            game->render = DISCO;
                        } else if (dyna2 == staryIcon) {
#ifdef USE_OPENAL_SOUND
                            play_sound(5, 1.0f, true);
#endif
                            game->render = STARYNIGHT;
                        }
                        break;
                    default:
                        break;
                }
                break;
            case XK_a:
                switch (game->render) {
                    case LEVELSEL:
                        if (dyna2 == fieldIcon) {
                            dyna2 = staryIcon;
                            game->levelMenu.selector.center.x = 
                                game->levelMenu.level2.center.x;
                            game->levelMenu.selector.center.y = 
                                game->levelMenu.level2.center.y;
                        } else if (dyna2 == staryIcon) {
                            dyna2 = fieldIcon;
                            game->levelMenu.selector.center.x = 
                                game->levelMenu.level1.center.x;
                            game->levelMenu.selector.center.y = 
                                game->levelMenu.level1.center.y;
                        }
                        break;
                    case STARYNIGHT:
                        if (game->level3.player[0].action != DASH) {
                            game->level3.player[0].direction = LEFT;
                            game->level3.player[0].action = MOVE;
                            game->level3.player[0].delta.x = -5.0f;
                        }
                        if (game->level3.player[0].onGround)
                            game->level3.player[0].delta.y = 1.25f;
                        break;
                    case FIELD:
                        if (game->level2.player[0].action != DASH) {
                            game->level2.player[0].direction = LEFT;
                            game->level2.player[0].action = MOVE;
                            game->level2.player[0].delta.x = -5.0f;
                        }
                        if (game->level2.player[0].onGround)
                            game->level2.player[0].delta.y = 1.25f;
                        break;
                    case DISCO:
                        if (game->level4.player[0].action != DASH) {
                            game->level4.player[0].direction = LEFT;
                            game->level4.player[0].action = MOVE;
                            game->level4.player[0].delta.x = -5.0f;
                        }
                        if (game->level4.player[0].onGround)
                            game->level4.player[0].delta.y = 1.25f;
                        break;
                    default:
                        break;
                }
                break;
            case XK_d:
                switch (game->render) {
                    case LEVELSEL:
                        if (dyna2 == fieldIcon) {
                            dyna2 = staryIcon;
                            game->levelMenu.selector.center.x = 
                                game->levelMenu.level2.center.x;
                            game->levelMenu.selector.center.y = 
                                game->levelMenu.level2.center.y;
                        } else if (dyna2 == staryIcon) {
                            dyna2 = fieldIcon;
                            game->levelMenu.selector.center.x = 
                                game->levelMenu.level1.center.x;
                            game->levelMenu.selector.center.y = 
                                game->levelMenu.level1.center.y;
                        }
                        break;
                    case STARYNIGHT:
                        if (game->level3.player[0].action != DASH) {
                            game->level3.player[0].direction = RIGHT;
                            game->level3.player[0].action = MOVE;
                            game->level3.player[0].delta.x = 5.0f;
                        }
                        if (game->level3.player[0].onGround)
                            game->level3.player[0].delta.y = 1.25f;
                        break;
                    case FIELD:
                        if (game->level2.player[0].action != DASH) {
                            game->level2.player[0].direction = RIGHT;
                            game->level2.player[0].action = MOVE;
                            game->level2.player[0].delta.x = 5.0f;
                        }
                        if (game->level2.player[0].onGround)
                            game->level2.player[0].delta.y = 1.25f;
                        break;
                    case DISCO:
                        if (game->level4.player[0].action != DASH) {
                            game->level4.player[0].direction = RIGHT;
                            game->level4.player[0].action = MOVE;
                            game->level4.player[0].delta.x = 5.0f;
                        }
                        if (game->level4.player[0].onGround)
                            game->level4.player[0].delta.y = 1.25f;
                        break;
                    default:
                        break;
                };
                break;
            case XK_p:
                break;
            case XK_v:
#ifdef USE_OPENAL_SOUND
                play_sound (1, 1.0f, true);
#endif
                game->render = DISCO; 
                break;

                break;
        }
    }
}
void end_game()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f );
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    Rect r;
    glEnable(GL_TEXTURE_2D);
    r.bot = game.levelMenu.textBox.center.y -10;
    r.left = game.levelMenu.textBox.center.x;
    r.center = game.levelMenu.textBox.center.x;
    unsigned int color = 0x0ff0000;
    char text[] = "GAME OVER";
    ggprint40(&r, 0, color, "%s", text);
    glDisable(GL_TEXTURE_2D);
}
bool endcheck()
{
    bool A = false;
    bool B = false;
    bool C = false;
    bool D = false;
    switch (game.render) {
        case STARYNIGHT :
            if (game.level3.player[0].status.lifeCount == 0 
                    && game.level3.player[0].status.lifeState == DEAD)
                A = true;
            if (game.level3.player[1].status.lifeCount == 0 
                    && game.level3.player[1].status.lifeState == DEAD)
                B = true;
            if (game.level3.player[2].status.lifeCount == 0 
                    && game.level3.player[2].status.lifeState == DEAD)
                C = true;
            if (game.level3.player[3].status.lifeCount == 0 
                    && game.level3.player[3].status.lifeState == DEAD)
                D = true;
            break;
        case FIELD :
            if (game.level2.player[0].status.lifeCount == 0 
                    && game.level2.player[0].status.lifeState == DEAD)
                A = true;
            if (game.level2.player[1].status.lifeCount == 0 
                    && game.level2.player[1].status.lifeState == DEAD)
                B = true;
            if (game.level2.player[2].status.lifeCount == 0 
                    && game.level2.player[2].status.lifeState == DEAD)
                C = true;
            if (game.level2.player[3].status.lifeCount == 0 
                    && game.level2.player[3].status.lifeState == DEAD)
                D = true;
            break;
        case DISCO:
            if (game.level4.player[0].status.lifeCount == 0 
                    && game.level4.player[0].status.lifeState == DEAD)
                A = true;
            if (game.level4.player[1].status.lifeCount == 0 
                    && game.level4.player[1].status.lifeState == DEAD)
                B = true;
            if (game.level4.player[2].status.lifeCount == 0 
                    && game.level4.player[2].status.lifeState == DEAD)
                C = true;
            if (game.level4.player[3].status.lifeCount == 0 
                    && game.level4.player[3].status.lifeState == DEAD)
                D = true;
            break;
        default: 
            break;
    }
    bool last1 = false;
    bool last2 = false;
    bool last3 = false;
    bool last4 = false;
    if (B && C && D) // last 1 alive is player 1
        last1 = true;
    if (A && C && D) // last 1 alive is player 2
        last2 = true;
    if (A && B && D) // last 1 alive is player 3
        last3 = true;
    if (A && B && C) // last one is player 4
        last4 = true;
    return (last1 || last2 || last3 || last4);
}


void Starynight_Level::etMove() 
{
    //etBox.center.x++;
    etBox.center.x+= 3;
    if (etBox.center.x > 1.75 * scrn->width) {
        etBox.center.x = -scrn->width;
    }

    etBox.center.y = 9 * etBox.center.x / 16 + 100;
}
