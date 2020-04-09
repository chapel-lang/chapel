makeInput(1);
makeInput(2.0);


proc makeInput(bucketID) {
  require "foo.h";
  extern type foo;
  extern proc printfoo(x: foo);
  extern proc zerofoo(ref x: foo);

   var x: foo;
   zerofoo(x);
   printfoo(x);
}
