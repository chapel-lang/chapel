use CTypes;

proc foo() { return 123; }

const foo_ptr : c_fn_ptr = c_ptrTo(foo);

writeln(foo_ptr != nil);
writeln(nil != foo_ptr);
writeln(foo_ptr == nil);
writeln(nil == foo_ptr);
