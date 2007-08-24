use Math;
use Tensor;
use FTree;
use Quadrature;
use TwoScale;
use Fn1d;

class Function1d {
    var k             = 5;    // use first k Legendre polynomials as the basis in each box
    var thresh        = 1e-5; // truncation threshold for small wavelet coefficients
    var f: Fn1d       = nil;  // analytic function f(x) to be projected into the numerical represntation
    var initial_level = 2;    // initial level of refinement
    var max_level     = 30;   // maximum level of refinement mostly as a sanity check
    var compressed    = false;// keep track of what basis we are in

    // Sum and Difference coefficients
    var s = FTree(order=k);
    var d = FTree(order=k);

    // Two-Scale relationship matrices
    var hgDom = [0..2*k-1, 0..2*k-1];
    var hg    : [hgDom] real;
    var hgT   : [hgDom] real;

    // Quadrature coefficients
    var quadDom   = [0..k-1];
    var quad_x    : [quadDom] real; // points
    var quad_w    : [quadDom] real; // weights

    var quad_phiDom = [0..k-1, 0..k-1];
    var quad_phi    : [quad_phiDom] real; // phi[point,i]
    var quad_phiT   : [quad_phiDom] real; // phi[point,i] transpose
    var quad_phiw   : [quad_phiDom] real; // phi[point,i]*weight[point]

    // blocks of the block tridiagonal derivative operator
    //var rm, r0, rp;

    def initialize() {
        writeln("Creating Function: k=", k, " thresh=", thresh);

        writeln("  initializing two-scale relation coefficients");
        hg  = hg_getCoeffs(k);
        transposeCopy(hgT, hg);

        writeln("  initializing quadrature coefficients");
        init_quadrature(k);

        // blocks of the block tridiagonal derivative operator
        // rm, r0, rp = make_dc_periodic(k)

        // initial refinement of analytic function f(x)
        if f != nil {
            writeln("  performing initial refinement of f(x)");
            for l in 0..2**initial_level-1 do
                refine(initial_level, l);
        }

        writeln("done.");
    }

    def init_quadrature(order: int) {
        quad_x   = gl_getPoints(k);
        quad_w   = gl_getWeights(k);

        for i in quad_phiDom.dim(1) {
            var p = phi(quad_x[i], k);
            for m in quad_phiDom.dim(2) {
                quad_phi[i, m]  = p[m];
                quad_phiw[i, m] = quad_w[i]*p[m];
            }
        }

        transposeCopy(quad_phiT, quad_phi);
    }


    /** s[n][l] = integral(phi[n][l](x) * f(x))
        for box (n,l) project f(x) using quadrature rule
        into scaling function basis
     */
    def project(n: int, l: int) {
        var s     : [quadDom] real;
        var h     = 0.5 ** n;
        var scale = sqrt(h);

        for mu in quad_phiDom.dim(1) {
            var x  = (l + quad_x[mu]) * h;
            var fx = f(x);
            for i in quad_phiDom.dim(2) do
                s[i] += scale * fx * quad_phiw[mu, i];
        }

        return s;
    }


    /** Refine numerical representation of f(x) to desired tolerance
     */
    def refine(n: int, l: int) {
        // project f(x) at next level
        var s0 = project(n+1, 2*l);
        var s1 = project(n+1, 2*l+1);
        var s: [0..2*k-1] real;

        s[0..k-1]   = s0;
        s[k..2*k-1] = s1;

        // apply the two scale relationship to get difference coeff
        // in 1d this is O(k^2) flops
        var d = transform(s, hgT);

        // check to see if within tolerance
        // normf() is Frobenius norm == 2-norm for vectors
        var nf = normf(d[k..2*k-1]);
        if((nf < thresh) || (n >= (max_level-1))) {
            this.s[n+1, 2*l]   = s0;
            this.s[n+1, 2*l+1] = s1;
        }
        else {
            // these recursive calls on sub-trees can go in parallel
            // if the HashMap is syncronized
            refine(n+1, 2*l  );
            refine(n+1, 2*l+1);
        }
    }


    /** Evaluate numerical representation of f at x.  Performed in scaling
        function basis only.
     */
    def this(x) {
        if compressed then reconstruct();
        return evaluate(0, 0, x);
    }


    /** eval f(x) using adaptively refined numerical representation of f(x)
        answer should be within tolerance of the analytical f(x)

        Descend tree looking for box (n,l) with scaling function
        coefficients containing the point x.
     */
    def evaluate(in n=0, in l=0, in x): real {
        if s.has_coeffs(n, l) {
            var p = phi(x, k);
            return inner(s[n, l], p)*sqrt(2.0**n);

        } else {
            (n, l, x) = (n+1, 2*l, 2*x);
            if x >= 1 then (l, x) = (l+1, x-1);
            return evaluate(n, l, x);
        }
    }


    /** change from scaling function basis to multi-wavelet basis (s -> d)
        tree is filled out with s[0][0] and d
        n is level in tree
        l is box index
     */
    def compress(n=0, l=0) {
        if compressed then return;

        // sub-trees can be done in parallel
        if !s.has_coeffs(n+1,   2*l) then compress(n+1,   2*l);
        if !s.has_coeffs(n+1, 2*l+1) then compress(n+1, 2*l+1);

        var sc: [0..2*k-1] real;
        sc[0..k-1]   = s[n+1,   2*l];
        sc[k..2*k-1] = s[n+1, 2*l+1];

        // apply the two scale relationship to get difference coeff
        // in 1d this is O(k^2) flops (in 3d this is O(k^4) flops)
        var dc = sc*hgT;

        var dc_upper: [0..k-1] => dc[k..2*k-1];  // Reindex the upper half of dc
        s[n, l] = dc[0..k-1];
        d[n, l] = dc_upper;
        
        s.remove(n+1,   2*l);
        s.remove(n+1, 2*l+1);

        if n==0 then compressed = true;
    }


    /** change from multi-wavelet basis to scaling function basis (d -> s)
        tree just has s at leaves
        n is level in tree
        l is box index
     */
    def reconstruct(n=0, l=0) {
        if !compressed then return;

        if d.has_coeffs(n, l) {
            var dc: [0..2*k-1] real;
            dc[0..k-1]   = s[n, l];
            dc[k..2*k-1] = d[n, l];
            d.remove(n, l);
            s.remove(n, l);

            // apply the two scale relationship to get difference coeff
            // in 1d this is O(k^2) flops (in 3d this is O(k^4) flops)
            var sc = dc*hg;
            
            var sc_upper: [0..k-1] => sc[k..2*k-1]; // Reindex the upper half of s
            s[n+1, 2*l  ] = sc[0..k-1];
            s[n+1, 2*l+1] = sc_upper;
            
            // sub-trees can be done in parallel
            reconstruct(n+1, 2*l);
            reconstruct(n+1, 2*l+1);
        }
        
        if n == 0 then compressed = false;
    }

    
    /** In s are scaling coefficients for box n,l ... apply twoscale to generate
        the corresponding coefficients on level n+1 and insert the results into
        the tree of scaling function coefficients.
     */
    def recur_down(n, l, sc) {
        writeln(" + recur_down(", n, ", ", l, ")");

        var dc : [0..2*k-1] real;
        dc[0..k-1] = sc;

        var new_sc = dc*hg;
        var new_sc_upper : [0..k-1] => new_sc[k..2*k-1];
        s[n+1, 2*l  ] = new_sc[0..k-1];
        s[n+1, 2*l+1] = new_sc_upper;
    }

    
    /** If the scaling coefficeints in box n,l exist, return them.
        (allow here for zero boundary conditions for boxes just off
        the ends of the domain)

        Else recur up to the next level looking for a parent.  If a
        parent exists, use two scale to recur those coefficients down
        to make n,l.  Note that this modifies the tree in place and
        you should eventually call sclean to tidy up when finished.

        Else, return None (corresponding child boxes exist at a finer scale)
     */
    
    // FIXME: Can't specify array return type and type can't be inferred
    //        because of the recursion.
    /* def get_coeffs(n, l) {
        if l < 0 || l >= 2**n then {
            var zero : [0..k-1] real;
            return zero;
        }

        if s.has_coeffs(n, l) then return s[n, l];

        if n > 0 {
            var sc = get_coeffs(n-1, l/2);
            if isNone(sc) then
                return None;
        } else {
            return None; // No parent was found
        

        recur_down(n-1, l/2, s);
        
        return s[n, l];
    } */  
    // FIXME: This is a workaround implementation of get_coeffs
    //        Result of get_coeffs will be put into the coeffs parameter
    //        coeffs should probably be a degenerate array but we won't assume that.
    def get_coeffs(n, l, coeffs) {
        if l < 0 || l >= 2**n then {
            coeffs.domain = [0..k-1];
            coeffs = 0.0;
            return;
        }

        // Found Coeffs
        if s.has_coeffs(n, l) then {
            coeffs.domain = [0..k-1];
            coeffs = s[n, l];
            return;
        }

        // No coeffs found.  They must be below (n, l) in the tree
        if n < 0 {
            coeffs.domain = [0..-1];
            return;
        }

        // Recursive case:
        get_coeffs(n-1, l/2, coeffs);

        if isNone(coeffs) then
            return;
        else {
            recur_down(n-1, l/2, coeffs);
            coeffs.domain = [0..k-1];
            coeffs = s[n, l];
            return;
        }
    }
     

    /** Differentiation (also inner and mul) may leave scaling function
        coefficients below their original level.  Recur down to the
        locally first box with scaling function coefficients.
        Delete all children below there. 
     */
    def sclean(n=0, l=0, in cleaning=false) {
        if cleaning { //then
            writeln(" + sclean deleting (", n, ", ", l, ")");
            s.remove(n, l);
        } else
            cleaning = s.has_coeffs(n, l);

        // Sub trees can run in parallel
        if n < max_level {
            if !cleaning || s.has_coeffs(n+1, 2*l) then
                sclean(n+1, 2*l, cleaning);
            if !cleaning || s.has_coeffs(n+1, 2*l+1) then
                sclean(n+1, 2*l+1, cleaning);
        }
    }

    
    /** Return sqrt(integral(f(x)**2))
     */
    def norm2() {
        if compressed then reconstruct();
        
        var sum = 0.0;
        for i in s do
            sum += normf(s[i(1), i(2)])**2;

        return sqrt(sum);
    }


    /** Mostly for debugging, print summary of coefficients,
        optionally printing the norm of each block
     */
    def summarize() {
        writeln("\n-----------------------------------------------------");
        writeln("sum coefficients:");
        for n in 0..max_level {
            var sum     = 0.0;
            var ncoeffs = 0;
            for i in s {
                var lvl = i(1);
                var idx = i(2);
                if (lvl == n && s.has_coeffs(lvl, idx)) {
                    var t = normf(s[lvl, idx]);
                    sum  += t**2;
                    ncoeffs += 1;
                }
            }
            if ncoeffs != 0 then
                writeln("   level ", n, "   #boxes=", ncoeffs, "  norm=", sqrt(sum));
        }

        writeln("difference coefficients:");
        for n in 0..max_level {
            var sum     = 0.0;
            var ncoeffs = 0;
            for i in d {
                var lvl = i(1);
                var idx = i(2);
                if (lvl == n && d.has_coeffs(lvl, idx)) {
                    var t = normf(d[lvl, idx]);
                    sum  += t**2;
                    ncoeffs += 1;
                }
            }
            if ncoeffs != 0 then
                writeln("   level ", n, "   #boxes=", ncoeffs, "  norm=", sqrt(sum));
        }

        writeln("-----------------------------------------------------\n");
    }


    /** Evaluate the analytic and numerical functions over the given interval
        and print the error.
    */
    def evalNPT(npt) {
        for i in 1..npt do
            writeln(" -- ", i/npt:real, ": f_analytic()=", f(i/npt:real),
                    " F_numeric()=", this(i/npt:real), " err=", this(i/npt:real)-f(i/npt:real));
    }
}



def main() {
    use MadFn1d;
    var npt = 10;

    writeln("Mad Chapel -- One Step Beyond\n");

    writeln("** var F1 : Function1d = nil;");
    var F1 : Function1d = nil;

    writeln("\n** var F2 = Function1d();");
    var F2 = Function1d();

    writeln("\n** var F3 = Function1d(f=test3);");
    var F3 = Function1d(k=5, thresh=1e-5, f=Fn_Test3());
    var a: [0..-1] real;
    F3.get_coeffs(30, 0, a);
    writeln("Coeffs at (30, 0) = ", a);
    F3.sclean();
    
    writeln("F3.norm2() = ", F3.norm2());

    F3.summarize();

    writeln("\nEvaluating F3 on [0, 1] (singularity at 0.5):");
    F3.evalNPT(10);

    writeln("\nCompressing F3 ...");
    F3.compress();
    F3.summarize();

    writeln("Reconstructing F3 ...");
    F3.reconstruct();
    F3.summarize();

    writeln("\nEvaluating F3 on [0, 1] (singularity at 0.5):");
    F3.evalNPT(10);
}
