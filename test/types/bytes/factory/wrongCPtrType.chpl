use CPtr;

var cPtr = c_malloc(int, 4);

config param case = 1;

select case {
  when 1 do createBytesWithBorrowedBuffer(cPtr, length=3, size=4);
  when 2 do createBytesWithOwnedBuffer(cPtr, length=3, size=4);
  when 3 do createBytesWithNewBuffer(cPtr, length=3, size=4);
}
