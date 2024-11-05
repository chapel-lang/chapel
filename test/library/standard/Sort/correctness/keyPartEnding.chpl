use Sort;
use List;

record TwoRepeated : writeSerializable {
  var first:int;
  var nFirst:int;
  var second:int;
  var nSecond:int;

  proc serialize(writer, ref serializer) throws {
    var a = (first:string)*nFirst;
    var b = (second:string)*nSecond;
    writer.write(a, b);
  }
}

proc stringToTwoRepeated(arg:string) {
  var A = new list((int, int));
  var cur:int = 0;
  var n:int = 0;
  for ch in arg {
    var c = ch:int;
    if c == cur {
      n += 1;
    } else if n != 0 {
      A.pushBack( (cur, n) );
      cur = c;
      n = 1;
    } else {
      cur = c;
      n = 1;
    }
  }
  if n != 0 then
    A.pushBack( (cur,n ) );

  assert(A.size == 1 || A.size == 2);
  if A.size == 1 {
    return new TwoRepeated(A[0](0), A[0](1), 0, 0);
  } else {
    return new TwoRepeated(A[0](0), A[0](1), A[1](0), A[1](1));
  }
}

record MyComparator: keyPartComparator {
  proc keyPart(arg:TwoRepeated, i:int) {
    var sum = arg.nFirst + arg.nSecond;
    if i < arg.nFirst then
      return (keyPartStatus.returned, arg.first);
    else if i < sum then
      return (keyPartStatus.returned, arg.second);
    else
      return (keyPartStatus.pre, 0);
  }
}

proc testSort(arr) {
  // First, sort the array as strings
  var a = arr;
  sort(a);
  writeln(a);
  assert(isSorted(a));

  // Then, sort the array as reverse strings
  var b = arr;
  sort(a, new reverseComparator(new defaultComparator()));
  writeln(b);
  assert(isSorted(a, new reverseComparator(new defaultComparator())));

  // Then, sort the array as records
  var c = for s in arr do stringToTwoRepeated(s);
  var cc = new MyComparator();
  cc.keyPart(c[0], 0);
  sort(c, new MyComparator());
  writeln(c);
  assert(isSorted(c, new MyComparator()));

  // And reverse
  var d = forall s in arr do stringToTwoRepeated(s);
  sort(d, new reverseComparator(new MyComparator()));
  writeln(d);
  assert(isSorted(d, new reverseComparator(new MyComparator())));
}

var t = stringToTwoRepeated("00011111");
writeln(t);
var u = stringToTwoRepeated("0");
writeln(u);
var v = stringToTwoRepeated("1");
writeln(v);
writeln();

testSort(["0", "011", "110",
          "11110", "000", "100",
          "00", "1", "1100", "1"]);
