// error for using an opaque C struct
// cf. test/extern/records/OpaqueStructNoField.chpl

extern {
  struct mydata;  // opaque
  static void acceptData(struct mydata* d) { }
}

var x: mydata;  // error: opaque / incomplete definition of 'mydata'

use CTypes;
acceptData(c_addrOf(x));
writeln(x);
