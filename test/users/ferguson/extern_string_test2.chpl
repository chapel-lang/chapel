extern proc get_string(ref string_out:string);
extern proc modify_string(ref string_out:string, string_in:string);

var a:string;
var b:string;

get_string(a);

modify_string(b, a);

writeln("a is: ", a);
writeln("b is: ", b);

