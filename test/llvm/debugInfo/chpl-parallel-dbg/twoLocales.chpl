use Debugger;

proc main() {

  writeln("Running on locale ", here.id);
  var myArr: [1..10] int;
  for i in myArr.domain do
    myArr[i] = i * (here.id + 1);
  writeln("Array on locale ", myArr.locale.id, ": ", myArr);

  on Locales.last {
    writeln("In on-clause on locale ", here.id);
    writeln("Array on locale ", myArr.locale.id, ": ", myArr);
    var localArr = myArr + 1;
    writeln("New array on locale ", localArr.locale.id, ": ", localArr);
    breakpoint;

    // CHECK: {{^}}(lldb) p myArr
    // CHECK-NEXT: (_ref(_array(unmanaged [domain(1,int(64),one)] int(64)))) 0x{{[0-9a-f]+}} [1..10] int
    // CHECK-NEXT: [1] = 1
    // CHECK-NEXT: [2] = 2
    // CHECK-NEXT: [3] = 3
    // CHECK-NEXT: [4] = 4
    // CHECK-NEXT: [5] = 5
    // CHECK-NEXT: [6] = 6
    // CHECK-NEXT: [7] = 7
    // CHECK-NEXT: [8] = 8
    // CHECK-NEXT: [9] = 9
    // CHECK-NEXT: [10] = 10

    // CHECK: {{^}}(lldb) p localArr
    // CHECK-NEXT: ([domain(1,int(64),one)] int(64)) [1..10] int(64)
    // CHECK-NEXT: [1] = 2
    // CHECK-NEXT: [2] = 3
    // CHECK-NEXT: [3] = 4
    // CHECK-NEXT: [4] = 5
    // CHECK-NEXT: [5] = 6
    // CHECK-NEXT: [6] = 7
    // CHECK-NEXT: [7] = 8
    // CHECK-NEXT: [8] = 9
    // CHECK-NEXT: [9] = 10
    // CHECK-NEXT: [10] = 11

  }

}
