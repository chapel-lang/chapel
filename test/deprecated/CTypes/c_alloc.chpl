use CTypes;

type t = c_int;

var a = c_malloc(t, 3);
var b = c_calloc(t, 2);
var c = c_aligned_alloc(t, c_sizeof(t) * 4, 4);

c_free(a);
c_free(b);
c_free(c);
