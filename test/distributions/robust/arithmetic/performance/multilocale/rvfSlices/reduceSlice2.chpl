module testWrapper {
  require "helper/reduceSlicePrivate.chpl";
  use reduceSlicePrivate;
}

// This test is essentially a clone of reduceSlice.chpl, but one that
// demonstrates something surprising: When the `use` in
// helper/reduceSlicePrivate.chpl is `private`, comm counts go up
// relative to when it's `public` because the calls to:
//
//   canResolveMethod(dom, "chpl__serialize")
//   canResolveMethod(arr, "chpl__serialize")
//
// within chpl__rvfMe() in modules/internal/ArrayViewSlice.chpl return
// false rather than true, disabling the slice serialization
// optimization.  The reason is that we don't resolve methods across
// `private use` statements even if we have an object reference that
// we've obtained legally (issue #14407).  Compare the comm counts for
// this test to that of reduceSlice.chpl in this same directory to see
// the difference.
