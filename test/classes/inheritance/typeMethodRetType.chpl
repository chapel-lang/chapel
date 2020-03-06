//
// Test to make sure type methods returning param still work as expected.
//

class Building { var x: int = 0; }
class Tower: Building { var height: int = 0; }

class Animal {
  var x: int = 0;
  proc type getBuildingType() type { return Building; }
  proc type getSelfType() type { return this; }
}

class Cat: Animal {
  var x: int = 0;
  proc type getBuildingType() type { return Tower; }
  proc type getSelfType() type { return this; }
}

proc main() {
  writeln(Animal.getBuildingType():string);   // "Building"
  writeln(Cat.getBuildingType():string);      // "Tower"

  writeln(Animal.getSelfType():string);   // "Animal"
  writeln(Cat.getSelfType():string);      // "Cat"
}
