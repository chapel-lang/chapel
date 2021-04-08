#ifndef _QT_THREADQUEUE_STACK_H_
#define _QT_THREADQUEUE_STACK_H_

#include "qt_asserts.h"

/***
 *
 * The element at storage[base] is always NULL.
 *
 * The element at storage[top] is the topmost
 * element in the stack. 
 *
 ***/
struct qt_stack_s {
    qthread_t** storage;
    unsigned int base;
    unsigned int top;
    unsigned int capacity;
    unsigned int empty;
};

typedef struct qt_stack_s qt_stack_t;

static void qt_stack_create(qt_stack_t *stack, 
                                   unsigned int capacity)
{
    stack->storage  = calloc(capacity, sizeof(qthread_t*));
    stack->base     = 0;
    stack->top      = 0;
    stack->capacity = capacity;
    stack->empty    = 1;
}

static void qt_stack_free(qt_stack_t *stack)
{
    free(stack->storage);
    stack->base = stack->top = stack->capacity = 0;
}

static QINLINE int qt_stack_is_empty(qt_stack_t *stack)
{
    return(stack->empty);
}

static QINLINE int qt_stack_is_full(qt_stack_t *stack)
{
    return(stack->base == ((stack->top + 1) % stack->capacity));
}

static QINLINE int qt_stack_size(qt_stack_t *stack)
{
    if (stack->top >= stack->base) {
        return(stack->top - stack->base);
    } else {
        return(stack->capacity - stack->base + stack->top);
    }
}

static void qt_stack_resize(qt_stack_t *stack)
{
    qthread_t **old_storage  = stack->storage;
    int         old_base     = stack->base;
    int         old_capacity = stack->capacity;
    qthread_t **storage      = calloc(2 * old_capacity, sizeof(qthread_t*));

    memcpy(storage, &old_storage[old_base], 
           (old_capacity - old_base) * sizeof(qthread_t*));
    memcpy(&storage[old_capacity - old_base], 
           old_storage, old_base * sizeof(qthread_t*));

    stack->storage  = storage;
    stack->base     = 0;
    stack->top      = old_capacity - 1;
    stack->capacity = 2 * old_capacity;
    free(old_storage); 
}

static QINLINE void qt_stack_push(qt_stack_t *stack, qthread_t *t)
{
    if (qt_stack_is_full(stack)) {
        qt_stack_resize(stack);
    }
    stack->top = (stack->top + 1) % (stack->capacity);
    stack->storage[stack->top] = t;
    stack->empty = 0;
}

static QINLINE void qt_stack_enq_base(qt_stack_t *stack, qthread_t *t)
{
    if (qt_stack_is_full(stack)) {
        qt_stack_resize(stack);
    }
    stack->storage[stack->base] = t;
    stack->base = (stack->base - 1 + stack->capacity) % (stack->capacity);
    stack->empty = 0;
}

static QINLINE qthread_t* qt_stack_pop(qt_stack_t *stack)
{
    if (qt_stack_is_empty(stack)) {
        return(NULL);
    }
    qthread_t *t = stack->storage[stack->top];
    assert(t != NULL);
    if ((t->flags & QTHREAD_REAL_MCCOY) && (qthread_worker(NULL) != 0)) {
        return(NULL);
    }
    stack->top = (stack->top - 1 + stack->capacity) % (stack->capacity);
    if (stack->top == stack->base) stack->empty = 1;
    return(t);
}

#endif // _QT_THREADQUEUE_STACK_H_
