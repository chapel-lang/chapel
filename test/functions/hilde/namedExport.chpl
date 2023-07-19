// namedExport.chpl
//
// Test that we can assign a name to an exported function.
//

export proc foo(i:int) do return i**2;
export proc bar(i:int) do return -i;
export "realFoo" proc foo(r:real) do return round(r);
