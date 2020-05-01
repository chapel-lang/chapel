// Credit goes to Bryant for this test of a former bug in string.join().

use Random.PCGRandom only PCGRandomStream;

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

    var s1c = s1.c_str();
    var s2c = s2.c_str();  // edit: Oops, was s1.c_str();
    if s1c != s2c {
      writeln("Mismatched!");
      writeln("  s1  = ", s1);
      writeln("  s1c = ", createStringWithNewBuffer(s1c));
      writeln("  s2  = ", s2);
      writeln("  s2c = ", createStringWithNewBuffer(s2c));
    }
  }
}
