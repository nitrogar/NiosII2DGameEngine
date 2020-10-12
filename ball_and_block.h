#include <stdio.h>
#include <stdlib.h>
#include "system.h"
#include "alt_types.h"
#include "gui.h"
#include "game_play.h"
#include <stdlib.h>
#include "altera_avalon_pio_regs.h"
#include "altera_up_avalon_video_character_buffer_with_dma.h"


int level = 0;
void level_builder(){
	if(level == 0)
		for(int k = 0 ; k < 2 ; k++)
			for(int  i = 0 ; i <10 ; i++)
				add_block(k ,i);

	else if (level == 1){
		printf("NEXT LEVEL");
		for(int k = 0 ; k < 4 ; k++)
			for( int i = 10 ; i > 0 ; i-=2)
				add_block(k ,i);

	}

}

void HitHandler_block(struct MapObject * map ,struct  GameObject * block , struct  GameObject * projectil ){
	hide_game_object(block);
	map_remove(map , block);
	delete_game_object(block);

	if(!map_find_by_name(World,"block")){
		alt_up_char_buffer_string(char_buf, "YOU WON ",35, 30);
		World->stop_time = 1;


		alt_up_char_buffer_string(char_buf, "Press any button to go to next level :)",30, 50);

		wait_for_input();
		alt_up_char_buffer_clear(char_buf);


		level = 1;
		level_builder();
		World->stop_time = 0;

	}
}
void setup_block_and_ball(){
	struct  GameObject * ball;
	struct  GameObject * player;
	player = (struct GameObject *)malloc(sizeof(struct GameObject));
	ball = (struct GameObject *)malloc(sizeof(struct GameObject));


	fill_game_object(ball , "main_ball" , SEQUARE, 2 , 2 , 0,0 ,-1 );
	ball->isDynamic = 1;
	ball->hit_response = DYNAMIC;
	ball->force.x = 1;
	ball->force.y = -1;
	fill_game_object(player , "player" , SEQUARE,  6 , 21 , 0,0,-1);
	map_add(World , player);
	map_add(World , ball);

	player->isDynamic = 1;
	player->hit_response = STATIC;
	player->force.x = 0;
	player->force.y = 0;
	player->color = color(23,46,184);


	level_builder();

	center(ball);
	set_player(player);
}

void update_block_and_ball(struct Controller * input){
	struct GameObject * ball = map_find_by_name(World , "main_ball");
	struct GameObject * player = map_find_by_name(World , "player");


	if(input->right)
		player->force.x = 2;
	else if(input->left)
		player->force.x = -2;
	else
		player->force.x = 0;

	if(ball->position.y > player->position.y + player->hight){
				alt_up_char_buffer_string(char_buf, "GAME OVER ",35, 30);
				World->stop_time = 1;

				alt_up_char_buffer_string(char_buf, "Press any button to restart :)",30, 50);

				wait_for_input();
				alt_up_char_buffer_clear(char_buf);
				map_empty(World);
				setup_block_and_ball();
				World->stop_time = 0;


			}
}

void add_block(short x , short y){
	struct  GameObject * block;
	short block_h = 8;
	short block_w = 19;
	block = (struct GameObject *)malloc(sizeof(struct GameObject));
	fill_game_object(block , "block" , SEQUARE, block_h , block_w , 50 + (block_w +1)* y ,40 +(block_h+1)* x ,-1);
	block->isDynamic = 0;
	block->hit_response = STATIC;
	block->isHitHandler = 1;
	block->HitHandler = HitHandler_block;
	map_add(World , block);



};

