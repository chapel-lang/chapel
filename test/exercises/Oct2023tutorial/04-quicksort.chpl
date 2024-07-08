// demostrating the use of 'cobegin' for synchronous parallelism of statements

var myArray = [7, 4, 9, 3, 99, 2, 16, 15, 13, 15, 3, 8, 6, 5, 12, 10, 11];

quickSort(myArray);
writeln("myArray = ", myArray);

proc quickSort(ref arr: [?D], low: int = D.low, high: int = D.high) {
  if high - low < 8 {
    bubbleSort(arr, low, high);
  } else {
    const pivotLoc = partition(arr, low, high);
    serial (here.runningTasks() > here.maxTaskPar) do
      cobegin {
        quickSort(arr, low, pivotLoc-1);
        quickSort(arr, pivotLoc+1, high);
      }
  }
}

proc bubbleSort(ref arr: [?D], low: int, high: int) {
  for i in low..high {
    for j in low..high-(i-low)-1 {
      if arr[j] > arr[j+1] then
        arr[j] <=> arr[j+1];
    }
  }
}

proc partition(ref arr: [?D], low: int, high: int) {
  var pivotVal = arr[high];
  var storeIndex = low-1;
  for j in low..high {
    if arr[j] < pivotVal {
      storeIndex += 1;
      arr[storeIndex] <=> arr[j];
    }
  }
  arr[storeIndex+1] <=> arr[high];
  return storeIndex+1;
}

