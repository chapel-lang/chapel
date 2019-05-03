// Records

//
// This primer covers the declaration and uses of records.
//

/*

   A record is a type that can contain variables and constants,
   called fields, as well as functions and iterators called methods.
   Records have many similarities to classes, but there are several important
   differences:

   * classes support inheritance and virtual dispatch while records do not
   * different class variables can refer to the same instance,
     while record variables refer to distinct memory
   * copy-initialization and assignment can be implemented for records
     but not for classes.

   A record type can be declared using the ``record`` keyword:
*/
record Color {
  var red: uint(8);
  var green: uint(8);
  var blue: uint(8);
}

// The ``new`` keyword creates a new record by calling an initializer.
// The record ``Color`` doesn't declare any initializers, and so the
// compiler-generated one is used. The compiler-generated initializer
// has an argument for each field in the record.

// The result of ``new`` can be captured in a variable:
var taupe = new Color(179, 139, 109);
writeln(taupe); // output: (red = 179, green = 139, blue = 109)

// Alternatively, a variable can be declared using the record type
// without explicitly initializing it.
// In that event the variable is default-initialized.
// The compiler-generated initializer allows default initialization.
// It simply default-initializes each field in turn.
var myColor: Color;
writeln(myColor); // output: (red = 0, green = 0, blue = 0)

// Records support methods, both declared within the record
// or declared outside of the record.
// Note that inside of a method, the fields may be accessed
// by name. Additionally `this` refers to the entire record.
proc Color.luminance() {
  return 0.2126*red + 0.7152*green + 0.0722*blue;
}
writeln(taupe.luminance());

// Here is an example of a record to store a point where
// a method is declared inside of the record declaration.
// This record also demonstrates a few initializers.

record Point {
  var x: real;
  var y: real;
  var z: real;

  // This is a default initializer
  proc init() {
    writeln("Point default-init");
    this.x = 0.0;
    this.y = 0.0;
    this.z = 0.0;
  }

  // This is a useful initializer for this type
  proc init(x: real, y: real, z: real) {
    this.x = x;
    this.y = y;
    this.z = z;
  }

  // This is a copy initializer
  proc init=(from: Point) {
    writeln("Point copy-init");
    this.x = from.x;
    this.y = from.y;
    this.z = from.z;
  }
  proc magnitude() {
    return sqrt(x*x + y*y + z*z);
  }
}

// Let's create a default-initialized Point
var emptyPoint:Point; // outputs: Point default-init

// Let's create a point by calling the useful initializer
// and then call the magnitude method on it.
var myPoint = new Point(1.0, 2.0, 2.0);
writeln(myPoint.magnitude()); // outputs: 3.0

// Since different record variables always refer
// to different records, the compiler will add calls
// to the record's copy-initializer when initializing a new
// record variable from another record. For example:
var otherPoint = myPoint; // prints: Point copy-init
otherPoint.x = 5.0;
writeln(otherPoint); // outputs: (x = 5.0, y = 2.0, z = 2.0)
// ``myPoint.x`` remains ``1.0`` since it has different storage.
writeln(myPoint); // outputs: (x = 1.0, y = 2.0, z = 2.0)

// For similar reasons, when assigning between two record variables,
// the compiler will add calls to the record assignment operator.
// The function below defines the assignment operator for Point.

proc = (ref lhs: Point, rhs: Point) {
  writeln("Point assignment");
  lhs.x = rhs.x;
  lhs.y = rhs.y;
  lhs.z = rhs.z;
}

// Assignment between records of this type will invoke the above function:
otherPoint = myPoint; // prints: Point assignment

// Records can also define a deinit method that will be called
// to clean up any resources used by the record when it goes
// out of scope.
proc Point.deinit() {
  writeln("Point deinit");
}

// This block demonstrates when Point.deinit is called
{
  var tmpPoint = myPoint; // prints: Point copy-init
  // tmpPoint now goes out of scope
  // prints: Point deinit
}

// Similarly to classes, records can define the special
// methods ``this`` and ``these``.

// The ``this`` method can make the record behave like
// a function or an array:

proc Point.this(i: int): real {
  if i == 1 then
    return x;
  if i == 2 then
    return y;
  if i == 3 then
    return z;

  return 0.0;
}

// Now we can access the coordinates numerically instead of by name:
writeln(myPoint(1)); // outputs: 1.0

// The ``this`` method enables both parenthesis-style access and square
// brace style:
writeln(myPoint[1]);

// The ``these`` method is an iterator that the compiler will call
// when the record is looped over directly.

iter Point.these() {
  yield x;
  yield y;
  yield z;
}

// Now we can easily loop over the coordinates in the point
for component in myPoint {
  writeln(component);
}
