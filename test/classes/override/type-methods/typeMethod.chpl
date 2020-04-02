//
// Test behavior of inherited type methods. One method is overridden.
//

class Building { var x: int = 0; }

class Animal {
  var x: int = 0;
  proc type doMakeBuilding() { return new Building(); }
  proc type doMakeSelf() { return new this(); }
}

class Cat: Animal {
  var x: int = 0;
  override proc type doMakeSelf() { return new this(); }
}

proc main() {
  writeln(Animal.doMakeBuilding().type:string);   // "Building"
  writeln(Cat.doMakeBuilding().type:string);      // "Building"

  writeln(Animal.doMakeSelf().type:string);   // "Animal"
  writeln(Cat.doMakeSelf().type:string);      // "Cat"
}
