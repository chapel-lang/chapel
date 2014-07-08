var myDom: domain(1);
var myModuleArray = foo();
myDom = {0..20};
dump();

proc foo() {
  var myArray: [myDom][1..2] uint(8);

  return myArray;
}




proc dump() {
  writeln("myDom is: ", myDom);
  for i in myDom {
    write("(", myModuleArray[i].domain, "): ");
    for j in myModuleArray[i].domain {
      write(myModuleArray[i][j], " ");
    }
    writeln();
  }
}
