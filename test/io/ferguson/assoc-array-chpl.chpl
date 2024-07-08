use IO, ChplFormat;

proc main() {
  var tmp = openTempFile();

  var A = ["one"=>1, "two"=>2, "three"=>3];
  var B:[A.domain] int;

  {
    tmp.writer(serializer = new chplSerializer(), locking=false).writef("%?\n", A);
  }

  {
    tmp.reader(deserializer = new chplDeserializer(), locking=false).readf("%?\n", B);
  }

  for key in A.domain.sorted() {
    writeln("A[", key, "]=", A[key]);
  }
  for key in B.domain.sorted() {
    writeln("B[", key, "]=", B[key]);
  }
}
