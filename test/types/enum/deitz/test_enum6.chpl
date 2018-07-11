var j : int = 2;

while (j < 100) {
  j *= (j + 1);
}

enum ExprTypes { ADD, MUL, DIV, NEG=j };

var E : ExprTypes = ExprTypes.ADD;
var i : int;

i = E: int;
writeln("The first type is ", E, " or, as an int, ", i);

E = ExprTypes.MUL;
i = E: int;
writeln("The second type is ", E, " or, as an int, ", i);

E = ExprTypes.DIV;
i = E: int;
writeln("The third type is ", E, " or, as an int, ", i);

E = ExprTypes.NEG;
i = E: int;
writeln("The fourth type is ", E, " or, as an int, ", i);
