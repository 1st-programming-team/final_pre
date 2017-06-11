#include "trie.h"

trie make_trie()
{
    trie t = (trie)calloc(1, sizeof(struct trie_struct___));
    t->capacity = TRIE_MAX_LEN;
    t->data = (poly*)malloc(t->capacity * sizeof(poly));
    t->root.idx = -1;
    return t;
}

static trie_node *make_trie_node()
{
    trie_node *p = (trie_node*)calloc(1, sizeof(trie_node));
    p->idx = -1;
    return p;
}

static int get_index_of(char ch)
{
    if (ch >= 'a' && ch <= 'z') {
        return ch - 'a';
    } else if (ch >= 'A' && ch <= 'Z') {
        return ch - 'A' + 26;
    } else if (ch >= '0' && ch <= '9') {
        return ch -'0' + 52;
    } else if (ch == '_') {
        return 62;
    } else {
        return 63;
    }
}

#define is_number(ch) \
    ((ch) >= '0' && (ch) <= '9')

void set_elem(trie t, char *name, poly p)
{
    if (!name || !name[0] || is_number(name[0])) {
        return;
    }
    trie_node *ptr = &t->root;
    while (name[1]) {
        int id = get_index_of(*name);
        if (!ptr->sub[id]) {
            ptr->sub[id] = make_trie_node();
        }
        ptr = ptr->sub[id];
        name++;
    }
    int id = get_index_of(*name);
    if (!ptr->sub[id]) {
        ptr->sub[id] = make_trie_node();
    }
    if (~ptr->sub[id]->idx) {
        if (t->data[ptr->sub[id]->idx]) {
            dispose_poly(t->data[ptr->sub[id]->idx]);
        }
        t->data[ptr->sub[id]->idx] = p;
    } else {    // alloc mem for this node
        ptr->sub[id]->idx = t->top;
        t->data[t->top++] = p;
        if (t->top == t->capacity) {    // full
            poly *new_data = (poly*)malloc(t->capacity * 2 * sizeof(poly));
            memcpy(new_data, t->data, sizeof(poly) * t->capacity);
            free(t->data);
            t->data = new_data;
            t->capacity *= 2;
        }
    }
}

void del_elem(trie t, char *name)
{
    if (!name || !*name) {
        return;
    }
    trie_node *ptr = &t->root;
    while (name[1]) {
            int id = get_index_of(*name);
        if (!ptr->sub[id]) {
            return;
        }
        ptr = ptr->sub[id];
        name++;
    }
    int id = get_index_of(*name);
    if (!ptr->sub[id]) {
        return;
    }
    if (~ptr->sub[id]->idx) {
        if (t->data[ptr->sub[id]->idx]) {
            dispose_poly(t->data[ptr->sub[id]->idx]);
            t->data[ptr->sub[id]->idx] = 0;
        }
    }
}

poly get_elem(trie t, char *name)
{
    if (!name || !*name) {
        return 0;
    }
    trie_node *ptr = &t->root;
    while (name[1]) {
        int id = get_index_of(*name);
        if (!ptr->sub[id]) {
            return 0;
        }
        ptr = ptr->sub[id];
        name++;
    }
    int id = get_index_of(*name);
    if (!ptr->sub[id]) {
        return 0;
    }
    if (~ptr->sub[id]->idx) {
        return t->data[ptr->sub[id]->idx];
    } else {
        return 0;
    }
}

static void dispose_trie_node(trie_node *r)
{
    trie_node **p = r->sub, **pend = p + TRIE_MAX_LEN;
    for (; p != pend; ++p) {
        if (*p) {
            dispose_trie_node(*p);
        }
    }
    free(r);
}

void dispose_trie(trie t)
{
    trie_node **p = t->root.sub, **pend = p + TRIE_MAX_LEN;
    for (; p != pend; ++p) {
        if (*p) {
            dispose_trie_node(*p);
        }
    }
    poly *pp = t->data, *ppend = pp + t->top;
    for (; pp != ppend; ++pp) {
        if (*pp) {
            dispose_poly(*pp);
        }
    }
    free(t->data);
    free(t);
}
