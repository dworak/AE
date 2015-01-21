#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

void rastrigin_function(int population_size, int chromosome_length, int space_dimension);
void easom_function(int population_size, int chromosome_length, int space_dimension);
void schaffer_f6_function(int population_size, int chromosome_length, int space_dimension);
void de_jong_1_function(int population_size, int chromosome_length, int space_dimension);
void rosenbrock_valley(int population_size, int chromosome_length, int space_dimension);
void griewank_function(int population_size, int chromosome_length, int space_dimension);

#endif // FUNCTIONS_H_INCLUDED
