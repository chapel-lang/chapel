var j : integer = 2;

while (j < 100) {
  j *= (j + 1);
}

enum ExprTypes { ADD, MUL, DIV, NEG=j };

var E : ExprTypes = ADD;
var i : integer;

i = E;
writeln("The first type is ", E, " or, as an integer, ", i);

E = MUL;
i = E;
writeln("The second type is ", E, " or, as an integer, ", i);

E = DIV;
i = E;
writeln("The third type is ", E, " or, as an integer, ", i);

E = NEG;
i = E;
writeln("The fourth type is ", E, " or, as an integer, ", i);
