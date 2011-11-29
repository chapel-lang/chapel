class dog {
  var weight : real;
  var paws : int;
  proc grow(pounds : real) {
    weight += pounds;
  }
}

var d : dog = new dog();

d.weight = 38.5;
d.paws = 4;
writeln("d: (weight = ", d.weight, ", paws = ", d.paws, ")");
writeln("Grow");
d.grow(4.5);
writeln("d: (weight = ", d.weight, ", paws = ", d.paws, ")");
