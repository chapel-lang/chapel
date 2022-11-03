use DistributedMap;
use IO;

var dm1 = new distributedMap(string, int);
var inputArr = ["John", "Jacob", "Jingleheimer", "Schmidt", "his", "name", "is",
                "my", "name", "too", "whenever", "we", "go", "out", "the",
                "people", "always", "shout", "there", "goes", "John", "Jacob",
                "Jingleheimer", "Schmidt", "la", "la", "la", "la", "la", "la",
                "la"];

forall key in inputArr with
(var agg = dm1.updateAggregator(updater=lambda (ref element: int) { element += 1; })) {
  agg.update(key);
}

// agg.flush(); // agg doesn't live outside the forall so can't flush here

var ch = openwriter("mapOutput.txt");

var first = true;
for key in dm1.keys() { // won't be great perf but that's okay for now
  ch.writeln(key, "=", dm1.getValue(key)); // TODO: different syntax for access?
}
ch.close();
