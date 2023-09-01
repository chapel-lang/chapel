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

            bias = new Tensor(1,real); // tn.randn(outputSize);
            weights = new Tensor(2,real); // tn.randn(outputSize, inputSize);

            biasGrad = new Tensor(1,real);
            weightsGrad = new Tensor(2,real);
        }

        proc ref initialize(input: Tensor(1)) {
            if !uninitialized then err("Dense initialize: already initialized");
            const (inputSize,) = input.shape;

            const stddevB = sqrt(2.0 / outputSize);
            const stddevW = sqrt(2.0 / (inputSize + outputSize));

            // These are some alternatives for initialization. I am not sure which one is best. 
            bias = tn.zeros(outputSize);
            weights = tn.randn(outputSize,inputSize);
            // bias = tn.randn(outputSize,mu=0.0,sigma=stddevB);
            // weights = tn.randn(outputSize, inputSize,mu=0.0,sigma=stddevW);
            biasGrad = tn.zeros(outputSize);
            weightsGrad = tn.zeros(outputSize, inputSize);
            uninitialized = false;
        }

        proc ref forwardProp(input: Tensor(1)): Tensor(1) {
            if uninitialized then initialize(input);
            const activation = (this.weights * input) + this.bias;
            return activation;
        }

        proc ref forwardPropBatch(batch: [] Tensor(1)): [] Tensor(1) {     
            if uninitialized then initialize(batch[0]);

            const batchSize = batch.size;

            var activations: [0..#batchSize] Tensor(1,real) = new Tensor({0..#outputSize});

            forall i in 0..#batchSize {
                activations[i] = (this.weights * batch[i]) + this.bias;
            }
            return activations;
        }

        proc ref backward(delta: Tensor(1), input: Tensor(1)): Tensor(1) {
            const newDelta = tn.transposeMultiply(weights,delta);// weights.transpose() * delta;
            biasGrad    += newDelta;
            weightsGrad += newDelta * input.transpose();
            return newDelta;
        }

        proc ref backwardBatch(deltas: [] Tensor(1), inputs: [] Tensor(1)): [] Tensor(1) {
            const batchSize = deltas.size;
            var newDeltas: [0..#batchSize] Tensor(1);

            var biasGradient = biasGrad.data;
            var weightsGradient = weightsGrad.data;

            const weightsT = weights.transpose();

            forall (delta,input,i) in zip(deltas,inputs,0..) with (ref this, + reduce biasGradient, + reduce weightsGradient) {
                const newDelta = weightsT * delta;
                biasGradient += delta.data;
                const wg = delta * input.transpose();
                weightsGradient += wg.data;
                newDeltas[i] = newDelta;
            }
            this.biasGrad.data += biasGradient;
            this.weightsGrad.data += weightsGradient;
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

        proc forwardProp(x: Tensor(?rank)): Tensor(rank) {
            const activation = tn.sigmoid(x);
            return activation;
        }

        proc forwardPropBatch(batch: [] ?tensorType) where isSubtype(tensorType, Tensor) {
            return [b in batch] forwardProp(b);
        }

        proc backward(delta: Tensor(?rank),lastInput: Tensor(rank)): Tensor(rank) {
            const sp = tn.sigmoidPrime(lastInput);
            const grad = delta * sp;
            return grad; 
        }

        proc backwardBatch(deltas: [] ?tensorType1, inputs: [] ?tensorType2) where isSubtype(tensorType1, Tensor) && isSubtype(tensorType2, Tensor) {
            return [(d,i) in zip(deltas,inputs)] backward(d,i);
        }

        proc optimize(mag: real) { }

        proc resetGradients() { }

        proc write(fw: IO.fileWriter) throws { }

        proc read(fr: IO.fileReader) throws { }

        proc signature(): string do
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
            // this.filters = tn.randn(numFilters,kernelSize,kernelSize,inChannels) / (kernelSize:real ** 2.0);
            
            this.filtersGrad = tn.zeros(numFilters,kernelSize,kernelSize,inChannels);
            this.stride = stride;
            this.padding = padding;
        }

        proc forwardPropBatch(batch: [] Tensor(3)): [] Tensor(3) {
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
                err("Conv forwardProp: inChannels mismatch");
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


            if dc != outChannels then err("Conv backward: outChannels mismatch");
            if channels != inChannels then err("Conv backward: inChannels mismatch");

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

        proc backwardBatch(deltas: [] Tensor(3), inputs: [] Tensor(3)): [] Tensor(3) {
            const batchSize = deltas.size;
            var newDeltas: [0..#batchSize] Tensor(3);
            var filtersGrad = this.filtersGrad;
            forall (delta,images,i) in zip(deltas,inputs,0..) with (+ reduce filtersGrad) {
                // coppied from above
                const (h,w,channels) = images.shape;
                const (outChannels,kh,kw,inChannels) = filters.shape;
                const (dh,dw,dc) = delta.shape;
                if dc != outChannels then err("Conv backward: outChannels mismatch");
                if channels != inChannels then err("Conv backward: inChannels mismatch");
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
            if nf != numFilters then err("Conv read: numFilters mismatch");
            filters.read(fr);
        }

        proc signature(): string {
            const (outChannels,kh,kw,inChannels) = filters.shape;
            return "Conv(" + inChannels:string + "," + outChannels:string + ",kernel=" + kw:string + ",stride=" + stride:string + ",padding=" + padding:string + ")";
        }
    }

    /*
        MaxPool is a max pooling layer. Its input and output dimensions are both 3.
        The input is a 3D tensor of shape (height,width,inChannels).
        The output is a 3D tensor of shape (height / 2,width / 2,inChannels).
        In the future, it would be easy to add a parameter to specify the pooling size.
    */

    record MaxPool {

        proc forwardPropBatch(batch: [] Tensor(3)): [] Tensor(3) {
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

        proc backwardBatch(deltas: [] Tensor(3), inputs: [] Tensor(3)): [] Tensor(3) {
            const batchSize = deltas.size;
            var newDeltas: [0..#batchSize] Tensor(3);
            forall (delta,inputs,i) in zip(deltas,inputs,0..) {
                newDeltas[i] = backward(delta,inputs);
            }
            return newDeltas;
        }

        proc optimize(mag: real(64)) { }

        proc resetGradients() { }

        proc write(fw: IO.fileWriter) throws { }

        proc read(fr: IO.fileReader) throws { }

        proc signature(): string do
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

        proc forwardProp(input: Tensor(?rank)) {
            var output = new Tensor(rank,real);
            output.reshapeDomain(input.domain);
            foreach i in output.domain {
                const y = input.data[i];
                output.data[i] = max(y,a * y);
            }
            return output;
        }

        proc forwardPropBatch(batch: [] ?tensorType) where isSubtype(tensorType, Tensor) {
            var outputs: [batch.domain] batch.eltType;
            for i in outputs.domain {
                outputs[i] = this.forwardProp(batch[i]);
            }
            return outputs;
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

        proc backwardBatch(deltas: [] ?tensorType1, inputs: [] ?tensorType2) where isSubtype(tensorType1, Tensor) && isSubtype(tensorType2, Tensor) {
            var newDeltas: [inputs.domain] inputs.eltType;
            for i in newDeltas.domain {
                newDeltas[i] = this.backward(deltas[i],inputs[i]);
            }
            return newDeltas;
        }

        proc optimize(mag: real(64)) { }

        proc resetGradients() { }

        proc write(fw: IO.fileWriter) throws { }

        proc read(fr: IO.fileReader) throws { }

        proc signature(): string do
            return "ReLU(" + a:string + ")";
    }

    /*
        Flatten is a layer that flattens the input into a 1D tensor.
        The input dimension can be arbitrary, but the output dimension is always 1.
    */
    record Flatten {

        proc init() { }

        proc forwardProp(input: Tensor(?inRank)): Tensor(1) do
            return input.flatten();

        proc forwardPropBatch(batch: [] ?tensorType): [] Tensor(1) where isSubtype(tensorType,Tensor) do
            return [b in batch] forwardProp(b);

        proc backward(delta: Tensor(1), input: Tensor(?inRank)): Tensor(inRank) do
            return delta.reshape(input.domain);

        proc backwardBatch(deltas: [] Tensor(1), inputs: [] ?tensorType): [] tensorType where isSubtype(tensorType,Tensor) do
            return [(d,i) in zip(deltas,inputs)] backward(d,i);

        proc optimize(mag: real(64)) { }

        proc resetGradients() { }

        proc write(fw: IO.fileWriter) throws { }

        proc read(fr: IO.fileReader) throws { }

        proc signature(): string do
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

        proc initialize(input: Tensor(?)) {
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
        
        proc forwardPropBatch(batch: [] ?tensorType): [] Tensor(1) where isSubtype(tensorType, Tensor) {
            if uninitialized then initialize(batch[0]);

            const batchSize = batch.size;
            var outputs: [0..#batchSize] Tensor(1);
            forall (input,i) in zip(batch,0..) with (ref this) {
                outputs[i] = forwardProp(input);
            }
            return outputs;
        }

        proc forwardProp(input: Tensor(?)): Tensor(1) {
            tn.debugWrite("[enter softmax forward]");

            if uninitialized then initialize(input);

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

        proc backwardBatch(deltas: [] Tensor(1), inputs: [] ?tensorType2) : [] tensorType2 where isSubtype(tensorType2, Tensor) {
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

        proc signature(): string do
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

        proc save(path: string) throws {
            var file = IO.open(path, IO.ioMode.cw);
            var serializer = new IO.BinarySerializer(IO.ioendian.big);
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
            var deserializer = new IO.BinaryDeserializer(IO.ioendian.big);
            var fr = file.reader(deserializer=deserializer);
            var size = fr.read(int);
            if size != layers.size then err("Network load: size mismatch");
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
}
