/*
 * "Hello World" example.
 *
 * This example prints 'Hello from Nios II' to the STDOUT stream. It runs on
 * the Nios II 'standard', 'full_featured', 'fast', and 'low_cost' example
 * designs. It runs with or without the MicroC/OS-II RTOS and requires a STDOUT
 * device in your system's hardware.
 * The memory footprint of this hosted application is ~69 kbytes by default
 * using the standard reference design.
 *
 * For a reduced footprint version of this template, and an explanation of how
 * to reduce the memory footprint for a given application, see the
 * "small_hello_world" template.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "system.h"
#include "alt_types.h"
#include "gui.h"
#include "game_play.h"
#include <stdlib.h>
#include "altera_up_avalon_video_character_buffer_with_dma.h"
#include "ball_and_block.h"
#include "ping_pong.h"
#include "pixel_art.h"

int main()
{
	volatile int  frame_id = 0;
	init();

	game_play_init();


	alt_up_char_buffer_string(char_buf, "Press A to play ping pong",28, 30);
	alt_up_char_buffer_string(char_buf, "Press B to play ball and blocks",28, 32);
	alt_up_char_buffer_string(char_buf, "Press UP for pixel art",28, 34);

	char game_type = 0 ;
	while(1){
		get_player_input(player1);
		if(player1->A){
			setup_ping_pong();
			game_type = 1;
			break;
		}
		else if(player1->B){
			setup_block_and_ball();
			game_type = 2;
			break;
		}
		else if(player1->up){
					setup_pixel_art();
					game_type = 3;
					break;
				}
	}
	alt_up_char_buffer_clear(char_buf);

	while(1){
		if(	game_type == 1 )
			update_ping_pong(player1);
		else if(game_type == 2)
			update_block_and_ball(player1);
		else if(game_type == 3)
			update_pixel_art(player1);
		game_engin();


		frame_id++;
	};
    return 0;
}

