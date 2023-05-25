// test for #15650

var n: nothing;
writeln(n);

var tuple_of_nothing_1: 1*nothing;
writeln(tuple_of_nothing_1);

var tuple_of_nothing:2*nothing;
writeln(tuple_of_nothing);

var onetuple = (none,);
writeln(onetuple);

var twotuple = (none, none);
writeln(twotuple);

var typedonetuple: 1*nothing = (none,);
writeln(onetuple);

var typedtwotuple: 2*nothing = (none, none);
writeln(twotuple);
