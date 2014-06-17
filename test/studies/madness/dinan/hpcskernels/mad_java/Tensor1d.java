/** Tensor1d class implements one-dimensional tensor/vector operations
 */
public class Tensor1d {

    private double[] A;
    private int n;

    // constructors

    /** Construct a Tensor1d of length n filled with zeroes
        @param n    length
    */
    public Tensor1d(int n) {
        this.A = new double[n];
        this.n = n;
        for (int i = 0; i < this.n; i++)
            this.A[i] = 0.0;
    }
    
    /** Construct a Tensor1d of length n filled with val
        @param n    length
        @param val  value to fill with   
    */
    public Tensor1d(int n, double val) {
        this.A = new double[n];
        this.n = n;
        for (int i = 0; i < this.n; i++)
            this.A[i] = val;
    }
    
    /** Construct a Tensor1d from a 1-d array.
        @param A    array of doubles.
    */
    public Tensor1d(double[] A) {
        this.A = new double[A.length];
        this.n = A.length;
        for (int i = 0; i < this.n; i++)
            this.A[i] = A[i];
    }

    // public methods

    /** Copy
        @return      new Tensor1d which contains copy of matrix
    */
    public Tensor1d copy(){
        return new Tensor1d(this.getArray()); 
    }

    /** get array assoc with Tensor1d
        @return      array assoc with Tensor1d
    */
    public double[] getArray(){
        return this.A;
    }

    /** get size
        @return       size of Tensor1d
    */
    public int getN(){
        return this.n;
    }

    /** get element (i)
        @param  i    index
        @return      value of element (i)
    */
    public double get(int i){
        return this.A[i];
    }
    
    /** Get this[s:e] into new Tensor1d
        @param s     start of range to copy from
        @param e     end of range to copy from
        @return      new Tensor1d with this[s:e] in it
    */
    public Tensor1d getTensor1d(int s, int e){
        int n = e - s;
        Tensor1d ret = new Tensor1d(n);
        double[] B = ret.getArray();
        for (int i = 0; i < n; i++)
            B[i] = this.A[i+s];
        return ret;
    }

    /** Get this[range] into new Tensor1d
        @param range   Range1d
        @return        new Tensor1d with this[range] in it
    */
    public Tensor1d getTensor1d(Range1d range){
        return getTensor1d(range.s,range.e);
    }
    
    /** Set element this[i] to val
        @param i     index
        @param val   value    
        @return      this so operations can be chained
    */
    public Tensor1d set(int i, double val){
        this.A[i] = val;
        return this;
    }
    
    
    /** Set elements of this[s:e] to val
        @param s     start of range to copy into
        @param e     end of range to copy into
        @param val   value
        @return      this so operations can be chained
    */
    public Tensor1d set(int s, int e, double val){
        for (int i = s; i < e; i++)
            this.A[i] = val;
        return this;
    }
    
    /** Set elements of this[s:e] from other[0:e-s]
        @param s     start of range to copy into
        @param e     end of range to copy into
        @param other another Tensor1d
        @return      this so operations can be chained
    */
    public Tensor1d setTensor1d(int s, int e, Tensor1d other){
        int n = e - s;
        double[] B = other.getArray();
        for (int i = 0; i < n; i++)
            this.A[i+s] = B[i];
        return this;
    }

    /** Set elements of this[range] from other[0:len(range)]
        @param range  Range1d
        @param other  another Tensor1d
        @return       this so operations can be chained
    */
    public Tensor1d setTensor1d(Range1d range, Tensor1d other){
        return setTensor1d(range.s, range.e, other);
    }
    
    /** Set all elements to value
        @param val   value to fill with
        @return      this so operations can be chained
    */
    public Tensor1d setFill(double val){
        return this.set(0,this.n,val);
    }
    
    
    /** Frobenius norm of this
        @return      sqrt of the sum of the squares of all elements
    */
    public double normf(){
        double sum = 0.0;
        for (int i = 0; i < this.n; i++)
            sum += this.A[i] * this.A[i];
        return Math.sqrt(sum);
    }

    /** Inner product of this and other.
        @param other another Tensor1d
        @return      inner product of this and other
    */
    public double inner(Tensor1d other){
        
        if(this.n != other.getN())
            throw new IllegalArgumentException("Tensor1d:inner: dims must match");

        double[] B = other.getArray();
        double sum = 0.0;
        
        for (int i = 0; i < this.n; i++)
            sum += this.A[i] * B[i];

        return sum;
    }

    /** Transform this by other.
        result(i) <-- sum(i') A(i') C(i',i)
        @param other a transform matrix Tensor2d
        @return      this transformed by other
        @exception  IllegalArgumentException dims must match
    */
    public Tensor1d transform(Tensor2d other){

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

    
    /** inplace gaxpy of this and other, modifies this, this = alpha*this + beta*other
        @param alpha scaling factor for this
        @param other another Tensor1d
        @param beta  scaling factor for other
        @return      this so operations can be chained
        @exception  IllegalArgumentException dims must match
    */
    public Tensor1d gaxpy(double alpha, Tensor1d other, double beta){
        
        if(this.n != other.getN())
            throw new IllegalArgumentException("Tensor1d:gaxpy: dims must match");

        double[] B = other.getArray();
        
        for(int i = 0; i < this.n;  i++)
            this.A[i] = alpha*this.A[i] + beta*B[i];
        
        return this;
    }
    
    /** inplace scale this by alpha, modifies this, this = alpha*this
        @param alpha scaling factor for this
        @return      this so operations can be chained
    */
    public Tensor1d scale(double alpha){
        
        for(int i = 0; i < n;  i++)
            A[i] = alpha*A[i];

        return this;
    }

    /** inplace elementwise multiply
        @param other another Tensor1d
        @return      this so operations can be chained
        @exception  IllegalArgumentException dims must match
    */
    public Tensor1d emul(Tensor1d other){

        if(this.n != other.getN())
            throw new IllegalArgumentException("Tensor1d:emul: dims must match");

        double[] B = other.getArray();

        for (int i = 0; i < n; i++)
            A[i] *= B[i];

        return this;
    }

    /** addition
        @param other another Tensor1d
        @return      new Tensor1d = this + other
    */
    public Tensor1d add(Tensor1d other){
        Tensor1d r = new Tensor1d(this.getArray());
        return r.gaxpy(1.0,other,1.0);
    }

    /** subtraction
        @param other another Tensor1d
        @return      new Tensor1d = this - other
    */
    public Tensor1d sub(Tensor1d other){
        Tensor1d r = new Tensor1d(this.getArray());
        return r.gaxpy(1.0,other,-1.0);
    }

    /** convert Tensor1d to a String
        @return      String representation of this
    */
    public String toString(){
        String r = super.toString() + "= { ";
        for (int i = 0; i < n-1; i++)
            r += A[i] + ", ";
        r += A[n-1]+" }";
        return r;
    }

    // test things out to see if they work
    // this method gets run if you run "java thisclass"
    public static void main (String argv[]) {
        // 5 long filled with zero
        Tensor1d t1 = new Tensor1d(5);
        System.out.println("t1 = "+t1);

        // 5 long filled with 5.0
        Tensor1d t2 = new Tensor1d(5, 5.0);
        System.out.println("t2 = "+t2);

        // 5 long uninitialized take 1d array of double as is
        Tensor1d t3 = new Tensor1d(new double[5]);

        // get element 3 out of t1
         System.out.println("t1.get(3); = "+t1.get(3));
        
        // fill t3 with { 5.0, 4.0, 4.0, 4.0 }
        t3.set(0, 5.0);
        t3.set(1, 5, 4.0);
        System.out.println("t3.set(0, 5.0); t3.set(1, 5, 4.0); = "+t3);

        // extract elements 2 and 3 from t2 and put into t1
        t1 = t2.getTensor1d(2, 4);
        System.out.println("t1 = t2.getTensor1d(2, 4); = "+t1);

        // set t3 to 1.0
        t3.set(0, 5, 1.0);
        // set elements 1 and 2 of t3 to contents of t1
        t3.setTensor1d(1, 3, t1);
        System.out.println("t3.set(0, 5, 1.0); t3.setTensor1d(1, 3, t3); = "+t3);

        // fill t2 with 2.0
        t2.setFill(2.0);
        System.out.println("t2.setFill(2.0); = "+t2);

        // try exceptions
        t1 = new Tensor1d(5, 2.0);
        t2 = new Tensor1d(4, 3.0);
        try
        {
            t1.inner(t2);
        }
        catch (IllegalArgumentException e)
        {
            System.out.println("caught it:"+ e);
        }
        try
        {
            t1.emul(t2);
        }
        catch (IllegalArgumentException e)
        {
            System.out.println("caught it:"+ e);
        }
        try
        {
            t1.add(t2);
        }
        catch (IllegalArgumentException e)
        {
            System.out.println("caught it:"+ e);
        }

        
    }
    
    
}
