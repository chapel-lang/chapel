import java.util.*;
import java.text.*;

/** Multiresolution representation of 1-d functions using a multiwavelet
    basis (similar to discontinuous spectral element with a hierarchal
    decomposition).
    
    The minimal functionality is
    
    init() (which calls init_twoscale, init_quadrature and refine),
    refine() (which calls project),
    compress(),
    and reconstruct()
*/
public class Function1d {

    // Provide defaults in the class to make it easier to have all instances
    // be consistent.    
    private static int autorefine = 1; 

    // per instance vars
    // use first k Legendre polynomials as the basis in each box
    private int k = 3;
    // truncation threshold for small wavelet coefficients
    private double thresh = 1e-5;
    // analytic function f(x) to be projected into the numerical represntation
    private Fn1d f = null;
    // initial level of refinement
    private int initial_level = 2;
    // maximum level of refinement mostly as a sanity check
    private int max_level = 30;
    // keep track of what basis we are in
    private int compressed = 0;

    // the adaptively refined coefficients (s for scaling function or polynomials,
    // and d for wavelets) are represented using HashMap[]
    // s[n] -> HashMap of coefficients at level n
    // s[n][l] -> if it exists is a vector of coefficients on level n box l

    // hashes for sum and difference coeffs
    private HashMap[] s = new HashMap[max_level+1];
    private HashMap[] d = new HashMap[max_level+1];

    // two scale relationship matrices
    private Tensor2d hg;
    private Tensor2d hgT;

    // Quadrature stuff
    private int quad_npt;
    private Tensor1d quad_x;    // points
    private Tensor1d quad_w;    // weights
    private Tensor2d quad_phi;  // phi[point,i]
    private Tensor2d quad_phiT; // phi[point,i] transpose
    private Tensor2d quad_phiw; // phi[point,i]*weight[point]
    
    // blocks of the block tridiagonal derivative operator
    private Tensor2d rm;
    private Tensor2d ro;
    private Tensor2d rp;
    
    // constructors and helpers

    private void init(){

        // create empty HashMaps for s and d
        for(int i = 0; i < this.max_level+1; i++)
        {
            this.s[i] = new HashMap();
            this.d[i] = new HashMap();
        }

        // initialize two scale relationship matrices
        init_twoscale(this.k);
        // initialize quatrature rule
        init_quadrature(this.k);

        // blocks of the block tridiagonal derivative operator
        // self.rm, self.r0, self.rp = make_dc_periodic(k)

        // initial refinement of analytic function f(x)
        if(this.f != null)
            // remember: 1<<initial_level == 2**initial_level
            for(int l = 0; l < 1<<this.initial_level; l++)
                this.refine(this.initial_level,l);
        
    }
    
    private void init_twoscale(int k){
        this.hg = new Tensor2d(TwoScaleCoeffs.getCoeffs(this.k));
        this.hgT = hg.transpose();
    }

    private void init_quadrature(int order){
        this.quad_x = new Tensor1d(Quadrature.getGLPoints(order));
        double[] quad_x = this.quad_x.getArray();
        this.quad_w = new Tensor1d(Quadrature.getGLWeights(order));
        double[] quad_w = this.quad_w.getArray();
        this.quad_npt = this.quad_w.getN();
        this.quad_phi  = new Tensor2d(this.quad_npt, this.k);
        double[][] quad_phi = this.quad_phi.getArray();
        this.quad_phiw = new Tensor2d(this.quad_npt, this.k);
        double[][] quad_phiw = this.quad_phiw.getArray();

        for(int i = 0; i < this.quad_npt; i++)
        {
            double[] p = TwoScaleCoeffs.phi(quad_x[i],this.k);
            for(int m = 0; m < this.k; m++)
            {
                quad_phi[i][m] = p[m];
                quad_phiw[i][m] = quad_w[i]*p[m];
            }
        }
        this.quad_phiT = this.quad_phi.transpose();
    }

    /** Constructor no args
     */
    Function1d(){
        init();
    }

    /** Constructor takes analytic function
     */
    Function1d(Fn1d f){
        this.f = f;

        init();
    }

    /** Constructor takes more args
     */
    Function1d(int k, double thresh, Fn1d f){
        this.k = k;
        this.thresh = thresh;
        this.f = f;

        init();
    }

    /** s[n][l] = integral(phi[n][l](x) * f(x))
        for box (n,l) project f(x) using quadrature rule
        into scaling function basis
        @param n   level
        @param l   box index
        @return    s[n][l] = integral(phi[n][l](x) * f(x))
    */
    public Tensor1d project(int n, int l){
        Tensor1d s = new Tensor1d(this.k);
        double[] A = s.getArray();
        double[] quad_x = this.quad_x.getArray();
        double[][] quad_phiw = this.quad_phiw.getArray();
        double h = Math.pow(0.5,n);
        double scale = Math.sqrt(h);
       for(int mu = 0; mu < this.quad_npt; mu++)
        {
            double x = (l + quad_x[mu]) * h;
            double f = this.f.f(x);
            for(int i = 0; i < this.k; i++)
                A[i] += scale * f * quad_phiw[mu][i];
        }
        return s;
    }

    /** refine numerical representation of f(x) to desired tolerance
        @param n   level
        @param l   box index
    */
    public void refine(int n, int l){

        int k = this.k;
        
        // project f(x) at next level
        Tensor1d s0 = this.project(n+1, 2*l);
        Tensor1d s1 = this.project(n+1, 2*l+1);
        Tensor1d s = new Tensor1d(2*k);
        s.setTensor1d(0,   k, s0);
        s.setTensor1d(k, 2*k, s1);

        // apply the two scale relationship to get difference coeff
        // in 1d this is O(k^2) flops
        Tensor1d d = s.transform(this.hgT);

        // check to see if within tolerance
        // normf() is Frobenius norm == 2-norm for vectors
        double normf = d.getTensor1d(k,2*k).normf();
        if((normf < this.thresh) || (n >= (this.max_level-1)))
        {
            // HashMap.put(Object key, Object value)
            this.s[n+1].put(new Integer(2*l  ), s0);
            this.s[n+1].put(new Integer(2*l+1), s1);
        }
        else
        {
            // these recursive calls on sub-trees can go in parallel
            // if the HashMap is syncronized
            this.refine(n+1, 2*l  );
            this.refine(n+1, 2*l+1);
        }
    }

    /** change from scaling function basis to multi-wavelet basis (s -> d)
        tree is filled out with s[0][0] and d
        @param n   level
        @param l   box index
    */
    public void compress(int n, int l){
        if(this.compressed == 1) return;
        
        Integer twol   = new Integer(2*l  );
        Integer twolp1 = new Integer(2*l+1);
        
        // sub-trees can be done in parallel
        // if the HashMap is syncronized
        if(!this.s[n+1].containsKey(twol  )) this.compress(n+1, 2*l  );
        if(!this.s[n+1].containsKey(twolp1)) this.compress(n+1, 2*l+1);

        int k = this.k;
        Tensor1d s = new Tensor1d(2*k);
        s.setTensor1d(0,   k, (Tensor1d)this.s[n+1].get(twol  ));
        s.setTensor1d(k, 2*k, (Tensor1d)this.s[n+1].get(twolp1));

        // apply the two scale relationship to get difference coeff
        // in 1d this is O(k^2) flops (in 3d this is O(k^4) flops)
        
        Tensor1d d = s.transform(this.hgT);

        Integer L = new Integer(l);
        this.s[n].put(L, d.getTensor1d(0,  k));
        this.d[n].put(L, d.getTensor1d(k,2*k));

        this.s[n+1].remove(twol  );
        this.s[n+1].remove(twolp1);
        
        if(n == 0) this.compressed = 1;
    }

    /** change from multi-wavelet basis to scaling function basis (d -> s)
        tree just has s at leaves
        @param n   level
        @param l   box index
    */
    public void reconstruct(int n, int l){
        if(this.compressed == 0) return;

        Integer L = new Integer(l);

        if(this.d[n].containsKey(L))
        {
            int k = this.k;
            Tensor1d d = new Tensor1d(2*k);
            d.setTensor1d(0,   k, (Tensor1d)this.s[n].get(L));
            d.setTensor1d(k, 2*k, (Tensor1d)this.d[n].get(L));
            this.s[n].remove(L);
            this.d[n].remove(L);

            // apply the two scale relationship to get difference coeff
            // in 1d this is O(k^2) flops (in 3d this is O(k^4) flops)
            Tensor1d s = d.transform(this.hg);

            this.s[n+1].put(new Integer(2*l  ), s.getTensor1d(0,   k));
            this.s[n+1].put(new Integer(2*l+1), s.getTensor1d(k, 2*k));
            
            // sub-trees can be done in parallel
            // if the HashMap is syncronized
            this.reconstruct(n+1, 2*l);
            this.reconstruct(n+1, 2*l+1);
        }

        if(n == 0) this.compressed = 0;
    }

    
    /** Mostly for debugging, print summary of coefficients,
        optionally printing the norm of each block
        @param printcoeff  print all coeff
    */
    public void summarize(int printcoeff){

        DecimalFormat rfmt = new DecimalFormat("0.00E00");
        DecimalFormat ifmt = new DecimalFormat("000");
        System.out.println("-----------------------------------------------------");
        System.out.println("sum coefficients");
        for(int n = 0; n <= max_level; n++)
        {
            double sum = 0.0;
            for(Iterator L = s[n].keySet().iterator(); L.hasNext(); )
            {
                double t = ((Tensor1d)s[n].get(L.next())).normf();
                sum += t*t;
            }
            if(s[n].size() != 0)
                System.out.println("   level "+ifmt.format(n)
                                   +"   #boxes="+ifmt.format(s[n].size())
                                   +"  norm="+rfmt.format(Math.sqrt(sum)));
        }

        System.out.println("difference coefficients");
        for(int n = 0; n <= max_level; n++)
        {
            double sum = 0.0;
            for(Iterator L = d[n].keySet().iterator(); L.hasNext(); )
            {
                Integer l = (Integer)L.next();
                double t = ((Tensor1d)d[n].get(l)).normf();
                sum += t*t;
            }
            if(d[n].size() != 0)
                System.out.println("   level "+ifmt.format(n)
                                   +"   #boxes="+ifmt.format(d[n].size())
                                   +"  norm="+rfmt.format(Math.sqrt(sum)));
        }
        

    }
    
    
    // test things out to see if they work
    // this method gets run if you run "java thisclass"
    public static void main (String argv[]) {

        int k = 5;
        double thresh = 1e-5;
        
        Function1d f = new Function1d(k,thresh,MadFn1d.test1);
        System.out.println("test1:");
        f.summarize(0);
        f.compress(0,0);
        System.out.println("test1: after compression");
        f.summarize(0);
        f.reconstruct(0,0);
        System.out.println("test1: after reconstruction");
        f.summarize(0);
        
        f = new Function1d(k,thresh,MadFn1d.dtest1);
        System.out.println("dtest1:");
        f.summarize(0);
        f.compress(0,0);
        System.out.println("dtest1: after compression");
        f.summarize(0);
        f.reconstruct(0,0);
        System.out.println("dtest1: after reconstruction");
        f.summarize(0);

        f = new Function1d(k,thresh,MadFn1d.test2);
        System.out.println("test2:");
        f.summarize(0);
        f.compress(0,0);
        System.out.println("test2: after compression");
        f.summarize(0);
        f.reconstruct(0,0);
        System.out.println("test2: after reconstruction");
        f.summarize(0);

        f = new Function1d(k,thresh,MadFn1d.dtest2);
        System.out.println("dtest2:");
        f.summarize(0);
        f.compress(0,0);
        System.out.println("dtest2: after compression");
        f.summarize(0);
        f.reconstruct(0,0);
        System.out.println("dtest2: after reconstruction");
        f.summarize(0);

        f = new Function1d(k,thresh,MadFn1d.test3);
        System.out.println("test3:");
        f.summarize(0);
        f.compress(0,0);
        System.out.println("test3: after compression");
        f.summarize(0);
        f.reconstruct(0,0);
        System.out.println("test3: after reconstruction");
        f.summarize(0);

        f = new Function1d(k,thresh,MadFn1d.dtest3);
        System.out.println("dtest3:");
        f.summarize(0);
        f.compress(0,0);
        System.out.println("dtest3: after compression");
        f.summarize(0);
        f.reconstruct(0,0);
        System.out.println("dtest3: after reconstruction");
        f.summarize(0);

    }


}

