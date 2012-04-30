/*
 * KNOWN BUGS :
 *
 * - Undefined behaviour if a codelet is used by more than one task
 * - buffers[x].mode must be STARPU_{R,W,RW} : cant be a variable
 */
@r@
identifier c;
identifier t;
expression id;
constant MODE;
expression H;
@@
 t->cl = &c;
<...
- t->buffers[id].handle = H;
+ t->handles[id] = H;
- t->buffers[id].mode = MODE;
...>

@s depends on r@
identifier r.c;
expression r.id;
constant r.MODE;
@@
struct starpu_codelet c = {
++	.modes[id] = MODE,
};

