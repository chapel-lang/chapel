use CTypes;
extern proc print_c_string(arg: c_string);

var str = "Hello";

writeln("Should be Hello:");
print_c_string(c_ptrToConst_helper(str):c_string);

extern proc print_c_ptrConst(arg: c_ptrConst(c_uchar));
var str2 = "Goodbye";
writeln("Should be Goodbye:");
print_c_ptrConst(c_ptrToConst_helper(str2));