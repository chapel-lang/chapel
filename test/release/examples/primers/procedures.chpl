// Procedures

/*
   This primer covers procedures including overloading, argument
   intents and dynamic dispatch.
*/

//
// A procedure groups computations that can be called from another part
// of the program.
// The procedure can be defined with zero or more "formal" arguments.
// Each formal argument can have a default value associated with it.
//
// Formal arguments are supplied with values when the procedure is called.
// The arguments supplied at the call site are the "actual" arguments.
// If a name and ``=`` precede an actual argument, the actual is assigned
// to the formal argument with that name.  Any remaining (unnamed) actual
// arguments are assigned to the remaining formal arguments in lexical order.
//
// A procedure can return zero, one or more values (as a tuple).
// The return value type can be specified after the formal argument list.
// If no explicit return value type is supplied, the Chapel compiler infers the
// return value type.
//

//
// Here is a procedure which takes an integer argument and returns an integer
// result.  It computes the factorial of the argument.
//
proc factorial(x: int) : int
{
  if x < 0 then
    halt("factorial -- Sorry, this is not the gamma procedure!");

  return if x == 0 then 1 else x * factorial(x-1);
}

writeln("A simple procedure");
writeln("6! is ", factorial(6));
writeln();

//
// Overloading Functions
// ---------------------
// Default integers in Chapel are 64-bits, so we may want to specify
// a version of factorial that operates on 32-bit integers to save
// space and potentially time (depending on the target architecture).
// This version also optimizes a bit, compressing the callstack by
// a factor of two by doing two multiplies.
//
// This version "overloads" the previous version of factorial.
// Upon a call to ``factorial()``, the compiler will choose the best fit.
//
proc factorial(x: int(32)) : int(32)
{
  if x < 1 then
    halt("factorial -- Invalid operand.");

  if x < 3 then return x;

  return x * (x-1) * factorial(x-2);
}

//
// The argument type of this version must be different, so the two
// versions of factorial can be differentiated.  If we pass in a
// (default) 64-bit integer value, we will get the 64-bit version.
//
writeln("Another simple procedure");
writeln("33! is ", factorial(33));

//
// Whereas passing in a 32-bit integer will cause us to get the
// 32-bit version:
writeln("6! is ", factorial(6:int(32)));
writeln();

//
// Overloading Operators
// ---------------------
// Procedure definitions allow you to overload operators, too.  Here
// we define a new type, ``Point``, and overload the definition of ``+``
// to handle that type.
//
record Point { var x, y: real; }

// Tell how to add two points together.
proc +(p1: Point, p2: Point)
{
  // Vector addition in 2-space.
  return new Point(p1.x + p2.x, p1.y + p2.y);
}

//
// We can also overload the ``writeThis()`` routine called by writeln.
//
proc Point.writeThis(w) throws
{
  // Writes it out as a coordinate pair.
  w.write("(");
  w.write(this.x);
  w.write(", ");
  w.write(this.y);
  w.write(")");
}

writeln("Using operator overloading");
var down = new Point(10.0, 0.0);
var over = new Point(0.0, -5.0);
writeln("down + over = ", down + over);
writeln();

//
// Details on Arguments
// ----------------------
// Here we define a class, ``Circle``, and a function which creates a
// specific instance of it using a different style of argument
// definition than we have previously encountered.
//
class Circle {
  var center : Point;
  var radius : real;
}

//
// Note that a default value for an argument can be provided,
// which will be used if a value for that argument is not
// specified in the call.  Here, instead of specifying the
// type of ``x``, ``y``, and ``diameter``, we provide them a default
// value of ``0.0``.  Because we did not specify their type
// but did provide a default value, the type of these arguments
// is inferred to be the type of that value - in this case, it is
// ``real``.
//
proc create_circle(x = 0.0, y = 0.0, diameter = 0.0)
{
  var result = new Circle();

  result.radius = diameter / 2;
  result.center.x = x;
  result.center.y = y;

  return result;
}

writeln("Using named arguments");

//
// Using named actual arguments in the call can prevent confusion.
// Specifying that the first value provided should be used for the
// argument ``diameter`` allows us to define the arguments in any
// order. Additionally, we can take advantage of the default value
// for ``y`` by not specifying a value to use instead.
// Thus this call creates a circle at ``(2.0, 0.0)`` with a radius
// of ``1.5``.
//
var c = create_circle(diameter=3.0,2.0);

writeln(c);
writeln();

//
// Procedures can also have arguments of indeterminate type: these are known
// as generic procedures.
//
proc unknownArg(x)
{
  writeln(x);
  if x.type == int then
    writeln("I see you've passed me an integer!");
  else if x.type == string {
    writeln("I liked that last variable so much, I'll write it again!");
    writeln(x);
  }
}
var intArg = 5;
var strArg = "Greetings, procedure unknownArg!";
var boolArg = false;
writeln("Using generic arguments");
unknownArg(intArg);
unknownArg(strArg);
unknownArg(boolArg);
writeln();

//
// Argument Intents
// ----------------
// Normal (default) intent means that a formal argument cannot be modified
// in the body of a procedure.
// To allow changing the formal (but not the actual), use the ``in`` intent.
//
config param useSleep = true; // Set at compile time, used to speed up testing
use Time;

proc countDown(in n : uint = 10) : void
{
  while n > 0
  {
    writeln(n, " ...");
    if useSleep then sleep(1);
    n -= 1;
  }
  writeln("Blastoff!");
}

writeln("Using the \"in\" intent");
var s = 5 : uint;
countDown(s);
writeln("s is still ", s);	// 5
writeln();

//
// The ``inout`` intent will write back the final value of a formal parameter
// when the procedure exits.
//
proc countDownToZero(inout n : uint = 10) : void
{
  while n > 0
  {
    writeln(n, " ...");
    if useSleep then sleep(1);
    n -= 1;
  }
  writeln("Boink?");
}

writeln("Using the \"inout\" intent");
var t = 3 : uint;
countDownToZero(t);
writeln("t is now ", t);	// 0
writeln();

//
// Similar to the ``inout`` intent, the ``ref`` intent causes the value of
// the actual to change depending on the function.  However, while the
// ``inout`` copies the argument in upon entering the function and copies
// the new value out upon exiting, using a ``ref`` intent causes any updates
// to the formal to immediately affect the call site.
//
proc countDownToZeroToo(ref n : uint = 10) : void
{
  while n > 0
  {
    writeln(n, " ...");
    if useSleep then sleep(1);
    n -= 1;
  }
  writeln("Flippity boop");
}

writeln("Using the \"ref\" intent");
var bip = 3 : uint;
countDownToZeroToo(bip);
writeln("bip is now ", bip);	// 0
writeln();

//
// The ``out`` intent causes the actual argument to be ignored when the
// procedure starts. The actual is assigned the value of the corresponding
// formal when the routine exits.
//
// This arctan routine puts the result in the argument with ``out`` intent
// and returns the number of iterations it needed to converge.
//
// ``atan x = x - x^3/3 + x^5/5 + sum_3^inf (-1)^i x^(2i+1)/(2i+1)``.
//
// This actually converges very slowly for x close to 1 in absolute value.
// So we set the error limit to be 3 significant digits.
//
proc atan(x : real, out result : real)
{
  result = 0.0;
  var count = 0;
  var lastresult = 0.0;
  for i in 1.. by 2
  {
    var twoIP1 = 2 * count + 1;
    var term = x ** twoIP1 / twoIP1;
    result += if count % 2 == 0 then term else -term;
    count += 1;
    if abs(result - lastresult) < 1.0e-3 then break;
    lastresult = result;
  }
  return count;
}

writeln("Using the \"out\" intent");
var theta : real;
var n = atan(1.0, theta);
writeln("Computed Pi as about ", 4.0 * theta, " in ", n, " iterations.");
writeln();

//
// A procedure can take a variable number of arguments -- of indeterminate type.
// It is expanded like a generic procedure, with the required number of
// arguments having types which match the actual arguments.
//
// Note: see the :doc:`varargs` primer for further information on procedures
// with a variable number of arguments
//
proc writeList(x ...?k) {
  var first = true;
  for param i in 1..k {
    if first then first = false; else write(" ");
    write(x(i));
  }
  writeln();
}

writeln("Using variable argument lists.");
writeList(1, "red", 8.72, 1..4);
writeln();



