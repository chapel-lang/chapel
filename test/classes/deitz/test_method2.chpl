class dog {
  var weight : float;
  var paws : integer;
  function grow(pounds : float) {
    weight += pounds;
  }
}

var d : dog = dog();

d.weight = 38.5;
d.paws = 4;
writeln("d: (weight = ", d.weight, ", paws = ", d.paws, ")");
writeln("Grow");
d.grow(4.5);
writeln("d: (weight = ", d.weight, ", paws = ", d.paws, ")");
