/**
Tabulation of the twoscale coefficients, and a routine to compute the
scaling functions (Legendre polyn scaled and shifted to the unit
interval) solely to make the 1d example standalone.
*/
module TwoScaleCoeffs {
    use Math;
    use hg;
  
    /** Return the two scale coeffs for k.
        @param k    two scale coeffs for k
        @return     Pointer to the two-dimensional array (2*k by 2*k) of two scale coeffs
        @exception  IllegalArgumentException k is out of bounds!
    */
    def getCoeffs(k: int){
        if ((k > 0) && (k <= getMaxK()))
            return hg_getCoeffs(k);
        else 
          writeln("TwoScaleCoeffs.getCoeffs(): k=", k, " is out of bounds!");
    }
    
    /** Return maximum k.
        @return     Maximum value of k
    */
    def getMaxK(): int {
        return hg_maxK;
    }

    /**
       Evaluate the Legendre polynomials up to the given order at x
       defined on [-1,1]
        @param x       point at which to evaluate polynomials
        @param order   max order of polynomials to evaluate
        @return        double[] first k polynomials evaluated at point x
    */
    def pn(x: real, order: int) /* FIXME: :[order+1] real */ {
        var p: [0..order] real;
        p[0] = 1.0;
        if(order == 0)
            return p;
        p[1] = x;
        for n in 1..order-1 do
            p[n+1] = n*(x*p[n] - p[n-1])/(n+1) + x*p[n];
        return p;
    }

    // FIXME: This should be static private
    var phi_norms: [1..0] real;
    var phi_initialized = false;
    
    /**
       Evaluate the shifted normalized Legendre polynomials up to the
       given order at x defined on [0,1].
       
       These are also our scaling functions, phi_i(x) , i=0..k-1
       
       In addition to forming an orthonormal basis on [0,1] we have
       phi_j(1/2-x) = (-1)^j phi_j(1/2+x)
       
       (the wavelets are similar with phase (-1)^(j+k)).
       
       @param x    point at which to evaluate polynomials
       @param k    evaluate first k polynomials
       @return     double[] first k polynomials evaluated at point x
    */ 
    def phi(x: real, k: int) /* FIXME: :[k] real */ {
        var p: [0..k-1] real = 0.0;
        var order = k-1;
        if (!phi_initialized)
        {
            phi_norms.domain = [0..100);
            for n in phi_norms.domain do
                phi_norms[n] = sqrt(2.0*n+1);
        }
        p = pn(2.0*x-1.0, order);
        for n in 1..k-1 do
            p[n] = p[n]*phi_norms[n];
        return p;
    }
    
    
    def mxmt(a: [] real, b: [] real) /* FIXME: where a.rank == 2 && b.rank == 2 */ {
                                     /* What really matters is that the dims of a and b match and are square */
        var c: [a.domain] real;
        
        //writeln("mxmt called on a=", a.domain, " b=", b.domain, " c=", c.domain);
        for (i, j) in a.domain do
          for k in a.domain.dim(1) do
            c[i, j] += a[k, i] * b[k, j];

        return c;
    }
    
    def err(a: [] real): real where a.rank == 2 {
        var sum = 0.0;
        
        //writeln("err called on ", a.domain);
        for i in a.domain.dim(1) {
          for j in 0..i-1 do
            sum += a[i, j]*a[i, j] + a[j, i]*a[j, i];
          sum += (a[i, i]-1.0)*(a[i, i]-1.0);
        }
        
        return sqrt(sum);
    }

    // test things out to see if they work
    // this method gets run if you run "java thisclass"
    def main () {
        for k in 1..getMaxK() {            
            var hg = getCoeffs(k);
            writeln("k= ", k, " err(mxmt(hg,hg)= ", err(mxmt(hg,hg)));
            //writeln("k= ", k, " pn(0.5,k)=", pn(0.5,k));
            //writeln("k= ", k, " phi(0.5,k)=", phi(0.5,k));
            //writeln("k= ", k, "\n", hg);
        }

        // test exception
        var hg = getCoeffs(getMaxK()+1);
    }
}
