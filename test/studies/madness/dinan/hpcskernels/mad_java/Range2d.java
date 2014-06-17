

public class Range2d {

    public Range1d[] d = new Range1d[2];

    // constructors

    /** Construct a Range2d length n = ([0,n),[0,n))
        @param n    length of both dim0 and dim1
    */
    public Range2d(int n){
        this.d[0] = new Range1d(n);
        this.d[1] = new Range1d(n);
    }

    /** Construct a Range2d ([0,n0),[0,n1))
        @param n0   length dim0
        @param n1   length dim1
    */
    public Range2d(int n0, int n1){
        this.d[0] = new Range1d(n0);
        this.d[1] = new Range1d(n1);
    }

    /** Construct a Range2d ([s0,e0),[s1,e1))
        @param s0    start dim0
        @param e0    end dim0(exclusive)
        @param s1    start dim1
        @param e1    end dim1 (exclusive)
    */
    public Range2d(int s0, int e0, int s1, int e1){
        this.d[0] = new Range1d(s0,e0);
        this.d[1] = new Range1d(s1,e1);
    }

    /** Construct a Range2d ([r0.s,r0.e),[r1.s,r1.e))
        @param r0    Range1d dim0
        @param r1    Range1d dim1
    */
    public Range2d(Range1d r0, Range1d r1){
        this.d[0] = new Range1d(r0.s,r0.e);
        this.d[1] = new Range1d(r1.s,r1.e);
    }

    /** Split each dimension  
        @return    this split
    */
    public Range2d[][] split(){
        Range1d[][] r = new Range1d[2][2];
        Range2d[][] ret = new Range2d[2][2];

        // loop over dimensions splitting each one
        for(int i = 0; i < 2; i++)
            r[i] = this.d[i].split();
        
        // cartesian product of 1d ranges should be 2^2
        for(int i = 0; i < 2; i++)
            for(int j = 0; j < 2; j++)
                ret[i][j] = new Range2d(r[0][i],r[1][j]);
        return ret;
    }

    public String toString(){
        String r = "("+this.d[0]+", "+this.d[1]+")";
        return r;
    }

    public static void main(String argv[]){
        Range2d f = new Range2d(4);
        System.out.println(f);
        Range2d[][] s = f.split();
        for(int i = 0; i < 2; i++)
            for(int j = 0; j < 2; j++)
                System.out.print("("+i+j+")"+s[i][j]);
        System.out.println("");        
    }
}

    
