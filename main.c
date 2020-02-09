#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "fxlib.h"


#include "MonochromeLib.h"
#include "serial.h"
#include "usefull.h"
#include "keyboard.h"

#include "sprites.h"

#include "main.h"

int main()
{

	Variables var;

	err = Serial_Open(config);
	Serial_ClearReceiveBuffer();
	Serial_ClearTransmitBuffer();

		init_vars(&var);

		ML_clear_vram();
		game_loop(&var);


	err = Serial_Close(0);

	return 0;
}








void game_loop(Variables* v) {
	int i;
	while(v->running) {

		ML_clear_vram();

		updateKeys();
		for(i = 0 ; i < KEYSL ; i++) {
			*(v->keys+i) = *(getKeys()+i);
		}

		switch(v->gs) {
			case MENU:
				screen_menu(v);
				break;

			case CONNECTION:
				screen_connection(v);
				break;

			case GAME:
				screen_game(v);
				break;

			case END_GAME:
				screen_end_game(v);
				break;

			case CONNECTION_LOST:
				screen_connection_lost(v);
				break;

			case HELP:
				screen_help(v);
				break;

		}
		ML_display_vram();
		v->ticks++;
		Sleep(2);
		setFps(4); // 4 -> 32fps
	}
}
 






/*________________SERIAL_____________*/

ServerDataPack getServerData() {
	ServerDataPack sd;
	err = Serial_ReadBytes(&sd, sizeof(ServerDataPack), &size);
	return sd;
}
unsigned char getClientData() {
	char cd;
	err = Serial_ReadByte(&cd);
	return cd;
}
void sendServerData(ServerDataPack* sd) {
	err = Serial_WriteBytes(sd, sizeof(ServerDataPack));
}
void sendClientData(char cd) {
	err = Serial_WriteByte(cd);
}

/*___________________________________*/










/*______________GAME_STATES___________*/

void screen_menu(Variables* v) { // done
	typedef enum Button { PLAY, HOST, JOIN, B_HELP, EXIT } Button;
	static Button b = PLAY;
	static bool play_selected = false,
				pressed = false;
	static init = false;
	if(!init) {
		init_vars(v);
		init = true;
	}

	{ /*update*/

	if(!pressed) {
		if(v->keys[K_DOWN] ) { // DOWN
			pressed = true;

			if(b==PLAY) {
				b = B_HELP;
			}else if(b==HOST) {
				b = B_HELP;
				if(play_selected) play_selected = false;
			}else if(b==JOIN) {
				b = B_HELP;
				if(play_selected) play_selected = false;
			}else if(b==B_HELP) {
				b = EXIT;
			}else if(b==EXIT) {
				b = PLAY;
			}
		}else if(v->keys[K_UP]) { // UP
			pressed = true;

			if(b==PLAY) {
				b = EXIT;
			}else if(b==HOST) {
				b = EXIT;
				if(play_selected) play_selected = false;
			}else if(b==JOIN) {
				b = EXIT;
				if(play_selected) play_selected = false;
			}else if(b==B_HELP) {
				b = PLAY;
			}else if(b==EXIT) {
				b = B_HELP;
			}

		}else if(v->keys[K_LEFT]) { // LEFT
			pressed = true;
			if(b==JOIN) b = HOST;

		}else if(v->keys[K_RIGHT]) { // RIGHT
			pressed = true;
			if(b==HOST) b = JOIN;

		}else if(v->keys[K_EXE]) { // EXE (SELECT BUTTON)
			pressed = true;

			if(b==PLAY) {				//Changement d'ecran :
				play_selected = true;
				b = HOST;

			}else if(b==HOST) {
				v->ps = SERVER;
				v->gs = CONNECTION;
				init = false;

			}else if(b==JOIN) {
				v->ps = CLIENT;
				v->gs = CONNECTION;
				init = false;

			}else if(b==B_HELP) {
				v->gs = HELP;
				init = false;

			}else if(b==EXIT) {
				v->running = false;
				init = false;
				return;
			}
		}/*else if(v->keys[K_F6]) {
			v->running = false;
			return;
		}*/
	}

	if(!v->keys[K_UP] && !v->keys[K_DOWN] && !v->keys[K_LEFT] && !v->keys[K_RIGHT] && !v->keys[K_EXE])
		pressed = false;

	}

	{ /*render*/
		ML_bmp_or(background, 0, 0, BACKGROUND_WIDTH, BACKGROUND_HEIGHT);
		ML_bmp_or(title, SCREEN_WIDTH/2 - TITLE_WIDTH/2 , 4, TITLE_WIDTH, TITLE_HEIGHT);

		#define SBUTTONH 22

		if(!play_selected) {
			ML_bmp_or(play_button, SCREEN_WIDTH/2 - BUTTON_WIDTH/2, SBUTTONH, BUTTON_WIDTH, BUTTON_HEIGHT);
		}else{
			ML_bmp_or(host_button, SCREEN_WIDTH/2 - BUTTON_WIDTH/2, SBUTTONH, SUBBUTTON_WIDTH, SUBBUTTON_HEIGHT);
			ML_bmp_or(join_button, SCREEN_WIDTH/2 - BUTTON_WIDTH/2 + SUBBUTTON_WIDTH +1, SBUTTONH, SUBBUTTON_WIDTH, SUBBUTTON_HEIGHT);
		}
		ML_bmp_or(help_button, SCREEN_WIDTH/2 - BUTTON_WIDTH/2, SBUTTONH+1 + BUTTON_HEIGHT, BUTTON_WIDTH, BUTTON_HEIGHT);
		ML_bmp_or(exit_button, SCREEN_WIDTH/2 - BUTTON_WIDTH/2, SBUTTONH+2 + BUTTON_HEIGHT*2, BUTTON_WIDTH, BUTTON_HEIGHT);

		switch(b) {
			case PLAY:
				ML_rectangle(SCREEN_WIDTH/2 - BUTTON_WIDTH/2 + 1,    SBUTTONH +1 ,                       SCREEN_WIDTH/2 - BUTTON_WIDTH/2 + BUTTON_WIDTH-2,        SBUTTONH - 2 + BUTTON_HEIGHT,         0, ML_WHITE, ML_XOR);
				break;
			case B_HELP:
				ML_rectangle(SCREEN_WIDTH/2 - BUTTON_WIDTH/2 + 1,    SBUTTONH +2 + BUTTON_HEIGHT,        SCREEN_WIDTH/2 - BUTTON_WIDTH/2 + BUTTON_WIDTH-2,        SBUTTONH - 1 + BUTTON_HEIGHT*2,   0, ML_WHITE, ML_XOR);
				break;
			case EXIT:
				ML_rectangle(SCREEN_WIDTH/2 - BUTTON_WIDTH/2 + 1,    SBUTTONH + 3 + BUTTON_HEIGHT*2,     SCREEN_WIDTH/2 - BUTTON_WIDTH/2 + BUTTON_WIDTH-2,        SBUTTONH + BUTTON_HEIGHT*3,   0, ML_WHITE, ML_XOR);
				break;
			case HOST:
				ML_rectangle(SCREEN_WIDTH/2 - BUTTON_WIDTH/2 + 1,    SBUTTONH +1 ,                       SCREEN_WIDTH/2 - BUTTON_WIDTH/2 + SUBBUTTON_WIDTH-2,        SBUTTONH - 2 + BUTTON_HEIGHT,         0, ML_WHITE, ML_XOR);
				break;
			case JOIN:
				ML_rectangle(SCREEN_WIDTH/2 - BUTTON_WIDTH/2 + 2 + SUBBUTTON_WIDTH,    SBUTTONH +1 ,                       SCREEN_WIDTH/2 - BUTTON_WIDTH/2 + SUBBUTTON_WIDTH*2-1,        SBUTTONH - 2 + BUTTON_HEIGHT,         0, ML_WHITE, ML_XOR);
				break;
		}
	}
}












void screen_connection(Variables *v) {
	{ /*update    -> EXPERIMENTAL */
		static bool initConnection = false;
		if(v->ps == SERVER) {
			if(!initConnection) {
				initConnection = true; //active une fois
				err = Serial_WriteByte('F');
			}
			if(!Serial_ReadByte(&data)) {
				if(data == 'R') {
					err = Serial_WriteByte('C');
					//lancement de la partie cote serveur :
					initConnection = false;
					v->gs = GAME;
				}
			}
		}else if(v->ps == CLIENT) {
			if(!Serial_ReadByte(&data)) {
				if(data == 'F') {
					err = Serial_WriteByte('R');
				}else if(data == 'C') {
					//lancement de la partie cote client :
					v->gs = GAME;
					initConnection = false;
				}
			}
		}
/***************************TODO _> GOTO MENU*******GAME: DEBUG***********************************/
		if(v->keys[K_F6]) {
			v->gs = MENU; // CONNECTION
			return;
		}
	}

	{ /*render*/


		ML_bmp_or(background, 0, 0, BACKGROUND_WIDTH, BACKGROUND_HEIGHT);
		ML_bmp_or(back_f6, 4, SCREEN_HEIGHT - BACK_HEIGHT - 3, BACK_WIDTH, BACK_HEIGHT);

		ML_bmp_or(connecting_label, SCREEN_WIDTH/2 - COLABEL_WIDTH/2, SCREEN_HEIGHT/2 - COLABEL_HEIGHT - 12, COLABEL_WIDTH, COLABEL_HEIGHT);
		ML_bmp_or(connecting, SCREEN_WIDTH/2 - CONNECTING_WIDTH/2, SCREEN_HEIGHT/2 - CONNECTING_HEIGHT/2, CONNECTING_WIDTH, CONNECTING_HEIGHT);
	}
}















#define Y_MIN 8
#define Y_MAX 62
#define Y_MIDDLE (Y_MIN+Y_MAX)/2
#define X_MIN 1 // LINE
#define X_MAX 126 // LINE
#define X_MIDDLE 64

#define DEFAULT_TICKS 3*32
#define BALL_XDEFAULT X_MIDDLE-BALL_WIDTH/2
#define BALL_YDEFAULT Y_MIDDLE-BALL_HEIGHT/2

#define MAX_VSPEED 20 // /10
#define MIN_VSPEED 1

void screen_game(Variables *v) {

    static bool ballStarted = false;
    static int ticksBeforeBallStart = DEFAULT_TICKS;

	char exit = 0;
	char game_end = 0;
	PlayerState winner = 0;

	unsigned char client_keys = 0; // si client;
	ServerDataPack server_data;

	Player* player = v->player;
	Player* other = v->other;

	static Player* serve;


	static bool init = false;
	{ /*update -> experimental*/
		if(!init) { // active une fois :
			init = true;
			if(v->ps == SERVER) {
				serverInit(v);
				serve = player;
			}else if(v->ps == CLIENT) {
				clientInit(v);
			}
			server_data.signal = 0; // ça serait bête de commencer la partie EN quittant la partir ... ;)
		}

		/*Detect keys */

			/***************************  SERVER  ********************************/

		if(v->ps == SERVER) { // server update

				if(Serial_GetRxBufferSize() > 0) { // recuperation des touches presses par le client (sous forme d'un seul char)
					data = getClientData();
				}

				// traitement des donnes -> deplacement du client
				{
					if((data & 1) == 1 && other->location.y > Y_MIN) { // touche K_UP pressee par client
						other->location.y -= other->speed;
					}else if((data & 2) == 2 && other->location.y + other->height < Y_MAX) { // touche K_DOWN pressee par client
						other->location.y += other->speed;
					}
				}
				//deplacement du serveur
				{
					if(v->keys[K_UP] && player->location.y > Y_MIN) { // touche K_UP pressee par serveur
						player->location.y -= player->speed;
					}else if(v->keys[K_DOWN] && player->location.y + player->height < Y_MAX) { // touche K_DOWN pressee par serveur
						player->location.y += player->speed;
					}
				}

                /*déplacement de la balle*/
                if(!ballStarted) {
                    ticksBeforeBallStart--;
                    if(!ticksBeforeBallStart) {
                        ballStarted = true;
                        ticksBeforeBallStart = DEFAULT_TICKS;
                    }
                } else {
                    //ball update
                    {
                        Ball ball = *(v->ball);
                        Vector location = ball.location;

                        static bool startDir = false;
                        if(!startDir) {
                            startDir = true;
                            v->ball->velX = (random(2)==0) ? 2 : -2;
                        }



                        if(v->ball->velX > 0) { // la balle va vers la droite -> collision other
                            if(location.y + ball.height >= v->other->location.y &&
                               location.y <= v->other->location.y + v->other->height) { // x aligned to other
                                    if(location.x+ball.width+ball.velX > v->other->location.x && location.x+ball.width+ball.velX < v->other->location.x + v->other->width) { // COLLISION TO SERVER
                                        v->ball->velX = -(v->ball->velX); // reverse velX
                                        if(ball.location.y+ball.height/2 < v->other->location.y + v->other->height/2) { // modifier la velY selon la location de la balle par rapport au player

                                            {
                                                float vspeed = -(random(MAX_VSPEED)/10)-MIN_VSPEED;
                                                v->ball->velY = vspeed;
                                            }
                                        }else{
                                            {
                                                float vspeed = random(MAX_VSPEED)/10+MIN_VSPEED;
                                                v->ball->velY = vspeed;
                                            }
                                        }
                                    }
                            }
                        }else if(v->ball->velX < 0) { // la balle va vers la gauche -> collision player
                            if(location.y + ball.height >= v->player->location.y &&
                               location.y <= v->player->location.y + v->player->height) { // x aligned to player
                                    if(location.x+ball.velX < v->player->location.x + v->player->width && location.x+ball.velX > v->player->location.x) { // COLLISION TO SERVER
                                        v->ball->velX = -(v->ball->velX); // reverse velX
                                        if(ball.location.y+ball.height/2 < v->player->location.y + v->player->height/2) { // modifier la velY selon la location de la balle par rapport au player

                                            {
                                                float vspeed = -(random(MAX_VSPEED)/10)-MIN_VSPEED;
                                                v->ball->velY = vspeed;
                                            }
                                        }else{
                                            {
                                                float vspeed = random(MAX_VSPEED)/10+MIN_VSPEED;
                                                v->ball->velY = vspeed;
                                            }
                                        }
                                    }
                            }
                        }

                        //up/down collision
                        if(ball.location.y < Y_MIN || ball.location.y + ball.height > Y_MAX) {
                            v->ball->velY = -(v->ball->velY); // inverse la vel Y;
                        }


                        //ball out of screen
                        if(location.x < 0) { // point to other
                            v->other->score++;
                            v->ball->location.x = BALL_XDEFAULT;
                            v->ball->location.y = BALL_YDEFAULT;
                            v->ball->velY = 0;
                            ballStarted = false;
                            startDir = false;
                        }else if(location.x + ball.width > SCREEN_WIDTH) { // point to player
                            v->player->score++;
                            v->ball->location.x = BALL_XDEFAULT;
                            v->ball->location.y = BALL_YDEFAULT;
                            v->ball->velY = 0;
                            startDir = false;
                            ballStarted = false;
                        }

                        //detection de la fin de la partie (score = 10)
                        if(v->player->score >= 10) { // hoster wins
                        	game_end = 1;
                        	winner = SERVER;
                        }else if(v->other->score >= 10) { //joiner wins
                        	game_end = 1;
                        	winner = CLIENT;
                        }

                    }
                    v->ball->location.x += v->ball->velX;
                    v->ball->location.y += v->ball->velY;
                }


                /* ball-end */

				if(v->keys[K_F6] || (data & 8) == 8) { // exit game ->goto connection error screen
					exit = 1;
				}

				//preparation du sdp
				server_data.cy = v->other->location.y;
				server_data.sy = v->player->location.y;
				server_data.bx = v->ball->location.x;
				server_data.by = v->ball->location.y;
				server_data.ss = v->player->score;
				server_data.cs = v->other->score;
				if(exit==1) server_data.signal = SIGNAL_EXIT;
				else if(game_end == 1) {
					if(winner == SERVER) {
						server_data.signal = SIGNAL_LOSE;
						v->win = true;
					}else if(winner == CLIENT) {
						server_data.signal = SIGNAL_WIN;
						v->win = false;
					}
				}

				sendServerData(&server_data);

				if(data==31) { // serveur affiche ecran d'erreur
					init = false;
					v->gs = CONNECTION_LOST;
				}else if(data==63) {
					init = false;
					v->gs = END_GAME;
				}



				/***************************  CLIENT  ********************************/

		} else if(v->ps == CLIENT) { // client update

				if(v->keys[K_UP]) {
					client_keys |= 1;
				}
				if(v->keys[K_DOWN]) {
					client_keys |= 2;
				}
				if(v->keys[K_F1]) {
					client_keys |= 4;
				}
				if(v->keys[K_F6]) {
					client_keys |= 8;
				}

				/*send_clientdata*/
				sendClientData(client_keys);


				/*receive_serverdata*/
				if( Serial_GetRxBufferSize() > sizeof(ServerDataPack) ) { // si le buffer de reception contient au moins 1 sdp;
					server_data = getServerData(); // on recupere le sdp;

					//stokage des donées server dans les objets players
					player->location.y = server_data.cy;
					player->score = server_data.cs;

					other->location.y = server_data.sy;
					other->score = server_data.ss;

					v->ball->location.y = server_data.by;
					v->ball->location.x = server_data.bx;

					if(server_data.signal==SIGNAL_EXIT) {
						sendClientData(31); // -> le serveur peu se déconnecter
						init = false;
						v->gs = CONNECTION_LOST;
					}else {
						if(server_data.signal==SIGNAL_WIN) {
							sendClientData(63);
							init = false;
							v->gs = END_GAME;
							v->win = true;
						} else if(server_data.signal==SIGNAL_LOSE) {
							sendClientData(63);
							init = false;
							v->gs = END_GAME;
							v->win = false;
						}
					}
				}
		}
	}
    {
        char serverScore[20];
        char clientScore[20];


        static Vector lastBallLocation = {0, 0};



        if(v->ps == SERVER) {
            sprintf(serverScore, "Hoster : %d", v->player->score);
            sprintf(clientScore, "Joiner : %d", v->other->score);
        }else if(v->ps == CLIENT) {
            sprintf(serverScore, "Hoster : %d", v->other->score);
            sprintf(clientScore, "Joiner : %d", v->player->score);
        }


        if(!exit){ /*render*/

            ML_line(X_MIN+1, Y_MIN-1, X_MAX, Y_MIN-1, ML_BLACK);
            ML_line(X_MIN+1, Y_MAX+1, X_MAX, Y_MAX+1, ML_BLACK);
            ML_line(X_MIN  , Y_MIN, X_MIN  , Y_MAX+1, ML_BLACK);
            ML_line(X_MAX  , Y_MIN, X_MAX  , Y_MAX+1, ML_BLACK);

            //render scores
            PrintMini(0, 1, serverScore, 0);
            PrintMini(SCREEN_WIDTH/2+20, 1, clientScore, 0);

            //render players & ball

            ML_bmp_or_cl(player_sprite, v->player->location.x, v->player->location.y, PLAYER_WIDTH, PLAYER_HEIGHT);
            ML_bmp_or_cl(player_sprite, v->other->location.x, v->other->location.y, PLAYER_WIDTH, PLAYER_HEIGHT);

            ML_bmp_or_cl(ball_sprite, lastBallLocation.x, lastBallLocation.y, BALL_WIDTH, BALL_HEIGHT); // trace
            ML_pixel(lastBallLocation.x+1, lastBallLocation.y+1, ML_BLACK); // effet visuel stylé
            ML_bmp_or_cl(ball_sprite, v->ball->location.x, v->ball->location.y, BALL_WIDTH, BALL_HEIGHT);


            ML_rectangle(0, Y_MIN, X_MIN-2, Y_MAX, 1, ML_WHITE, ML_WHITE);
            ML_rectangle(X_MAX+1, Y_MIN, SCREEN_WIDTH, Y_MAX, 1, ML_WHITE, ML_WHITE);

            lastBallLocation.x = v->ball->location.x;
            lastBallLocation.y = v->ball->location.y;
        }
    }
}

















void end_game(Variables *v) {
	delete_player(v->player);
	delete_player(v->other);
	delete_ball(v->ball);
}









void screen_end_game(Variables *v) {
	
	static init = false;

	if(!init) {
		init = true;
		
	}

	if(v->keys[K_F6]) {
		v->gs = MENU;
		end_game(v);
		init = false;
		return;
	}

	//render
	{
		ML_bmp_or(background, 0, 0, BACKGROUND_WIDTH, BACKGROUND_HEIGHT);
		ML_bmp_or(back_f6, 4, SCREEN_HEIGHT - BACK_HEIGHT - 3, BACK_WIDTH, BACK_HEIGHT);

		if(v->win == true) {
			ML_bmp_or(win, SCREEN_WIDTH/2 - WIN_WIDTH/2, SCREEN_HEIGHT/2 - END_GAME_MESSAGE_HEIGHT/2, WIN_WIDTH, END_GAME_MESSAGE_HEIGHT);
		}else if(v->win == false) {
			ML_bmp_or(lose, SCREEN_WIDTH/2 - LOSE_WIDTH/2, SCREEN_HEIGHT/2 - END_GAME_MESSAGE_HEIGHT/2, LOSE_WIDTH, END_GAME_MESSAGE_HEIGHT);
		}
	}
}









void screen_connection_lost(Variables *v) {

	static init = false;
	if(!init && v->keys[K_F6]) {
		return;
	}
	init = true;

	if(v->keys[K_F6]) {
			v->gs = MENU; // CONNECTION
			end_game(v);
			init = false;
			return;
	}

	{ /*render*/


		ML_bmp_or(background, 0, 0, BACKGROUND_WIDTH, BACKGROUND_HEIGHT);
		ML_bmp_or(back_f6, 4, SCREEN_HEIGHT - BACK_HEIGHT - 3, BACK_WIDTH, BACK_HEIGHT);

		ML_bmp_or(connection_error_label, SCREEN_WIDTH/2 - COLABEL_WIDTH/2, SCREEN_HEIGHT/2 - COLABEL_HEIGHT - 12, COLABEL_WIDTH, COLABEL_HEIGHT);
		ML_bmp_or(connection_error, SCREEN_WIDTH/2 - CONNECTING_WIDTH/2, SCREEN_HEIGHT/2 - CONNECTING_HEIGHT/2, CONNECTING_WIDTH, CONNECTING_HEIGHT);
	}
}








void screen_help(Variables* v) { // done
	ML_bmp_or(help, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	if(v->keys[K_F6]) {
		v->gs = MENU;
		return;
	}
}
/*____________________________________*/










/*___________INGAME_FUNCTION__________*/


void serverInit(Variables* v) {
	v->player = create_player(X_MIN+4, Y_MIDDLE-PLAYER_HEIGHT/2, true);
	v->other = create_player(X_MAX-PLAYER_WIDTH-3, Y_MIDDLE-PLAYER_HEIGHT/2, false);
	v->ball = create_ball(BALL_XDEFAULT, BALL_YDEFAULT);

}



void clientInit(Variables* v) {
	v->other = create_player(X_MIN+4, Y_MIDDLE-PLAYER_HEIGHT/2, false);
	v->player = create_player(X_MAX-PLAYER_WIDTH-3, Y_MIDDLE-PLAYER_HEIGHT/2, false);
	v->ball = create_ball(X_MIDDLE-BALL_WIDTH/2, Y_MIDDLE-BALL_HEIGHT/2);
}
/*____________________________________*/








void init_vars(Variables *v) {
	int i;
	for(i = 0 ; i < KEYSL ; i++) {
		*(v->keys+i) = 0;
	}

	v->gs = MENU;
	v->running = true;
	v->ticks = 0;
	v->ps = UNDEFINED;
	v->win = false;
}







Player* create_player(char x, char y, bool control) {
	Player* p = NULL;

	while(p==NULL) {
		p = malloc( sizeof(Player) );
	}

	p->location.x = x;
	p->location.y = y;
	p->width = 3;
	p->height = 12;

	p->score = 0;
	p->speed = 2;

	p->control = control;

	return p;
}







Ball* create_ball(char x, char y) {
	Ball* b = NULL;

	while(b==NULL) {
		b = malloc( sizeof(Ball) );
	}

	b->location.x = x;
	b->location.y = y;
	b->width = 4;
	b->height = 4;
	b->velX = 0;
	b->velY = 0;

	return b;
}







void delete_player(Player* p) {
	free(p);
}



void delete_ball(Ball* b) {
	free(b);
}













int AddIn_main(int isAppli, unsigned short OptionNum) {
	return main();
}

#pragma section _BR_Size
unsigned long BR_Size;
#pragma section
#pragma section _TOP

int InitializeSystem(int isAppli, unsigned short OptionNum) {
	return INIT_ADDIN_APPLICATION(isAppli, OptionNum);
}
	#pragma section

