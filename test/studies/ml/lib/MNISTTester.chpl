import Chai as torch;
import Tensor as tn;
use Tensor;
import Math;
import MNIST;
import IO;
import Time;
import AutoMath;

config param perfTest = false;

tn.seedRandom(0);

config const dataPath = "./data";

// Single input forward function
proc forward(ref net, x: Tensor(?), lb: int) {
    const output = net.forwardProp(x);
    const loss = -Math.log(output[lb]);
    const acc = if tn.argmax(output.data) == lb then 1 else 0;
    return (output,loss,acc);
}

proc train(ref net, data: [] (Tensor(3),int), lr: real = 0.005) {
    const size = data.domain.size;

    net.resetGradients();

    const inputs = [im in data] im[0];
    const labels = [im in data] im[1];

    // Compute the outputs of the network using batched method. 
    const outputs = net.forwardPropBatch(inputs);

    var loss = 0.0;
    var acc = 0;
    var gradients: [0..#size] Tensor(1);
    forall (output,lb,gradient) in zip(outputs,labels,gradients) with (+ reduce loss, + reduce acc) {
        loss += -Math.log(output[lb]);
        acc += if tn.argmax(output.data) == lb then 1 else 0;
        gradient = tn.zeros(10);
        gradient[lb] = -1.0 / output[lb];
    }

    net.backwardPropBatch(inputs,gradients);
    net.optimize(lr / size);

    return (loss,acc);
}

proc decFormat(x: real, n: int) {
    return AutoMath.floor(x * (10.0**(n:real))) / (10.0**(n:real));
}

proc train(ref network, 
           numTrainImages: int,
           numTestImages: int,
           learnRate: real,
           batchSize: int,
           numEpochs: int,
           savePath: string,
           watch: bool = false) {
    
    const numImages = numTrainImages + numTestImages;

    var imageRawData = MNIST.loadImages(numImages,"../lib/mnist/data/train-images-idx3-ubyte");
    imageRawData -= 0.5;
    var (labels,labelVectors) = MNIST.loadLabels(numImages,"../lib/mnist/data/train-labels-idx1-ubyte");


    var images = [im in imageRawData] (new Tensor(im)).reshape(28,28,1);
    var labeledImages = for a in zip(images,labels) do a;

    tn.shuffle(labeledImages);

    var trainingData = labeledImages[0..#numTrainImages];
    var testingData = labeledImages[numTrainImages..#numTestImages];

    network.forwardProp(trainingData[0][0]);
    writeln("network initialized.");

    var t = new Time.stopwatch();
    t.start();

    for epoch in 0..#numEpochs {
        
        writeln("Epoch ",epoch + 1);
        network.forwardProp(trainingData[0][0]);

        tn.shuffle(trainingData);

        for i in 0..#(trainingData.size / batchSize) {
            const batchRange = (i * batchSize)..#batchSize;
            const batch = trainingData[batchRange];
            const (loss,acc) = train(network,batch,learnRate);
            if watch then IO.stdout.write("\r","[",i + 1," of ",trainingData.size / batchSize,
                                          "] (loss: ", loss / batchSize, ", accuracy: ", 
                                          acc, " / ", batchSize, ")");
            IO.stdout.flush();
        }
        IO.stdout.write("\n");
        IO.stdout.flush();

        writeln("Evaluating...");

        var loss = 0.0;
        var numCorrect = 0;

        forall (im,lb) in testingData with (+ reduce loss, + reduce numCorrect, ref network) {
            const (output_,loss_,acc_) = forward(network,im,lb);
            loss += loss_;
            numCorrect += acc_;
        }

        writeln("End of epoch ", epoch + 1,
                " Loss ", decFormat(loss / testingData.size,2),
                " Accuracy ", numCorrect, 
                " / ", testingData.size);

        if !perfTest then network.save(savePath);
    }

    if perfTest then writeln("time: ", t.elapsed());

}

proc classificationEval(ref network, numImages: int, modelPath: string) {

    network.load(modelPath);

    var imageRawData = MNIST.loadImages(numImages,"../lib/mnist/data/train-images-idx3-ubyte");
    imageRawData -= 0.5;
    const (labels,labelVectors) = MNIST.loadLabels(numImages,"../lib/mnist/data/train-labels-idx1-ubyte");


    const images = [im in imageRawData] (new Tensor(im)).reshape(28,28,1);
    const testingData = for a in zip(images,labels) do a;

    var t = new Time.stopwatch();
    t.start();

    var loss = 0.0;
    var acc = 0;

    forall (im,lb) in testingData with (+ reduce loss, + reduce acc, ref network) {
        const (output,loss_,acc_) = forward(network,im,lb);
        loss += loss_;
        acc += acc_;
    }

    loss /= numImages;

    writeln("Loss: ", decFormat(loss,2),
            " Accuracy: ",acc ,
            " / ", numImages, 
            " ", decFormat((acc * 100):real / (numImages:real),2), " %");

    t.stop();

    if perfTest then writeln("time: ", t.elapsed());
}
