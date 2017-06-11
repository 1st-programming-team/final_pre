#include "quicksort.h"

void quicksort(int *array, int left_offset, int right_offset)
{
    int offset;
    int div_offset;
    int tmp;

    if (right_offset - left_offset <= 1)
        return;

    for (offset = left_offset + 1, div_offset = left_offset; offset < right_offset; offset++)
    {
        if (array[offset] < array[left_offset])
        {
            div_offset++;
            tmp = array[div_offset];
            array[div_offset] = array[offset];
            array[offset] = tmp;
        }
    }

    tmp = array[div_offset];
    array[div_offset] = array[left_offset];
    array[left_offset] = tmp;

    quicksort(array, left_offset, div_offset);
    quicksort(array, div_offset + 1, right_offset);
}

const int qwidth = 5;

void bbs(int a[], int begin, int end)
{
    register int i, j;
    for (i = begin; i != end; ++i) {
        for (j = i + 1; j != end; ++j) {
            if (a[i] > a[j]) {
                int tmp = a[i];
                a[i] = a[j];
                a[j] = tmp;
            }
        }
    }
}

void quicksort2(int *array, int left_offset, int right_offset)
{
    int offset;
    int div_offset;
    int tmp;

    if (right_offset - left_offset <= qwidth) {
        bbs(array, left_offset, right_offset);
        return;
    }
        //return;

    for (offset = left_offset + 1, div_offset = left_offset; offset < right_offset; offset++)
    {
        if (array[offset] < array[left_offset])
        {
            div_offset++;
            tmp = array[div_offset];
            array[div_offset] = array[offset];
            array[offset] = tmp;
        }
    }

    tmp = array[div_offset];
    array[div_offset] = array[left_offset];
    array[left_offset] = tmp;

    quicksort2(array, left_offset, div_offset);
    quicksort2(array, div_offset + 1, right_offset);
}

