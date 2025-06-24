//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//

/* START_EXAMPLE_0 */
// Compile with: 'chpl Test.chpl -suseProcedurePointers=true'
//

use DynamicLoading;

// A binary may or may not exist at this path.
const path = './TestBinary.so';

// If loading fails an error will be issued and the 'try!' will halt.
const bin = try! binary.load(path);
/* STOP_EXAMPLE_0 */

/* START_EXAMPLE_1 */
// Declare the type of the procedure.
type P = proc(): void;

// Retrieve a procedure named 'foo' from 'bin' with the type 'P'.
const p1 = try! bin.retrieve('foo', P);

// Call the procedure.
p1();
/* STOP_EXAMPLE_1*/

