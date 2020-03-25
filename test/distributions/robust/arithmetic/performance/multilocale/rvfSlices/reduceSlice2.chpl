module testWrapper {
  require "helper/reduceSlicePrivate.chpl";
  use reduceSlicePrivate;
}

// This test is essentially a clone of reduceSlice.chpl, but one that
// demonstrated something surprising: When the `use` in
// helper/reduceSlicePrivate.chpl was `private`, comm counts went up
// relative to when it's `public` because the calls to:
//
//   canResolveMethod(dom, "chpl__serialize")
//   canResolveMethod(arr, "chpl__serialize")
//
// within chpl__rvfMe() in modules/internal/ArrayViewSlice.chpl returned false
// rather than true, disabling the slice serialization optimization.  We used to
// not resolve methods across `private use` statements even if we had an object
// reference that we'd obtained legally (issue #14407).
