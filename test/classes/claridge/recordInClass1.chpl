/*
This code is here because it exhibits a bug when compiled and run on JC's Mac.
Namely, the generated executable segfaults right after printing out "hello".

So I am making this a non-future test case, expecting it to run successfully
on our regular testing platforms but raise the issue when run on the Mac.

Triage (on the Mac):
- Seems like the C generated code is the same on mac and my linux
  box (modulo passing the line numbers).
- The C representation corresponding to 'c.r' in the Chapel source
  seems to get corrupted, possibly during the call to writeln().
- When in gdb, the crash is reproducible - unless I step through
  the program line by line, in which case it runs to completion successfully.

This makes me think that there is something else going on on the Mac
in parallel with the main program being executed, making the behavior
nondeterministic.
*/

record R {
  var indices = [1..0];
}

class Container {
  var r = new R();
}

proc main {
  var c = new Container();
  // writeln(c.r.indices);
  writeln("hello");
  c.r.indices = [1..1];
}
