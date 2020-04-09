// Classes

/*
   This primer covers the declaration and use of classes.
*/

//
// A class is a type that can contain variables and constants, called
// fields, as well as functions and iterators called methods.  A new
// class type is declared using the ``class`` keyword.
//
class C {
  var a, b: int;
  proc printFields() {
    writeln("a = ", a, " b = ", b);
  }
}

//
// The ``new`` keyword creates an instance of a class by calling an
// initializer. The class ``C`` above does not declare any initializers,
// and so the compiler-generated one is used. The compiler-generated
// initializer has an argument for each field in the class. Once a class
// has been initialized, its methods can be called.
//
// Classes have various memory management strategies that determine how
// they are freed.  We'll discuss these more below, but for now, know
// that ``new C(...)`` is equivalent to writing out ``new owned C(...)``
// where ``owned`` is one of these memory management strategies.
//
// A class variable can refer to an *instance* of a class.
var foo = new C(1, 3);
foo.printFields();

//
// Default output is supported so a class can be written by making a
// call to ``write`` or ``writeln``.  Default input is also supported.
//
writeln(foo);


// A class variable can refer to an *instance* of a class. Different class
// variables can refer to the same instance. For example, ``alias`` below
// refers to the same memory that stores the fields of ``foo``.
//
// We'll talk more about ``borrow`` below.
var alias = foo.borrow();
// now ``alias.b`` and ``foo.b`` refer to the same field,
// so the next line also modifies ``foo.b``
alias.b -= 1;
writeln(foo);

//
// Methods can also be defined outside of the class body by prefixing
// the method name with the class name.  All methods have an implicit
// ``this`` argument that is a reference to the class instance, or
// object.  The ``this`` argument can be used to access a field
// explicitly.  For example, in the method below, the ``this`` argument
// is used to access the ``b`` field which is otherwise shadowed by the
// ``b`` argument.
//
proc C.sum_a_b_b(b: int) {
  return a + b + this.b;
}
writeln(foo.sum_a_b_b(3));

//
// Here, a class named ``D`` is declared as a derived class from ``C``.
// This new class has all of the fields and methods from ``C``, plus any
// additional fields or methods it declares.  The ``printFields`` method
// has the same signature as a method from ``C`` -- it is overridden.
//
class D: C {
  var c = 1.2, d = 3.4;
  override proc printFields() {
    writeln("a = ", a, " b = ", b, " c = ", c, " d = ", d);
  }
}

//
// The static type of the variable ``foo``, declared above, is ``C``.
// Because the class ``D`` is derived from ``C``, the variable ``foo`` can
// reference an object of type ``D``.  If an overridden method such as
// ``printFields`` is called, it is dynamically dispatched to the method
// with the most specific dynamic type.
//
// Note that since ``foo`` is an ``owned C``, assigning to it
// will delete the previous instance "owned" by that variable.
foo = new D(3, 4);
foo.printFields();


// A class type includes a memory management strategy. The currently supported
// strategies are ``owned``, ``shared``, ``unmanaged``, and ``borrowed``.
var unm: unmanaged C = new unmanaged C();
// ``unm`` refers to a manually managed instance. It needs to have ``delete``
// called on it to free the memory.
delete unm;

var own: owned C = new owned C(1, 10);
// The instance referred to by ``own`` is deleted when it is no longer in scope.
// Only one ``owned C`` can refer to a given instance at a time, but the
// ownership can be transferred to another variable.

var own2 = new C(1, 10);
assert(own.type == own2.type);
// The example above shows that ``new C(...)`` can be used as a
// shorthand for ``new owned C(...)`` because ``owned`` is the default
// memory management strategy for classes.

var share: shared C = new shared C(1, 10);
// The instance referred to by ``share`` is reference counted -- that is,
// several ``shared C`` variables can refer to the same instance and
// will be reclaimed when the last one goes out of scope.

var tmp: borrowed C = new borrowed C(1, 10);
// The instance referred to by ``tmp`` will be deleted when it is no longer in
// scope. The ownership can't be transferred to another variable.

// It is possible to ``borrow`` from another class pointer.
// One way to do that is by calling the ``borrow()`` method directly:

var b1 = own.borrow();
// now b1 and own refer to the same instance
// it is illegal to:
//  * use the borrow after whatever it is borrowed from goes out of scope
//  * use the borrow after the instance is deleted (for example if
//    own is assigned to)

// A class type without a decorator, such as ``C``, has generic management.
// The ``this`` argument of a method is generally ``borrowed C``.

// The compiler automatically adds conversion from ``owned``, ``shared``,
// or ``unmanaged`` in the process of resolving a function call,
// method call, or variable initialization.

var b2: borrowed C = own; // same as b2 = own.borrow();
own.printFields(); // same as own.borrow().printFields();
proc printSum(arg: borrowed C) {
  var sum = arg.a + arg.b;
  writeln(sum);
}
printSum(own); // same as printSum(own.borrow())

// A variable of class type cannot store ``nil`` unless it is
// declared to have nilable class type. To create a nilable class
// type, apply the ``?`` operator to another class type
var x: borrowed C?; // default-initializes to ``nil``

// Non-nilable class types can be implicitly converted to the corresponding
// nilable class type.
x = b2; // converting from borrowed C to borrowed C?

// The method printFields is available on ``borrowed C``,
// but not on ``borrowed C?``
//
// As a result, the call ``x.printFields()`` needs adjustment.
// The ``!`` operator is available to assert that an expression
// is not ``nil`` and return it as a non-nilable type. This operator
// will halt if the value is actually ``nil``.
//
// Note that when applied to an ``owned`` or ``shared`` variable, ``!`` will
// result in a borrow from that variable.
x!.printFields();

// There are a few method names that cause the method to have special
// meaning. Please see :ref:`primers-specialMethods` for details.
