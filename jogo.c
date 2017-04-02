#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "jogo.h"
#include "browser.h"
#include "path.h"

/* Verifica se esta posição está em cima do caminho entre o heroi e a saida */
int isOnPath(ESTADO e, POSICAO p, int pathSize, POSICAO path[]){ 
	int i, flag;
	flag=0;
	for (i=1;i<MAX_CAMINHO && !flag;i++){
		if (path[i].x == p.x || path[i].y == p.y){
			flag=1;
		}
	}
	return flag;
}

/* verifica se esta algum monstro, pedra ou jogador nesta posição*/
int pos_ocupada (ESTADO e, POSICAO p){
	return com_jogador(e,p) || com_pedras(e,p) || com_monstros(e,p);
}

/* Coloca uma pedra numa posição aleatoria */
ESTADO colocar_pedra (ESTADO e, int pathSize, POSICAO path[]){
	POSICAO p;
	for(e.num_pedras=0; e.num_pedras < 20; e.num_pedras++){
		p.x=(char) random() % SIZE;
		p.y=(char) random() % SIZE;
		if (pos_ocupada(e,p) || isOnPath(e,p,pathSize,path)){
			e.num_pedras--;
		}else{
			e.pedras[(int) e.num_pedras].x=p.x;
			e.pedras[(int) e.num_pedras].y=p.y;
		}
	}
	return e;
}

/* Coloca um monstro numa posição aleatoria */
ESTADO colocar_monstro (ESTADO e){
	POSICAO p;
	for(e.num_monstros=0;e.num_monstros<10;e.num_monstros++){
		p.x=(char) random() % SIZE;
		p.y=(char) random() % SIZE;
		if (pos_ocupada(e,p)){
			e.num_monstros--;
		}else{
			e.monstros [(int) e.num_monstros].x=p.x;
			e.monstros [(int) e.num_monstros].y=p.y;
		}
	}
	return e;
}

/* Coloca todas as pedras para inicializar o estado */
ESTADO colocar_pedras (ESTADO e, int pathSize, POSICAO path[]){
	int i;
	for(i=0;i<MAX_PEDRAS;i++){
		e=colocar_pedra (e,pathSize,path);
	}
	return e;
}

/* Coloca todas os monstros para inicializar o estado */
ESTADO colocar_monstros (ESTADO e, int pathSize, POSICAO path[]){
	int i;
	for (i=0;i<MAX_MONSTROS;i++){
		e=colocar_monstro(e,pathSize,path);
	}
	return e;
}

/* Inicializa o estado do jogo */
ESTADO inicializar(){

	ESTADO e;

	POSICAO path[MAX_CAMINHO];
	int n=pathMaker(path);
	
	e.jog.x=path[0].x;
	e.jog.y=path[0].y;
	e.saida.x=path[n-1].x;
	e.saida.y=path[n-1].y;
	
	e=colocar_monstros(e,n,path,MAX_MONSTROS);
	e=colocar_pedras(e,n,path,MAX_PEDRAS);
	
	return e;
}

/* Verifica se o jogador esta num certo par de coordenadas */
int com_jogador (ESTADO e,POSICAO p){
	return (e.jog.x == p.x) && (e.jog.y == p.y);
}

/* Verifica se existem pedras nas Coordenadas dadas */
int com_pedras (ESTADO e, POSICAO p){
	int i;
	for(i=0;i<MAX_PEDRAS;i++){
		if (e.pedras[i].x == p.x && e.pedras[i].y == p.y){
			return 1;
		}
	}
	return 0;
}

/* Verifica se existem monstros nas coordenadas dadas */
int com_monstros (ESTADO e, POSICAO p){
	int i;
	for (i=0;i<MAX_MONSTROS;i++){
		if (e.monstros[i].x == p.x && e.monstros[i].y == p.y){
			return 1;
		}
	}
	return 0;
} 

ESTADO ler_estado (char *args){
	if(strlen(args) == 0){
		return inicializar();
	}
	return str2estado(args);
}

int main(){
	print_header ();
	srandom(time(NULL));
	int x,y;
	POSICAO p;

	ESTADO e = ler_estado(getenv("QUERY_STRING"));
	for(y = 0; y < 10; y++){
		for(x = 0; x < 10; x++){
			p.x=(char) x;
			p.y=(char) y;
			imprime_casa(p);
		}
	}
	imprime_jogador(e);
	imprime_monstros(e);
	imprime_pedras(e);
	print_footer ();

	return 0;
}