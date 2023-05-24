use CTypes;
extern proc print_c_string(arg: c_string);

var str = "Hello";

writeln("Should be Hello:");
print_c_string(c_ptrToConst_helper(str):c_string);
