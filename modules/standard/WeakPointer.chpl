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

/*
Contains the ``weak`` smart-pointer type for use with ``shared`` objects.

A ``weak`` provides a reference to a :type:`~SharedObject.shared` class object
without requiring it to stay allocated. Such a pattern is useful for
implementing graph or tree structures with bidirectional references, or for
implementing cache-like data structures that maintain a list of objects but
don't require them to stay allocated.

A "strong" shared reference to the relevant class object can be obtained via
the :proc:`~WeakPointer.weak.upgrade` method, or by casting the
``weak`` to a ``shared t`` or a ``shared t?``. If the underlying object is
not valid (i.e., its shared reference count has already dropped to zero
causing it to be de-initialized) the upgrade attempt will fail.

Weak pointers are implemented using task-safe reference counting.
*/
@unstable("The `weak` type is experimental; expect this API to change in the future")
module WeakPointer {
  use Errors, Atomics, ChapelBase, SharedObject;

  record weak : writeSerializable {
    /* The shared class type referenced by this pointer */
    type classType;

    pragma "owned"
    var chpl_p: __primitive("to nilable class", _to_unmanaged(classType)); // instance pointer

    pragma "owned"
    var chpl_pn: unmanaged ReferenceCount?; // reference counter

    // ---------------- Initializers ----------------

    @chpldoc.nodoc
    proc init(c : unmanaged) {
      this.classType = c.type;
      compilerError(
        "cannot initialize a `weak` from an unmanaged class: '" + c.type:string + "'"
      );
    }

    @chpldoc.nodoc
    proc init(c : owned) {
      this.classType = c.type;
      compilerError(
        "cannot initialize a `weak` from an owned class: '" + c.type:string + "'"
      );
    }

    @chpldoc.nodoc
    proc init(c : borrowed) {
      this.classType = c.type;
      compilerError(
        "cannot initialize a `weak` from a borrowed class: '" + c.type:string + "'"
      );
    }

    // disallow initialization from all other types
    @chpldoc.nodoc
    proc init(c) {
      this.classType = c.type;
      compilerError("cannot initialize a `weak` from: '" + c.type:string + "'");
    }

    /*
        Create a new weak reference to a shared class instance 'c'
    */
    @unstable("The `weak` type is experimental; expect this API to change in the future.")
    proc init(c : shared) {
        var ptr = _to_unmanaged(c.chpl_p): _to_nilable(_to_unmanaged(c.chpl_t));
        var count = c.chpl_pn;

        // increment the weak reference count (or store nil if the class is nil)
        if ptr != nil then count!.incrementWeak(); else count = nil;

        this.classType = shared c.chpl_t;
        this.chpl_p = ptr;
        this.chpl_pn = count;
    }

    /*
        Copy-initialize a new ``weak`` from an existing ``weak``.

        Increments the weak-reference count.
    */
    proc init=(pragma "nil from arg" const ref src: weak) {
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


    /*
      Create an empty ``weak`` for the given class type.

      Attempting to upgrade the resulting ``weak`` will always fail.
    */
    proc init(type classType: shared) {
      if !isClass(classType) then
        compilerError("a `weak` can only be initialized from a shared class");
      this.classType = classType;
      this.chpl_p = nil;
      this.chpl_pn = nil;
    }

    // ---------------- Other ----------------

    /*
      Attempt to recover a shared object from this ``weak``

      If the pointer is valid (i.e., at least one ``shared`` reference
      to the data exists), a nilable `shared` object will be returned.

      If the pointer is invalid (or the object itself is ``nil``) then a
      ``nil`` value will be returned.
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
      When a ``weak`` is deinitialized, the weak reference count is
      decremented.

      If there are no other references (weak or strong), the backing pointer
      is freed.
    */
    proc ref deinit() {
      this.doClear();
    }

    @chpldoc.nodoc
    proc ref doClear() {
      if this.chpl_p != nil {
          const totalCount = this.chpl_pn!.releaseWeak();
          if totalCount == 0 then delete this.chpl_pn;
      }
      this.chpl_p = nil;
      this.chpl_pn = nil;
    }

    /*
      Get the number of ``weak`` variables currently pointing at the same
      ``shared`` class as this one.
    */
    proc getWeakCount(): int {
      if const counts = this.chpl_pn
        then return (counts.totalCount.read() - counts.strongCount.read());
        else return 0;
    }

    /*
      Get the number of ``shared`` variables currently pointing at the same
      ``shared`` class as this ``weak``.

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

      Otherwise it will return a nilable :type:`~SharedObject.shared`
      ``t``.
  */
  inline operator :(const ref x: weak, type t: shared class?)
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

      Otherwise it will return a :type:`~SharedObject.shared` ``t``.
  */
  inline operator :(const ref x: weak, type t: shared class) throws
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
      Assign one existing ``weak`` to an other.

      Decrements the weak-reference count of the ``lhs`` pointer.

      This will result in the deinitialization of the ``lhs``'s backing
      pointer if it is the last ``weak`` or ``shared`` that points
      to its object.
  */
  inline operator =(ref lhs: weak, rhs: weak)
    where !(isNonNilableClass(lhs) && isNilableClass(rhs))
  {
    if rhs.chpl_pn != nil then rhs.chpl_pn!.incrementWeak();
    const chpl_p_tmp = rhs.chpl_p;
    const chpl_pn_tmp = rhs.chpl_pn;

    lhs.doClear();
    lhs.chpl_p = chpl_p_tmp;
    lhs.chpl_pn = chpl_pn_tmp;
  }

  proc weak.serialize(writer, ref serializer) throws {
    if const ptr = this.chpl_p {
      if this.chpl_pn!.strongCount.read() > 0 {
        // ptr could be invalidated between /\ and \/ (not worrying about that for now).
        writer.write(ptr);
      } else {
        writer.write("invalid-ptr");
      }
    } else {
      writer.write("nil-object");
    }
  }

}
