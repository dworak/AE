#ifndef SELECTION_H_INCLUDED
#define SELECTION_H_INCLUDED
void range_selection(int population_size, int chromosome_length, int space_dimension, int function_direction, double selection_pressure);
void tournament_selection(int population_size, int chromosome_length, int space_dimension, int function_direction, int tournament_size);
void proportional_selection(int population_size, int chromosome_length, int space_dimension, int function_direction);
void selection(int population_size, int chromosome_length, int space_dimension, int function_direction, int selection_number, int tournament_size, double selection_pressure, int elite_size);
#endif // SELECTION_H_INCLUDED
