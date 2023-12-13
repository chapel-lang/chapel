module M {
        // this would be in the standard library...
        operator =(ref lhs: int, rhs: int) {
          __primitive("=", lhs, rhs);
        }

        proc test() {
          var x: int = 0;
          var y = x;
          try {
            y;
            y;
          } catch {
            x;
          }
        }
      }