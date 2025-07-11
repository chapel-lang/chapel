//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//
use Subprocess;

/* START_EXAMPLE */
use IO, JSON;

record R {
  // fields...
}

var f = open("data.json", ioMode.cwr),
    r: R;

// write an 'R' in JSON format
f.writer(serializer = new jsonSerializer()).writef("%?", new R(/* ... */));

// read into an 'R' from JSON format
f.reader(deserializer = new jsonDeserializer()).readf("%?", r);
/* STOP_EXAMPLE */

writeln("r = ", r);

var fileReader = f.reader();
for line in fileReader.lines() do
  writeln(line);

// delete test file
var sub = spawn(["rm", "data.json"]);
