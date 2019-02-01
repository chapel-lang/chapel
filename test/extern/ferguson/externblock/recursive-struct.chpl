extern {
  struct loopy {
    struct loopy* ptr;
    int x;
  };
}

var mystruct: loopy;
mystruct.x = 1;
mystruct.ptr = c_ptrTo(mystruct);
writeln(mystruct.x);
writeln(mystruct.ptr == c_ptrTo(mystruct));
