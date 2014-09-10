/*
 * Classes Primer
 *
 * This primer covers the declaration and use of classes.
 *
 */

//
// A class is a type that can contain variables and constants, called
// fields, as well as functions and iterators called methods.  A new
// class type is declared using the 'class' keyword.
//
class C {
  var a, b: int;
  proc printFields() {
    writeln("a = ", a, " b = ", b);
  }
}

//
// Class variables can be declared using the 'new' keyword to call the
// constructor for the class C.  The default constructor has an argument for
// each field in the class.  Once a class has been constructed, its methods
// can be called.
//
var c = new C(1, 2);
c.printFields();

//
// Default output is supported so a class can be written by making a
// call to write or writeln.  Default input is also supported.
//
writeln(c);

//
// Methods can also be defined outside of the class body by prefixing
// the method name with the class name.  All methods have an implicit
// 'this' argument that is a reference to the class instance, or
// object.  The 'this' argument can be used to access a field
// explicitly.  For example, in the method below, the 'this' argument
// is used to access the 'b' field which is otherwise shadowed by the
// 'b' argument.
//
proc C.sum_a_b_b(b: int) {
  return a + b + this.b;
}
writeln(c.sum_a_b_b(3));

//
// Here, a class named 'D' is declared as a derived class from 'C'.
// This new class has all of the fields and methods from 'C', plus any
// additional fields or methods it declares.  The printFields method
// has the same signature as a method from 'C'--it is overridden.
//
class D: C {
  var c = 1.2, d = 3.4;
  proc printFields() {
    writeln("a = ", a, " b = ", b, " c = ", c, " d = ", d);
  }
}

//
// The static type of the variable 'c', declared above, is 'C'.
// Because the class 'D' is derived from 'C', the variable 'c' can
// reference an object of type 'D'.  If an overridden method such as
// printFields is called, it is dynamically dispatched to the method
// with the most specific dynamic type.  The 'delete' keyword can be
// used to free memory associated with c before pointing it an instance
// of 'D'.
//
delete c;
c = new D(3, 4);
c.printFields();
delete c;

//
// There are a few method names that cause the method to have special
// meaning.  A method named 'this' allows a class to be indexed like
// an array.  A method named 'these' allows a class to be iterated
// over.
//
class ArrayLike {
  var a, b, c, d: int;
  proc this(i:int) ref {
    select i {
      when 1 do return a;
      when 2 do return b;
      when 3 do return c;
      when 4 do return d;
      otherwise halt("index out of bounds", i);
    }
  }

  iter these() ref {
    yield a;
    yield b;
    yield c;
    yield d;
  }
}

//
// An 'ArrayLike' object can be indexed like a 4 element array and it
// can be iterated over in a loop.
//
var a = new ArrayLike();
a(2) = 1; // call to this method
a(4) = 2; // call to this method

for elt in a do // invocation of these iterator
  writeln(elt);
delete a;
