// The routines tested here are not part of the public interface,
// but their correct operation is necessary to make strings work.
//
// This test may need to change more often than tests that exercise
// the public interface.
const s: string = "événement";

writeln("_cpIndexLen full traversal:");
for (ch, idx, nbytes) in s._cpIndexLen() do
  writeln(ch, ", ", idx, ", ", nbytes);
write("\n");

writeln("_cpIndexLen starting inside a multibyte char:");
for (ch, idx, nbytes) in s._cpIndexLen(4:byteIndex) do
  writeln(ch, ", ", idx, ", ", nbytes);
write("\n");

writeln("_indexLen full traversal:");
for (idx, nbytes) in s._indexLen() do
  writeln(idx, ", ", nbytes);
write("\n");

writeln("_indexLen starting inside a multibyte char:");
for (idx, nbytes) in s._indexLen(4:byteIndex) do
  writeln(idx, ", ", nbytes);
