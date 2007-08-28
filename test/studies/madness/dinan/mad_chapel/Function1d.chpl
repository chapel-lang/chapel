use Math;
use Tensor;
use FTree;
use Quadrature;
use TwoScale;
use Fn1d;

config const verbose = false;
config const debug   = false;

class Function1d {
    var k             = 5;    // use first k Legendre polynomials as the basis in each box
    var thresh        = 1e-5; // truncation threshold for small wavelet coefficients
    var f: Fn1d       = nil;  // analytic f(x) to project into the numerical represntation
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
    var dcDom = [0..k-1, 0..k-1];
    var rm    : [dcDom] real;
    var r0    : [dcDom] real;
    var rp    : [dcDom] real;

    def initialize() {
        if debug then writeln("Creating Function1d: k=", k, " thresh=", thresh);

        if debug then writeln("  initializing two-scale relation coefficients");
        hg  = hg_getCoeffs(k);
        transposeCopy(hgT, hg);

        if debug then writeln("  initializing quadrature coefficients");
        init_quadrature(k);

        // blocks of the block tridiagonal derivative operator
        if debug then writeln("  initializing tridiagonal derivative operator");
        make_dc_periodic(k);

        // initial refinement of analytic function f(x)
        if f != nil {
            if debug then writeln("  performing initial refinement of f(x)");
            for l in 0..2**initial_level-1 do
                refine(initial_level, l);
        }

        if debug then writeln("done.");
    }


    /** Initialize the quadrature coefficient matricies.
     */
    def init_quadrature(order: int) {
        quad_x   = gl_getPoints(k);
        quad_w   = gl_getWeights(k);

        for i in quad_phiDom.dim(1) {
            var p = phi(quad_x[i], k);
            quad_phi [i..i, ..] = p;            // FIXME: quad_phi [i, ..] = p;
            quad_phiw[i..i, ..] = quad_w[i] * p;//        quad_phiw[i, ..] = ...;
            quad_phiT[.., i..i] = p;
        }
    }


    /** Return the level-0 blocks rm, r0, rp of the central
        difference derivative operator with periodic boundary
        conditions on either side.
     */
    def make_dc_periodic(k) {
        var iphase = 1.0;
        for i in dcDom.dim(1) {
            var jphase = 1.0;

            for j in dcDom.dim(2) {
                var gammaij = sqrt((2*i+1) * (2*j+1));
                var Kij = if (i-j) > 0 && ((i-j) % 2) == 1 then
                        2.0 else 0.0;
                r0[i,j] = 0.5*(1.0 - iphase*jphase - 2.0*Kij)*gammaij;
                rm[i,j] = 0.5*jphase*gammaij;
                rp[i,j] =-0.5*iphase*gammaij;
                jphase = -jphase;
            }
            iphase = -iphase;
        }
    }


    /** Return a copy of this Function
     */
    def copy() {
        return Function1d(k, thresh, f, initial_level, max_level, compressed, s.copy(), d.copy());
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
        var sc : [0..2*k-1] real;

        sc[0..k-1]   = s0;
        sc[k..2*k-1] = s1;

        // apply the two scale relationship to get difference coeff
        // in 1d this is O(k^2) flops
        var dc = sc*hgT;

        // check to see if within tolerance
        // normf() is Frobenius norm == 2-norm for vectors
        var nf = normf(dc[k..2*k-1]);
        if((nf < thresh) || (n >= (max_level-1))) {
            s[n+1, 2*l]   = s0;
            s[n+1, 2*l+1] = s1;
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
    def recur_down(n, l) {
        if debug then writeln(" + recur_down(", n, ", ", l, ")");

        var sc : [0..2*k-1] real;
        sc[0..k-1] = s[n, l];

        var new_sc = sc*hg;
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
    def get_coeffs(_n, _l) {
        if debug then writeln(" @ get_coeffs(", _n, ", ", _l, ")");
        var n = _n, l = _l;

        // Walk up the tree to find scaling coeffs
        while n >= 0 {
            if l < 0 || l >= 2**n {
                var coeffs : [0..k-1] real;
                return coeffs;
            }

            if s.has_coeffs(n, l) then break;

            n -= 1;
            l  = l/2;
        }

        // Didn't find coeffs, they must be below (_n, _l)
        if n < 0 then return None;

        // Recur down to (_n, _l)
        for i in n.._n-1 {
            l = _l/(2**(_n-i));
            recur_down(i, l);
        }

        return s[_n, _l];
    }


    /** Differentiation (also inner and mul) may leave scaling function
        coefficients below their original level.  Recur down to the
        locally first box with scaling function coefficients.
        Delete all children below there. 
     */
    def sclean(n=0, l=0, in cleaning=false) {
        if cleaning { //then
            if debug then writeln(" + sclean deleting (", n, ", ", l, ")");
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


    /** Differentiate the function, which corresponds to application
        of a block triadiagonal matrix.  For an adaptively refined
        target function we may need to refine boxes down until three
        boxes exist in the same scale.
     */
    def diff() {
        def diffHelper(n = 0, l = 0, result) {
            if debug then writeln(" * diff(", n, ", ", l, ")");
            if !s.has_coeffs(n, l) {
                // Sub trees can run in parallel
                // Run down tree until we hit scaling function coefficients
                diffHelper(n+1, 2*l  , result);
                diffHelper(n+1, 2*l+1, result);
            } else {
                // These can also go in parallel since may involve
                // recurring up & down the tree.
                if debug then writeln(" * diff: coeffs at (", n, ", ", l, ")");
                var sm = get_coeffs(n, l-1);
                var sp = get_coeffs(n, l+1);
                var s0 = s[n, l];

                // We have s0, check if we found sm and sp at this level
                if !isNone(sm) && !isNone(sp) {
                    var r = rp*sm + r0*s0 + rm*sp;
                    result.s[n, l] = r * 2.0**n;
                } else {
                    recur_down(n, l);
                    // Sub trees can run in parallel
                    diffHelper(n+1, 2*l  , result);
                    diffHelper(n+1, 2*l+1, result);
                }
            }
        }

        if compressed then reconstruct();
        var result = Function1d(k, thresh);

        diffHelper(0, 0, result);

        sclean();
        return result;
    }
   

    /** Return sqrt(integral(f(x)**2))
     */
    def norm2() {
        if compressed then reconstruct();
        return sqrt(+ reduce [i in s] normf(s[i(1), i(2)])**2);
    }


    /** Mostly for debugging, print summary of coefficients,
        optionally printing the norm of each block
     */
    def summarize() {
        writeln("\n-----------------------------------------------------");
        writeln("k=", k, " thresh=", thresh, " compressed=", compressed);
        writeln("sum coefficients:");
        for n in 0..max_level {
            var sum = 0.0, ncoeffs = 0;
            for i in s {
                var (lvl, idx) = i;
                if (lvl == n && s.has_coeffs(lvl, idx)) {
                    sum     += normf(s[lvl, idx])**2;
                    ncoeffs += 1;
                }
            }
            if ncoeffs != 0 then
                writeln("   level ", n, "   #boxes=", ncoeffs, "  norm=", sqrt(sum));
        }

        writeln("difference coefficients:");
        for n in 0..max_level {
            var sum = 0.0, ncoeffs = 0;
            for i in d {
                var (lvl, idx) = i;
                if (lvl == n && d.has_coeffs(lvl, idx)) {
                    sum     += normf(d[lvl, idx])**2;
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
        for i in 1..npt {
            var (fval, Fval) = (f(i/npt:real), this(i/npt:real));
            writeln(" -- ", i/npt:real, ": f_analytic()=", fval, " F_numeric()=", Fval,
                " err=", Fval-fval, if (Fval-fval) > thresh then "  !! > thresh" else "");
        }
    }
}



def main() {
    use MadFn1d;
    var npt = 10;

    writeln("Mad Chapel -- One Step Beyond\n");

    var fcn  : [1..3] Fn1d = (Fn_Test1():Fn1d,  Fn_Test2():Fn1d,  Fn_Test3():Fn1d);
    var dfcn : [1..3] Fn1d = (Fn_dTest1():Fn1d, Fn_dTest2():Fn1d, Fn_dTest3():Fn1d);

    for i in fcn.domain {
        writeln("** Testing function ", i);
        var F = Function1d(k=5, thresh=1e-5, f=fcn[i]);

        writeln("F", i, ".norm2() = ", F.norm2());

        if verbose {
            F.summarize();
            writeln("Evaluating F", i, " on [0, 1]:");
            F.evalNPT(npt);
            writeln();
        }

        writeln("Compressing F", i, " ...");
        F.compress();
        if verbose then F.summarize();

        writeln("Reconstructing F", i, " ...");
        F.reconstruct();
        writeln("F", i, ".norm2() = ", F.norm2());
        if verbose then F.summarize();

        writeln("Evaluating F", i, " on [0, 1]:");
        F.evalNPT(npt);

        writeln("\nDifferentiating F", i, " ...");
        var dF = F.diff();
        dF.f = dfcn[i]:Fn1d; // Fudge it for the sake of evalNPT()
        if verbose then dF.summarize();

        writeln("\nEvaluating dF", i, " on [0, 1]:");
        dF.evalNPT(npt);

        if i < 3 then writeln("\n======================================================================\n");
    }
}
