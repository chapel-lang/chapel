// this test is a summary of a scenario where Arkouda leaked some string data.
// The root cause of the leak was that the decodeByteBuffer helper was
// allocating a 1-byte buffer even if the input buffer was 0-length. This was
// then causing that allocation to be dropped on the floor without proper
// deallocation
use CTypes;
var empty = "";
var cPtrString = "".c_str();
var sFromPtr = string.createBorrowingBuffer(cPtrString);
sFromPtr = empty;
writeln(sFromPtr.size);
