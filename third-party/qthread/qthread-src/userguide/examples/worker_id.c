#include <stdio.h>
#include <stdlib.h>
#include <qthread/qthread.h>

typedef struct Vector {
    int n;
    int *array;
} Vector;

static aligned_t sum(void *arg){
    int i;
    int *array = ((Vector*) arg)->array;
    int n = ((Vector*)arg)->n;
    int result = 0;
    qthread_worker_id_t id = qthread_worker(NULL);

    for(i=0; i<n; i++)
        result += array[id * n + i];

    return result;
}

int main(int argc, char *argv[]){
    srand(1);
    int elems_per_worker = 100;
    qthread_initialize();
    int n = qthread_num_workers();
    int* array = malloc(n * elems_per_worker * sizeof(int));
    aligned_t* return_value = malloc(n * elems_per_worker * sizeof(int)); 
    int i, results=0;

    printf("Number of workers: %d\n", n);
    printf("Number of array elements: %d\n", n * elems_per_worker);

    Vector input;
    input.n = n;
    input.array = array;

    for(i=0; i<n*elems_per_worker; i++)
        array[i] = rand(); 

    for(i=0; i<n; i++)
        qthread_fork(sum, &input, return_value + i);

    for(i=0; i<n; i++){
        qthread_readFF(NULL, return_value + i);
        results += return_value[i];
    }

    printf("Result: %d\n", results);

    return 0;
}
