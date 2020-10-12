/*
 * ping_pong.h
 *
 *  Created on: Dec 9, 2019
 *      Author: atom
 */

#ifndef PING_PONG_H_
#define PING_PONG_H_
#include <stdio.h>
#include <stdlib.h>
#include "system.h"
#include "alt_types.h"
#include "gui.h"
#include "game_play.h"
#include <stdlib.h>
#include "altera_avalon_pio_regs.h"
#include "altera_up_avalon_video_character_buffer_with_dma.h"



void setup_ping_pong(){

		struct  GameObject * player_1;
		struct  GameObject * player_2;
		struct  GameObject * ball;
		player_1 = (struct GameObject *)malloc(sizeof(struct GameObject));
		player_2 = (struct GameObject *)malloc(sizeof(struct GameObject));
		ball = (struct GameObject *)malloc(sizeof(struct GameObject));


		fill_game_object(ball , "main_ball" , SEQUARE, 2 , 2 , 0,0 ,-1 );
		ball->isDynamic = 1;
		ball->hit_response = DYNAMIC;
		ball->force.x = 1;
		ball->force.y = -1;
		fill_game_object(player_1 , "player_one" , SEQUARE,  21 , 6 , 0,0,-1);
		fill_game_object(player_2 , "player_2" , SEQUARE,  21 , 6 , 0,0,-1);

		map_add(World , player_2);
		map_add(World , player_1);
		map_add(World , ball);

		player_1->isDynamic = 1;
		player_1->hit_response = STATIC;
		player_1->force.x = 0;
		player_1->force.y = 0;

		player_2->isDynamic = 1;
		player_2->hit_response = STATIC;
		player_2->force.x = 0;
		player_2->force.y = 0;

		set_player_1(player_1);
		set_player_2(player_2);
		center(ball);
}


void update_ping_pong(struct Controller * input){


	struct GameObject * ball = map_find_by_name(World , "main_ball");
	struct GameObject * player_1 = map_find_by_name(World , "player_one");
	struct GameObject * player_2 = map_find_by_name(World , "player_2");


		if(input->up)
			player_1->force.y = 2;
		else if(input->down)
			player_1->force.y = -2;
		else
			player_1->force.y = 0;



		if(input->A)
			player_2->force.y = 2;
		else if(input->B)
			player_2->force.y = -2;
		else
			player_2->force.y = 0;

		if(ball->position.x < player_1->position.x - player_1->width_raduis){
					alt_up_char_buffer_string(char_buf, "Player_2 WON",35, 30);
					World->stop_time = 1;

					alt_up_char_buffer_string(char_buf, "Press any button to restart :)",30, 50);

					wait_for_input();
					alt_up_char_buffer_clear(char_buf);

					ball->force.x = 1;
					ball->force.y = -1;


					center(ball);
					set_player_1(player_1);
					set_player_2(player_2);
					World->stop_time = 0;

				}
		else if(ball->position.x > player_2->position.x + player_2->width_raduis){

				alt_up_char_buffer_string(char_buf, "Player_1 WON",35, 30);
				World->stop_time = 1;

				alt_up_char_buffer_string(char_buf, "Press any button to restart :)",30, 50);
				for(int i = 0 ; i < 100000 ; i++);
				wait_for_input();
				alt_up_char_buffer_clear(char_buf);

				ball->force.x = 1;
				ball->force.y = -1;


				center(ball);
				set_player_1(player_1);
				set_player_2(player_2);
				World->stop_time = 0;
		}




}

set_player_1(struct GameObject * x){
	x->position.x = 10;
	x->position.y = ROWNUM/2;

};
set_player_2(struct GameObject * x){
	x->position.x = 300;
	x->position.y = ROWNUM/2;
};

#endif /* PING_PONG_H_ */
