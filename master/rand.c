#include <stdlib.h>
#include "rand.h"

// http://www.azillionmonkeys.com/qed/random.html

#define RAND_INV_RANGE(r) (((long) RAND_MAX + 1) / (r))

int random_below(int RANGE)
{
    int x;
    do {
        x = rand();
    }
    while (x >= RANGE * RAND_INV_RANGE (RANGE));
    x /= RAND_INV_RANGE (RANGE);
    return x;
}

int seed = 1;
int really_random_below(int RANGE)
{
    srand(seed);
    int x;
    do {
        x = rand();
    }
    while (x >= RANGE * RAND_INV_RANGE (RANGE));
    seed = x;
    x /= RAND_INV_RANGE (RANGE);
    return x;
}
