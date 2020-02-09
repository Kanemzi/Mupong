#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define SIGNAL_EXIT 1
#define SIGNAL_WIN 2
#define SIGNAL_LOSE 3



typedef enum bool {
	false = 0, true = 1
} bool;


typedef enum State {
	MENU, CONNECTION, GAME, END_GAME, CONNECTION_LOST, HELP
} State;


typedef enum PlayerState {
	SERVER, CLIENT, UNDEFINED
} PlayerState;


typedef struct Vector {
	char x;
	char y;
} Vector;

typedef struct Player {
	Vector location;
	char width;
	char height;

	char score;
	char speed;

	bool control;
} Player;


typedef struct Ball {
	Vector location;
	char width;
	char height;

	char velX;
	float velY;

} Ball;

/*________________SERIAL______________*/

/* La calculatrice de l'hebergeur se charge de gerer tous les evenements dans le
 * jeu (collision, nouveau point, deplacements de la balle et des joueurs etc...)
 * puis envoie au client les donnes necessaires pour pouvoir dessiner les differents
 * composants du jeu. La calculatrice du client se charge d'envoyer a l'hebergeur, les
 * etats de ses touches directionnelles.
 *
 * [initialisation de la communication : SERVER ENVOIT 'F' (first);
 *										 QUAND CLIENT RECOIT 'F' , IL ENVOIT 'R' (received);
 *										 QUAND SERVER RECOIT 'R' , IL ENVOIT 'C' (confirm)
 *				-> LA PARTIE COMMENCE
 *	]
 */

unsigned char config[] = {0, 9, 0, 0, 0, 0};
char data;
short size;
int err;

typedef struct ServerDataPack {
	char cy; //client y
	char sy; // server y

	char bx; // ball x
  	char by; // ball y

	char ss; // server score
	char cs; // client score

	char signal;

} ServerDataPack;

typedef struct ClientDataPack {
	char keys;

} ClientDataPack;

/*____________________________________*/

typedef struct Variables {
	char keys[KEYSL];
	State gs;
	bool running;
	long ticks;

	Player* player;
	Player* other;
	Ball* ball;

	PlayerState ps;
	bool win;

} Variables;


ServerDataPack getServerData();
unsigned char getClientData();

void sendServerData(ServerDataPack* sd);
void sendClientData(char cd);



/*______________GAME_STATES___________*/

void screen_menu(Variables* v);
void screen_connection(Variables *v);
void screen_game(Variables *v);
void screen_end_game(Variables *v);
void screen_connection_lost(Variables *v);
void screen_help(Variables* v);

/*____________________________________*/

void init_vars(Variables *v);
void game_loop(Variables* v);


void serverInit(Variables* v);
void clientInit(Variables* v);


Player* create_player(char x, char y, bool control);
	void delete_player(Player* p);
Ball* create_ball(char x, char y);
	void delete_ball(Ball* b);

void end_game(Variables *v);

#endif
