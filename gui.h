/*
 * gui.h
 *
 *  Created on: Nov 22, 2019
 *      Author: atom
 */
#include <stdio.h>
#include "altera_up_avalon_video_pixel_buffer_dma.h"
#include "altera_up_avalon_video_character_buffer_with_dma.h"

#include <io.h>

#ifndef GUI_H_
#define GUI_H_

#define X_MASK 0x1ff
#define Y_MASK 0xff
#define Y_OFFSET 9
#define X_OFFSET 0
#define BASE_ADDRESS 0
#define ROWNUM 240
#define COLNUM 320
#define MROWNUM 480
#define MCOLNUM 640

#define GREEN_MASK 0x3f
#define BLUE_MASK 0x1f
#define RED_MASK 0x1f
#define GREEN_OFFSET 5
#define BLUE_OFFSET 0
#define RED_OFFSET 11

alt_up_char_buffer_dev* char_buf;
alt_up_pixel_buffer_dma_dev* pixel ;
short * back;
// ,lh hts] hg]dk hgh lg,; , hpfhv s,x ,vifhkih
// clown damage random ensure main keen accuse inform wife sand convince stuff
void set_box(unsigned short y1 ,unsigned short y2 ,unsigned short x1 ,unsigned short x2 , short value , char backBuffer){
	for(int i = y1 ; i < y2 ; i++)
		for(int k = x1 ; k < x2 ; k++){
			if(x2 > COLNUM &&  y2 < ROWNUM){
				x2 = COLNUM;
				continue;
			}
			if(y2 > ROWNUM){
					y2 = ROWNUM;
					continue;
				}
			set_pixel( i , k , value ,backBuffer);

		}

		//alt_up_pixel_buffer_dma_draw_box(pixel, x1, y1, x2, y2, value, 0);
}

void set_circle_center (unsigned short y, unsigned short x, unsigned short radius, short color , char backBuffer)
{
	int x_position, y_position, d, delta_e, delta_se;
	x_position = -1;
	y_position = radius;
	d = 1 - radius;
	delta_e = -1;
	delta_se = (-radius << 1) + 3;
	while (y_position > x_position) {
		delta_e += 2;
		x_position++;
		if (d < 0) {
			d += delta_e;
			delta_se += 2;
	     }
		else {
			d += delta_se;
			delta_se += 4;
	        y_position--;
	     }

	        set_pixel ( y + y_position, x + x_position, color , 1);
	        set_pixel (y + x_position,x + y_position,  color,1);
	        set_pixel ( y - x_position,x + y_position, color,1);
	        set_pixel ( y - y_position,x + x_position, color,1);

	        set_pixel ( y - y_position, x - x_position, color,1);
	        set_pixel ( y - x_position,x - y_position, color,1);
	        set_pixel ( y + x_position,x - y_position, color,1);
	        set_pixel ( y + y_position, x - x_position,color,1);
	    }

}
void init(){
	char_buf = alt_up_char_buffer_open_dev("/dev/video_character_buffer_with_dma_0_avalon_char_buffer_slave");
	alt_up_char_buffer_clear(char_buf);
	pixel =  alt_up_pixel_buffer_dma_open_dev("/dev/video_pixel_buffer_dma_0") ;

	alt_up_pixel_buffer_dma_change_back_buffer_address(pixel ,0x080a0000 );
	set_box(0 , ROWNUM , 0 , COLNUM , 0,1 );
	alt_up_pixel_buffer_dma_swap_buffers(pixel);
	while(alt_up_pixel_buffer_dma_check_swap_buffers_status(pixel));

	alt_up_pixel_buffer_dma_change_back_buffer_address(pixel,0x08050000);
	set_box(0 , ROWNUM , 0 , COLNUM , 0,1);
	while(alt_up_pixel_buffer_dma_check_swap_buffers_status(pixel));

}
void my_memcpy(short * base ,short * buffer , unsigned int len ){
	int  i ;
	for(i = 0 ; i < len ; i++){
		base[i]= buffer[i];
	}
}
void set_row(unsigned short y , short value , char backBuffer){
	for(int l = 0 ; l < 10000 ; l++);

	for(int i = 0 ; i < COLNUM ; i++ )
		set_pixel(y ,i , value , backBuffer);

}

void set_box_center(unsigned short y ,unsigned short x ,unsigned short h ,unsigned short w , short value , char backBuffer){
	short y_start , y_end , x_start , x_end , y_dist , x_dist;
	y_dist = h/2;
	x_dist = w/2;
	y_start = y - y_dist;
	y_end = y + y_dist;

	x_start = x - x_dist;
	x_end = x + x_dist;
	if(y_start  < 0)
			y_start = 0;
	if(x_start < 0)
		x_start = 0;
	//printf("Box : y = (%d , %d) x = (%d , %d )\n" , y_start , y_end , x_start , x_end);
	set_box(y_start , y_end ,x_start , x_end , value , backBuffer);


}
void set_fill(unsigned short y1 ,unsigned short y2 ,unsigned short x1 ,unsigned short x2 , short value){
	set_rows(y1 , y2 , value);
	set_cols( x1 , x2 , value);
}
void set_col(unsigned short x , short value){
	for(int l = 0 ; l < 10000 ; l++);
	for(int i = 0 ; i < ROWNUM ; i++ )
		set_pixel(i ,x , value , 1);

}

void set_rows(unsigned short start ,unsigned short end , short value){
	for(int k = start ; k < end ; k++)
		for(int i = 0 ; i < COLNUM ; i++ )
			set_pixel(k ,i , value,1);

}

void set_cols(unsigned short start ,unsigned short end , short value){
	for(int k = start ; k < end ; k++)
		for(int i = 0 ; i < ROWNUM ; i++ )
			set_pixel(i ,k , value,1);

}
void   set_pixel(unsigned short y , unsigned short x , short value , char backBuffer){
	if(y > ROWNUM){
		//printf("Out of range row %d  with color %d\n" , y , value);
		return ;
	}
	if(x > COLNUM){
			//printf("Out of range column %d  with color %d\n" , x , value);
			return;
	}
	volatile short * address ;
	if(backBuffer)
		address = (short*)pixel->back_buffer_start_address;

	else
		address = (short*)pixel->buffer_start_address;

	address += COLNUM * y ;
	IOWR_16DIRECT(address, x << 1, value);


	//address[COLNUM * y + x] = value;
	//printf("color pixel %d , %d with %d\n" , y , x ,value);
}

void display(){
	alt_up_pixel_buffer_dma_swap_buffers(pixel);

	while(alt_up_pixel_buffer_dma_check_swap_buffers_status(pixel));
	//while(alt_up_pixel_buffer_dma_check_swap_buffers_status(pixel));

	//set_box(0 , ROWNUM , 0 , COLNUM , 0);

}


short color(char red , char green , char blue){

	return (0x0000 | (red & RED_MASK) << RED_OFFSET | (green & GREEN_MASK) << GREEN_OFFSET | (blue & BLUE_MASK) << BLUE_OFFSET);
}
#endif /* GUI_H_ */
