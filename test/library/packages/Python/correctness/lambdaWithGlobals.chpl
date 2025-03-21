use Python;

var interp = new Interpreter();

interp.set("a", 1);

{
  var f = interp.compileLambda("lambda y,z,: a+y+z");
  var g = f(2, 3);
  writeln("f(2, 3)=", g);
}

interp.run("""
def getFunc(x):
  return lambda y: x + y
""");

{
  var f = interp.compileLambda("lambda x,: getFunc(x)(2)+1");
  var g = f(2);
  writeln("f(2)=", g);
}


{
  var f = interp.compileLambda("lambda x,: x");
  var f2 = interp.compileLambda("lambda x,y,z: func(x) + func(y) + func(z)");
  interp.set("func", f);
  var g = f2(1, 2, 3);
  writeln("f2(1, 2, 3)=", g);
}
