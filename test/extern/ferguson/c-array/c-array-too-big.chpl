use CPtr;

proc main() {
  // this isn't going to go well if it compiles
  var x: c_array(int, 1024*1024*1024*8);
}
