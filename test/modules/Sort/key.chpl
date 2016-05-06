use Sort;

proc main() {

  // Sort array by absolute value
  var Arr = [-1,-4, 2, 3];

  writeln(Arr);

  BubbleSort(Arr, doublecheck=true);
  BubbleSort(Arr, doublecheck=true, comparator=new comparator());

  writeln("After bubblesort:");
  writeln(Arr);

  Arr = [-1,-4, 2, 3];
  InsertionSort(Arr, doublecheck=true);
  InsertionSort(Arr, doublecheck=true, comparator=new comparator());


  writeln("After insertionsort:");
  writeln(Arr);

  Arr = [-1,-4, 2, 3];
  QuickSort(Arr, doublecheck=true);
  QuickSort(Arr, doublecheck=true, comparator=new comparator());

  writeln("After quicksort:");
  writeln(Arr);

  Arr = [-1,-4, 2, 3];
  MergeSort(Arr, doublecheck=true);
  MergeSort(Arr, doublecheck=true, comparator=new comparator());

  writeln("After mergesort:");
  writeln(Arr);

  Arr = [-1,-4, 2, 3];
  HeapSort(Arr, doublecheck=true);
  HeapSort(Arr, doublecheck=true, comparator=new comparator());

  writeln("After heapsort:");
  writeln(Arr);

  /* Sorts not updated:
   * SelectionSort - not sure how to do comparator with reduce intents
   */
}


record comparator { }

// key function for sorting
proc comparator.key(a) { return abs(a); }
