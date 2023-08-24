module Tensor {

    import Math;
    import IO;
    import IO.FormattedIO;
    import ChapelArray;
    import Random;

    param debugPrint = false;

    var rng = new Random.RandomStream(eltType=real(64));
    
    proc seedRandom(seed) {
        rng = new Random.RandomStream(eltType=real(64),seed=seed);
    }

    proc err(args...?n) {
        var s = "";
        for param i in 0..<n {
            s += args(i): string;
        }
        try! throw new Error(s);
    }
    proc debugWrite(args...?n) {
        if debugPrint {
            var s = "";
            for param i in 0..<n {
                s += args(i): string;
            }
            try! IO.stdout.write(s);
            try! IO.stdout.flush();
        }
    }

    iter cartesian(X,Y) {
        for x in X {
            for y in Y {
                yield (x,y);
            }
        }
    }
    iter cartesian(param tag: iterKind,X,Y) where tag == iterKind.standalone {
        forall x in X {
            forall y in Y {
                yield (x,y);
            }
        }
    }

    proc emptyDomain(param rank: int): domain(rank,int) {
        var d: rank*range;
        for r in d do r = 0..#0;
        return {(...d)};
    }

    proc domainFromShape(shape: int ...?d): domain(d,int) {
        var ranges: d*range;
        for (size,r) in zip(shape,ranges) do r = 0..#size;
        return {(...ranges)};
    }

    // Returns the nth element in a domain of shape `bounds`. Arbitrary mixed base counter.
    proc nbase(bounds: ?rank*int, n: int): rank*int {
        var filled: rank*int;
        var idx: int = rank - 1;
        var curr: int = 0;
        var carry: bool = false;
        while curr < n {
            filled[idx] += 1;
            if filled[idx] >= bounds[idx] {
                carry = true;
                filled[idx] = 0;
                idx -= 1;
                if idx < 0 then err("Error in nbase: ", n," is too large for bounds.");
            } else {
                carry = false;
                idx = rank - 1;
                curr += 1;
            }
        }
        return filled;
    }

    record Tensor {
        param rank: int;
        type eltType = real(64);

        var _domain: domain(rank,int);
        var data: [_domain] eltType;

        proc shape do return this._domain.shape;
        proc _dom do return this._domain;

        forwarding data only this;
        forwarding data only these;

        proc reshapeDomain(d: this._domain.type) do
            this._domain = d;

        proc init(param rank: int, type eltType) {
            this.rank = rank;
            this.eltType = eltType;
            var ranges: rank*range;
            for r in ranges do r = 0..#0;
            this._domain = {(...ranges)};
        }
        proc init(type eltType, shape: int ...?dim) {
            this.init(rank=dim,eltType=eltType);
            this.reshapeDomain(domainFromShape((...shape)));
        }
        proc init(shape: int ...?dim) {
            this.init(rank=dim,eltType=real);
            this.reshapeDomain(domainFromShape((...shape)));
        }
        proc init(data: [?d] ?eltType) {
            this.rank = d.rank;
            this.eltType = eltType;
            this._domain = d;
            this.data = data;
        }
        proc init(dom: ?d) where isDomainType(d) {
            this.rank = dom.rank;
            this.eltType = real;
            this._domain = dom;
        }
        proc init(itr) where itr.type:string == "promoted expression" || itr.type:string == "iterator" {
            const A = itr;
            this.init(A);
            writeln("init(iter)");
        }

        proc init=(other: Tensor(?rank,?eltType)) {
            this.rank = other.rank;
            this.eltType = other.eltType;
            this._domain = other._domain;
            this.data = other.data;
        }

        operator =(ref lhs: Tensor(?rank,?eltType), rhs: Tensor(rank,eltType)) {
            lhs._domain = rhs._domain;
            lhs.data = rhs.data;
        }

        operator =(ref lhs: Tensor(?rank,?eltType), rhs: [?d] eltType) where d.rank == rank {
            lhs._domain = d;
            lhs.data = rhs;
        }

        proc init=(rhs: [?d] eltType) where d.rank == rank {
            this.init(d.rank,eltType);
            this.reshapeDomain(d);
            this.data = rhs;
        }

        operator :(from: [?d] ?eltType, type toType: Tensor(d.rank,eltType)) {
            var t: Tensor(d.rank,eltType) = from;
            return t;
        }
        
        // Wasnt sure what to do with these
        // operator =(ref lhs: Tensor(?rank,?eltType), in rhs: ?it) where (isRefIterType(it) || (isArray(rhs) && rhs.eltType == eltType)) && rhs.rank == rank {
        //     lhs.reshapeDomain(rhs.domain);
        //     lhs.data = rhs;
        // }
        // proc init=(in rhs: ?it) where isRefIterType(it) || isArray(rhs) {
        //     this.init(rank,eltType);
        //     this.reshapeDomain(rhs.domain);
        //     this.data = rhs;
        // }
        // operator :(in from: ?it, type toType: Tensor(?rank,?eltType)) where (isRefIterType(it) || (isArray(from) && from.eltType == eltType)) && from.rank == rank {
        //     // compilerError("Cannot convert from ",from.type:string," to ",toType:string);
        //     var t: Tensor(rank,eltType) = from;
        //     return t;
        // }

        operator =(ref lhs: Tensor(?rank,?eltType), itr) where itr.type:string == "promoted expression" || itr.type:string == "iterator" {
            lhs.reshapeDomain(itr.domain);
            lhs.data = itr;
        }

        proc init=(itr) where itr.type:string == "promoted expression" || itr.type:string == "iterator" {
            const A = itr;
            this.init(A);
        }

        operator :(itr, type toType: Tensor(?rank,?eltType)) where itr.type:string == "promoted expression" || itr.type:string == "iterator" {
            var t: Tensor(rank,eltType) = itr;
            return t;
        }


        // Transposes a vector to a row matrix
        proc transpose() where rank == 1 {
            const (p,) = shape;
            var t = new Tensor(eltType,1,p);
            t.data[0,..] = this.data;
            return t;
        }

        // Matrix transpose
        proc transpose() where rank == 2 {
            const (m,n) = this.shape;
            var M = new Tensor(2,eltType);
            M.reshapeDomain({0..#n,0..#m});
            foreach (i,j) in M.domain {
                M.data[i,j] = this.data[j,i];
            }
            return M;
        }

        // Normalizes the tensor to have unit frobenius norm
        proc normalize() {
            const norm = sqrt(frobeniusNormPowTwo(this));
            const data = this.data / norm;
            return new Tensor(data);
        }

        // Retruns new tensor with provided domain
        proc reshape(dom) {
            var t = new Tensor(dom.rank,eltType);
            t.reshapeDomain(dom);
            t.data = for (i,a) in zip(t.domain,this.data) do a;
            return t;
        }

        // Returns new tensor with provided shape
        proc reshape(shape: int ...?d) {
            const dom = domainFromShape((...shape));
            return this.reshape(dom);            
        }

        // Returns new tensor with rank 1
        proc flatten() {
            const size = this.data.domain.size;
            return this.reshape({0..#size});
        }

        // Returns a tensor with argument function applied to each element
        proc fmap(fn) {
            var t = new Tensor(rank,eltType);
            t.reshapeDomain(this.domain);
            t.data = fn(this.data);
            return t;
        }
        
        // Prints the tensor (only really works for rank 1 and 2)
        proc writeThis(fw: IO.fileWriter) throws {
            fw.write("tensor(");
            const shape = this.shape;
            var first: bool = true;
            for (x,i) in zip(data,0..) {
                const idx = nbase(shape,i);
                if idx[rank - 1] == 0 {
                    if !first {
                        fw.write("\n       ");
                    }
                    fw.write("[");
                }
                fw.writef("%{##.##########}",x);
                
                if idx[rank - 1] < shape[rank - 1] - 1 {
                    if rank == 1 then
                        fw.write("\n        ");
                    else
                        fw.write("  ");
                } else {
                    fw.write("]");
                }
                first = false;
            }
            fw.writeln(", shape=",this.shape,")");
        }

        // Serializer for tensor: rank,...shape,...data
        proc write(fw: IO.fileWriter) throws {
            fw.write(rank);
            for s in shape do
                fw.write(s:int);
            for i in data.domain do
                fw.write(data[i]);
        }

        // Deserializer for tensor: rank,...shape,...data
        proc read(fr: IO.fileReader) throws {
            var r = fr.read(int);
            if r != rank then
                err("Error reading tensor: rank mismatch.", r , " != this." , rank);
            var s = this.shape;
            for i in 0..#rank do
                s[i] = fr.read(int);
            var d = domainFromShape((...s));
            this._domain = d;
            for i in d do
                this.data[i] = fr.read(eltType);
        }
    }
    

    operator +(lhs: Tensor(?rank,?eltType), rhs: Tensor(rank,eltType)) {
        var t = new Tensor(rank=rank,eltType=eltType);
        t.reshapeDomain(lhs._domain);
        t.data = lhs.data + rhs.data;
        return t;
    }
    operator +=(ref lhs: Tensor(?d), const ref rhs: Tensor(d)) {
        lhs.data += rhs.data;
    }
    operator +=(ref lhs: Tensor(?rank,?eltType), rhs) where (isArray(rhs) && rhs.rank == rank) || rhs.type == eltType {
        lhs.data += rhs;
    }
    operator +=(ref lhs: Tensor(?rank,?eltType), rhs) where rhs.type:string == "promoted expression" || rhs.type:string == "iterator" {
        lhs.data += rhs;
    }
    operator -(lhs: Tensor(?rank,?eltType), rhs: Tensor(rank,eltType)) {
        var t = new Tensor(rank=rank,eltType=eltType);
        t.reshapeDomain(lhs._domain);
        t.data = lhs.data - rhs.data;
        return t;
    }
    operator -=(ref lhs: Tensor(?d), const ref rhs: Tensor(d)) {
        lhs.data -= rhs.data;
    }
    operator -=(ref lhs: Tensor(?rank,?eltType), rhs) where (isArray(rhs) && rhs.rank == rank) || rhs.type == eltType {
        lhs.data -= rhs;
    }
    operator *(c: ?eltType, rhs: Tensor(?rank,eltType)) {
        var t = new Tensor(rank=rank,eltType=eltType);
        t.reshapeDomain(rhs._domain);
        t.data = c * rhs.data;
        return t;
    }
    operator *(lhs: Tensor(?rank,?eltType), c: eltType) {
        var t = new Tensor(rank=rank,eltType=eltType);
        t.reshapeDomain(lhs._domain);
        t.data = lhs.data * c;
        return t;
    }
    operator *(lhs: Tensor(?rank,?eltType), rhs: Tensor(rank,eltType)) {
        // Hermitian product, not composition
        var t = new Tensor(rank=rank,eltType=eltType);
        t.reshapeDomain(lhs._domain);
        t.data = lhs.data * rhs.data;
        return t;
    }
    operator *=(ref lhs: Tensor(?d), const ref rhs: Tensor(d)) {
        lhs.data *= rhs.data;
    }
    operator *=(ref lhs: Tensor(?rank,?eltType), rhs) where (isArray(rhs) && rhs.rank == rank) || rhs.type == eltType {
        lhs.data *= rhs;
    }

    // Matrix-vector multiplication
    operator *(lhs: Tensor(2,?eltType), rhs: Tensor(1,eltType)): Tensor(1,eltType) {
        const (m,n) = lhs.shape;
        const (p,) = rhs.shape;
        if n != p then
            err("Trying to apply a matrix of shape ",lhs.shape, " to a vector of shape ", rhs.shape);

        const a = lhs.data;
        const v = rhs.data;
        var w = new Tensor(rank=1,eltType=eltType);
        w.reshapeDomain({0..#m});
        forall i in 0..#m with (ref w) {
            const row = a[i,..];
            w[i] = + reduce (row * v);
        }
        return w;
    }

    // Vector-row-matrix multiplication. Fills out multiplication table between a vector and a transposed vector
    operator *(lhs: Tensor(1,?eltType), rhs: Tensor(2,eltType)): Tensor(2,eltType) {
        const (p,) = lhs.shape;
        const (m,n) = rhs.shape;
        if m != 1 then
            err("Trying to apply a vector of shape ",lhs.shape, " to a matrix of shape ", rhs.shape);
        
        var b = new Tensor(rank=2,eltType=eltType);
        b.reshapeDomain({0..#p, 0..#n});
        foreach (i,j) in {0..#p, 0..#n} {
            b[i,j] = lhs[i] * rhs[0,j];
        }
        return b; 
    }

    operator /(lhs: Tensor(?d,?eltType), c: eltType) {
        const data = lhs.data / c;
        return new Tensor(data);
    }

    // Sigmoid function
    proc _sigmoid(x: real): real {
        return 1.0 / (1.0 + Math.exp(-x));
    }
    // Derivative of sigmoid function
    proc _sigmoidPrime(x: real): real {
        const s = _sigmoid(x);
        return s * (1.0 - s);
    }

    // Apply sigmoid function to each element of tensor
    proc sigmoid(t: Tensor(?d)): Tensor(d) {
        return t.fmap(_sigmoid);
    }

    // Apply derivative of sigmoid function to each element of tensor
    proc sigmoidPrime(t: Tensor(?d)): Tensor(d) {
        return t.fmap(_sigmoidPrime);
    }

    // Get forbenius norm before sqrt
    proc frobeniusNormPowTwo(t: Tensor(?d)): real {
        const AA = t.data ** 2.0;
        return + reduce AA;
    }

    // Apply exponential function to each element of tensor
    proc exp(t: Tensor(?d)): Tensor(d) {
        var y = new Tensor(t.domain);
        foreach i in t.domain do
            y.data[i] = Math.exp(t.data[i]);
        return y;
    }

    // Wikipedia implementation (helper for randn)
    // mu : mean
    // sigma : standard deviation
    proc boxMuller(mu: real, sigma: real) {
        var u1 = rng.getNext();
        var u2 = rng.getNext();
        var z0 = sqrt(-2.0 * Math.log(u1)) * Math.cos(2.0 * Math.pi * u2);
        return mu + (sigma * z0);
    }
    proc normal() {
        return boxMuller(0.0,1.0);
    }

    // Initialize a tensor with random values from a normal distribution
    proc randn(shape: int ...?d): Tensor(d,real) {
        var t = new Tensor((...shape));
        for i in t.domain {
            t.data[i] = normal();
        }
        return t;
    }

    // Initialize a tensor with random values from a normal distribution
    proc randn(shape: int ...?d, mu: real, sigma: real): Tensor(d,real) {
        var t = new Tensor((...shape));
        var m: [t.data.domain] real;
        for i in m.domain {
            var x: real = boxMuller(mu,sigma);
            m[i] = x;
        }
        return new Tensor(m);
    }

    // Initialize a tensor with zero values
    proc zeros(shape: int ...?d): Tensor(d,real) {
        return new Tensor((...shape));
    }

    // Shuffle a tensor in place
    proc shuffle(x) {
        Random.shuffle(x,seed=rng.seed);
    }

    // Get the max value index in an array
    proc argmax(A: [?d] real) where d.rank == 1 {
        var am: int = 0;
        for i in A.domain {
            if A[i] > A[am] {
                am = i;
            }
        }
        return am;
    }

    // Return a matrix padded by `padding` zeros on each side
    proc pad(const ref x: Tensor(2), padding: int) {
        var t = new Tensor(2,real);
        const (h,w) = x.shape;
        t.reshapeDomain({0..#(h + 2 * padding),0..#(w + 2 * padding)});
        t.data[padding..#h, padding..#w] = x.data;
        return t;
    }

    // Given a volume with shape (m,n,c), return a volume with shape (m + 2 * padding, n + 2 * padding, c)
    proc pad(const ref x: Tensor(3), padding: int) {
        var t = new Tensor(3,real);
        const (h,w,c) = x.shape;
        t.reshapeDomain({0..#(h + 2 * padding),0..#(w + 2 * padding),0..#c});
        forall (i,j,c) in x.data.domain with (ref t) {
            t[i + padding,j + padding,c] = x[i,j,c];
        }
        return t;
    }

    // Compute the resulting tensor shape of a cross correlation
    proc correlateShape(filterShape: 2*int, inputShape: 2*int, stride: int, padding: int) {
        const (kh,kw) = filterShape;
        const (nh,nw) = inputShape;
        if kh != kw then err("Correlation only works with square filters.", kh, " != ", kw);
        return ((nh - kh + padding + stride) / stride,(nw - kw + padding + stride) / stride);
    }

    // Compute the resulting tensor shape of a cross correlation
    proc correlate(const ref filter: Tensor(?), const ref input: Tensor(?), stride: int = 1, padding: int = 0) {
        if padding > 0 then 
            return correlate_(filter,pad(input,padding),stride,padding);
        return correlate_(filter=filter,input=input,stride,padding);
    }

    // Compute the resulting matrix of a cross correlation
    proc correlate_(const ref filter: Tensor(2), const ref input: Tensor(2), stride: int, padding: int): Tensor(2) {
        const (kh,kw) = filter.shape;
        const (nh,nw) = input.shape;
        if kh != kw then err("Correlation only works with square filters.", kh, " != ", kw);
        const (outH,outW): 2*int = ((nh - kh + padding + stride) / stride,(nw - kw + padding + stride) / stride);

        var corr = new Tensor(2,real);
        corr.reshapeDomain({0..#outH,0..#outW});

        forall (x,y) in corr.data.domain with (ref corr) {
            var sum = 0.0;
            forall (i,j) in filter.data.domain with (+ reduce sum) {
                sum += input[x * stride + i, y * stride + j] * filter[i,j];
            }
            corr[x,y] = sum;
        }
        return corr;
    }

    // Compute the sum of cross correlations for each filter and input channel
    proc correlate_(const ref filter: Tensor(3), const ref input: Tensor(3), stride: int, padding: int): Tensor(2) {
        const (kh,kw,cIn) = filter.shape;
        const (nh,nw,nc) = input.shape;
        if kh != kw then err("Correlation only works with square filters.", kh, " != ", kw);
        if cIn != nc then err("Correlation only works with filters and inputs of the same depth.", cIn, " != ", nc);

        const (outH,outW): 2*int = ((nh - kh + padding + stride) / stride,(nw - kw + padding + stride) / stride);

        var corr = new Tensor(2,real);
        corr.reshapeDomain({0..#outH,0..#outW});

        forall (x,y) in corr.data.domain with (ref corr) {
            var sum = 0.0;
            forall (i,j,c) in filter.data.domain with (+ reduce sum) {
                sum += input[x * stride + i, y * stride + j,c] * filter[i,j,c];
            }
            corr[x,y] = sum;
        }

        return corr;
    }

    // Compute the resulting tensor shape of a kernel dialation
    proc dialateShape(filterShape: 2*int, stride: int) {
        const (kh,kw) = filterShape;
        return (kh + (stride * (kh - 1)), kw + (stride * (kw - 1)));
    }

    // Dialate a filter
    proc dialate(const ref filter: Tensor(2), stride: int = 1) {
        const (kh,kw) = filter.shape;
        var d = new Tensor(2,real);
        const (dh,dw) = (kh + (stride * (kh - 1)), kw + (stride * (kw - 1)));
        d.reshapeDomain({0..#dh,0..#dw});
        forall (i,j) in filter.data.domain with (ref d) {
            d[i + i * stride,j + j * stride] = filter[i,j];
        }
        return d;
    }

    // Dialate a volume of filters
    proc dialate(const ref filter: Tensor(3), stride: int = 1) {
        const (kh,kw,kc) = filter.shape;
        var d = new Tensor(3,real);
        const (dh,dw) = (kh + (stride * (kh - 1)), kw + (stride * (kw - 1)));
        d.reshapeDomain({0..#dh,0..#dw,0..#kc});
        forall (i,j,c) in filter.data.domain with (ref d) {
            d[i + i * stride,j + j * stride,c] = filter[i,j,c];
        }
        return d;
    }

    // Compute the gradient of a loss with respect to a filter
    proc filterGradient(const ref input: Tensor(2), const ref delta: Tensor(2), stride: int = 1, padding: int = 0) {
        const d = dialate(delta,stride - 1);
        return correlate(d,input,stride=1,padding=padding);
    }

    // Compute the gradient of a loss with respect to a volume of filters
    proc filterGradient(const ref input: Tensor(3), const ref delta: Tensor(3), stride: int = 1, padding: int = 0,kernelSize: int) {
        const (inH,inW,inC) = input.shape;
        const (outH,outW,outC) = delta.shape;

        const (dkh,dkw) = dialateShape((outH,outW),stride - 1);
        const (kh,kw) = correlateShape((dkh,dkw),(inH,inW),stride=1,padding);

        var grad = new Tensor(4,real);
        grad.reshapeDomain({0..#outC,0..#kh,0..#kw,0..#inC});
        forall (ci,co) in {0..#inC,0..#outC} with (ref grad, var del = zeros(outH,outW), var img = zeros(inH,inW)) {
            del = delta[..,..,co];
            img = input[..,..,ci];
            const d = dialate(del,stride - 1);
            grad[co,..,..,ci] = correlate(d,img,stride=1,padding=padding);
        }
        return grad;
    }

    // Compute the gradient of a loss with respect to an input
    proc correlateWeight(const ref filter: Tensor(2), pIn: 2*int, pOut: 2*int, stride: int = 1, padding: int = 0) {
        const (m,n) = pIn;
        const (i,j) = pOut;
        const diff = (m - (stride * i - padding), n - (stride * j - padding));
        const (dx,dy) = diff;
        const (kh,kw) = filter.shape;
        if dx >= 0 && dy >= 0 && dx < kh && dy < kw then
            return filter[diff];
        return 0.0;
    }

    // Compute the index of the gradient of a loss with respect to an input. Probably the most important function here.
    proc correlateWeightIdx(filterShape: 2*int, pIn: 2*int, pOut: 2*int, stride: int = 1, padding: int = 0) {
        const (m,n) = pIn;
        const (i,j) = pOut;
        const (dx,dy) = (m - (stride * i - padding), n - (stride * j - padding));
        const (kh,kw) = filterShape;
        if dx >= 0 && dy >= 0 && dx < kh && dy < kw then
            return (dx,dy);
        return (-1,-1);
    }
    
    // Softmax but returns the sum of the exponentials and the exponentials themselves
    proc softmaxParts(t: Tensor(?rank)) {
        const m = max reduce t.data;
        var y = t;
        y.data -= m;
        foreach i in y.data.domain {
            y.data[i] = Math.exp(y.data[i]);
        }
        const sum = + reduce y.data;
        
        return (y,sum,y / sum);
    }

    // Softmax function
    proc softmax(t: Tensor(?rank)) {
        const m = max reduce t.data;
        var y = t;
        y.data -= m;
        foreach i in y.data.domain {
            y.data[i] = Math.exp(y.data[i]);
        }
        const sum = + reduce y.data;
        y.data /= sum;
        return y;
    }















/* these functions were not needed for my final implementation. they are also wrong, but I would like to see the most efficient implementation of them in chapel. */

    proc convolve(kernel: [?dk] ?eltType, X: [?dx] eltType) where dx.rank == 2 && dk.rank == 2 {
        const (h,w) = X.shape;
        const (kh,kw) = kernel.shape;
        const newH = h - (kh - 1);
        const newW = w - (kw - 1);
        var Y: [0..#newH,0..#newW] eltType;
        // forall (i,j) in Y.domain with (var region: [0..#kh,0..#kw] eltType) {
        //     region = X[i..#kh, j..#kw];
        //     Y[i,j] = + reduce (region * kernel);
        // }

        forall (i,j) in Y.domain {
            var sum = 0.0;
            forall (k,l) in kernel.domain with (+ reduce sum) {
                sum += X[i + k, j + l] * kernel[k,l];
            }
            Y[i,j] = sum;
        }
        return Y;
    }


    proc convolveRotateRefPadding(const ref kernel: [?dk] ?eltType, const ref X: [?dx] eltType, ref Y: [?dy] eltType) where dx.rank == 2 && dk.rank == 2 {
        const (h,w) = X.shape;
        const (kh,kw) = kernel.shape;
        const newH = h - (kh - 1);
        const newW = w - (kw - 1);
        // var Y: [0..#newH,0..#newW] eltType;
        
        forall (i,j) in Y.domain {
            var sum = 0.0;
            forall (k,l) in kernel.domain with (+ reduce sum) {
                sum += X[h - i - k - 1, h - j - l - 1] * kernel[k,l];
            }
            Y[i,j] = sum;
        }
        // return Y;
    }

    proc convolveRotate(kernel: [?dk] ?eltType, X: [?dx] eltType) where dx.rank == 2 && dk.rank == 2 {
        const (h,w) = X.shape;
        const (kh,kw) = kernel.shape;
        const newH = h - (kh - 1);
        const newW = w - (kw - 1);
        var Y: [0..#newH,0..#newW] eltType;
        
        forall (i,j) in Y.domain {
            var sum = 0.0;
            forall (k,l) in kernel.domain with (+ reduce sum) {
                sum += X[i + k, j + l] * kernel[kh - k - 1, kw - l - 1];
            }
            Y[i,j] = sum;
        }
        return Y;
    }

    proc convolve(kernel: Tensor(2), X: Tensor(2)): Tensor(2) {
        return new Tensor(convolve(kernel.data,X.data));
    }

    proc rotate180(kernel: [?d] ?eltType) where d.rank == 2 {
        const (kh,kw) = kernel.shape;
        var ker: [0..#kh,0..#kw] eltType;
        forall (i,j) in ker.domain {
            ker[i,j] = kernel[kh - i - 1, kw - j - 1];
        }
        return ker;
    }

    proc rotate180(kernel: Tensor(2)): Tensor(2) {
        return new Tensor(rotate180(kernel.data));
    }

    proc fullConvolve(kernel: [?dk] ?eltType, X: [?dx] eltType) where dx.rank == 2 && dk.rank == 2 {
        const (h,w) = X.shape;
        const (kh,kw) = kernel.shape;
        const (paddingH,paddingW) = (kh - 1,kw - 1);
        const newH = h + 2 * paddingH;
        const newW = w + 2 * paddingW;
        var Y: [0..#newH,0..#newW] eltType;
        Y = 0.0;
        Y[paddingH..#h, paddingW..#w] = X;
        return convolve(kernel,Y);
    }

    proc fullConvolve(kernel: Tensor(2), X: Tensor(2)): Tensor(2) {
        return new Tensor(fullConvolve(kernel.data,X.data));
    }

}

