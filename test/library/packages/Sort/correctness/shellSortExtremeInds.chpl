use Sort.ShellSort;

const loStartHi =
  ((0x20:int(8), 0x30:int(8), 0x70:int(8)),
   (0x70:uint(8), 0x80:uint(8), 0xf0:uint(8)),
   (-0x40:int(8), -0x30:int(8), 0x10:int(8)),
   (0x7fffffffffffff70, 0x7fffffffffffff80, 0x7ffffffffffffff0),
   (0xffffffffffffff70:uint(64), 0xffffffffffffff80:uint(64), 0xfffffffffffffff0:uint(64))
   );

for param i in 0..#loStartHi.size {
  var (lo,start,hi) = loStartHi(i);
  var A : [lo..hi] int;
  forall i in A.domain do
    A[i] = (A.size - (i - A.domain.low)):int;

  writeln(A);
  shellSort(A, start=start, end=hi);

  // (start,end) doesn't include the whole array.  The first 16
  // elements will retain their initial (descending) order.
  writeln(A);
}
