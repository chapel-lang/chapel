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
module OwnedObject {
  use ChapelStandard;

  /*
     :record:`owned` manages the deletion of a class instance assuming
     that this :record:`owned` is the only thing responsible for managing
     the lifetime of the class instance.
   */
  pragma "no copy"
  pragma "copy mutates"
  pragma "managed pointer"
  record _owned {
    @chpldoc.nodoc
    type chpl_t;                // contained type (class type)

    // contained pointer (class type)
    // uses primitive as a workaround for compiler issues
    pragma "owned"
    @chpldoc.nodoc
    var chpl_p:__primitive("to nilable class", chpl_t);

    // Note that the compiler also allows coercion to the borrow type.
    forwarding borrow();

    /*
       Default-initialize a :record:`owned` to store type `chpl_t`
     */
    pragma "leaves this nil"
    proc init(type chpl_t) {
      if !isClass(chpl_t) then
        compilerError("owned only works with classes");

      this.chpl_t = _to_borrowed(chpl_t);
      this.chpl_p = nil;
    }

    @chpldoc.nodoc
    proc init(p:borrowed) {
      compilerError("cannot initialize owned from a borrow");
      this.init(_to_unmanaged(p));
    }

    @chpldoc.nodoc
    proc init(pragma "nil from arg" p:unmanaged) {
      this.chpl_t = _to_borrowed(p.type);
      this.chpl_p = _to_borrowed(p);
    }

    @chpldoc.nodoc
    proc init(p:?T) where isClass(T) == false &&
                          isSubtype(T, _owned) == false  &&
                          isIterator(p) == false {
      compilerError("owned only works with classes");
      this.chpl_t = T;
      this.chpl_p = p;
    }

    /*
       Copy-initializer. Creates a new :record:`owned`
       that takes over ownership from `src`. `src` will
       refer to `nil` after this call.
     */
    proc init=(pragma "leaves arg nil" pragma "nil from arg" ref src:_owned) {
      // Use 'this.type.chpl_t' if it is set in case RHS is a subtype
      this.chpl_t = if this.type.chpl_t != ?
                    then this.type.chpl_t
                    else _to_borrowed(src.type);

      if isCoercible(src.chpl_t, this.type.chpl_t) == false then
        compilerError("cannot initialize '", this.type:string, "' from a '", src.type:string, "'");

      this.chpl_p = owned.release(src);
      this.complete();

      if isNonNilableClass(this.type) && isNilableClass(src) then
        compilerError("cannot initialize '", this.type:string, "' from a '", src.type:string, "'");
    }

    @chpldoc.nodoc
    proc init=(src: shared) {
      compilerError("cannot initialize '", this.type:string, "' from a '", src.type:string, "'");
      this.chpl_t = if this.type.chpl_t != ?
                    then this.type.chpl_t
                    else _to_borrowed(src.type);
    }

    @chpldoc.nodoc
    proc init=(src: borrowed) {
      compilerError("cannot initialize '", this.type:string, "' from a '", src.type:string, "'");
      this.chpl_t = if this.type.chpl_t != ?
                    then this.type.chpl_t
                    else _to_borrowed(src.type);
    }

    @chpldoc.nodoc
    proc init=(src: unmanaged) {
      compilerError("cannot initialize '", this.type:string, "' from a '", src.type:string, "'");
      this.chpl_t = if this.type.chpl_t != ?
                    then this.type.chpl_t
                    else _to_borrowed(src.type);
    }

    pragma "leaves this nil"
    @chpldoc.nodoc
    proc init=(src : _nilType) {
      if this.type.chpl_t == ? then
        compilerError("Cannot establish type of owned when initializing with 'nil'");

      this.init(this.type.chpl_t);

      if isNonNilableClass(chpl_t) then
        compilerError("cannot initialize '", this.type:string, "' from 'nil'");
    }

    // Copy-init implementation to allow for 'new _owned(foo)' in module code
    @chpldoc.nodoc
    proc init(pragma "leaves arg nil" pragma "nil from arg" ref src:_owned) {
      this.chpl_t = src.chpl_t;
      this.chpl_p = owned.release(src);
    }




    @chpldoc.nodoc
    proc type adopt(source) {
      compilerError("cannot adopt a ", source.type:string);
    }

    /*
      Starts managing the argument class instance `obj`
      using the `owned` memory management strategy.
      The result type preserves nilability of the argument type.

      It is an error to directly delete the class instance
      after passing it to `owned.adopt()`.
    */
    inline proc type adopt(pragma "nil from arg" in obj: unmanaged) {
      return new _owned(obj);
    }

    @chpldoc.nodoc
    proc type release(source) {
      compilerError("cannot release a ", source.type:string);
    }

    /*
      Empty `obj` so that it manages `nil` and
      return the instance previously managed by this owned object.

      If the argument is `nil` it returns `nil`.
    */
    inline proc type release(pragma "leaves arg nil" ref obj: owned) {
      var oldPtr = obj.chpl_p;
      type t = obj.chpl_t;

      obj.chpl_p = nil;

      return if _to_nilable(t) == t
                then _to_unmanaged(oldPtr)
                else _to_unmanaged(oldPtr!);
    }



    // Issue a compiler error for illegal uses.
    @chpldoc.nodoc
    proc type create(source) {
      compilerError("cannot create an 'owned' from ", source.type:string);
    }

    /* Creates a new `owned` class reference, taking over the ownership
       of the argument. The result has the same type as the argument.
       If the argument is non-nilable, it must be recognized by the compiler
       as an expiring value. */
    @deprecated(notes="owned.create from an owned is deprecated - please use assignment instead")
    inline proc type create(pragma "nil from arg" in take: owned) {
      return take;
    }

    /* Starts managing the argument class instance `p`
       using the `owned` memory management strategy.
       The result type preserves nilability of the argument type.

       It is an error to directly delete the class instance
       after passing it to `owned.create()`. */
    pragma "unsafe"
    @deprecated(notes="owned.create from an unmanaged is deprecated - please use :proc:`owned.adopt` instead")
    inline proc type create(pragma "nil from arg" p : unmanaged) {
      // 'result' may have a non-nilable type
      var result: (p.type : owned);
      result.retain(p);
      return result;
    }

    /*
       The deinitializer for :record:`owned` will destroy the class
       instance it manages when the :record:`owned` goes out of scope.
     */
    proc deinit() {
      if isClass(chpl_p) { // otherwise, let error happen on init call
        if chpl_p != nil then
          delete _to_unmanaged(chpl_p);
      }
    }

    /*
       Empty this :record:`owned` so that it stores `nil`.
       Deletes the previously managed object, if any.
     */
    pragma "leaves this nil"
    @deprecated(notes="owned.clear is deprecated - please assign `nil` to the owned object instead")
    proc ref clear() {
      if chpl_p != nil {
        delete _to_unmanaged(chpl_p);
        chpl_p = nil;
      }
    }


    /*
       Change the instance managed by this class to `newPtr`.
       If this record was already managing a non-nil instance,
       that instance will be deleted.
     */
    @deprecated(notes="owned.retain is deprecated - please use :proc:`owned.adopt` instead")
    proc ref retain(pragma "nil from arg" newPtr:unmanaged) {
      if !isCoercible(newPtr.type, chpl_t) then
        compilerError("cannot retain '" + newPtr.type:string + "' " +
                      "(expected '" + _to_unmanaged(chpl_t):string + "')");

      var oldPtr = chpl_p;
      chpl_p = newPtr;
      if oldPtr then
        delete _to_unmanaged(oldPtr);
    }

    /*
       Empty this :record:`owned` so that it manages `nil`.
       Returns the instance previously managed by this :record:`owned`.
     */
    pragma "leaves this nil"
    pragma "nil from this"
    @deprecated(notes="owned.release is deprecated - please use the :proc:`owned.release` type method instead")
    proc ref release() {
      var oldPtr = chpl_p;
      chpl_p = nil;

      if _to_nilable(chpl_t) == chpl_t {
        return _to_unmanaged(oldPtr);
      } else {
        return _to_unmanaged(oldPtr!);
      }
    }

    /*
       Return the object managed by this :record:`owned` without
       impacting its lifetime at all. It is an error to use the
       value returned by this function after the :record:`owned`
       goes out of scope or deletes the contained class instance
       for another reason, such as with `=` or ``owned.adopt``.
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
  }

  /*
    Assignment between two :record:`owned` transfers ownership of the object
    managed by ``rhs`` to ``lhs``. This is done by setting ``rhs`` to `nil` and
    then setting ``lhs`` to point to the object that ``rhs`` managed before,
    if any. After that, it deletes the object previously managed by ``lhs``,
    if any.
  */
  operator =(ref lhs:_owned,
         pragma "leaves arg nil"
         ref rhs: _owned)
    where ! (isNonNilableClass(lhs) && isNilableClass(rhs))
  {
    import HaltWrappers;
    // Work around issues in associative arrays of owned
    // TODO: remove this workaround
    if lhs.chpl_p == nil && rhs.chpl_p == nil then
        return;

    // Check only if --nil-checks is enabled or user requested
    if chpl_checkNilDereferences || enablePostfixBangChecks {
      // Add check for lhs non-nilable.
      // Do it even if rhs non-nilable, as for now static checking has holes.
      if isNonNilableClass(lhs.chpl_t) {
          if rhs.chpl_p == nil {
            HaltWrappers.nilCheckHalt("assigning nil to non-nilable owned");
          }
      }
    }

    if !isCoercible(rhs.chpl_t, lhs.chpl_t) then
        compilerError("cannot assign to '" + lhs.type:string + "' " +
                      "from '" + rhs.type:string + "'");

    var oldPtr = _to_unmanaged(lhs.chpl_p);
    lhs.chpl_p = owned.release(rhs);
    delete oldPtr;
  }

  @chpldoc.nodoc
  operator =(pragma "leaves arg nil" ref lhs:_owned, rhs:_nilType) {
    delete owned.release(lhs);
  }
  /*
    Swap two :record:`owned` objects.
  */
  operator <=>(ref lhs:_owned, ref rhs:lhs.type) {
    lhs.chpl_p <=> rhs.chpl_p;
  }


  // initCopy is defined explicitly as a workaround
  // for problems with initializers in this case
  pragma "init copy fn"
  @chpldoc.nodoc
  proc chpl__initCopy(pragma "leaves arg nil" pragma "nil from arg"
                      ref src: _owned,
                      definedConst: bool) {
    var ret = new _owned(src);
    return ret;
  }

  // autoCopy is defined explicitly as a workaround
  // for problems with initializers in this case
  pragma "auto copy fn"
  @chpldoc.nodoc
  proc chpl__autoCopy(pragma "leaves arg nil" pragma "nil from arg"
                      ref src: _owned,
                      definedConst: bool) {
    var ret = new _owned(src);
    return ret;
  }
  // This is a workaround - compiler was resolving
  // chpl__autoDestroy(x:object) from internal coercions.
  pragma "auto destroy fn"
  @chpldoc.nodoc
  proc chpl__autoDestroy(ref x: _owned) {
    __primitive("call destructor", __primitive("deref", x));
  }

  @chpldoc.nodoc
  proc _owned.readThis(f) throws {
    _readWriteHelper(f);
  }

  @chpldoc.nodoc
  proc _owned.writeThis(f) throws {
    _readWriteHelper(f);
  }

  // Don't print out 'chpl_p' when printing an _owned, just print class pointer
  @chpldoc.nodoc
  proc _owned._readWriteHelper(f) throws {
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
        this = nil;
    }
  }

  // cast to owned?, no class downcast
  @chpldoc.nodoc
  inline operator :(pragma "nil from arg" in x:owned class, type t:owned class?)    where isSubtype(x.chpl_t,_to_nonnil(t.chpl_t))
  {
    var castPtr = x.chpl_p:_to_nilable(_to_unmanaged(t.chpl_t));
    x.chpl_p = nil;
    // t stores a nilable type
    return new _owned(castPtr);
  }

  // cast to owned?, no class downcast
  @chpldoc.nodoc
  inline operator :(pragma "nil from arg" in x:owned class?, type t:owned class?)
    where isSubtype(x.chpl_t,t.chpl_t)
  {
    var castPtr = x.chpl_p:_to_nilable(_to_unmanaged(t.chpl_t));
    x.chpl_p = nil;
    // t stores a nilable type
    return new _owned(castPtr);
  }

  // cast to owned!, no class downcast, no casting away nilability
  @chpldoc.nodoc
  inline operator :(pragma "nil from arg" in x:owned class, type t:owned class)
    where isSubtype(x.chpl_t,t.chpl_t)
  {
    var castPtr = x.chpl_p:_to_nilable(_to_unmanaged(t.chpl_t));
    x.chpl_p = nil;
    // t stores a non-nilable type
    return new _owned(castPtr!);
  }

  // cast to owned!, no class downcast, casting away nilability
  @chpldoc.nodoc
  inline operator :(in x:owned class?, type t:owned class) throws
    where isSubtype(_to_nonnil(x.chpl_t),t.chpl_t)
  {
    var castPtr = x.chpl_p:_to_nilable(_to_unmanaged(t.chpl_t));
    if castPtr == nil {
      throw new owned NilClassError();
    }
    x.chpl_p = nil;
    // t stores a non-nilable type
    return new _owned(castPtr!);
  }

  // this version handles downcast to non-nil owned
  @chpldoc.nodoc
  inline operator :(ref x:owned class?, type t:owned class) throws
    where isProperSubtype(t.chpl_t,_to_nonnil(x.chpl_t))
  {
    if x.chpl_p == nil {
      throw new owned NilClassError();
    }
    // the following line can throw ClassCastError
    var castPtr = try x.chpl_p:_to_nonnil(_to_unmanaged(t.chpl_t));
    x.chpl_p = nil;
    return new _owned(castPtr);
  }
  @chpldoc.nodoc
  inline operator :(ref x:owned class, type t:owned class) throws
    where isProperSubtype(t.chpl_t,x.chpl_t)
  {
    // the following line can throw ClassCastError
    var castPtr = try x.chpl_p:_to_nonnil(_to_unmanaged(t.chpl_t));
    x.chpl_p = nil;
    return new _owned(castPtr);
  }


  // this version handles downcast to nilable owned
  @chpldoc.nodoc
  inline operator :(pragma "nil from arg" ref x:owned class?, type t:owned class?)
    where isProperSubtype(t.chpl_t,x.chpl_t)
  {
    // this cast returns nil if the dynamic type is not compatible
    var castPtr = x.chpl_p:_to_nilable(_to_unmanaged(t.chpl_t));
    if castPtr != nil {
      x.chpl_p = nil;
    }
    return new _owned(castPtr);
  }
  // this version handles downcast to nilable owned
  @chpldoc.nodoc
  inline operator :(ref x:owned class, type t:owned class?)
    where isProperSubtype(_to_nonnil(t.chpl_t),x.chpl_t)
  {
    // this cast returns nil if the dynamic type is not compatible
    var castPtr = x.chpl_p:_to_nilable(_to_unmanaged(t.chpl_t));
    if castPtr != nil {
      x.chpl_p = nil;
    }
    return new _owned(castPtr);
  }

  // cast from nil to owned
  @chpldoc.nodoc
  inline operator :(pragma "nil from arg" x:_nilType, type t:_owned)  {
    if isNonNilableClass(t.chpl_t) then
      compilerError("Illegal cast from nil to non-nilable owned type");
    if isGenericType(t) then
      compilerError("illegal cast from nil to a generic owned type");

    var tmp:t;
    return tmp;
  }

  pragma "always propagate line file info"
  @chpldoc.nodoc
  inline proc postfix!(const ref x:_owned) {
    import HaltWrappers;
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
