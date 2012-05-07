proc p { record R {} }
proc q { record Q {} return Q(); }
if false then q;
writeln("hello");
