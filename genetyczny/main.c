#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "core.h"


/*
 Tablica chromosomow. Przyjmuje wartosci 0 lub 1;
 Kazdy wiersz stanowi jeden chromosom.
 W chromosomie zawarte sa kolejne liczby reprezentujace rozwiazanie funkcji dopasowania.
 */
int **chromosomes;

//Tymczasowa kopia tablicy chromosomow
int **temp_chromosomes;

//Tablica przechowujaca reprezentacje dziesietna liczb zawartych w chromosomach.
double **chromosome_decimal_values;

//Tymczasowa kopia dziesietnych reprezentacji chromosomow
double **temp_chromosome_decimal_values;

/*
 Tablica dopasowan poszczegolnych chromosomow.
 Jej wartosci zmieniane sa w kazdej kolejnej iteracji.
 */
double *fitness;

//Tymczasowa tablica dopasowan.
double *temp_fitness;

//Tablica unikalnych wartosci losowych.
double *random_values;

//Tablica indeksow unikalnych wartosci losowych.
int *random_indexes;

//Tablica dopasowan do posortowania
double *sorted_fitness;

//Tablica indeksow dopasowan do posortowania
int *sorted_fitness_indexes;

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
 + 0: Funkcja Rastrigina
 + 1: Funkcja Easoma
 + 2: Funkcja Schaffera F6
 + 3: Funkcja De Jonga 1
 + 4: Dolina Rosenbrocka
 + 5: Funkcja Griewanka
 9. selekcja (odpowiednio liczby)
 + 0: selekcja proporcjonalna (ruletka)
 + 1: selekcja turniejowa
 + 2: selekcja oparta na rangach (rangowa)
 10. operator krzyzowania (odpowiednio liczby):
 + 0: krzyzowanie jednopunktowe
 + 1: krzyzowanie wielopunktowe
 + 2: krzyzowanie jednostajne
 + 3: whole arithmetic crossover
 + 4: single arithmetic crossover
 + 5: simple arighmetic crossover
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
 17. wartoœæ c dla mutacji dla mutacji arytmetycznej (nale¿y i tak obowi¹zkowo podaæ)
 */
int main(int argc, char* argv[])
{
    int space_dimension = 15;
    int left_margin = -5;
    int right_margin = 5;
    int precision = 6;
    double mutation_probability = 0.001;
    int population_size = 600;
    int number_of_iterations = 10000;
    /*
    + 0: Funkcja Rastrigina
 + 1: Funkcja Easoma
 + 2: Funkcja Schaffera F6
 + 3: Funkcja De Jonga 1
 + 4: Dolina Rosenbrocka
 + 5: Funkcja Griewanka
 */
    int function_number = 5;
    int selection_number = 2;
    int crossover_number = 1;
    int tournament_size = 4;
    double selection_pressure = 1.2;
    int elite_size = 0;
    int stop_condition = 0;
    double fitness_to_achieve = 2;
    int mutation_type = 2;
    int mutation_c = 2;

    // sprawdzenie poprawnosci podanych parametrow
    {


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


    }
    // aby random generowal wartosci losowe
    srand(time(NULL));

    // iteratory
    int i, j;

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
                selection(population_size, chromosome_length, space_dimension, function_direction, selection_number, tournament_size, selection_pressure, elite_size);
                crossover(population_size, chromosome_length, space_dimension, crossover_number, left_margin, domain_length, binary_number_length);
                mutation(population_size, chromosome_length, mutation_type, left_margin, domain_length, mutation_probability, i, number_of_iterations, mutation_c, space_dimension, binary_number_length);
            }
            break;
        case 1:
            for(i = 0; i < ceil((double)number_of_iterations / (double)population_size); i++)
            {
                calculate_fitness(population_size, binary_number_length, chromosome_length, space_dimension, function_number);

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
                if(number_of_iterations_without_changes >= number_of_iterations) break;
                selection(population_size, chromosome_length, space_dimension, function_direction, selection_number, tournament_size, selection_pressure, elite_size);
                crossover(population_size, chromosome_length, space_dimension, crossover_number, left_margin, domain_length, binary_number_length);
                mutation(population_size, chromosome_length, mutation_type, left_margin, domain_length, mutation_probability, number_of_calculated_iterations, (number_of_iterations > number_of_calculated_iterations ? number_of_iterations : number_of_calculated_iterations), mutation_c, space_dimension, binary_number_length);
            }
            break;
    }


    // obliczenie ostatecznego dopasowania funkcji
    calculate_fitness(population_size, binary_number_length, chromosome_length, space_dimension, function_number);

    // odczytanie najlepszego wyniku
    best_object = fitness[0];
    best_object_index = 0;
    for(j = 0; j < population_size; j++)
    {
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

