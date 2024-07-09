
use IO;
use JSON;

proc helper(thing) {
  var w = stdout.withSerializer(jsonSerializer);
  w.writeln(thing);
}

proc main() {
  helper(1..10);
  helper({1..10});
  helper(for i in 1..10 do i);
}
