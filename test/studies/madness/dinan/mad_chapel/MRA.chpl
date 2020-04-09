/** MRA -- MultiResolution Analysis Module

    Multiresolution representation of 1-d functions using a multiwavelet
    basis (similar to discontinuous spectral element with a hierarchal
    decomposition).
    
    Mad Chapel Author: James Dinan <dinan@cray.com>
                 Date: August, 2007
    
    This code is an adaptation of 1d Madness, originally written by Robert
    Harrison <harrisonrj@ornl.gov>, et al.
 */

use Math;
use Tensor;
public use FTree;
use Quadrature;
use TwoScale;
public use AnalyticFcn;

config const verbose = false;
config const debug   = false;

class Function {
    const k             = 5;    // use first k Legendre polynomials as the basis in each box
    const thresh        = 1e-5; // truncation threshold for small wavelet coefficients
    var   f: unmanaged AFcn?       = nil;  // analytic f(x) to project into the numerical represntation
    const initial_level = 2;    // initial level of refinement
    const max_level     = 30;   // maximum level of refinement mostly as a sanity check
    var   autorefine    = true; // automatically refine during multiplication
    var   compressed    = false;// keep track of what basis we are in

    // Sum and Difference coefficients
    var   s = new unmanaged FTree(order=k);
    var   d = new unmanaged FTree(order=k);

    // FIXME: Ideally all of these matrices should be const as well but they
    //        can't be presently since they must be assigned in postinit()

    // Two-Scale relationship matrices
    const hgDom = {0..2*k-1, 0..2*k-1};
    const hg    : [hgDom] real = hg_getCoeffs(k);
    const hgT   : [hgDom] real = transpose(hg);

    // Quadrature coefficients
    const quadDom   = {0..k-1};
    const quad_x    : [quadDom] real = gl_getPoints(k);
    const quad_w    : [quadDom] real = gl_getWeights(k);

    const quad_phiDom = {0..k-1, 0..k-1};
    var   quad_phi    : [quad_phiDom] real; // phi[point,i]
    var   quad_phiT   : [quad_phiDom] real; // phi[point,i] transpose
    var   quad_phiw   : [quad_phiDom] real; // phi[point,i]*weight[point]

    // blocks of the block tridiagonal derivative operator
    const dcDom = {0..k-1, 0..k-1};
    var   rm    : [dcDom] real;
    var   r0    : [dcDom] real;
    var   rp    : [dcDom] real;

    proc deinit() {
        delete s;
        delete d;
    }

    proc postinit() {
        if debug then writeln("Creating Function: k=", k, " thresh=", thresh);

        if debug then writeln("  initializing quadrature coefficients");
        init_quadrature(k);

        // blocks of the block tridiagonal derivative operator
        if debug then writeln("  initializing tridiagonal derivative operator");
        make_dc_periodic();

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
    proc init_quadrature(order: int) {
        for i in quad_phiDom.dim(0) {
            var p = phi(quad_x[i], k);
            quad_phi [i, ..] = p;
            quad_phiw[i, ..] = quad_w[i] * p;
            quad_phiT[.., i] = p;
        }
    }


    /** Return the level-0 blocks rm, r0, rp of the central
        difference derivative operator with periodic boundary
        conditions on either side.
     */
    proc make_dc_periodic() {
        var iphase = 1.0;
        for i in dcDom.dim(0) {
            var jphase = 1.0;

            for j in dcDom.dim(1) {
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


    /** Return a deep copy of this Function
     */
    proc copy() {
        return new unmanaged Function(k=k, thresh=thresh, f=f, initial_level=initial_level,
                max_level=max_level, autorefine=autorefine, compressed=compressed,
                s=s.copy(), d=d.copy());
    }


    /** Return a copy of this function's skeleton
     */
    proc skeletonCopy() {
        // Omit: f, compressed, s, d
        return new unmanaged Function(k=k, thresh=thresh, initial_level=initial_level,
                max_level=max_level, autorefine=autorefine);
    }


    /** s[n, l] = integral(phi[n][l](x) * f(x))
        for box (n,l) project f(x) using quadrature rule
        into scaling function basis
     */
    proc project(n: int, l: int) {
        var s     : [quadDom] real;
        var h     = 0.5 ** n;
        var scale = sqrt(h);

        for mu in quad_phiDom.dim(0) {
            var x  = (l + quad_x[mu]) * h;
            var fx = f!(x);
            for i in quad_phiDom.dim(1) do
                s[i] += scale * fx * quad_phiw[mu, i];
        }

        return s;
    }


    /** Refine numerical representation of f(x) to desired tolerance
     */
    proc refine(n, l) {
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
    proc this(x) {
        if compressed then reconstruct();
        return evaluate(0, 0, x);
    }


    /** eval f(x) using adaptively refined numerical representation of f(x)
        answer should be within tolerance of the analytical f(x)

        Descend tree looking for box (n,l) with scaling function
        coefficients containing the point x.
     */
    proc evaluate(in n=0, in l=0, in x): real {
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
    proc compress(n=0, l=0) {
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

        s[n, l] = dc[0..k-1];
        d[n, l] = dc[k..2*k-1];
        
        s.remove(n+1,   2*l);
        s.remove(n+1, 2*l+1);

        if n==0 then compressed = true;
    }


    /** change from multi-wavelet basis to scaling function basis (d -> s)
        tree just has s at leaves
        n is level in tree
        l is box index
     */
    proc reconstruct(n=0, l=0) {
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
            
            s[n+1, 2*l  ] = sc[0..k-1];
            s[n+1, 2*l+1] = sc[k..2*k-1];
            
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
    proc recur_down(n, l) {
        if debug then writeln(" + recur_down(", n, ", ", l, ")");

        var sc : [0..2*k-1] real;
        sc[0..k-1] = s[n, l];

        var new_sc = sc*hg;
        s[n+1, 2*l  ] = new_sc[0..k-1];
        s[n+1, 2*l+1] = new_sc[k..2*k-1];
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
    proc get_coeffs(_n, _l) {
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
    proc sclean(n=0, l=0, in cleaning=false) {
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
    proc diff() {
        proc diffHelper(n = 0, l = 0, result) {
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
        var result = skeletonCopy();

        diffHelper(0, 0, result);

        sclean();
        return result;
    }
   

    /** Return sqrt(integral(f(x)**2))
     */
    proc norm2() {
        if compressed then reconstruct();
        return sqrt(+ reduce [i in s] normf(i)**2);
    }


    /** Perform GAXPY in the multi-wavelet (compressed) basis
        this = alpha*this + beta*other (other is not changed).
     */
    proc gaxpy(alpha, other, beta) {
        // recursive "iteration" for gaxpy
        proc gaxpy_iter(n=0, l=0) {
            if d.has_coeffs(n, l) || other.d.has_coeffs(n, l) {
                if d.has_coeffs(n, l) && other.d.has_coeffs(n, l) then
                    d[n, l] = d[n, l]*alpha + other.d[n, l]*beta;

                else if !d.has_coeffs(n, l) && other.d.has_coeffs(n, l) then
                    d[n, l] = other.d[n, l] * beta;

                else /* d.has_coeffs(n, l) && !other.d.has_coeffs(n, l) */
                    d[n, l] *= alpha;

                // calls on sub-trees can go in parallel
                gaxpy_iter(n+1, 2*l);
                gaxpy_iter(n+1, 2*l+1);
            }
        }

        if !compressed then compress();
        if !other.compressed then other.compress();

        s[0, 0] = s[0, 0]*alpha + other.s[0, 0]*beta; // Do scaling coeffs
        gaxpy_iter();                                 // Do multi-wavelet coeffs

        // return this so operations can be chained
        return _to_unmanaged(this);
    }


    /** Add this function to another and return the result in a new
        function.  This and other are unchanged.
     */ 
    proc add(other) {
        return copy().gaxpy(1.0, other, 1.0);
    }


    /** Subtract this function from another and return the result in a new
        function.  This and other are unchanged.
     */ 
    proc subtract(other) {
        return copy().gaxpy(1.0, other, -1.0);
    }

    
    /** Recursively multiply f1 and f2 put the result into this
     */ 
    proc multiply(f1, f2, n=0, l=0) {
        if f1.s.has_coeffs(n, l) && f2.s.has_coeffs(n, l) {
            if autorefine && n+1 <= max_level {
                // if autorefine is set we are multiplying two polynomials
                // of order k-1 the result could be up to order 2(k-1) so
                // refine one more level to reduce the error and try to
                // keep it within the threshold

                // refine both one more level
                f1.recur_down(n, l);
                f2.recur_down(n, l);

                // scale factor for this level = sqrt((2^d)^(n+1))
                var scale_factor = sqrt(2.0**(n+1));

                // multiply f1.s[n+1][2*l] and f2.s[n+1][2*l]
                var f = f1.s[n+1, 2*l] * quad_phiT;
                var g = f2.s[n+1, 2*l] * quad_phiT;
                f *= g; // Note: Element-wise multiplication
                s[n+1, 2*l] = (f * quad_phiw) * scale_factor;

                // multiply f1.s[n+1][2*l+1] and f2.s[n+1][2*l+1]
                f  =  f1.s[n+1, 2*l+1] * quad_phiT;
                g  =  f2.s[n+1, 2*l+1] * quad_phiT;
                f *= g;
                s[n+1, 2*l+1] = (f * quad_phiw) * scale_factor;

            } else {
                // if autorefine is not set or we are at the max_level
                // live with what you get
                var f = f1.s[n, l] * quad_phiT;
                var g = f2.s[n, l] * quad_phiT;
                f *= g;

                // scale factor for this level = sqrt((2^d)^(n+1))
                s[n, l] = (f * quad_phiw) * sqrt(2.0**(n));
            }

        } else {
            if f1.s.has_coeffs[n, l] && !f2.s.has_coeffs[n, l] then
                // refine this box down to next level in f1
                f1.recur_down(n, l);
            else if !f1.s.has_coeffs[n, l] && f2.s.has_coeffs[n, l] then
                // refine this box down to next level in f2
                f2.recur_down(n, l);

            // calls on sub-trees can go in parallel
            multiply(f1, f2, n+1, 2*l);
            multiply(f1, f2, n+1, 2*l+1);
        }
    }


    /** Function Multiplication:
        For multiply, both operands need to be in the scaling function basis
        so possibly call reconstruct on one or both of them first
     */
    proc multiply(other) {
        if compressed then reconstruct();
        if other.compressed then other.reconstruct();

        // Store the result in a new Function
        var result = skeletonCopy();
        result.multiply(this, other);

        sclean();
        other.sclean();

        return result;
    }


    /** Mostly for debugging, print summary of coefficients,
        optionally printing the norm of each block
     */
    proc summarize() {
        writeln("\n-----------------------------------------------------");
        writeln("k=", k, " thresh=", thresh, " compressed=", compressed);
        writeln("sum coefficients:");
        for n in 0..max_level {
            var sum = 0.0, ncoeffs = 0;
            for box in s.lvl_iter(n) {
                sum     += normf(box)**2;
                ncoeffs += 1;
            }
            if ncoeffs != 0 then 
	        writef("   level %{##}   #boxes=%{####}  norm=%0.2er\n",
		       n, ncoeffs, truncate(sqrt(sum)));
        }

        writeln("difference coefficients:");
        for n in 0..max_level {
            var sum = 0.0, ncoeffs = 0;
            for box in d.lvl_iter(n) {
                sum     += normf(box)**2;
                ncoeffs += 1;
            }
            if ncoeffs != 0 then
	        writef("   level %{##}   #boxes=%{####}  norm=%0.2er\n",
		       n, ncoeffs, truncate(sqrt(sum)));
        }

        writeln("-----------------------------------------------------\n");
    }


    /** Evaluate the analytic and numerical functions over the given interval
        and print the error.
     */
    proc evalNPT(npt) {
        for i in 0..npt {
	    // This truncation may lead to a loss of precision in the error calculation.
            // var (fval, Fval) = (truncate(f(i/npt:real)), truncate(this(i/npt:real)));
            var (fval, Fval) = (f!(i/npt:real), this(i/npt:real));
            writef(" -- %.2dr:  F_numeric()=% .8dr  f_analytic()=% .8dr err=% .8dr%s\n",
		   i/npt:real, truncate(Fval), truncate(fval), truncate(Fval-fval), 
		   if abs(Fval-fval) > thresh then "  > thresh" else "");
        }
    }
}


/*************************************************************************/
/* Standard operations on Function objects:                              */
/*************************************************************************/


proc +(F: unmanaged Function, G: unmanaged Function): unmanaged Function {
    return F.add(G);
}

proc -(F: unmanaged Function, G: unmanaged Function): unmanaged Function {
    return F.subtract(G);
}
    
proc *(F: unmanaged Function, G: unmanaged Function): unmanaged Function {
    return F.multiply(G);
}
