// Credit goes to Bryant for this test of a former bug in string.join().

use Random.PCGRandom only PCGRandomStream;
use CTypes;

config const count = 100;

proc get_str_with_concat(x: int, y: int): string {
  return "out_" + x:string + "_" + y:string;
}

proc get_str_with_join(x: int, y: int): string {
  return "_".join("out", x :string, y :string);
}

proc main() {
  var r = new owned PCGRandomStream(int);

  for i in 1..count {
    var x = r.getNext(1, 20000);
    var y = r.getNext(1, 20000);

    var s1 = get_str_with_concat(x, y);
    var s2 = get_str_with_join(x, y);
    assert(s1 == s2);

    var s1c = c_ptrToConst_helper(s1):c_string;
    var s2c = c_ptrToConst_helper(s2):c_string;  // edit: Oops, was c_ptrToConst_helper(s1):c_string;
    if s1c != s2c {
      writeln("Mismatched!");
      writeln("  s1  = ", s1);
      writeln("  s1c = ", string.createCopyingBuffer(s1c));
      writeln("  s2  = ", s2);
      writeln("  s2c = ", string.createCopyingBuffer(s2c));
    }
  }
}
