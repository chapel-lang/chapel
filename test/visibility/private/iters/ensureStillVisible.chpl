use definesPrivate;

writeln("Running remote bar");
for i in bar(2) {
  writeln(i);
}
// Ensures that the module level iterator bar is still visible
// when there are private iterators present.
writeln("Calling remote function to write foo");
printFoo();
// Ensure that calls to outside functions which use the iterator
// are also still accurate.
