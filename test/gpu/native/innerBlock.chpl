on here.getChild(1) {
  var A = [1,2,3,4,5];
  var outerVar = 10;

  forall a in A {
    for i in 0..5 {
      a += i * outerVar;
    }
  }

  writeArr(A);
}

on here.getChild(1) {
  var A = [1,2,3,4,5];
  var outerVar = 10;

  forall a in A {
    var i = 0;
    while i <= 5 {
      a += i * outerVar;
      i += 1;
    }
  }

  writeArr(A);
}

on here.getChild(1) {
  var A = [1,2,3,4,5];
  var outerVar = 10;

  forall a in A {
    var i = 0;
    do {
      a += i * outerVar;
      i += 1;
    } while i<= 5;
  }

  writeArr(A);
}

on here.getChild(1) {
  var A = [1,2,3,4,5];
  var outerVar = 10;

  forall a in A {
    for i in 0..5 {
      if i%2 == 0 then
        a += i * outerVar;
      else
        a -= i * outerVar;
    }
  }

  writeArr(A);

}

proc writeArr(A) {
  write("Array: ");
  for a in A do write(a, " ");
  writeln();
}
