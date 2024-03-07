iter foo(arg:int(8) = 1) { yield 1; yield 2; yield 3; }
iter foo(param tag:iterKind, arg:int(8) = 1) where tag == iterKind.leader {yield 1; yield 2; yield 3; }
iter foo(param tag:iterKind, arg:int = 1, followThis) where tag == iterKind.follower { yield 1; yield 2; yield 3; }

for i in foo(7) { writeln(i); }
forall i in foo(7) { writeln(i); }
