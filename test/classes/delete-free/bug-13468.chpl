class MyClass { var x: int; }

var x : (shared MyClass?, shared MyClass?);
x = (new shared MyClass?(1), new shared MyClass?(2));
writeln(x);

var y : (owned MyClass?, owned MyClass?);
y = (new owned MyClass?(3), new owned MyClass?(4));
writeln(y);
