use CTypes;
extern proc get_string(ref string_out:c_ptr(c_uchar));
extern proc modify_string(ref string_out:c_ptr(c_uchar), string_in:c_ptrConst(c_uchar));

var a:string;
var b:string;

var ca: c_ptr(c_uchar);
get_string(ca);

var cb: c_ptr(c_uchar);
modify_string(cb, ca);

a = string.createCopyingBuffer(ca);
b = string.createCopyingBuffer(cb);
writeln("a is: ", a);
writeln("b is: ", b);

