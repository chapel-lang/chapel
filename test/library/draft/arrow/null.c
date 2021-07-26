#include<null.h>

int isNull(void *ptr){
    return ptr == NULL ? 1 :0;
}

void* getNULL(){
    return NULL;
}
