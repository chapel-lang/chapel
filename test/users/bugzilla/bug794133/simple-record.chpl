var myDom: domain(1);
record R {
  var A: [1..2] uint(8);
}
var myArray: [myDom] R;

proc main() {
  myDom = {0..20};
  dump();
}

proc dump() {
  writeln("myDom is: ", myDom);
  for i in myDom {
    write("(", myArray[i].A.domain, "): ");
    for j in myArray[i].A.domain {
      write(myArray[i].A[j], " ");
    }
    writeln();
  }
}
