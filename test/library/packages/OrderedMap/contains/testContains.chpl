use OrderedMap;
use utilFunctions;

config param impl: mapImpl;

var m = new orderedMap(string, string, false, defaultComparator, impl);

for i in 1..99 by 3 {
  var number = intToEnglish(i);
  m[number] = i:string + " value";
}

for i in 1..99 {
  var number = intToEnglish(i);
  if m.contains(number) {
    writeln("map contains ", number );
  }
}
