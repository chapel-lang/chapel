/*
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
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

:record:`shared` (along with :record:`~OwnedObject.owned`) manage the
deallocation of a class instance. :record:`shared` is meant to be used when many
different references will exist to the object at the same time and these
references need to keep the object alive.

Please see also the language spec section :ref:`Class_Lifetime_and_Borrows`.

Using `shared`
--------------

To use :record:`shared`, allocate a class instance following this
pattern:

.. code-block:: chapel

 var mySharedObject = new shared MyClass(...));

When ``mySharedObject`` and any copies of it go out of scope, the class
instance it refers to will be deleted.

Copy initializing or assigning from mySharedObject will make
other variables refer to the same class instance. The class instance
will be deleted after all of these references go out of scope.

.. code-block:: chapel

 var globalSharedObject:shared MyClass;

 proc makeGlobalSharedObject() {
   var mySharedObject = new shared MyClass(...);
   globalSharedObject = mySharedObject;
   // the reference count is decremented when mySharedObject
   // goes out of scope. Since it's not zero after decrementing, the
   // MyClass instance is not deleted until globalSharedObject
   // goes out of scope.
 }

Borrowing from `shared`
-----------------------

The :proc:`shared.borrow` method returns the pointer managed by the
:record:`shared`. This pointer is only valid as long as the :record:`shared` is
storing that pointer. The compiler includes some checking for errors in this
case. In these ways, :record:`shared` is similar to
:record:`~OwnedObject.owned`.

See :ref:`about-owned-borrowing` for more details and examples.

Coercions for `shared`
----------------------

As with :record:`~OwnedObject.owned`, :record:`shared` supports
coercions to the class type as well as
coercions from a ``shared(T)`` to ``shared(U)`` where ``T`` is a
subclass of ``U``.

See :ref:`about-owned-coercions` for more details and examples.

`shared` Default Intent
-----------------------

The default intent for :record:`shared` types is ``const ref``.

 */
module SharedObject {

  private use ChapelError, Atomics, ChapelBase;
  use OwnedObject;

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

     :record:`shared` manages the deletion of a class instance in a way
     that supports multiple owners of the class instance.

     This is currently implemented with task-safe reference counting.

   */
  pragma "managed pointer"
  record _shared {
    pragma "no doc"
    type chpl_t;         // contained type (class type)

    // contained pointer (class type)
    // uses primitive as a workaround for compiler issues
    pragma "no doc"
    pragma "owned"
    var chpl_p:__primitive("to nilable class", chpl_t);

    // Note that compiler also allows coercion to the borrow type.
    forwarding borrow();

    pragma "no doc"
    pragma "owned"
    var chpl_pn:unmanaged ReferenceCount?; // reference counter

    /*
       Default-initialize a :record:`shared`.
     */
    pragma "leaves this nil"
    proc init(type chpl_t) {
      if !isClass(chpl_t) then
        compilerError("shared only works with classes");

      this.chpl_t = _to_borrowed(chpl_t);
      this.chpl_p = nil;
      this.chpl_pn = nil;
    }

    pragma "no doc"
    proc init(p : borrowed) {
      compilerError("cannot initialize shared from a borrow");
      this.init(_to_unmanaged(p));
    }

    /*
       Initialize a :record:`shared` with a class instance.
       This :record:`shared` will take over the deletion of the class
       instance. It is an error to directly delete the class instance
       while it is managed by :record:`shared`.

       :arg p: the class instance to manage. Must be of unmanaged class type.
     */
    proc init(pragma "nil from arg" p : unmanaged) {
      this.chpl_t = _to_borrowed(p.type);

      // Boost version default-initializes px and pn
      // and then swaps in different values.

      var rc:unmanaged ReferenceCount? = nil;

      if p != nil then
        rc = new unmanaged ReferenceCount();

      this.chpl_p = _to_borrowed(p);
      this.chpl_pn = rc;

      this.complete();

      // Boost includes a mechanism for classes inheriting from
      // enable_shared_from_this to record a weak pointer back to the
      // shared pointer. That would need to be handled in a Phase 2
      // since it would refer to `this` as a whole here.
    }

    pragma "no doc"
    proc init(p: ?T)
    where isClass(T) == false &&
          isSubtype(T, _shared) == false &&
          isIterator(p) == false {
      compilerError("shared only works with classes");
      this.chpl_t = T;
      this.chpl_p = p;
    }

    /*
       Initialize a :record:`shared` taking a pointer from
       a :record:`~OwnedObject.owned`.

       This :record:`shared` will take over the deletion of the class
       instance. It is an error to directly delete the class instance
       while it is managed by :record:`shared`.

       :arg take: the owned value to take ownership from
     */
    proc init(pragma "nil from arg" in take:owned) {
      var p = take.release();
      this.chpl_t = if this.type.chpl_t == ? then _to_borrowed(p.type) else this.type.chpl_t;

      if !isClass(p) then
        compilerError("shared only works with classes");

      var rc:unmanaged ReferenceCount? = nil;

      if p != nil then
        rc = new unmanaged ReferenceCount();

      this.chpl_p = p;
      this.chpl_pn = rc;

      this.complete();
    }

    /* Private move-initializer for use in coercions,
       only makes sense when `src` was already copied in in intent. */
    pragma "no doc"
    proc init(_private: bool, type t, ref src:_shared) {
      this.chpl_t = t;
      this.chpl_p = src.chpl_p:_to_nilable(_to_unmanaged(t));
      this.chpl_pn = src.chpl_pn;

      src.chpl_p = nil;
      src.chpl_pn = nil;
    }

    /* Private initializer for casts. This one increments the reference
       count if the stored pointer is not nil. */
    pragma "no doc"
    proc init(_private: bool, type t, p, pn) {
      var ptr = p:_to_nilable(_to_unmanaged(t));
      var count = pn;
      if ptr != nil {
        // increment the reference count
        count!.retain();
      } else {
        // don't store a count for the nil pointer
        count = nil;
      }

      this.chpl_t = t;
      this.chpl_p = ptr;
      this.chpl_pn = count;
    }


    // Initialize generic 'shared' var-decl from owned:
    //   var s : shared = ownedThing;
    pragma "no doc"
    proc init=(pragma "nil from arg" in take: owned) {
      if isNonNilableClass(this.type) && isNilableClass(take) then
        compilerError("cannot create a non-nilable shared variable from a nilable class instance");

      this.init(take);
    }

    /*
       Copy-initializer. Creates a new :record:`shared`
       that refers to the same class instance as `src`.
       These will share responsibility for managing the instance.
     */
    proc init=(pragma "nil from arg" const ref src:_shared) {
      if isNonNilableClass(this.type) && isNilableClass(src) then
        compilerError("cannot create a non-nilable shared variable from a nilable class instance");

      if isCoercible(src.chpl_t, this.type.chpl_t) == false then
        compilerError("cannot coerce '", src.type:string, "' to '", this.type:string, "' in initialization");

      this.chpl_t = this.type.chpl_t;
      this.chpl_p = src.chpl_p;
      this.chpl_pn = src.chpl_pn;

      this.complete();

      if this.chpl_pn != nil then
        this.chpl_pn!.retain();
    }

    pragma "no doc"
    proc init=(src: borrowed) {
      compilerError("cannot create a shared variable from a borrowed class instance");
      this.chpl_t = int; //dummy
    }

    pragma "no doc"
    proc init=(src: unmanaged) {
      compilerError("cannot create a shared variable from an unmanaged class instance");
      this.chpl_t = int; //dummy
    }

    pragma "no doc"
    pragma "leaves this nil"
    proc init=(src : _nilType) {
      this.init(this.type.chpl_t);

      if isNonNilableClass(chpl_t) then
        compilerError("Assigning non-nilable shared to nil");
    }

    pragma "no doc"
    proc ref doClear() {
      if chpl_p != nil && chpl_pn != nil {
        var count = chpl_pn!.release();
        if count == 0 {
          delete _to_unmanaged(chpl_p);
          delete chpl_pn;
        }
      }
      chpl_p = nil;
      chpl_pn = nil;
    }

    /*
       The deinitializer for :record:`shared` will destroy the class
       instance once there are no longer any copies of this
       :record:`shared` that refer to it.
     */
    proc deinit() {
      if isClass(chpl_p) { // otherwise, let error happen on init call
        doClear();
      }
    }

    /*
       Change the instance managed by this class to `newPtr`.
       If this record was the last :record:`shared` managing a
       non-nil instance, that instance will be deleted.
     */
    proc ref retain(pragma "nil from arg" newPtr:unmanaged) {
      if !isCoercible(newPtr.type, chpl_t) then
        compilerError("cannot retain '" + newPtr.type:string + "' " +
                      "(expected '" + _to_unmanaged(chpl_t):string + "')");

      doClear();
      this.chpl_p = newPtr;
      if newPtr != nil {
        this.chpl_pn = new unmanaged ReferenceCount();
      }
    }

    /*
       Empty this :record:`shared` so that it stores `nil`.
       Deletes the managed object if this :record:`shared` is the
       last :record:`shared` managing that object.
       Does not return a value.

       Equivalent to ``shared.retain(nil)``.
     */
    pragma "leaves this nil"
    proc ref clear() {
      doClear();
    }

    /*
       Return the object managed by this :record:`shared` without
       impacting its lifetime at all. It is an error to use the
       value returned by this function after the last :record:`shared`
       goes out of scope or deletes the contained class instance
       for another reason, including calls to
       `=`, or ``shared.retain`` when this is the last :record:`shared`
       referring to the instance.
       In some cases such errors are caught at compile-time.
     */
    pragma "nil from this"
    proc /*const*/ borrow() {
      if _to_nilable(chpl_t) == chpl_t {
        return chpl_p;
      } else {
        return chpl_p!;
      }
    }

    // = should call retain-release
    // copy-init should call retain
  }


  /*
     Assign one :record:`shared` to another.
     Deletes the object managed by ``lhs`` if there are
     no other :record:`shared` referring to it. On return,
     ``lhs`` will refer to the same object as ``rhs``.
   */
  proc =(ref lhs:_shared, rhs: _shared)
    where ! (isNonNilableClass(lhs) && isNilableClass(rhs))
  {
    // retain-release
    if rhs.chpl_pn != nil then
      rhs.chpl_pn!.retain();
    lhs.doClear();
    lhs.chpl_p = rhs.chpl_p;
    lhs.chpl_pn = rhs.chpl_pn;
  }

  /*
     Set a :record:`shared` from a :record:`~OwnedObject.owned`.
     Deletes the object managed by ``lhs`` if there are
     no other :record:`shared` referring to it.
     On return, ``lhs`` will refer to the object previously
     managed by ``rhs``, and ``rhs`` will refer to `nil`.
   */
  proc =(ref lhs:_shared, in rhs:owned)
    where ! (isNonNilableClass(lhs) && isNilableClass(rhs))
  {
    lhs.retain(rhs.release());
  }

  pragma "no doc"
  proc =(pragma "leaves arg nil" ref lhs:shared, rhs:_nilType)
    where ! isNonNilableClass(lhs)
  {
    lhs.clear();
  }

  /*
     Swap two :record:`shared` objects.
   */
  proc <=>(ref lhs: _shared, ref rhs: _shared) {
    lhs.chpl_pn <=> rhs.chpl_pn;
    lhs.chpl_p <=> rhs.chpl_p;
  }

  // This is a workaround
  pragma "no doc"
  pragma "auto destroy fn"
  proc chpl__autoDestroy(ref x: _shared) {
    __primitive("call destructor", __primitive("deref", x));
  }

  // Don't print out 'chpl_p' when printing an Shared, just print class pointer
  pragma "no doc"
  proc _shared.readWriteThis(f) throws {
    f <~> this.chpl_p;
  }

  // Note, coercion from _shared -> _shared.chpl_t is sometimes directly
  // supported in the compiler via a call to borrow() and
  // sometimes uses this cast.
  pragma "no doc"
  inline proc _cast(type t:borrowed, pragma "nil from arg" const ref x:_shared) where isSubtype(t,x.chpl_t) {
    return x.borrow();
  }

  // cast to shared?, no class downcast
  pragma "no doc"
  inline proc _cast(type t:shared class?, pragma "nil from arg" in x:shared class)
    where isSubtype(x.chpl_t,_to_nonnil(t.chpl_t))
  {
    return new _shared(true, _to_nilable(t.chpl_t), x);
  }

  // cast to shared?, no class downcast
  pragma "no doc"
  inline proc _cast(type t:shared class?, pragma "nil from arg" in x:shared class?)
    where isSubtype(x.chpl_t,t.chpl_t)
  {
    return new _shared(true, t.chpl_t, x);
  }

  // cast to shared!, no class downcast, no casting away nilability
  pragma "no doc"
  inline proc _cast(type t:shared class, in x:shared class)
    where isSubtype(x.chpl_t,t.chpl_t)
  {
    return new _shared(true, t.chpl_t, x);
  }

  // cast to shared!, no class downcast, casting away nilability
  pragma "no doc"
  inline proc _cast(type t:shared class, in x:shared class?) throws
    where isSubtype(_to_nonnil(x.chpl_t),t.chpl_t)
  {
    if x.chpl_p == nil {
      throw new owned NilClassError();
    }

    return new _shared(true, _to_nonnil(t.chpl_t), x);
  }

  // this version handles downcast to non-nil shared
  pragma "no doc"
  inline proc _cast(type t:shared class, const ref x:shared class?) throws
    where isProperSubtype(t.chpl_t,_to_nonnil(x.chpl_t))
  {
    if x.chpl_p == nil {
      throw new owned NilClassError();
    }
    // the following line can throw ClassCastError
    var p = try x.chpl_p:_to_nonnil(_to_unmanaged(t.chpl_t));

    return new _shared(true, _to_borrowed(p.type), p, x.chpl_pn);
  }
  pragma "no doc"
  inline proc _cast(type t:shared class, const ref x:shared class) throws
    where isProperSubtype(t.chpl_t,x.chpl_t)
  {
    // the following line can throw ClassCastError
    var p = try x.chpl_p:_to_nonnil(_to_unmanaged(t.chpl_t));

    return new _shared(true, _to_borrowed(p.type), p, x.chpl_pn);
  }


  // this version handles downcast to nilable shared
  pragma "no doc"
  inline proc _cast(type t:shared class?, pragma "nil from arg" const ref x:shared class?)
    where isProperSubtype(t.chpl_t,x.chpl_t)
  {
    // this cast returns nil if the dynamic type is not compatible
    var p = x.chpl_p:_to_nilable(_to_unmanaged(t.chpl_t));
    return new _shared(true, _to_borrowed(p.type), p, x.chpl_pn);
  }
  pragma "no doc"
  inline proc _cast(type t:shared class?, const ref x:shared class)
    where isProperSubtype(t.chpl_t,_to_nilable(x.chpl_t))
  {
    // this cast returns nil if the dynamic type is not compatible
    var p = x.chpl_p:_to_nilable(_to_unmanaged(t.chpl_t));
    return new _shared(true, _to_borrowed(p.type), p, x.chpl_pn);
  }

  // cast from nil to shared
  pragma "no doc"
  inline proc _cast(type t:_shared, pragma "nil from arg" x:_nilType) {
    if isNonNilableClass(t.chpl_t) then
      compilerError("Illegal cast from nil to non-nilable shared type");

    var tmp:t;
    return tmp;
  }

  pragma "no doc"
  pragma "always propagate line file info"
  inline proc postfix!(x:_shared) {
    use HaltWrappers only;
    // Check only if --nil-checks is enabled
    if chpl_checkNilDereferences {
      // Add check for nilable types only.
      if _to_nilable(x.chpl_t) == x.chpl_t {
        if x.chpl_p == nil {
          HaltWrappers.nilCheckHalt("argument to ! is nil");
        }
      }
    }
    return _to_nonnil(x.chpl_p);
  }
}
