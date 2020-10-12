/*
 * game_play.h
 *
 *  Created on: Nov 23, 2019
 *      Author: atom
 */

#ifndef GAME_PLAY_H_
#define GAME_PLAY_H_
#include "math.h"
#include "gui.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "altera_avalon_pio_regs.h"

#include <string.h>
#define PLAYER_ROW 200
#define BACKGROUND 0
short world_counter = 0;
enum SHAPE {NONE , CIRCLE , SEQUARE , LINE , PIONT};
enum HitResponse{STATIC , DYNAMIC  ,GHOST , IGNORE};
enum MASK_CHAR {b_0  =0b00000001 ,
				b_1  =0b00000010,
				b_2  =0b00000100,
				b_3  =0b00001000,
				b_4  =0b00010000,
				b_5  =0b00100000,
				b_6  =0b01000000,
				b_7  =0b10000000,
				b_0n =0b11111110,
				b_1n=0b11111101,
				b_2n=0b11111011,
				b_3n=0b11110111,
				b_4n=0b11101111,
				b_5n=0b11011111,
				b_6n=0b10111111,
				b_7n=0b01111111};

struct GameObject * current ;
struct MapObject * World ;

struct Controller * player1;
struct MapObject {
	short id;
	char * name;
	short o_counter;
	short color;
	struct Sequare dim;
	char stop_time ;
	struct GameObject * first;
	struct GameObject * current;
};
struct GameObject{
	char * name;
	struct GameObject * next;
	void (*HitHandler)( struct MapObject * ,struct GameObject * , struct GameObject *);
	enum SHAPE shape;
	enum HitResponse hit_response;
	struct Vector position;
	struct Vector position_buffer0;
	struct Vector position_buffer1;
	struct Vector force;
	struct Vector force_next;
	short width_raduis;
	short hight;
	short color;
	short id ;

	char buffer_id ;
	char isDynamic;
	char isHitHandler;
};

struct Controller{
	char isActive;
	char up;
	char down;
	char left;
	char right;
	char A;
	char B;

};
struct Sequare screen ;

void copy_game_object(struct GameObject * dist , struct GameObject * src ){
	dist->name = src->name;
	dist->hight = src->hight;
	dist->width_raduis = src->width_raduis;
	dist->color = src->color;

	dist->position.x = src->position.x;
	dist->position.y = src->position.y;


}
void HitHandler_empty(struct MapObject * map ,struct  GameObject * block , struct  GameObject * projectil ){
	return;
}
void fill_game_object(struct GameObject * obj , char * name ,enum SHAPE shape, short hight , short width_raduis , short x , short y , short color){
	obj->id = 0;
	int len = strlen(name);
	char* heap_name = (char *)malloc(len);

	strcpy(heap_name , name);
	obj->name = heap_name;
	obj->hight = hight;
	obj->width_raduis = width_raduis;
	obj->color = color;
	obj->shape = shape;
	obj->position.x = x;
	obj->position.y = y;
	obj->buffer_id = 0;
	obj->isHitHandler = 0;
	obj->HitHandler = HitHandler_empty;
	obj->position_buffer0.x = obj->position.x;
	obj->position_buffer0.y = obj->position.y;

	obj->position_buffer1.x = obj->position.x;
	obj->position_buffer1.y = obj->position.y;
	obj->next = 0;
//	if(current != 0)
//		current->next = obj;
//	current = obj;
//	world_counter++;
}
void delete_game_object(struct GameObject * obj ){
	free(obj->name);
	free(obj);
}
void fill_map_object(struct MapObject * obj , char * name ,short id ,short y1 , short x1 ,short y2 , short x2 ,short color  ){

		obj->id =id;
		int len = strlen(name);
		char* heap_name = (char *)malloc(len);

		strcpy(heap_name , name);
		obj->name = heap_name;
		obj->dim.p1.y = y1;
		obj->dim.p1.x = x1;
		obj->dim.p2.y = y2;
		obj->dim.p2.x = x2;
		obj->color = color;
		obj->o_counter = 0;
		obj->first = 0;
		obj->current = 0;
		obj->stop_time = 0;
}

void map_add(struct MapObject * map , struct GameObject * obj){
	if(!map->first){
		map->first = obj;
		map->current = obj;
	}
	else {
		map->current->next = obj;
		map->current = obj;
	}
	obj->id = map->o_counter;
	map->o_counter++;
}


void map_empty(struct MapObject * map ){
	struct GameObject *  i= map->first ;
	struct GameObject *  next;
	while(i){
		next = i->next;
		hide_game_object(i);

		delete_game_object(i);
		map->o_counter--;
		i = next;
	}
	map->first = 0;
	map->current = 0;


}
void map_remove(struct MapObject * map , struct GameObject * obj){
	struct GameObject *  i ;
	for(i = map->first ; i != 0 && i->next->id != obj->id  ; i = i->next);

	i->next = i->next->next;

	map->o_counter--;

}

struct GameObject * map_find_by_name(struct MapObject * map ,  char * name){
	struct GameObject *  i ;
	for(i = map->first ; i != 0; i = i->next)
		if(!strcmp(name , i->name))
			return i;

	return 0;

}

void print_info(struct GameObject * obj){
	printf("\n\nObject name : %s\n\thight : %d\n\twidth_raduis : %d\n\tpostion :\n\t\tx = %d\n\t\ty = %d\n\nbuffer_id = %d\n\nbuffer_0\n\n\tx = %d\n\n\ty = %d\n\nbuffer_1\n\n\tx = %d\n\n\ty = %d" , obj->name , obj->hight , obj->width_raduis , obj->position.x , obj->position.y , obj->buffer_id , obj->position_buffer0.x , obj->position_buffer0.y, obj->position_buffer1.x , obj->position_buffer1.y );

}
void center(struct GameObject * v){
	v->position.x = COLNUM / 2;
	v->position.y = ROWNUM / 2;
}

void set_player(struct GameObject * v){
	v->position.x = COLNUM / 2;
	v->position.y = PLAYER_ROW;
}

void draw_sequare(struct GameObject * obj){
	if(obj->buffer_id == 0){
				//printf("\nObject %s \nERASING buffer_id = 0  , y =%d , x = %d, new y = %d , new x = %d" ,obj->name,obj->position_buffer0.y ,obj->position_buffer0.x ,obj->position.y ,obj->position.x  );
				set_box_center(obj->position_buffer0.y ,obj->position_buffer0.x , obj->hight , obj->width_raduis , color(0,0,0),1);

				obj->position_buffer0.x = obj->position.x ;
				obj->position_buffer0.y = obj->position.y ;

				obj->buffer_id = 1;


			}
			else if(obj->buffer_id == 1){
			//	printf("\nObject %s \nERASING buffer_id = 1  , y =%d , x = %d, new y = %d , new x = %d" ,obj->name ,obj->position_buffer1.y ,obj->position_buffer1.x ,obj->position.y ,obj->position.x  );
				set_box_center(obj->position_buffer1.y ,obj->position_buffer1.x , obj->hight , obj->width_raduis , color(0,0,0),1);

				obj->position_buffer1.x = obj->position.x ;
				obj->position_buffer1.y = obj->position.y ;

				obj->buffer_id = 0;

			}

			set_box_center(obj->position.y ,obj->position.x , obj->hight , obj->width_raduis , obj->color,1);

}

void physics_engine(){
	struct Vector tmp , old_position ;
	int state;
	for(struct GameObject * obj = World->first ; obj != 0 ; obj = obj->next){

		// *************** APPLAY FORCE ******************//
		// *************** CHECK & HANDLE COLLISION******************//
		if(obj->isDynamic){
			vector_copy(&old_position , &obj->position);
			move_by(obj , &obj->force);

			state = within_map(obj , World);
			if(state){
				vector_copy(&obj->position , &old_position);
				if(state == -1 || state == 1)
					obj->force.x *=-1;
				if(state == -2 || state == 2)
					obj->force.y *=-1;

				move_by(obj , &obj->force);
			}
			else if(obj->force.x != 0 || obj->force.y != 0){
				for(struct GameObject * obj2 = World->first ; obj2 != 0 ; obj2 = obj2->next){
					if(obj2->id == obj->id)
						continue;

					if(obj2->hit_response != IGNORE){
						state = check_collide_sequare(obj ,obj2);
						if(state == -1){
							if(obj2->hit_response == STATIC ||obj2->hit_response == DYNAMIC  ){
							vector_copy(&obj->position , &old_position);
							get_unit_noraml(obj , obj2 , &tmp);
							vector_reflect(&obj->force , &tmp , &obj->force);


							}

							if(obj2->isHitHandler)
								obj2->HitHandler(World , obj2 , obj);
						}
					}

				}
			}


		}
	}






}

void game_engin(){
	get_player_input(player1);
	if(!World->stop_time)
		physics_engine();

	render();
	display();

}
void hide_game_object(struct GameObject * obj){
	obj->color = BACKGROUND;
	if(obj->shape == SEQUARE){
		set_box_center(obj->position_buffer0.y ,obj->position_buffer0.x , obj->hight , obj->width_raduis , color(0,0,0),0);
		set_box_center(obj->position_buffer1.y ,obj->position_buffer1.x , obj->hight , obj->width_raduis , color(0,0,0),1);
	}
	else if(obj->shape == CIRCLE){
		set_circle_center(obj->position_buffer0.y ,obj->position_buffer0.x ,obj->width_raduis , color(0,0,0) , 0);
		set_circle_center(obj->position_buffer1.y ,obj->position_buffer1.x ,obj->width_raduis , color(0,0,0) , 1);
	}

}
void render(){
	for(struct GameObject * obj = World->first ; obj != 0 ; obj = obj->next){
		draw_game_object(obj);
	}

}
void get_player_input(struct Controller * cnt){
	int x = ~(IORD_ALTERA_AVALON_PIO_DATA(PIO_0_BASE));


	cnt->left  = (x&1);
	cnt->right = ((x>>1)&1);
	cnt->down  = ((x>>4)&1);
	cnt->up    = ((x>>2)&1);
	cnt->A = ((x>>3)&1);
	cnt->B = ((x>>5)&1);
}

void get_unit_noraml(struct GameObject * o1 , struct GameObject * o2 , struct Vector * n){
	short upper_x = (o2->position.x + o2->width_raduis/2);
	short lower_x = (o2->position.x - o2->width_raduis/2);


	if(o1->shape == SEQUARE)
		if(o1->position.x <upper_x  && o1->position.x > lower_x ){
			if(o1->force.y > 0){
				n->x = 0 ;
				n->y = 1;
				return;
			}
			else{
				n->x = 0 ;
				n->y = -1;
				return;
			}


		}

		else
			if(o1->force.x >= 0){
				n->x = -1 ;
				n->y = 0;
				return;
			}
			else if(o1->force.x <= 0){
				n->x = 1 ;
				n->y = 0;
				return;
			}







}


void draw_circle(struct GameObject * obj){

	if(obj->buffer_id == 0){
				//printf("\nObject %s \nERASING buffer_id = 0  , y =%d , x = %d, new y = %d , new x = %d" ,obj->name,obj->position_buffer0.y ,obj->position_buffer0.x ,obj->position.y ,obj->position.x  );
		set_circle_center(obj->position_buffer0.y ,obj->position_buffer0.x , obj->width_raduis , color(0,0,0),1);

				obj->position_buffer0.x = obj->position.x ;
				obj->position_buffer0.y = obj->position.y ;

				obj->buffer_id = 1;


			}
			else if(obj->buffer_id == 1){
			//	printf("\nObject %s \nERASING buffer_id = 1  , y =%d , x = %d, new y = %d , new x = %d" ,obj->name ,obj->position_buffer1.y ,obj->position_buffer1.x ,obj->position.y ,obj->position.x  );
				set_circle_center(obj->position_buffer1.y ,obj->position_buffer1.x ,obj->width_raduis , color(0,0,0),1);

				obj->position_buffer1.x = obj->position.x ;
				obj->position_buffer1.y = obj->position.y ;

				obj->buffer_id = 0;

			}

	set_circle_center(obj->position.y ,obj->position.x , obj->width_raduis , obj->color , 1);

}


void draw_game_object(struct GameObject * obj){
	if(obj->shape == SEQUARE)
		draw_sequare(obj);

	else if(obj->shape == CIRCLE)
		draw_circle(obj);
}

void move_by(struct GameObject * obj , struct Vector * force){
/*	if(obj->buffer_id == 1){
		obj->position_buffer1.x = obj->position.x ;
		obj->position_buffer1.y = obj->position.y ;
	}
	else if(obj->buffer_id == 0){
			obj->position_buffer0.x = obj->position.x ;
			obj->position_buffer0.y = obj->position.y ;
		}
	*/
	obj->position.x += force->x;
	obj->position.y += force->y;
}


/*
 * return values :
 * 	0 ===> OK
 * 	-1 ===> small x
 * 	1 ===> big x
 * 	-2  ===> small y
 * 	2 ==> big y
 * */

int move_by_inside(struct GameObject * obj , struct Vector * force , struct Sequare * limit){
	short x_new = obj->position.x + force->x;
	short y_new = obj->position.y + force->y;
	if(x_new < limit->p1.x)
		return -1;
	if( x_new > limit->p2.x)
		return 1;

	if(y_new < limit->p1.y)
			return -2;
	if( y_new > limit->p2.y)
			return 2;

	move_by(obj , force);
	return 0;

}

int move_by_inside_collistion(struct GameObject * obj , struct Vector * force , struct Sequare * limit){

return 0;
}

int within_map(struct  GameObject * obj , struct MapObject * m){
		if(obj->position.x < m->dim.p1.x)
			return -1;
		if( obj->position.x > m->dim.p2.x)
			return 1;

		if(obj->position.y < m->dim.p1.y)
				return -2;
		if( obj->position.y > m->dim.p2.y)
				return 2;
		return 0;

}
int check_collide_sequare(struct GameObject * o1 ,struct GameObject * o2 ){
	if(o1->position.x < (o2->position.x + o2->width_raduis/2) && o1->position.x > (o2->position.x - o2->width_raduis/2)){
		//printf("\nObject %s share x with %s " , o1->name , o2->name);
		if(o1->position.y < (o2->position.y + o2->hight/2) && o1->position.y > (o2->position.y - o2->hight/2))
			return -1;
	}
	return 0;
}
int move_by_inside_screen(struct GameObject * obj , struct Vector * force){
	return move_by_inside(obj , force , &screen);
}

void game_play_init(){
	player1 = (struct Controller *) malloc(sizeof(struct Controller));
	World = (struct MapObject *) malloc(sizeof(struct MapObject));
	fill_map_object(World , "World" , 0,0 , 0 ,ROWNUM , COLNUM , 0 );
}
void wait_for_input(){
	while(~(IORD_ALTERA_AVALON_PIO_DATA(PIO_0_BASE)) == 0);
}
#endif /* GAME_PLAY_H_ */
