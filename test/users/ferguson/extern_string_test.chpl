extern proc return_string_test():c_string;
extern proc return_string_arg_test(ref c_string);

writeln("returned string ", createStringWithNewBuffer(return_string_test()));
var cs: c_string;
return_string_arg_test(cs);
var s = createStringWithNewBuffer(cs);
writeln("returned string arg ",s);

