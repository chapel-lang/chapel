//
//
//
class Building { var x: int = 0; }
class Tower: Building { var height: int = 0; }

class Animal {
  var x: int = 0;
  proc getBuildingType() type { return Building; }
  proc getSelfType() type { return this.type; }
}

class Cat: Animal {
  var x: int = 0;
  proc getBuildingType() type { return Tower; }
  proc getSelfType() type { return this.type; }
}

proc main() {
  var a: Animal = new Animal();
  var b: Cat = new Cat();
  var c: Animal = new Cat();

  writeln(a.getBuildingType():string);  // "Building"
  writeln(b.getBuildingType():string);  // "Building"
  writeln(c.getBuildingType():string);  // "Building"

  writeln(a.getSelfType():string);  // "Animal"
  writeln(b.getSelfType():string);  // "Animal"
  writeln(c.getSelfType():string);  // "Animal"
}
