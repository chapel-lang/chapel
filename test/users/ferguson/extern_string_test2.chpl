use CTypes;
extern proc get_string(ref string_out:c_ptrConst(c_char));
extern proc modify_string(ref string_out:c_ptrConst(c_char), string_in:c_ptrConst(c_char));

var a:string;
var b:string;

var ca: c_ptrConst(c_char);
get_string(ca);

var cb: c_ptrConst(c_char);
modify_string(cb, ca);

a = string.createCopyingBuffer(ca);
b = string.createCopyingBuffer(cb);
writeln("a is: ", a);
writeln("b is: ", b);

