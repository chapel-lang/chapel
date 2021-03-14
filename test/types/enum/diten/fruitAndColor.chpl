enum fruit { apple, mango, orange, strawberry };
enum color { red, green, blue, orange, purple, silver };

var a: fruit = fruit.orange;
var b: color = color.orange;
var fruit_orange = 3; // designed to clash with fruit.orange's c name

writeln(a);
writeln(b);
writeln(fruit_orange);
