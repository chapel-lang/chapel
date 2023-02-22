/*
 * Copyright 2020-2023 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
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
 */
module SharedObject {

  use Errors, Atomics, ChapelBase;
  use OwnedObject;

  private use WeakPointer;

  // TODO unify with RefCountBase. Even though that one is for
  // intrusive ref-counting and this one isn't, there's no fundamental
  // reason it couldn't be one class.
  pragma "no doc"
  class ReferenceCount {
    // the number of 'shared' class variables that point to the allocated data
    var strongCount: atomic int;
    // the number of 'shared' class variables or 'weakPointer's that point to the data
    var totalCount: atomic int; // weakCount + strongCount

    // ---------------- 'shared' interface ----------------

    // a 'ReferenceCount' should only ever be initialized during 'shared' initialization
    // 'weakPointer's should only get a non-nil 'ReferenceCount' by copying from a 'shared'
    proc init() {
      this.complete();
      strongCount.write(1);
      totalCount.write(1);
    }

    // increment the strong reference count
    proc retain() {
      strongCount.add(1);
      totalCount.add(1);
    }

    // decrement the strong reference count and return the new strong- and total-counts
    proc release() {
      var oldValue = strongCount.fetchSub(1);
      return (oldValue - 1, totalCount.fetchSub(1) - 1);
    }


    // ---------------- 'weakPointer' interface ----------------

    /* attempt to atomically increment the strong reference count

     - if the current strong-count does not match 'expected', then 'expected'
       is updated to whatever the current value is, and 'false' is returned
     - if they do match, the strong-count is incremented, the total-count is
       incremented, and 'true' is returned.

     This method is used to safely upgrade a 'weakPointer' to a 'shared'
     reference. This is done by calling the method in a while-loop that can
     either fail if the expected value drops to zero (i.e., the last 'shared'
     was dropped by someone else during the upgrade attempt), or loop until
     the value can be incremented safely (s.t. there are no conflicts
     between concurrent upgrades)
    */
    proc tryRetainWeak(ref expected: int) {
      const next = expected + 1;
      //TODO: compare performance of compareExchange and compareExchangeWeak
      if strongCount.compareExchangeWeak(expected, next) {
        totalCount.add(1);
        return true;
      } else {
        return false;
      }
    }

    // decrement the weak-count and return the new total-count
    proc releaseWeak() {
      var oldValue = totalCount.fetchSub(1);
      return oldValue - 1;
    }

    // increment the weak-count
    proc incrementWeak() {
      totalCount.add(1);
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

    pragma "no doc"
    proc init(pragma "nil from arg" p : unmanaged) {
      this.chpl_t = _to_borrowed(p.type);

      // Boost version default-initializes px and pn
      // and then swaps in different values.

      var rc:unmanaged ReferenceCount? = nil;

      if p != nil then
        rc = new unmanaged ReferenceCount();

      this.chpl_p = _to_borrowed(p);
      this.chpl_pn = rc;

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

    /* Private initializer for casting from weak to shared.
      assumes the shared reference count in 'pn' has already been incremented */
    pragma "no doc"
    proc init(_private: bool, pragma "nil from arg" p: unmanaged, pn) {
      this.chpl_t = _to_borrowed(p.type);
      this.chpl_p = p;
      this.chpl_pn = pn;
    }

    // Initialize generic 'shared' var-decl from owned:
    //   var s : shared = ownedThing;
    pragma "no doc"
    deprecated "assigning owned class to shared class is deprecated."
    proc init=(pragma "nil from arg" in take: owned) {
      var p = take.release();

      this.chpl_t = if this.type.chpl_t != ?
                    then this.type.chpl_t
                    else _to_borrowed(p.type);

      var rc:unmanaged ReferenceCount? = nil;

      if p != nil then
        rc = new unmanaged ReferenceCount();

      this.chpl_p = p;
      this.chpl_pn = rc;

      this.complete();

      if isNonNilableClass(this.type) && isNilableClass(take) then
        compilerError("cannot initialize '", this.type:string, "' from a '", take.type:string, "'");
    }

    /*
       Copy-initializer. Creates a new :record:`shared`
       that refers to the same class instance as `src`.
       These will share responsibility for managing the instance.
     */
    proc init=(pragma "nil from arg" const ref src:_shared) {
      this.chpl_t = if this.type.chpl_t != ?
                    then this.type.chpl_t
                    else _to_borrowed(src.type);

      if isCoercible(src.chpl_t, this.type.chpl_t) == false then
        compilerError("cannot initialize '", this.type:string, "' from a '", src.type:string, "'");

      this.chpl_p = src.chpl_p;
      this.chpl_pn = src.chpl_pn;

      this.complete();

      if this.chpl_pn != nil then
        this.chpl_pn!.retain();

      if isNonNilableClass(this.type) && isNilableClass(src) then
        compilerError("cannot initialize '", this.type:string, "' from a '", src.type:string, "'");

    }

    pragma "no doc"
    proc init=(src: borrowed) {
      compilerError("cannot initialize '", this.type:string, "' from a '", src.type:string, "'");

      this.chpl_t = if this.type.chpl_t != ?
                    then this.type.chpl_t
                    else _to_borrowed(src.type);
    }

    pragma "no doc"
    proc init=(src: unmanaged) {
      compilerError("cannot initialize '", this.type:string, "' from a '", src.type:string, "'");
      this.chpl_t = if this.type.chpl_t != ?
                    then this.type.chpl_t
                    else _to_borrowed(src.type);
    }

    pragma "no doc"
    pragma "leaves this nil"
    proc init=(src : _nilType) {
      if this.type.chpl_t == ? then
        compilerError("cannot establish type of shared when initializing with 'nil'");

      this.init(this.type.chpl_t);

      if isNonNilableClass(chpl_t) then
        compilerError("cannot initialize '", this.type:string, "' from 'nil'");
    }

    pragma "no doc"
    proc ref doClear() {
      if chpl_p != nil && chpl_pn != nil {
        var (strongCount, totalCount) = chpl_pn!.release();
        if strongCount == 0 {
          // this is the last strong pointer, free the underlying class
          delete _to_unmanaged(chpl_p);
          if totalCount == 0 {
            // There are no weak pointers, free the reference counter too
            delete chpl_pn;
          }
        }
      }
      chpl_p = nil;
      chpl_pn = nil;
    }

    // Issue a compiler error for illegal uses.
    pragma "no doc"
    proc type create(source) {
      compilerError("cannot create a 'shared' from ", source.type:string);
    }

    /* Changes the memory management strategy of the argument from `owned`
       to `shared`, taking over the ownership of the argument.
       The result type preserves nilability of the argument type.
       If the argument is non-nilable, it must be recognized by the compiler
       as an expiring value. */
    inline proc type create(pragma "nil from arg" in take: owned) {
      var result : shared = take;
      return result;
    }

    /* Creates a new `shared` class reference to the argument.
       The result has the same type as the argument. */
    inline proc type create(pragma "nil from arg" in src: shared) {
      return src;
    }

    /* Starts managing the argument class instance `p`
       using the `shared` memory management strategy.
       The result type preserves nilability of the argument type.

       It is an error to directly delete the class instance
       after passing it to `shared.create()`. */
    pragma "unsafe"
    inline proc type create(pragma "nil from arg" p : unmanaged) {
      // 'result' may have a non-nilable type
      var result: (p.type : shared);
      result.retain(p);
      return result;
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

    /*
      Create a :record:`~WeakPointer.weakPointer` to this object
    */
    @unstable "the 'weakPointer' interface is experimental - this method is likely to change in the future"
    proc downgrade() {
      return new WeakPointer.weakPointer(this);
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
  operator =(ref lhs:_shared, rhs: _shared)
    where ! (isNonNilableClass(lhs) && isNilableClass(rhs))
  {
    // retain-release
    if rhs.chpl_pn != nil then
      rhs.chpl_pn!.retain();
    const chpl_p_tmp = rhs.chpl_p;
    const chpl_pn_tmp = rhs.chpl_pn;
    lhs.doClear();
    lhs.chpl_p = chpl_p_tmp;
    lhs.chpl_pn = chpl_pn_tmp;
  }

  /*
     Set a :record:`shared` from a :record:`~OwnedObject.owned`.
     Deletes the object managed by ``lhs`` if there are
     no other :record:`shared` referring to it.
     On return, ``lhs`` will refer to the object previously
     managed by ``rhs``, and ``rhs`` will refer to `nil`.
   */
  deprecated "assignment from an owned class to a shared class is deprecated"
  operator =(ref lhs:_shared, in rhs:owned)
    where ! (isNonNilableClass(lhs) && isNilableClass(rhs))
  {
    lhs.retain(rhs.release());
  }

  pragma "no doc"
  operator =(pragma "leaves arg nil" ref lhs:shared, rhs:_nilType)
    where ! isNonNilableClass(lhs)
  {
    lhs.clear();
  }

  /*
     Swap two :record:`shared` objects.
   */
  operator <=>(ref lhs: _shared, ref rhs: _shared) {
    lhs.chpl_pn <=> rhs.chpl_pn;
    lhs.chpl_p <=> rhs.chpl_p;
  }

  // This is a workaround
  pragma "no doc"
  pragma "auto destroy fn"
  proc chpl__autoDestroy(ref x: _shared) {
    __primitive("call destructor", __primitive("deref", x));
  }

  pragma "no doc"
  proc _shared.readThis(f) throws {
    _readWriteHelper(f);
  }

  pragma "no doc"
  proc _shared.writeThis(f) throws {
    _readWriteHelper(f);
  }

  // Don't print out 'chpl_p' when printing an Shared, just print class pointer
  pragma "no doc"
  proc _shared._readWriteHelper(f) throws {
    if isNonNilableClass(this.chpl_t) {
      var tmp = this.chpl_p! : borrowed class;
      if f.writing then f.write(tmp); else tmp = f.read(tmp.type);
      if tmp == nil then halt("internal error - read nil");
      if tmp != this.chpl_p then halt("internal error - read changed ptr");
    } else {
      var tmp = this.chpl_p : borrowed class?;
      if f.writing then f.write(tmp); else tmp = f.read(tmp.type);
      if tmp != this.chpl_p then halt("internal error - read changed ptr");
      if tmp == nil then
        this.doClear();
    }
  }

  // Note, coercion from _shared -> _shared.chpl_t is sometimes directly
  // supported in the compiler via a call to borrow() and
  // sometimes uses this cast.
  pragma "no doc"
  inline operator :(pragma "nil from arg" const ref x:_shared, type t:borrowed) where isSubtype(t,x.chpl_t) {
    return x.borrow();
  }

  // cast to shared?, no class downcast
  pragma "no doc"
  inline operator :(pragma "nil from arg" in x:shared class, type t:shared class?)
    where isSubtype(x.chpl_t,_to_nonnil(t.chpl_t))
  {
    return new _shared(true, _to_nilable(t.chpl_t), x);
  }

  // cast to shared?, no class downcast
  pragma "no doc"
  inline operator :(pragma "nil from arg" in x:shared class?, type t:shared class?)
    where isSubtype(x.chpl_t,t.chpl_t)
  {
    return new _shared(true, t.chpl_t, x);
  }

  // cast to shared!, no class downcast, no casting away nilability
  pragma "no doc"
  inline operator :(in x:shared class, type t:shared class)
    where isSubtype(x.chpl_t,t.chpl_t)
  {
    return new _shared(true, t.chpl_t, x);
  }

  // cast to shared!, no class downcast, casting away nilability
  pragma "no doc"
  inline operator :(in x:shared class?, type t:shared class) throws
    where isSubtype(_to_nonnil(x.chpl_t),t.chpl_t)
  {
    if x.chpl_p == nil {
      throw new owned NilClassError();
    }

    return new _shared(true, _to_nonnil(t.chpl_t), x);
  }

  // this version handles downcast to non-nil shared
  pragma "no doc"
  inline operator :(const ref x:shared class?, type t:shared class) throws
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
  inline operator :(const ref x:shared class, type t:shared class) throws
    where isProperSubtype(t.chpl_t,x.chpl_t)
  {
    // the following line can throw ClassCastError
    var p = try x.chpl_p:_to_nonnil(_to_unmanaged(t.chpl_t));

    return new _shared(true, _to_borrowed(p.type), p, x.chpl_pn);
  }


  // this version handles downcast to nilable shared
  pragma "no doc"
  inline operator :(pragma "nil from arg" const ref x:shared class?, type t:shared class?)
    where isProperSubtype(t.chpl_t,x.chpl_t)
  {
    // this cast returns nil if the dynamic type is not compatible
    var p = x.chpl_p:_to_nilable(_to_unmanaged(t.chpl_t));
    return new _shared(true, _to_borrowed(p.type), p, x.chpl_pn);
  }
  pragma "no doc"
  inline operator :(const ref x:shared class, type t:shared class?)
    where isProperSubtype(t.chpl_t,_to_nilable(x.chpl_t))
  {
    // this cast returns nil if the dynamic type is not compatible
    var p = x.chpl_p:_to_nilable(_to_unmanaged(t.chpl_t));
    return new _shared(true, _to_borrowed(p.type), p, x.chpl_pn);
  }

  // cast from nil to shared
  pragma "no doc"
  inline operator :(pragma "nil from arg" x:_nilType, type t:_shared)  {
    if isNonNilableClass(t.chpl_t) then
      compilerError("Illegal cast from nil to non-nilable shared type");
    if isGenericType(t) then
      compilerError("illegal cast from nil to a generic shared type");

    var tmp:t;
    return tmp;
  }

  // cast from owned to shared
  pragma "no doc"
  inline operator :(pragma "nil from arg" pragma "leaves arg nil" in x:owned, type t:_shared) {
    if t.chpl_t != ? && t.chpl_t != x.chpl_t then
      compilerError("Cannot change class type in conversion from '",
                    x.type:string, "' to '", t:string, "'");

    var p = x.release();
    var rc: unmanaged ReferenceCount? = nil;
    if p != nil then
      rc = new unmanaged ReferenceCount();

    var tmp: shared t.chpl_t?;
    tmp.chpl_p = p;
    tmp.chpl_pn = rc;

    return try! tmp:shared t.chpl_t;
  }

  pragma "no doc"
  pragma "always propagate line file info"
  inline proc postfix!(x:_shared) {
    import HaltWrappers;
    // Check only if --nil-checks is enabled or user requested
    if chpl_checkNilDereferences || enablePostfixBangChecks {
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

/*
The ``weakPointer`` type is a special smart pointer type designed to be used in
tandem with :record:`~SharedObject.shared` objects.

A ``weakPointer`` provides a reference to a ``shared`` class object without
requiring it to stay allocated. Such a pattern is useful for implementing graph
or tree structures with bidirectional references, or for implementing cache-like
data structures that maintain a list of objects but don't require them to stay
allocated.

A "strong" shared reference to the relevant class object can be obtained via
the :proc:`~WeakPointer.weakPointer.upgrade` method, or by casting the
weakPointer to a ``shared t`` or a ``shared t?``. If the underlying object is
not valid (i.e., its shared reference count has already dropped to zero
causing it to be de-initialized) the upgrade attempt will fail.

Weak pointers are implemented using task-safe reference counting.

.. Warning::
  The ``weakPointer`` type is experimental, please use this feature with caution.

*/
module WeakPointer {
  use Errors, Atomics, ChapelBase;

  record weakPointer {
    /* The shared class type referenced by this pointer */
    type classType;

    pragma "no doc"
    pragma "owned"
    var chpl_p: __primitive("to nilable class", _to_unmanaged(classType)); // instance pointer

    pragma "no doc"
    pragma "owned"
    var chpl_pn: unmanaged ReferenceCount?; // reference counter

    // ---------------- Initializers ----------------

    pragma "no doc"
    proc init(c : unmanaged) {
      this.classType = c.type;
      compilerError(
        "cannot initialize a weakPointer from an unmanaged class: '" + c.type:string + "'"
      );
    }

    pragma "no doc"
    proc init(c : owned) {
      this.classType = c.type;
      compilerError(
        "cannot initialize a weakPointer from an owned class: '" + c.type:string + "'"
      );
    }

    pragma "no doc"
    proc init(c : borrowed) {
      this.classType = c.type;
      compilerError(
        "cannot initialize a weakPointer from a borrowed class: '" + c.type:string + "'"
      );
    }

    // disallow initialization from all other types
    pragma "no doc"
    proc init(c) {
      this.classType = c.type;
      compilerError("cannot initialize a weakPointer from: '" + c.type:string + "'");
    }

    /*
        Create a new weak pointer to a shared class instance 'c'
    */
    @unstable "The 'weakPointer' type is experimental and is likely to change in the future"
    proc init(c : shared) {
        var ptr = c.chpl_p: _to_nilable(_to_unmanaged(c.chpl_t));
        var count = c.chpl_pn;

        // increment the weak reference count (or store nil if the class is nil)
        if ptr != nil then count!.incrementWeak(); else count = nil;

        this.classType = shared c.chpl_t;
        // this.complete();

        this.chpl_p = ptr;
        this.chpl_pn = count;
    }

    /*
        Copy-initialize a new ``weakPointer`` from an existing ``weakPointer``.

        Increments the weak-reference count.
    */
    proc init=(pragma "nil from arg" const ref src: weakPointer) {
      this.classType = src.classType;

      if src.chpl_p!= nil {
        this.chpl_p = src.chpl_p;
        src.chpl_pn!.incrementWeak();
        this.chpl_pn = src.chpl_pn;
      } else {
        this.chpl_p = nil;
        this.chpl_pn = nil;
      }
    }

    // type-only constructor for array initialization, etc.
    pragma "no doc"
    proc init(type classType: shared) {
      if !isClass(classType) then
        compilerError("'weakPointer' only works with shared classes");
      this.classType = classType;
      this.chpl_p = nil;
      this.chpl_pn = nil;
    }

    // ---------------- Other ----------------

    /*
      Attempt to recover a shared object from this `weakPointer`

      If the pointer is valid (i.e., at least one `shared` reference
      to the data exists), a nilable `shared` object will be returned.

      If the pointer is invalid (or the object itself is `nil`) then a
      `nil` value will be returned.
    */
    proc upgrade(): this.classType? {
      if this.chpl_p != nil {
        var sc = this.chpl_pn!.strongCount.read();
        if sc == 0 {
            return nil;
        } else {
            while !this.chpl_pn!.tryRetainWeak(sc) {
                if sc == 0 {
                    return nil;
                }
            }
            var result: this.classType?;
            result.chpl_p = this.chpl_p;
            result.chpl_pn = this.chpl_pn;
            return result;
        }
      } else {
          return nil;
      }
    }

    /*
      When a ``weakPointer`` is deinitialized, the weak reference count is
      decremented.

      If there are no other references (weak or strong), the backing pointer
      is freed.
    */
    proc deinit() {
      this.doClear();
    }

    pragma "no doc"
    proc doClear() {
      if this.chpl_p != nil {
          const totalCount = this.chpl_pn!.releaseWeak();
          if totalCount == 0 then delete this.chpl_pn;
      }
      this.chpl_p = nil;
      this.chpl_pn = nil;
    }

    /*
      Get the number of ``weakPointers`` currently pointing at the same
      ``shared`` class as this one.
    */
    proc getWeakCount(): int {
      if const counts = this.chpl_pn
        then return (counts.totalCount.read() - counts.strongCount.read());
        else return 0;
    }

    /*
      Get the number of ``shared`` variables currently pointing at the same
      ``shared`` class as this ``weakPointer``

      .. Warning
        this value should not be used to predict whether this pointer
        can successfully be cast to a ``shared`` class. Even if the value
        is greater than zero, it is possible for all the other ``shared``
        references to deinitialize the class instance before this weak
        pointer can be upgraded.
    */
    proc getStrongCount(): int {
      if const counts = this.chpl_pn
        then return counts.strongCount.read();
        else return 0;
    }
  }


  // ---------------- Cast Operators ----------------

  // TODO, add "nil from arg" pragmas where necessary

  /*
      Cast a weak pointer to a nilable class type.

      If the referenced class has already been deinitialized, or is
      itself ``nil``, this cast will return a ``nil`` value.

      Otherwise it will return a nilable :record:`~SharedObject.shared`
      ``t``.
  */
  inline operator :(const ref x: weakPointer, type t: shared class?)
    where isSubtype(_to_nonnil(x.classType), _to_nonnil(t.chpl_t))
  {
    if x.chpl_p != nil {
      var sc = x.chpl_pn!.strongCount.read();
      if sc == 0 {
        // the class value has already been deinitialized
        return nil;
      } else {
        while !x.chpl_pn!.tryRetainWeak(sc) {
          if sc == 0 {
            // the class value was deinitialized while this process
            // was trying to increment the strong reference count
            return nil;
          }
        }
        // otherwise, the strong-count was successfully incremented
        var result: t;
        result.chpl_p = x.chpl_p;
        result.chpl_pn = x.chpl_pn;
        return result;
      }
    } else {
      // class value itself was nil
      return nil;
    }
  }

  /*
      Cast a weak pointer to a non-nilable class type.

      If the referenced class has already been deinitialized, or is
      itself ``nil``, this cast will throw a :class:`~Errors.NilClassError`.

      Otherwise it will return a :record:`~SharedObject.shared` ``t``.
  */
  inline operator :(const ref x: weakPointer, type t: shared class) throws
    where isSubtype(_to_nonnil(x.classType), t.chpl_t)
  {
    if x.chpl_p != nil {
        var sc = x.chpl_pn!.strongCount.read();
        if sc == 0 {
          // the class value has already been deinitialized
          throw new NilClassError();
        } else {
          while !x.chpl_pn!.tryRetainWeak(sc) {
            if sc == 0 {
              // the class value was deinitialized while this process
              // was trying to increment the strong reference count
              throw new NilClassError();
            }
          }
          // otherwise, the strong-count was successfully incremented
          return new _shared(true, x.chpl_p!, x.chpl_pn);
        }
    } else {
      // class value itself was nil
      throw new NilClassError();
    }
  }

  // ---------------- Other Operators ----------------

  /*
      Assign one existing ``weakPointer`` to an other.

      Decrements the weak-reference count of the ``lhs`` pointer.

      This will result in the deinitialization of the ``lhs``'s backing
      pointer if it is the last ``weakPointer`` or ``shared`` that points
      to its object.
  */
  inline operator =(ref lhs: weakPointer, rhs: weakPointer)
    where !(isNonNilableClass(lhs) && isNilableClass(rhs))
  {
    if rhs.chpl_pn != nil then rhs.chpl_pn!.incrementWeak();
    const chpl_p_tmp = rhs.chpl_p;
    const chpl_pn_tmp = rhs.chpl_pn;

    lhs.doClear();
    lhs.chpl_p = chpl_p_tmp;
    lhs.chpl_pn = chpl_pn_tmp;
  }

  proc weakPointer.writeThis(ch) throws {
    if const ptr = this.chpl_p {
      if this.chpl_pn!.strongCount.read() > 0 {
        // ptr could be invalidated between /\ and \/ (not worrying about that for now).
        ch.write(ptr);
      } else {
        ch.write("invalid-ptr");
      }
    } else {
      ch.write("nil-object");
    }
  }

}
