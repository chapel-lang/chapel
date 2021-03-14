//
// Test to make sure type methods returning param still work as expected.
//

class Building { var x: int = 0; }
class Tower: Building { var height: int = 0; }

class Animal {
  var x: int = 0;
  proc type isHerbivore() param { return true; }
}

class Cat: Animal {
  var x: int = 0;
  override proc type isHerbivore() param { return false; }
}

proc main() {
  writeln(Animal.isHerbivore():string);
  writeln(Cat.isHerbivore():string);
}
