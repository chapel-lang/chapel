proc foo(x: real(32)) {
  writeln("In real32 foo");
}

/*  disabled due to no int->complex coercions
proc bar(x: complex(64)) {
  writeln("In complex64 bar");
}
*/
foo(2 << 25);
//bar(2 << 25);
