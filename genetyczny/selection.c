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
 Selekcja rangowa.
 */
void range_selection(int population_size, int chromosome_length, int space_dimension, int function_direction, double selection_pressure)
{
    int i, j;
    for(i = 0; i < population_size; i++)
    {
        sorted_fitness[i] = fitness[i];
        sorted_fitness_indexes[i] = i;
    }

    quickSort(sorted_fitness, sorted_fitness_indexes, 0, population_size - 1);

    int *ranges = (int*)malloc(sizeof(int) * population_size);

    for(i = 0; i < population_size; i++)
    {
        if(function_direction == 1)
        {
            ranges[sorted_fitness_indexes[i]] = i + 1;
        }
        else
        {
            ranges[sorted_fitness_indexes[i]] = abs(population_size - i);
        }
    }

    int summary_range = 0;
    for(i = 0; i < population_size; i++)
    {
        summary_range += ranges[i];
    }

    // algorytm ruletki

    // tablica przechowujaca szanse wyboru danego osobnika z populacji
    double *proportional_selection_values = (double*)malloc(sizeof(double) * population_size);
    for(i = 0; i < population_size; i++)
    {
        //proportional_selection_values[i] = (double)ranges[i] / (double)summary_range;
        proportional_selection_values[i] = ((2 - selection_pressure) / (population_size) + (2 * (double)ranges[i] * (selection_pressure - 1)) / (population_size * (population_size - 1)));
    }

    // tablica przechowujaca sumy wszystkich kolejnych proporcji - swoista dystrybuanta
    double *proportional_selection_sums = (double*)malloc(sizeof(double) * population_size);
    proportional_selection_sums[0] = proportional_selection_values[0];
    for(i = 1; i < population_size; i++)
    {
        proportional_selection_sums[i] = proportional_selection_sums[i - 1] + proportional_selection_values[i];
    }

    // tablica przechowujaca wybranych osobnikow
    int *selected_objects = (int*)malloc(sizeof(int) * population_size);
    for(i = 0; i < population_size; i++)
    {
        // gornym ograniczeniem generowanej wartosci jest najwieksza wartosc dystrybuanty, w tym wypadku 1
        double generated_value = generate_random_double_value(0, proportional_selection_sums[population_size - 1]);
        for(j = 0; j < population_size; j++)
        {
            if(generated_value < proportional_selection_sums[j])
            {
                selected_objects[i] = j;
                break;
            }
        }
    }

    // tworzenie populacji rodzicow
    for(i = 0; i < population_size; i++)
    {
        for(j = 0; j < chromosome_length; j++)
        {
            chromosomes[i][j] = temp_chromosomes[selected_objects[i]][j];
        }
        for(j = 0; j < space_dimension; j++)
        {
            chromosome_decimal_values[i][j] = temp_chromosome_decimal_values[selected_objects[i]][j];
        }
        temp_fitness[i] = fitness[selected_objects[i]];
    }

    free(proportional_selection_sums);
    free(proportional_selection_values);
    free(selected_objects);
    free(ranges);
}

/*
 Selekcja turniejowa.
 */
void tournament_selection(int population_size, int chromosome_length, int space_dimension, int function_direction, int tournament_size)
{
    int i, j;
    double best_object;
    int best_object_index;
    for(i = 0; i < population_size; i++)
    {
        generate_set_of_unique_values(population_size, random_values, random_indexes);

        best_object = fitness[random_indexes[0]];
        best_object_index = random_indexes[0];
        for(j = 1; j < tournament_size; j++)
        {
            if(function_direction == 0)
            {
                if(fitness[random_indexes[j]] < best_object)
                {
                    best_object = fitness[random_indexes[j]];
                    best_object_index = random_indexes[j];
                }
            }
            else
            {
                if(fitness[random_indexes[j]] > best_object)
                {
                    best_object = fitness[random_indexes[j]];
                    best_object_index = random_indexes[j];
                }
            }
        }
        for(j = 0; j < chromosome_length; j++)
        {
            chromosomes[i][j] = temp_chromosomes[best_object_index][j];
        }

        temp_fitness[i] = fitness[best_object_index];
    }
}

/*
 Selekcja proporcjonalna, metoda ruletki.
 */
void proportional_selection(int population_size, int chromosome_length, int space_dimension, int function_direction)
{
    int i, j;
    double sum_of_function_values = 0;
    // najgorszy osobnik - uzywane tylko w przypadku minimalizacji funkcji
    double worst_object = fitness[0];

    // tablica przechowujaca szanse wyboru danego osobnika z populacji
    double *proportional_selection_values = (double*)malloc(sizeof(double) * population_size);
    for(i = 0; i < population_size; i++)
    {
        sum_of_function_values += fitness[i];
        if(fitness[i] > worst_object) worst_object = fitness[i];
    }
    for(i = 0; i < population_size; i++)
    {
        if(function_direction == 0) proportional_selection_values[i] = (worst_object - fitness[i] + 1) / (sum_of_function_values + 1);
        else proportional_selection_values[i] = fitness[i] / sum_of_function_values;
    }

    // tablica przechowujaca sumy wszystkich kolejnych proporcji - swoista dystrybuanta
    double *proportional_selection_sums = (double*)malloc(sizeof(double) * population_size);
    proportional_selection_sums[0] = proportional_selection_values[0];
    for(i = 1; i < population_size; i++)
    {
        proportional_selection_sums[i] = proportional_selection_sums[i - 1] + proportional_selection_values[i];
    }

    // tablica przechowujaca wybranych osobnikow
    int *selected_objects = (int*)malloc(sizeof(int) * population_size);
    for(i = 0; i < population_size; i++)
    {
        // gornym ograniczeniem generowanej wartosci jest najwieksza wartosc dystrybuanty
        // dla maksymalizacji zawsze bedzie to 1, dla minimalizacji roznie.
        double generated_value = generate_random_double_value(0, proportional_selection_sums[population_size - 1]);
        for(j = 0; j < population_size; j++)
        {
            if(generated_value < proportional_selection_sums[j])
            {
                selected_objects[i] = j;
                break;
            }
        }
    }

    // tworzenie populacji rodzicow
    for(i = 0; i < population_size; i++)
    {
        for(j = 0; j < chromosome_length; j++)
        {
            chromosomes[i][j] = temp_chromosomes[selected_objects[i]][j];
        }
        for(j = 0; j < space_dimension; j++)
        {
            chromosome_decimal_values[i][j] = temp_chromosome_decimal_values[selected_objects[i]][j];
        }
        temp_fitness[i] = fitness[selected_objects[i]];
    }

    free(proportional_selection_sums);
    free(proportional_selection_values);
    free(selected_objects);
}

/*
 Selekcja.
 */
void selection(int population_size, int chromosome_length, int space_dimension, int function_direction, int selection_number, int tournament_size, double selection_pressure, int elite_size)
{
    // indeksy najlepszych osobnikow w populacji
    int *elite;
    int i, j;

    rewrite_chromosome_lists(population_size, chromosome_length, space_dimension);

    if(elite_size > 0)
    {
        elite = (int*)malloc(sizeof(int) * population_size);
        for(i = 0; i < population_size; i++)
        {
            sorted_fitness[i] = fitness[i];
            sorted_fitness_indexes[i] = i;
        }

        quickSort(sorted_fitness, sorted_fitness_indexes, 0, population_size - 1);
        for(i = 0; i < population_size; i++)
        {
            if(function_direction == 0)
            {
                elite[i] = sorted_fitness_indexes[i];
            }
            else
            {
                elite[i] = sorted_fitness_indexes[population_size - i - 1];
            }
        }

    }
    switch(selection_number)
    {
        case 0:
            proportional_selection(population_size, chromosome_length, space_dimension, function_direction);
            break;
        case 1:
            tournament_selection(population_size, chromosome_length, space_dimension, function_direction, tournament_size);
            break;
        case 2:
            range_selection(population_size, chromosome_length, space_dimension, function_direction, selection_pressure);
            break;
        default:
            break;
    }

    if(elite_size > 0)
    {

        for(i = 0; i < population_size; i++)
        {
            sorted_fitness[i] = temp_fitness[i];
            sorted_fitness_indexes[i] = i;
        }

        quickSort(sorted_fitness, sorted_fitness_indexes, 0, population_size - 1);

        // zastepowanie najgorszych osobnikow najlepszymi
        for(i = 0; i < elite_size; i++)
        {
            if(function_direction == 0)
            {
                for(j = 0; j < chromosome_length; j++)
                {
                    chromosomes[sorted_fitness_indexes[population_size - i - 1]][j] = temp_chromosomes[elite[i]][j];
                }
                for(j = 0; j < space_dimension; j++)
                {
                    chromosome_decimal_values[sorted_fitness_indexes[population_size - i - 1]][j] = temp_chromosome_decimal_values[elite[i]][j];
                }
            }
            else
            {
                for(j = 0; j < chromosome_length; j++)
                {
                    chromosomes[sorted_fitness_indexes[i]][j] = temp_chromosomes[elite[i]][j];
                }
                for(j = 0; j < space_dimension; j++)
                {
                    chromosome_decimal_values[sorted_fitness_indexes[i]][j] = temp_chromosome_decimal_values[elite[i]][j];
                }
            }
        }

        free(elite);
    }
}

