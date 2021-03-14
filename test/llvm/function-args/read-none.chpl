// This test checks whether readnone function attributes is generated for
// functions marked with the relevant pragma.

//CHECK: Function Attrs
//CHECK: readnone
//CHECK: define
//CHECK: testfunction
//CHECK: )
//CHECK: attributes
//CHECK: readnone

pragma "llvm readnone"
proc testfunction(a:int, b:int, c:int)
{
  return a+b*c;
}

var x = testfunction(1,2,3);
writeln(x);
