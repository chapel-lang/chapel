// refCountedType.chpl
//
// Implementation of a reference counted type for use in other tests.
//


// The implementation of a reference-counted type.
// Invariants:
//  - The contents of the reference count in the implementation is always
//    equal to the number of handles pointing to it.
class Impl 
{
  var count: int; // The reference count
  var value: real; // the payload.
  var freed: bool = false;

  proc retain() { count += 1; }
  proc release() : int { count -= 1; return count; }
}

// A handle to the reference-counted type,.
// Copying a handle creates a new reference to the implementation, so the 
// reference count therein must be increased.
// Deleting a handle removes a reference from the implementation, so the
// reference count therein must be decremented.
// When the reference count goes to zero, the implementation is deleted.
record Handle
{
  var _impl : Impl;
//  ctor Handle() { _impl = nil; } // Future
//  ctor Handle(val: real) { _impl = new Impl(1, val); }
  proc Handle(val: real) { _impl = new Impl(1, val); }

// Copy-constructors are not yet implemented.  See chpl__initCopy() below.
//  ctor Handle(ref h: Handle) { _impl = h._impl; _impl.retain(); }

// Destructor
  proc deinit() { this.release_helper(); }
  inline proc release_helper()
  {
    if _impl != nil && _impl.release() == 0
    {
      _impl.freed = true;
      delete _impl;
      _impl = nil;
    }
  }

// Accessors
  proc value return _impl.value;
  proc freed return _impl.freed;
}

// Copy-constructor implementation
// This should really be named chpl__autoCopy in the current vernacular, but
// that is really moot under the assumption that both will be replaced by a
// proper copy-constructor.
// The argument should have ref intent, which is also a minor point.
pragma "init copy fn"
proc chpl__initCopy(h: Handle)
{
  if h._impl == nil then halt("Illegal copy of uninitialized Handle.");

  h._impl.retain();
  return h;
  // The returned value is copied bit-by-bit into the storage allocated to hold
  // it in the caller, but without calling the copy-constructor (which, of
  // course, would cause infinite recursion.
  // The caller is responsible for calling ~Handle() on the value thus
  // initialized, or memory will be leaked.
}

// The incorrectly-implemented POD optimization causes the chpl__initCopy routine to
// fail to be called.

// Assignment
// proc =(ref lhs: Handle, ref rhs:Handle)
// This signature does not work.  We get:
// sharedRefCount.chpl:8: In function 'foo':
// sharedRefCount.chpl:14: error: non-lvalue actual is passed to 'ref' formal 'rhs' of = [functionResolution.cpp:2543]
// which means that there is no ref promotion...
// proc =(ref lhs: Handle, rhs:Handle)
// That's where we want to go, but we're not there yet.  Again, since there is
// no ref promotion, we don't bind the lhs either, and end up using the
// catch-all implementation on line 242 of ChapelBase.chpl.
// We have to bind to this version, or we don't get the desired
// reference-counting behavior
proc =(ref lhs: Handle, rhs:Handle)
{
  // The rhs gains a new reference.
  if rhs._impl == nil then halt("Illegal read of uninitialized Handle.");
  rhs._impl.retain();

  // The lhs loses a reference.
  lhs.release_helper();

  // The handle now copies a reference to its implementation from the RHS.
  // The implementation is of class type, so this is a pointer copy.
  lhs._impl = rhs._impl;
}