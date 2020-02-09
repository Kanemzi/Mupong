#ifndef USEFULL_H
#define USEFULL_H

#include "stdio.h"

#define OS2(x,y) ((OSVersionAsInt() >= 0x02020000)?y:x)

void key_inject(int keycode);
void key_inject1(int keycode);
unsigned char key_down(unsigned char code); // fonction plus rapide et compatible SH4 de IsKeyDown()
int time_getTicks(); //renvoye le nombre de ticks (1/128 de seconde) depuis minuit selon le processeur de la calto
int getFps(); //renvoye le nombre de FPS
void setFps(int fpsWish); //regle le nombre de fps : 1 pour 128 FPS,2 pour 64, 4 pour 32, 5 pour 25 => 128/fpsWish
int max(int a,int b);  //renvoye le max
int min(int a,int b);
int random(int max);
int abs(int nombre);
int writeFile(unsigned char* name,unsigned char* extension,unsigned char* source,int taille);
/*
fonction de sauvegarde
ecrit taille octets du tableau source dans le fichier name.extension
Pour ecrire un tableau d'int, castez simplement le tableau (unsigned char*)tableauDInt
Pour ecrire un int simple, faites writeFile(name,extension,(unsigned char*)&variable,4);
*/
int readFile(unsigned char* name,unsigned char* extension,unsigned char* reception);
/*
fonction de chargement de sauvegarde
ecrit le contenu du fichier name.extension dans le tableau reception
pour lui passer un tableau d'int, castez simplement le tableau (unsigned char*)tableauDInt
Pour ecrire dans un int simple, faites writeFile(name,extension,(unsigned char*)&variable); mais faites attention a ce que le fichier ne soit pas >4 octets
*/

#define K_0 71
#define K_1 72
#define K_2 62
#define K_3 52
#define K_4 73
#define K_5 63
#define K_6 53
#define K_7 74
#define K_8 64
#define K_9 54

#define K_DP 61

#define K_EXP 51
#define K_PMINUS 41
#define K_PLUS 42
#define K_MINUS 32
#define K_MULT 43
#define K_DIV 33
#define K_FRAC 75
#define K_LPAR 55
#define K_RPAR 45
#define K_COMMA 35
#define K_STORE 25
#define K_LOG 66
#define K_LN 56
#define K_SIN 46
#define K_COS 36
#define K_TAN 26
#define K_SQUARE 67

#define K_POW 57

#define K_EXE 31
#define K_DEL 44
#define K_AC 32
#define K_FD 65
#define K_EXIT 47
#define K_SHIFT 78
#define K_ALPHA 77
#define K_OPTN 68
#define K_VARS 58
#define K_UP 28
#define K_DOWN 37
#define K_LEFT 38
#define K_RIGHT 27
#define K_F1 79
#define K_F2 69
#define K_F3 59
#define K_F4 49
#define K_F5 39
#define K_F6 29
#define K_MENU 48
#define K_XT 76


#endif
