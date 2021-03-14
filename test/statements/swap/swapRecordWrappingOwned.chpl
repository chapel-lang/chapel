class Dog {
  var name: string;
}

record DogOwner {
  var pet: owned Dog?;

  proc init(s: string) {
    this.pet = new owned Dog(s);
  }
}

var p1 = new DogOwner("Rex");
var p2 = new DogOwner("Spot");

p1 <=> p2;

writeln(p1, p2);
