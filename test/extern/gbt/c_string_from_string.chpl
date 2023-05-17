extern proc print_c_string(arg: c_string);

var str = "Hello";

writeln("Should be Hello:");
print_c_string(str:c_ptrConst(c_char):c_string);
