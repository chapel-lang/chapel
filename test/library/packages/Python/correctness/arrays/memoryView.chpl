use Python;

proc printArray(func, arr) {
  var s = func(string, arr);
  writeln(s);
}

proc main() {
  var interp = new Interpreter();
  var memoryView = interp.importModule('memoryView');
  var dump_memory_view = memoryView.get('dump_array');

  {
    var arr = [1, 2, 3];
    printArray(dump_memory_view, new Array(interp, arr));
  }

  {
    var arr = [1, 2, 3]:uint(32);
    printArray(dump_memory_view, new Array(interp, arr));
  }


  {
    var arr = [1, 2, 3]:uint(8);
    printArray(dump_memory_view, new Array(interp, arr));
  }

  {
    var arr = [true, false, true, false, false, false;
               true, false, true, false, false, false;
               true, false, true, false, false, false;
               true, false, true, false, false, false;];
    printArray(dump_memory_view, new Array(interp, arr));
  }

  {
    var arr = [2.3, 4.5, 6.7; 19.0, 20.0, 21.0];
    printArray(dump_memory_view, new Array(interp, arr));
  }

  {
    var arr = [1.0, 2.0, 3.0; 4.0, 5.0, 6.0;;
               7.0, 8.0, 9.0; 10.0, 11.0, 12.0]:real(32);
    printArray(dump_memory_view, new Array(interp, arr));
  }

  {
    const D = {1..10, -5..5, 0..11, 17..20};
    var arr: [D] int(16);
    forall i in 1..#D.size do
      arr[D.orderToIndex(i-1)] = (i * (if i%2 == 0 then 1 else -1)):arr.eltType;
    var pyArr = new Array(interp, arr);
    printArray(dump_memory_view, pyArr);

    arr *= 2;
    printArray(dump_memory_view, pyArr);
  }

  {
    // round trip an array through the memoryview/buffer object
    var arr = [1.0, 2.0, 3.0; 4.0, 5.0, 6.0;;
               7.0, 8.0, 9.0; 10.0, 11.0, 12.0;;
               13.0, 14.0, 15.0; 16.0, 17.0, 18.0]:real(32);

    writeln("Chapel Arr:\n", arr);

    // convert the pyArr to a memoryview and call tolist
    var arrRef = new Array(interp, arr);
    var memView = interp.get('memoryview')(arrRef);
    var lst = memView.call(owned PyList, 'tolist');
    writeln("tolist: ", lst);

    var pyArray = arrRef.value(owned PyArray(real(32), 3));
    var result = pyArray.values();
    writeln("Round tripped array:\n", result);
  }

}
