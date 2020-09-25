use CPtr;

var cPtr = c_malloc(int, 4);

config param case = 1;

select case {
  when 1 do createStringWithBorrowedBuffer(cPtr, length=3, size=4);
  when 2 do createStringWithOwnedBuffer(cPtr, length=3, size=4);
  when 3 do createStringWithNewBuffer(cPtr, length=3, size=4);
}
