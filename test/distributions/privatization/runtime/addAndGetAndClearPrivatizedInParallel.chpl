use PrivatizationWrappers;

config const doClear = false;
var classSizes = [0, 10, 100, 1000, 10000, 100000];

// add some initial values
forall i in classSizes[1]..classSizes[3]-1 {
  var newValue = new C(i);
  insertPrivatized(newValue, i);
}

// concurrently add some additional values, while reading some old ones
// and while deleting some older ones
for classNum in 3..classSizes.size-1 {
  cobegin {

    {
      for i in classSizes[classNum]..classSizes[classNum+1]-1 {
        var newValue = new C(i);
        insertPrivatized(newValue, i);
      }
    }

    {
      for i in classSizes[classNum-2]..classSizes[classNum-1]-1 {
        var c = getPrivatized(i);
        delete c;
        clearPrivatized(i);
      }
    }

    {
      coforall 1..#here.maxTaskPar-1 {
        for i in classSizes[classNum-1]..classSizes[classNum]-1 {
          assert(getPrivatized(i).i == i);
        }
      }
    }

  }
}

// Report no leaks
var big:int;
big = max reduce classSizes;
for i in 0..big-1 {
  var c = getPrivatized(i);
  delete c;
  clearPrivatized(i);
}

writeln("OK");
