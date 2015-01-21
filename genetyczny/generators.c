#include <stdlib.h>
#include <time.h>
#include <math.h>

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
