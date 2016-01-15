extern proc get_string(ref string_out:c_string);
extern proc modify_string(ref string_out:c_string, string_in:c_string);

var a:string;
var b:string;

var ca: c_string;
get_string(ca);

var cb: c_string;
modify_string(cb, ca);

a = ca:string;
b = cb:string;
writeln("a is: ", a);
writeln("b is: ", b);

