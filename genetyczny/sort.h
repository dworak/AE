#ifndef SORT_H_INCLUDED
#define SORT_H_INCLUDED

void quickSort(double *a, int *indexes, int l, int r);
int partition(double *a, int *indexes, int l, int r);
void quickSort_indexes(int *a, int l, int r);
int partition_indexes(int *a, int l, int r);

#endif // SORT_H_INCLUDED
