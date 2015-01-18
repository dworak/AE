#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

//Contributors Joka, Dworakowski, Dziemianczuk

/*
 Tablica chromosomow. Przyjmuje wartosci 0 lub 1;
 Kazdy wiersz stanowi jeden chromosom.
 W chromosomie zawarte sa kolejne liczby reprezentujace rozwiazanie funkcji dopasowania.
 */
int **chromosomes;
/*
 Tymczasowa kopia tablicy chromosomow
 */
int **temp_chromosomes;

/*
 Tablica przechowujaca reprezentacje dziesietna liczb zawartych w chromosomach.
 */
double **chromosome_decimal_values;

/*
 Tymczasowa kopia dziesietnych reprezentacji chromosomow
 */
double **temp_chromosome_decimal_values;

/*
 Tablica dopasowan poszczegolnych chromosomow.
 Jej wartosci zmieniane sa w kazdej kolejnej iteracji.
 */
double *fitness;

/*
 Tymczasowa tablica dopasowan.
 */
double *temp_fitness;

/*
 Tablica unikalnych wartosci losowych.
 */
double *random_values;

/*
 Tablica indeksow unikalnych wartosci losowych.
 */
int *random_indexes;

/*
 Tablica dopasowan do posortowania
 */
double *sorted_fitness;

/*
 Tablica indeksow dopasowan do posortowania
 */
int *sorted_fitness_indexes;

/*
 Funkcja generujaca losowa liczbe zmiennoprzecinkowa z zakresu [min, max).
 */
double generate_random_double_value(double min, double max)
{
    double range = (max - min);
    double div = RAND_MAX / range;
    return min + (rand() / div);
}

/*
 Funkcja generujaca losowa liczbe calkowita z zakresu [min, max].
 */
int generate_random_integer_value(int min, int max)
{
    return (rand() % (max + 1 - min)) + min;
}

/*
 Funkcja generujaca liczbe calkowita z zakresu [0,1].
 */
int generate_random_binary_value()
{
    return rand() % 2;
}

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

/*
 Funkcja generujaca ciag unikalnych liczb calkowitych z przedzialu [0, rozmiar_populacji).
 */
void generate_set_of_unique_values(int size, double *values, int *indexes)
{
    int i, j;
    double temp;
    int temp_int;
    
    for(i = 0; i < size; i++)
    {
        values[i] = generate_random_double_value(0, 1);
        indexes[i] = i;
    }
    
    quickSort(values, indexes, 0, size - 1);
    
    // sortowanie babelkowe, w razie czego
    /*for (i = (size - 1); i > 0; i--)
     {
     for (j = 1; j <= i; j++)
     {
     if (values[j-1] > values[j])
     {
     temp = values[j-1];
     values[j-1] = values[j];
     values[j] = temp;
     temp_int = indexes[j-1];
     indexes[j-1] = indexes[j];
     indexes[j] = temp_int;
     }
     }
     }*/
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

/*
 Funkcja Aksleya.
 Zakres zmiennych: [-1, 1].
 Osiaga minimum globalne dla wartosci 0.
 */
void aksley_function(int population_size, int chromosome_length, int space_dimension)
{
    int i, j;
    for(i = 0; i < population_size; i++)
    {
        double first_part_result = 0;
        double second_part_result = 0;
        for(j = 0; j < space_dimension; j++)
        {
            first_part_result += pow(chromosome_decimal_values[i][j], 2);
            second_part_result += cos(2 * M_PI * chromosome_decimal_values[i][j]);
        }
        fitness[i] = -20 * pow(M_E, (-0.2 * sqrt(first_part_result / space_dimension))) - pow(M_E, (second_part_result / space_dimension)) + 20 + M_E;
    }
}

/*
 Funkcja 1 Bohachevsky'ego.
 Osiaga minimum globalne wynoszace 0 dla wartosci zmiennych wynoszacych 0.
 */
void bohachevsky_1_function(int population_size, int chromosome_length, int space_dimension)
{
    int i;
    for(i = 0; i < population_size; i++)
    {
        fitness[i] = pow(chromosome_decimal_values[i][0], 2) + 2 * pow(chromosome_decimal_values[i][1], 2) - 0.3 * cos(3 * M_PI * chromosome_decimal_values[i][0]) - 0.4 * cos(4 * M_PI * chromosome_decimal_values[i][1]) + 0.7;
    }
}

/*
 Funkcja 2 Bohachevsky'ego.
 Osiaga minimum globalne wynoszace 0 dla wartosci zmiennych wynoszacych 0.
 */
void bohachevsky_2_function(int population_size, int chromosome_length, int space_dimension)
{
    int i;
    for(i = 0; i < population_size; i++)
    {
        fitness[i] = pow(chromosome_decimal_values[i][0], 2) + 2 * pow(chromosome_decimal_values[i][1], 2) - 0.3 * (cos(3 * M_PI * chromosome_decimal_values[i][0]) * cos(4 * M_PI * chromosome_decimal_values[i][1])) + 0.3;
    }
}

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
 Funkcja Michalewicza.
 Zakres zmiennych: 0 <= xi <= pi.
 Minima globalne zaleza od ilosci wymiarow. Przykladowo:
 - 2 wymiary: -1.8013
 - 5 wymiarow: -4.687658
 - 10 wymiarow: -9.66015
 */
void michalewicz_m12_function(int population_size, int chromosome_length, int space_dimension)
{
    int i, j;
    for(i = 0; i < population_size; i++)
    {
        double partial_result = 0;
        for(j = 0; j < space_dimension; j++)
        {
            partial_result += sin(chromosome_decimal_values[i][j]) * pow(sin((i + 1) * pow(chromosome_decimal_values[i][j], 2) / M_PI), (2 * 10));
        }
        fitness[i] = - partial_result;
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
 Funkcja Schaffera F7.
 Osiaga minimum globalne wynoszace 0 dla wartosci zmiennych wynoszacych 0.
 */
void schaffer_f7_function(int population_size, int chromosome_length, int space_dimension)
{
    int i, j;
    for(i = 0; i < population_size; i++)
    {
        double partial_result = 0;
        for(j = 0; j < space_dimension; j++)
        {
            partial_result += pow(chromosome_decimal_values[i][j], 2);
        }
        fitness[i] = pow(partial_result, 0.25) * (pow(sin(50 * pow(partial_result, 0.1)), 2) + 1);
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

/*
 Obliczanie funkcji dopasowania.
 */
void calculate_fitness(int population_size, int binary_number_length, int chromosome_length, int space_dimension, int function_number)
{
    switch(function_number)
    {
        case 0:
            aksley_function(population_size, chromosome_length, space_dimension);
            break;
        case 1:
            bohachevsky_1_function(population_size, chromosome_length, space_dimension);
            break;
        case 2:
            bohachevsky_2_function(population_size, chromosome_length, space_dimension);
            break;
        case 3:
            rastrigin_function(population_size, chromosome_length, space_dimension);
            break;
        case 4:
            easom_function(population_size, chromosome_length, space_dimension);
            break;
        case 5:
            michalewicz_m12_function(population_size, chromosome_length, space_dimension);
            break;
        case 6:
            schaffer_f6_function(population_size, chromosome_length, space_dimension);
            break;
        case 7:
            schaffer_f7_function(population_size, chromosome_length, space_dimension);
            break;
        case 8:
            de_jong_1_function(population_size, chromosome_length, space_dimension);
            break;
        case 9:
            rosenbrock_valley(population_size, chromosome_length, space_dimension);
            break;
        case 10:
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
 Krzyzowanie diagonalne.
 */
void diagonal_crossover(int population_size, int chromosome_length)
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
    
    for(i = 0; i < population_size; i++)
    {
        for(k = 0; k < number_of_cross_points; k++)
        {
            if(k == number_of_cross_points - 1)
            {
                for(j = cross_points[k] + 1; j <= chromosome_length; j++)
                {
                    chromosomes[i][j] = temp_chromosomes[((i + k + 1) % population_size)][j];
                }
            }
            else
            {
                for(j = cross_points[k] + 1; j <= cross_points[k + 1]; j++)
                {
                    chromosomes[i][j] = temp_chromosomes[((i + k + 1) % population_size)][j];
                }
            }
            
        }
    }
    
    free(potential_cross_points);
    free(cross_points);
    free(sort_list);
}

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
    int i;
    
    
    
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
            diagonal_crossover(population_size, chromosome_length);
            calculate_decimal_chromosome_values(population_size, binary_number_length, chromosome_length, space_dimension, left_margin, domain_length);
            break;
        case 4:
            whole_arithmetic_crossover(population_size, space_dimension);
            calculate_binary_chromosome_values(population_size, binary_number_length, chromosome_length, space_dimension, left_margin, domain_length);
            break;
        case 5:
            single_arithmetic_crossover(population_size, space_dimension);
            calculate_binary_chromosome_values(population_size, binary_number_length, chromosome_length, space_dimension, left_margin, domain_length);
            break;
        case 6:
            simple_arithmetic_crossover(population_size, space_dimension);
            calculate_binary_chromosome_values(population_size, binary_number_length, chromosome_length, space_dimension, left_margin, domain_length);
            break;
        default:
            break;
    }
}

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

/*
 Parametry (nalezy wprowadzac w kolejnosci po spacji):
 1. ilosc zmiennych - wymiar przestrzeni minus jeden (liczba calkowita nieujemna)
 2. lewa granica przestrzeni (liczba calkowita),
 3. prawa granica przestrzeni (liczba calkowita),
 4. precyzja - ilosc miejsc po przecinku (liczba calkowita nieujemna),
 5. prawdopodobienstwo mutacji (liczba rzeczywista),
 6. wielkosc populacji (liczba calkowita dodatnia),
 7. w zaleznosci od wartosci parametru 14 (liczba calkowita dodatnia):
 + 0: ilosc iteracji
 + 1: ilosc obliczonych dopasowan
 + 2: nieuzywany
 + 3: ilosc iteracji, przez ktora nie ulega zmianie najlepszy osobnik
 8. funkcja (odpowiednio liczby):
 + 0: Funkcja Ackleya
 + 1: Funkcja 1 Bohachevsky'ego
 + 2: Funkcja 2 Bohachevsky'ego
 + 3: Funkcja Rastrigina
 + 4: Funkcja Easoma
 + 5: Funkcja Michalewicza F12
 + 6: Funkcja Schaffera F6
 + 7: Funkcja Schaffera F7
 + 8: Funkcja De Jonga 1
 + 9: Dolina Rosenbrocka
 + 10: Funkcja Griewanka
 9. selekcja (odpowiednio liczby)
 + 0: selekcja proporcjonalna (ruletka)
 + 1: selekcja turniejowa
 + 2: selekcja oparta na rangach (rangowa)
 10. operator krzyzowania (odpowiednio liczby):
 + 0: krzyzowanie jednopunktowe
 + 1: krzyzowanie wielopunktowe
 + 2: krzyzowanie jednostajne
 + 3: krzyzowanie diagonalne
 + 4: whole arithmetic crossover
 + 5: single arithmetic crossover
 + 6: simple arighmetic crossover
 11. rozmiar turnieju dla selekcji turniejowej (liczba calkowita)
 12. presja selekcyjna dla selekcji rangowej
 13. wartosc wielkosci elity (0 - sukcesja trywialna, > 0 - sukcesja elitarna)
 14. warunek stopu (odpowiednio liczby):
 + 0: zadana liczba iteracji
 + 1: zadana liczba obliczonych dopasowan (zalozenie: dana populacja i tak zostanie policzona do konca)
 + 2: osiagniecie zadanej jako kolejny parametr wartosci funkcji
 + 3: brak zmiany najlepszego osobnika przez zadana liczbe populacji
 15. wartosc funkcji, po ktorej osiagnieciu algorytm zatrzymuje sie (liczba zmiennoprzecinkowa)
 16. rodzaj mutacji (liczba nieparzysta - binarna, liczba parzysta - arytmetyczna)
 17. wartość c dla mutacji dla mutacji arytmetycznej (należy i tak obowiązkowo podać)
 */
int main(int argc, char* argv[])
{
    int space_dimension = 1000;
    int left_margin = 0;
    int right_margin = 500;
    int precision = 0.2;
    double mutation_probability = 0.1;
    int population_size = 10000;
    int number_of_iterations = 1000;
    int function_number = 5;
    int selection_number = 2;
    int crossover_number = 1;
    int tournament_size = 100;
    double selection_pressure = 2;
    int elite_size = 10;
    int stop_condition = 2;
    double fitness_to_achieve = 8;
    int mutation_type = 2;
    int mutation_c = 2;
    
    // sprawdzenie poprawnosci podanych parametrow
    if(space_dimension < 1)
    {
        printf("Wymiar przestrzeni musi wynosic przynajmniej 1.\n");
        return -1;
    }
    
    if(right_margin <= left_margin)
    {
        printf("Prawe ograniczenie powinno byc wieksze od lewego.\n");
        return -1;
    }
    
    if(precision < 0)
    {
        printf("Precyzja nie moze byc mniejsza od 0.\n");
        return -1;
    }
    
    if(mutation_probability < 0 || mutation_probability > 1)
    {
        printf("Prawdopodobienstwo mutacji musi byc liczba z zakresu [0,1].\n");
        return -1;
    }
    
    if(population_size < 2)
    {
        printf("Rozmiar populacji nie moze byc mniejszy od 2.\n");
        return -1;
    }
    
    if(population_size % 2 != 0)
    {
        printf("Rozmiar populacji musi byc liczba parzysta.\n");
        return -1;
    }
    
    if(number_of_iterations < 1)
    {
        printf("Liczba iteracji nie moze byc mniejsza od 1.\n");
        return -1;
    }
    
    if(function_number < 0 || function_number > 10)
    {
        printf("Numer funkcji musi byc liczba calkowita z przedzialu [0,10].\n");
        return -1;
    }
    
    if(selection_number < 0 || selection_number > 2)
    {
        printf("Numer selekcji musi byc liczba calkowita z przedzialu [0,2].\n");
        return -1;
    }
    
    if(crossover_number < 0 || crossover_number > 6)
    {
        printf("Numer operatora krzyzowania musi byc liczba calkowita z przedzialu [0,36.\n");
        return -1;
    }
    
    if(tournament_size < 2 || tournament_size > population_size)
    {
        printf("Rozmiar turnieju musi byc liczba z zakresu [2,rozmiar_populacji].\n");
        return -1;
    }
    
    if(selection_pressure <= 1 || selection_pressure > 2)
    {
        printf("Napor selekcyjny musi byc liczba z zakresu (1,2].\n");
        return -1;
    }
    
    if(elite_size < 0 || elite_size >= population_size)
    {
        printf("Wielkosc elity musi byc liczba z zakresu [0, rozmiar_populacji - 1].\n");
        return -1;
    }
    
    if((function_number == 1 || function_number == 2 || function_number == 4) && space_dimension != 2)
    {
        printf("Dla wybranej funkcji wymiar przestrzeni musi wynosic 2. Nastepuje zmiana podanego wymiaru przestrzeni na odpowiedni.\n");
        space_dimension = 2;
    }
    
    if(stop_condition < 0 || stop_condition > 3)
    {
        printf("Warunek zatrzymania musi byc liczba calkowita z przedzialu [0,3].\n");
        return -1;
    }
    
    if(stop_condition == 1 && number_of_iterations < population_size)
    {
        printf("Przy zadanym warunku zatrzymania liczba iteracji nie moze byc mniejsza niz rozmiar populacji.\n");
        return -1;
    }
    
    if(fitness_to_achieve < 0)
    {
        printf("Docelowa wartosc funkcji celu nie moze byc mniejsza od 0.\n");
        return -1;
    }
    
    if(mutation_c < 0)
    {
        printf("Wartosc c powinna byc dodatnia");
        return -1;
    }
    // aby random generowal wartosci losowe
    srand(time(NULL));
    
    // iteratory
    int i, j, k, l;
    
    // kierunek funkcji (0 dla minimalizacji, 1 dla maksymalizacji)
    int function_direction;
    if(function_number <= 10)
    {
        function_direction = 0;
    }
    else
    {
        function_direction = 1;
    }
    
    // ilosc czesci calkowitych liczby
    int domain_length = abs(right_margin - left_margin);
    
    // ilosc mozliwych do wylosowania liczb (czesc calkowita + czesc po przecinku);
    // bardziej matematycznie - ilosc podprzestrzeni, na ktore przestrzen (jednowymiarowa!) zosatnie podzielona
    int number_of_ranges = domain_length * pow(10, precision);
    
    // ilosc bitow potrzebna do reprezentacji jednej liczby
    int binary_number_length;
    for(binary_number_length = 1;; binary_number_length++)
    {
        if(pow(2, binary_number_length) >= number_of_ranges) break;
    }
    
    // dlugosc chromosomu = dlugosc liczby * wymiar przestrzeni.
    // jeden chromosom zawiera liczby nastepujace kolejno po sobie.
    int chromosome_length = binary_number_length * space_dimension;
    
    initialize_structures(population_size, chromosome_length, space_dimension);
    
    populate_chromosomes(population_size, chromosome_length);
    
    calculate_decimal_chromosome_values(population_size, binary_number_length, chromosome_length, space_dimension, left_margin, domain_length);
    
    // wlasciwa czesc algorytmu genetycznego
    
    int number_of_iterations_without_changes = 0;
    int number_of_calculated_iterations = 0;
    double best_object;
    int best_object_index;
    
    switch(stop_condition)
    {
        case 0:
            for(i = 0; i < number_of_iterations; i++)
            {
                calculate_fitness(population_size, binary_number_length, chromosome_length, space_dimension, function_number);
                /*for(j = 0; j < population_size; j++)
                 {
                 printf("%2f ", fitness[j]);
                 }
                 printf("\n");*/
                selection(population_size, chromosome_length, space_dimension, function_direction, selection_number, tournament_size, selection_pressure, elite_size);
                crossover(population_size, chromosome_length, space_dimension, crossover_number, left_margin, domain_length, binary_number_length);
                mutation(population_size, chromosome_length, mutation_type, left_margin, domain_length, mutation_probability, i, number_of_iterations, mutation_c, space_dimension, binary_number_length);
            }
            break;
        case 1:
            for(i = 0; i < ceil((double)number_of_iterations / (double)population_size); i++)
            {
                calculate_fitness(population_size, binary_number_length, chromosome_length, space_dimension, function_number);
                /*for(j = 0; j < population_size; j++)
                 {
                 printf("%2f ", fitness[j]);
                 }
                 printf("\n");*/
                selection(population_size, chromosome_length, space_dimension, function_direction, selection_number, tournament_size, selection_pressure, elite_size);
                crossover(population_size, chromosome_length, space_dimension, crossover_number, left_margin, domain_length, binary_number_length);
                mutation(population_size, chromosome_length, mutation_type, left_margin, domain_length, mutation_probability, i, ceil((double)number_of_iterations / (double)population_size), mutation_c, space_dimension, binary_number_length);
            }
            break;
        case 2:
            while(1)
            {
                number_of_calculated_iterations++;
                int result_achieved = 0;
                calculate_fitness(population_size, binary_number_length, chromosome_length, space_dimension, function_number);
                for(j = 0; j < population_size; j++)
                {
                    // printf("%2f ", fitness[j]);
                    if(function_direction == 0)
                    {
                        if(fitness[j] <= fitness_to_achieve)
                        {
                            result_achieved = 1;
                        }
                    }
                    else
                    {
                        if(fitness[j] >= fitness_to_achieve)
                        {
                            result_achieved = 1;
                        }
                    }
                }
                //printf("\n");
                if(result_achieved == 1) break;
                selection(population_size, chromosome_length, space_dimension, function_direction, selection_number, tournament_size, selection_pressure, elite_size);
                crossover(population_size, chromosome_length, space_dimension, crossover_number, left_margin, domain_length, binary_number_length);
                mutation(population_size, chromosome_length, mutation_type, left_margin, domain_length, mutation_probability, number_of_calculated_iterations, number_of_iterations, mutation_c, space_dimension, binary_number_length);
            }
            break;
        case 3:
            number_of_calculated_iterations = 0;
            if(function_direction == 0)
            {
                best_object = 999999;
            }
            else
            {
                best_object = -999999;
            }
            
            while(1)
            {
                number_of_calculated_iterations++;
                calculate_fitness(population_size, binary_number_length, chromosome_length, space_dimension, function_number);
                double best_object_this_iteration;
                best_object_this_iteration = fitness[0];
                for(j = 0; j < population_size; j++)
                {
                    //printf("%2f ", fitness[j]);
                    if(function_direction == 0)
                    {
                        if(fitness[j] < best_object)
                        {
                            best_object_this_iteration = fitness[j];
                        }
                    }
                    else
                    {
                        if(fitness[j] > best_object)
                        {
                            best_object_this_iteration = fitness[j];
                        }
                    }
                }
                if(function_direction == 0)
                {
                    if(best_object_this_iteration < best_object)
                    {
                        best_object = best_object_this_iteration;
                        number_of_iterations_without_changes = 0;
                    }
                    else
                    {
                        number_of_iterations_without_changes++;
                    }
                }
                else
                {
                    if(best_object_this_iteration > best_object)
                    {
                        best_object = best_object_this_iteration;
                        number_of_iterations_without_changes = 0;
                    }
                    else
                    {
                        number_of_iterations_without_changes++;
                    }
                }
                //printf("%2f\n", best_object);
                //printf("\n");
                if(number_of_iterations_without_changes >= number_of_iterations) break;
                selection(population_size, chromosome_length, space_dimension, function_direction, selection_number, tournament_size, selection_pressure, elite_size);
                crossover(population_size, chromosome_length, space_dimension, crossover_number, left_margin, domain_length, binary_number_length);
                mutation(population_size, chromosome_length, mutation_type, left_margin, domain_length, mutation_probability, number_of_calculated_iterations, (number_of_iterations > number_of_calculated_iterations ? number_of_iterations : number_of_calculated_iterations), mutation_c, space_dimension, binary_number_length);
            }
            break;
    }
    
    
    // obliczenie ostatecznego dopasowania funkcji
    calculate_fitness(population_size, binary_number_length, chromosome_length, space_dimension, function_number);
    /*printf("Dopasowanie populacji finalnej:\n");
     for(i = 0; i < population_size; i++)
     {
     printf("%2f ", fitness[i]);
     }
     printf("\n");*/
    
    // odczytanie najlepszego wyniku
    best_object = fitness[0];
    best_object_index = 0;
    for(j = 0; j < population_size; j++)
    {
        //printf("%2f ", fitness[j]);
        if(function_direction == 0)
        {
            if(fitness[j] < best_object)
            {
                best_object = fitness[j];
            }
        }
        else
        {
            if(fitness[j] > best_object)
            {
                best_object = fitness[j];
            }
        }
    }
    printf("%2f\t%d\n", best_object, number_of_calculated_iterations - number_of_iterations);
    printf("%d\n", number_of_calculated_iterations);
    printf("Najlepszy obiekt:\n");
    printf("%2f\n\n", best_object);
    printf("Wartosci zmiennych dla najlepszego obiektu:\n");
    for(i = 0; i < space_dimension; i++)
    {
        printf("%2f\n", chromosome_decimal_values[best_object_index][i]);
    }
    
    // zwalnianie pamieci
    for(i = 0; i < population_size; i++)
    {
        free(chromosomes[i]);
    }
    free(chromosomes);
    for(i = 0; i < population_size; i++)
    {
        free(chromosome_decimal_values[i]);
    }
    
    free(chromosome_decimal_values);
    for(i = 0; i < population_size; i++)
    {
        free(temp_chromosomes[i]);
    }
    free(temp_chromosomes);
    for(i = 0; i < population_size; i++)
    {
        free(temp_chromosome_decimal_values[i]);
    }
    free(temp_chromosome_decimal_values);
    free(fitness);
    free(temp_fitness);
    free(random_values);
    free(random_indexes);
    free(sorted_fitness);
    free(sorted_fitness_indexes);
    return 0;
}

