// Note: this test is not valid without --no-use-noinit thrown.
// You should not print out the contents of a variable you expect
// to be uninitialized.
var foo: complex = noinit;
writeln(foo);
// Lydia note: when all types utilize constructors, this test should
// override the constructor for complexes to print out a message.
// When this test was added, the default value was determined through
// a function which must create a noinit complex instance and initialize
// its contents within itself.  Obviously skipping noinit in that
// context would not allow the program to function.
