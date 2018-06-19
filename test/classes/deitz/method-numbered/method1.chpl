class dog {
  var weight : real;
  var paws : int;

  proc grow() {
    writeln("woof");
  }

  proc grow(pounds : real) {
    weight += pounds;
    writeln("woof");
  }
}

var d : borrowed dog = new borrowed dog();

d.weight = 38.5;
d.paws   = 4;

writeln("d: (weight = ", d.weight, ", paws = ", d.paws, ")");

writeln("Grow 4.5");
d.grow(4.5);
writeln("d: (weight = ", d.weight, ", paws = ", d.paws, ")");

writeln("Grow");
d.grow();
writeln("d: (weight = ", d.weight, ", paws = ", d.paws, ")");
