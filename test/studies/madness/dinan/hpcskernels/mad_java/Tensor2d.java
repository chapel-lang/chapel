/** Tensor2d class implements two-dimensional tensor/matrix operations
 */
public class Tensor2d {

    private double[][] A;
    private int d0;
    private int d1;
    
    // constructors

    /** Construct a Tensor2d of length n filled with zeroes
        @param n    length dim0 and dim1
    */
    public Tensor2d(int n) {
        this.A = new double[n][n];
        this.d0 = n;
        this.d1 = n;
        for (int i = 0; i < this.d0; i++)
            for (int j = 0; j < this.d1; j++)
                this.A[i][j] = 0.0;
    }

    /** Construct a Tensor2d of length n filled with zeroes
        @param n    length dim0 and dim1
        @param val  value to fill with
    */
    public Tensor2d(int n, double val) {
        this.A = new double[n][n];
        this.d0 = n;
        this.d1 = n;
        for (int i = 0; i < this.d0; i++)
            for (int j = 0; j < this.d1; j++)
                this.A[i][j] = val;
    }

    /** Construct a Tensor2d of (n x m)  filled with zeroes
        @param d0    length dim0
        @param d1    length dim1
    */
    public Tensor2d(int d0, int d1) {
        this.A = new double[d0][d1];
        this.d0 = d0;
        this.d1 = d1;
        for (int i = 0; i < this.d0; i++)
            for (int j = 0; j < this.d1; j++)
            this.A[i][j] = 0.0;
    }
    
    /** Construct a Tensor2d of (n x m) filled with val
        @param d0    length dim0
        @param d1    length dim1
        @param val  value to fill with   
    */
    public Tensor2d(int d0, int d1, double val) {
        this.A = new double[d0][d1];
        this.d0 = d0;
        this.d1 = d1;
        for (int i = 0; i < this.d0; i++)
            for (int j = 0; j < this.d1; j++)
                this.A[i][j] = val;
    }
    
    /** Construct a Tensor2d from a 2-d array.
        @param A    Two-dimensional array of doubles.
        @exception  IllegalArgumentException All rows must have the same length
    */
    public Tensor2d(double[][] A) {
        this.d0 = A.length;
        this.d1 = A[0].length;
        this.A = new double[d0][d1];
        for (int i = 0; i < this.d0; i++)
        {
            if(A[i].length == this.d1)
                for (int j = 0; j < this.d1; j++)
                    this.A[i][j] = A[i][j];
            else
                throw new IllegalArgumentException("Tensor2d(double[][] A): rows have same length?");
        }
    }
    
    // public methods

    /** Copy
        @return      new Tensor2d which contains copy of matrix
    */
    public Tensor2d copy(){
        return new Tensor2d(this.getArray()); 
    }

    /** Transpose
        @return      new Tensor2d which contains transpose of matrix
    */
    public Tensor2d transpose(){
        Tensor2d ret = new Tensor2d(this.d1, this.d0);
        double[][] B = ret.getArray();

        for (int i = 0; i < this.d0; i++)
            for (int j = 0; j < this.d1; j++)
                B[j][i] = this.A[i][j];

        return ret;
    }

    /** rand
        @return      new Tensor2d which contains random entries between 0 and 1
    */
    public static Tensor2d rand(int n){
        Tensor2d ret = new Tensor2d(n);
        double[][] B = ret.getArray();

        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                B[j][i] = Math.random();

        return ret;
    }
    
    /** get array assoc with Tensor2d
        @return      array assoc with Tensor2d
    */
    public double[][] getArray(){
        return this.A;
    }

    /** get dim0 size
        @return       dim0 size of Tensor2d
    */
    public int getDim0(){
        return this.d0;
    }

    /** get dim1 size
        @return       dim1 size of Tensor2
    */
    public int getDim1(){
        return this.d1;
    }

    /** get element (i,j)
        @param  i    dim0 index
        @param  j    dim1 index
        @return      value of element (i,j)
    */
    public double get(int i, int j){
        return this.A[i][j];
    }

    /** Get elements from this[s0:e0][s1:e1]
        @param s0    start of range to copy into, dim0
        @param e0    end of range to copy into, dim0
        @param s1    start of range to copy into, dim1
        @param e1    end of range to copy into, dim1
        @return      new Tensor2d which contains sub-block of matrix
    */
    public Tensor2d getTensor2d(int s0, int e0, int s1, int e1){
        int d0 = e0 - s0;
        int d1 = e1 - s1;        
        Tensor2d ret = new Tensor2d(d0,d1);
        double[][] B = ret.getArray();
        for (int i = 0; i < d0; i++)
            for (int j = 0; j < d1; j++)
                B[i][j] = this.A[i+s0][j+s1];
        return ret;
    }

    /** Get elements from this[range.d[0].s:range.d[0].e][range.d[1].s:range.d[1].e]
        @param range Range2d
        @return      new Tensor2d which contains sub-block of matrix
    */
    public Tensor2d getTensor2d(Range2d range){
        return getTensor2d(range.d[0].s, range.d[0].e,
                           range.d[1].s, range.d[1].e);
    }
    
    /** Set element this[i][j] to val
        @param i     index dim0
        @param j     index dim1       
        @param val   value    
        @return      this so operations can be chained
    */
    public Tensor2d set(int i, int j, double val){
        this.A[i][j] = val;
        return this;
    }
    
    /** Set elements of this[start to end-1] to val
        @param s0     start of range to copy into
        @param e0     end of range to copy into
        @param s1     start of range to copy into
        @param e1     end of range to copy into
        @param val   value
        @return      this so operations can be chained
    */
    public Tensor2d set(int s0, int e0, int s1, int e1, double val){
        for (int i = s0; i < e0; i++)
            for (int j = s1; j < e1; j++)
                this.A[i][j] = val;
        return this;
    }
    
    /** Set all to val
        @param val   value
        @return      this so operations can be chained
    */
    public Tensor2d setFill(double val){
        return this.set(0,this.d0,0,this.d1,val);
    }
    
    /** Set elements of this[s0:e0][s1:e1] from other[0:e0-s0][0:e1-s1]
        @param s0     start of range to copy into
        @param e0     end of range to copy into
        @param s1     start of range to copy into
        @param e1     end of range to copy into
        @param other another Tensor2d
        @return      this so operations can be chained
    */
    public Tensor2d setTensor2d(int s0, int e0, int s1, int e1, Tensor2d other){
        int d0 = e0 - s0;
        int d1 = e1 - s1;
        double[][] B = other.getArray();
        for (int i = 0; i < d0; i++)
            for (int j = 0; j < d1; j++)
                this.A[i+s0][j+s1] = B[i][j];
        return this;
    }
    
    /** Set elements of this[s0:e0][s1:e1] from other[0:e0-s0][0:e1-s1]
        @param range  Range2d range to copy into
        @param other  another Tensor2d
        @return      this so operations can be chained
    */
    public Tensor2d setTensor2d(Range2d range, Tensor2d other){
        return setTensor2d(range.d[0].s, range.d[0].e,
                           range.d[1].s, range.d[1].e, other);
    }
    
    /** Set elements of this[s0:e0][s1:e1] from other[0:e0-s0][0:e1-s1]
        @param s0     start of range to copy into
        @param e0     end of range to copy into
        @param s1     start of range to copy into
        @param e1     end of range to copy into
        @param other another Tensor1d
        @return      this so operations can be chained
    */
    public Tensor2d setTensor2d(int s0, int e0, int s1, int e1, double[][] other){
        int d0 = e0 - s0;
        int d1 = e1 - s1;
        double[][] B = other;
        for (int i = 0; i < d0; i++)
            for (int j = 0; j < d1; j++)
                this.A[i+s0][j+s1] = B[i][j];
        return this;
    }
    
    
    /** Frobenius norm of this
        @return      sqrt of the sum of the squares of all elements
    */
    public double normf(){
        double sum = 0.0;
        for (int i = 0; i < this.d0; i++)
            for (int j = 0; j < this.d1; j++)
                sum += this.A[i][j] * this.A[i][j];
        return Math.sqrt(sum);
    }

    /** Matrix Multiply  result = this * other
        @param other another Tensor2d
        @return      new Tensor2d = product
        @exception  IllegalArgumentException dims must match
    */
    public Tensor2d mul(Tensor2d other){

        if(this.d1 != other.getDim0())
            throw new IllegalArgumentException("Tensor2d:mul: dims must match");
        double[][] B = other.getArray();
        int d2 = other.getDim1();

        Tensor2d ret = new Tensor2d(d0,d2);
        double[][] R = ret.getArray();

        // d0xd1 X d1xd2 => d0xd2
        for (int i = 0; i < this.d0; i++)
            for (int j = 0; j < d2; j++)
                for (int k = 0; k < this.d1; k++)
                    R[i][j] += A[i][k] * B[k][j];

        return ret;
    }

    /** Transform this by other.
        @param other a transform matrix Tensor2d
        @return      new Tensor2d = this transformed by other
        @exception  IllegalArgumentException A must be square and dims must match
    */
    public Tensor2d transform_slow(Tensor2d other){
        
        if(this.d0 != this.d1)
            throw new IllegalArgumentException("Tensor2d:transform: A must be square");
        
        double[][] C = other.getArray();
        int d0 = other.getDim0();
        if(d0 != this.d0)
            throw new IllegalArgumentException("Tensor2d:transform: C and A dims must match");
        int d1 = other.getDim1();
        
        Tensor2d ret = new Tensor2d(d1);
        double[][] R = ret.getArray();
        
        // transform
        // result(i,j,k...) <-- sum(i',j', k',...) A(i',j',k',...) C(i',i) C(j',j) C(k',k)
        // The input dimensions of A must all be the same and agree with the first dimension of C
        for (int i = 0; i < d0; i++)
            for (int j = 0; j < d0; j++)
            {
                R[i][j] = 0.0;
                for (int ip = 0; ip < d1; ip++)
                    for (int jp = 0; jp < d1; jp++)
                        R[i][j] += A[ip][jp] * C[ip][i] * C[jp][j];
            }
        
        return ret;
    }

    /** Transform this by other.
        result(i,j) <-- sum(i',j') A(i',j') C(i',i) C(j',j)
        @param other a transform matrix Tensor2d
        @return      new Tensor2d = this transformed by other
        @exception  IllegalArgumentException A must be square and dims must match
    */
    public Tensor2d transform(Tensor2d other){
        
        if(this.d0 != this.d1)
            throw new IllegalArgumentException("Tensor2d:transform: A must be square");
        
        double[][] C = other.getArray();
        int c_d0 = other.getDim0();
        if(c_d0 != this.d0)
            throw new IllegalArgumentException("Tensor2d:transform: C and A dims must match");
        int c_d1 = other.getDim1();

        Tensor2d ret = new Tensor2d(c_d1);
        double[][] R = ret.getArray();
        
        // transform
        // result(i,j,k...) <-- sum(i',j', k',...) A(i',j',k',...) C(i',i) C(j',j) C(k',k)
        // The input dimensions of A must all be the same and agree with the first dimension of C

        double[] v_jpi = new double[c_d0];
        double v_ij;
        
        for (int i = 0; i < c_d1; i++)
        {
            // Transform along 1st dimension
            for (int jp = 0; jp < c_d0; jp++)
            {
                v_jpi[jp] = 0.0;
                for (int ip = 0; ip < c_d0; ip++)
                    v_jpi[jp] += A[ip][jp] * C[ip][i];
            }
        
            // Transform along 2nd dimension
            for (int j = 0; j < c_d1; j++)
            {
                v_ij = 0.0;
                for (int jp = 0; jp < c_d0; jp++)
                    v_ij +=  v_jpi[jp] * C[jp][j];

                R[i][j] = v_ij;
            }
        }
        
        return ret;
    }

    /** gaxpy of this and other, modifies this, this = alpha*this + beta*other
        @param alpha scaling factor for this
        @param other another Tensor1d
        @param beta  scaling factor for other
        @return      this so operations can be chained
        @exception  IllegalArgumentException dims must match
    */
    public Tensor2d gaxpy(double alpha, Tensor2d other, double beta){
        
        if((this.d0 != other.getDim0()) && (this.d1 != other.getDim1()))
            throw new IllegalArgumentException("Tensor2d:gaxpy: dims must match");

        double[][] B = other.getArray();
        
        for(int i = 0; i < this.d0;  i++)
            for(int j = 0; j < this.d1;  j++)
                this.A[i][j] = alpha*this.A[i][j] + beta*B[i][j];
        
        return this;
    }
    
    /** scale this by alpha, modifies this, this = alpha*this
        @param alpha scaling factor for this
        @return      this so operations can be chained
    */
    public Tensor2d scale(double alpha){
        
        for(int i = 0; i < d0;  i++)
            for(int j = 0; j < d1;  j++)
                A[i][j] = alpha*A[i][j];

        return this;
    }

    /** elementwise multiply
        @param other another Tensor2d
        @return      this so operations can be chained
        @exception  IllegalArgumentException dims must match
    */
    public Tensor2d emul(Tensor2d other){
        
        if((this.d0 != other.getDim0()) && (this.d1 != other.getDim1()))
            throw new IllegalArgumentException("Tensor2d:emul: dims must match");
        
        double[][] B = other.getArray();
        
        for(int i = 0; i < d0;  i++)
            for(int j = 0; j < d1; j++)
                A[i][j] *= B[i][j];
        
        return this;
    }

    /** addition
        @param other another Tensor2d
        @return      new Tensor2d = this + other
    */
    public Tensor2d add(Tensor2d other){
        return this.copy().gaxpy(1.0,other,1.0);
    }

    /** Subtraction
        @param other another Tensor2d
        @return      new Tensor2d = this - other
    */
    public Tensor2d sub(Tensor2d other){
        return this.copy().gaxpy(1.0,other,-1.0);
    }

    /** convert Tensor1d to a String
        @return      String representation of this
    */
    public String toString(){
        String r = super.toString()+"\n{";
        for (int i = 0; i < this.d0-1; i++)
        {
            r += "{";
            for (int j = 0; j < this.d1-1; j++)
                r += this.A[i][j] + ", ";
            r += this.A[i][this.d1-1] +"},\n";
        }
        r += "{";
        for (int j = 0; j < this.d1-1; j++)
            r += this.A[this.d0-1][j] + ", ";
        r += this.A[this.d0-1][this.d1-1] +"}}";
        return r;
    }

    // test things out to see if they work
    // this method gets run if you run "java thisclass"
    public static void main (String argv[]) {

        Tensor2d t1;
        Tensor2d t2;
        Tensor2d t3;

        t1 = new Tensor2d(5);
        System.out.println("t1 = new Tensor2d(5); = "+t1);
        
        t1 = new Tensor2d(5, 1.0);
        System.out.println("t1 = new Tensor2d(5, 1.0); = "+t1);

        t1 = new Tensor2d(5, 4);
        System.out.println("t1 = new Tensor2d(5, 4); = "+t1);

        t1 = new Tensor2d(5, 4, 1.0);
        System.out.println("t1 = new Tensor2d(5, 4, 1.0); = "+t1);

        t2 = t1.transpose();
        System.out.println("t2 = t1.transpose(); = "+t2);

        t1 = new Tensor2d(new double[3][4]);
        t1.setFill(0.0);
        System.out.println("t1 = new Tensor2d(new double[3][4]);t1.setFill(0.0) = "+t1);

        t1 = new Tensor2d(5,4, 2.0);
        t1.scale(5.0);
        System.out.println("t1 = t1.scale(t2); = "+t1);

        t1 = new Tensor2d(5, 2.0);
        t2 = new Tensor2d(5, 3.0);
        t1.emul(t2);
        System.out.println("t1.emul(t2); = "+t1);

        t1 = new Tensor2d(5, 2.0);
        t2 = new Tensor2d(5, 3.0);
        t1 = t1.add(t2);
        System.out.println("t1 = t1.add(t2); = "+t1);

        t1 = new Tensor2d(5, 2.0);
        t2 = new Tensor2d(5, 3.0);
        t1 = t1.sub(t2);
        System.out.println("t1 = t1.sub(t2); = "+t1);

        t1 = new Tensor2d(5,4, 2.0);
        t2 = new Tensor2d(4,3, 3.0);
        t1 = t1.mul(t2);
        System.out.println("t1 = t1.mul(t2); = "+t1);
         
        // test exception
        try
        {
            t1 = new Tensor2d(5,4, 2.0);
            t2 = new Tensor2d(4,3, 3.0);
            t1.transform(t2);
        }
        catch (IllegalArgumentException e)
        {
            System.out.println("caught it:"+ e);
        }
        try
        {
            t1 = new Tensor2d(5,5, 2.0);
            t2 = new Tensor2d(4,3, 3.0);
            t1.transform(t2);
        }
        catch (IllegalArgumentException e)
        {
            System.out.println("caught it:"+ e);
        }

        t1 = new Tensor2d(5,5, 2.0);
        t2 = new Tensor2d(4,3, 3.0);
        try
        {
            t1.mul(t2);
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

        Tensor2d hg = new Tensor2d(TwoScaleCoeffs.getCoeffs(10));
        Tensor2d hgT = hg.transpose();
        t1 = Tensor2d.rand(hg.getDim0());
        t2 = t1.transform(hg);
        t3 = t1.transform_slow(hg);
        t3 = t3.sub(t2);
        System.out.println("t3.sub(t2).normf()="+t3.normf());
        
    }

}
