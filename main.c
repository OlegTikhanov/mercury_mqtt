#include <stdio.h>
#include "mercury.h"
#include <unistd.h>

int main(void)
{
    int addres = 000000;
    mercury_counters counter;
    if (getCounters(addres, &counter) == 0)
    {
        printf("%.2fkW\n", counter.cnt1 / 1000.);
        printf("%.2fkW\n", counter.cnt2 / 1000.);
    }
    sleep(1);
    int power = getPower(addres);
    if (power != -1)
    {
        printf("%.2fkW\n", power/1000.);
    }
}