var x = [i in 1..10] i;
var typeStr = x.type:string;

proc foo(x) : string {
  return x.type:string;
}

var exprTypeStr = foo([i in 1..10] i);
writeln(typeStr == exprTypeStr); // relies on == comparing string contents
