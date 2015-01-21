#include "core.h"

/*
 Tablica chromosomow. Przyjmuje wartosci 0 lub 1;
 Kazdy wiersz stanowi jeden chromosom.
 W chromosomie zawarte sa kolejne liczby reprezentujace rozwiazanie funkcji dopasowania.
 */
extern int **chromosomes;

//Tablica przechowujaca reprezentacje dziesietna liczb zawartych w chromosomach.
extern double **chromosome_decimal_values;

/*
 Mutacja.
 */
void binary_mutation(int population_size, int chromosome_length, double mutation_probability)
{
    int i, j;
    double mutation_decision;

    for(i = 0; i < population_size; i++)
    {
        for(j = 0; j < chromosome_length; j++)
        {
            mutation_decision = generate_random_double_value(0, 1);
            if(mutation_decision < mutation_probability)
            {
                if(chromosomes[i][j] == 0) chromosomes[i][j] = 1;
                else chromosomes[i][j] = 0;
            }
        }
    }
}

void arithmetic_mutation(int population_size, int space_dimension, int current_iteration, int number_of_iterations, int left_margin, int domain_length, int c)
{
    double r = generate_random_double_value(0, 1);
    int i, j;
    for(i = 0; i < population_size; ++i)
    {
        for(j = 0; j < space_dimension; ++j)
        {
            if(rand()%2)
            {
                chromosome_decimal_values[i][j] += (left_margin + domain_length - chromosome_decimal_values[i][j]) * (1 - pow(r, (1 - current_iteration / (double)number_of_iterations)*c));
            } else {
                chromosome_decimal_values[i][j] -= (chromosome_decimal_values[i][j] - left_margin) * (1 - pow(r, (1 - current_iteration / (double)number_of_iterations)*c));
            }
        }
    }

}

void mutation(int population_size, int chromosome_length, int type, int left_margin, int domain_length, double mutation_probability, int current_iteration, int number_of_iterations, int c, int space_dimension, int binary_number_length)
{
    if(type%2)
    {
        binary_mutation(population_size, chromosome_length, mutation_probability);
        calculate_decimal_chromosome_values(population_size, binary_number_length, chromosome_length, space_dimension, left_margin, domain_length);
    } else {
        arithmetic_mutation(population_size, space_dimension, current_iteration, number_of_iterations, left_margin, domain_length, c);
        calculate_binary_chromosome_values(population_size, binary_number_length, chromosome_length, space_dimension, left_margin, domain_length);
    }
}


