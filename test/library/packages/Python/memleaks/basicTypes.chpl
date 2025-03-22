use Python, List, Map;

config const print = false;
var interp = new Interpreter();

proc isEqual(A, B) {
  if isArray(A) && isArray(B) {
    if A.size != B.size {
      return false;
    }
    if A.isAssociative() {
      var same = true;
      forall (ai, bi) in zip(A.domain, B.domain) with (&& reduce same) {
        same = same && isEqual(ai, bi) && isEqual(A[ai], B[bi]);
      }
      return same;
    } else {
      var same = true;
      forall (a, b) in zip(A, B) with (&& reduce same) do
        same = same && isEqual(a, b);
      return same;
    }
  } else {
    return A == B;
  }
}

proc testType(type t, value: t) {
  if print then writeln("  type: ", t:string, " value: ", value);
  var res = interp.toPython(value);
  if print then writeln("    res: ", res);
  const from = interp.fromPython(t, res);
  if !isEqual(value, from) {
    writeln("mismatch: ", value, " != ", from);
  }
}

proc main() {

  testType(uint(16), 42);
  testType(int, 100);
  testType(real(32), 3.14);
  testType(string, "this is a decently long string that may require extra work");
  { // homogeneous tuple
    var tup1: 4*int = (5, 4, 3, 2);
    testType(tup1.type, tup1);
  }

  { // heterogeneous tuple
    var tup1: (int, real, bool) = (17, 3.14, false);
    testType(tup1.type, tup1);
  }

  { // tuple with a list
    var tup1: (int, list(int)) = (5, new list([2, 1, 5]));
    testType(tup1.type, tup1);
  }

  { // homogeneous tuple with strings
    var tup1: 2*string = ("5", "oof");
    testType(tup1.type, tup1);
  }

  { // homogeneous tuple with bytes
    var tup1: 2*bytes = (b"5", b"oof");
    testType(tup1.type, tup1);
  }

  { // heterogeneous tuple with a string
    var tup1: (int, string, bool) = (17, "blah", false);
    testType(tup1.type, tup1);
  }

  { // int arrays
    var arr: [0..<10] int = 0..<10;
    testType(arr.type, arr);

    var l = new List.list(arr);
    testType(l.type, l);

    var l2 = new List.list(list(int));
    l2.pushBack(l); l2.pushBack(l);
    testType(l2.type, l2);
  }

  { // string arrays
    var strArr = [i in 0..<100] i:string;
    testType(strArr.type, strArr);

    var l = new List.list(strArr);
    testType(l.type, l);

    var l2 = new List.list(list(string));
    l2.pushBack(l); l2.pushBack(l);
    testType(l2.type, l2);
  }

  { // assoc arrays
    var assoc = ["foo" => "bar", "baz" => "qux"];
    testType(assoc.type, assoc);
  }

  { // maps
    var m = new map(string, int);
    m.add("one", 1);
    m.add("two", 2);
    m.add("three", 3);

    testType(m.type, m);
  }

}
