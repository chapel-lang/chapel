use CTypes;

extern proc INFER_INT_KIND(x): c_int;
extern proc INFER_FLOAT_KIND(x): c_int;
extern proc INFER_PTR_KIND(p): c_int;
extern "INFER_PTR_KIND" proc typedPtrKind(p: c_ptr(c_int)): c_int;
extern proc INFER_PTR_VALUE(p): c_double;
extern proc INFER_PAIR_KIND(a, b): c_int;
extern proc INFER_CSTRING_LEN(s): c_size_t;

writeln("integers: ", INFER_INT_KIND(7:c_int), " ",
        INFER_INT_KIND(7:c_longlong));
writeln("floats: ", INFER_FLOAT_KIND(1.5:c_float), " ",
        INFER_FLOAT_KIND(1.5:c_double));

var i: c_int = 17;
var d: c_double = 2.5;
writeln("pointers: ", INFER_PTR_KIND(c_ptrTo(i)), " ",
        INFER_PTR_KIND(c_ptrTo(d)));
writeln("values: ", INFER_PTR_VALUE(c_ptrTo(i)), " ",
        INFER_PTR_VALUE(c_ptrTo(d)));
writeln("mixed: ", INFER_PAIR_KIND(7:c_int, 2.5:c_double), " ",
        INFER_PAIR_KIND(1.5:c_float, c_ptrTo(i)));

var ints: c_array(c_int, 2);
ints[0] = 17;
writeln("array as c_ptr: ", typedPtrKind(ints));

var text = "hello";
var chars: c_array(c_char, 3);
chars[0] = 97:c_char;
chars[1] = 98:c_char;
chars[2] = 0:c_char;
writeln("strings: ", INFER_CSTRING_LEN(text.c_str()), " ",
        INFER_CSTRING_LEN("a longer string".c_str()), " ",
        INFER_CSTRING_LEN(c_ptrTo(chars[0])));
