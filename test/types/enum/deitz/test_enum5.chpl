enum ExprTypes { ADD, MUL, DIV, NEG=-1 };

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
