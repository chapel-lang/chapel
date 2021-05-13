/*
This test prototypes "forall intents", at the Chapel source level.

MOTIVATION

We intent to change the semantics of Chapel's forall loops with
"forall intents". They will work the same way as "task intents" (r21940),
except they will apply to forall loops.

For example:

  var i = 5;
  // task intents - already implemented
  coforall ... {
    writeln(i); // the "captured" version of 'i' is referenced
    i = 6;      // illegal
  }
  // forall intents - upcoming
  forall ... {
    writeln(i); // the "captured" version of 'i' is referenced
    i = 6;      // illegal
  }

INFORMAL SEMANTICS

Informally, forall intents will work as if the leader iterator
for the forall loop is inlined and the already-implemented
task intents are applied. For example, imagine that the leader
in the above 'forall' has a 'coforall' and 'yield XYZ' inside
that coforall. After inlining it in the 'forall' loop, we'd get:

  var i = 5;
  { // start inlined leader iterator
    ...
    coforall ... {
  
      // yield XYZ; - replaced with the body of the leader loop
      {
        followThis = XYZ;
        for ... in followIterator(followThis) {
  
          // the body of the original forall loop goes here
  	writeln(i);
          i = 6;
  
        }
      }
    }
    ...
  } // end inlined leader iterator

The effect of the already-implemented task intents is that
'i' will be captured by the 'coforall' and the resulting
shadow copy of 'i' will be implicitly referenced in the
forall loop body.

IMPLEMENTATION PROPOSAL

The actual implementation strategy we are leaning towards is
different, however, because of (a) technical difficulties in marrying
leader inlining and with task intents, and (b) unclarity in how easily
we would be able to implement recursive leaders.

Instead, our implementation proposal is illustrated in this example
with captureWithIntent().

We will stick with converting a forall into the leader and follower
loops, like it is done in Chapel already. The compiler will continue
doing this transformation in the parser.

We will identify the variables to apply forall intents to in the same
manner as those for task intents - they will be the variables referenced
in the forall loop body and declared outside the loop. Explicit ref()
clause will be available like for the tasking constructs. We also
intent to provide clauses for other intents, for tasking constructs
as well as 'forall'.

The variables subject to forall intents will be "threaded" through
the leader: it will get them as arguments and yield them as part
of the yielded value. The yielded value in the Chapel source code
will be wrapped in a tuple with these additional values.
The variables specified with a ref or ref-like intent in the forall
will be yielded by reference - which will need to be implemented
for most types.

The references to those variables within the forall loop body
will be replaced with references to the respective components of
the yielded value.

THE CONTENTS OF THIS EXAMPLE

The code in this example shows four variations of a forall loop.
Each variant uses synchronization and executes the forall loop within
a "cobegin ref(i)" - to allow for concurrent modification of 'i'.
This is done to illustrate reliably the difference between
capture and no-capture for 'i'.

* nocapture() - the current forall, where there is no capture.

* captureManually() - a small mod to show what it means to capture 'i'.
  'iCapture' plays the role of the captured shadow copy.

* noCaptureWithIntent() - shows a simplification of how the compiler
  converts the forall from nocapture() to the leader+follower loops.
  No intents are applied here - the semantics is unchanged.

* captureWithIntent() - illustrates the implementation strategy we are
  leaning towards. Here, 'i' is the variable to apply forall intents to.
  We thread it manually through the leader and replace the references
  to 'i' in the body of the loop with 'iCapture' that comes from
  the leader. We create a modified version of the leader that accepts
  an 'i' argument and includes it in the yielded value. The already-
  implemented task intents apply to 'i' inside the modified leader.

*/

nocapture();
captureManually();
noCaptureWithIntent();
captureWithIntent();

iter ITR(numIters:int) {
  for yld in 1..numIters do
    yield yld;
}

iter ITR(numIters:int, param tag: iterKind)
  where tag == iterKind.leader
{
  const halfpoint = numIters/2;
  cobegin {
    yield 1..halfpoint;
    yield halfpoint+1..numIters;
  }
}

iter ITR(numIters:int, param tag: iterKind, followThis: range)
  where tag == iterKind.follower
{
  for yld in followThis do
    yield yld;
}

proc nocapture() {
  writeln("A%start nocapture()");
  var i: int = 1;
  var s$: sync bool;
  var q$: single bool;

  cobegin with (ref i) {
    {
      s$.readFE();
      i = 5;
      q$.writeEF(true);
    }
    {
      forall j in ITR(3) with (ref i) {
        s$.writeXF(true);
        q$.readFF();
        writeln("A-", j, "  i=", i);
      }
    }
  }
  writeln("A/done nocapture()");
}

proc captureManually() {
  writeln("B%start captureManually()");
  var i: int = 1;
  var s$: sync bool;
  var q$: single bool;

  cobegin with (ref i) {
    {
      s$.readFE();
      i = 5;
      q$.writeEF(true);
    }
    {
      var iCapture = i;
      forall j in ITR(3) {
        s$.writeXF(true);
        q$.readFF();
        writeln("B-", j, "  i=", iCapture);
      }
    }
  }
  writeln("B/done captureManually()");
}

proc noCaptureWithIntent() {
  writeln("C%start noCaptureWithIntent()");
  var i: int = 1;
  var s$: sync bool;
  var q$: single bool;

  cobegin with (ref i) {
    {
      s$.readFE();
      i = 5;
      q$.writeEF(true);
    }
    {
      // Convert 'forall' into leader+follower, like what the parser does.
      for followThis in ITR(3, tag=iterKind.leader) {
        for j in ITR(3, tag=iterKind.follower, followThis) {

          s$.writeXF(true);
          q$.readFF();
          writeln("C-", j, "  i=", i);

        }  // follower
      }  // leader
    }
  }
  writeln("C/done noCaptureWithIntent()");
}

proc captureWithIntent() {
  writeln("D%start captureWithIntent()");
  var i: int = 1;
  var s$: sync bool;
  var q$: single bool;

  cobegin with (ref i) {
    {
      s$.readFE();
      i = 5;
      q$.writeEF(true);
    }
    {
      // Convert 'forall' into leader+follower, like what the parser does.
      // Then manually thread the 'i' through the leader, ammended
      // with an extra argument and yield - see iter ITRcapture() below.
      // See the comments at the top.
      for (followThis, iCapture) in ITRcapture(3, tag=iterKind.leader, i) {
        for j in ITR(3, tag=iterKind.follower, followThis) {

          s$.writeXF(true);
          q$.readFF();
          writeln("D-", j, "  i=", iCapture);

        }  // follower
      }  // leader
    }
  }

  writeln("D/done captureWithIntent()");
}

iter ITRcapture(numIters:int, param tag: iterKind, i:int)
  where tag == iterKind.leader
{
  const halfpoint = numIters/2;
  cobegin {
    yield (1..halfpoint, i);
    yield (halfpoint+1..numIters, i);
  }
}
