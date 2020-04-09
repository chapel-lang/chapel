extern {
  typedef struct {
    double field;
  } S;
 
  const S const1;
 
  static void func1(S x) {
  }
}

var x : S;

func1(const1);
func1(x);
