#ifndef INIT_H_INCLUDED
#define INIT_H_INCLUDED

void generate_set_of_unique_values(int size, double *values, int *indexes);
void initialize_structures(int population_size, int chromosome_length, int space_dimension);
void rewrite_chromosome_lists(int population_size, int chromosome_length, int space_dimension);

#endif // INIT_H_INCLUDED
