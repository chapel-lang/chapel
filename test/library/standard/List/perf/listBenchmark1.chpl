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

//
//
//

var a = createList(n);
var b = createLinkedList(n);
var c = createArray(n);

