extern {
  typedef struct {
    double (*x)(double x);
  } S;
 
  const S const1;
  
  void func(S x) {
  }
}

var x : S;

func(const1);
