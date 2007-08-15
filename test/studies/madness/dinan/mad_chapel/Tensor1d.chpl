/** Tensor1d class implements one-dimensional tensor/vector operations
 */
class Tensor1d {
    var n: int = 0;
    var val = 0.0;
    // FIXME: Private Members:
    var A: [0..n-1] real;

    // FIXME: Need constructors plz thx

    def initialize() {
      A = val;
    }

    /** Construct a Tensor1d from a 1-d array.
        @param A    array of doubles.
    */
    def dupFrom(B: [] real) {
        writeln("dupFrom: ", B);
        n = B.domain.dim(1).high - B.domain.dim(1).low + 1;
        A.domain = B.domain;
        A = B;
        return this;
    }

    def this(i) var {
      return A[i];
    }

    def these() var {
      yield A.domain;
    }

    // def methods

    /** Copy
        @return      new Tensor1d which contains copy of matrix
    */
    def copy(): Tensor1d {
        return Tensor1d().dupFrom(A);
    }

    /** get array assoc with Tensor1d
        @return      array assoc with Tensor1d
    */
    def getArray() var {
        return A;
    }

    /** get size
        @return       size of Tensor1d
    */
    def getN(): int {
        return n;
    }

    /** get element (i)
        @param  i    index
        @return      value of element (i)
    */
    def get(i: int): real {
        return A[i];
    }
    
    /** Get this[s:e] into new Tensor1d
        @param s     start of range to copy from
        @param e     end of range to copy from
        @return      new Tensor1d with this[s:e] in it
    */
    def getTensor1d(s: int, e: int): Tensor1d {
        var n   = e - s;
        var ret = Tensor1d(n);
        for i in 0..n-1 do
          ret.set(i, A[i+s]);
        return ret;
    }

    /** Get this[range] into new Tensor1d
        @param range   Range1d
        @return        new Tensor1d with this[range] in it
    */
/*    def Tensor1d getTensor1d(Range1d range){
        return getTensor1d(range.s,range.e);
    }
*/    
    /** Set element this[i] to val
        @param i     index
        @param val   value    
        @return      this so operations can be chained
    */
    def set(i: int, val: real): Tensor1d {
        A[i] = val;
        return this;
    }
    
    
    /** Set elements of this[s:e] to val
        @param s     start of range to copy into
        @param e     end of range to copy into
        @param val   value
        @return      this so operations can be chained
    */
    def set(s: int, e: int, val: real): Tensor1d {
        for i in s..e-1 do
            A[i] = val;
        return this;
    }
    
    /** Set elements of this[s:e] from other[0:e-s]
        @param s     start of range to copy into
        @param e     end of range to copy into
        @param other another Tensor1d
        @return      this so operations can be chained
    */
    def setTensor1d(s: int, e: int, other: Tensor1d): Tensor1d {
        var n = e - s;
        var B = other.getArray();
        for i in 0..n-1 do
            A[i+s] = B[i];
        return this;
    }

    /** Set elements of this[range] from other[0:len(range)]
        @param range  Range1d
        @param other  another Tensor1d
        @return       this so operations can be chained
    */
/*    def Tensor1d setTensor1d(Range1d range, Tensor1d other){
        return setTensor1d(range.s, range.e, other);
    }
*/    
    /** Set all elements to value
        @param val   value to fill with
        @return      this so operations can be chained
    */
    def setFill(val: real): Tensor1d {
        A = val;
        return this;
    }
    
    
    /** Frobenius norm of this
        @return      sqrt of the sum of the squares of all elements
    */
    def normf(): real {
        return sqrt(+ reduce A**2);
    }

    /** Inner product of this and other.
        @param other another Tensor1d
        @return      inner product of this and other
    */
/*    def double inner(Tensor1d other){
        
        if(this.n != other.getN())
            throw new IllegalArgumentException("Tensor1d:inner: dims must match");

        double[] B = other.getArray();
        double sum = 0.0;
        
        for (int i = 0; i < this.n; i++)
            sum += this.A[i] * B[i];

        return sum;
    }
    */

    /** Transform this by other.
        result(i) <-- sum(i') A(i') C(i',i)
        @param other a transform matrix Tensor2d
        @return      this transformed by other
        @exception  IllegalArgumentException dims must match
    */
/*    def Tensor1d transform(Tensor2d other){

        double[][] C = other.getArray();
        int n = other.getDim0();
        if(n != this.n)
            throw new IllegalArgumentException("Tensor1d:transform: C and A dims must match");
        int m = other.getDim1();
        Tensor1d ret = new Tensor1d(m);
        double[] R = ret.getArray();
        
        // transform
        // result(i,j,k...) <-- sum(i',j', k',...) A(i',j',k',...) C(i',i) C(j',j) C(k',k)
        // The input dimensions of A must all be the same and agree with the first dimension of C
        for (int i = 0; i < m; i++)
        {
            R[i] = 0.0;
            for (int ip = 0; ip < n; ip++)
                R[i] += A[ip] * C[ip][i];
        }
        
        return ret;
    }
    */

    
    /** inplace gaxpy of this and other, modifies this, this = alpha*this + beta*other
        @param alpha scaling factor for this
        @param other another Tensor1d
        @param beta  scaling factor for other
        @return      this so operations can be chained
        @exception  IllegalArgumentException dims must match
    */
/*    def Tensor1d gaxpy(double alpha, Tensor1d other, double beta){
        
        if(this.n != other.getN())
            throw new IllegalArgumentException("Tensor1d:gaxpy: dims must match");

        double[] B = other.getArray();
        
        for(int i = 0; i < this.n;  i++)
            this.A[i] = alpha*this.A[i] + beta*B[i];
        
        return this;
    }
    */
    
    /** inplace scale this by alpha, modifies this, this = alpha*this
        @param alpha scaling factor for this
        @return      this so operations can be chained
    */
/*    def Tensor1d scale(double alpha){
        
        for(int i = 0; i < n;  i++)
            A[i] = alpha*A[i];

        return this;
    }
    */

    /** inplace elementwise multiply
        @param other another Tensor1d
        @return      this so operations can be chained
        @exception  IllegalArgumentException dims must match
    */
/*    def Tensor1d emul(Tensor1d other){

        if(this.n != other.getN())
            throw new IllegalArgumentException("Tensor1d:emul: dims must match");

        double[] B = other.getArray();

        for (int i = 0; i < n; i++)
            A[i] *= B[i];

        return this;
    }
    */

    /** addition
        @param other another Tensor1d
        @return      new Tensor1d = this + other
    */
/*    def Tensor1d add(Tensor1d other){
        Tensor1d r = new Tensor1d(this.getArray());
        return r.gaxpy(1.0,other,1.0);
    }
    */

    /** subtraction
        @param other another Tensor1d
        @return      new Tensor1d = this - other
    */
/*    def Tensor1d sub(Tensor1d other){
        Tensor1d r = new Tensor1d(this.getArray());
        return r.gaxpy(1.0,other,-1.0);
    }
    */

    /** convert Tensor1d to a String
        @return      String representation of this
    */
/*    def String toString(){
        String r = super.toString() + "= { ";
        for (int i = 0; i < n-1; i++)
            r += A[i] + ", ";
        r += A[n-1]+" }";
        return r;
    }
    */
}

// test things out to see if they work
// this method gets run if you run "java thisclass"
def main () {
  // 5 long filled with zero
  var t1 = Tensor1d(5);
  writeln("t1 = Tensor1d(5) = ", t1);

  // 5 long filled with 5.0
  var t2 = Tensor1d(10, 5.0);
  writeln("t2 = Tensor1d(10, 5.0) = ", t2);

  // fill t3 with { 5.0, 4.0, 4.0, 4.0 }
  t2.set(0, 5.0);
  t2.set(1, 5, 4.0);
  writeln("t2.set(0, 5.0); t2.set(1, 5, 4.0); = ", t2);

  // extract elements 2 and 3 from t2 and put into t1
  t1 = t2.getTensor1d(2, 4);
  writeln("t1 = t2.getTensor1d(2, 4); = ", t1);

  // set t3 to 1.0
  t2.set(0, 5, 1.0);
  // set elements 1 and 2 of t3 to contents of t1
  t2.setTensor1d(1, 3, t1);
  writeln("t3.set(0, 5, 1.0); t3.setTensor1d(1, 3, t3); = ", t2);

  // fill t2 with 2.0
  t2.setFill(2.0);
  writeln("t2.setFill(2.0); = ", t2);

  writeln("t2.copy() = ", t2.copy());
}
