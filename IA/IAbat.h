#ifndef __BAT_H__
#define __BAT_H__
#include "IA.h"

ESTADO persegue_bat(ESTADO e,int i,POSICAO p);
POSICAO buscaB1 (ESTADO e,int a[SIZE][SIZE]);
POSICAO buscaB2 (ESTADO e,int a[SIZE][SIZE]);
POSICAO buscaB3 (ESTADO e,int a[SIZE][SIZE]);
POSICAO buscaB4 (ESTADO e,int a[SIZE][SIZE]);
POSICAO mapa1 (ESTADO e,int i,POSICAO p,int q);
int livre (ESTADO e,int x,int y);
ESTADO estrat_bat1 (ESTADO e, int i, POSICAO intersect);
ESTADO defB1 (ESTADO e, int i,POSICAO p,int num);
ESTADO defB2 (ESTADO e, int i,POSICAO p);
ESTADO defB3 (ESTADO e, int i,POSICAO p);
ESTADO defB4 (ESTADO e, int i,POSICAO p);
ESTADO estrat_bat(ESTADO e,int i,POSICAO p);

#endif