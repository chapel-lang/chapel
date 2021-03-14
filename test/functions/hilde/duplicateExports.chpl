// duplicateExports.chpl
//
// Expect an error if two exports have the same name.
//

export proc foo(i:int) return i**2;
export proc bar(i:int) return -i;
export proc foo(r:real) return atan(r);
