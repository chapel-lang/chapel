/*
.. primers-yminutes-classes:

Classes
-------
*/
// Classes are similar to those in C++ and Java, allocated on the heap.
class MyClass {

// Member variables
  var memberInt : int;
  var memberBool : bool = true;

// By default, any class that doesn't define an initializer gets a
// compiler-generated initializer, with one argument per field and
// the field's initial value as the argument's default value.
// Alternatively, the user can define initializers manually as shown
// in the following commented-out routine:
//
/* .. code-block:: chapel

      // proc init(val : real) {
      //   this.memberInt = ceil(val): int;
      // }
*/

// Explicitly defined deinitializer.
// If we did not write one, we would get the compiler-generated deinitializer,
// which has an empty body.
  proc deinit() {
    writeln("MyClass deinitializer called ", (this.memberInt, this.memberBool));
  }

// Class methods.
  proc setMemberInt(val: int) {
    this.memberInt = val;
  }

  proc setMemberBool(val: bool) {
    this.memberBool = val;
  }

  proc getMemberInt(): int{
    return this.memberInt;
  }

  proc getMemberBool(): bool {
    return this.memberBool;
  }
} // end MyClass

// We can define an operator on our class as well, but
// the definition has to be outside the class definition.
proc +(A : MyClass, B : MyClass) : owned MyClass {
  return
    new owned MyClass(memberInt = A.getMemberInt() + B.getMemberInt(),
                      memberBool = A.getMemberBool() || B.getMemberBool());
}

// Call compiler-generated initializer, using default value for memberBool.
proc ttTest() {
  var myObject = new owned MyClass(10);
      myObject = new owned MyClass(memberInt = 10); // Equivalent
  writeln(myObject.getMemberInt());

  // Same, but provide a memberBool value explicitly.
  var myDiffObject = new owned MyClass(-1, true);
      myDiffObject = new owned MyClass(memberInt = -1,
                                       memberBool = true); // Equivalent
  writeln(myDiffObject);

  // Similar, but rely on the default value of memberInt, passing in memberBool.
  var myThirdObject = new owned MyClass(memberBool = true);
  writeln(myThirdObject);

  // If the user-defined initializer above had been uncommented, we could
  // make the following calls:
  //
  /* .. code-block:: chapel

        // var myOtherObject = new MyClass(1.95);
        //     myOtherObject = new MyClass(val = 1.95);
        // writeln(myOtherObject.getMemberInt());
  */

  var plusObject = myObject + myDiffObject;
  writeln(plusObject);

  // Destruction of an object: calls the deinit() routine and frees its memory.
  // ``unmanaged`` variables should have ``delete`` called on them.
  // ``owned`` variables are destroyed when they go out of scope.
}
ttTest();
