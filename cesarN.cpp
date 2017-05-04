//Author: Cesar Nuno
//This file handles all the menu screens

/*
April 13: worked for 2 hours to implement a level select menu to the game. 
also added functionality to "B" button on menu screens"
   */

#include "headers.h"

using namespace std;

float A, B, C, D;
extern Ppmimage *medMeltTitle;
extern Ppmimage *pillIcon;
extern Ppmimage *playIcon;
extern Ppmimage *optionsIcon;
extern Ppmimage *exitIcon;
extern Ppmimage *dyna;
extern GLuint silTitle;
extern GLuint silPill;
extern GLuint silPlay;
extern GLuint silOptions;
extern GLuint silExit;
extern GLuint medMeltTexture;
extern GLuint pillTexture;
extern GLuint playTexture;
extern GLuint optionsTexture;
extern GLuint exitTexture;
#ifdef USE_OPENAL_SOUND
extern int titledrop;
extern float pitch;
#endif
extern Ppmimage *resumeIcon;
extern Ppmimage *resumeSelIcon;
extern Ppmimage *optionsIcon;
extern Ppmimage *quitIcon;
extern Ppmimage *quitSelIcon;
extern Ppmimage *dyna1;
extern Ppmimage *dyna2;
extern Ppmimage *fieldIcon;
extern Ppmimage *staryIcon;
extern GLuint resumeTexture;
extern GLuint fieldTexture;
extern GLuint staryTexture;
extern GLuint resumeSelTexture;
extern GLuint optionsTexture;
extern GLuint quitTexture;
extern GLuint quitSelTexture;
extern void resetMain(Game*game);
extern Game game;
extern int done;
int choice=1;


void pause_move(Joystick *joystick)
{
    JoystickEvent event;

    if (!joystick->isFound()) {
        //printf("open failed.\n");
    }

    if (game.render == PAUSE) {
        if (joystick->sample(&event) && event.isButton() && event.value == 1) {
            switch (event.number) {
                case 0 :
                    if (dyna1 == resumeIcon) {
                        game.render = game.prevState;
                    } else if (dyna1 == optionsIcon) {
                    } else if (dyna1 == quitIcon) {
                        resetMain(&game);
                    }
                    break;

                case 13 :    //up on d-pad
                    if (dyna1 == resumeIcon) {
                        dyna1 = quitIcon;
                        choice = 3;
                        game.pauseMenu.selector.center.x = 
                            game.pauseMenu.qbox.center.x;
                        game.pauseMenu.selector.center.y = 
                            game.pauseMenu.qbox.center.y;
                    } else if (dyna1 == optionsIcon) {
                        dyna1 = resumeIcon;
                        choice = 1;
                        game.pauseMenu.selector.center.x = 
                            game.pauseMenu.rbox.center.x;
                        game.pauseMenu.selector.center.y = 
                            game.pauseMenu.rbox.center.y;
                    } else if (dyna1 == quitIcon) {
                        dyna1 = optionsIcon;
                        choice = 2;
                        game.pauseMenu.selector.center.x = 
                            game.pauseMenu.obox.center.x;
                        game.pauseMenu.selector.center.y = 
                            game.pauseMenu.obox.center.y;
                    }
                    break;
                case 14:
                    if (dyna1 == resumeIcon) {
                        dyna1 = optionsIcon;
                        choice = 2;
                        game.pauseMenu.selector.center.x = 
                            game.pauseMenu.obox.center.x;
                        game.pauseMenu.selector.center.y = 
                            game.pauseMenu.obox.center.y;
                    } else if (dyna1 == optionsIcon) {
                        dyna1 = quitIcon;
                        choice = 3;
                        game.pauseMenu.selector.center.x = 
                            game.pauseMenu.qbox.center.x;
                        game.pauseMenu.selector.center.y = 
                            game.pauseMenu.qbox.center.y;
                    } else if (dyna1 == quitIcon) {
                        dyna1 = resumeIcon;
                        choice = 1;
                        game.pauseMenu.selector.center.x = 
                            game.pauseMenu.rbox.center.x;
                        game.pauseMenu.selector.center.y = 
                            game.pauseMenu.rbox.center.y;
                    }
                    break;
                default : 
                    break;

            }
        }
    }
}

void menu_move(Joystick *joystick, Game* game)
{
    JoystickEvent event;

    if (!joystick->isFound()) {
        //printf("open failed.\n");
    }

    if (game->render == MAINMENU) {
        if (joystick->sample(&event) && event.isButton() && event.value == 1) {
            switch (event.number) {
                case 0 :
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
                            /*case PAUSE:
                              if (dyna1 == resumeIcon) {
                              game->render = game->prevState;
                              }
                              else if (dyna1 == optionsIcon) {
                              }
                              else if (dyna1 == quitIcon) {
                              resetMain(game);
                              }*/
                        default:
                            break;
                    }
                    return;
                case 6 :
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

                    return;

                case 13 :    //up on d-pad
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
                            /*case PAUSE:
                              if (dyna1 == resumeIcon) {
                              dyna1 = quitIcon;
                              game->pauseMenu.selector.center.x = 
                              game->pauseMenu.qbox.center.x;
                              game->pauseMenu.selector.center.y = 
                              game->pauseMenu.qbox.center.y;
                              }
                              else if (dyna1 == optionsIcon) {
                              dyna1 = resumeIcon;
                              game->pauseMenu.selector.center.x = 
                              game->pauseMenu.rbox.center.x;
                              game->pauseMenu.selector.center.y = 
                              game->pauseMenu.rbox.center.y;
                              }
                              else if (dyna1 == quitIcon) {
                              dyna1 = optionsIcon;
                              game->pauseMenu.selector.center.x = 
                              game->pauseMenu.obox.center.x;
                              game->pauseMenu.selector.center.y = 
                              game->pauseMenu.obox.center.y;
                              }
                              break;*/
                        default : 
                            break;
                    }
                    return;
                case 14:
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
                            /*case PAUSE:
                              if (dyna1 == resumeIcon) {
                              dyna1 = optionsIcon;
                              game->pauseMenu.selector.center.x = 
                              game->pauseMenu.obox.center.x;
                              game->pauseMenu.selector.center.y = 
                              game->pauseMenu.obox.center.y;
                              }
                              else if (dyna1 == optionsIcon) {
                              dyna1 = quitIcon;
                              game->pauseMenu.selector.center.x = 
                              game->pauseMenu.qbox.center.x;
                              game->pauseMenu.selector.center.y = 
                              game->pauseMenu.qbox.center.y;
                              }
                              else if (dyna1 == quitIcon) {
                              dyna1 = resumeIcon;
                              game->pauseMenu.selector.center.x = 
                              game->pauseMenu.rbox.center.x;
                              game->pauseMenu.selector.center.y = 
                              game->pauseMenu.rbox.center.y;
                              }
                              break;*/
                        default : 
                            break;
                    }
                    return;

                default : 
                    break;

            }
        }
    }
}
Joystick *temp;
void toggle_pause(Joystick *joystick)
{
    game.prevState = game.render;
    game.render = PAUSE;
    temp = joystick;
}

void cpause(Game *game)
{
    render_pause(game);
    pause_move(temp);
}

void render_pause(Game *game)
{
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.1);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //glColor4f(1.0,1.0,0, 0.75);
    /*glBegin(GL_QUADS);
      glVertex3f(game->WINDOW_WIDTH/10,game->WINDOW_HEIGHT/10, 0.0); 
      glVertex3f(game->WINDOW_WIDTH/10,game->WINDOW_HEIGHT - 
      game->WINDOW_HEIGHT/10, 0.0); 
      glVertex3f(game->WINDOW_WIDTH - 
      game->WINDOW_WIDTH/10, game->WINDOW_HEIGHT - 
      game->WINDOW_HEIGHT/10, 0.0);
      glVertex3f(game->WINDOW_WIDTH - 
      game->WINDOW_WIDTH/10, game->WINDOW_HEIGHT/10, 0.0);
      glEnd();*/

    // box still not transparent without clearing
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f );
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (game->prevState == FIELD) {
        game->level2.render();
        for (int i = 0; i < MAX_PLAYER; i++) {
            game->level2.player[i].render();
        }
    } else if (game->prevState == STARYNIGHT) {
        game->level3.render();
        for (int i = 0; i < MAX_PLAYER; i++) {
            game->level3.player[i].render();
        }
    } else if (game->prevState == DISCO) {
        game->level4.render();
        for (int i = 0; i < MAX_PLAYER; i++) {
            game->level4.player[i].render();
        }
    }
    /*A = game->pauseMenu.selector.center.x - dyna1->width/2;
      B = game->pauseMenu.selector.center.y - dyna1->height/2;
      C = game->pauseMenu.selector.center.y + dyna1->height/2;
      D = game->pauseMenu.selector.center.x + dyna1->width/2;

      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glColor4f(1.0,1.0,0.0, 1.0);
      glBegin(GL_QUADS);
      glVertex2i(A,B);
      glColor4f(1.0,1.0,0.0,0.0);
      glVertex2i(A,C);
      glVertex2i(D,C);
      glColor4f(1.0,1.0,0.0, 1.0);
      glVertex2i(D,B);
      glEnd();
      glDisable(GL_BLEND);*/
    if (choice == 1) {
        A = game->pauseMenu.rbox.center.x - resumeSelIcon->width/2;
        B = game->pauseMenu.rbox.center.y - resumeSelIcon->height/2;
        C = game->pauseMenu.rbox.center.y + resumeSelIcon->height/2;
        D = game->pauseMenu.rbox.center.x + resumeSelIcon->width/2;

        glPushMatrix();
        glBindTexture(GL_TEXTURE_2D,resumeSelTexture);
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

    } else {
        A = game->pauseMenu.rbox.center.x - resumeIcon->width/2;
        B = game->pauseMenu.rbox.center.y - resumeIcon->height/2;
        C = game->pauseMenu.rbox.center.y + resumeIcon->height/2;
        D = game->pauseMenu.rbox.center.x + resumeIcon->width/2;

        glPushMatrix();
        glBindTexture(GL_TEXTURE_2D,resumeTexture);
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
    }

    A = game->pauseMenu.obox.center.x - optionsIcon->width/2;
    B = game->pauseMenu.obox.center.y - optionsIcon->height/2;
    C = game->pauseMenu.obox.center.y + optionsIcon->height/2;
    D = game->pauseMenu.obox.center.x + optionsIcon->width/2;

    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D,optionsTexture);
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

    if (choice == 3) {
        A = game->pauseMenu.qbox.center.x - quitSelIcon->width/2;
        B = game->pauseMenu.qbox.center.y - quitSelIcon->height/2;
        C = game->pauseMenu.qbox.center.y + quitSelIcon->height/2;
        D = game->pauseMenu.qbox.center.x + quitSelIcon->width/2;

        glPushMatrix();
        glBindTexture(GL_TEXTURE_2D,quitSelTexture);
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
    } else {
        A = game->pauseMenu.qbox.center.x - quitIcon->width/2;
        B = game->pauseMenu.qbox.center.y - quitIcon->height/2;
        C = game->pauseMenu.qbox.center.y + quitIcon->height/2;
        D = game->pauseMenu.qbox.center.x + quitIcon->width/2;

        glPushMatrix();
        glBindTexture(GL_TEXTURE_2D,quitTexture);
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

    }

}

void cesarN()
{
    cout << "Cesar" << endl;
}

void menu(Game *game) 
{
    movement_menu(game);
    render_menu(game);
}

void render_menu(Game *game)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f );
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    A = game->mainMenu.selector.center.x - dyna->width/4;
    B = game->mainMenu.selector.center.y - dyna->height/4;
    C = game->mainMenu.selector.center.y + dyna->height/4;
    D = game->mainMenu.selector.center.x + dyna->width/4;

    //transparent background
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(1.0,1.0,0.0, 1.0);
    glBegin(GL_QUADS);
    glVertex2i(A,B);
    glColor4f(1.0,1.0,0.0,0.0);
    glVertex2i(A,C);
    glVertex2i(D,C);
    glColor4f(1.0,1.0,0.0, 1.0);
    glVertex2i(D,B);
    glEnd();
    glDisable(GL_BLEND);
    //end of background

    A = game->mainMenu.pillBox.center.x - pillIcon->width/1.5;
    B = game->mainMenu.pillBox.center.y - pillIcon->height/1.5;
    C = game->mainMenu.pillBox.center.y + pillIcon->height/1.5;
    D = game->mainMenu.pillBox.center.x + pillIcon->width/1.5;

    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D,pillTexture);
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

    A = game->mainMenu.titleBox.center.x - medMeltTitle->width/2;
    B = game->mainMenu.titleBox.center.y - medMeltTitle->height/2; 
    C = game->mainMenu.titleBox.center.y + medMeltTitle->height/2; 
    D = game->mainMenu.titleBox.center.x + medMeltTitle->width/2;

    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D,silTitle);
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

    A = game->mainMenu.playBox.center.x - playIcon->width/4;
    B = game->mainMenu.playBox.center.y - playIcon->height/4;
    C = game->mainMenu.playBox.center.y + playIcon->height/4;
    D = game->mainMenu.playBox.center.x + playIcon->width/4;

    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D,playTexture);
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

    A = game->mainMenu.optionBox.center.x - optionsIcon->width/4;
    B = game->mainMenu.optionBox.center.y - optionsIcon->height/4;
    C = game->mainMenu.optionBox.center.y + optionsIcon->height/4;
    D = game->mainMenu.optionBox.center.x + optionsIcon->width/4;

    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D,optionsTexture);
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

    A = game->mainMenu.exitBox.center.x - exitIcon->width/4;
    B = game->mainMenu.exitBox.center.y - exitIcon->height/4;
    C = game->mainMenu.exitBox.center.y + exitIcon->height/4;
    D = game->mainMenu.exitBox.center.x + exitIcon->width/4;

    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D,exitTexture);
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
}

void movement_menu(Game *game) 
{
    game->mainMenu.titleVel.y -= GRAVITY;
    game->mainMenu.titleBox.center.y += game->mainMenu.titleVel.y;
    if (game->mainMenu.titleBox.center.y <= 
            game->WINDOW_HEIGHT-game->WINDOW_HEIGHT/3.5) {
#ifdef USE_OPENAL_SOUND
        if (titledrop < 11) {
            play_sound(0, pitch, false);
            titledrop++;
            pitch = pitch + .7f;
        }
#endif
        game->mainMenu.titleBox.center.y = 
            game->WINDOW_HEIGHT-game->WINDOW_HEIGHT/
            3.5-game->mainMenu.titleBox.height/2;
        game->mainMenu.titleVel.y = -game->mainMenu.titleVel.y * 0.75f;
    }
}
void levelsel(Game *game)
{
    render_levelsel(game);
}

void render_levelsel(Game *game)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f );
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    A = game->levelMenu.selector.center.x - 1.05*(dyna2->width/4);
    B = game->levelMenu.selector.center.y - 1.05*(dyna2->height/4);
    C = game->levelMenu.selector.center.y + 1.05*(dyna2->height/4);
    D = game->levelMenu.selector.center.x + 1.05*(dyna2->width/4);

    //transparent background
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(1.0,1.0,0.0, 1.0);
    glBegin(GL_QUADS);
    glVertex2i(A,B);
    glColor4f(1.0,1.0,0.0,0.0);
    glVertex2i(A,C);
    glVertex2i(D,C);
    glColor4f(1.0,1.0,0.0, 1.0);
    glVertex2i(D,B);
    glEnd();
    glDisable(GL_BLEND);
    //end of background

    //text===========================
    Rect r;
    glEnable(GL_TEXTURE_2D);
    r.bot = game->levelMenu.textBox.center.y -10;
    r.left = game->levelMenu.textBox.center.x;
    r.center = game->levelMenu.textBox.center.x;
    unsigned int color = 0x0ff0000;
    char text[] = "Level Select";
    ggprint40(&r, 20, color, "%s", text);
    glDisable(GL_TEXTURE_2D);
    //end of text======================

    A = game->levelMenu.level1.center.x - fieldIcon->width/4;
    B = game->levelMenu.level1.center.y - fieldIcon->height/4;
    C = game->levelMenu.level1.center.y + fieldIcon->height/4;
    D = game->levelMenu.level1.center.x + fieldIcon->width/4;

    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D,fieldTexture);
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

    A = game->levelMenu.level2.center.x - staryIcon->width/4;
    B = game->levelMenu.level2.center.y - staryIcon->height/4;
    C = game->levelMenu.level2.center.y + staryIcon->height/4;
    D = game->levelMenu.level2.center.x + staryIcon->width/4;

    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D,staryTexture);
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

}

void levelsel_move(Joystick *joystick, Game *game)
{
    JoystickEvent event;

    if (game->render == LEVELSEL) {
        if (joystick->sample(&event) && event.isButton() && event.value == 1) {
            switch (event.number) {
                default : 
                    break;
                case 0 : 
                    if (dyna2 == fieldIcon) {
#ifdef USE_OPENAL_SOUND
                        play_sound(1, 1.0f, true);
#endif
                        game->render = DISCO;
                    } else if (dyna2 == staryIcon) {
#ifdef USE_OPENAL_SOUND
                        play_sound(5, 1.0f, true);
#endif
                        game->render = STARYNIGHT;
                    }
                    break;
                case 1 : 
                    game->render = MAINMENU;
                    break;
                case 11 :
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
                case 12 :
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

            }
        }
    }


}


