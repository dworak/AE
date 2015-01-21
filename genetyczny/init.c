#include "core.h"
/*
 Tablica chromosomow. Przyjmuje wartosci 0 lub 1;
 Kazdy wiersz stanowi jeden chromosom.
 W chromosomie zawarte sa kolejne liczby reprezentujace rozwiazanie funkcji dopasowania.
 */
extern int **chromosomes;

//Tymczasowa kopia tablicy chromosomow
extern int **temp_chromosomes;

//Tablica przechowujaca reprezentacje dziesietna liczb zawartych w chromosomach.
extern double **chromosome_decimal_values;

//Tymczasowa kopia dziesietnych reprezentacji chromosomow
extern double **temp_chromosome_decimal_values;

/*
 Tablica dopasowan poszczegolnych chromosomow.
 Jej wartosci zmieniane sa w kazdej kolejnej iteracji.
 */
extern double *fitness;

//Tymczasowa tablica dopasowan.
extern double *temp_fitness;

//Tablica unikalnych wartosci losowych.
extern double *random_values;

//Tablica indeksow unikalnych wartosci losowych.
extern int *random_indexes;

//Tablica dopasowan do posortowania
extern double *sorted_fitness;

//Tablica indeksow dopasowan do posortowania
extern int *sorted_fitness_indexes;

/*
 Funkcja generujaca ciag unikalnych liczb calkowitych z przedzialu [0, rozmiar_populacji).
 */
void generate_set_of_unique_values(int size, double *values, int *indexes)
{
    int i;

    for(i = 0; i < size; i++)
    {
        values[i] = generate_random_double_value(0, 1);
        indexes[i] = i;
    }

    quickSort(values, indexes, 0, size - 1);
}

/*
 Inicjalizacja struktur globalnych.
 */
void initialize_structures(int population_size, int chromosome_length, int space_dimension)
{
    int i;

    chromosomes = (int**)malloc(sizeof(int*) * population_size);
    for(i = 0; i < population_size; i++)
    {
        chromosomes[i] = (int*)malloc(sizeof(int) * chromosome_length);
    }

    temp_chromosomes = (int**)malloc(sizeof(int*) * population_size);
    for(i = 0; i < population_size; i++)
    {
        temp_chromosomes[i] = (int*)malloc(sizeof(int) * chromosome_length);
    }

    chromosome_decimal_values = (double**)malloc(sizeof(double*) * population_size);
    for(i = 0; i < population_size; i++)
    {
        chromosome_decimal_values[i] = (double*)malloc(sizeof(double) * space_dimension);
    }

    temp_chromosome_decimal_values = (double**)malloc(sizeof(double*) * population_size);
    for(i = 0; i < population_size; i++)
    {
        temp_chromosome_decimal_values[i] = (double*)malloc(sizeof(double) * space_dimension);
    }

    fitness = (double*)malloc(sizeof(double) * population_size);

    temp_fitness = (double*)malloc(sizeof(double) * population_size);

    random_values = (double*)malloc(sizeof(double) * population_size);

    random_indexes = (int*)malloc(sizeof(int) * population_size);

    sorted_fitness = (double*)malloc(sizeof(double) * population_size);

    sorted_fitness_indexes = (int*)malloc(sizeof(int) * population_size);
}

/*
 Przepisywanie list chromosomow i ich dziesietnych reprezentacji do list tymczasowych.
 Operacja ta moze zostac wykonana tylko raz w ramach jednej operacji elementarnej (selekcja, krzyzowanie, mutacja).
 */
void rewrite_chromosome_lists(int population_size, int chromosome_length, int space_dimension)
{
    int i, j;

    // tworzenie nowej populacji rodzicow skladajacej sie z wylosowanych osobnikow
    for(i = 0; i < population_size; i++)
    {
        for(j = 0; j < chromosome_length; j++)
        {
            temp_chromosomes[i][j] = chromosomes[i][j];
        }
    }

    // tworzenie tymczasowej tablicy wartosci dziesietnych chromosomow
    for(i = 0; i < population_size; i++)
    {
        for(j = 0; j < space_dimension; j++)
        {
            temp_chromosome_decimal_values[i][j] = chromosome_decimal_values[i][j];
        }
    }
}
