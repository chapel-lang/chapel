
use IO;
use Json;
use FormatHelper;
use List;
use arrays;

proc main() {
  {
    var D : domain(int) = {1, 2, 3, 5, 6, 8, 10};
    var A : [D] int;
    [d in D] A[d] = d**2;
    arrays.test(A);
  }
}
