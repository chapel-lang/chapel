
#include <stdio.h>
#include <stdlib.h>
#include "fact.h"



int fact(int x){
    if(x<=1)
        return 1;
    return x*fact(x-1);
}



data* getNewData(){
    data *d = malloc(sizeof(data));
    d->x = 5;
    return d;
}

data* fact_d(data *d){
    data *s = malloc(sizeof(data));
    
    if(d->x <= 1){
        s->x = 1;
        return s;
    }
    s->x = d->x-1;
    data *f = fact_d(s);
    f->x *= d->x;
    return f;
}

data* getDataStructPtr(){
    data *d = malloc(sizeof(data));
    d->x = 10;
    return d;
}
