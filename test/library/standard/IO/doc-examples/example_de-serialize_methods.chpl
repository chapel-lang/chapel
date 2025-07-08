//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//
use IO;

{
record T: serializable {};

/* START_EXAMPLE_1 */
proc T.serialize(writer: fileWriter(locking=false, ?),
                 ref serializer: ?st) throws {/*...*/}

proc ref T.deserialize(reader: fileReader(locking=false, ?),
                       ref deserializer: ?dt) throws {/*...*/}
/* STOP_EXAMPLE_1 */
}

{
class T: serializable {}

/* START_EXAMPLE_2 */
override proc T.serialize(writer: fileWriter(locking=false, ?),
                          ref serializer: ?st) throws {/*...*/}

override proc T.deserialize(reader: fileReader(locking=false, ?),
                            ref deserializer: ?dt) throws {/*...*/}
/* STOP_EXAMPLE_2 */
}
