enum ExprTypes { ADD, MUL, DIV, NEG=8 };

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
