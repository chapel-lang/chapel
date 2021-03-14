proc foo() {
  serial {
    coforall i in 0..4 {
      on Locales[numLocales-1] {
        coforall j in 0..9 {
          cobegin {
            on Locales[0] {
              writeln(i,j,0);
            }
            {
              writeln(i,j,1);
            }
            {
              begin writeln(i,j,2);
              on Locales[0] do begin writeln(i,j,3);
              begin writeln(i,j,4);
            }
          }
        }
      }
    }
  }
}


foo();
