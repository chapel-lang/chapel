enum foo {a=0, b=1};

writeln(chpl_enum_first(foo));
var example1: foo;
writeln(example1);

enum bar {b=1, a=0};

writeln(chpl_enum_first(bar));
var example2: bar;
writeln(example2);
