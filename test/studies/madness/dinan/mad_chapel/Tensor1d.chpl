/** Tensor1d class implements one-dimensional tensor/vector operations
 */
class Tensor1d {
    // FIXME: This is only meant to stand in for a constructor.  Don't use!
    var n: int;
    // FIXME: Private Members:
    var A: [0..n-1] real;

    /** Construct a Tensor1d from a 1-d array.
        @param A    array of doubles.
    */
    def dupFrom(B: [] real) {
        A.domain = B.domain;
        A = B;
        return this;
    }

    def this(i) var {
       return A[i];
    }

    def these() var {
        for i in A.domain do yield i;
    }

    // def methods

    def copy(): Tensor1d {
        return Tensor1d().dupFrom(A);
    }

    def getArray() var {
        return A;
    }

    /** Get [s..e] into new Tensor1d
        @param s     start of range to copy from
        @param e     end of range to copy from
        @return      new Tensor1d with this[s:e] in it
    */
    def getTensor1d(s: int, e: int): Tensor1d {
        return Tensor1d().dupFrom(A[s..e]);
    }

    /** Get [r] into new Tensor1d
        @param r   Range1d
        @return    new Tensor1d with this[range] in it
    */
    def getTensor1d(r): Tensor1d {
        return Tensor1d().dupFrom(a[r]);
    }
    
    
    
    /** Set elements of this[s:e] to val
        @param s     start of range to copy into
        @param e     end of range to copy into
        @param val   value
        @return      this so operations can be chained
    */
    def set(r, val): Tensor1d {
        for i in r do
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
    def gaxpy(alpha, other: Tensor1d, beta) {
        if (A.domain != other.domain) then
            halt("Tensor1d:gaxpy: dims must match");

        A = alpha*A + beta*other.getArray();
        
        return this;
    }
    
    /** inplace scale this by alpha, modifies this, this = alpha*this
        @param alpha scaling factor for this
        @return      this so operations can be chained
    */
    def scale(alpha) {
        A *= alpha;
        return this;
    }

    /** inplace elementwise multiply
        @param other another Tensor1d
        @return      this so operations can be chained
        @exception  IllegalArgumentException dims must match
    */
    def emul(other: Tensor1d) {
        if (A.domain != other.domain) then
            halt("Tensor1d:emul: dims must match");

        A *= other.getArray();

        return this;
    }

    /** addition
        @param other another Tensor1d
        @return      new Tensor1d = this + other
    */
    def add(other: Tensor1d) {
        var r = this.copy();
        return r.gaxpy(1.0,other,1.0);
    }

    /** subtraction
        @param other another Tensor1d
        @return      new Tensor1d = this - other
    */
    def sub(other: Tensor1d){
        var r = this.copy();
        return r.gaxpy(1.0,other,-1.0);
    }

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
  var t2 = Tensor1d(10);
  writeln("t2 = Tensor1d(10, 5.0) = ", t2);

  // fill t3 with { 5.0, 4.0, 4.0, 4.0 }
  t2[0] = 5.0;
  t2.set(1..5, 4.0);
  writeln("t2.set(0, 5.0); t2.set(1, 5, 4.0); = ", t2);

  // extract elements 2 and 3 from t2 and put into t1
  t1 = t2.getTensor1d(2, 4);
  writeln("t1 = t2.getTensor1d(2, 4); = ", t1);

  // set t3 to 1.0
  writeln("t2= ", t2, t2.A.domain, t1.A.domain);
  t2.set(0..5, 1.0);
  // set elements 1 and 2 of t3 to contents of t1
  t2.setTensor1d(1, 3, t1);
  writeln("t3.set(0, 5, 1.0); t3.setTensor1d(1, 3, t3); = ", t2);

  // fill t2 with 2.0
  t2.setFill(2.0);
  writeln("t2.setFill(2.0); = ", t2);

  writeln("t2.copy() = ", t2.copy());
}
