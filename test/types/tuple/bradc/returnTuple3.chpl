proc returntwo() {
  var loc_t: (int, real) = (1, 2.3);
  return loc_t;
}

var t = returntwo();

writeln("t is: ", t);
