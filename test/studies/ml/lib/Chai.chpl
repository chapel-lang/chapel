
module Chai {
    
    import Tensor as tn;
    import IO;
    import BinaryIO;

    use Tensor;
    

    record Dense {

        var outputSize: int;

        var bias: Tensor(1);
        var weights: Tensor(2);

        var biasGrad: Tensor(1);
        var weightsGrad: Tensor(2);

        var uninitialized = true;


        proc init(outputSize: int) {
            this.outputSize = outputSize;

            bias = new Tensor(1,real); // tn.randn(outputSize);
            weights = new Tensor(2,real); // tn.randn(outputSize, inputSize);

            biasGrad = new Tensor(1,real);
            weightsGrad = new Tensor(2,real);
        }

        proc forwardProp(input: Tensor(1)): Tensor(1) {
            if uninitialized {
                const inputSize = * reduce input.shape;
                const stddevB = sqrt(2.0 / outputSize);
                const stddevW = sqrt(2.0 / (inputSize + outputSize));
                bias = tn.zeros(outputSize); // tn.randn(outputSize,mu=0.0,sigma=stddevB);
                weights = tn.randn(outputSize, inputSize,mu=0.0,sigma=stddevW);

                biasGrad = tn.zeros(outputSize);
                weightsGrad = tn.zeros(outputSize, inputSize);
                uninitialized = false;
            }
            const activation = (weights * input) + bias;
            return activation;
        }
        proc forwardProp(batch: [] Tensor(1)): [] Tensor(1) {
            const batchSize = batch.size;

            var activations: [0..#batchSize] Tensor(2,real);
            activations.reshapeDomain({0..#outputSize});
            forall i in 0..#batchSize {
                activations[i] = forwardProp(batch[i]);
            }
            return activations;
        }


        proc backward(delta: Tensor(1), input: Tensor(1)): Tensor(1) {
            const newDelta = weights.transpose() * delta;
            biasGrad    += newDelta;
            weightsGrad += newDelta * input.transpose();
            return newDelta;
        }

        proc backward(deltas: [] Tensor(1), inputs: [] Tensor(1)): [] Tensor(1) {
            const batchSize = deltas.size;
            var newDeltas: [0..#batchSize] Tensor(1);

            var biasGrad = this.biasGrad;
            var weightsGrad = this.weightsGrad;
            forall (delta,input,i) in zip(deltas,inputs,0..) with (+ reduce biasGrad, + reduce weightsGrad) {
                const newDelta = weights.transpose() * delta;
                biasGrad    += newDelta;
                weightsGrad += newDelta * input.transpose();
                newDeltas[i] = newDelta;
            }
            this.biasGrad.data = biasGrad.data;
            this.weightsGrad.data = weightsGrad.data;

            return newDeltas;
        }

        proc optimize(mag: real(64)) {
            bias -= mag * biasGrad;
            weights -= mag * weightsGrad;
        }
        proc resetGradients() {
            biasGrad.data = 0;
            weightsGrad.data = 0;
        }

        proc write(fw: IO.fileWriter) throws {
            bias.write(fw);
            weights.write(fw);
        }
        proc read(fr: IO.fileReader) throws {
            bias.read(fr);
            weights.read(fr);
            uninitialized = false;
        }
        proc signature(): string {
            return "Dense(" + outputSize + ")";
        }
    }

    record Sigmoid {
        proc init(size: int) { }

        proc forwardProp(x: Tensor(1)): Tensor(1) {
            const activation = tn.sigmoid(x);
            return activation;
        }

        proc backward(delta: Tensor(1),lastInput: Tensor(1)): Tensor(1) {
            const sp = tn.sigmoidPrime(lastInput);
            const grad = delta * sp;
            return grad; 
        }

        proc optimize(mag: real) { }
        proc resetGradients() { }

        proc write(fw: IO.fileWriter) throws {
            // fw.write("[sigmoid]");
        }
        proc read(fr: IO.fileReader) throws { }

    }

    record Conv {

        var numFilters: int;
        var filters: Tensor(4);
        var filtersGrad: Tensor(4);
        var isFirstLayer = false;
        var stride: int = 1;
        var padding: int = 0;

        proc init(inChannels: int,outChannels: int, kernelSize: int = 3, stride: int = 1, padding: int = 0) {
            const numFilters = outChannels;
            this.numFilters = numFilters;

            const fanIn = kernelSize * kernelSize * inChannels;
            const fanOut = outChannels;
            const stddev = sqrt(2.0 / (fanOut + fanIn));
            this.filters = tn.randn(outChannels,kernelSize,kernelSize,inChannels,mu=0.0,sigma=stddev);
            // this.filters = tn.randn(numFilters,kernelSize,kernelSize,inChannels) / (kernelSize:real ** 2.0);
            
            this.filtersGrad = tn.zeros(numFilters,kernelSize,kernelSize,inChannels);
            this.stride = stride;
            this.padding = padding;
        }
        iter regions(images: Tensor(3)) {

        }
        iter regions(image: Tensor(2)) {
            const (h,w) = image.shape;
            for i in 0..#(h-2) {
                for j in 0..#(w-2) {
                    var region = image[i..i+3, j..j+3];
                    yield (region,i,j);
                }
            }
        }

        proc forwardProp(batch: [] Tensor(3)): [] Tensor(3) {
            const batchSize = batch.size;
            var convs: [0..#batchSize] Tensor(3);
            forall (image,i) in zip(batch,0..) {
                convs[i] = forwardProp(image);
            }
            return convs;
        }

        proc forwardProp(images: Tensor(3)): Tensor(3) {
            const (h,w,channels) = images.shape;
            const (outChannels,kh,kw,inChannels) = filters.shape;

            if channels != inChannels {
                writeln("input: ", images.shape);
                writeln("filters: ", filters.shape);
                tn.err("Conv forwardProp: inChannels mismatch");
            }

            var convs = new Tensor(3,real);
            const (newH,newW) = correlateShape((kh,kw),(h,w),stride,padding);
            convs.reshapeDomain({0..#newH, 0..#newW, 0..#outChannels});
            forall f in 0..#outChannels with (ref convs, var filter: Tensor(3) = tn.zeros(kh,kw,channels)) {
                filter.data = filters[f,..,..,..];
                convs[..,..,f] = correlate(filter=filter,input=images,stride=stride,padding=padding);
            }
            convs.data /= (inChannels:real);
            return convs;
        }
    

        proc backward(delta: Tensor(3), images: Tensor(3)): Tensor(3) {
            const (h,w,channels) = images.shape;
            const (outChannels,kh,kw,inChannels) = filters.shape;
            const (dh,dw,dc) = delta.shape;


            if dc != outChannels then tn.err("Conv backward: outChannels mismatch");
            if channels != inChannels then tn.err("Conv backward: inChannels mismatch");

            const dL_dF = tn.filterGradient(images,delta,stride,padding,kh);
            filtersGrad += dL_dF;

            var dL_dX = new Tensor(3,real);
            dL_dX.reshapeDomain({0..#h, 0..#w, 0..#inChannels});
            forall (m,n,ci) in {0..#h, 0..#w, 0..#inChannels} with (ref dL_dX) {
                var sum = 0.0;
                forall co in 0..#outChannels with (+ reduce sum) {
                    forall (i,j) in {0..#dh, 0..#dw} with (+ reduce sum) {
                        const (dXi,dXj) = correlateWeightIdx((kh,kw),(m,n),(i,j),stride,padding);
                        if dXi != -1 then
                            sum += delta[i,j,co] * filters[co,dXi,dXj,ci];
                    }
                }
                dL_dX[m,n,ci] = sum;
            }
            return dL_dX;
        }

        proc backward(deltas: [] Tensor(3), imagess: [] Tensor(3)): [] Tensor(3) {
            const batchSize = deltas.size;
            var newDeltas: [0..#batchSize] Tensor(3);
            var filtersGrad = this.filtersGrad;
            forall (delta,images,i) in zip(deltas,imagess,0..) with (+ reduce filtersGrad) {
                // coppied from above
                const (h,w,channels) = images.shape;
                const (outChannels,kh,kw,inChannels) = filters.shape;
                const (dh,dw,dc) = delta.shape;
                if dc != outChannels then tn.err("Conv backward: outChannels mismatch");
                if channels != inChannels then tn.err("Conv backward: inChannels mismatch");
                const dL_dF = tn.filterGradient(images,delta,stride,padding,kh);
                filtersGrad += dL_dF;
                var dL_dX = new Tensor(3,real);
                dL_dX.reshapeDomain({0..#h, 0..#w, 0..#inChannels});
                forall (m,n,ci) in {0..#h, 0..#w, 0..#inChannels} with (ref dL_dX) {
                    var sum = 0.0;
                    forall co in 0..#outChannels with (+ reduce sum) {
                        forall (i,j) in {0..#dh, 0..#dw} with (+ reduce sum) {
                            const (dXi,dXj) = correlateWeightIdx((kh,kw),(m,n),(i,j),stride,padding);
                            if dXi != -1 then
                                sum += delta[i,j,co] * filters[co,dXi,dXj,ci];
                        }
                    }
                    dL_dX[m,n,ci] = sum;
                }
                newDeltas[i] = dL_dX;
            }
            this.filtersGrad.data = filtersGrad.data;
            return newDeltas;
        }

        proc optimize(mag: real(64)) {
            filters -= mag * filtersGrad;

        }
        proc resetGradients() {
            filtersGrad.data = 0.0;
        }

        proc write(fw: IO.fileWriter) throws {
            fw.write(numFilters);
            filters.write(fw);
        }

        proc read(fr: IO.fileReader) throws {
            var nf = fr.read(int);
            if nf != numFilters then tn.err("Conv read: numFilters mismatch");
            filters.read(fr);
        }

        proc signature(): string {
            const (outChannels,kh,kw,inChannels) = filters.shape;
            return "Conv(" + inChannels:string + "," + outChannels:string + ",kernel=" + kw:string + ",stride=" + stride:string + ",padding=" + padding:string + ")";
        }
    }

    record MaxPool {

        proc forwardProp(batch: [] Tensor(3)): [] Tensor(3) {
            const batchSize = batch.size;
            var pools: [0..#batchSize] Tensor(3);
            forall (convs,i) in zip(batch,0..) {
                pools[i] = forwardProp(convs);
            }
            return pools;
        }

        proc forwardProp(convs: Tensor(3)): Tensor(3) {
            const (h,w,numFilters) = convs.shape;
            const newH: int = h / 2;
            const newW: int = w / 2;

            // Using arrays
            var pools: [0..#newH, 0..#newW, 0..#numFilters] real;
            forall (i,j,k) in pools.domain {
                const region = convs[i*2..#2, j*2..#2, ..];
                pools[i,j,k] = max reduce region[..,..,k];
            }
            const output = new Tensor(pools);

            return output;
        }

        proc argmax(m: [?d] real) where d.rank == 2 {
            var max: real = m[d.first];
            var maxIndex: 2*int = d.first;
            for (i,j) in m.domain {
                if m[i,j] > max {
                    max = m[i,j];
                    maxIndex = (i,j);
                }
            }
            return maxIndex - d.first;
        }

        proc backward(delta: Tensor(3), convs: Tensor(3)): Tensor(3) {
            const (h,w,numFilters) = convs.shape;

            const newH: int = h / 2;
            const newW: int = w / 2;

            var grad: [0..#h, 0..#w, 0..#numFilters] real;
            forall (i,j,k) in delta.data.domain {
                const region = convs[i*2..#2, j*2..#2, k];
                const (maxI,maxJ) = argmax(region);
                grad[i*2+maxI,j*2+maxJ,k] = delta[i,j,k];

            }
            const output = new Tensor(grad);

            return output;
        }

        proc backward(deltas: [] Tensor(3), convs: [] Tensor(3)): [] Tensor(3) {
            const batchSize = deltas.size;
            var newDeltas: [0..#batchSize] Tensor(3);
            forall (delta,convs,i) in zip(deltas,convs,0..) {
                newDeltas[i] = backward(delta,convs);
            }
            return newDeltas;
        }

        proc optimize(mag: real(64)) { }
        proc resetGradients() { }

        proc write(fw: IO.fileWriter) throws {
            // fw.write("[maxpool]");
        }
        proc read(fr: IO.fileReader) throws { }

        proc signature(): string {
            return "MaxPool()";
        }
    }

    record ReLU {
        var a: real = 0.0;
        proc init(a: real = 0.0) { this.a = a; }
        proc forwardProp(input: Tensor(?rank)) {
            var output = new Tensor(rank,real);
            output.reshapeDomain(input.domain);
            foreach i in output.domain {
                const y = input.data[i];
                output.data[i] = max(y,a * y);
            }
            return output;
        }
        proc backward(delta: Tensor(?rank),input: Tensor(rank)) {
            var output = new Tensor(rank,real);
            output.reshapeDomain(input.domain);
            foreach i in output.domain {
                const y = input.data[i];
                const dy = delta.data[i];
                output.data[i] = if y > 0.0 then dy else a * dy;
            }
            return output;
        }
        proc optimize(mag: real(64)) { }
        proc resetGradients() { }
        proc write(fw: IO.fileWriter) throws { }
        proc read(fr: IO.fileReader) throws { }

        proc signature(): string {
            return "ReLU(" + a:string + ")";
        }
    }

    record Flatten {

        proc init() { }
        proc forwardProp(input: Tensor(?inRank)): Tensor(1) {
            return input.flatten();
        }
        proc backward(delta: Tensor(1), input: Tensor(?inRank)): Tensor(inRank) {
            return delta.reshape(input.domain);
        }
        proc optimize(mag: real(64)) { }
        proc resetGradients() { }
        proc write(fw: IO.fileWriter) throws { }
        proc read(fr: IO.fileReader) throws { }

        proc signature(): string {
            return "Flatten()";
        }
    }

    record SoftMax {

        var weights: Tensor(2);
        var biases: Tensor(1);

        var weightsGrad: Tensor(2);
        var biasesGrad: Tensor(1);

        var uninitialized: bool = true;
        var outputSize: int = 0;

        proc init(inputLength: int, nodes: int) {
            weights = tn.randn(nodes,inputLength);// / inputLength;
            biases = tn.randn(nodes);

            weightsGrad = tn.zeros(nodes,inputLength);
            biasesGrad = tn.zeros(nodes);
            uninitialized = false;
        }

        proc init(outputSize: int) {
            this.outputSize = outputSize;
        }

        proc forwardProp(batch: [] Tensor(1)): [] Tensor(1) {
            const batchSize = batch.size;
            var outputs: [0..#batchSize] Tensor(1);
            forall (input,i) in zip(batch,0..) {
                outputs[i] = forwardProp(input);
            }
            return outputs;
        }


        proc forwardProp(input: Tensor(?)): Tensor(1) {
            tn.debugWrite("[enter softmax forward]");

            if uninitialized {
                const inputSize = * reduce input.shape;
                if inputSize < 1 then tn.err("Softmax input size must be > 0");

                const stddev = sqrt(2.0 / (inputSize + outputSize));
                // weights = tn.randn(outputSize,inputSize,mu=0.0,sigma=stddev);
                weights = tn.randn(outputSize,inputSize) / (inputSize: real);
                biases = tn.zeros(outputSize);// tn.randn(outputSize) / (outputSize: real);

                weightsGrad = tn.zeros(outputSize,inputSize);
                biasesGrad = tn.zeros(outputSize);

                uninitialized = false;
            }

            const flattened = input.flatten();
            const z = (weights * flattened) + biases;
            return tn.softmax(z);
        }

        proc backward(delta: Tensor(1), input: Tensor(?outRank)): Tensor(outRank) {
            const flattened = input.flatten();
            const Z = (weights * flattened) + biases;
            const (exp,expSum,softmax) = tn.softmaxParts(Z);

            const dL_dOut = delta;

            var nonZeroIdx: int = -1;
            for i in delta.data.domain do
                if delta[i] != 0.0 { nonZeroIdx = i; break; }
            
            if nonZeroIdx == -1 then tn.err("Softmax backward: delta is zero vector");
            const i = nonZeroIdx;

            var dOut_dZ: Tensor(1) = (- exp[i]) * (exp / (expSum ** 2.0));
            dOut_dZ[i] = exp[i] * (expSum - exp[i]) / (expSum ** 2.0);
            

            const dZ_dW: Tensor(1) = flattened;
            const dZ_dB: real = 1;
            const dZ_dIn: Tensor(2) = weights;

            const dL_dZ: Tensor(1) = dL_dOut[i] * dOut_dZ;

            const dL_dW: Tensor(2) = dL_dZ * dZ_dW.transpose(); // This should be dL_dW * dL_dZ.transpose();
            const dL_dB: Tensor(1) = dL_dZ * dZ_dB;
            const dL_dIn: Tensor(1) = dZ_dIn.transpose() * dL_dZ; // this is the problem

            weightsGrad += dL_dW;
            biasesGrad += dL_dB;

            return dL_dIn.reshape(input.domain);
        }

        proc backward(deltas: [] Tensor(1), inputs: [] Tensor(?)): [] Tensor(1) {
            const batchSize = deltas.size;
            var newDeltas: [0..#batchSize] Tensor(?);
            var weightsGrad = this.weightsGrad;
            var biasesGrad = this.biasesGrad;
            forall (delta,input,idx) in zip(deltas,inputs,0..) with (+ reduce weightsGrad, + reduce biasesGrad) {
                // Coppied from above
                const flattened = input.flatten();
                const Z = (weights * flattened) + biases;
                const (exp,expSum,softmax) = tn.softmaxParts(Z);
                const dL_dOut = delta;
                var nonZeroIdx: int = -1;
                for i in delta.data.domain do
                    if delta[i] != 0.0 { nonZeroIdx = i; break; }
                if nonZeroIdx == -1 then tn.err("Softmax backward: delta is zero vector");
                const i = nonZeroIdx;
                var dOut_dZ: Tensor(1) = (- exp[i]) * (exp / (expSum ** 2.0));
                dOut_dZ[i] = exp[i] * (expSum - exp[i]) / (expSum ** 2.0);
                const dZ_dW: Tensor(1) = flattened;
                const dZ_dB: real = 1;
                const dZ_dIn: Tensor(2) = weights;
                const dL_dZ: Tensor(1) = dL_dOut[i] * dOut_dZ;
                const dL_dW: Tensor(2) = dL_dZ * dZ_dW.transpose();
                const dL_dB: Tensor(1) = dL_dZ * dZ_dB;
                const dL_dIn: Tensor(1) = dZ_dIn.transpose() * dL_dZ;
                weightsGrad += dL_dW;
                biasesGrad += dL_dB;
                newDeltas[idx] = dL_dIn.reshape(input.domain);
            }
            this.weightsGrad.data = weightsGrad.data;
            this.biasesGrad.data = biasesGrad.data;
            return newDeltas;
        }

        proc optimize(mag: real(64)) {
            weights.data -= mag * weightsGrad.data;
            biases.data -= mag * biasesGrad.data;
        }
        proc resetGradients() {
            weightsGrad.data = 0.0;
            biasesGrad.data = 0.0;
        }

        proc write(fw: IO.fileWriter) throws {
            weights.write(fw);
            biases.write(fw);
        }
        proc read(fr: IO.fileReader) throws {
            weights.read(fr);
            biases.read(fr);
            uninitialized = false;
        }

        proc signature(): string {
            return "SoftMax(" + outputSize:string + ")";
        }

    }


    proc forwardPropHelp(ref layers, param n: int, x: Tensor(?)) {
        if n == layers.size then return x;

        const xNext = layers[n].forwardProp(x);
        return forwardPropHelp(layers, n+1, xNext);
    }

    proc forwardPropHelpBatch(ref layers, param n: int, xs) {
        if n == layers.size then return xs;
        const xNexts = layers[n].forwardProp(xs);
        return forwardPropHelpBatch(layers, n+1, xNexts);
    }

    proc backwardPropHelp(ref layers, param n: int, x: Tensor(?)) {
        if n == 0 then return layers[0].backward(x);

        const xNext = layers[n].backward(x);
        return backwardPropHelp(layers, n-1, xNext);
    }

    proc backwardForwardPropHelp(ref layers, param n: int, x: Tensor(?), lastDelta: Tensor(?)) {

        if n == layers.size - 1 {
            return layers[n].backward(lastDelta,x);
        }

        const lastInput = layers[n].forwardProp(x);
        const delta = backwardForwardPropHelp(layers, n+1, lastInput, lastDelta);
        return layers[n].backward(delta,x);
    }

    proc backwardForwardPropHelpBatch(ref layers, param n: int, xs, lastDeltas) {
        if n == layers.size { return lastDeltas; }

        const lastInputs = layers[n].forwardProp(xs);
        const deltas = backwardForwardPropHelpBatch(layers, n+1, lastInputs, lastDeltas);
        return layers[n].backward(deltas,xs);
    }

    record Network {
        var layers;

        proc init(layers) {
            this.layers = layers;
            this.layers[0].isFirstLayer = true;
        }

        proc forwardProp(x: Tensor(?)) {
            return forwardPropHelp(this.layers, 0, x);
        }
        proc forwardPropBatch(xs) {
            return forwardPropHelpBatch(this.layers, 0, xs);
        }
        proc backwardProp(x: Tensor(?)) {
            return backwardPropHelp(this.layers,this.layers.size - 1,x);
        }
        proc backwardProp(x: Tensor(?), delta: Tensor(?)) {
            return backwardForwardPropHelp(this.layers,0,x,delta);
        }
        proc backwardPropBatch(xs, deltas) {
            return backwardForwardPropHelpBatch(this.layers,0,xs,deltas);
        }
        proc optimize(mag: real) {
            for param i in 0..#(layers.size) {
                layers[i].optimize(mag);
            }
        }
        proc resetGradients() {
            for param i in 0..#(layers.size) {
                layers[i].resetGradients();
            }
        }

        proc cost(x: Tensor(?), y: Tensor(?)) {
            const z = this.forwardProp(x);
            return tn.frobeniusNormPowTwo(y - z);
        }

        proc optimize(x: Tensor(?),y: Tensor(?),mag: real) {
            const z = this.forwardProp(x);
            const delta = z - y;
            backwardForwardPropHelp(this.layers,0,x,delta);
            // this.optimize(mag);
        }



        proc train(data, learningRate: real) {
            var cost = 0.0;
            forall ((x,y),i) in zip(data,0..) with (ref this, ref cost) {
                // this.optimize(x,y,learningRate);
                const z = this.forwardProp(x);
                const delta = z - y;
                cost += tn.frobeniusNormPowTwo(delta);
                backwardForwardPropHelp(this.layers,0,x,delta);
                if i % ((data.size / 100):int + 1) == 0 {
                    // try! IO.stderr.write(">");
                    write(">");
                    try! IO.stdout.flush();
                }

            }
            // try! IO.stderr.writeln();
            writeln();
            cost /= data.size;
            writeln("Optimizing...");
            for param i in 0..#(layers.size) {
                layers[i].optimize(learningRate / data.size);
            }

            return cost;
        }

        proc save(path: string) throws {
            var file = IO.open(path, IO.ioMode.cw);
            var serializer = new BinaryIO.BinarySerializer(IO.ioendian.big);
            var fw = file.writer(serializer=serializer);
            // fw.write("[network]");
            fw.write(layers.size);
            for param i in 0..#(layers.size) {
                layers[i].write(fw);
            }
            fw.close();
        }

        proc load(path: string) throws {
            var file = IO.open(path, IO.ioMode.rw);
            var deserializer = new BinaryIO.BinaryDeserializer(IO.ioendian.big);
            var fr = file.reader(deserializer=deserializer);
            var size = fr.read(int);
            if size != layers.size then tn.err("Network load: size mismatch");
            for param i in 0..#(layers.size) {
                layers[i].read(fr);
            }
            return this;
        }

        proc signature(): string {
            var sig = "[";
            for param i in 0..#(layers.size) {
                sig += layers[i].signature();
                if i != layers.size - 1 then sig += ",";
            }
            sig += "]";
            return sig;
        }



    }

    proc main() {

        // Don't expect this to work

        var n2 = new Network(
            (
                new Conv(1,8,3),
                new MaxPool(),
                new Conv(8,12,3),
                new MaxPool(),
                new SoftMax(5 * 5 * 12, 10)
            )
        );
        const image = tn.randn(28,28,1);
        writeln(image);
        const convs = n2.forwardProp(image);
        writeln(convs);

    }
}