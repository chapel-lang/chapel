const pi = 3.14,           // pi is a real
      coord = 1.2 + 3.4i,  // coord is a complex…
      coord2 = pi*coord,   // …as is coord2
      name = "brad",       // name is a string
      verbose = false;     // verbose is boolean

proc addem(x, y) {         // addem() has generic arguments
  return x + y;            //   and an inferred return type
}

var sum = addem(1, pi),              // sum is a real
    fullname = addem(name, "ford");  // fullname is a string

writeln((sum, fullname));

