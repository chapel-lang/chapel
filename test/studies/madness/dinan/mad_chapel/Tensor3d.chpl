/** Tensor3d class implements three-dimensional tensor/matrix operations
 */
public class Tensor3d {

    private double[][][] A;
    private int d0;
    private int d1;
    private int d2;
    
    // constructors

    /** Construct a Tensor3d of length n filled with zeroes
        @param n    length dim0 and dim1
    */
    public Tensor3d(int n) {
        this.A = new double[n][n][n];
        this.d0 = n;
        this.d1 = n;
        this.d2 = n;
        for (int i = 0; i < this.d0; i++)
            for (int j = 0; j < this.d1; j++)
                for (int k = 0; k < this.d2; k++)
                    this.A[i][j][k] = 0.0;
    }

    /** Construct a Tensor3d of length n filled with zeroes
        @param n    length dim0 and dim1
        @param val  value to fill with
    */
    public Tensor3d(int n, double val) {
        this.A = new double[n][n][n];
        this.d0 = n;
        this.d1 = n;
        this.d2 = n;
        for (int i = 0; i < this.d0; i++)
            for (int j = 0; j < this.d1; j++)
                for (int k = 0; k < this.d2; k++)
                    this.A[i][j][k] = val;
    }

    /** Construct a Tensor3d of (d0 x d1 x d2) filled with zeroes
        @param d0    length dim0
        @param d1    length dim1
        @param d2    length dim2        
    */
    public Tensor3d(int d0, int d1, int d2) {
        this.A = new double[d0][d1][d2];
        this.d0 = d0;
        this.d1 = d1;
        this.d2 = d2;
        for (int i = 0; i < this.d0; i++)
            for (int j = 0; j < this.d1; j++)
                for (int k = 0; k < this.d2; k++)
                    this.A[i][j][k] = 0.0;
    }
    
    /** Construct a Tensor3d of (d0 x d1 x d2) filled with val
        @param d0    length dim0
        @param d1    length dim1
        @param d2    length dim2        
        @param val   value to fill with   
    */
    public Tensor3d(int d0, int d1, int d2, double val) {
        this.A = new double[d0][d1][d2];
        this.d0 = d0;
        this.d1 = d1;
        this.d2 = d2;
        for (int i = 0; i < this.d0; i++)
            for (int j = 0; j < this.d1; j++)
                for (int k = 0; k < this.d2; k++)
                    this.A[i][j][k] = val;
    }
        
    /** Construct a Tensor3d from a 3-d array.
        @param A    Three-dimensional array of doubles.
        @exception  IllegalArgumentException dim same length?
    */
    public Tensor3d(double[][][] A) {
        this.d0 = A.length;
        this.d1 = A[0].length;
        this.d2 = A[0][0].length;
        this.A = new double[this.d0][this.d1][this.d2];
        for (int i = 0; i < this.d0; i++)
        {
            if(A[i].length == this.d1)
                for (int j = 0; j < this.d1; j++)
                {
                    if(A[i][j].length == this.d2)
                        for (int k = 0; k < this.d2; k++)
                            this.A[i][j][k] = A[i][j][k];
                    else
                        throw new IllegalArgumentException("Tensor3d(double[][][] A): dim2 same length?");
                }
            else
                throw new IllegalArgumentException("Tensor3d(double[][][] A): dim1 same length?");
        }
    }

    // public methods

    /** Copy
        @return      new Tensor3d which contains copy of matrix
    */
    public Tensor3d copy(){
        return new Tensor3d(this.getArray()); 
    }

    /** rand
        @return      new Tensor3d which contains random entries between 0 and 1
    */
    public static Tensor3d rand(int n){
        Tensor3d ret = new Tensor3d(n);
        double[][][] B = ret.getArray();

        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                for (int k = 0; k < n; k++)
                B[i][j][k] = Math.random();

        return ret;
    }
    
    /** get array assoc with Tensor3d
        @return      array assoc with Tensor3d
    */
    public double[][][] getArray(){
        return this.A;
    }

    /** get dim0 size
        @return       dim0 size of Tensor3d
    */
    public int getDim0(){
        return this.d0;
    }

    /** get dim1 size
        @return       dim1 size of Tensor3d
    */
    public int getDim1(){
        return this.d1;
    }

    /** get dim2 size
        @return       dim2 size of Tensor3d
    */
    public int getDim2(){
        return this.d2;
    }

    /** get element (i,j,k)
        @param  i    dim0 index
        @param  j    dim1 index
        @param  k    dim2 index
        @return      value of element (i,j,k)
    */
    public double get(int i, int j, int k){
        return this.A[i][j][k];
    }

    /** Get elements from this[s0:e0][s1:e1][s2:e2]
        @param s0    start of range to copy into, dim0
        @param e0    end of range to copy into, dim0
        @param s1    start of range to copy into, dim1
        @param e1    end of range to copy into, dim1
        @param s2    start of range to copy into, dim2
        @param e2    end of range to copy into, dim2
        @return      new Tensor3d which contains sub-block of matrix
    */
    public Tensor3d getTensor3d(int s0, int e0, int s1, int e1, int s2, int e2){
        int d0 = e0 - s0;
        int d1 = e1 - s1;
        int d2 = e2 - s2;
        Tensor3d ret = new Tensor3d(d0,d1,d2);
        double[][][] B = ret.getArray();
        for (int i = 0; i < d0; i++)
            for (int j = 0; j < d1; j++)
                for (int k = 0; k < d2; k++)
                B[i][j][k] = this.A[i+s0][j+s1][k+s2];
        return ret;
    }

    /** Get elements from this[range.d[0].s:range.d[0].e][range.d[1].s:range.d[1].e]
        [range.d[2].s:range.d[2].e]
        @param range Range3d
        @return      new Tensor3d which contains sub-block of matrix
    */
    public Tensor3d getTensor2d(Range3d range){
        return getTensor3d(range.d[0].s, range.d[0].e,
                           range.d[1].s, range.d[1].e,
                           range.d[2].s, range.d[2].e);
    }
    
    /** Set element this[i][j][k] to val
        @param i     index dim0
        @param j     index dim1       
        @param k     index dim2      
        @param val   value    
        @return      this so operations can be chained
    */
    public Tensor3d set(int i, int j, int k, double val){
        this.A[i][j][k] = val;
        return this;
    }
    
    /** Set elements of this[start to end-1] to val
        @param s0     start of range to copy into
        @param e0     end of range to copy into
        @param s1     start of range to copy into
        @param e1     end of range to copy into
        @param s2     start of range to copy into
        @param e2     end of range to copy into
        @param val   value
        @return      this so operations can be chained
    */
    public Tensor3d set(int s0, int e0, int s1, int e1, int s2, int e2, double val){
        for (int i = s0; i < e0; i++)
            for (int j = s1; j < e1; j++)
                for (int k = s2; k < e2; k++)
                this.A[i][j][k] = val;
        return this;
    }
    
    /** Set all to val
        @param val   value
        @return      this so operations can be chained
    */
    public Tensor3d setFill(double val){
        return this.set(0,this.d0,0,this.d1,0,this.d2,val);
    }
    
    /** Set elements of this[s0:e0][s1:e1][s2:e2] from other[0:e0-s0][0:e1-s1][0:e2-s2]
        @param s0     start of range to copy into
        @param e0     end of range to copy into
        @param s1     start of range to copy into
        @param e1     end of range to copy into
        @param s2     start of range to copy into
        @param e2     end of range to copy into
        @param other another Tensor3d
        @return      this so operations can be chained
    */
    public Tensor3d setTensor3d(int s0, int e0, int s1, int e1, int s2, int e2, Tensor3d other){
        int d0 = e0 - s0;
        int d1 = e1 - s1;
        int d2 = e2 - s2;        
        double[][][] B = other.getArray();
        for (int i = 0; i < d0; i++)
            for (int j = 0; j < d1; j++)
                for (int k = 0; k < d2; k++)
                this.A[i+s0][j+s1][k+s2] = B[i][j][k];
        return this;
    }
    
    /** Set elements of this[s0:e0][s1:e1][s2:e2] from other[0:e0-s0][0:e1-s1][0:e2-s2]
        @param range  Range3d range to copy into
        @param other  another Tensor3d
        @return      this so operations can be chained
    */
    public Tensor3d setTensor3d(Range3d range, Tensor3d other){
        return this.setTensor3d(range.d[0].s, range.d[0].e,
                                range.d[1].s, range.d[1].e,
                                range.d[2].s, range.d[2].e, other);
    }
    
    /** Frobenius norm of this
        @return      sqrt of the sum of the squares of all elements
    */
    public double normf(){
        double sum = 0.0;
        for (int i = 0; i < this.d0; i++)
            for (int j = 0; j < this.d1; j++)
                for (int k = 0; k < this.d2; k++)
                    sum += this.A[i][j][k] * this.A[i][j][k];
        return Math.sqrt(sum);
    }

    /** Transform this by other.
        @param other a transform matrix Tensor2d
        @return      new Tensor3d = this transformed by other
        @exception  IllegalArgumentException A must be square and C and A dims must match
    */
    public Tensor3d transform_slow(Tensor2d other){
        
        if((this.d0 != this.d1) && (this.d0 != this.d2))
            throw new IllegalArgumentException("Tensor3d:transform: A must be square");
        double[][] C = other.getArray();
        int c_d0 = other.getDim0();
        if(c_d0 != this.d0)
            throw new IllegalArgumentException("Tensor3d:transform: C and A dims must match");
        int c_d1 = other.getDim1();        
        Tensor3d ret = new Tensor3d(c_d1);
        double[][][] R = ret.getArray();
        
        // transform
        // result(i,j,k...) <-- sum(i',j', k',...) A(i',j',k',...) C(i',i) C(j',j) C(k',k)
        // The input dimensions of A must all be the same and agree with the first dimension of C
        for (int i = 0; i < c_d0; i++)
            for (int j = 0; j < c_d0; j++)
                for (int k = 0; k < c_d0; k++)
                {
                    R[i][j][k] = 0.0;
                    for (int ip = 0; ip < c_d1; ip++)
                        for (int jp = 0; jp < c_d1; jp++)
                            for (int kp = 0; kp < c_d1; kp++)
                                R[i][j][k] += A[ip][jp][kp] * C[ip][i] * C[jp][j] * C[kp][k];
                }
        
        return ret;
    }

    /** Transform this by other.
        result(i,j,k) <-- sum(i',j',k') A(i',j') C(i',i) C(j',j) C(k',k)
        @param other a transform matrix Tensor2d
        @return      new Tensor3d = this transformed by other
        @exception  IllegalArgumentException A must be square and C and A dims must match
    */
    public Tensor3d transform(Tensor2d other){
        
        if((this.d0 != this.d1) && (this.d0 != this.d2))
            throw new IllegalArgumentException("Tensor3d:transform: A must be square");
        double[][] C = other.getArray();
        int c_d0 = other.getDim0();
        if(c_d0 != this.d0)
            throw new IllegalArgumentException("Tensor3d:transform: C and A dims must match");
        int c_d1 = other.getDim1();
        Tensor3d ret = new Tensor3d(c_d1);
        double[][][] R = ret.getArray();
        
        // transform
        // result(i,j,k...) <-- sum(i',j', k',...) A(i',j',k',...) C(i',i) C(j',j) C(k',k)
        // The input dimensions of A must all be the same and agree with the first dimension of C

        double[][] v_jpkpi = new double[c_d0][c_d0];
        double[] v_kpij = new double[c_d0];
        double v_ijk;
        
        // Transform along 1st dimension
        for (int i = 0; i < c_d1; i++)
        {
            for (int kp = 0; kp < c_d0; kp++)
            {
                for (int jp = 0; jp < c_d0; jp++)
                {
                    v_jpkpi[jp][kp] = 0.0;
                    for (int ip = 0; ip < c_d0; ip++)
                        v_jpkpi[jp][kp] += A[ip][jp][kp] * C[ip][i];
                }
            }
            
            // Transform along 2nd dimension
            for (int j = 0; j < c_d1; j++)
            {
                for (int kp = 0; kp < c_d0; kp++)
                {
                    v_kpij[kp] = 0.0;
                    for (int jp = 0; jp < c_d0; jp++)
                        v_kpij[kp] +=  v_jpkpi[jp][kp] * C[jp][j];
                }
                
                // Transform along 3rd dimension
                for (int k = 0; k < c_d1; k++)
                {
                    v_ijk = 0.0;
                    for (int kp = 0; kp < c_d0; kp++)
                        v_ijk +=  v_kpij[kp] * C[kp][k];
                    
                    R[i][j][k] = v_ijk;
                }
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
    public Tensor3d gaxpy(double alpha, Tensor3d other, double beta){
        
        if((this.d0 != other.getDim0()) &&
           (this.d1 != other.getDim1()) &&
           (this.d2 != other.getDim2()))
            throw new IllegalArgumentException("Tensor3d:gaxpy: dims must match");

        double[][][] B = other.getArray();
        
        for(int i = 0; i < this.d0;  i++)
            for(int j = 0; j < this.d1;  j++)
                for(int k = 0; k < this.d2;  k++)
                this.A[i][j][k] = alpha*this.A[i][j][k] + beta*B[i][j][k];
        
        return this;
    }
    
    /** scale this by alpha, modifies this, this = alpha*this
        @param alpha scaling factor for this
        @return      this so operations can be chained
    */
    public Tensor3d scale(double alpha){
        
        for(int i = 0; i < d0;  i++)
            for(int j = 0; j < d1;  j++)
                for(int k = 0; k < this.d2;  k++)
                    A[i][j][k] = alpha*A[i][j][k];

        return this;
    }

    /** elementwise multiply
        @param other another Tensor3d
        @return      this so operations can be chained
        @exception  IllegalArgumentException dims must match
    */
    public Tensor3d emul(Tensor3d other){
        
        if((this.d0 != other.getDim0()) &&
           (this.d1 != other.getDim1()) &&
           (this.d2 != other.getDim2()))
            throw new IllegalArgumentException("Tensor3d:emul: dims must match");
        
        double[][][] B = other.getArray();
        
        for(int i = 0; i < d0;  i++)
            for(int j = 0; j < d1; j++)
                for(int k = 0; k < this.d2;  k++)
                    A[i][j][k] *= B[i][j][k];
        
        return this;
    }

    /** addition
        @param other another Tensor3d
        @return      new Tensor3d = this + other
    */
    public Tensor3d add(Tensor3d other){
        return this.copy().gaxpy(1.0,other,1.0);
    }

    /** Subtraction
        @param other another Tensor3d
        @return      new Tensor3d = this - other
    */
    public Tensor3d sub(Tensor3d other){
        return this.copy().gaxpy(1.0,other,-1.0);
    }

    /** convert Tensor3d to a String
        @return      String representation of this
    */
    public String toString(){
        String r = super.toString()+"\n{";
        for (int i = 0; i < this.d0-1; i++)
        {
            r += "{";
            for (int j = 0; j < this.d1; j++)
            {
                r += "{";
                for (int k = 0; k < this.d2; k++)
                    r += this.A[i][j][k] + ", ";
                r += "},\n";
            }
        }
        r += "{";
        for (int j = 0; j < this.d1; j++)
        {
            r += "{";
            for (int k = 0; k < this.d2; k++)
                r += this.A[this.d0-1][j][k] + ", ";
            r += "},\n";
        }
        r += "}}}";
        return r;
    }

    // test things out to see if they work
    // this method gets run if you run "java thisclass"
    public static void main (String argv[]) {

        Tensor3d t1;
        Tensor3d t2;
        Tensor3d t3;

        t1 = new Tensor3d(5);
        System.out.println("t1 = new Tensor3d(5); = "+t1);
        
        t1 = new Tensor3d(5, 1.0);
        System.out.println("t1 = new Tensor3d(5, 1.0); = "+t1);

        t1 = new Tensor3d(5, 4, 3);
        System.out.println("t1 = new Tensor3d(5, 4); = "+t1);

        t1 = new Tensor3d(5, 4, 3, 1.0);
        System.out.println("t1 = new Tensor3d(5, 4, 1.0); = "+t1);

        t1 = new Tensor3d(new double[5][3][4]);
        t1.setFill(0.0);
        System.out.println("t1 = new Tensor3d(new double[3][4]);t1.setFill(0.0) = "+t1);

        t1 = new Tensor3d(5,4,3, 2.0);
        t1.scale(5.0);
        System.out.println("t1 = t1.scale(t2); = "+t1);

        t1 = new Tensor3d(5, 2.0);
        t2 = new Tensor3d(5, 3.0);
        t1.emul(t2);
        System.out.println("t1.emul(t2); = "+t1);

        t1 = new Tensor3d(5, 2.0);
        t2 = new Tensor3d(5, 3.0);
        t1 = t1.add(t2);
        System.out.println("t1 = t1.add(t2); = "+t1);

        t1 = new Tensor3d(5, 2.0);
        t2 = new Tensor3d(5, 3.0);
        t1 = t1.sub(t2);
        System.out.println("t1 = t1.sub(t2); = "+t1);

         
        // test exception
        try
        {
            t1 = new Tensor3d(5,4,3, 2.0);
            Tensor2d t10 = new Tensor2d(4,3, 3.0);
            t1.transform(t10);
        }
        catch (IllegalArgumentException e)
        {
            System.out.println("caught it:"+ e);
        }
        try
        {
            t1 = new Tensor3d(5,5,5, 2.0);
            Tensor2d t10 = new Tensor2d(4,3, 3.0);
            t1.transform(t10);
        }
        catch (IllegalArgumentException e)
        {
            System.out.println("caught it:"+ e);
        }

        t1 = new Tensor3d(5,5,5, 2.0);
        t2 = new Tensor3d(4,3,4, 3.0);
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
        t1 = Tensor3d.rand(hg.getDim0());
        long s = System.currentTimeMillis();
        t2 = t1.transform(hg);
        long e = System.currentTimeMillis();
        System.out.println("fast="+((2.0*3.0*(10.0*10.0*10.0*10.0) / ((double)(e-s)*1e-3)))+"flop/s");
        s = System.currentTimeMillis();
        t3 = t1.transform_slow(hg);
        e = System.currentTimeMillis();
        System.out.println("slow="+((2.0*(10.0*10.0*10.0*10.0*10.0*10.0) / ((double)(e-s)*1e-3)))+"flop/s");
        t3 = t3.sub(t2);
        System.out.println("t3.sub(t2).normf()="+t3.normf());
        
    }

}
