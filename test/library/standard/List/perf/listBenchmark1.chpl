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
private use Random;
private use Search;
private use Sort;
private use Time;

config const n: int = 1024 * 1024 * 64;
config const linearN: int = (n / 2);
config const seed: int = 13;


