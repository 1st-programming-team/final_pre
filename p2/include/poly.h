#ifndef POLY__HEADER_FILE
#define POLY__HEADER_FILE
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct poly_struct;
typedef struct poly_struct
{
	int a, p;
	struct poly_struct *next;
} *poly, poly_node;

poly make_poly(int a, int p);
void add_poly(poly lhs, poly rhs);
void sub_poly(poly lhs, poly rhs);
void mul_poly(poly lhs, poly rhs);
int div_poly(poly lhs, poly rhs);
poly copy_poly(poly p);
void dispose_poly(poly p);
void reverse_poly(poly p);
void put_poly(poly p);

#endif
