#ifndef MUTATION_H_INCLUDED
#define MUTATION_H_INCLUDED
void binary_mutation(int population_size, int chromosome_length, double mutation_probability);
void arithmetic_mutation(int population_size, int space_dimension, int current_iteration, int number_of_iterations, int left_margin, int domain_length, int c);
void mutation(int population_size, int chromosome_length, int type, int left_margin, int domain_length, double mutation_probability, int current_iteration, int number_of_iterations, int c, int space_dimension, int binary_number_length);



#endif // MUTATION_H_INCLUDED
