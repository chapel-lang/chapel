use DistributedMap;

var dm1 = new distributedMap(string, int);
var inputArr = ["John", "Jacob", "Jingleheimer", "Schmidt", "his", "name", "is",
                "my", "name", "too", "whenever", "we", "go", "out", "the",
                "people", "always", "shout", "there", "goes", "John", "Jacob",
                "Jingleheimer", "Schmidt", "la", "la", "la", "la", "la", "la",
                "la"];

forall key in inputArr with
(var agg = dm1.updateAggregator(updater=lambda (key: string, ref element: int) { element += 1; })) {
  agg.update(key);
}

// agg.flush(); // agg doesn't live outside the forall so can't flush here

var first = true;
for key in dm1.keys() { // won't be great perf but that's okay for now
  if (!first) {
    writeln(",");
  } else {
    first = false;
  }
  write(key, "=", dm1[key]); // TODO: different syntax for access?
}
writeln();
