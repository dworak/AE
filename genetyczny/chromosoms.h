#ifndef CHROMOSOMS_H_INCLUDED
#define CHROMOSOMS_H_INCLUDED
void calculate_fitness(int population_size, int binary_number_length, int chromosome_length, int space_dimension, int function_number);
void populate_chromosomes(int population_size, int chromosome_length);
void calculate_decimal_chromosome_values(int population_size, int binary_number_length, int chromosome_length, int space_dimension, int left_margin, int domain_length);
void calculate_binary_chromosome_values(int population_size, int binary_number_length, int chromosome_length, int space_dimension, int left_margin, int domain_length);




#endif // CHROMOSOMS_H_INCLUDED
