
var names = ["alice", "bob", "carl", "dana", "eric"];

var dom : domain(string);
var data : [dom] int;

for name in names do data[name] = name.length;

for name in names do
  if !dom.member(name) {
    writeln("FAILURE: name added via array did not exist in backing domain");
    exit(1);
  }

writeln("SUCCESS");
