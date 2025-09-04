#include <stdio.h>
#include <stdlib.h>
#include "bug.h"

// Function that calls the create callback
int testCreateCallback(MyCreateCallback callback) {
    printf("C: Calling create callback...\n");

    MyHandle handle = NULL;

    // Call the callback (implemented in Chapel)
    int result = callback(&handle);

    if (handle != NULL) {
        printf("C: Handle created\n");
    }

    return result;
}

// Function that calls the modify callback
int testModifyCallback(MyModifyCallback callback, MyHandle handle) {
    printf("C: Calling modify callback...\n");

    if (handle != NULL) {
        printf("C: Before modify\n");
    }

    // Call the callback (implemented in Chapel)
    int result = callback(&handle);

    if (handle != NULL) {
        printf("C: After modify\n");
    }

    return result;
}
