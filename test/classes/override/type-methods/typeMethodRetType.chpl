//
// Test to make sure type methods returning param still work as expected.
//

class Car { var x: int = 0; }
class Building { var x: int = 0; }
class Tower: Building { var height: int = 0; }

class Animal {
  var x: int = 0;
  proc type getCarType() type { return Car; }
  proc type getBuildingType() type { return Building; }
  proc type getSelfType() type { return this; }
}

class Cat: Animal {
  var x: int = 0;
  override proc type getBuildingType() type { return Tower; }
  override proc type getSelfType() type { return this; }
}

proc main() {
  writeln(Animal.getCarType():string);    // "Car"
  writeln(Cat.getCarType():string);       // "Car"

  writeln(Animal.getBuildingType():string);   // "Building"
  writeln(Cat.getBuildingType():string);      // "Tower"

  writeln(Animal.getSelfType():string);   // "Animal"
  writeln(Cat.getSelfType():string);      // "Cat"
}
