#ifndef FACT_H
#define FACT_H

int fact(int x);

typedef struct _data {
    int  x;
} data;

data* getNewData(void);
data* fact_d(data* d);
data* getDataStructPtr(void);

#endif /* FACT_H */
