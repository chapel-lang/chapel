//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//
use IO;

/* START_EXAMPLE */
// A record 'R' that serializes as an integer
record R : writeSerializable {
  var x : int;

  proc serialize(writer: fileWriter(locking=false, ?),
                 ref serializer: ?st) {
    writer.write(x);
  }
}

var val = new R(5);
writeln(val); // prints '5'
/* STOP_EXAMPLE */
