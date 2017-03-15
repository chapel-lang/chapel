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

   :record:`Shared` (along with :record:`Owned`) manage the deallocation
   of a class instance. :record:`Shared` is meant to be used when many
   different references will exist to the object and these references
   need to keep the object alive.

   To use :record:`Shared`, allocate a class instance following this
   pattern:

   .. code-block:: chapel

     var mySharedObject = new Shared(new MyClass(...));

   now, when mySharedObject goes out of scope, the class instance
   it refers to will be deleted.

   Now, copy initializing or assigning from mySharedObject will make
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


  record Shared {
    pragma "no doc"
    var p;                 // contained pointer (class type)
    pragma "no doc"
    var pn:ReferenceCount; // reference counter

    proc Shared(p) {

      // Boost version default-initializes px and pn
      // and then swaps in different values.

      if !isClass(p.type) then
        compilerError("RefCounted only works with classes");

      this.p = p;
      this.pn = new ReferenceCount();

      //super.init();

      // Boost includes a mechanism for classes inheriting from
      // enable_shared_from_this to record a weak pointer back to the
      // shared pointer. That would need to be handled in a Phase 2
      // since it would refer to `this` as a whole here.
    }

    proc ~Shared() {
      release();
    }


    proc retain() {
      pn.retain();
    }

    proc release() {
      var count = pn.release();
      if count == 0 {
        delete p;
        delete pn;
      }
    }

    proc /*const*/ borrow() {
      return p;
    }

    // = should call retain-release
    // copy-init should call retain
  }

  pragma "init copy fn"
  pragma "no doc"
  proc chpl__initCopy(src: Shared) {
    // This pragma may be unnecessary.
    pragma "no auto destroy"
    var ret:src.type;
    ret.p = src.p;
    ret.pn = src.pn;
    ret.retain();
    return ret;
  }

  pragma "donor fn"
  pragma "auto copy fn"
  pragma "no doc"
  proc chpl__autoCopy(src: Shared) {
    // This pragma may be unnecessary.
    pragma "no auto destroy"
    var ret:src.type;
    ret.p = src.p;
    ret.pn = src.pn;
    ret.retain();
    return ret;
  }

  proc =(ref lhs:Shared, rhs: Shared) {
    // retain-release
    rhs.retain();
    lhs.release();
    lhs.p = rhs.p;
    lhs.pn = rhs.pn;
  }

}
