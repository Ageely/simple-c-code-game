#include <stdio.h>
#include <termios.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#define SIZE 25
#define COUNT 3
#define FRAME_TIME 10
#define HERO 'I'
#define BULLET '-'
#define BULLET_SPEED 0.03
struct bullet {
	double x;
	double s;
};
char map[] = {' ', '*', '$', '|', '#', '>', '<'};
//                     1,2,3,4,5,6,7,8, 9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24
int rm[COUNT][SIZE]={
                    {0,4,2,2,1,0,2,0,0, 2, 2, 0, 2, 0, 0, 0, 0, 2, 0, 2, 2, 3, 2, 0, 5},
                    {6,0,0,0,0,0,2,0,0, 0, 2, 0, 0, 1, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 5},
		    {6,0,0,0,0,0,0,0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};
int *r;
int hx;
int gold;
int weap;
int damage;
int health;
int loc;
int ammo;
struct bullet b;

void msleep(int milisec){
	struct timespec ts = { 0 , milisec * 1000000};
	struct  timespec tr;
	nanosleep( &ts, &tr);
};
void stop_bullet(void){
	b.s = 0;
	b.x = -1;
};
void starter_stats(void){
        loc = 0;
        r = rm[loc];
        hx = 13;
        gold = 0;
        weap = 0;
        damage = 1;
        health = 3;
        ammo = 1000;
        b.x = -1;
        b.s = 0;
};
int fire_ready(void){
        return ammo > 0 && (b.x < 0 || b.x > 26) && weap > 0;
};
int main(int argc, char **argv){
	int i;
	char op;
	struct termios orig_attr;
	struct termios new_attr;
	tcgetattr(fileno(stdin), &orig_attr);
	memcpy(&new_attr, &orig_attr, sizeof(struct termios));
	new_attr.c_lflag &= ~(ECHO|ICANON);
	new_attr.c_cc[VTIME] = 0;
	new_attr.c_cc[VMIN] = 0;
	tcsetattr(fileno(stdin), TCSANOW, &new_attr);
        starter_stats();
	while(1){
	if(r[hx] == 2){
		gold++;
		r[hx] = 0;
	};
        

	//|||||||
	printf("\033[2J");
	printf("room %d $ %d damage %d health %d ammo %d \n",loc+1 ,gold, damage, health, ammo);
	if(weap == 1){
                printf("weapone: *\n");
	} else if(weap == 0){
	        printf("weapone: none\n");
	} else if(weap == 3){
                printf("weapone: |\n");
        };
        for(i=0; i<SIZE+2; i++)
                printf("%c",'-');
        printf("\n|");

        for(i=0; i<SIZE; i++){
		if(i != hx && i != (int) b.x){
                        printf("%c",map[r[i]]);
		}else if(i == hx){
		        printf("%c",HERO);
		}else{ 
		       printf("%c",BULLET);
		};	
	};	
	printf("|\n");
        for(i=0; i<SIZE+2; i++)
		printf("%c",'-');
	printf("\n");
	//||||||||
	i = read(0,&op,1);
	if(i <= 0){
	    msleep(FRAME_TIME);
	    if(b.x >= 0 && r[(int) b.x] > 0 && r[(int) b.x] != 5 && r[(int) b.x] != 6){
		    r[(int) b.x] = 0;
		    stop_bullet();
            }else if((r[(int) b.x] == 5 || r[(int) b.x] == 6)){
                    b.s = -b.s;
	    };
	    if(b.x > 0 && b.x < 26){
	    b.x = b.x + b.s * FRAME_TIME;
	    }else{
		    stop_bullet();
            };
	    continue;
	};
	switch(op){
		case 'a':
			if(hx > 0){
		        hx--;
			};
                        break;
		case 'd':
			if(hx < SIZE-1){
			hx++;
			};
			break;
		case 'x':
	                tcsetattr(fileno(stdin), TCSANOW, &orig_attr);
			return 0;
			break;
		case 'r':
                        if(r[hx] == 0){
			r[hx] = weap;
		        weap = 0;
		        damage = 1;
		        } else if(r[hx] == 1){                        
                        r[hx] = weap;
                        weap = 1;
                        damage = 2;
                        } else if(r[hx] == 3){                        
                        r[hx] = weap;
                        weap = 3;
                        damage = 3;
	                } else
			if(r[hx] == 5){
                        loc++;
			r = rm[loc];
			hx = 2;
			} else

			if(r[hx] == 6){
			loc--;
			r = rm[loc];
			hx = 24;
			};

		case 'm':
			if(r[hx] == 4){
			gold = gold + damage;
			weap = 0;
			damage = 1;
			};
			break;
		case 'q':
			if(hx > 0 && fire_ready()){
			b.x = hx - 1;
                        b.s = -BULLET_SPEED; 
                        ammo--;
			};
			break;
                case 'e':
                        if(hx < SIZE-1 && fire_ready()){ 
			b.x = hx + 1;
                        b.s = BULLET_SPEED;
                        ammo--;
                        };
                        break;
		default:
			break;
	};
        };
};                      
