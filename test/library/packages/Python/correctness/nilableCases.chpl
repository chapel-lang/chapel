//
// A collection of potential issues that may arise with nilable types.
//

use Python;

var interp = new Interpreter();

{
  var v = new Value?(interp, 10);
  writeln(v);
}

{
  var l = interp.compileLambda("lambda x,: x");
  var v = l(owned Value?, 10);
  writeln(v);
}

{
  var s = interp.get(owned Value?, 'set');
  writeln(s);
  var v = s!();
  writeln(v);
}

{
  var s = interp.get('set');
  var v = s(owned Value?);
  writeln(v);
}


{
  var l = interp.compileLambda("lambda x,: (x,x)");
  var v = l((owned Value?, owned Value?), 10);
  writeln(v, " ", v.type:string);
}
