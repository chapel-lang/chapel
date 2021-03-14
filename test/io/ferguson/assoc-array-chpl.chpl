use IO;

proc main() {
  var tmp = opentmp();

  var A = ["one"=>1, "two"=>2, "three"=>3];
  var B:[A.domain] int;

  {
    tmp.writer().writef("%ht\n", A);
  }

  {
    tmp.reader().readf("%ht\n", B);
  }

  for key in A.domain.sorted() {
    writeln("A[", key, "]=", A[key]);
  }
  for key in B.domain.sorted() {
    writeln("B[", key, "]=", B[key]);
  }
}
