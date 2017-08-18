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
