/*
 * Copyright 2004-2017 Cray Inc.
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

   :record:`Owned` (along with :record:`Shared`) manage the deallocation
   of a class instance. :record:`Owned` is meant to be used when only
   one reference to an object needs to manage that object's storage.

   To use :record:`Owned`, allocate a class instance following this
   pattern:

   .. code-block:: chapel

     var myOwnedObject = new Owned(new MyClass(...));

   now, when myOwnedObject goes out of scope, the class instance
   it refers to will be deleted.

   It is an error to copy initialize from myOwnedObject or to assign
   it to another :record:`Owned`.

 */
module OwnedObject {

  /*
     :record:`Owned` manages the deletion of a class instance assuming
     that this :record:`Owned` is the only thing responsible for managing
     the lifetime of the class instance.
   */
  pragma "no copy"
  record Owned {
    pragma "no doc"
    var p;                 // contained pointer (class type)

    forwarding p;

    /*
       Initialize a :record:`Owned` with a class instance.
       When this :record:`Owned` goes out of scope, it will
       delete whatever class instance it is storing.

       It is an error to directly delete the class instance
       while it is managed by a :record:`Owned`.

       :arg p: the class instance to manage. Must be of class type.
     */
    proc Owned(p) {
      if !isClass(p.type) then
        compilerError("Owned only works with classes");

      this.p = p;
      //super.init();
    }

    // No copy-init is defined
    // no copy may be made

    /*
       The deinitializer for :record:`Owned` will destroy the class
       instance in manages when the :record:`Owned` goes out of scope.
     */
    proc ~Owned() {
      if p then
        delete p;
    }

    /*
       Change the instance managed by this class to `newPtr`.
       If this class was already managing a non-nil instance,
       that instance will be deleted.
     */ 
    proc ref reset(newPtr:p.type) {
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
    compilerError("Owned may not be assigned. Use lhs.reset(rhs.release()).");
    //lhs.reset(rhs.release());
  }

  // workaround for problems with generic initializers
  pragma "init copy fn"
  pragma "no doc"
  pragma "erroneous initcopy"
  proc chpl__initCopy(src: Owned) {
    return src;
  }

}
