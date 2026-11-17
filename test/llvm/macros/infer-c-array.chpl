use CTypes;

extern proc INFER_CSTRING_LEN(s): c_size_t;

var chars: c_array(c_char, 3);
writeln(INFER_CSTRING_LEN(chars));
