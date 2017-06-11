#ifndef POLY__TRIE_HEADER_FILE
#define POLY__TRIE_HEADER_FILE
#include "poly.h"

#define TRIE_MAX_LEN (64)

struct trie_node_struct__;
typedef struct trie_node_struct__ {
    struct trie_node_struct__ *sub[TRIE_MAX_LEN];
    int idx;
} trie_node;

struct trie_struct___;
typedef struct trie_struct___ {
    trie_node root;
    int capacity;
    int top;
    poly *data;
} *trie;

trie make_trie();
void set_elem(trie, char*, poly);
poly get_elem(trie, char*);
void del_elem(trie, char*);
void dispose_trie(trie);

#endif // POLY__TRIE_HEADER_FILE
