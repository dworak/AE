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
 Funkcja Rastrigina.
 Osiaga minimum globalne wynoszace -wymiar_przestrzeni dla wartosci zmiennych wynoszacych 0.
 */
void rastrigin_function(int population_size, int chromosome_length, int space_dimension)
{
    int i, j;
    for(i = 0; i < population_size; i++)
    {
        double partial_result = 0;
        for(j = 0; j < space_dimension; j++)
        {
            partial_result += pow(chromosome_decimal_values[i][j], 2) - cos(18 * M_PI * chromosome_decimal_values[i][j]);
        }
        fitness[i] = partial_result;
    }
}

/*
 Funkcja Easoma.
 Osiaga minimum globalne wynoszace -1 dla wartosci zmiennych wynoszacych pi.
 */
void easom_function(int population_size, int chromosome_length, int space_dimension)
{
    int i;
    for(i = 0; i < population_size; i++)
    {
        fitness[i] = - cos(chromosome_decimal_values[i][0]) * cos(chromosome_decimal_values[i][1]) * pow(M_E, (- pow((chromosome_decimal_values[i][0] - M_PI), 2) - pow((chromosome_decimal_values[i][1] - M_PI), 2)));
    }
}

/*
 Funkcja Schaffera F6.
 Osiaga minimum globalne wynoszace 0 dla wartosci zmiennych wynoszacych 0.
 Bardzo duza szansa znalezieniu minimum lokalnego zamiast globalnego!
 */
void schaffer_f6_function(int population_size, int chromosome_length, int space_dimension)
{
    int i, j;
    for(i = 0; i < population_size; i++)
    {
        double partial_result = 0;
        for(j = 0; j < space_dimension; j++)
        {
            partial_result += pow(chromosome_decimal_values[i][j], 2);
        }
        fitness[i] = 0.5 + ((pow((sin(sqrt(partial_result))), 2)) - 0.5) / (pow((1 + 0.001 * partial_result), 2));
    }
}

/*
 Funkcja De Jonga 1.
 Osiaga minimum globalne wynoszace 0 dla wartosci zmiennych wynoszacych 0.
 */
void de_jong_1_function(int population_size, int chromosome_length, int space_dimension)
{
    int i, j;
    for(i = 0; i < population_size; i++)
    {
        double partial_result = 0;
        for(j = 0; j < space_dimension; j++)
        {
            partial_result += pow(chromosome_decimal_values[i][j], 2);
        }
        fitness[i] = partial_result;
    }
}

/*
 Dolina Rosenbrocka.
 Osiaga minimum globalne wynoszace 0 dla wartosci zmiennych wynoszacych 1.
 */
void rosenbrock_valley(int population_size, int chromosome_length, int space_dimension)
{
    int i, j;
    for(i = 0; i < population_size; i++)
    {
        double partial_result = 0;
        for(j = 0; j < space_dimension - 1; j++)
        {
            partial_result += pow(chromosome_decimal_values[i][j + 1] - pow(chromosome_decimal_values[i][j], 2), 2) + pow((1 - chromosome_decimal_values[i][j]), 2);
        }
        fitness[i] = 100 * partial_result;
    }
}

/*
 Funkcja Griewanka.
 Osiaga minimum globalne wynoszace 0 dla wartosci zmiennych wynoszacych 0.
 */
void griewank_function(int population_size, int chromosome_length, int space_dimension)
{
    int i, j;
    for(i = 0; i < population_size; i++)
    {
        double first_part_result = 0;
        double second_part_result = 1;
        for(j = 0; j < space_dimension; j++)
        {
            first_part_result += pow(chromosome_decimal_values[i][j], 2);
            second_part_result *= cos(chromosome_decimal_values[i][j] / sqrt(i + 1));
        }
        fitness[i] = 1 + first_part_result / 4000 - second_part_result;
    }
}
