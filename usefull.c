#include "usefull.h"
#include "fxlib.h"
#include "MonochromeLib.h"
#define SCA 0xD201D002
#define SCB 0x422B0009
#define SCE 0x80010070

/************************************

Classe d'utilitaires divers

************************************/

typedef int(*sc_i2cp2sip) (char*, char*, short int*, short int*);
typedef int(*sc_iv) (void);
typedef int(*sc_4i) (int, int, int);

const unsigned int sc003b[] = {SCA, SCB, SCE, 0x3B};
const unsigned int sc0015[] = {SCA, SCB, SCE, 0x15};
const unsigned int sc0248[] = {SCA, SCB, SCE, 0x248};

#define RTC_GetTicks (*(sc_iv)sc003b)
#define PutKey (*(sc_4i)sc0248)
#define GlibGetOSVersionInfo (*(sc_i2cp2sip)sc0015)

int OSVersionAsInt(void)
{
	unsigned char mainversion;
	unsigned char minorversion;
	unsigned short release;
	unsigned short build;
	GlibGetOSVersionInfo( &mainversion, &minorversion, &release, &build );
	return ( ( mainversion << 24 ) & 0xFF000000 ) | ( ( minorversion << 16 ) & 0x00FF0000 ) | ( release & 0x0000FFFF );
}
#define isOS2 (OSVersionAsInt() >= 0x02020000)
#define OS2(x,y) ((OSVersionAsInt() >= 0x02020000)?y:x)
static int SysCallCode[] = {0xD201422B,0x60F20000,0x80010070};
static int (*SysCall)( int R4, int R5, int R6, int R7, int FNo ) = (void*)&SysCallCode;

void key_inject1(int keycode)
{
	(*SysCall)(keycode, 0, 0, 0, 0x248);
}


int time_getTicks()
{
	return RTC_GetTicks();
}

int getFps()
{
static unsigned int fps = 0, fps_count = 0, time = 0;

if( time_getTicks() - time >= 128)
		{
			fps = fps_count;
			fps_count = 1;
			time = time_getTicks();
		} else fps_count++;

return fps;
}


int random(int max)
{
static unsigned int seed = 0x12345678;
if(max<1)seed=-max;
seed = (0x41C64E6D * seed) + 0x3039;
if(max>0)return ((seed >> 16) % max);

}

int abs(int nombre)
{
return (nombre<0)?-nombre:nombre;
}

void key_inject(int keycode)
{
	PutKey(-1, -1, keycode);
}

static void delay()
{
	unsigned char i;
	for(i=0 ; i<5 ; i++);
}

unsigned char CheckKeyRow(unsigned char code)
{
	unsigned char result=0;
	short*PORTB_CTRL=(void*)0xA4000102;
	short*PORTM_CTRL=(void*)0xA4000118;
	char*PORTB=(void*)0xA4000122;
	char*PORTM=(void*)0xA4000138;
	char*PORTA=(void*)0xA4000120;
	short smask;
	char cmask;
	unsigned char column, row;

	column = code>>4;
	row = code &0x0F;

	smask = 0x0003 << (( row %8)*2);
	cmask = ~( 1 << ( row %8) );
	if(row <8)
	{
		*PORTB_CTRL = 0xAAAA ^ smask;
		*PORTM_CTRL = (*PORTM_CTRL & 0xFF00 ) | 0x00AA;
		delay();
		*PORTB = cmask;
		*PORTM = (*PORTM & 0xF0 ) | 0x0F;
	}
	else
	{
		*PORTB_CTRL = 0xAAAA;
		*PORTM_CTRL = ((*PORTM_CTRL & 0xFF00 ) | 0x00AA)  ^ smask;
		delay();
		*PORTB = 0xFF;
		*PORTM = (*PORTM & 0xF0 ) | cmask;
	}
	delay();
	result = (~(*PORTA))>>column & 1;
	delay();
	*PORTB_CTRL = 0xAAAA;
	*PORTM_CTRL = (*PORTM_CTRL & 0xFF00 ) | 0x00AA;
	delay();
	*PORTB_CTRL = 0x5555;
	*PORTM_CTRL = (*PORTM_CTRL & 0xFF00 ) | 0x0055;
	delay();



	return result;
}



unsigned char key_down(unsigned char code)
{
	unsigned short key[8];
	const unsigned short* keyboardregister = (unsigned short*)0xA44B0000;
	unsigned char row;

	if(isOS2)
        {
		memcpy(&key, keyboardregister, sizeof(unsigned short) << 3);
		row= code%10;
		return (0 != (key[row >> 1] & 1 << code / 10 - 1 + ((row & 1) << 3)));
        }
	else
        {
        return CheckKeyRow((code % 10) + ((code / 10 - 1) << 4));
        }
}





void setFps(int fpsWish)
{
static unsigned int fps = 0, fps_count = 0;

do
{
fps = time_getTicks();
}
while(fps < fps_count+fpsWish);
fps_count = time_getTicks();

}


int max(int a,int b)
{
if(a < b)return b;
return a;
}
int min(int a,int b)
{
if(a>b)return b;
return a;
}


int writeFile(unsigned char* name,unsigned char* extension,unsigned char* source,int taille)
{

FONTCHARACTER filename[20]={'\\','\\','f','l','s','0','\\'};
int a = 0;
int handle;
do
    {
    filename[7+a]=name[a];
    a++;
    }while(name[a] != 0 && a<7);
filename[7+a] = '.';
filename[8+a] = extension[0];
filename[9+a] = extension[1];
filename[10+a] = extension[2];
filename[11+a] = 0;

Bfile_DeleteFile(filename);
Bfile_CreateFile(filename,taille);
handle = Bfile_OpenFile(filename,_OPENMODE_WRITE);
if(handle>-1)
    {
    Bfile_WriteFile(handle,source,taille);
    Bfile_CloseFile(handle);
    return 1;
    }
return 0;
}


int readFile(unsigned char* name,unsigned char* extension,unsigned char* reception)
{

int taille;
FONTCHARACTER filename[20]={'\\','\\','f','l','s','0','\\'};
int handle;
int a = 0;
do
    {
    filename[7+a]=name[a];
    a++;
    }while(name[a] !=  0 && a<7);
filename[7+a] = '.';
filename[8+a] = extension[0];
filename[9+a] = extension[1];
filename[10+a] = extension[2];
filename[11+a] = 0;


handle = Bfile_OpenFile(filename,_OPENMODE_READ);

if(handle<0)return 0;
taille = Bfile_GetFileSize(handle);
if(handle>-1)
    {
    Bfile_ReadFile(handle,reception,taille,0);

    Bfile_CloseFile(handle);
return 1;
    }
return 0;
}


