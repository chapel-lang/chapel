/*
 * Classes Example
 *
 * This example demonstrates the declaration and use of classes.
 *
 */

//
// A class is a type that can contain variables and constants as well as
// methods. A new class is declared using the 'class' keyword.
//
class C {
  var a, b: int;

  def printFields() {
    writeln("a = ", a, " b = ", b);
  }
}

//
// Class variables can be declared using the 'new' keyword to call the
// constructor for the class C. The default constructor has an argument for
// each field in the class. Once a class has been constructed, its methods
// can be called. 
//
var c = new C(1, 2);
c.printFields();

//
// Methods can also be defined outside of the class body by prefixing the
// method name with the class name. All methods have an implicit 'this'
// argument that is a reference to the object the method was called with.
// The 'this' argument can be used to access a field explicitly.
//
def C.sum_a_b_b(b: int) {
  return a + b + this.b;
}

writeln(c.sum_a_b_b(3));


//
// Here, a class named 'D' is declared as a derived class from 'C'. This new
// class has all of the fields and methods from 'C', plus any additional fields
// or methods it declares. The printFields method has the same signature as
// a method from 'C' - it is overridden.
//
class D: C {
  var c = 1.2, d = 3.4;

  def printFields() {
    writeln("a = ", a, " b = ", b, " c = ", c, " d = ", d);
  }
}

//
// The static type of the variable 'c', declared above, is 'C'. Because the
// class 'D' is derived from 'C', this variable is able to reference an object
// of type 'D'. If an overridden method such as printFields is called, it
// is dynamically dispatched to the method with the most specific dynamic
// type possible.
//
c = new D(3, 4);
c.printFields();

//
// There are a few method names that cause a method to have special meaning.
// A method named 'this' allows a class to be indexed like an array. A method
// named 'these' allows a class to be iterated over.
class ArrayLike {
  var a, b, c, d: int;
  def this(i:int) var {
    select i {
      when 1 do return a;
      when 2 do return b;
      when 3 do return c;
      when 4 do return d;
      otherwise halt("index out of bounds", i);
    }
  }

  def these() var {
    yield a;
    yield b;
    yield c;
    yield d;
  }
}

//
// An 'ArrayLike' object can be indexed like a 4 element array and it can
// be iterated over in a loop. 
//
var a = new ArrayLike();
a(2) = 1;
a(4) = 2;

for elt in a do
  writeln(elt);

