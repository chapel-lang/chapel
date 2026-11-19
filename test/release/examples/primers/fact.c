
#include <stdio.h>
#include <stdlib.h>
#include "fact.h"



int fact(int x){
    if(x<=1)
        return 1;
    return x*fact(x-1);
}



factData* getNewData(void){
    factData *d = (factData*)malloc(sizeof(factData));
    d->x = 5;
    return d;
}

factData* fact_d(factData *d){
    factData *s = (factData*)malloc(sizeof(factData));

    if(d->x <= 1){
        s->x = 1;
        return s;
    }
    s->x = d->x-1;
    factData *f = fact_d(s);
    f->x *= d->x;
    free(s);
    return f;
}

factData* getDataStructPtr(void){
    factData *d = (factData*)malloc(sizeof(factData));
    d->x = 10;
    return d;
}

void cFree(void* ptr){
    free(ptr);
}
