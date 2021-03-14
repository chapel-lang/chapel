use definesPrivate;

writeln(bar);
// Ensures that the module level variable bar is still visible
// when there are private variables present.
printFoo();
// Ensure that calls to outside functions which use the variable
// are also still accurate.
