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

   :record:`Owned` (along with :record:`~SharedObject.Shared`) manage the deallocation
   of a class instance. :record:`Owned` is meant to be used when only
   one reference to an object needs to manage that object's storage.

   To use :record:`Owned`, allocate a class instance following this
   pattern:

   .. code-block:: chapel

     var myOwnedObject = new Owned(new MyClass(...));

   When myOwnedObject goes out of scope, the class instance
   it refers to will be deleted.

   It is an error to copy initialize from myOwnedObject or to assign
   it to another :record:`Owned`.

   .. note::

     The ways in which :record:`Owned` may be used are currently limited.
     Copy-initialization, assignment, and `in` intent are expected to work.
     However, it is an error to use a :record:`Owned` in a way that causes the
     compiler to add an implicitly copy, such as by returning a :record:`Owned`
     that was passed by reference.

   .. note::

     :record:`Owned` arguments with `const in` intent do not work yet.

 */
module OwnedObject {

  /*
     :record:`Owned` manages the deletion of a class instance assuming
     that this :record:`Owned` is the only thing responsible for managing
     the lifetime of the class instance.
   */
  pragma "no copy"
  pragma "copy mutates"
  record Owned {
    pragma "no doc"
    type t;                // contained type (class type)

    pragma "no doc"
    var p:t;               // contained pointer (class type)

    forwarding p;

    /*
       Default-initialize a :record:`Owned`.
     */
    proc Owned(type t) {
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
    proc Owned(p, type t=p.type) {
      if !isClass(p) then
        compilerError("Owned only works with classes");

      this.p = p;
      //super.init();
    }

    /*
       Copy-initializer. Creates a new :record:`Owned`
       that takes over ownership from `src`. `src` will
       refer to `nil` after this call.
     */
    proc Owned(ref src:Owned, type t=src.t) {
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
  proc =(ref lhs:Owned, ref rhs: Owned) {
    lhs.retain(rhs.release());
  }

  // initCopy is defined explicitly as a workaround
  // for problems with generic initializers
  pragma "init copy fn"
  pragma "no doc"
  proc chpl__initCopy(ref src: Owned) {
    var ret = new Owned(src);
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
  proc chpl__autoCopy(ref src: Owned) {
    var ret = new Owned(src);
    return ret;
  }
}
