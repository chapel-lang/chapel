//
// Test to make sure methods returning param and their overrides work as
// expected.
//

class Building {
  var x: int = 0;
  var y: int = 0;
  proc hasMultipleFloors() param { return false; }
}

class Tower: Building {
  var floors: int = 2;
  override proc hasMultipleFloors() param { return true; }
}

proc main() {
  var a: Building = new Building();
  var b: Tower = new Tower();
  var c: Building = new Tower();

  writeln(a.hasMultipleFloors():string);  // False.
  writeln(b.hasMultipleFloors():string);  // True.
  writeln(c.hasMultipleFloors():string);  // False!
}
