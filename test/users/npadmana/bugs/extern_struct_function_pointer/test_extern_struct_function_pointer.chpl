extern {
  typedef struct {
    double (*x)(double x);
  } S;
 
  const S const1;
  const double const2;
  
  static void func1(S x) {
  }
}

var x : S;

func1(const1);
var y= const2;
