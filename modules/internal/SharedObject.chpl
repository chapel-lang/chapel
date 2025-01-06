/*
 * Copyright 2020-2025 Hewlett Packard Enterprise Development LP
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

/* */
module SharedObject {

  use Errors, Atomics, ChapelBase;
  use OwnedObject;

  private use WeakPointer;

  // TODO unify with RefCountBase. Even though that one is for
  // intrusive ref-counting and this one isn't, there's no fundamental
  // reason it couldn't be one class.
  @chpldoc.nodoc
  class ReferenceCount {
    // the number of 'shared' class variables that point to the allocated data
    var strongCount: atomic int;
    // the number of 'shared' class variables or 'weak' references that point to the data
    var totalCount: atomic int; // weakCount + strongCount

    // ---------------- 'shared' interface ----------------

    // a 'ReferenceCount' should only ever be initialized during 'shared' initialization
    // 'weak' references should only get a non-nil 'ReferenceCount' by copying from a 'shared'
    proc init() {
      init this;
      strongCount.write(1);
      totalCount.write(1);
    }

    // increment the strong reference count
    proc retain() {
      strongCount.add(1);
      totalCount.add(1);
    }

    // decrement the strong reference count and return its new value
    inline proc releaseStrong() {
      return strongCount.fetchSub(1) - 1;
    }

    // decrement the total reference count and return its new value
    inline proc releaseTotal() {
      return totalCount.fetchSub(1) - 1;
    }

    // ---------------- 'weak' interface ----------------

    /* attempt to atomically increment the strong reference count

     - if the current strong-count does not match 'expected', then 'expected'
       is updated to whatever the current value is, and 'false' is returned
     - if they do match, the strong-count is incremented, the total-count is
       incremented, and 'true' is returned.

     This method is used to safely upgrade a 'weak' to a 'shared'
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


  // Ideally, this can be marked with nodoc and the doc put at the module level
  //   since shared isn't really a 'record' or a 'type'
  // But that prevents us from referencing a "top-level" shared reference
  // If we one day support :noindexentry:, that could be applied at the module
  //   level (instead of :noindex:)
  // And then we could do :mod:`shared <SharedObject>`
  // For now, `fixInternalDocs.sh` replaces `.. record:: shared` with `.. type:: shared`
  /*
    :type:`shared` manages the deletion of a class instance in a way
    that supports multiple owners of the class instance.

    This is currently implemented with task-safe reference counting.
  */
  pragma "managed pointer"
  record _shared : writeSerializable, readDeserializable {
    type chpl_t;         // contained type (class type)

    // contained pointer (class type)
    // uses primitive as a workaround for compiler issues
    pragma "owned"
    var chpl_p:__primitive("to nilable class", chpl_t);

    // Note that compiler also allows coercion to the borrow type.
    forwarding borrow();

    pragma "owned"
    var chpl_pn:unmanaged ReferenceCount?; // reference counter
  }

  /* Default-initialize a :type:`shared`. */
  pragma "leaves this nil"
  @chpldoc.nodoc // hide init/record impl details
  proc _shared.init(type chpl_t) {
    // TODO: today (06/15/2024), the compiler has a special check for a non-class type
    // being used to instantiate _shared, so this check is likely redundant and
    // should be removed. See other _shared.init methods for similar checks that
    // are likely also redundant.
    if !isClass(chpl_t) then
      compilerError("shared only works with classes");

    this.chpl_t = _to_borrowed(chpl_t);
    this.chpl_p = nil;
    this.chpl_pn = nil;
  }

  @chpldoc.nodoc
  proc _shared.init(p : borrowed) {
    compilerError("cannot initialize shared from a borrow");
    this.init(_to_unmanaged(p));
  }

  @chpldoc.nodoc
  proc _shared.init(pragma "nil from arg" p : unmanaged) {
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

  @chpldoc.nodoc
  proc _shared.init(p: ?T)
  where isClass(T) == false &&
        isSubtype(T, _shared) == false &&
        isIterator(p) == false {
    compilerError("shared only works with classes");
    this.chpl_t = T;
    this.chpl_p = p;
  }

  /*
    Initialize a :type:`shared` taking a pointer from
    a :type:`~OwnedObject.owned`.

    This :type:`shared` will take over the deletion of the class
    instance. It is an error to directly delete the class instance
    while it is managed by :type:`shared`.

    :arg take: the owned value to take ownership from
  */
  // this init is not user facing
  @chpldoc.nodoc
  proc _shared.init(pragma "nil from arg" in take:owned) {
    var p = take.release();
    this.chpl_t = if this.type.chpl_t == ? then _to_borrowed(p.type) else this.type.chpl_t;

    if !isClass(p) then
      compilerError("shared only works with classes");

    var rc:unmanaged ReferenceCount? = nil;

    if p != nil then
      rc = new unmanaged ReferenceCount();

    this.chpl_p = p;
    this.chpl_pn = rc;

    init this;
  }

  /*
    Private move-initializer for use in coercions,
    only makes sense when `src` was already copied in in intent.
  */
  @chpldoc.nodoc
  proc _shared.init(_private: bool, type t, ref src:_shared) {
    this.chpl_t = t;
    this.chpl_p = _to_unmanaged(src.chpl_p):_to_nilable(_to_unmanaged(t));
    this.chpl_pn = src.chpl_pn;

    src.chpl_p = nil;
    src.chpl_pn = nil;
  }

  /* Private initializer for casts. This one increments the reference
      count if the stored pointer is not nil. */
  @chpldoc.nodoc
  proc _shared.init(_private: bool, type t, p, pn) {
    var ptr = _to_unmanaged(p):_to_nilable(_to_unmanaged(t));
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

  /*
    Private initializer for casting from weak to shared.
    assumes the shared reference count in 'pn' has already been incremented
  */
  @chpldoc.nodoc
  proc _shared.init(_private: bool, pragma "nil from arg" p: unmanaged, pn) {
    this.chpl_t = _to_borrowed(p.type);
    this.chpl_p = p;
    this.chpl_pn = pn;
  }

@chpldoc.nodoc
proc _shared.init=(pragma "nil from arg" in take: owned) {
  compilerError("cannot initialize '", this.type:string, "' from a '", take.type:string, "'");
  this.chpl_t = take.chpl_t;
  this.chpl_p = nil;
}

  /*
    Copy-initializer. Creates a new :type:`shared`
    that refers to the same class instance as `src`.
    These will share responsibility for managing the instance.
  */
  proc _shared.init=(pragma "nil from arg" const ref src:_shared) {
    this.chpl_t = if this.type.chpl_t != ?
                  then this.type.chpl_t
                  else _to_borrowed(src.type);

    if isCoercible(src.chpl_t, this.type.chpl_t) == false then
      compilerError("cannot initialize '", this.type:string, "' from a '", src.type:string, "'");

    this.chpl_p = src.chpl_p;
    this.chpl_pn = src.chpl_pn;

    init this;

    if this.chpl_pn != nil then
      this.chpl_pn!.retain();

    if isNonNilableClass(this.type) && isNilableClass(src) then
      compilerError("cannot initialize '", this.type:string, "' from a '", src.type:string, "'");

  }

  @chpldoc.nodoc
  proc _shared.init=(src: borrowed) {
    compilerError("cannot initialize '", this.type:string, "' from a '", src.type:string, "'");

    this.chpl_t = if this.type.chpl_t != ?
                  then this.type.chpl_t
                  else _to_borrowed(src.type);
  }

  @chpldoc.nodoc
  proc _shared.init=(src: unmanaged) {
    compilerError("cannot initialize '", this.type:string, "' from a '", src.type:string, "'");
    this.chpl_t = if this.type.chpl_t != ?
                  then this.type.chpl_t
                  else _to_borrowed(src.type);
  }

  pragma "leaves this nil"
  @chpldoc.nodoc
  proc _shared.init=(src : _nilType) {
    if this.type.chpl_t == ? then
      compilerError("cannot establish type of shared when initializing with 'nil'");

    this.init(this.type.chpl_t);

    if isNonNilableClass(chpl_t) then
      compilerError("cannot initialize '", this.type:string, "' from 'nil'");
  }

  @chpldoc.nodoc
  proc ref _shared.doClear() {
    if chpl_p != nil && chpl_pn != nil {
      const sc = chpl_pn!.releaseStrong();
      if sc == 0 then
        delete _to_unmanaged(chpl_p);

      const tc = chpl_pn!.releaseTotal();
      if tc == 0 then
        delete chpl_pn;
    }
    chpl_p = nil;
    chpl_pn = nil;
  }

  // Issue a compiler error for illegal uses.
  @chpldoc.nodoc
  proc type _shared.adopt(source) {
    compilerError("cannot adopt a ", source.type:string);
  }

  /*
    Changes the memory management strategy of the argument from `owned`
    to `shared`, taking over the ownership of the argument.
    The result type preserves nilability of the argument type.
    If the argument is non-nilable, it must be recognized by the compiler
    as an expiring value.
  */
  inline proc type _shared.adopt(pragma "nil from arg" in obj: owned) {
    var ptr = owned.release(obj);
    return shared.adopt(ptr);
  }

  /*
    Starts managing the argument class instance `obj`
    using the `shared` memory management strategy.
    The result type preserves nilability of the argument type.

    It is an error to directly delete the class instance
    after passing it to `shared.adopt()`.
  */
  inline proc type _shared.adopt(pragma "nil from arg" in obj: unmanaged) {
    return new _shared(obj);
  }

  /*
    The deinitializer for :type:`shared` will destroy the class
    instance once there are no longer any copies of this
    :type:`shared` that refer to it.
  */
  proc ref _shared.deinit() {
    if isClass(chpl_p) { // otherwise, let error happen on init call
      doClear();
    }
  }

  /*
    Return the object managed by this :type:`shared` without
    impacting its lifetime at all. It is an error to use the
    value returned by this function after the last :type:`shared`
    goes out of scope or deletes the contained class instance
    for another reason, including calls to
    `=`, or ``shared.retain`` when this is the last :type:`shared`
    referring to the instance.
    In some cases such errors are caught at compile-time.
  */
  pragma "nil from this"
  proc /*const*/ _shared.borrow() {
    if _to_nilable(chpl_t) == chpl_t {
      return chpl_p;
    } else {
      return chpl_p!;
    }
  }

  /* Create a :record:`~WeakPointer.weak` reference to this object */
  @unstable("The `weak` type is experimental; expect this method to change in the future.")
  proc _shared.downgrade() {
    return new WeakPointer.weak(this);
  }

  // = should call retain-release
  // copy-init should call retain
  /*
     Assign one :type:`shared` to another.
     Deletes the object managed by ``lhs`` if there are
     no other :type:`shared` referring to it. On return,
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

 @chpldoc.nodoc
 operator =(ref lhs:_shared, in rhs:owned)
  do compilerError("cannot assign '", rhs.type:string, "' to a '", lhs.type:string, "'");

  @chpldoc.nodoc
  operator =(pragma "leaves arg nil" ref lhs:shared, rhs:_nilType)
  {
    lhs.doClear();
  }

  /*
     Swap two :type:`shared` objects.
   */
  operator <=>(ref lhs: _shared, ref rhs: _shared) {
    lhs.chpl_pn <=> rhs.chpl_pn;
    lhs.chpl_p <=> rhs.chpl_p;
  }

  // This is a workaround
  pragma "auto destroy fn"
  proc chpl__autoDestroy(const ref x: _shared) {
    __primitive("call destructor", __primitive("deref", x));
  }

  @chpldoc.nodoc
  proc ref _shared.deserialize(reader, ref deserializer) throws {
    _readWriteHelper(reader);
  }

  @chpldoc.nodoc
  proc _shared.serialize(writer, ref serializer) throws {
    _readWriteHelper(writer);
  }

  // Don't print out 'chpl_p' when printing an Shared, just print class pointer
  @chpldoc.nodoc
  proc _shared._readWriteHelper(f) throws {
    if isNonNilableClass(this.chpl_t) {
      var tmp = this.chpl_p! : borrowed class;
      if f._writing then f.write(tmp); else tmp = f.read(tmp.type);
      if tmp == nil then halt("internal error - read nil");
      if tmp != this.chpl_p then halt("internal error - read changed ptr");
    } else {
      var tmp = this.chpl_p : borrowed class?;
      if f._writing then f.write(tmp); else tmp = f.read(tmp.type);
      if tmp != this.chpl_p then halt("internal error - read changed ptr");
      if tmp == nil then
        this.doClear();
    }
  }

  // Note, coercion from _shared -> _shared.chpl_t is sometimes directly
  // supported in the compiler via a call to borrow() and
  // sometimes uses this cast.
  @chpldoc.nodoc
  inline operator :(pragma "nil from arg" const ref x:_shared, type t:borrowed)
    where isSubtype(t, x.chpl_t) {
    return x.borrow();
  }

  // cast to shared?, no class downcast
  @chpldoc.nodoc
  inline operator :(pragma "nil from arg" in x:shared class, type t:shared class?)
    where isSubtype(x.chpl_t, t.chpl_t:class)
  {
    return new _shared(true, _to_nilable(t.chpl_t), x);
  }

  // cast to shared?, no class downcast
  @chpldoc.nodoc
  inline operator :(pragma "nil from arg" in x:shared class?, type t:shared class?)
    where isSubtype(x.chpl_t, t.chpl_t)
  {
    return new _shared(true, t.chpl_t, x);
  }

  // cast to shared!, no class downcast, no casting away nilability
  @chpldoc.nodoc
  inline operator :(in x:shared class, type t:shared class)
    where isSubtype(x.chpl_t, t.chpl_t)
  {
    return new _shared(true, t.chpl_t, x);
  }

  // cast to shared!, no class downcast, casting away nilability
  @chpldoc.nodoc
  inline operator :(in x:shared class?, type t:shared class) throws
    where isSubtype(x.chpl_t:class, t.chpl_t)
  {
    if x.chpl_p == nil {
      throw new owned NilClassError();
    }

    return new _shared(true, _to_nonnil(t.chpl_t), x);
  }

  // this version handles downcast to non-nil shared
  @chpldoc.nodoc
  inline operator :(const ref x:shared class?, type t:shared class) throws
    where isProperSubtype(t.chpl_t, x.chpl_t:class)
  {
    if x.chpl_p == nil {
      throw new owned NilClassError();
    }
    // the following line can throw ClassCastError
    var p = try x.chpl_p:_to_nonnil(_to_unmanaged(t.chpl_t));

    return new _shared(true, _to_borrowed(p.type), p, x.chpl_pn);
  }
  @chpldoc.nodoc
  inline operator :(const ref x:shared class, type t:shared class) throws
    where isProperSubtype(t.chpl_t, x.chpl_t)
  {
    // the following line can throw ClassCastError
    var p = try x.chpl_p:_to_nonnil(_to_unmanaged(t.chpl_t));

    return new _shared(true, _to_borrowed(p.type), p, x.chpl_pn);
  }


  // this version handles downcast to nilable shared
  @chpldoc.nodoc
  inline operator :(pragma "nil from arg" const ref x:shared class?, type t:shared class?)
    where isProperSubtype(t.chpl_t, x.chpl_t)
  {
    // this cast returns nil if the dynamic type is not compatible
    var p = x.chpl_p:_to_nilable(_to_unmanaged(t.chpl_t));
    return new _shared(true, _to_borrowed(p.type), p, x.chpl_pn);
  }
  @chpldoc.nodoc
  inline operator :(const ref x:shared class, type t:shared class?)
    where isProperSubtype(t.chpl_t, x.chpl_t:class?)
  {
    // this cast returns nil if the dynamic type is not compatible
    var p = x.chpl_p:_to_nilable(_to_unmanaged(t.chpl_t));
    return new _shared(true, _to_borrowed(p.type), p, x.chpl_pn);
  }

  // cast from nil to shared
  @chpldoc.nodoc
  inline operator :(pragma "nil from arg" x:_nilType, type t:_shared)  {
    if isNonNilableClass(t.chpl_t) then
      compilerError("Illegal cast from nil to non-nilable shared type");
    if isGenericType(t) then
      compilerError("illegal cast from nil to a generic shared type");

    var tmp:t;
    return tmp;
  }

  pragma "always propagate line file info"
  @chpldoc.nodoc
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
