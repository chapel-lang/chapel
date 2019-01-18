extern {
  typedef struct {
    double field;
  } S;
 
  const S const1;
 
  static void func0(S *x) {
  }
}

var x : S;

func0(c_ptrTo(x));
func0(c_ptrTo(const1));

