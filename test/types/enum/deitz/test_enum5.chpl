enum ExprTypes { ADD=1, MUL, DIV, NEG=-1 };

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
