use PrivatizationWrappers;

var classSizes = [0, 10, 100, 1000, 10000, 100000];

// add some initial values
forall i in classSizes[0]..classSizes[1]-1 {
  var newValue = new unmanaged C(i);
  insertPrivatized(newValue, i);
}

// concurrently add some additional values, while reading all the old ones
for classNum in 1..classSizes.size-2 {
  var arr: [classSizes[0]..classSizes[classNum]-1] int;
  cobegin {

    {
      for i in classSizes[classNum]..classSizes[classNum+1]-1 {
        var newValue = new unmanaged C(i);
        insertPrivatized(newValue, i);
      }
    }

    {
      coforall 1..#here.maxTaskPar-1 {
        for i in classSizes[0]..classSizes[classNum]-1 {
          arr[i] = getPrivatized(i).i;
        }
      }
    }

  }
  for i in arr do writeln(i);
}

// Report no leaks
var big:int;
big = max reduce classSizes;
for i in 0..big-1 {
  var c = getPrivatized(i);
  delete c;
  clearPrivatized(i);
}
