/*
 Sortowanie quicksort tablic wraz z ich indeksami. Sortowanie rosnace.
 */
void quickSort(double *a, int *indexes, int l, int r)
{
    int j;

    if( l < r )
    {
        j = partition(a, indexes, l, r);
        quickSort(a, indexes, l, j-1);
        quickSort(a, indexes, j+1, r);
    }
}

/*
 Funkcja podzialu sortowania quicksort.
 */
int partition(double *a, int *indexes, int l, int r)
{
    int i, j, temp_int;
    double t;
    double pivot;
    pivot = a[l];
    i = l;
    j = r+1;

    while(1)
    {
        do ++i;
        while(i <= r && a[i] <= pivot);
        do --j;
        while( a[j] > pivot );
        if( i >= j ) break;
        t = a[i];
        a[i] = a[j];
        a[j] = t;
        temp_int = indexes[i];
        indexes[i] = indexes[j];
        indexes[j] = temp_int;
    }
    t = a[l];
    a[l] = a[j];
    a[j] = t;
    temp_int = indexes[l];
    indexes[l] = indexes[j];
    indexes[j] = temp_int;
    return j;
}

/*
 Sortowanie quicksort samych indeksow. Sortowanie rosnace.
 */
void quickSort_indexes(int *a, int l, int r)
{
    int j;

    if( l < r )
    {
        j = partition_indexes(a, l, r);
        quickSort_indexes(a, l, j-1);
        quickSort_indexes(a, j+1, r);
    }
}

/*
 Funkcja podzialu sortowania quicksort.
 */
int partition_indexes(int *a, int l, int r)
{
    int i, j;
    int t;
    int pivot;
    pivot = a[l];
    i = l;
    j = r+1;

    while(1)
    {
        do ++i;
        while(i <= r && a[i] <= pivot);
        do --j;
        while( a[j] > pivot );
        if( i >= j ) break;
        t = a[i];
        a[i] = a[j];
        a[j] = t;
    }
    t = a[l];
    a[l] = a[j];
    a[j] = t;
    return j;
}
