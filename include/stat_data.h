#ifndef STAT_DATA
#define STAT_DATA

typedef struct StatData { 
    long  id; 
    int   count; 
    float cost; 
    unsigned int primary:1; 
    unsigned int mode:3; 
} StatData; 

#endif // STAT_DATA