class Parent {
  iter these(): int { halt("Should not be called..."); yield 1; }
  iter these(param tag: iterKind): int where tag == iterKind.standalone { halt("Should not be called..."); yield 2;}
  iter these(param tag: iterKind): int where tag == iterKind.leader { halt("Should not be called..."); yield 3; }
  iter these(param tag: iterKind, followThis): int where tag == iterKind.follower { halt("Should not be called..."); yield 4; }
}

class Child: Parent {
  override iter these(): int { for loc in Locales do yield loc.id; }
  override iter these(param tag: iterKind): int where tag == iterKind.standalone { coforall loc in Locales do on loc do yield here.id; }
  override iter these(param tag: iterKind): int where tag == iterKind.leader { coforall loc in Locales do on loc do yield here.id; }
  override iter these(param tag: iterKind, followThis): int where tag == iterKind.follower { yield followThis; }
}

var A: [0..#numLocales] int;

var child: borrowed Parent = new borrowed Child();

for c in child do A[c] = c; writeln(A); A = 0;
forall c in child do A[c] = c; writeln(A); A = 0;
forall (c, c1) in zip(child, child) do A[c] = c1; writeln(A);
