/*
A simplification of the sorted() call in
 test/associative/bharshbarg/arrays/opEquals.chpl
*/

module Application {
  use MySort;
  proc main {
    sort(5);
  }
}

module MySort {
  proc sort(Data) {
    MyQuickSort.quickSort(Data);
  }

  proc chpl_check_comparator() { // to be invoked from quickSort()
    compilerError("success");
  }

  module MyQuickSort {
    // Resolving the chpl_check_comparator call requires
    // either the functions visible in the scope of the caller / POI
    // or import MySort.chpl_check_comparator.
    proc quickSort(Data) {
      chpl_check_comparator();
    }
  }
}
