use LinkedLists;

class C { var x: int = 123; }

proc main() {
  const z = makeList((new C()).borrow(), (new C()).borrow());
  writeln(z);
}
