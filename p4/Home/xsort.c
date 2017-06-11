#include "xsort.h"

typedef int T;

#define swap(a, b) \
    do {\
        T __tmp = (a);\
        (a) = (b);\
        (b) = __tmp;\
    } while (0)

#define cmp(a, b) \
    ((a) < (b))

#define MAX_W (5)

void xsort(T a[], int begin, int end)
{
    register T *i, *m, *r;
    register T le;
    
    register int top = 1;
    register T **stack = (T**)malloc((((end - begin) >> 1) + 1) * sizeof(T*));
    stack[0] = a + end;
    stack[1] = a + begin - 1;
    
    while (top--) {
        T *l = m = stack[top + 1] + 1;
        for (r = stack[top]; r - l > MAX_W; r = m, m = l) {
            for (i = m + 1, le = *m; i != r; ++i) {
                if (cmp(*i, le)) {
                    ++m; swap(*m, *i);
                }
            }
            swap(*l, *m);
            stack[++top] = m;       // ??
        }
        for (i = l; i != r; ++i) {
            for (m = i + 1; m != r; ++m) {
                if (cmp(*m, *i)) {
                    swap(*m, *i);
                }
            }
        }
    }
    free(stack);
}