extern proc return_string_test():string;
extern proc return_string_arg_test(inout string);

writeln("returned string ",return_string_test());
var s:string;
return_string_arg_test(s);
writeln("returned string arg ",s);

