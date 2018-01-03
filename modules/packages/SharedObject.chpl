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

   :record:`Shared` (along with :record:`~OwnedObject.Owned`) manage the deallocation
   of a class instance. :record:`Shared` is meant to be used when many
   different references will exist to the object and these references
   need to keep the object alive.

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

 */
module SharedObject {


  // TODO unify with RefCountBase. Even though that one is for
  // intrusive ref-counting and this one isn't, there's no fundamental
  // reason it couldn't be one class.
  pragma "no doc"
  class ReferenceCount {
    var count: atomic int;

    // count should be initialized to 1 in default initializer.
    // Pretty much any strategy can do that.
    // There is the wrinkle that initializing an atomic
    // currently happens with a.write(1). We could presumably
    // support = for initialing atomics, however.
    proc ReferenceCount() {
      //count = 1;      // this should work
      //super.init();
      count.write(1); // how you'd write it now
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
  record Shared {
    pragma "no doc"
    type t;              // contained type (class type)

    pragma "no doc"
    var p:t;             // contained pointer (class type)

    forwarding p;

    pragma "no doc"
    var pn:ReferenceCount; // reference counter

    /*
       Default-initialize a :record:`Shared`.
     */
    proc Shared(type t) {
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
    proc Shared(p, type t=p.type) {

      // Boost version default-initializes px and pn
      // and then swaps in different values.

      if !isClass(p) then
        compilerError("Shared only works with classes");

      var rc:ReferenceCount = nil;

      if p != nil then
        rc = new ReferenceCount();

      this.p = p;
      this.pn = rc;

      //super.init();

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
    proc Shared(src:Shared, type t=src.t) {
      this.p = src.p;
      this.pn = src.pn;

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


  // initCopy is here as a workaround for problems
  // with generic initializers.
  pragma "init copy fn"
  pragma "no doc"
  proc chpl__initCopy(src: Shared) {
    // This pragma may be unnecessary.
    //pragma "no auto destroy"
    var ret = new Shared(src);
    return ret;
  }

  /*
     Assign one :record:`Shared` to another.
     Deletes the object managed by `lhs` if there are
     no other :record:`Shared` referring to it. On return,
     `lhs` will refer to the same object as `rhs`.
   */
  proc =(ref lhs:Shared, rhs: Shared) {
    // retain-release
    if rhs.pn != nil then
      rhs.pn.retain();
    lhs.clear();
    lhs.p = rhs.p;
    lhs.pn = rhs.pn;
  }
}
