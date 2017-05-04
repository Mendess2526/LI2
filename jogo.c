#define DEBUG
#include "estado.h"
#include "path.h"
#include "html/htmlMaster.h"
#include "score.h"
#include "levelMaker.h"
#include "move_monst.h"
#include "colisions.h"
#include "jogo.h"
/**
\brief Inicializa o estado do jogo
*/
ESTADO inicializar(){

	ESTADO e;
	POSICAO path[MAX_CAMINHO];
	int n=pathMaker(path);
	srandom(time(NULL));
	e.screen=4;					//Ecra onde o começar o jogo
	e.classe=0;					//Classe {Warrior=0, Archer=1, Mage=2}
	e.hp=getClassHp(e.classe);	//Vida do jogador
	e.mp=getClassMp(e.classe);	//Mana do jogador
	e.world_lvl=0;				//Nivel
	e.score=0;					//Score
	e.turn=0;					//Turno
	e.direction=0;				//Lado para que o jogador esta a olhar 0:drt e 1:esq
	e.action=0;					//Action
	e.jog.x=path[0].x;			//Posição do jogador (x)
	e.jog.y=path[0].y;			//Posição do jogador (y)
	e.saida.x=path[n-1].x;		//Posição da saida (x)
	e.saida.y=path[n-1].y;		//Posição da saida (y)
	e.num_monstros=0;			//Numero de Monstros
	e.num_pedras=0;				//Numero de pedras
	e=colocar_pedras(e,n,path);	//Posições da pedras
	e=colocar_monstros(e);		//Posições dos monstros

	return e;
}
/**
\brief Gera um novo nivel quando o jogador chega a saida
@param e Estado do Jogo
*/
ESTADO newLevel(ESTADO e){

	POSICAO path[MAX_CAMINHO];
	int n=pathMaker(path);
	srandom(time(NULL));
	//Vida do jogador
	if(e.hp>(getClassHp(e.classe)-NEW_LEVEL_HP_BONUS)){
		e.hp=getClassHp(e.classe);
	}else{
		e.hp+=NEW_LEVEL_HP_BONUS;
	}
	//Mana do jogador
	if(e.mp>(getClassMp(e.classe)-NEW_LEVEL_MP_BONUS)){
		e.mp=getClassMp(e.classe);
	}else{
		e.mp+=NEW_LEVEL_MP_BONUS;
	}
	e.world_lvl+=1;				//Nivel
	e.score+=NEW_LEVEL_SC_BONUS;//Score
	e.turn=0;					//Turno
	e.direction=0;				//Lado para que o jogador esta a olhar 0:direita e 1:esquerda
	e.action=0;					//Action
	e.jog.x=path[0].x;			//Posição do jogador (x)
	e.jog.y=path[0].y;			//Posição do jogador (y)
	e.saida.x=path[n-1].x;		//Posição da saida (x)
	e.saida.y=path[n-1].y;		//Posição da saida (y)
	e.num_monstros=0;			//Numero de Monstros
	e.num_pedras=0;				//Numero de pedras
	e=colocar_pedras(e,n,path);	//Posições da pedras
	e=colocar_monstros(e);		//Posições dos monstros

	return e;
}
/**
\brief Calcula a nova posição do jogador
@param jog A posição antiga do jogador
@param act Ação selecionada
*/
POSICAO calculaNovaPosicao(POSICAO jog, int act){
	int x[10]={5,-1, 0, 1,-1, 5, 1,-1, 0, 1};
	//         0  1  2  3  4  5  6  7  8  9
	int y[10]={5, 1, 1, 1, 0, 5, 0,-1,-1,-1};

	if(act!=0 && act!=5){
		jog.x+=x[act];
		jog.y+=y[act];
	}
	return jog;
}
ESTADO calcularCombate(ESTADO e){
	return e;
}
/**
\brief Calcula um novo estado conforme a ação que esteja no estado que recebe
@param e Estado do jogo
*/
ESTADO calcularNovoEstado(ESTADO e){
	//novo jogo
	if(e.action==0){
		return inicializar();
	}
	//saida
	if(e.action==5){
		return newLevel(e);
	}
	//set direction
	if(e.action==9 || e.action==6 || e.action==3){
		e.direction=0;
	}
	if(e.action==7 || e.action==4 || e.action==1){
		e.direction=1;
	}
	if(e.action>0 && e.action<10){
		e.jog=calculaNovaPosicao(e.jog,e.action);
	}
	if(e.action>10 && e.action>20){
		e=calcularCombate(e);
	}

	e=move_monstros(e);

	e.turn+=1;
	
	if(e.hp<1){
		e.hp=1;
	}
	if(e.mp<1){
		e.mp=1;
	}

	return e;
}
/**
\brief Lê o estado de um ficheiro
Converte o estado que estava em hexadecimal no ficheiro para uma struct ESTADO e muda a action conforme a que está na QUERY_STRING
@param args QUERY_STRING
@param gamestateFile Apontador para um ficheiro com o estado
*/
ESTADO ler_estado (char *args,FILE *gamestateFile){
	//char str[MAX_BUFFER];
	int act;
	//fgets(str,MAX_BUFFER,gamestateFile);
	ESTADO e = str2estado(gamestateFile);
	sscanf(args,"%d",&act);
	e.action = act;
	return e;
}
/**
\brief Corre o jogo. 
Cria um novo jogo se estiver a começar ou faz "update" ao estado conforme o que o jogador fez.
*/
ESTADO runGame(){
	char *args = getenv("QUERY_STRING");
	FILE *gamestateFile;
	ESTADO e;
	char filepath[15];

	#ifdef DEBUG
	strcpy(filepath,"gamestate");
	#else
	strcpy(filepath,"/tmp/gamestate");
	#endif
	
	if(strlen(args)==0){
		gamestateFile = fopen(filepath,"w");
		e = inicializar();
	}else{
		gamestateFile = fopen(filepath,"r+");
		e = ler_estado(args,gamestateFile);
		e = calcularNovoEstado(e);
		gamestateFile = freopen(filepath,"w",gamestateFile);
	}
	//fprintf(gamestateFile,"%s",estado2str(e));
	estado2str(e,gamestateFile);
	fclose(gamestateFile);
	return e;
}
/**
\brief Main
*/
int main(){

	ESTADO e = runGame();
	if(e.hp<=0){
		updateScoreBoard(e.score);
	}

	imprime(e);

	return 0;
}
