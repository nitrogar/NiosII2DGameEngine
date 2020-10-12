/*
 * math.h
 *
 *  Created on: Nov 23, 2019
 *      Author: atom
 */

#ifndef MATH_H_
#define MATH_H_
#include <stdio.h>
struct Vector{
	short x ;
	short y;
};

struct Sequare {
	struct Vector p1;
	struct Vector p2;

};


struct Vector *  vector_copy(struct Vector * v1 , struct Vector * v2){
	v1->x = v2->x;
	v1->y = v2->y;
	return v1;
}
struct Vector * vector_add(struct Vector *v1 , struct Vector *v2){
	v1->x += v2->x;
	v1->y += v2->y;
	return v1;
}

struct Vector * vector_mul(struct Vector* v1 , short x_scale ){
	v1->x *= x_scale;
	v1->y *= x_scale;
	return v1;

}
struct Vector * vector_sub(struct Vector * v1 , struct Vector * v2 , struct Vector * res ){
	res->x = v1->x - v2->x;
	res->y = v1->y - v2->y;
	return res;
}
struct Vector * vector_projection(struct Vector * v1 , struct Vector * v2 , struct Vector * res){
	vector_copy(res , v2);
	int nem = vector_dot(v1 , v2);
	int dem = vector_dot(v2 , v2);
	vector_mul(res , nem/dem);
	return res;
}

struct Vector * vector_reflect(struct Vector * v1 , struct Vector * normal , struct Vector * res){
	struct Vector tmp;
	vector_projection(v1 , normal , &tmp);
	vector_mul(&tmp , 2);
	vector_sub(v1 , &tmp , res);
	//vector_mul(res , -1);
	return res;

};
int vector_dot(struct Vector * v1 , struct Vector * v2){
	return v1->x * v2->x + v1->y * v2->y;
}
struct Vector * vector_rejection(struct Vector * v1 , struct Vector * v2 , struct Vector * res){
	struct Vector tmp;
	return vector_sub( v1,vector_projection(v1 , v2 ,&tmp)  , res);
}





void print_vector_info(struct Vector * v){
	printf("x = %d , y = %d" , v->x , v->y);

}

#endif /* MATH_H_ */
