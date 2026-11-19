#ifndef FACT_H
#define FACT_H

int fact(int x);

typedef struct factData {
    int  x;
} factData;

factData* getNewData(void);
factData* fact_d(factData* d);
factData* getDataStructPtr(void);

void cFree(void* ptr);

#endif /* FACT_H */
