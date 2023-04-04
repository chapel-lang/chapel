use CTypes;

var cPtr = c_malloc(int, 4);

config param case = 1;

select case {
  when 1 do string.createWithBorrowedBuffer(cPtr, length=3, size=4);
  when 2 do string.createWithOwnedBuffer(cPtr, length=3, size=4);
  when 3 do string.createWithNewBuffer(cPtr, length=3, size=4);
}
