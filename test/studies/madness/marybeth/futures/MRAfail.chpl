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

config const verbose = false;
config const debug   = false;
const PI = 3.14159265358979323846;

var None: [0..-1] real;

config const hg_inputfile = "hg_coeffs.dat";

var hg_MaxK   : int;
var hg_coeffs : [0..-1, 0..-1, 0..-1] real;
var phi_norms : [0..-1] real;
var phi_initialized = false;

class AFcn {
    def this(x: real): real {
        halt("Fn1d.f() is abstract and should not be called directly");
        return 0.0;
    }
}
record Coeff {
    var dom  : domain(1);
    var data : [dom] real;
}

// Check if we have None.
def isNone(x) {
    return x.numElements == 0;
}

class FTree {
    var order    : int;
    var coeffDom = [0..order-1];
    var idx_t    : 2*int;           // FIXME: Can't write domain(2*int)

    var indices  : domain(idx_t);   // Indexed by 2-tuples of integers
    var nodes    : [indices] Coeff; // Associative Mapping: (:int, :int) => Coeff

    var zeroes   : [coeffDom] real; // Return zeroes from this() when reading
                                    // something that has not yet been set.


    def initialize() {
        if order == 0 then
            halt("FTree must be initialized with an order > 0");
    }


    /** Access an element in the associative domain.  If the element
        doesn't exist it will be created.
        
        We have to be really careful about creating extra boxes when we don't
        mean to.  The algorithms tend to be really sensitive to the structure
        of the tree.  Scaling coefficients are used to mark the leaves in the
        sum tree, so if you accidentally create scaling coeffs somewhere in the
        middle of the tree you will short circuit a whole subtree!
        
     */
    def this(lvl: int, idx: int) var {
        if !indices.member((lvl, idx)) {
            if setter {
              indices += ((lvl, idx));
              nodes[(lvl, idx)] = Coeff(coeffDom);
            } else {
              // This is a getter so it shouldn't be modifying what
              // we return, should be safe to return the zero vector.
              // FIXME: Zeroes should really be a const, but can'ti
              //        return const from a var fcn.
              return zeroes;
            }
        }

        return nodes[(lvl, idx)].data;
    }

    def this((lvl, idx)) var {
        if !indices.member((lvl, idx)) {
            if setter {
              indices += ((lvl, idx));
              nodes[(lvl, idx)] = Coeff(coeffDom);
            } else {
              // This is a getter so it shouldn't be modifying what
              // we return, should be safe to return the zero vector.
              // FIXME: Zeroes should really be a const, but can'ti
              //        return const from a var fcn.
              return zeroes;
            }
        }

        return nodes[(lvl, idx)].data;
    }

    def get_children((lvl, idx)) {
        return ((lvl+1, 2*idx), (lvl+1, 2*idx+1));
    }

    def get_neighbors((lvl, idx)) {
        return ((lvl, idx-1), (lvl, idx+1));
    }

    /** Unordered iterator over all coefficients
     */
    def these() {
        for n in nodes yield n.data;
    }

    /** Check if there are coefficients in box (lvl, idx)
     */
    def has_coeffs((lvl, idx)) {
        return indices.member((lvl, idx));
    }


    /** Remove an element from the associative domain.  If the element
        does not exist, it is ignored.
     */
    def remove((lvl,idx)) {
        if indices.member((lvl, idx)) then indices.remove((lvl, idx));
    }
}

class Function {
    const k             = 5;    // use first k Legendre polynomials as the basis in each box
    const thresh        = 1e-5; // truncation threshold for small wavelet coefficients
    const f: AFcn       = nil;  // analytic f(x) to project into the numerical represntation
    const initial_level = 2;    // initial level of refinement
    const max_level     = 30;   // maximum level of refinement mostly as a sanity check
    var   autorefine    = true; // automatically refine during multiplication
    var   compressed    = false;// keep track of what basis we are in

    // Sum and Difference coefficients
    var   s = FTree(order=k);
    var   d = FTree(order=k);

    // FIXME: Ideally all of these matrices should be const as well but they
    //        can't be presently since they must be assigned in initialize()

    // Two-Scale relationship matrices
    const hgDom = [0..2*k-1, 0..2*k-1];
    var   hg    : [hgDom] real; // FIXME: hg  =  hg_getCoeffs(k);
    var   hgT   : [hgDom] real; //        hgT => transpose(hg);

    // Quadrature coefficients
    const quadDom   = [0..k-1];
    var   quad_x    : [quadDom] real; // points
    var   quad_w    : [quadDom] real; // weights

    const quad_phiDom = [0..k-1, 0..k-1];
    var   quad_phi    : [quad_phiDom] real; // phi[point,i]
    var   quad_phiT   : [quad_phiDom] real; // phi[point,i] transpose
    var   quad_phiw   : [quad_phiDom] real; // phi[point,i]*weight[point]

    // blocks of the block tridiagonal derivative operator
    const dcDom = [0..k-1, 0..k-1];
    var   rm    : [dcDom] real;
    var   r0    : [dcDom] real;
    var   rp    : [dcDom] real;

    def initialize() {
        if debug then writeln("Creating Function: k=", k, " thresh=", thresh);

        if debug then writeln("  initializing two-scale relation coefficients");
        hg  = hg_getCoeffs(k);
        transposeCopy(hgT, hg);

        if debug then writeln("  initializing quadrature coefficients");
        init_quadrature(k);

        // blocks of the block tridiagonal derivative operator
        if debug then writeln("  initializing tridiagonal derivative operator");
        make_dc_periodic();

        // initial refinement of analytic function f(x)
        if f != nil {
            if debug then writeln("  performing initial refinement of f(x)");
            for l in 0..2**initial_level-1 do
                refine((initial_level, l));
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
    def make_dc_periodic() {
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


    /** Return a copy of this function's skeleton
     */
    def skeletonCopy() {
        // Omit: f, compressed, s, d
        return Function(k=k, thresh=thresh, initial_level=initial_level,
                max_level=max_level, autorefine=autorefine);
    }


    /** s[n, l] = integral(phi[n][l](x) * f(x))
        for box (n,l) project f(x) using quadrature rule
        into scaling function basis
     */
    def project(curNode: 2*int) {
        const (n, l) = curNode;
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
    def refine(curNode: 2*int) {
        // project f(x) at next level
        const (n, _ ) = curNode;
        const child = s.get_children(curNode);
        var sc : [0..2*k-1] real;
        var s0 : [0..k-1] => sc[0..k-1];
        var s1 : [0..k-1] => sc[k..2*k-1];

        s0 = project(child(1));
        s1 = project(child(2));

        // apply the two scale relationship to get difference coeff
        // in 1d this is O(k^2) flops
        var dc = sc*hgT;

        // check to see if within tolerance
        // normf() is Frobenius norm == 2-norm for vectors
        var nf = normf(dc[k..2*k-1]);
        if((nf < thresh) || (n >= (max_level-1))) {
            s[child(1)] = s0;
            s[child(2)] = s1;
        }
        else {
            // these recursive calls on sub-trees can go in parallel
            // if the HashMap is syncronized
            refine(child(1));
            refine(child(2));
        }
    }


    /** Evaluate numerical representation of f at x.  Performed in scaling
        function basis only.
     */
    def this(x) {
        if compressed then reconstruct();
        return evaluate( (0,0), x);
    }


    /** eval f(x) using adaptively refined numerical representation of f(x)
        answer should be within tolerance of the analytical f(x)

        Descend tree looking for box (n,l) with scaling function
        coefficients containing the point x.
     */
    def evaluate(curNode = (0,0), in x): real {
        const (n,_) = curNode;
        const child = s.get_children(curNode);
        if s.has_coeffs(curNode) {
            var p = phi(x, k);
            return inner(s[curNode], p)*sqrt(2.0**n);

        } else {
            if (2*x < 1) then
                return evaluate (child(1), 2*x); 
            else
                return evaluate (child(2), 2*x-1);
        }
    }


    /** change from scaling function basis to multi-wavelet basis (s -> d)
        tree is filled out with s[0][0] and d
        n is level in tree
        l is box index
     */
    def compress(curNode = (0,0)) {
        if compressed then return;
        const (n,_) = curNode;
        const child = s.get_children(curNode);

        // sub-trees can be done in parallel
        if !s.has_coeffs(child(1)) then compress(child(1));
        if !s.has_coeffs(child(2)) then compress(child(2));

        var sc: [0..2*k-1] real;
        sc[0..k-1]   = s(child(1));
        sc[k..2*k-1] = s(child(2));

        // apply the two scale relationship to get difference coeff
        // in 1d this is O(k^2) flops (in 3d this is O(k^4) flops)
        var dc = sc*hgT;

        s[curNode] = dc[0..k-1];
        d[curNode] = dc[k..2*k-1];
        
        s.remove(child(1));
        s.remove(child(2));

        if n==0 then compressed = true;
    }


    /** change from multi-wavelet basis to scaling function basis (d -> s)
        tree just has s at leaves
        n is level in tree
        l is box index
     */
    def reconstruct(curNode = (0,0)) {
        if !compressed then return;
        const (n, _) = curNode;
        const child = s.get_children(curNode);

        if d.has_coeffs(curNode) {
            var dc: [0..2*k-1] real;
            dc[0..k-1]   = s[curNode];
            dc[k..2*k-1] = d[curNode];
            d.remove(curNode);
            s.remove(curNode);

            // apply the two scale relationship to get difference coeff
            // in 1d this is O(k^2) flops (in 3d this is O(k^4) flops)
            var sc = dc*hg;
            
            s[child(1)] = sc[0..k-1];
            s[child(2)] = sc[k..2*k-1];
            
            // sub-trees can be done in parallel
            reconstruct(child(1));
            reconstruct(child(2));
        }
        
        if n == 0 then compressed = false;
    }

    
    /** In s are scaling coefficients for box n,l ... apply twoscale to generate
        the corresponding coefficients on level n+1 and insert the results into
        the tree of scaling function coefficients.
     */
    def recur_down(curNode: 2*int) {
        const child = s.get_children(curNode);
        if debug then writeln(" + recur_down",curNode);

        var sc : [0..2*k-1] real;
        sc[0..k-1] = s[curNode];

        var new_sc = sc*hg;
        s(child(1)) = new_sc[0..k-1];
        s(child(2)) = new_sc[k..2*k-1];
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
    def get_coeffs(curNode: 2*int) {
        if debug then writeln(" @ get_coeffs",curNode);
        const (nCur, lCur) = curNode;
        var (n,l) = curNode;

        // Walk up the tree to find scaling coeffs
        while n >= 0 {
            if l < 0 || l >= 2**n {
                var coeffs : [0..k-1] real;
                return coeffs;
            }

            if s.has_coeffs((n, l)) then break;

            n -= 1;
            l  = l/2;
        }

        // Didn't find coeffs, they must be below curNode
        if n < 0 then return None;

        // Recur down to curNode
        for i in n..nCur-1 {
            l = lCur/(2**(nCur-i));
            recur_down((i, l));
        }

        return s[curNode];
    }


    /** Differentiation (also inner and mul) may leave scaling function
        coefficients below their original level.  Recur down to the
        locally first box with scaling function coefficients.
        Delete all children below there. 
     */
    def sclean(curNode = (0,0), in cleaning=false) {
        const (n, _) = curNode;
        const child = s.get_children(curNode);
        if cleaning { //then
            if debug then writeln(" + sclean deleting ", curNode);
            s.remove(curNode);
        } else
            cleaning = s.has_coeffs(curNode);

        // Sub trees can run in parallel
        if n < max_level {
            if !cleaning || s.has_coeffs(child(1)) then
                sclean(child(1), cleaning);
            if !cleaning || s.has_coeffs(child(2)) then
                sclean(child(2), cleaning);
        }
    }


    /** Differentiate the function, which corresponds to application
        of a block triadiagonal matrix.  For an adaptively refined
        target function we may need to refine boxes down until three
        boxes exist in the same scale.
     */
   
  def diff() {
    def diffHelper(curNode = (0,0), result) {
        const (n,l) = curNode;
        const child = s.get_children(curNode);
        if debug then writeln(" * diff",curNode);
        if !s.has_coeffs(curNode) {
            // Sub trees can run in parallel
            // Run down tree until we hit scaling function coefficients
            diffHelper(child(1), result);
            diffHelper(child(2), result);
        } else {
            // These can also go in parallel since may involve
            // recurring up & down the tree.
            const neighbor = s.get_neighbors(curNode);
            if debug then writeln(" * diff: coeffs at ",curNode);
            var sm = get_coeffs(neighbor(1));
            var sp = get_coeffs(neighbor(2));
            var s0 = s[curNode];

            // We have s0, check if we found sm and sp at this level
            if !isNone(sm) && !isNone(sp) {
                var r = rp*sm + r0*s0 + rm*sp;
                result.s[curNode] = r * 2.0**n;
            } else {
                recur_down(curNode);
                // Sub trees can run in parallel
                diffHelper(child(1), result);
                diffHelper(child(2), result);
            }
        }
    }

    if compressed then reconstruct();
    var result = skeletonCopy();

    diffHelper((0, 0), result);

    sclean();
    return result;
  }


    /** Return sqrt(integral(f(x)**2))
     */
    def norm2() {
        if compressed then reconstruct();
        return sqrt(+ reduce [i in s] normf(i)**2);
    }


    /** Evaluate the analytic and numerical functions over the given interval
        and print the error.
     */
    def evalNPT(npt) {
        for i in 0..npt {
            var (fval, Fval) = (f(i/npt:real), this(i/npt:real));
            writeln(" -- ", toString("%0.2f", i/npt:real), ":  F_numeric()=", toString("% 0.8f", Fval),
                    "  f_analytic()=", toString("% 0.8f", fval), " err=", toString("% 0.1e", Fval-fval),
                    if abs(Fval-fval) > thresh then "  > thresh" else "");
        }
    }
}


// Copy matrix B's transpose into matrix A
def transposeCopy(A: [] real, B: [] real) where A.rank == 2 && B.rank == 2 {
    forall (i, j) in A.domain do
        A[i, j] = B[j, i];
}

// Vector-Matrix transformation
def *(V: [] real, M: [] real) where V.rank == 1 && M.rank == 2 {
    var R: [M.domain.dim(2)] real = 0.0;

    if V.domain.dim(1) != M.domain.dim(1) then
        halt("*: Vector and matrix dims must match");

    for (i, j) in M.domain do
        R[i] += V[j] * M[j, i];

    return R;
}

// Matrix-Vector multiplication 
def *(M: [] real, V: [] real) where V.rank == 1 && M.rank == 2 {
    var R: [M.domain.dim(1)] real = 0.0;

    if V.domain.dim(1) != M.domain.dim(2) then
        halt("*: Vector and matrix dims must match");

    for (i, j) in M.domain do
        R[i] += V[j] * M[i, j];

    return R;
}

// Frobenius Norm -- 2-norm for Vectors
def normf(A) where A.rank == 1 {
    return sqrt(+ reduce A**2);
}

// Inner product of A and B
def inner(A, B) where A.rank == 1 && B.rank == 1 {
    return + reduce A*B;
}


/** Return Gauss-Legendre points for order k.
 */
def gl_getPoints(k: int) {
    var ret: [0..k-1] real;
    
    select k {
        when 1 do
            ret = (0.50000000000000000);
        when 2 do
            ret = (0.78867513459481287, 0.21132486540518713);
        when 3 do
            ret = (0.88729833462074170, 0.50000000000000000, 0.11270166537925830);
        when 4 do
            ret = (0.93056815579702623, 0.66999052179242813, 0.33000947820757187, 0.06943184420297371);
        when 5 do 
            ret = (0.95308992296933193, 0.76923465505284150, 0.50000000000000000, 0.23076534494715845,
                    0.04691007703066802);
        when 6 do
            ret = (0.96623475710157603, 0.83060469323313224, 0.61930959304159849, 0.38069040695840156,
                    0.16939530676686776, 0.03376524289842397);
        when 7 do
            ret = (0.97455395617137930, 0.87076559279969723, 0.70292257568869854, 0.50000000000000000,
                    0.29707742431130141, 0.12923440720030277, 0.02544604382862070);
        when 8 do
            ret = (0.98014492824876809, 0.89833323870681336, 0.76276620495816450, 0.59171732124782495,
                    0.40828267875217511, 0.23723379504183550, 0.10166676129318664, 0.01985507175123191);
        when 9 do 
            ret = (0.98408011975381304, 0.91801555366331788, 0.80668571635029518, 0.66212671170190451,
                    0.50000000000000000, 0.33787328829809554, 0.19331428364970482, 0.08198444633668206,
                    0.01591988024618696);
        when 10 do
            ret = (0.98695326425858587, 0.93253168334449232, 0.83970478414951222, 0.71669769706462361,
                    0.57443716949081558, 0.42556283050918442, 0.28330230293537639, 0.16029521585048778,
                    0.06746831665550773, 0.01304673574141413);
        when 11 do
            ret = (0.98911432907302843, 0.94353129988404771, 0.86507600278702468, 0.75954806460340585,
                    0.63477157797617245, 0.50000000000000000, 0.36522842202382755, 0.24045193539659410,
                    0.13492399721297532, 0.05646870011595234, 0.01088567092697151);
        otherwise 
            halt("gl_getPoints: k=", k, " is out of bounds!");
    }

    return ret;
}


/** Return Gauss-Legendre weights for order k.
 */
def gl_getWeights(k: int) {
    var ret: [0..k-1] real;

    select k {
        when 1 do
            ret = (1.00000000000000000);
        when 2 do
            ret = (0.50000000000000011, 0.50000000000000011);
        when 3 do
            ret = (0.27777777777777751, 0.44444444444444442, 0.27777777777777751);
        when 4 do
            ret = (0.17392742256872701, 0.32607257743127305, 0.32607257743127305, 0.17392742256872701);
        when 5 do 
            ret = (0.11846344252809465, 0.23931433524968321, 0.28444444444444444, 0.23931433524968321,
                    0.11846344252809465);
        when 6 do
            ret = (0.08566224618958508, 0.18038078652406936, 0.23395696728634546, 0.23395696728634546,
                    0.18038078652406936, 0.08566224618958508);
        when 7 do
            ret = (0.06474248308443417, 0.13985269574463832, 0.19091502525255946, 0.20897959183673470,
                    0.19091502525255946, 0.13985269574463832, 0.06474248308443417);
        when 8 do
            ret = (0.05061426814518921, 0.11119051722668717, 0.15685332293894369, 0.18134189168918102,
                    0.18134189168918102, 0.15685332293894369, 0.11119051722668717, 0.05061426814518921);
        when 9 do 
            ret = (0.04063719418078738, 0.09032408034742866, 0.13030534820146775, 0.15617353852000135,
                    0.16511967750062989, 0.15617353852000135, 0.13030534820146775, 0.09032408034742841,
                    0.04063719418078738);
        when 10 do
            ret = (0.03333567215434403, 0.07472567457529021, 0.10954318125799088, 0.13463335965499817,
                    0.14776211235737641, 0.14776211235737641, 0.13463335965499817, 0.10954318125799088,
                    0.07472567457529021, 0.03333567215434403);
        when 11 do
            ret = (0.02783428355808731, 0.06279018473245226, 0.09314510546386703, 0.11659688229599521,
                    0.13140227225512346, 0.13646254338895031, 0.13140227225512346, 0.11659688229599521,
                    0.09314510546386703, 0.06279018473245226, 0.02783428355808731);
        otherwise
            halt("gl_getWeights: k=", k, " is out of bounds!");
    }

    return ret;
}

/** Read two scale coeffs from the input file.
 */
def hg_readCoeffs(inputfile) {
    var max_k: int;
    var coeffData = file(inputfile, path='./', mode='r');

    coeffData.open();
    coeffData.read(max_k);
    hg_MaxK = max_k;

    hg_coeffs.domain = [0..max_k, 0..2*max_k-1, 0..2*max_k-1];

    // hg_coeffs[0,..,..] is not used
    for i in 1..max_k {
        for (j, k) in [0..i*2-1, 0..i*2-1] {
            var x: real;
            coeffData.read(x);
            hg_coeffs[i, j, k] = x;
        }
    }

    coeffData.close();
}


/** Return maximum k.
 */
def hg_getMaxK(): int {
    return hg_MaxK;
}


/** Return the twoscale coefficients for the multiwavelets of order k.
 */
def hg_getCoeffs(k: int) {
    var kthCoeffs: [0..2*k-1, 0..2*k-1] real;
    // FIXME: Ideally I want to slice a 2d matrix out of the 3d matrix.
    //        Current copy code is a workaround.

    if k < 0 || k > hg_MaxK then
        halt("TwoScaleCoeffs.getCoeffs(): k=", k, " is out of bounds!");

    for (i, j) in kthCoeffs.domain do 
        kthCoeffs(i, j) = hg_coeffs(k, i, j);

    return kthCoeffs;
}


/** Evaluate the Legendre polynomials up to the given order at x
    defined on [-1,1]
    
     * x       point at which to evaluate polynomials
     * order   max order of polynomials to evaluate
     * return  [] real first k polynomials evaluated at point x
 */
def pn(x: real, order: int) {
    var p: [0..order] real;

    p[0] = 1.0;
    if order == 0 then return p;
    p[1] = x;

    for n in 1..order-1 do
        p[n+1] = n*(x*p[n] - p[n-1])/(n+1) + x*p[n];
    return p;
}


/** Evaluate the shifted normalized Legendre polynomials up to the
    given order at x defined on [0,1].

    These are also our scaling functions, phi_i(x) , i=0..k-1

    In addition to forming an orthonormal basis on [0,1] we have
    phi_j(1/2-x) = (-1)^j phi_j(1/2+x)

    (the wavelets are similar with phase (-1)^(j+k)).

     * x       point at which to evaluate polynomials
     * k       evaluate first k polynomials
     * return  double[] first k polynomials evaluated at point x
 */ 
def phi(x: real, k: int) {
    var p: [0..k-1] real = 0.0;
    var order = k-1;

    if (!phi_initialized) {
        phi_norms.domain = [0..100);
        for n in phi_norms.domain do
            phi_norms[n] = sqrt(2.0*n+1);
    }
    
    p = pn(2.0*x-1.0, order);
    for n in 1..k-1 do
        p[n] = p[n]*phi_norms[n];
    return p;
}

_extern def snprintf(buf: string, size: int, fmt: string, vals...?numvals): string;

def toString(fmt: string, x) {
    var buf: string = "12345678901234567890123456789012345678901234567890";
    snprintf(buf, 50, fmt, x);
    return buf;
}


/** Sample analytic functions 
 */

/** gaussian with square normalized to 1 */
class Fn_Test1: AFcn {
    def this(x: real): real {
        var a = 500.0;
        return (2*a/PI)**0.25 * exp(-a * (x-0.5)**2.0);
    }
};


/** derivative of test1 */
class Fn_dTest1: AFcn {
    def this(x: real): real {
        var a = 500.0;
        return -2.0*a*(x-0.5) * (2*a/PI) ** 0.25 * exp(-a * (x-0.5)**2.0);
    }
};

def main() {
    var npt = 10;

    writeln("Mad Chapel -- Differentiation Test\n");

    var fcn  : [1..1] AFcn = (Fn_Test1():AFcn);  
    var dfcn : [1..1] AFcn = (Fn_dTest1():AFcn);

    hg_readCoeffs(hg_inputfile);
    for i in fcn.domain {
        writeln("** Testing function ", i);
        var F = Function(k=5, thresh=1e-5, f=fcn[i]);

        writeln("F", i, ".norm2() = ", F.norm2());

        if verbose {
            writeln("Evaluating F", i, " on [0, 1]:");
            F.evalNPT(npt);
            writeln();
        }

        writeln("Compressing F", i, " ...");
        F.compress();

        writeln("Reconstructing F", i, " ...");
        F.reconstruct();
        writeln("F", i, ".norm2() = ", F.norm2());

        writeln("Evaluating F", i, " on [0, 1]:");
        F.evalNPT(npt);

        writeln("\nDifferentiating F", i, " ...");
        var dF = F.diff();
        dF.f = dfcn[i]:AFcn; // Fudge it for the sake of evalNPT()

        writeln("\nEvaluating dF", i, " on [0, 1]:");
        dF.evalNPT(npt);

        if i < fcn.domain.dim(1).high then
            writeln("\n======================================================================\n");
    }
}
