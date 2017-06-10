#include "IAdragon.h"

ESTADO fireCone(ESTADO e){
	if(e.jog.x>=((e.dragon.pos.x+1)-(e.jog.y-e.dragon.pos.y-1)) && e.jog.x<=((e.dragon.pos.x+2)+(e.jog.y-e.dragon.pos.y-1))){
		e.hp-=5;
	}
	return e;
}
ESTADO Wing_attack (ESTADO e){
	if(e.jog.x>=1 && e.jog.x<=8 && e.jog.y>=e.dragon.pos.y+4 && e.jog.y<=e.dragon.pos.y+7){
		e.hp-=10;
	}
	if(e.jog.y+3<10){
		e.jog.y+=3;
	}
	if(e.jog.y+3>9){
		e.jog.y=9;
	}
	return e;
}
ESTADO fireball_damage (ESTADO e){
 int i;
	for(i=0;i<e.dragon.num_fireballs;i++){
		if(e.dragon.fireballCenters[i].x==e.jog.x && e.dragon.fireballCenters[i].y==e.jog.y){
			e.hp-=15;
		}
	}
	return e;
}
ESTADO fireball_attack (ESTADO e){
   int i,x,y,b1,b2;
   y=e.dragon.pos.y+3;// minimo
   i=(rand() % 7)+1;
   e.dragon.num_fireballs=i;
   for(x=0;x<i;x++){
     do{
     	 b1=(rand() % 10);
     	 b2=(rand() % 10);
     }while(b1<y||b1>y+3||(b2<e.jog.x-2)|| b2>(e.jog.x+2)|| b2>9 || b2<0);
     e.dragon.fireballCenters[x].x=b2;
     e.dragon.fireballCenters[x].y=b1;
   }
   e=fireball_damage(e);
   return e;
}
ESTADO dragon_movement (ESTADO e){
	int flag=1;
	POSICAO p;
	p.x=e.dragon.pos.x;
	p.y=e.dragon.pos.y;
	if (e.jog.x<=p.x+1 && p.x>0){
		e.dragon.pos.x-=1;
		flag=0;
	}
	if(flag && e.jog.x>=p.x+2 && p.x+3<9){
		e.dragon.pos.x+=1;
	}
	if(e.dragon.attack==1 && abs(e.jog.y-p.y)>=1){
        e.dragon.pos.y+=1;
	}
	if(e.dragon.attack==2 && abs(e.jog.y-p.y)==1 && p.y>0){
		e.dragon.pos.y-=1;
	}
	if(e.dragon.attack==2 && abs(e.jog.y-p.y)>3){
		e.dragon.pos.y+=1;
	}
	if(p.y>3){
		e.dragon.pos.y-=1;
	}
	return e;
}
// 1-fireball;2-wingAttack;3-fireCone
ESTADO estrat_dragon (ESTADO e){
	e=dragon_movement(e);
	if(e.turn%2!=0){
   	srand(time(NULL));
   	e.dragon.attack=(rand() % 3)+1;
 }
 if(e.turn !=0 && e.turn%2==0){
 	if(e.dragon.attack==1){
      	e=fireball_attack(e);
    }
    if(e.dragon.attack==2){
      	e=Wing_attack(e);
    }
    if(e.dragon.attack==3){
       e=fireCone(e);
    }
 }
 return e;
}