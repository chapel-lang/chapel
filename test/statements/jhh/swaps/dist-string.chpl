var x = "Hello";

on Locales[1] {
    var y = "World!";

    writeln("x is: ", x);
    writeln("y is: ", y);
    x <=> y;
    writeln("x is: ", x);
    writeln("y is: ", y);
}
writeln("x is: ", x);
