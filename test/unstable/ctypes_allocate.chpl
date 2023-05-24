use CTypes;

var a = allocate(int(64), 3, alignment=c_sizeof(int(64))*2);
deallocate(a);
