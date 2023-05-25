var x = b"Hello";

on Locales[1] {
    var y = b"World!";

    writeln("x is: ", x);
    writeln("y is: ", y);
    x <=> y;
    writeln("x is: ", x);
    writeln("y is: ", y);
}
writeln("x is: ", x);
