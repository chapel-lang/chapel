use CTypes;

var cPtr = allocate(int, 4);

config param case = 1;

select case {
  when 1 do bytes.createBorrowingBuffer(cPtr, length=3, size=4);
  when 2 do bytes.createAdoptingBuffer(cPtr, length=3, size=4);
  when 3 do bytes.createCopyingBuffer(cPtr, length=3, size=4);
}
