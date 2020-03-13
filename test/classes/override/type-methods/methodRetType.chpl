//
// Test to make sure methods returning types and their overrides work as
// expected. 
//

class Car { var x: int = 0; }
class Building { var x: int = 0; }
class Tower: Building { var height: int = 0; }

class Animal {
  var x: int = 0;
  proc getCarType() type { return Car; }
  proc getBuildingType() type { return Building; }
  proc getSelfType() type { return this.type; }
}

class Cat: Animal {
  var x: int = 0;
  override proc getBuildingType() type { return Tower; }
  override proc getSelfType() type { return this.type; }
}

proc main() {
  var a: Animal = new Animal();
  var b: Cat = new Cat();
  var c: Animal = new Cat();

  writeln(a.getCarType():string);     // "Car"
  writeln(b.getCarType():string);     // "Car"
  writeln(c.getCarType():string);     // "Car"

  writeln(a.getBuildingType():string);    // "Building"
  writeln(b.getBuildingType():string);    // "Tower"
  writeln(c.getBuildingType():string);    // "Building"

  writeln(a.getSelfType():string);    // "Animal"
  writeln(b.getSelfType():string);    // "Cat"
  writeln(c.getSelfType():string);    // "Animal"
}
