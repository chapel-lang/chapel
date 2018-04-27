/*
 * Copyright 2004-2018 Cray Inc.
 * Other additional copyright holders may be indicated within.
 *
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


/*

   :record:`Owned` (along with :record:`~SharedObject.Shared`) manage the
   deallocation of a class instance. :record:`Owned` is meant to be used when
   only one reference to an object needs to manage that object's storage.

   To use :record:`Owned`, store a new class instance in a new Owned
   record as is shown in this example:

   .. code-block:: chapel

     use OwnedObject;
     class MyClass { }

     var myOwnedObject = new Owned(new MyClass());

   When myOwnedObject goes out of scope, the class instance
   it refers to will be deleted.

   Copy initializing from ``myOwnedObject`` or assigning it to another
   :record:`Owned` will leave ``myOwnedObject`` storing a nil value
   and transfer the owned class instance to the other value.

   .. code-block:: chapel

     var otherOwnedObject = myOwnedObject;
     // now myOwnedObject stores nil
     // the value it stored earlier has moved to otherOwnedObject

     myOwnedObject = otherOwnedObject;
     // this assignment moves the value from the right-hand-side
     // to the left-hand-side, leaving the right-hand-side empty.
     // after the assignment, otherOwnedObject stores nil
     // and myOwnedObject stores a value that will be deleted
     // when myOwnedObject goes out of scope.

   The compiler includes support for introducing automatic coercions
   from :record:`Owned` to the contained class type. For example:

   .. code-block:: chapel


     proc f(arg:MyClass) {
       writeln(arg);
     }

     var myOwned = new Owned(new MyClass());
     f(myOwned); // compiler coerces to MyClass via borrow()

   Additionally, the compiler includes support for coercing a value
   of type ``Owned(T)`` to ``Owned(U)`` when ``T`` is a subclass of ``U``.
   For example:

   .. code-block:: chapel

     class Person { }
     class Student : Person { }

     var myStudent = new Owned(new Student());
     var myPerson:Owned(Person) = myStudent;
     // relies on coercion from Owned(Student) to Owned(Person)
     // moves the instance from myStudent to myPerson, leaving
     // myStudent containing nil.

   .. note::

     The ways in which :record:`Owned` may be used are currently limited.
     Copy-initialization, assignment, and `in` intent are expected to work.
     However, it is currently an error to use a :record:`Owned` in a way that
     causes the compiler to add an implicitly copy, such as by returning a
     :record:`Owned` that was passed by reference.

 */
module OwnedObject {

  /*
     :record:`Owned` manages the deletion of a class instance assuming
     that this :record:`Owned` is the only thing responsible for managing
     the lifetime of the class instance.
   */
  pragma "no copy"
  pragma "copy mutates"
  pragma "managed pointer"
  record _owned {
    pragma "no doc"
    type t;                // contained type (class type)

    pragma "no doc"
    pragma "owned"
    var p:t;               // contained pointer (class type)

    forwarding p;

    /*
       Default-initialize a :record:`Owned`.
     */
    proc _owned(type a, type t=_to_borrowed(a)) {
      this.p = nil;
    }

    /*
       Initialize a :record:`Owned` with a class instance.
       When this :record:`Owned` goes out of scope, it will
       delete whatever class instance it is storing.

       It is an error to directly delete the class instance
       while it is managed by a :record:`Owned`.

       :arg p: the class instance to manage. Must be of class type.
     */
    proc _owned(p, type t=_to_borrowed(p.type)) {
      if !isClass(p) then
        compilerError("Owned only works with classes");

      this.p = p;
    }

    /*
       Copy-initializer. Creates a new :record:`Owned`
       that takes over ownership from `src`. `src` will
       refer to `nil` after this call.
     */
    proc _owned(ref src:_owned, type t=src.t) {
      this.p = src.release();
    }

    /*
       The deinitializer for :record:`Owned` will destroy the class
       instance it manages when the :record:`Owned` goes out of scope.
     */
    proc deinit() {
      if isClass(p) { // otherwise, let error happen on init call
        if p != nil then
          delete p;
      }
    }

    /*
       Empty this :record:`Owned` so that it stores `nil`.
       Deletes the previously managed object, if any.
     */
    proc ref clear() {
      if p != nil {
        delete p;
        p = nil;
      }
    }


    /*
       Change the instance managed by this class to `newPtr`.
       If this record was already managing a non-nil instance,
       that instance will be deleted.
     */
    proc ref retain(newPtr:p.type) {
      var oldPtr = p;
      p = newPtr;
      if oldPtr then
        delete oldPtr;
    }

    /*
       Empty this :record:`Owned` so that it manages `nil`.
       Returns the instance previously managed by this :record:`Owned`.
     */
    proc ref release():p.type {
      var oldPtr = p;
      p = nil;
      return oldPtr;
    }

    /*
       Return the object managed by this :record:`Owned` without
       impacting its lifetime at all. It is an error to use the
       value returned by this function after the :record:`Owned`
       goes out of scope.
     */
    proc /*const*/ borrow() {
      return p;
    }
  }

  pragma "no doc"
  proc =(ref lhs:_owned, ref rhs: _owned) {
    lhs.retain(rhs.release());
  }
  pragma "no doc"
  proc =(ref lhs:_owned, rhs:_nilType) {
    lhs.clear();
  }
  pragma "no doc"
  proc <=>(ref lhs:_owned(?t), ref rhs:_owned(t)) {
    lhs.p <=> rhs.p;
  }


  // initCopy is defined explicitly as a workaround
  // for problems with generic initializers
  pragma "init copy fn"
  pragma "no doc"
  pragma "unsafe"
  proc chpl__initCopy(ref src: _owned) {
    var ret = new _owned(src);
    return ret;
  }

  // autoCopy is defined explicitly to create a
  // compilation error if it is invoked
  // (if we decided it was OK, we'd need to do const-checking
  //  on the argument to this autoCopy call).
  pragma "no doc"
  pragma "donor fn"
  pragma "auto copy fn"
  pragma "erroneous autocopy"
  pragma "unsafe"
  proc chpl__autoCopy(ref src: _owned) {
    var ret = new _owned(src);
    return ret;
  }
  // This is a workaround - compiler was resolving
  // chpl__autoDestroy(x:object) from internal coercions.
  pragma "no doc"
  proc chpl__autoDestroy(x: _owned) {
    __primitive("call destructor", x);
  }

  // Don't print out 'p' when printing an _owned, just print class pointer
  pragma "no doc"
  proc _owned.readWriteThis(f) {
    f <~> this.p;
  }

  // Note, coercion from _owned -> _owned.t is directly
  // supported in the compiler via a call to borrow().

  // This cast supports coercion from _owned(SubClass) to _owned(ParentClass)
  // (i.e. when class SubClass : ParentClass ).
  // It only works in a value context (i.e. when the result of the
  // coercion is a value, not a reference).
  pragma "no doc"
  inline proc _cast(type t, in x) where t:_owned && x:_owned && x.t:t.t {
    // the :t.t cast in the next line is what actually changes the
    // returned value to have type t; otherwise it'd have type _owned(x.type).
    var ret = new _owned(x.release():t.t);
    return ret;
  }

  type Owned = _owned;
}
