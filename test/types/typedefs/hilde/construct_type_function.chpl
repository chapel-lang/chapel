// construct_type_function.chpl
//
// Show that we can use a type function to nominate the type to be constructed.
//

class B { var i:int; }
class C { var r:real; }

proc foo(param flag: bool) type
{
  if flag then return C;
  else return B;
}

var b = new foo(false)();
b.i = 3;
writeln(b);
delete b;

var c = new foo(true)();
c.r = 7.5;
writeln(c);
delete c;

