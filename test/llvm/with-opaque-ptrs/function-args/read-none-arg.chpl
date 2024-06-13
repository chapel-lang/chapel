// This test checks whether readnone parameter attributes is generated for
// functions marked with the relevant pragma.
// This test is for LLVM 16+, where readnone is a parameter attribute

//CHECK: define
//CHECK: testfunction
//CHECK: (
//CHECK: , ptr
//CHECK: readnone
//CHECK: %2
//CHECK: )

pragma "llvm readnone"
proc testfunction(a:int, b:int, const ref c:int)
{
  return a+b*c;
}

var x = testfunction(1,2,3);
writeln(x);
