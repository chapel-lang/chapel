module Tensor {

    import Math;
    import IO;
    import IO.FormattedIO;
    import ChapelArray;
    import NPBRandom;
    import AutoMath;

    param debugPrint = false;

    var rng = new NPBRandom.NPBRandomStream(eltType=real(64),seed=5);
    
    proc seedRandom(seed) {
        rng = new NPBRandom.NPBRandomStream(eltType=real(64),seed=(2 * seed + 1));
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

    // Does the same thing, but linear in the shape size instead. 
    proc indexInShape(shape: ?rank*int, in n: int): rank*int {
        var idxs: rank*int;
        const size = * reduce shape;
        if n > size then err("Error in indexInShape: ", n," is too large for shape.");
        for param i in 0..#rank {
            const dim = shape[rank - i - 1];
            idxs[rank - i - 1] = n % dim;
            n = Math.divFloor(n,dim);
        }
        return idxs;
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

        proc ref reshapeDomain(d: this._domain.type) {
            this._domain = d;
        }

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
        proc init(dom: ?d,type eltType) where isDomainType(d) {
            this.rank = dom.rank;
            this.eltType = eltType;
            this._domain = dom;
        }
        proc init(dom: ?d) where isDomainType(d) {
            this.rank = dom.rank;
            this.eltType = real;
            this._domain = dom;
        }
        proc init(itr) where itr.type:string == "promoted expression" || isSubtype(itr.type, _iteratorRecord) {
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
            this.init(d,eltType);
            this.data = rhs;
        }

        operator :(from: [?d] ?eltType, type toType: Tensor(d.rank,eltType)) {
            var t: Tensor(d.rank,eltType) = from;
            return t;
        }
        

        // Efficient assignment and casting for iteratable expressions. The `itr.type:string` is a type checking workaround. 

        operator =(ref lhs: Tensor(?rank,?eltType), itr) where itr.type:string == "promoted expression" || isSubtype(itr.type, _iteratorRecord) {
            lhs.reshapeDomain(itr.domain);
            lhs.data = itr;
        }

        proc init=(itr) where itr.type:string == "promoted expression" || isSubtype(itr.type, _iteratorRecord) {
            this.init(itr.domain);
            this.data = itr;
        }

        operator :(itr, type toType: Tensor(?rank,?eltType)) where itr.type:string == "promoted expression" || isSubtype(itr.type, _iteratorRecord) {
            const t: Tensor(rank,eltType) = itr;
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
            forall (i,j) in M.domain with (ref M, ref this) {
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
        proc reshape(dom_) {
            const dom = domainFromShape((...dom_.shape));
            var t = new Tensor(dom,eltType);
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
            var t = new Tensor(this.domain,eltType);
            t.data = fn(this.data);
            return t;
        }
        
        // Prints the tensor (only really works for rank 1 and 2)
        proc serializer(writer: IO.fileWriter(?), ref serializer) throws {
            const ref fw = writer;
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
        proc write(fw: IO.fileWriter(?)) throws {
            fw.write(rank);
            for s in shape do
                fw.write(s:int);
            for i in data.domain do
                fw.write(data[i]);
        }

        // Deserializer for tensor: rank,...shape,...data
        proc ref read(fr: IO.fileReader(?)) throws {
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
    
    inline proc SumReduceScanOp.accumulate(x: Tensor(?)) {
        if this.value.domain.size == 0 then this.value.reshapeDomain(x.domain);
        this.value += x;
    }
    inline proc SumReduceScanOp.combine(x: SumReduceScanOp(Tensor(?))) {
        if this.value.domain.size == 0 then this.value.reshapeDomain(x.value.domain);
        this.value += x.value;
    }
    inline proc SumReduceScanOp.accumulateOntoState(ref state: Tensor(?d), x: Tensor(d)) {
        if state.domain.size == 0 then state.reshapeDomain(x.domain);
        state += x;
    }

    operator +(lhs: Tensor(?rank,?eltType), rhs: Tensor(rank,eltType)) {
        if lhs.domain.size != rhs.domain.size then
            err("Cannot add tensors of different sizes. + ", lhs.domain.size, " != ", rhs.domain.size,"  [",lhs.shape," + ",rhs.shape,"]");
        var t = new Tensor(lhs.domain,eltType=eltType);
        t.data = lhs.data + rhs.data;
        return t;
    }

    operator +=(ref lhs: Tensor(?d), const ref rhs: Tensor(d)) {
        if lhs.domain.size == rhs.domain.size {
            lhs.data += rhs.data;
        } else if lhs.domain.size == 0 && rhs.domain.size != 0 {
            lhs.reshapeDomain(rhs.domain);
            lhs.data = rhs.data;
        } else if lhs.domain.size != 0 && rhs.domain.size == 0 {
            // do nothing
        } else {
            err("Cannot add tensors of different sizes. += ", lhs.domain.size, " != ", rhs.domain.size,"  [",lhs.shape," += ",rhs.shape,"]");
        }
    }

    operator +=(ref lhs: Tensor(?rank,?eltType), rhs) where (isArray(rhs) && rhs.rank == rank) || rhs.type == eltType {
        lhs.data += rhs;
    }

    operator +=(ref lhs: Tensor(?rank,?eltType), rhs) where rhs.type:string == "promoted expression" || rhs.type:string == "iterator" {
        lhs.data += rhs;
    }

    operator -(lhs: Tensor(?rank,?eltType), rhs: Tensor(rank,eltType)) {
        var t = new Tensor(lhs._domain,eltType=eltType);
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
        var t = new Tensor(rhs.domain,eltType=eltType);
        t.data = c * rhs.data;
        return t;
    }

    operator *(lhs: Tensor(?rank,?eltType), c: eltType) {
        var t = new Tensor(lhs.domain,eltType=eltType);
        t.data = lhs.data * c;
        return t;
    }

    operator *(lhs: Tensor(?rank,?eltType), rhs: Tensor(rank,eltType)) {
        // Hermitian product, not composition
        var t = new Tensor(lhs.domain,eltType=eltType);
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
        var w = new Tensor({0..#m});
        forall i in 0..#m with (ref w) {
            for j in 0..#n {
                w[i] += a[i, j] * v[j];
            }
        }
        return w;
    }

    // Vector-row-matrix multiplication. Fills out multiplication table between a vector and a transposed vector
    operator *(lhs: Tensor(1,?eltType), rhs: Tensor(2,eltType)): Tensor(2,eltType) {
        const (p,) = lhs.shape;
        const (m,n) = rhs.shape;
        if m != 1 then
            err("Trying to apply a vector of shape ",lhs.shape, " to a matrix of shape ", rhs.shape, ". m needs to be 1");
        
        var b = new Tensor({0..#p, 0..#n},eltType=eltType);
        foreach (i,j) in {0..#p, 0..#n} with (ref b, ref lhs, ref rhs) {
            b[i,j] = lhs[i] * rhs[0,j];
        }
        return b; 
    }

    // Matrix-matrix multiplication
    operator *(lhs: Tensor(2,?eltType), rhs: Tensor(2,eltType)): Tensor(2,eltType) {
        const (m,n) = lhs.shape;
        const (p,q) = rhs.shape;
        if n != p then
            err("Trying to apply a matrix of shape ",lhs.shape, " to a matrix of shape ", rhs.shape);

        const a = lhs.data;
        const b = rhs.data;
        var c = new Tensor({0..#m, 0..#q},eltType=eltType);
        forall (i,j) in c.domain with (ref c) {
            const row = a[i,..];
            const col = b[..,j];
            c[i,j] = + reduce (row * col);
        }
        return c;
    }

    operator /(lhs: Tensor(?d,?eltType), c: eltType) {
        var t = lhs;
        t.data /= c;
        return t;
    }

    operator -(lhs: Tensor(?d,?eltType), c: eltType) {
        var t = lhs;
        t.data -= c;
        return t;
    }

    // Compute the outer product of two vectors. 
    proc outer(a: Tensor(1), b: Tensor(1)): Tensor(2) {
        const (m,) = a.shape;
        const (n,) = b.shape;
        var c = new Tensor({0..#m, 0..#n});
        forall (i,j) in c.domain with (ref c) {
            c[i,j] = a[i] * b[j];
        }
        return c;
    }

    proc transposeMultiply(M: Tensor(2), v: Tensor(1)) {
        const (m,n) = M.shape;
        const (p,) = v.shape;
        if m != p then
            err("Trying to apply a matrix of shape ",(n,m), " to a vector of shape ", v.shape);

        var w = new Tensor({0..#n});
        forall i in 0..#n with (ref w) {
            for j in 0..#m {
                w[i] += M[j, i] * v[j];
            }
        }
        return w;
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

    inline proc relu(a: real, x: real) do
        return max(x,a * x);

    inline proc relu(a: real, in t: Tensor(?d)) {
        t.data = max(t.data,a * t.data);
        return t;
    }

    inline proc reluPrime(a: real, x: real) do
        return if x > 0.0 then 1.0 else a;
    
    inline proc reluPrime(a: real, in t: Tensor(?d)) {
        t.data = reluPrime(a,t.data);
        return t;
    }

    // Apply exponential function to each element of tensor
    proc exp(t: Tensor(?d)): Tensor(d) {
        var y = new Tensor(t.domain);
        forall i in t.domain do
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
        use Random;
        var u1, u2: [domainFromShape((...shape))] real;
        fillRandom(u1,seed=(rng.getNext() * 10):int);
        fillRandom(u2,seed=(rng.getNext() * 10):int);
        const u1p = -2.0 * Math.log(u1);
        const u2p = 2.0 * Math.pi * u2;
        var z0 = sqrt(u1p);
        z0 *= Math.cos(u2p);
        return new Tensor(z0);
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
    proc shuffle(ref x) {
        use Random;
        Random.shuffle(x,seed=(rng.getNext() * 10):int);
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
        const (h,w) = x.shape;
        var t = new Tensor({0..#(h + 2 * padding),0..#(w + 2 * padding)},real);
        t.data[padding..#h, padding..#w] = x.data;
        return t;
    }

    // Given a volume with shape (m,n,c), return a volume with shape (m + 2 * padding, n + 2 * padding, c)
    proc pad(const ref x: Tensor(3), padding: int) {
        const (h,w,c) = x.shape;
        var t = new Tensor({0..#(h + 2 * padding),0..#(w + 2 * padding),0..#c},real);
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
        return (AutoMath.floor((nh - kh + 2* padding):real / stride:real):int + 1, AutoMath.floor((nw - kw + 2 * padding):real / stride:real):int + 1);
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

        const (outH,outW): 2*int = correlateShape((kh,kw),(nh,nw),stride,padding);
        var corr = new Tensor({0..#outH,0..#outW},real);

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

        // const (outH,outW): 2*int = ((nh - kh + padding + stride) / stride,(nw - kw + padding + stride) / stride);
        const (outH,outW): 2*int = correlateShape((kh,kw),(nh,nw),stride,padding);

        var corr = new Tensor({0..#outH,0..#outW},real);

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
        const (dh,dw) = (kh + (stride * (kh - 1)), kw + (stride * (kw - 1)));
        var d = new Tensor({0..#dh,0..#dw},real);
        forall (i,j) in filter.data.domain with (ref d) {
            d[i * stride, j * stride] = filter[i,j];
            // d[i + i * stride,j + j * stride] = filter[i,j];
        }
        return d;
    }

    // Dialate a volume of filters
    proc dialate(const ref filter: Tensor(3), stride: int = 1) {
        const (kh,kw,kc) = filter.shape;
        const (dh,dw) = (kh + (stride * (kh - 1)), kw + (stride * (kw - 1)));
        var d = new Tensor({0..#dh,0..#dw,0..#kc},real);
        forall (i,j,c) in filter.data.domain with (ref d) {
            d[i * stride,j * stride,c] = filter[i,j,c];

            // d[i + i * stride,j + j * stride,c] = filter[i,j,c];
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
        if kh != kernelSize {
            grad.reshapeDomain({0..#outC,0..#kernelSize,0..#kernelSize,0..#inC});
            forall (ci,co) in {0..#inC,0..#outC} with (ref grad, var del = zeros(outH,outW), var img = zeros(inH,inW)) {
                del = delta[..,..,co];
                img = input[..,..,ci];
                const d = dialate(del,stride - 1);
                grad[co,..,..,ci] = correlate(d,img,stride=1,padding=padding)[0..#kernelSize,0..#kernelSize];
            }
            return grad;
        }

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
        var ys = new Tensor(t.domain);
        forall (x,y) in zip(t.data,ys.data) {
            y = Math.exp(x - m);
        }
        const sum = + reduce ys.data;
        return (ys,sum,ys / sum);
    }

    // Softmax function
    proc softmax(t: Tensor(?rank)) {
        const m = max reduce t.data;
        var ys = new Tensor(t.domain);
        forall (x,y) in zip(t.data,ys.data) {
            y = Math.exp(x - m);
        }
        const sum = + reduce ys.data;
        ys.data /= sum;
        return ys;
    }

    proc crossEntropyLoss(p: Tensor(1),q: Tensor(1)) {
        const sum = + reduce [(a,b) in zip(p,q)] a * Math.log(b);
        return -sum;
    }

    proc crossEntropyDelta(p: Tensor(1),q: Tensor(1)) {
        return p - q;
    }

    iter convolutionRegions(dom: domain(2,int), k: int, stride: int) {
        const (h,w) = correlateShape((k,k),dom.shape,stride,padding=0);
        for (i,j) in {0..#h,0..#w} {
            yield dom[i * stride..#k,j * stride..#k];
        }
    }

    proc kernelGradient(X: Tensor(2), dY: Tensor(2),kernelSize: int,stride: int = 1) {
        const k = kernelSize;
        const (outH,outW) = correlateShape((k,k),X.shape,stride,padding=0);

        var data: [0..#k,0..#k] real;
        const regions = convolutionRegions(X.domain,k,stride);
        forall (region,i) in zip(regions,0..#dY.domain.size) with (+ reduce data) {
            data += X[region] * dY[indexInShape(dY.shape,i)];
        }
        var dK = new Tensor({0..#k,0..#k},real);
        dK.data = data;
        return dK;
    }
}
