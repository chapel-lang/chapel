record R {
  var c: int;
  proc initialize () {c = 2;}
}

proc init (x: [] ?t) {
  for i in x.domain do {
    var y: t;
    x[i] = y;
  }
}

var a: [1..2] R;
init(a);
a[1].c = 1;
writeln(a[1].c, " ", a[2].c);

// MPF - This test used to call
// writeln(a), but that has not-so-clear
// semantics because any copying of a record
// in A seems to be calling the initialize function.

// Since we plan to remove the initialize
// function, and this test seems to be more
// about initialization order in arrays,
// I've removed that from the test.

// This test had a .skipif for --baseline because
// of related issues. I'm including Tom's comment
// about them below:

/* 
    Add classes/figueroa/RecordConstructor2.skipif
    
    This test arguably works correctly in both configurations, even though its output differs
    when run with the --baseline option compared to when run without it.  That is because
    --baseline disables the removeUnnecessaryAutoCopyCalls() optimization.  The compiler may
    add or remove autoCopy calls at will, so the user should not write code that depends upon
    its having run (except as needed for memory management reasons).
    
    In this test, running autoCopy has a side-effect (setting the contents of the record to
    2).  The autoCopy function does this because every constructor implicitly calls the
    initialize() function, and that is what is done by the initialize() function for this
    record type.
    
    The compiler could do a better job of aligning --baseline vs. optimized behavior by
    inserting fewer autoCopy calls to begin with.  Also, the fact that the compiler-generated
    autoCopy calls initialize is quite hidden.

*/
