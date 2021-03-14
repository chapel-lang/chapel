class Parent {
  iter these(): int { halt("Should not be called..."); yield 1; }
  iter these(param tag: iterKind): int where tag == iterKind.standalone { halt("Should not be called..."); yield 2;}
  iter these(param tag: iterKind): int where tag == iterKind.leader { halt("Should not be called..."); yield 3; }
  iter these(param tag: iterKind, followThis): int where tag == iterKind.follower { halt("Should not be called..."); yield 4; }
}

class Child: Parent {
  override iter these(): int { yield 1; }
  override iter these(param tag: iterKind): int where tag == iterKind.standalone { yield 2; }
  override iter these(param tag: iterKind): int where tag == iterKind.leader { yield 3; }
  override iter these(param tag: iterKind, followThis): int where tag == iterKind.follower { yield 4; }
}

var child: borrowed Parent = new borrowed Child();

for c in child do writeln(c);
forall c in child do writeln(c);
forall c in zip(child, child) do writeln(c);
