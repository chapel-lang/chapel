// https://github.com/chapel-lang/chapel/issues/17406

// The goal of this code is to trigger a compilation error
// in an internal module that is nested in another call.
// Currently it calls chpl__atomicType(string) which will
// call chpl__processorAtomicType(string) which calls
// compilerError.
//
// If this trick stops working, we could do something else, ex.
// use the --prepend-internal-module-dir compiler flag to either:
//  - replace an already-used module with a dummy module that will trigger
//    this error, or
//  - add a new dummy module with a function that will trigger
//    a compilation error and adjust ChapelStandard to also publicly use
//    that dummy module.

var x: int, y: uint;

proc main() {
  bar();
}

proc bar() {
  foo();
}

proc foo() {
  chpl__atomicType(string);
}
