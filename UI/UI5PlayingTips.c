#include "UI5PlayingTips.h"

void imprime_texto(int x, int y, char *text, int fontSize){
	int i=0;
	int lineNumber=0;
	printf("<text x=%d y=%d style=\"fill:#FFFFFF;font-size:%dpx\">\n",x,y,fontSize);
	while(text[i]){
		char line[50];
		int lineI=0;
		while(text[i] && text[i]!='\n'){
			line[lineI++]=text[i++];
		}
		line[lineI]='\0';
		printf("\t<tspan x=%d y=%d >%s</tspan>\n",x,y+((fontSize+1)*lineNumber),line);
		lineNumber++;
		if(text[i]=='\n'){i++;};
	}
	printf("</text>\n");
}
void imprime_helpButton(char *name){
	ABRIR_LINK(name,"10");
	IMAGEM(745,310,TAM,TAM,"button_help_toggle.png");
	FECHAR_LINK;
}
void imprime_helpBoss(char *name, POSICAO pos){
	char query[6];
	sprintf(query,"%d",10000+(pos.x*100)+pos.y);
	ABRIR_LINK(name,query);
	IMAGEM((pos.x+1)*TAM,(pos.y+1)*TAM,200,150,"button_help_ask.png");
	FECHAR_LINK;
}
void imprime_helpEnemies(char *name, MSTR monstros[], int num_monstros){
	int i;
	for(i=0;i<num_monstros;i++){
		char query[6];
		sprintf(query,"%d",10000+(monstros[i].x*100)+monstros[i].y);
		ABRIR_LINK(name,query);
		IMAGEM_FORMATED(monstros[i].x,monstros[i].y,TAM,TAM,"button_help_ask.png");
		FECHAR_LINK;
	}
}
void imprime_helpInvetory(char *name, INVT bag){
	int i;
	for(i=0;i<INVT_SIZE;i++){
		if(bag.inv[i]!=0){
		int X = 620+(TAM*(i/2));
		int Y = 110+(50*!(i%2));
		char query[3];
		sprintf(query,"4%d",i);
		ABRIR_LINK(name,query);
		IMAGEM(X,Y,TAM,TAM,"button_help_ask.png");
		FECHAR_LINK;
		}
	}
	if(bag.weapon){
		ABRIR_LINK(name,"46");
		IMAGEM(690,260,TAM,TAM,"button_help_ask.png");
		FECHAR_LINK;
	}
	if(bag.armour){
		ABRIR_LINK(name,"47");
		IMAGEM(690,310,TAM,TAM,"button_help_ask.png");
		FECHAR_LINK;
	}
}
void imprime_itemDescription(int item){
	char *ITEM_DESC[] = ITEM_DESCRIPTIONS;
	char *ITEM_TP[] = ITEM_TYPES;
	int itemType = 0;
	itemType = item > 2  ? 1 : 0;
	itemType = item > 9  ? 2 : itemType;
	itemType = item > 13 ? 3 : itemType;
	int itemTier=0;
	if(item>13){
		itemTier=item-13;
	}else if(item>9){
		itemTier=item-9;
	}
	printf("<text x=600 y=410 style=\"stroke:#FFFFFF;fill:#FFFFFF;\">TYPE: %s</text>\n",ITEM_TP[itemType]);
	if(item>9){
		printf("<text x=600 y=430 style=\"stroke:#FFFFFF;fill:#FFFFFF;\">TIER: %d</text>\n",itemTier);
	}
	imprime_texto(600, 450, ITEM_DESC[item],17);
}
void imprime_monsterDescription(int isInBossBattle, MSTR monstro){
	int select = isInBossBattle ? 4 : monstro.monType;

	int BASE_HPS[]    = {MON_HP_WOLF,MON_HP_BAT,MON_HP_OGRE,MON_HP_ARCHER,MON_HP_DRAGON};
	char *MSTR_DESC[] = {WOLF_DESC  ,BAT_DESC  ,OGRE_DESC  ,ARCHER_DESC  ,DRAGON_DESC  };
	
	printf("<text x=600 y=410 style=\"stroke:#FFFFFF;fill:#FFFFFF;\">MAX HP: %d</text>\n",BASE_HPS[select]);
	imprime_texto(600, 430, MSTR_DESC[select],15);
}
void imprime_monsterAttackArea(ESTADO e, MSTR monstro){
	int select = e.isInBossBattle ? 4 : monstro.monType;
	switch(select){
		case 0: i_wolfAttackArea(e,monstro);
				break;
		case 1: i_batAttackArea(e,monstro);
				break;
		case 2: i_ogreAttackArea(e,monstro);
				break;
		case 3: i_archerAttackArea(e,monstro);
				break;
		case 4: i_dragonAttackArea(e);
				break;
	}
}
void i_wolfAttackArea(ESTADO e,MSTR monstro){
	int i,j;
	for (i = -1; i < 2; ++i){
		for (j = -1; j < 2; ++j){
			POSICAO mon = {monstro.x+i,monstro.y+j};
			if(!outOfBounds(mon) && !com_monstros(e,mon) && !com_pedras(e,mon)){
				SQUARE_FORMATED(mon.x,mon.y,"red",0.3,"black",0.0,TAM);
			}
		}
	}
}
void i_batAttackArea(ESTADO e, MSTR monstro){
	int i,j;
	for (i = -1; i < 2; ++i){
		for (j = -1; j < 2; ++j){
			POSICAO mon = {monstro.x+i,monstro.y+j};
			if(!outOfBounds(mon) && !com_monstros(e,mon) && !com_pedras(e,mon)){
				SQUARE_FORMATED(mon.x,mon.y,"red",0.3,"black",0.0,TAM);
			}
		}
	}
}
void i_ogreAttackArea(ESTADO e, MSTR monstro){
	int i,j;
	for (i = -1; i < 2; ++i){
		for (j = -1; j < 2; ++j){
			POSICAO mon = {monstro.x+i,monstro.y+j};
			if((i==0 || j==0) && !outOfBounds(mon) && !com_monstros(e,mon) && !com_pedras(e,mon)){
				SQUARE_FORMATED(mon.x,mon.y,"red",0.3,"black",0.0,TAM);
			}
		}
	}
}
void i_archerAttackArea(ESTADO e, MSTR monstro){
	int x,y;
	for (x = -3; x < 4; ++x){
		for (y = -3; y < 4; ++y){
			POSICAO mon = {monstro.x+x,monstro.y+y};
			if(y<((-1*abs(x))+4) && y>(abs(x)-4)  && !outOfBounds(mon) && !com_monstros(e,mon) && !com_pedras(e,mon)){
				SQUARE_FORMATED(mon.x,mon.y,"red",0.3,"black",0.0,TAM);	
			}
		}
	}
}
void i_dragonAttackArea(ESTADO e){
	int i,j;
	for (i = 0; i < SIZE; ++i){
		for (j = 0; j < SIZE; ++j){
			POSICAO p = {i,j};
			if(!com_monstros(e,p)){
				SQUARE_FORMATED(i,j,"red",0.3,"black",0.0,TAM);
			}
		}
	}
}
void imprime_feedback(int feedback){
	IMAGEM(595,390,200,150,"ScreenFeedback.png");
	char *feedbackMessages[] = FEEDBACK_MSGS;
	imprime_texto(600, 470, feedbackMessages[feedback],17);
}