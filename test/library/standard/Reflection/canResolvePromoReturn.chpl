proc printHelper(expr) {
  writeln(expr);
  writeln(expr.type : string);
  writeln(iteratorToArrayElementType(expr.type) : string);
  for x in expr do writeln(x);
}

use Reflection;
var x = [0,2,5];
var y = [0,2,5];

writeln(canResolve("==",x,y));
printHelper(x==y);

printHelper(x!=y);
writeln(canResolve("!=",x,y));
