/*
  Each layer must have the following methods:
    forwardPropBatch(batch: [] Tensor): [] Tensor;
        - Feeds each input in the batch through the layer and returns the outputs.
    backwardBatch(deltas: [] Tensor, inputs: [] Tensor): [] Tensor;
        - Computes the gradient of the loss with respect to the inputs and returns it.
    optimize(mag: real);
        - Updates the parameters of the layer using the gradients.
    resetGradients();
        - Resets the gradients to zero.
    write(fw: IO.fileWriter) throws;
        - Writes the parameters of the layer to the file.
    read(fr: IO.fileReader) throws;
        - Reads the parameters of the layer from the file.
    signature(): string;
        - Returns a string that uniquely identifies the layer and its hyperparameters parameters.
*/
module Chai {

    import Tensor as tn;
    use Tensor only Tensor;

    import IO;

    proc err(args...?n) {
        var s = "";
        for param i in 0..<n {
            s += args(i): string;
        }
        try! throw new Error(s);
    }


    /*
        Dense layer is a fully connected layer that represents an affine transformation.
        The input and output dimensions are both 1, and the output size (neurons) is specified in the initializer.
    */
    record Dense {

        var outputSize: int;
        var bias: Tensor(1);
        var weights: Tensor(2);
        var biasGrad: Tensor(1);
        var weightsGrad: Tensor(2);
        var uninitialized = true;

        proc init(outputSize: int) {
            this.outputSize = outputSize;

            this.bias = new Tensor(1,real); // tn.randn(outputSize);
            this.weights = new Tensor(2,real); // tn.randn(outputSize, inputSize);

            this.biasGrad = new Tensor(1,real);
            this.weightsGrad = new Tensor(2,real);
        }

        proc ref initialize(input: Tensor(1)) {
            if !uninitialized then err("Dense initialize: already initialized");
            const (inputSize,) = input.shape;

            // These may need to be initialized according to some distribution
            bias = tn.zeros(outputSize);
            weights = tn.randn(outputSize,inputSize);

            biasGrad = tn.zeros(outputSize);
            weightsGrad = tn.zeros(outputSize, inputSize);
            uninitialized = false;
        }

        proc ref forwardProp(input: Tensor(1)): Tensor(1) {
            if uninitialized then initialize(input);
            return (weights * input) + bias;
        }

        proc ref forwardPropBatch(batch: [] Tensor(1)): [] Tensor(1) {     
            if uninitialized then initialize(batch[0]);
            return [input in batch] (weights * input) + bias;
        }

        proc ref backward(delta: Tensor(1), input: Tensor(1)): Tensor(1) {
            weightsGrad += tn.outer(delta,input);
            biasGrad += delta;
            return tn.transposeMultiply(weights,delta);
        }

        proc ref backwardBatch(deltas: [] Tensor(1), inputs: [] Tensor(1)): [] Tensor(1) {

            var newDeltas: [inputs.domain] Tensor(1);

            var biasGradient = biasGrad.data;
            var weightsGradient = weightsGrad.data;

            forall (delta,input,newDelta) in zip(deltas,inputs,newDeltas) with (+ reduce biasGradient, + reduce weightsGradient) {
                weightsGradient += tn.outer(delta,input).data;
                biasGradient += delta.data;
                newDelta = tn.transposeMultiply(weights,delta);
            }

            biasGrad.data += biasGradient;
            weightsGrad.data += weightsGradient;

            return newDeltas;
        }

        proc ref optimize(mag: real(64)) {
            this.bias -= mag * this.biasGrad;
            this.weights -= mag * this.weightsGrad;
        }

        proc ref resetGradients() {
            this.biasGrad.data = 0;
            this.weightsGrad.data = 0;
        }

        proc ref write(fw: IO.fileWriter(?)) throws {
            bias.write(fw);
            weights.write(fw);
        }

        proc ref read(fr: IO.fileReader(?)) throws {
            bias.read(fr);
            weights.read(fr);
            uninitialized = false;
        }

        proc ref signature(): string {
            return "Dense(" + outputSize:string + ")";
        }
    }

    /*
        Sigmoid layer is a non-linear activation function.
        It is used to introduce non-linearity into the network.
        The input and output dimensions are both are the same, and can be arbitrary. 
    */
    record Sigmoid {

        proc init() { }

        proc ref forwardProp(x: Tensor(?rank)): Tensor(rank) {
            return tn.sigmoid(x);
        }

        proc ref forwardPropBatch(batch: [] ?tensorType) where isSubtype(tensorType, Tensor) {
            return forwardProp(batch);
        }

        proc const ref backward(delta: Tensor(?rank),lastInput: Tensor(rank)): Tensor(rank) {
            return delta * tn.sigmoidPrime(lastInput); 
        }

        proc const ref backwardBatch(deltas: [] ?tensorType1, inputs: [] ?tensorType2) 
            where isSubtype(tensorType1, Tensor) && isSubtype(tensorType2, Tensor) {
            return backward(deltas,inputs);
        }

        proc ref optimize(mag: real) { }

        proc ref resetGradients() { }

        proc ref write(fw: IO.fileWriter(?)) throws { }

        proc ref read(fr: IO.fileReader(?)) throws { }

        proc ref signature(): string do
            return "Sigmoid()";
    }


    /*
        Conv is a convolutional layer. Its input and output dimensions are both 3.
        The input is a 3D tensor of shape (height,width,inChannels).
        The output is a 3D tensor of shape (height',width',outChannels).
        The input and output channels are specified in the initializer, along with the kernel size, stride, and padding.
    */
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
            
            this.filtersGrad = tn.zeros(numFilters,kernelSize,kernelSize,inChannels);
            this.stride = stride;
            this.padding = padding;
        }

        proc ref forwardPropBatch(batch: [] Tensor(3)): [] Tensor(3) {
            return forwardProp(batch);
        }

        proc ref forwardProp(images: Tensor(3)): Tensor(3) {
            const (h,w,channels) = images.shape;
            const (outChannels,kh,kw,inChannels) = filters.shape;

            if channels != inChannels {
                writeln("input: ", images.shape);
                writeln("filters: ", filters.shape);
                err("Conv forwardProp: inChannels mismatch");
            }

            const (newH,newW) = correlateShape((kh,kw), (h,w), stride, padding);
            var convs = new Tensor({0..#newH, 0..#newW, 0..#outChannels});
            forall f in 0..#outChannels with (ref convs) {
                const filter: Tensor(3) = filters[f,..,..,..];
                convs[..,..,f] = correlate(filter=filter, input=images, stride=stride, padding=padding);
            }
            convs.data /= (inChannels:real);
            return convs;
        }

        proc ref backward(delta: Tensor(3), images: Tensor(3)): Tensor(3) {
            const (h,w,channels) = images.shape;
            const (outChannels,kh,kw,inChannels) = filters.shape;
            const (dh,dw,dc) = delta.shape;


            if dc != outChannels then err("Conv backward: outChannels mismatch");
            if channels != inChannels then err("Conv backward: inChannels mismatch");

            const dL_dF = tn.filterGradient(images,delta,stride,padding,kh);
            filtersGrad += dL_dF;

            var dL_dX = new Tensor({0..#h, 0..#w, 0..#inChannels});
            const deltaKernelDom = {0..#dh, 0..#dw};
            forall (m,n,ci) in {0..#h, 0..#w, 0..#inChannels} with (ref dL_dX) {
                var sum = 0.0;
                forall co in 0..#outChannels with (+ reduce sum) {
                    forall (i,j) in deltaKernelDom with (+ reduce sum) {
                        const (dXi,dXj) = correlateWeightIdx((kh,kw),(m,n),(i,j),stride,padding);
                        if dXi != -1 then
                            sum += delta[i,j,co] * filters[co,dXi,dXj,ci];
                    }
                }
                dL_dX[m,n,ci] = sum;
            }
            return dL_dX;
        }

        proc ref backwardBatch(deltas: [] Tensor(3), inputs: [] Tensor(3)): [] Tensor(3) {
            var newDeltas: [deltas.domain] Tensor(3);
            var filtersGradient = filtersGrad.data;
            forall (delta,images,newDelta) in zip(deltas,inputs,newDeltas) with (+ reduce filtersGradient) {
                // coppied from above
                const (h,w,channels) = images.shape;
                const (outChannels,kh,kw,inChannels) = filters.shape;
                const (dh,dw,dc) = delta.shape;
                if dc != outChannels then err("Conv backward: outChannels mismatch");
                if channels != inChannels then err("Conv backward: inChannels mismatch");
                const dL_dF = tn.filterGradient(images,delta,stride,padding,kh);
                filtersGradient += dL_dF.data;
                var dL_dX = new Tensor({0..#h, 0..#w, 0..#inChannels});
                const deltaKernelDom = {0..#dh, 0..#dw};
                forall (m,n,ci) in dL_dX.domain with (ref dL_dX) {
                    var sum = 0.0;
                    forall co in 0..#outChannels with (+ reduce sum) {
                        forall (i,j) in deltaKernelDom with (+ reduce sum) {
                            const (dXi,dXj) = correlateWeightIdx((kh,kw),(m,n),(i,j),stride,padding);
                            if dXi != -1 then
                                sum += delta[i,j,co] * filters[co,dXi,dXj,ci];
                        }
                    }
                    dL_dX[m,n,ci] = sum;
                }
                newDelta = dL_dX;
            }
            filtersGrad.data = filtersGradient;
            return newDeltas;
        }

        proc ref optimize(mag: real(64)) {
            filters -= mag * filtersGrad;
        }

        proc ref resetGradients() {
            filtersGrad.data = 0.0;
        }

        proc ref write(fw: IO.fileWriter(?)) throws {
            fw.write(numFilters);
            filters.write(fw);
        }

        proc ref read(fr: IO.fileReader(?)) throws {
            var nf = fr.read(int);
            if nf != numFilters then err("Conv read: numFilters mismatch");
            filters.read(fr);
        }

        proc ref signature(): string {
            const (outChannels,kh,kw,inChannels) = filters.shape;
            return "Conv(" + inChannels:string 
                    + "," + outChannels:string
                    + ",kernel=" + kw:string 
                    + ",stride=" + stride:string 
                    + ",padding=" + padding:string + ")";
        }
    }

    /*
        MaxPool is a max pooling layer. Its input and output dimensions are both 3.
        The input is a 3D tensor of shape (height,width,inChannels).
        The output is a 3D tensor of shape (height / 2,width / 2,inChannels).
        In the future, it would be easy to add a parameter to specify the pooling size.
    */

    record MaxPool {

        proc const ref forwardPropBatch(batch: [] Tensor(3)): [] Tensor(3) {
            return [convs in batch] forwardProp(convs);
        }

        proc const ref forwardProp(convs: Tensor(3)): Tensor(3) {
            const (h,w,numFilters) = convs.shape;
            const newH: int = h / 2;
            const newW: int = w / 2;

            var pools = new Tensor({0..#newH, 0..#newW, 0..#numFilters});
            forall (pool,(i,j,k)) in zip(pools.data,pools.domain) do
                pool = max reduce convs[i*2..#2, j*2..#2, k];
            
            return pools;
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

        proc ref backward(delta: Tensor(3), convs: Tensor(3)): Tensor(3) {
            const (h,w,numFilters) = convs.shape;

            const newH: int = h / 2;
            const newW: int = w / 2;

            var grad = new Tensor({0..#h, 0..#w, 0..#numFilters});
            forall (deltaRegion,(i,j,k)) in zip(delta.data,delta.data.domain) with (ref grad) {
                const region = convs[i*2..#2, j*2..#2, k];
                const (maxI,maxJ) = argmax(region);
                grad[i*2+maxI,j*2+maxJ,k] = deltaRegion;

            }
            return grad;
        }

        proc ref backwardBatch(deltas: [] Tensor(3), inputs: [] Tensor(3)): [] Tensor(3) {
            var newDeltas: [deltas.domain] Tensor(3);
            forall (delta,inputs,newDelta) in zip(deltas,inputs,newDeltas) with (ref this) {
                newDelta = backward(delta,inputs);
            }
            return newDeltas;
        }

        proc ref optimize(mag: real(64)) { }

        proc ref resetGradients() { }

        proc ref write(fw: IO.fileWriter(?)) throws { }

        proc ref read(fr: IO.fileReader(?)) throws { }

        proc ref signature(): string do
            return "MaxPool()";
    }

    /*
        ReLU (leaky) is a non-linear activation function.
        It is used to introduce non-linearity into the network.
        The input and output dimensions are both are the same, and can be arbitrary.
        The initializer takes one argument, which is the slope of the negative part of the function.
    */
    record ReLU {
        var a: real = 0.0;

        proc init(a: real = 0.0) do
            this.a = a;

        proc ref forwardProp(input: Tensor(?rank)) {
            return tn.relu(a,input);
        }

        proc ref forwardPropBatch(batch: [] ?tensorType) where isSubtype(tensorType, Tensor) {
            return tn.relu(a,batch);
        }

        proc ref backward(delta: Tensor(?rank),input: Tensor(rank)) {
            var output = new Tensor(input.domain);
            output.data = delta.data * tn.reluPrime(a,input.data);
            return output;
        }

        proc ref backwardBatch(deltas: [] ?tensorType1, inputs: [] ?tensorType2) where isSubtype(tensorType1, Tensor) && isSubtype(tensorType2, Tensor) {
            return [(delta,input) in zip(deltas,inputs)] backward(delta,input);
        }

        proc ref optimize(mag: real(64)) { }

        proc ref resetGradients() { }

        proc ref write(fw: IO.fileWriter(?)) throws { }

        proc ref read(fr: IO.fileReader(?)) throws { }

        proc ref signature(): string do
            return "ReLU(" + a:string + ")";
    }

    /*
        Flatten is a layer that flattens the input into a 1D tensor.
        The input dimension can be arbitrary, but the output dimension is always 1.
    */
    record Flatten {

        proc init() { }

        proc ref forwardProp(input: Tensor(?inRank)): Tensor(1) do
            return input.flatten();

        proc ref forwardPropBatch(batch: [] ?tensorType): [] Tensor(1) where isSubtype(tensorType,Tensor) do
            return forwardProp(batch);

        proc ref backward(delta: Tensor(1), input: Tensor(?inRank)): Tensor(inRank) do
            return delta.reshape(input.domain);

        proc ref backwardBatch(deltas: [] Tensor(1), inputs: [] ?tensorType): [] tensorType where isSubtype(tensorType,Tensor) do
            return backward(deltas,inputs);

        proc ref optimize(mag: real(64)) { }

        proc ref resetGradients() { }

        proc ref write(fw: IO.fileWriter(?)) throws { }

        proc ref read(fr: IO.fileReader(?)) throws { }

        proc ref signature(): string do
            return "Flatten()";
    }

    /*
        SoftMax is a layer that normalizes the input into a probability distribution.
        The input dimension can be arbitrary, but the output dimension is always 1.
        The initializer takes one argument, which is the number of output neurons.
    */
    record SoftMax {

        var weights: Tensor(2);
        var biases: Tensor(1);

        var weightsGrad: Tensor(2);
        var biasesGrad: Tensor(1);

        var uninitialized: bool = true;
        var outputSize: int = 0;

        proc init(outputSize: int) do
            this.outputSize = outputSize;

        proc ref initialize(input: Tensor(?)) {
            const inputSize = * reduce input.shape;
            if inputSize < 1 then err("Softmax input size must be > 0");

            const stddev = sqrt(2.0 / (inputSize + outputSize));
            // Alternative intializations
            // weights = tn.randn(outputSize,inputSize,mu=0.0,sigma=stddev);
            weights = tn.randn(outputSize,inputSize) / (inputSize: real);
            biases = tn.zeros(outputSize);// tn.randn(outputSize) / (outputSize: real);

            weightsGrad = tn.zeros(outputSize,inputSize);
            biasesGrad = tn.zeros(outputSize);

            uninitialized = false;
        }
        
        proc ref forwardPropBatch(batch: [] ?tensorType): [] Tensor(1) where isSubtype(tensorType, Tensor) {
            if uninitialized then initialize(batch[0]);

            var outputs: [batch.domain] Tensor(1);
            forall (input,output) in zip(batch,outputs) with (ref this) do
                output = forwardProp(input);
            return outputs;
        }

        proc ref forwardProp(input: Tensor(?)): Tensor(1) {
            if uninitialized then initialize(input);

            const flattened = input.flatten();
            const z = (weights * flattened) + biases;
            return tn.softmax(z);
        }

        proc ref backward(delta: Tensor(1), input: Tensor(?outRank)): Tensor(outRank) {
            const flattened = input.flatten();
            const Z = (weights * flattened) + biases;
            const (exp,expSum,softmax) = tn.softmaxParts(Z);

            const dL_dOut = delta;

            var nonZeroIdx: int = -1;
            for i in delta.data.domain do
                if delta[i] != 0.0 { nonZeroIdx = i; break; }
            
            if nonZeroIdx == -1 then err("Softmax backward: delta is zero vector");
            const i = nonZeroIdx;

            var dOut_dZ: Tensor(1) = (- exp[i]) * (exp / (expSum ** 2.0));
            dOut_dZ[i] = exp[i] * (expSum - exp[i]) / (expSum ** 2.0);
            

            const dZ_dW: Tensor(1) = flattened;
            const dZ_dB: real = 1;
            const dZ_dIn: Tensor(2) = weights;

            const dL_dZ: Tensor(1) = dL_dOut[i] * dOut_dZ;

            const dL_dW: Tensor(2) = dL_dZ * dZ_dW.transpose(); // This should be dL_dW * dL_dZ.transpose();
            const dL_dB: Tensor(1) = dL_dZ * dZ_dB;
            const dL_dIn: Tensor(1) = tn.transposeMultiply(dZ_dIn,dL_dZ); // dZ_dIn.transpose() * dL_dZ; 

            weightsGrad += dL_dW;
            biasesGrad += dL_dB;

            return dL_dIn.reshape(input.domain);
        }

        proc ref backwardBatch(deltas: [] Tensor(1), inputs: [] ?tensorType2) : [] tensorType2 where isSubtype(tensorType2, Tensor) {
            const batchSize = deltas.size;
            var newDeltas: [0..#batchSize] tensorType2;
            newDeltas.reshapeDomain(inputs[0].domain);

            var weightsGrad = this.weightsGrad;
            var biasesGrad = this.biasesGrad;
            forall (delta,input,idx) in zip(deltas,inputs,newDeltas.domain) with (ref this, ref deltas, ref inputs, ref newDeltas, + reduce weightsGrad, + reduce biasesGrad) {
                // Coppied from above
                const flattened = input.flatten();
                const Z = (weights * flattened) + biases;
                const (exp,expSum,softmax) = tn.softmaxParts(Z);
                const dL_dOut = delta;
                var nonZeroIdx: int = -1;
                for i in delta.data.domain do
                    if delta[i] != 0.0 { nonZeroIdx = i; break; }
                if nonZeroIdx == -1 then err("Softmax backward: delta is zero vector");
                const i = nonZeroIdx;
                var dOut_dZ: Tensor(1) = (- exp[i]) * (exp / (expSum ** 2.0));
                dOut_dZ[i] = exp[i] * (expSum - exp[i]) / (expSum ** 2.0);
                const dZ_dW: Tensor(1) = flattened;
                const dZ_dB: real = 1;
                const dZ_dIn: Tensor(2) = weights;
                const dL_dZ: Tensor(1) = dL_dOut[i] * dOut_dZ;
                const dL_dW: Tensor(2) = dL_dZ * dZ_dW.transpose();
                const dL_dB: Tensor(1) = dL_dZ * dZ_dB;
                const dL_dIn: Tensor(1) = tn.transposeMultiply(dZ_dIn,dL_dZ); // dZ_dIn.transpose() * dL_dZ;
                weightsGrad += dL_dW;
                biasesGrad += dL_dB;
                newDeltas[idx] = dL_dIn.reshape(input.domain);;
            }
            this.weightsGrad.data = weightsGrad.data;
            this.biasesGrad.data = biasesGrad.data;
            return newDeltas;
        }

        proc ref optimize(mag: real(64)) {
            weights.data -= mag * weightsGrad.data;
            biases.data -= mag * biasesGrad.data;
        }

        proc ref resetGradients() {
            weightsGrad.data = 0.0;
            biasesGrad.data = 0.0;
        }

        proc ref write(fw: IO.fileWriter(?)) throws {
            weights.write(fw);
            biases.write(fw);
        }
        
        proc ref read(fr: IO.fileReader(?)) throws {
            weights.read(fr);
            biases.read(fr);
            uninitialized = false;
        }

        proc ref signature(): string do
            return "SoftMax(" + outputSize:string + ")";
    }

    // Helper function that feeds the inputs through the layers and returns the outputs.
    // This is needed because the layer inputs and outputs are not all the same type.
    proc forwardPropHelpBatch(ref layers, param n: int, inputs) {
        if n == layers.size then return inputs;
        const nextInputs = layers[n].forwardPropBatch(inputs);
        return forwardPropHelpBatch(layers, n+1, nextInputs);
    }

    // Helper function that computes the gradients of the loss with respect to the inputs.
    // This is needed because the layer inputs and outputs are not all the same type.
    proc backwardForwardPropHelpBatch(ref layers, param n: int, inputs, lastDeltas) {
        if n == layers.size { return lastDeltas; }

        const lastInputs = layers[n].forwardPropBatch(inputs);
        const deltas = backwardForwardPropHelpBatch(layers, n+1, lastInputs, lastDeltas);
        return layers[n].backwardBatch(deltas=deltas,inputs=inputs);
    }

    /*
        Network is a record that contains a list of layers.
        It has methods for forward and backward propagation.
        It also has methods for saving/loading.
    */
    record Network {
        var _layers;
        proc ref layers ref do return this._layers;

        proc init(layers ...) {
            this._layers = layers;
            if this._layers[0].type == Conv then this._layers[0].isFirstLayer = true;
        }

        proc init(layers) where isTuple(layers) {
            this._layers = layers;
            if this._layers[0].type == Conv then this._layers[0].isFirstLayer = true;
        }

        proc ref forwardProp(input: Tensor(?)) {
            const inputs = [input];
            return this.forwardPropBatch(inputs)[0];
        }

        proc ref backwardProp(input: Tensor(?), delta: Tensor(?)) {
            const inputs = [input];
            const deltas = [delta];
            return this.backwardPropBatch(inputs,deltas)[0];
        }

        proc ref forwardPropBatch(inputs) {
            return forwardPropHelpBatch(this.layers, 0, inputs);
        }
        proc ref backwardPropBatch(inputs, deltas) {
            return backwardForwardPropHelpBatch(this.layers,0,inputs=inputs,lastDeltas=deltas);
        }

        proc ref optimize(mag: real) {
            for param i in 0..#(this.layers.size) {
                this.layers[i].optimize(mag);
            }
        }

        proc ref resetGradients() {
            for param i in 0..#(this.layers.size) {
                this.layers[i].resetGradients();
            }
        }

        proc ref save(path: string) throws {
            var file = IO.open(path, IO.ioMode.cw);
            var serializer = new IO.binarySerializer(IO.endianness.big);
            var fw = file.writer(serializer=serializer, locking=false);
            // fw.write("[network]");
            fw.write(layers.size);
            for param i in 0..#(layers.size) {
                layers[i].write(fw);
            }
            fw.close();
        }

        proc ref load(path: string) throws {
            var file = IO.open(path, IO.ioMode.rw);
            var deserializer = new IO.binaryDeserializer(IO.endianness.big);
            var fr = file.reader(deserializer=deserializer, locking=false);
            var size = fr.read(int);
            if size != layers.size then err("Network load: size mismatch");
            for param i in 0..#(layers.size) {
                layers[i].read(fr);
            }
            return this;
        }

        proc ref signature(): string {
            var sig = "[";
            for param i in 0..#(layers.size) {
                sig += layers[i].signature();
                if i != layers.size - 1 then sig += ",";
            }
            sig += "]";
            return sig;
        }
    }
}
