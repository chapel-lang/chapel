
public class Range1d {

    public int s;
    public int e;

    // constructors

    /** Construct a Range1d length n = [0,n)
        @param n    length
    */
    public Range1d(int n){
        if(n > 0)
        {
            this.s = 0;
            this.e = n;
        }
        else
            throw new IllegalArgumentException("Range1d(n): need (n > 0)");
    }

    /** Construct a Range1d [start,end)
        @param s    start
        @param e    end (exclusive)
    */
    public Range1d(int s, int e){
        if((s <= e) && (s >= 0))
        {
            this.s = s;
            this.e = e;
        }
        else
            throw new IllegalArgumentException("Range1d(s,e): need ((s <= e) && (s >= 0))");
    }

    /** Split in half r0 = [s,s+(e-s)/2) and r1 = [s+(e-s)/2,e) 
        @return    this split in half
    */
    public Range1d[] split(){
        Range1d[] ret = new Range1d[2];
        ret[0] = new Range1d(this.s, this.s+((this.e-this.s)/2));
        ret[1] = new Range1d(this.s+((this.e-this.s)/2), this.e);
        return ret;
    }

    public String toString(){
        String r = "["+this.s+", "+this.e+")";
        return r;
    }

    public static void main(String argv[]){
        Range1d f = new Range1d(8);
        System.out.println(f);
        Range1d[] s = f.split();
        for(int i = 0; i < 2; i++)
                System.out.print(s[i]);
        System.out.println("");
    }


}

