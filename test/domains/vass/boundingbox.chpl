// As of #22858, range,domain.boundingBox() are not user-facing.

proc test(arg) {
  writeln(arg, "   ", arg.boundingBox());
}

test(1..10);
test(1..10 by 2);
test(1..10 by 2 align 4);
writeln();

enum color {red, orange, yellow, green, blue, indigo, violet};
use color;
test(red..violet);
test(red..violet by 3);
test(orange..violet by 3);
test(orange..violet by 3 align yellow);
writeln();

test({1..10});
test({1..10 by 2});
test({1..10 by 2 align 4});
writeln();

test({1..10, 1..10 by 2});
test({1..10 by 2 align 4, 1..10 by 3});
test({1..10 by -3 align -5, 1..10 by -2 align -3, 1..10 by -1});
