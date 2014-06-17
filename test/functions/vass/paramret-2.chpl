/*
Note: the goal of this test is to ensure that the compiler emits
a warning about the "paramret" bug.

The behavior recorded in the .good assumes that the bug is there.

When the bug is fixed, this test will fail. Then,
the compiler warning should be eliminated and
this test retired completely.
The correct behavior is verified by paramret-1.chpl,
which should be un-futurized at that point.
*/

proc b() param :int return 6;
proc d   param :int return 8;

writeln(b());
writeln(d);
