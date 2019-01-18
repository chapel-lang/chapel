proc foo() {
  serial {
    coforall i in 0..4 {
      coforall j in 0..9 {
        cobegin {
          {
            writeln(i,j,0);
          }
          {
            writeln(i,j,1);
          }
          {
            begin writeln(i,j,2);
            begin writeln(i,j,3);
            begin writeln(i,j,4);
          }
        }
      }
    }
  }
}


foo();
