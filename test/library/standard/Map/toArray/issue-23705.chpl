use Map, Set;
type t = (set(int), );
var uniqueWarnings = new map(string, t);

uniqueWarnings["a"][0].add(1);
writeln(uniqueWarnings.toArray());
writeln(uniqueWarnings.keysToArray());
writeln(uniqueWarnings.valuesToArray());
