proc foo() {
  label outer for i in 1..4 {
    var B: [1..3] int;
    for j in 1..4 {
      var A: [1..3] int;
      if j == 2 then
        break;
      if j == 3 then
        break outer;
      writeln(A);
    }
    writeln(B);
  }
}

foo();
