#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "quicksort.h"
#include "mergesort.h"
#include "xsort.h"

typedef int T;
typedef void (*sort_func)(T a[], int l, int r);

//#define DEBUG

#ifdef DEBUG
const size_t nsize = 10;
const size_t ncase = 1;
const unsigned int repeat = 1000000;
#else
size_t nsize;
size_t ncase;
unsigned int repeat;
#endif // DEBUG

const T limit_l = 0;
const T limit_h = INT_MAX / 2;

T** gen_data(size_t ncase, size_t size)
{
    T **data = (T**)calloc(ncase, sizeof(T*));
    int i = 0, j;
#ifdef DEBUG
    printf("[src: ");
#endif // DEBUG
    for (; i != ncase; ++i) {
        data[i] = (T*)calloc(size, sizeof(T));
        for (j = 0; j != size; ++j) {
            data[i][j] = rand() % (limit_h - limit_l) + limit_l;
#ifdef DEBUG
            printf("%d ", data[i][j]);
#endif // DEBUG
        }
    }
#ifdef DEBUG
    puts("]");
#endif // DEBUG
    return data;
}

T **copy_data(size_t ncase, size_t size, T **src)
{
    T **data = (T**)calloc(ncase, sizeof(T*));
    int i = 0;
    for (; i != ncase; ++i) {
        data[i] = (T*)calloc(size, sizeof(T));
        memcpy(data[i], src[i], sizeof(T) * size);
    }
    return data;
}

void free_data(size_t ncase, T **data)
{
    int i = 0;
    for (; i != ncase; ++i) {
        free(data[i]);
    }
    free(data);
}

void test_sort(sort_func fn, unsigned int rep, size_t ncase, size_t nsize, T **src, int is_std)
{
    static T ***std;
    register int i = 0;
    if (!~is_std) {
        for (i = 0; i != rep; ++i) {
            free_data(ncase, std[i]);
        }
        free(std);
        return;
    } else if (is_std) {
        printf("[std]");
    }
    T ***data = (T***)calloc(rep, sizeof(T**));
    for (; i != rep; ++i) {
        data[i] = copy_data(ncase, nsize, src);
    }
    register int j;
    clock_t t1 = clock();
    for (i = 0; i != rep; ++i) {
        for (j = 0; j != ncase; ++j) {
            fn(data[i][j], 0, nsize);
        }
    }
    clock_t t2 = clock();
    printf("%lf ms\n", ((double)(t2 - t1))/(rep*ncase));
    if (!is_std) {
        for (i = 0; i != ncase; ++i) {
            for (j = 0; j != nsize; ++j) {
                if (std[0][i][j] != data[0][i][j]) {
                    printf("mismatch at case %d id %d: %d[std] : %d\n", i, j, std[0][i][j],
                        data[0][i][j]);
                }
            }
        }
        for (i = 0; i != rep; ++i) {
            free_data(ncase, data[i]);
        }
        free(data);
    } else {
#ifdef DEBUG
        for (j = 0; j != ncase; ++j) {
            printf("case %d: ", j);
            for (i = 0; i != nsize; ++i) {
                printf("%d ", data[0][j][i]);
            }
            printf("\n");
        }
#endif // DEBUG
        std = data;
    }
}

#define test(s) \
    do {\
        printf("%s: ", #s);\
        test_sort(s, repeat, ncase, nsize, data, 0);\
    } while(0)
#define std(s) \
    do {\
        printf("%s: ", #s);\
        test_sort(s, repeat, ncase, nsize, data, 1);\
    } while(0)
#define endtest() \
    test_sort(0, 0, 0, 0, 0, -1)

int cmp(const void *a, const void *b)
{
    return *(const int*)a - *(const int *)b;
}

void q_sort(T a[], int begin, int end)
{
    qsort(a, end - begin, sizeof(T), cmp);
}

int main(int argc, char *argv[])
{
#ifndef DEBUG
    while (1) {
        printf("nsize ncase repeat = ");
        scanf("%d%d%d", &nsize, &ncase, &repeat);
#endif // !DEBUG
        srand(time(0));
        T **data = gen_data(ncase, nsize);
        printf("sort %d ints, average %d cases, repeat %d times\n", nsize, ncase, repeat);
        std(q_sort);
        test(quicksort);
        test(quicksort2);
        test(xsort);
        test(SelectionSort);
        test(bbs);
        test(mergeSort);
        test(mergeSort2);
        endtest();
        free_data(ncase, data);
#ifndef DEBUG
    }
#endif // DEBUG
    system("pause");
}