// namedExport.chpl
//
// Test that we can assign a name to an exported function.
//

export proc foo(i:int) return i**2;
export proc bar(i:int) return -i;
export "realFoo" proc foo(r:real) return atan(r);
