var j : int = 2;

while (j < 100) {
  j *= (j + 1);
}

enum ExprTypes { ADD, MUL, DIV, NEG=j };

var E : ExprTypes = ExprTypes.ADD;
var i : int;

i = E;
writeln("The first type is ", E, " or, as an int, ", i);

E = ExprTypes.MUL;
i = E;
writeln("The second type is ", E, " or, as an int, ", i);

E = ExprTypes.DIV;
i = E;
writeln("The third type is ", E, " or, as an int, ", i);

E = ExprTypes.NEG;
i = E;
writeln("The fourth type is ", E, " or, as an int, ", i);
