proc ref int.foo() { // returns void
  this += 1;
}

var A = [1,2,3,4,5,6,7,8,9];
writeln(A);

var B = A.foo();  // shouldn't this be an error?
writeln(B.type:string);
writeln(B);


//var C = A[1].foo();  // this is an error
//writeln(C.type:string);
//writeln(C);
