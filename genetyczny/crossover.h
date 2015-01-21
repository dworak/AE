#ifndef CROSSOVER_H_INCLUDED
#define CROSSOVER_H_INCLUDED

void one_point_crossover(int population_size, int chromosome_length);
void multiple_points_crossover(int population_size, int chromosome_length);
void uniform_crossover(int population_size, int chromosome_length);
void whole_arithmetic_crossover(int population_size, int chromosome_length);
void single_arithmetic_crossover(int population_size, int chromosome_length);
void simple_arithmetic_crossover(int population_size, int chromosome_length);
void crossover(int population_size, int chromosome_length, int space_dimension, int crossover_number, int left_margin, int domain_length, int binary_number_length);



#endif // CROSSOVER_H_INCLUDED
