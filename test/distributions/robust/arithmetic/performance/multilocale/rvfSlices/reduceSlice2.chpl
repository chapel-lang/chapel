module testWrapper {
  require "helper/reduceSlicePrivate.chpl";
  use reduceSlicePrivate;
}

// This test is essentially a clone of reduceSlice.chpl, but one that
// demonstrates something strange that I can't explain or fix quickly
// enough for this PR.  When the `use` in helper/reduceSlicePrivate is
// `private`, comm counts go up relative to when it's `public` because
// the calls to:
//
//   canResolveMethod(dom, "chpl__serialize")
//   canResolveMethod(arr, "chpl__serialize")
//
// within chpl__rvfMe() in modules/internal/ArrayViewSlice.chpl return
// false rather than true.  E.g., compare the comm counts for this
// test to that of reduceSlice.chpl in this same directory.