// BUG: When generating calls to exported functions, under LLVM, if the
// function has an indirect return (e.g. it returns a struct), the
// call emits garbage. This is because the loop that emits code for
// arguments does not adjust the LLVM arg offsets to account for the
// indirect return (which is the 0th formal).
extern {
  typedef struct foo {
    char flag;
    void* pointer;
    long long int size;
  } foo;

  // The LVT for 'printFoo' will be pre-populated by this decl.
  foo takeAndReturnFoo(foo someFoo);
}

export proc takeAndReturnFoo(in someFoo: foo) {
  return someFoo;
}

proc test() {
  // ABI support should fire for this call (because it is an exported fn).
  var foo1 = new foo();
  var foo2 = takeAndReturnFoo(foo1);
  writeln(foo1);
  writeln(foo2);
  assert(foo1 == foo2);
}
test();

