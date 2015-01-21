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
 Krzyzowanie jednopunktowe.
 */
void one_point_crossover(int population_size, int chromosome_length)
{
    int i, j;

    for(i = 0; i < population_size; i+= 2)
    {
        // wyznaczanie punktu, po ktorym poprowadzona zostanie linia ciecia
        int cross_point = generate_random_integer_value(0, chromosome_length - 2);
        for(j = cross_point + 1; j < chromosome_length; j++)
        {
            chromosomes[random_indexes[i]][j] = temp_chromosomes[random_indexes[i + 1]][j];
            chromosomes[random_indexes[i + 1]][j] = temp_chromosomes[random_indexes[i]][j];
        }
    }
}

/*
 Krzyzowanie wielopunktowe.
 */
void multiple_points_crossover(int population_size, int chromosome_length)
{
    int i, j, k;

    int *potential_cross_points = (int*)malloc(sizeof(int) * chromosome_length);
    double *sort_list = (double*)malloc(sizeof(double) * chromosome_length);

    generate_set_of_unique_values(chromosome_length, sort_list, potential_cross_points);

    int number_of_cross_points = generate_random_integer_value(0, chromosome_length - 2);

    int *cross_points = (int*)malloc(sizeof(int) * number_of_cross_points);

    for(i = 0; i < number_of_cross_points; i++)
    {
        cross_points[i] = potential_cross_points[i];
    }

    quickSort_indexes(cross_points, 0, number_of_cross_points - 1);

    for(i = 0; i < population_size; i+= 2)
    {

        for(k = 0; k < number_of_cross_points - 1; k+= 2)
        {
            for(j = cross_points[k] + 1; j <= cross_points[k + 1]; j++)
            {
                chromosomes[random_indexes[i]][j] = temp_chromosomes[random_indexes[i + 1]][j];
                chromosomes[random_indexes[i + 1]][j] = temp_chromosomes[random_indexes[i]][j];
            }
        }
    }

    free(potential_cross_points);
    free(cross_points);
    free(sort_list);
}

/*
 Krzyzowanie jednostajne.
 */
void uniform_crossover(int population_size, int chromosome_length)
{
    int i, j;
    double should_gene_be_crossed;

    for(i = 0; i < population_size; i+= 2)
    {
        for(j = 0; j < chromosome_length; j++)
        {
            should_gene_be_crossed = generate_random_double_value(0, 1);
            if(should_gene_be_crossed > 0.5)
            {
                chromosomes[random_indexes[i]][j] = temp_chromosomes[random_indexes[i + 1]][j];
                chromosomes[random_indexes[i + 1]][j] = temp_chromosomes[random_indexes[i]][j];
            }
        }
    }
}

/*
Pe³ne krzyzowanie arytmetyczne.
 */
void whole_arithmetic_crossover(int population_size, int chromosome_length)
{
    double alpha = generate_random_double_value(0, 1);
    int i, j;
    for(i = 0; i < population_size; i+=2)
    {
        for(j = 0; j < chromosome_length; ++j)
        {
            chromosome_decimal_values[random_indexes[i]][j] = alpha * temp_chromosome_decimal_values[random_indexes[i]][j] + (1 - alpha) * temp_chromosome_decimal_values[random_indexes[i+1]][j];
            chromosome_decimal_values[random_indexes[i+1]][j] = alpha * temp_chromosome_decimal_values[random_indexes[i+1]][j] + (1 - alpha) * temp_chromosome_decimal_values[random_indexes[i]][j];
        }
    }
}

/*
Pojedyncze krzyzowanie arytmetyczne.
 */
void single_arithmetic_crossover(int population_size, int chromosome_length)
{
    double alpha = generate_random_double_value(0, 1);
    int allele = rand() % chromosome_length, i;
    for(i = 0; i < population_size; i+=2)
    {
        chromosome_decimal_values[random_indexes[i]][allele] = alpha * temp_chromosome_decimal_values[random_indexes[i]][allele] + (1 - alpha) * temp_chromosome_decimal_values[random_indexes[i+1]][allele];
        chromosome_decimal_values[random_indexes[i+1]][allele] = alpha * temp_chromosome_decimal_values[random_indexes[i+1]][allele] + (1 - alpha) * temp_chromosome_decimal_values[random_indexes[i]][allele];
    }
}

/*
Proste krzyzowanie arytmetyczne.
 */
void simple_arithmetic_crossover(int population_size, int chromosome_length)
{
    double alpha = generate_random_double_value(0, 1);
    int cross_point = rand() % (chromosome_length - 1), i, j;
    for(i = 0; i < population_size; i+=2)
    {
        for(j = cross_point; j < chromosome_length; ++j)
        {
            chromosome_decimal_values[random_indexes[i]][j] = alpha * temp_chromosome_decimal_values[random_indexes[i]][j] + (1 - alpha) * temp_chromosome_decimal_values[random_indexes[i+1]][j];
            chromosome_decimal_values[random_indexes[i+1]][j] = alpha * temp_chromosome_decimal_values[random_indexes[i+1]][j] + (1 - alpha) * temp_chromosome_decimal_values[random_indexes[i]][j];
        }
    }
}

/*
 Krzyzowanie.
 */
void crossover(int population_size, int chromosome_length, int space_dimension, int crossover_number, int left_margin, int domain_length, int binary_number_length)
{
    generate_set_of_unique_values(population_size, random_values, random_indexes);

    rewrite_chromosome_lists(population_size, chromosome_length, space_dimension);

    switch(crossover_number)
    {
        case 0:
            one_point_crossover(population_size, chromosome_length);
            calculate_decimal_chromosome_values(population_size, binary_number_length, chromosome_length, space_dimension, left_margin, domain_length);
            break;
        case 1:
            multiple_points_crossover(population_size, chromosome_length);
            calculate_decimal_chromosome_values(population_size, binary_number_length, chromosome_length, space_dimension, left_margin, domain_length);
            break;
        case 2:
            uniform_crossover(population_size, chromosome_length);
            calculate_decimal_chromosome_values(population_size, binary_number_length, chromosome_length, space_dimension, left_margin, domain_length);
            break;
        case 3:
            whole_arithmetic_crossover(population_size, space_dimension);
            calculate_binary_chromosome_values(population_size, binary_number_length, chromosome_length, space_dimension, left_margin, domain_length);
            break;
        case 4:
            single_arithmetic_crossover(population_size, space_dimension);
            calculate_binary_chromosome_values(population_size, binary_number_length, chromosome_length, space_dimension, left_margin, domain_length);
            break;
        case 5:
            simple_arithmetic_crossover(population_size, space_dimension);
            calculate_binary_chromosome_values(population_size, binary_number_length, chromosome_length, space_dimension, left_margin, domain_length);
            break;
        default:
            break;
    }
}
