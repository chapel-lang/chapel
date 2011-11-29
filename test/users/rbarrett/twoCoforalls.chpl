use
   Types;

var NumThreads = 1 : int;

proc foo(i) {
  writeln("In foo()");
}

proc foobar(i) {
  writeln("In foobar()");
}

proc main ( ) {

   var i : int; // Counter.

   // ---------------------
   // Executable Statements
   // ---------------------

   coforall i in 1..NumThreads
      {
         foo ( i );
      }

   // Could be some serial processing here.

   // Run multiple instances of foobar:

   coforall i in 1..NumThreads
      {
         foobar ( i );
      }
}
