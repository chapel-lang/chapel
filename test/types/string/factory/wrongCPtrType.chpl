use CTypes;

var cPtr = allocate(int, 4);

config param case = 1;

select case {
  when 1 do string.createBorrowingBuffer(cPtr, length=3, size=4);
  when 2 do string.createAdoptingBuffer(cPtr, length=3, size=4);
  when 3 do string.createCopyingBuffer(cPtr, length=3, size=4);
}
