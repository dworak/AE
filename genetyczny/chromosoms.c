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
 Obliczanie funkcji dopasowania.
 */
void calculate_fitness(int population_size, int binary_number_length, int chromosome_length, int space_dimension, int function_number)
{
    switch(function_number)
    {
        case 0:
            rastrigin_function(population_size, chromosome_length, space_dimension);
            break;
        case 1:
            easom_function(population_size, chromosome_length, space_dimension);
            break;
        case 2:
            schaffer_f6_function(population_size, chromosome_length, space_dimension);
            break;
        case 3:
            de_jong_1_function(population_size, chromosome_length, space_dimension);
            break;
        case 4:
            rosenbrock_valley(population_size, chromosome_length, space_dimension);
            break;
        case 5:
            griewank_function(population_size, chromosome_length, space_dimension);
            break;
        default:
            break;
    }
}


/*
 Generowanie chromosomow.
 */
void populate_chromosomes(int population_size, int chromosome_length)
{
    int i, j;
    for(i = 0; i < population_size; i++)
    {
        for(j = 0; j < chromosome_length; j++)
        {
            chromosomes[i][j] = generate_random_binary_value();
        }
    }
}

/*
 Obliczanie dziesietnych reprezentacji wartosci chromosomow.
 */
void calculate_decimal_chromosome_values(int population_size, int binary_number_length, int chromosome_length, int space_dimension, int left_margin, int domain_length)
{
    int i, j, k;
    for(i = 0; i < population_size; i++)
    {
        for(j = 0; j < space_dimension; j++)
        {
            int temp_value = 0;
            for(k = 0; k < binary_number_length; k++)
            {
                temp_value += pow(2, k) * chromosomes[i][j * binary_number_length + k];
            }
            chromosome_decimal_values[i][j] = left_margin + temp_value * domain_length / (pow(2, binary_number_length) - 1);
        }
    }

}

void calculate_binary_chromosome_values(int population_size, int binary_number_length, int chromosome_length, int space_dimension, int left_margin, int domain_length)
{
    int i, j, k;
    for(i = 0; i < population_size; ++i)
    {
        for(j = 0; j < space_dimension; ++j)
        {
            double value = chromosome_decimal_values[i][j] - left_margin;
            value = value * (pow(2, binary_number_length) - 1) / domain_length;
            for(k = binary_number_length; k > 0; --k)
            {
                double divisor = pow(2, k-1);
                chromosomes[i][j * binary_number_length + k-1] = floor(value / divisor);
                value -= chromosomes[i][j * binary_number_length + k - 1] * divisor;
            }
        }
    }
}
