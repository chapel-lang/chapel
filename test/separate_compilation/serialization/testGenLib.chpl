
use MyMod;
use OtherMod;

// This test expects to invoke a procedure 'foo' from MyMod, which will
// print a string. The 'precomp' file for this test will generate the .dyno
// file for "MyMod.chpl", and will modify the string in the binary output
// to prove that this test is using the .dyno file and not the .chpl file.
proc main() {
  MyMod.foo();
  OtherMod.bar();
}
