

public class Range3d {

    public Range1d[] d = new Range1d[3];

    // constructors

    /** Construct a Range3d length n = ([0,n),[0,n),[0,n))
        @param n    length of both dim0, dim1, and dim2
    */
    public Range3d(int n){
        this.d[0] = new Range1d(n);
        this.d[1] = new Range1d(n);
        this.d[2] = new Range1d(n);
    }

    /** Construct a Range3d ([0,d0),[0,d1),[0,d2))
        @param d0   length dim0
        @param d1   length dim1
        @param d2   length dim2
    */
    public Range3d(int d0, int d1, int d2){
        this.d[0] = new Range1d(d0);
        this.d[1] = new Range1d(d1);
        this.d[2] = new Range1d(d2);
    }

    /** Construct a Range3d ([s0,e0),[s1,e1),[s2,e2))
        @param s0    start dim0
        @param e0    end dim0(exclusive)
        @param s1    start dim1
        @param e1    end dim1 (exclusive)
        @param s2    start dim2
        @param e2    end dim2 (exclusive)
    */
    public Range3d(int s0, int e0, int s1, int e1, int s2, int e2){
        this.d[0] = new Range1d(s0,e0);
        this.d[1] = new Range1d(s1,e1);
        this.d[2] = new Range1d(s2,e2);
    }

    /** Construct a Range3d ([r0.s,r0.e),[r1.s,r1.e),[r2.s,r2.e))
        @param r0    Range1d dim0
        @param r1    Range1d dim1
        @param r2    Range1d dim2
    */
    public Range3d(Range1d r0, Range1d r1, Range1d r2){
        this.d[0] = new Range1d(r0.s,r0.e);
        this.d[1] = new Range1d(r1.s,r1.e);
        this.d[2] = new Range1d(r2.s,r2.e);
    }

    /** Split each dimension  
        @return    this split
    */
    public Range3d[][][] split(){
        Range1d[][] r = new Range1d[3][2];
        Range3d[][][] ret = new Range3d[2][2][2];

        // loop over dimensions splitting each one
        for(int i = 0; i < 3; i++)
            r[i] = this.d[i].split();
        
        // cartesian product of 1d ranges should be 2^3
        for(int i = 0; i < 2; i++)
            for(int j = 0; j < 2; j++)
                for(int k = 0; k < 2; k++)
                ret[i][j][k] = new Range3d(r[0][i],r[1][j],r[2][k]);
        return ret;
    }

    public String toString(){
        String r = "("+this.d[0]+", "+this.d[1]+", "+this.d[2]+")";
        return r;
    }

    public static void main(String argv[]){
        Range3d f = new Range3d(4);
        System.out.println(f);
        Range3d[][][] s = f.split();
        for(int i = 0; i < 2; i++)
            for(int j = 0; j < 2; j++)
                for(int k = 0; k < 2; k++)
                System.out.print("("+i+j+k+")"+s[i][j][k]);
        System.out.println("");        
    }
}

    
