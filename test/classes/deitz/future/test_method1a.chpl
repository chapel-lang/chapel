class dog {
  var weight : float;
  var paws : integer;
  function grow() {
    writeln("woof");
  }
}

var d : dog = dog();

d.weight = 38.5;
d.paws = 4;
writeln("d: (weight = ", d.weight, ", paws = ", d.paws, ")");
writeln("Grow");
d.grow();
writeln("d: (weight = ", d.weight, ", paws = ", d.paws, ")");
