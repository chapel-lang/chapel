//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//
/* START_EXAMPLE_1 */
record point : writeSerializable {
  var x : int;
  var y : int;
}
/* STOP_EXAMPLE_1 */

/* START_EXAMPLE_2 */
proc point.serialize(writer: fileWriter(locking=false, ?),
                     ref serializer: ?st) {
  // Start serializing and get the helper object
  // '2' represents the number of tuple elements to be serialized
  var ser = serializer.startTuple(writer, 2);

  ser.writeElement(x); // serialize 'x' as a tuple element
  ser.writeElement(y); // serialize 'y' as a tuple element

  // End serialization of the tuple
  ser.endTuple();
}
/* STOP_EXAMPLE_2 */

/* START_EXAMPLE_3 */
use IO, JSON;

var p = new point(4, 2);

// Prints '(4, 2)' in the default serialization format
stdout.writeln(p);

// Prints '[4, 2]' in the JSON serialization format
var jsonWriter = stdout.withSerializer(jsonSerializer);
jsonWriter.writeln(p);
/* STOP_EXAMPLE_3 */
