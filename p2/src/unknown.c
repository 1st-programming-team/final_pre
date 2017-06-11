#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parser.h"
#include "trie.h"

trie dict = 0;

poly figure(char *buffer)
{
	tokens token_list = lex(buffer);
	poly ret = 0;
	if (!token_list.list) {
		puts("Expression error!");
		return 0;
	}
	poly_data p = get_expr(&token_list);
	if (p.data) {
		if (token_list.index < token_list.size) {
			puts("Syntax Error!\nunexpected token");
            dispose_poly(p.data);
		} else {
			ret = p.data;
		}
	}
	free(token_list.list);
	return ret;
}

char *get_line()
{
    int ch, capacity = 1, idx = 0;
    char *buffer = (char*)malloc(capacity * sizeof(char));
    while (ch = getchar(), ch != EOF && ch != '\n') {
        buffer[idx++] = ch;
        if (idx == capacity) {
            char *ptr = (char*)malloc(2 * capacity * sizeof(char));
            memcpy(ptr, buffer, capacity * sizeof(char));
            free(buffer); buffer = ptr;
            capacity <<= 1;
        }
    }
    buffer[idx] = 0;
    return buffer;
}

#define is_number(ch) \
    ((ch) >= '0' && (ch) <= '9')

void menu_input_poly()
{
    puts("input a poly.\ne.g. y = x^2 + 2x + 1");
    char *buffer, *p;
    rep: buffer = get_line(), p = buffer;
    while (*p == ' ' || *p == '\n' || *p == '\t' || *p == '\r') {
        ++p;
    }
    if (is_number(*p)) {
        puts("invalid name.");
        free(buffer);
        goto rep;
    }
    char name[1024];
    if (sscanf(p + 1, "%[A-Za-z0-9_]", name + 1) != 1) {
        name[1] = 0;
    }
    name[0] = *p;
    p += strlen(name);
    if (*p != ' ' && *p != '\n' && *p != '\t' && *p != '\r') {
        puts("invalid name.");
        free(buffer);
        goto rep;
    }
    while (*p == ' ' || *p == '\n' || *p == '\t' || *p == '\r') {
        ++p;
    }
    if (*p != '=') {
        puts("invalid expression.");
        free(buffer);
        goto rep;
    }
    ++p;
    poly pp = figure(p);
    if (!pp) {
        puts("invalid poly input.");
        free(buffer);
        goto rep;
    }
    free(buffer);
    set_elem(dict, name, pp);
}

void menu_add_poly()
{
    puts("add poly b to poly a.(a += b)\ne.g. a b");
    char *buffer, *p;
    rep: buffer = get_line(), p = buffer;
    while (*p == ' ' || *p == '\n' || *p == '\t' || *p == '\r') {
        ++p;
    }
    if (is_number(*p)) {
        puts("invalid name.");
        free(buffer);
        goto rep;
    }
    char name[1024], name1[1024];
    if (sscanf(p + 1, "%[A-Za-z0-9_]", name + 1) != 1) {
        name[1] = 0;
    }
    name[0] = *p;
    p += strlen(name);
    if (*p != ' ' && *p != '\n' && *p != '\t' && *p != '\r') {
        puts("invalid name.");
        free(buffer);
        goto rep;
    }
    while (*p == ' ' || *p == '\n' || *p == '\t' || *p == '\r') {
        ++p;
    }
    if (is_number(*p)) {
        puts("invalid name.");
        free(buffer);
        goto rep;
    }
    if (sscanf(p + 1, "%[A-Za-z0-9_]", name1 + 1) != 1) {
        name1[1] = 0;
    }
    name1[0] = *p;
    p += strlen(name);
    if (*p && *p != ' ' && *p != '\n' && *p != '\t' && *p != '\r') {
        puts("invalid name.");
        free(buffer);
        goto rep;
    }
    poly lhs = get_elem(dict, name);
    poly rhs = get_elem(dict, name1);
    if (!lhs || !rhs) {
        puts("variable undefined.");
        getchar();
    }
    add_poly(lhs, rhs);
}

void menu_sub_poly()
{
    puts("sub poly a by poly b.(a -= b)\ne.g. a b");
    char *buffer, *p;
    rep: buffer = get_line(), p = buffer;
    while (*p == ' ' || *p == '\n' || *p == '\t' || *p == '\r') {
        ++p;
    }
    if (is_number(*p)) {
        puts("invalid name.");
        free(buffer);
        goto rep;
    }
    char name[1024], name1[1024];
    if (sscanf(p + 1, "%[A-Za-z0-9_]", name + 1) != 1) {
        name[1] = 0;
    }
    name[0] = *p;
    p += strlen(name);
    if (*p != ' ' && *p != '\n' && *p != '\t' && *p != '\r') {
        puts("invalid name.");
        free(buffer);
        goto rep;
    }
    while (*p == ' ' || *p == '\n' || *p == '\t' || *p == '\r') {
        ++p;
    }
    if (is_number(*p)) {
        puts("invalid name.");
        free(buffer);
        goto rep;
    }
    if (sscanf(p + 1, "%[A-Za-z0-9_]", name1 + 1) != 1) {
        name1[1] = 0;
    }
    name1[0] = *p;
    p += strlen(name);
    if (*p && *p != ' ' && *p != '\n' && *p != '\t' && *p != '\r') {
        puts("invalid name.");
        free(buffer);
        goto rep;
    }
    poly lhs = get_elem(dict, name);
    poly rhs = get_elem(dict, name1);
    if (!lhs || !rhs) {
        puts("variable undefined.");
        getchar();
    }
    sub_poly(lhs, rhs);
}

void menu_mul_poly()
{
    puts("mul poly b to poly a.(a *= b)\ne.g. a b");
    char *buffer, *p;
    rep: buffer = get_line(), p = buffer;
    while (*p == ' ' || *p == '\n' || *p == '\t' || *p == '\r') {
        ++p;
    }
    if (is_number(*p)) {
        puts("invalid name.");
        free(buffer);
        goto rep;
    }
    char name[1024], name1[1024];
    if (sscanf(p + 1, "%[A-Za-z0-9_]", name + 1) != 1) {
        name[1] = 0;
    }
    name[0] = *p;
    p += strlen(name);
    if (*p != ' ' && *p != '\n' && *p != '\t' && *p != '\r') {
        puts("invalid name.");
        free(buffer);
        goto rep;
    }
    while (*p == ' ' || *p == '\n' || *p == '\t' || *p == '\r') {
        ++p;
    }
    if (is_number(*p)) {
        puts("invalid name.");
        free(buffer);
        goto rep;
    }
    if (sscanf(p + 1, "%[A-Za-z0-9_]", name1 + 1) != 1) {
        name1[1] = 0;
    }
    name1[0] = *p;
    p += strlen(name);
    if (*p && *p != ' ' && *p != '\n' && *p != '\t' && *p != '\r') {
        puts("invalid name.");
        free(buffer);
        goto rep;
    }
    poly lhs = get_elem(dict, name);
    poly rhs = get_elem(dict, name1);
    if (!lhs || !rhs) {
        puts("variable undefined.");
        getchar();
    }
    mul_poly(lhs, rhs);
}

void menu_show_poly()
{
    puts("input a poly var name.\ne.g. y");
    char *buffer, *p;
    rep: buffer = get_line(), p = buffer;
    while (*p == ' ' || *p == '\n' || *p == '\t' || *p == '\r') {
        ++p;
    }
    if (is_number(*p)) {
        puts("invalid name.");
        free(buffer);
        goto rep;
    }
    char name[1024];
    if (sscanf(p + 1, "%[A-Za-z0-9_]", name + 1) != 1) {
        name[1] = 0;
    }
    name[0] = *p;
    p += strlen(name);
    if (*p && *p != ' ' && *p != '\n' && *p != '\t' && *p != '\r') {
        puts("invalid name.");
        free(buffer);
        goto rep;
    }
    poly pp = get_elem(dict, name);
    if (!pp) {
        puts("variable undefined.");
        getchar();
    } else {
        printf("get poly:\n%s = ", name); put_poly(pp);
        getchar();
    }
}

int pop_menu()
{
    system("cls");
    printf("1. input poly\n2. add poly\n3. sub poly\n4. mul poly\n5. show poly\n6. exit\n");
    char *buffer;
    buffer = get_line();
    switch (buffer[0]) {
    default: {
        exit_: free(buffer);
        return 1;
    }
    case '1': {
        system("cls");
        menu_input_poly();
    } goto exit_;
    case '2': {
        system("cls");
        menu_add_poly();
    } goto exit_;
    case '3': {
        system("cls");
        menu_sub_poly();
    } goto exit_;
    case '4': {
        system("cls");
        menu_mul_poly();
    } goto exit_;
    case '5': {
        system("cls");
        menu_show_poly();
    } goto exit_;
    case '6': {
        free(buffer);
        return 0;
    }
    }
}
/*
    while (1) {
		char *expr = get_line();
		figure(expr);
		free(expr);
	}
	return 0;
*/
int main(int argc, char **argv)
{
    dict = make_trie();
	while (pop_menu());
	dispose_trie(dict);
	return 0;
}
