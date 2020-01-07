//
// A performance test benchmarking the following common operations, comparing
// between List, LinkedList, and the Chapel array (when applicable):
//
//    - Insertions of various sizes
//    - Removal of various sizes
//    - Iterations of various sizes
//    - Sequential-access indexing
//    - Random-access indexing
//

private use LinkedLists;
private use List;
private use Random;
private use Search;
private use Sort;
private use Time;

config const trials: int = 128;
config const n: int = 1024 * 1024 * 32;
config const linearN: int = (n / 2);
config const seed: int = 13;

type byte = int(8);

proc createList(size: int) {
  var result: list(byte);
  for i in 1..size do result.append(i);
  return result;
}

proc createLinkedList(size: int) {
  var result: LinkedList(byte);
  for i in 1..size do result.append(i);
  return result;
}

proc createArray(size: int) {
  var result: [1..size] byte;
  for i in 1..size do result[i] = i;
  return result;
}

proc generateNoise() {
  for i in 1..trials {
    var size: int = i * 128;

    var a = createList(size);
    var b = createLinkedList(size);
    var c = createArray(size);

    for (a, b, c) in zip(a, b, c) {
      var rd: int = (a + b + c) % 128;
      a = rd;
      b = rd;
      c = rd;
    }
  }

  return;
}

iter trace(lo: int, hi: int, size: int=(hi-lo)) {
  extern proc srand(seed: int);
  extern proc rand(): int;

  srand(seed);

  var words: int = size / 64 + 1;
  var bmp: [1..words] int(64);
  var touched = 0;

  while touched != size {   
    const val = rand();
    const (int, int) ofs = (val / 64, val % 64);
    const flag = bmp[ofs
     
  }

}

//
// Generate some noise to warm up before the test.
//
generateNoise(16);

//
//
//
var t: timer;
var a = createList(n);
var b = createLinkedList(n);
var c = createArray(n);

//
// Time creating list/linked list/arrays of various sizes.
//

//
// Generate a random trace of indices and then use the trace to test inserts.
//

//
// Reset the testing arrays.
//

//
// Generate a random trace of indices and then use the trace to test removes.
//

//
// Reset the testing arrays.
//

//
// Serial iteration.
//

//
// Parallel iteration.
//

//
// Generate a random trace of indices for random access.
//

//
// Random access indexing.
//


