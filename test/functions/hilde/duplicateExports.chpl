// duplicateExports.chpl
//
// Expect an error if two exports have the same name.
//

export proc foo(i:int) do return i**2;
export proc bar(i:int) do return -i;
export proc foo(r:real) do return atan(r);
