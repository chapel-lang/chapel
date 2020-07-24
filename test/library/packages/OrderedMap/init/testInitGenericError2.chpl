use OrderedMap;

config param impl: mapImpl;

class C {
  var x = 10;
}

var l = new orderedMap(int, C);  // C has generic management
writeln(l);
