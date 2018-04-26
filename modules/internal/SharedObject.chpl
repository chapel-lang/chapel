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

   :record:`Shared` (along with :record:`~OwnedObject.Owned`) manage the
   deallocation of a class instance. :record:`Shared` is meant to be used when
   many different references will exist to the object and these references need
   to keep the object alive.

   To use :record:`Shared`, allocate a class instance following this
   pattern:

   .. code-block:: chapel

     var mySharedObject = new Shared(new MyClass(...));

   When mySharedObject and any copies of it go out of scope, the class
   instance it refers to will be deleted.

   Copy initializing or assigning from mySharedObject will make
   other variables refer to the same class instance. The class instance
   will be deleted after all of these references go out of scope.

   .. code-block:: chapel

     var globalSharedObject:Shared(MyClass);

     proc makeGlobalSharedObject() {
       var mySharedObject = new Shared(new MyClass(...));
       globalSharedObject = mySharedObject;
       // now mySharedObject is deinitialized, but the MyClass
       // instance is not deleted until globalSharedObject is deinitialized.
     }

   :record:`Shared` supports coercions to the class type as well as
   coercions from a ``Shared(T)`` to ``Shared(U)`` where ``T`` is a
   subclass of ``U``. See :record:`~OwnedObject.Owned` for examples
   of these coercions.

 */
module SharedObject {


  // TODO unify with RefCountBase. Even though that one is for
  // intrusive ref-counting and this one isn't, there's no fundamental
  // reason it couldn't be one class.
  pragma "no doc"
  class ReferenceCount {
    var count: atomic int;

    // count should be initialized to 1 in default initializer.
    proc init() {
      // Want this:      count = 1;
      this.complete();
      count.write(1);
    }

    proc retain() {
      count.add(1);
    }
    proc release() {
      var oldValue = count.fetchSub(1);
      return oldValue - 1;
    }
  }


  /*

     The :record:`Shared` manages the deletion of a class instance in a way
     that supports multiple owners of the class instance.

     This is currently implemented with task-safe reference counting.

   */
  pragma "managed pointer"
  record _shared {
    pragma "no doc"
    type t;              // contained type (class type)

    pragma "no doc"
    pragma "owned"
    var p:t;             // contained pointer (class type)

    forwarding p;

    pragma "no doc"
    pragma "owned"
    var pn:ReferenceCount; // reference counter

    /*
       Default-initialize a :record:`Shared`.
     */
    proc init(type t) {
      this.t = _to_borrowed(t);
      this.p = nil;
      this.pn = nil;
    }

    /*
       Initialize a :record:`Shared` with a class instance.
       This :record:`Shared` will take over the deletion of the class
       instance. It is an error to directly delete the class instance
       while it is managed by :record:`Shared`.

       :arg p: the class instance to manage. Must be of class type.
     */
    proc init(p) {
      this.t = _to_borrowed(p.type);

      // Boost version default-initializes px and pn
      // and then swaps in different values.

      if !isClass(p) then
        compilerError("Shared only works with classes");

      var rc:ReferenceCount = nil;

      if p != nil then
        rc = new ReferenceCount();

      this.p = p;
      this.pn = rc;

      this.complete();

      // Boost includes a mechanism for classes inheriting from
      // enable_shared_from_this to record a weak pointer back to the
      // shared pointer. That would need to be handled in a Phase 2
      // since it would refer to `this` as a whole here.
    }

    /*
       Copy-initializer. Creates a new :record:`Shared`
       that refers to the same class instance as `src`.
       These will share responsibility for managing the instance.
     */
    proc init(src:_shared(?)) {
      this.t = src.t;
      this.p = src.p;
      this.pn = src.pn;

      this.complete();

      if this.pn != nil then
        this.pn.retain();
    }

    /*
       The deinitializer for :record:`Shared` will destroy the class
       instance once there are no longer any copies of this
       :record:`Shared` that refer to it.
     */
    proc deinit() {
      clear();
    }

    /*
       Change the instance managed by this class to `newPtr`.
       If this record was the last :record:`Shared` managing a
       non-nil instance, that instance will be deleted.
     */
    proc ref retain(newPtr:p.type) {
      clear();
      this.p = newPtr;
      if newPtr != nil {
        this.pn = new ReferenceCount();
      }
    }

    /*
       Empty this :record:`Shared` so that it stores `nil`.
       Deletes the managed object if this :record:`Shared` is the
       last :record:`Shared` managing that object.
       Does not return a value.

       Equivalent to ``Shared.retain(nil)``.
     */
    proc ref clear() {
      if isClass(p) { // otherwise, let error happen on init call
        if p != nil && pn != nil {
          var count = pn.release();
          if count == 0 {
            delete p;
            delete pn;
          }
        }
        p = nil;
        pn = nil;
      }
    }

    /*
       Return the object managed by this :record:`Shared` without
       impacting its lifetime at all. It is an error to use the
       value returned by this function after the :record:`Shared`
       goes out of scope.
     */
    proc /*const*/ borrow() {
      return p;
    }

    // = should call retain-release
    // copy-init should call retain
  }


  /*
     Assign one :record:`Shared` to another.
     Deletes the object managed by `lhs` if there are
     no other :record:`Shared` referring to it. On return,
     `lhs` will refer to the same object as `rhs`.
   */
  proc =(ref lhs:_shared, rhs: _shared) {
    // retain-release
    if rhs.pn != nil then
      rhs.pn.retain();
    lhs.clear();
    lhs.p = rhs.p;
    lhs.pn = rhs.pn;
  }

  /*
     Swap two :record:`Shared` objects.
   */
  proc <=>(ref lhs: _shared, ref rhs: _shared) {
    lhs.pn <=> rhs.pn;
    lhs.p <=> rhs.p;
  }

  // This is a workaround
  pragma "no doc"
  proc chpl__autoDestroy(x: _shared) {
    __primitive("call destructor", x);
  }

  // Don't print out 'p' when printing an Shared, just print class pointer
  pragma "no doc"
  proc _shared.readWriteThis(f) {
    f <~> this.p;
  }

  // Note, coercion from Shared -> Shared.t is directly
  // supported in the compiler via a call to borrow().

  // This cast supports coercion from Shared(SubClass) to Shared(ParentClass)
  // (i.e. when class SubClass : ParentClass ).
  // It only works in a value context (i.e. when the result of the
  // coercion is a value, not a reference).
  pragma "no doc"
  inline proc _cast(type t, x) where t:_shared && x:_shared && x.t:t.t {
    var ret:t; // default-init the Shared type to return
    ret.p = x.p:t.t; // cast the class type
    ret.pn = x.pn;
    if ret.pn != nil then
      ret.pn.retain();
    return ret;
  }

  type Shared = _shared;
}
