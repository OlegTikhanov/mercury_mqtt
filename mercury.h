#ifndef MERCURY
#define MERCURY

#define DEBUG 1 // 0-2
#define OPEN_PORT_TIMEOUT 2000 //ms
#define READ_TIMEOUT 1000 //ms
#include <stdint.h>

typedef struct 
{
    uint64_t cnt1;
    uint64_t cnt2;
    uint64_t cnt3;
    uint64_t cnt4;
} mercury_counters;

int getCounters(uint32_t address, mercury_counters *counters);
int getPower(uint32_t address);
#endif