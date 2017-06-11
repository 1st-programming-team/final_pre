#include "mergesort.h"

void Merge(int a[], int low, int mid, int high);

void MergeSort(int a[], int low, int high);

void mergeSort(int a[], int l, int h) {
    MergeSort(a, l, h - 1);
}

void MergeSort(int a[], int low, int high) {
    int mid = 0;
    if (high > low) {
        mid = (high + low) / 2;
        MergeSort(a, low, mid);
        MergeSort(a, mid + 1, high);
        //if((high-low)>=1)
        Merge(a, low, mid, high);
        return;
    }
    else return;
}

void Merge(int a[], int low, int mid, int high) {
    int i = low, j = mid + 1, p = 0;
    int * A;
    A = (int*)malloc((high - low + 1) * sizeof(int*));

    if (!A) return;
    //range the smaller one to the front
    //both part in not NULL
    while (i <= mid&&j <= high) {
        A[p++] = (a[i] < a[j]) ? a[i++] : a[j++];
    }

    //First one is not NULL
    while (i <= mid) {
        A[p++] = a[i++];
    }

    //Second one is not NULL
    while (j <= high) {
        A[p++] = a[j++];
    }

    //copy the result to original array
    for (p = 0, i = low; i <= high; i++, p++) {
        a[i] = A[p];
    }
    free(A);
}


void SelectionSort(int a[], int low, int high) {
    int i, j, minflag = 0;
    int temp;
    high--;
    for (j = low; j <= high; j++) {
        //find min
        for (i = j, minflag = j; i < high; i++) {
            if (a[minflag] > a[i + 1]) {

                minflag = i + 1;
            }
        }

        //chage it to the front
        temp = a[j];
        a[j] = a[minflag];
        a[minflag] = temp;
    }
}


typedef int T;

#define cmp(a, b) \
    ((a) > (b))

static void MergeSort2(T *l, T *h);

static void Merge2(T *l, T *m, T *h);

void mergeSort2(int a[], int l, int h) {
    MergeSort2(a + l, a + h);
}

#define swap(a, b) \
    do {\
        T __tmp = (a);\
        (a) = (b);\
        (b) = __tmp;\
    } while(0)

#define MERGE_W    (0x08U)
#define MERGE_MASK (~(MERGE_W - 1))

static void MergeSort2(T *l, T *h) {
    register int top = 1, segt = 1;
    
    register T **stack = (T**)malloc((((h - l) >> 1) + 5) * sizeof(T*));
    register T **seg = (T**)malloc((((h - l) >> 1) + 5) * sizeof(T*));
    T *A = (T*)malloc((h - l + 5) * sizeof(T));
    
    stack[0] = h;
    stack[1] = l;
    
    seg[0] = l + MERGE_W;
    seg[1] = l;
    
    while (top--) {
        register T *l = stack[top + 1], *r = stack[top], *i;
        while ((r - l) & MERGE_MASK) {
            stack[++top] = r = l + ((r - l) >> 1);
        }
        for (; l != r; ++l) {
            for (i = l + 1; i != r; ++i) {
                if (cmp(*l, *i)) {
                    swap(*l, *i);
                }
            }
        }
        while (seg[segt] - seg[segt - 1] == ((r - seg[segt - 1]) >> 1)) {
            l = seg[segt - 1];
            register T *m = i = seg[segt];
            register T *p = A;

            while (l != m && i != r) {
                *p++ = cmp(*i, *l) ? *l++ : *i++;
            }

            while (l != m) {
                *p++ = *l++;
            }

            while (i != r) {
                *p++ = *i++;
            }

            for (p = A, i = seg[segt - 1]; i != r; i++, p++) {
                *i = *p;
            }
            --segt;
        }
        seg[++segt] = r;
    }

    free(stack);
    free(seg);
    free(A);
}

