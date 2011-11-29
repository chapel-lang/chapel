import math
from quadrature import gauss_legendre
from twoscalecoeffs import twoscalecoeffs,phi
from tensor import Vector, Matrix

'''

Multiresolution representation of 1-d functions using a multiwavelet
basis (similar to discontinuous spectral element with a hierarchal
decomposition).

The minimal functionality is

   __init__ (which calls init_twoscale, init_quadrature and refine)
   refine (which calls project)
   compress
   reconstruct

'''

class Function:

    # Provide defaults in the class to make it easier to have all instances
    # be consistent.    
    autorefine = 1                      # Refine during mul


    def __init__(self,k,thresh,f=None,initial_level=2):
        '''
        constructor called to initialize a Function object
        f=None and initial_level=2 are default values.
        '''
        # use first k Legendre polynomials as the basis in each box
        self.k = k

        # truncation threshold for small wavelet coefficients
        self.thresh = thresh

        # analytic function f(x) to be projected into the numerical represntation
        self.f = f

        # maximum level of refinement mostly as a sanity check
        self.max_level = 30

        # the adaptively refined coefficients (s for scaling function or polynomials,
        # and d for wavelets) are represented using nested dictionaries
        # s[n] -> dictionary of coefficients at level n
        # s[n][l] -> if it exists is a vector of coefficients on level n box l

        # initialize nested dictionaries (using python-style parallel assignment)
        self.d, self.s = {}, {}
        for n in range(self.max_level+1):
            self.d[n], self.s[n] = {}, {}

        # initialize two scale relationship between two levels
        # used to move refinement up or down one level
        self.init_twoscale(k)

        # initialize the Guass-Legendre quadrature rule
        self.init_quadrature(k)

        # blocks of the block tridiagonal derivative operator
        self.rm, self.r0, self.rp = make_dc_periodic(k)

        # keep track of what basis we are in
        self.compressed = 0

        # initial refinement of analytic function f(x)
        if f:
            for l in range(2**initial_level):
                self.refine(initial_level,l)


    def copy(self):
        '''
        Return a deep copy of self
        '''
        result = Function(self.k,self.thresh)
        result.compressed = self.compressed
        result.f = self.f
        for n in self.s.keys():
            for l in self.s[n].keys():
                result.s[n][l] = Vector(self.s[n][l])
        for n in self.d.keys():
            for l in self.d[n].keys():
                result.d[n][l] = Vector(self.d[n][l])
        return result


    def init_twoscale(self,k):
        hg = twoscalecoeffs(k)
        self.hg = Matrix(2*k,2*k)
        self.hgT = Matrix(2*k,2*k)
        for i in range(2*k):
            for j in range(2*k):
                self.hg[i,j]  = hg[i][j]
                self.hgT[i,j] = hg[j][i]


    def init_quadrature(self,order):
        x, w = gauss_legendre(order)
        self.quad_w = Vector(w)
        self.quad_x = Vector(x)
        self.quad_npt = npt = len(w)
        self.quad_phi  = Matrix(npt,self.k) # phi[point,i]
        self.quad_phiT = Matrix(self.k,npt) # phi[point,i] transpose
        self.quad_phiw = Matrix(npt,self.k) # phi[point,i]*weight[point]
        for i in range(npt):
            p = phi(self.quad_x[i],self.k)
            for m in range(self.k):
                self.quad_phi[i,m] = p[m]
                self.quad_phiT[m,i] = p[m]
                self.quad_phiw[i,m] = w[i]*p[m]


    def project(self,n,l):
        '''
        s[n][l] = integral(phi[n][l](x) * f(x))
        for box (n,l) project f(x) using quadrature rule
         into scaling function basis
        '''
        s = Vector(self.k)
        h = 0.5**n
        scale = math.sqrt(h)
        for mu in range(self.quad_npt):
            x = (l + self.quad_x[mu]) * h
            f = self.f(x)
            for i in range(self.k):
                s[i] += scale*f*self.quad_phiw[mu,i]
        return s


    def refine(self,n,l):
        '''
        refine numerical representation of f(x) to desired tolerance
        n is level in tree
        l is box index
        '''
        # project f(x) at next level
        s0,s1 = self.project(n+1,2*l), self.project(n+1,2*l+1)
        k = self.k
        s = Vector(2*k)
        s[:k],s[k:] = s0,s1

        # apply the two scale relationship to get difference coeff
        # in 1d this is O(k^2) flops (in 3d this is O(k^4) flops)
        d = s*self.hgT
        
        # check to see if within tolerance
        # normf() is Frobenius norm == 2-norm for vectors
        if d[k:].normf() < self.thresh or n >= (self.max_level-1):
            # put into tree at level n+1
            self.s[n+1][2*l], self.s[n+1][2*l+1] = s0,s1
        else:
            # these recursive calls on sub-trees can go in parallel
            self.refine(n+1, 2*l)
            self.refine(n+1, 2*l+1)


    def __evaluate(self,n,l,x):
        '''
        eval f(x) using adaptively refined numerical representation of f(x)
        answer should be within tolerance of the analytical f(x)

        Descend tree looking for box (n,l) with scaling function
        coefficients containing the point x.
        '''
        if self.s[n].has_key(l):
            p = Vector(phi(x,self.k))
            return self.s[n][l].inner(p)*math.sqrt(2.0**n)
        else:
            n, l, x = n+1, 2*l, 2*x
            if x >= 1:  l, x = l+1, x-1
            return self.__evaluate(n,l,x)


    def __call__(self,x):
        '''
        Evaluate function at x ... scaling function basis only
        call to self after creation
        looks like a Function evaluation
        say g = Function(5,1e-3,f) so g(1.0) should be similar to f(1.0)
        '''
        if self.compressed: self.reconstruct()
        return self.__evaluate(0,0,x)

    
    def norm2(self):
        '''
        Return sqrt(integral(f(x)**2))
        '''
        if self.compressed: self.reconstruct()
        
        sum = 0.0
        for n in self.s:
            for l in self.s[n]:
                sum += self.s[n][l].normf()**2
        return math.sqrt(sum)


    def compress(self, n=0, l=0):
        '''
        change from scaling function basis to multi-wavelet basis (s -> d)
        tree is filled out with s[0][0] and d
        n is level in tree
        l is box index
        '''
        if self.compressed: return

        # sub-trees can be done in parallel
        if not self.s[n+1].has_key(2*l  ): self.compress(n+1,2*l)
        if not self.s[n+1].has_key(2*l+1): self.compress(n+1,2*l+1)

        k = self.k
        s = Vector(2*k)
        s[:k],s[k:] = self.s[n+1][2*l], self.s[n+1][2*l+1]

        # apply the two scale relationship to get difference coeff
        # in 1d this is O(k^2) flops (in 3d this is O(k^4) flops)
        d = s*self.hgT

        self.s[n][l] = Vector(d[:k])
        self.d[n][l] = Vector(d[k:])
        del self.s[n+1][2*l], self.s[n+1][2*l+1]

        if n==0: self.compressed = 1


    def reconstruct(self, n=0, l=0):
        '''
        change from multi-wavelet basis to scaling function basis (d -> s)
        tree just has s at leaves
        n is level in tree
        l is box index
        '''
        if not self.compressed: return

        if self.d[n].has_key(l):
            k = self.k
            d = Vector(2*k)
            d[:k],d[k:] = self.s[n][l], self.d[n][l]
            del self.d[n][l], self.s[n][l]

            # apply the two scale relationship to get difference coeff
            # in 1d this is O(k^2) flops (in 3d this is O(k^4) flops)
            s = d*self.hg

            self.s[n+1][2*l  ] = Vector(s[:k])
            self.s[n+1][2*l+1] = Vector(s[k:])

            # sub-trees can be done in parallel
            self.reconstruct(n+1, 2*l)
            self.reconstruct(n+1, 2*l+1)

        if n == 0: self.compressed = 0


    def mul_iter(self, f1, f2, n=0, l=0):
        '''
        recursive "iteration" for mul
        multiply f1 and f2 put result into self
        '''
        if f1.s[n].has_key(l) and f2.s[n].has_key(l):
            if Function.autorefine and n+1 <= self.max_level:
                # if autorefine is set
                # we are multiplying two polynomials of order k-1
                # the result could be up to order 2(k-1)
                # so refine one more level to reduce the error
                # and try to keep it within the threshold

                # refine both one more level
                f1.recur_down(n, l, f1.s[n][l])
                f2.recur_down(n, l, f2.s[n][l])

                # scale factor for this level = sqrt((2^d)^(n+1))
                scale_factor = math.sqrt(2.0**(n+1))
                
                # multiply f1.s[n+1][2*l] and f2.s[n+1][2*l]
                f = f1.s[n+1][2*l] * self.quad_phiT
                g = f2.s[n+1][2*l] * self.quad_phiT
                f.emul(g)
                self.s[n+1][2*l] = (f * self.quad_phiw).scale(scale_factor)
                
                # multiply f1.s[n+1][2*l+1] and f2.s[n+1][2*l+1]
                f =  f1.s[n+1][2*l+1] * self.quad_phiT
                g =  f2.s[n+1][2*l+1] * self.quad_phiT
                f.emul(g)
                self.s[n+1][2*l+1] = (f * self.quad_phiw).scale(scale_factor)
                
            else:
                # if autorefine is not set or we are at the max_level
                # live with what you get
                f = f1.s[n][l] * self.quad_phiT
                g = f2.s[n][l] * self.quad_phiT
                f.emul(g)

                # scale factor for this level = sqrt((2^d)^(n+1))
                self.s[n][l] = (f * self.quad_phiw).scale(math.sqrt(2.0**(n)))

        else:
            if f1.s[n].has_key(l) and not f2.s[n].has_key(l):
                # refine this box down to next level in f1
                f1.recur_down(n, l, f1.s[n][l])
            elif not f1.s[n].has_key(l) and f2.s[n].has_key(l):
                # refine this box down to next level in f2
                f2.recur_down(n, l, f2.s[n][l])

            # calls on sub-trees can go in parallel
            self.mul_iter(f1, f2, n+1, 2*l)
            self.mul_iter(f1, f2, n+1, 2*l+1)


    def mul(self, other):
        '''
        multiplication
        For multiply both operands need to be in the scaling function basis
        so possibly call reconstruct on one or both of them first
        '''
        if self.compressed: self.reconstruct()
        if other.compressed: other.reconstruct()

        result = Function(self.k, self.thresh)
        result.mul_iter(self, other)

        self.sclean()
        other.sclean()

        return result


    def __mul__(self,other):
        '''
        python overloaded multiply operator for Function
        '''
        if isinstance(other,Function):
            return self.mul(other)
        else:
            raise "for now you can only multiply a Function by another Function"


    def gaxpy_iter(self, alpha, other, beta, n=0, l=0):
        '''
        recursive "iteration" for gaxpy
        '''
        if self.d[n].has_key(l) or other.d[n].has_key(l):
            if self.d[n].has_key(l) and other.d[n].has_key(l):
                self.d[n][l].gaxpy(alpha, other.d[n][l], beta)
                
            elif not self.d[n].has_key(l) and other.d[n].has_key(l):
                self.d[n][l] = Vector(other.d[n][l]).scale(beta)

            elif self.d[n].has_key(l) and not other.d[n].has_key(l):
                self.d[n][l].scale(alpha)

            # calls on sub-trees can go in parallel
            self.gaxpy_iter(alpha, other, beta, n+1, 2*l)
            self.gaxpy_iter(alpha, other, beta, n+1, 2*l+1)


    def gaxpy(self, alpha, other, beta):
        '''
        only in multi-wavelet basis
        i.e. run compress first on one or both operands

        self = alpha*self + beta*other

        Other is not changed.
        '''
        if not self.compressed: self.compress()
        if not other.compressed: other.compress()

        self.s[0][0].gaxpy(alpha, other.s[0][0], beta)
        self.gaxpy_iter(alpha, other, beta)
        
        # return self so operations can be chained
        return self


    def add(self,other):
        '''
        basic addition
        '''
        return self.copy().gaxpy(1.0, other, 1.0)


    def __add__(self,other):
        '''
        python overloaded addition operator for Function
        '''
        if isinstance(other,Function):
            return self.add(other)
        else:
            raise "for now you can only add a Function to another Function"


    def sub(self,other):
        '''
        basic subtraction
        '''
        return self.copy().gaxpy(1.0, other, -1.0)


    def __sub__(self,other):
        '''
        python overloaded minus operator for Function
        '''
        if isinstance(other,Function):
            return self.sub(other)
        else:
            raise "for now you can only subtract a Function from another Function"


    def summarize(self,printcoeff=0):
        '''
        Mostly for debugging, print summary of coefficients,
        optionally printing the norm of each block
        '''
        print "\n-----------------------------------------------------"
        print "k=%d thresh=%f compressed=%d" % (self.k, self.thresh, self.compressed)
        print "sum coefficients"
        for n in self.s:
            sum = 0.0
            for l in self.s[n]:
                if printcoeff:
                    print "%3d %6d %.2e" % (n,l,self.s[n][l].normf())
                else:
                    sum += self.s[n][l].normf()**2
            if not printcoeff:
                if len(self.s[n]):
                    print "   level %2d   #boxes=%3d  norm=%.2e" % \
                          (n, len(self.s[n]), math.sqrt(sum))
        print "difference coefficients"
        for n in self.s:
            sum = 0.0
            for l in self.d[n]:
                if printcoeff:
                    print "%3d %6d %.2e" % (n,l,self.d[n][l].normf())
                else:
                    sum += self.d[n][l].normf()**2
            if not printcoeff:
                if len(self.d[n]):
                    print "   level %2d   #boxes=%3d  norm=%.2e" % \
                          (n, len(self.d[n]), math.sqrt(sum))
        print "-----------------------------------------------------\n"


    def recur_down(self,n,l,s):
        '''
        In s are scaling coefficients for box n,l ... apply twoscale to generate
        the corresponding coefficients on level n+1 and insert the results into
        the tree of scaling function coefficients.
        '''
        k = self.k
        d = Vector(2*k)
        d[:k] = s
        s = d*self.hg
        self.s[n+1][2*l  ] = Vector(s[:k])
        self.s[n+1][2*l+1] = Vector(s[k:])
        

    def get_coeffs(self,n,l):
        '''
        If the scaling coefficeints in box n,l exist, return them.
        (allow here for zero boundary conditions for boxes just off
        the ends of the domain)

        Else recur up to the next level looking for a parent.  If a
        parent exists, use two scale to recur those coefficients down
        to make n,l.  Note that this modifies the tree in place and
        you should eventually call sclean to tidy up when finished.
        
        Else, return None (corresponding child boxes exist at a finer scale)
        '''
        if l<0 or l>=2**n: return Vector(self.k)
        if self.s[n].has_key(l): return self.s[n][l]

        if n>0:
            s = self.get_coeffs(n-1,l/2)
            if not s: return None
        else:
            return None  # no parent was found

        self.recur_down(n-1,l/2,s)
        
        return self.s[n][l]
        

    def sclean(self,n=0,l=0,cleaning=0):
        '''
        Differentiation (also inner and mul) may leave scaling function
        coefficients below their original level.  Recur down to the
        locally first box with scaling function coefficients.
        Delete all children below there. 
        '''
        if cleaning:
            del self.s[n][l]
        else:
            cleaning = self.s[n].has_key(l)

        # Sub trees can run in parallel
        if n < self.max_level:
            if (not cleaning) or self.s[n+1].has_key(2*l  ):
                self.sclean(n+1,2*l  ,cleaning)
            if (not cleaning) or self.s[n+1].has_key(2*l+1):
                self.sclean(n+1,2*l+1,cleaning)

        
    def diff(self,n=0,l=0,result=None):
        '''
        Differentiate the function, which corresponds to application
        of a block triadiagonal matrix.  For an adaptively refined
        target function we may need to refine boxes down until three
        boxes exist in the same scale.
        '''

        if n == 0:
            if self.compressed: self.reconstruct()
            result = Function(self.k,self.thresh)
        
        if not self.s[n].has_key(l):
            # Sub trees can run in parallel
            # Run down tree until we hit scaling function coefficients
            self.diff(n+1, 2*l  ,result)
            self.diff(n+1, 2*l+1,result)
        else:
            # These can also go in parallel since may involve
            # recurring up & down the tree.
            sm = self.get_coeffs(n,l-1)
            sp = self.get_coeffs(n,l+1)
            s0 = self.s[n][l]

            if sm and s0 and sp:
                r = self.rp*sm + self.r0*s0 + self.rm*sp
                result.s[n][l] = r.scale(2.0**n)
            else:
                self.recur_down(n,l,s0)
                # Sub trees can run in parallel
                self.diff(n+1, 2*l  ,result)
                self.diff(n+1, 2*l+1,result)

        if n == 0:
            self.sclean()
            return result


def make_dc_periodic(k):
    '''
    Return the level-0 blocks rm, r0, rp of the central
    difference derivative operator with periodic boundary
    conditions on either side.
    '''
    r0 = Matrix(k,k)
    rp = Matrix(k,k)
    rm = Matrix(k,k)
    
    iphase = 1.0
    for i in range(k):
        jphase = 1.0
        for j in range(k):
            gammaij = math.sqrt((2*i+1)*(2*j+1))
            if ((i-j)>0) and (((i-j)%2)==1):
                Kij = 2.0
            else:
                Kij = 0.0
            r0[i,j] = 0.5*(1.0 - iphase*jphase - 2.0*Kij)*gammaij
            rm[i,j] = 0.5*jphase*gammaij
            rp[i,j] =-0.5*iphase*gammaij
            jphase = -jphase
        iphase = -iphase
    return (rm, r0, rp)



if __name__ == "__main__":
    def test1(x):
        ''' gaussian with square normalized to 1 '''
        a = 500.0
        return pow(2*a/math.pi,0.25)*math.exp(-a*(x-0.5)**2)

    def dtest1(x):
        ''' derivative of test1 '''
        a = 500.0
        return -2.0*a*(x-0.5)*pow(2*a/math.pi,0.25)*math.exp(-a*(x-0.5)**2)

    def test2(x):
        ''' superposition of multiple gaussians '''
        return test1(x-0.3) + test1(x)  + test1(x+0.3)

    def dtest2(x):
        ''' derivative of test2 '''
        return dtest1(x-0.3) + dtest1(x)  + dtest1(x+0.3)

    def test3(x):
        '''
        a more interesting (singular and oscillating) function
        ... note it is never computed exactly at the singularity except by
        the test code below.
        '''
        a = 100.0*math.pi
        if x == 0.5:
            return 0.0
        else:
            return math.cos(a*(x-0.5))/math.sqrt(abs(x-0.5))

    def dtest3(x):
        ''' derivative of test3 '''

        a = 100.0*math.pi
        if x == 0.5:
            return 0.0
        else:
            s = 1.0
            if x<0.5: s = -1.0
            return -a*math.sin(a*(x-0.5))/math.sqrt(abs(x-0.5)) \
                   - s*0.5*math.cos(a*(x-0.5))/abs(x-0.5)**1.5


    # Run the code on each test function in turn

    # Note that test3 does not satisfy the boundary condition
    # assumed by the derivative operator that the function is
    # zero at and beyond the boundary ... thus, close to the
    # boundary, the derivative will be incorrect.  It will
    # be correct in the interior.

    npt = 20 # No. points to sample on test printing
    k = 5 # order of wavelet
    thresh = 1e-5 # truncation threshold
    for test,dtest in (test1,dtest1), (test2,dtest2), (test3,dtest3):
        print "\n\n"
        f = Function(k,thresh,test)
        print "norm of function is", f.norm2()

        for x in range(npt+1):
            x = x/float(npt)
            print "f(%.2f)=%12.8f exact(%.2f)=%12.8f err=%9.1e" % (x,f(x),x,test(x),f(x)-test(x))
            
    
        print "coefficients before compressing"
        f.summarize()

        f.compress()

        print "\ncoefficients after compressing"
        f.summarize()
    
        f.reconstruct()
        print "\ncoefficients after reconstructing"
        f.summarize()

        df = f.diff()
        for x in range(npt+1):
            x = x/float(npt)
            print "df/dx(%.2f)=%12.8f exact(%.2f)=%12.8f err=%9.1e" % (x,df(x),x,dtest(x),df(x)-dtest(x))

    # addition test which in turn tests gaxpy
    
    npt = 20 # No. points to sample on test printing
    k = 5 # order of wavelet
    thresh = 1e-5 # truncation threshold
    for tf1,tf2 in (test1,test1), (test1,test2), (test1,test3):
        print "\n\n"
        f1 = Function(k,thresh,tf1)
        print "norm of f1 is", f1.norm2()
        f2 = Function(k,thresh,tf2)
        print "norm of f2 is", f2.norm2()
        f3 = f1 + f2
        print "norm of f3 = f1 + f2 is", f3.norm2()
        f3.summarize()

        for x in range(npt+1):
            x = x/float(npt)
            f3_x = f3(x)
            exact_x = tf1(x)+tf2(x)
            err_x = f3_x - exact_x
            print "f3(%.2f)=%12.8f exact(%.2f)=%12.8f err=%9.1e" % (x,f3_x,x,exact_x,err_x)
            if err_x > thresh:
                print "outside thresh", thresh - err_x

    # multiplication test which in turn tests gaxpy

    npt = 20 # No. points to sample on test printing
    k = 5 # order of wavelet
    thresh = 1e-5 # truncation threshold
    Function.autorefine = 1
    for tf1,tf2 in (test1,test1), (test1,test2), (test1,test3):
        print "\n\n"
        f1 = Function(k,thresh,tf1)
        print "norm of f1 is", f1.norm2()
        f2 = Function(k,thresh,tf2)
        print "norm of f2 is", f2.norm2()
        f3 = f1 * f2
        print "norm of f3 = f1 * f2 is", f3.norm2()
        f3.summarize()
        for x in range(npt+1):
            x = x/float(npt)
            f3_x = f3(x)
            exact_x = tf1(x)*tf2(x)
            err_x = f3_x - exact_x
            print "f3(%.2f)=%12.8f exact(%.2f)=%12.8f err=%9.1e" % (x,f3_x,x,exact_x,err_x)
            if err_x > thresh:
                print "outside thresh", thresh - err_x
                
