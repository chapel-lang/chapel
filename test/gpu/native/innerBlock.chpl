use GpuDiagnostics;

startGpuDiagnostics();
on here.gpus[0] {
  var A = [1,2,3,4,5];
  var outerVar = 10;

  forall a in A {
    for i in 0..5 {
      a += i * outerVar;
    }
  }

  writeArr(A);
}

on here.gpus[0] {
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

on here.gpus[0] {
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

on here.gpus[0] {
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

on here.gpus[0] {
  var A = [1,2,3,4,5];
  var outerVar = true;

  forall a in A {
    if outerVar then
      a += 1;
    else
      a -= 1;
  }

  writeArr(A);
}

stopGpuDiagnostics();

writeln(getGpuDiagnostics());

proc writeArr(A) {
  write("Array: ");
  for a in A do write(a, " ");
  writeln();
}
