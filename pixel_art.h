/*
 * pixel_art.h
 *
 *  Created on: Dec 9, 2019
 *      Author: atom
 */

#ifndef PIXEL_ART_H_
#define PIXEL_ART_H_
#include <stdio.h>
#include <stdlib.h>
#include "system.h"
#include "alt_types.h"
#include "gui.h"
#include "game_play.h"
#include <stdlib.h>
#include "altera_avalon_pio_regs.h"
#include "altera_up_avalon_video_character_buffer_with_dma.h"
short ink[3] ;
char ink_sq = 0;
void add_ink(short x , short y){
	struct  GameObject * block;
	short block_h = 2;
	short block_w = 2;
	short c = ink[ink_sq % 3] ;
	block = (struct GameObject *)malloc(sizeof(struct GameObject));

	fill_game_object(block , "ink" , SEQUARE, block_h , block_w ,y ,x ,c);
	block->isDynamic = 0;
	block->hit_response = GHOST;
	block->isHitHandler = 0;
	//block->HitHandler = HitHandler_block;
	map_add(World , block);

};
void update_pixel_art(struct Controller * input){
	struct GameObject * pen = map_find_by_name(World , "pen");

		if(input->right)
			pen->force.x = 1;
		else if(input->left)
			pen->force.x = -1;

		else if(input->up)
			pen->force.y = -1;
		else if(input->down)
			pen->force.y = 1;
		else{
			pen->force.x = 0;
			pen->force.y = 0;
		}
		if(input->A)
					add_ink(pen->position.y ,pen->position.x );
		if(input->B)
			ink_sq++;




}


void setup_pixel_art(){

		struct  GameObject * pen;

		pen = (struct GameObject *)malloc(sizeof(struct GameObject));

		fill_game_object(pen , "pen" , SEQUARE, 2 ,2 , 0,0 ,-1 );
		pen->isDynamic = 1;
		pen->hit_response = DYNAMIC;

		map_add(World , pen);
		center(pen);

		 ink[0] = color(-1,0,0) ;
		 ink[1] = color(0,0,-1);
		 ink[2] = color(0,-1,0);
}


#endif /* PIXEL_ART_H_ */
