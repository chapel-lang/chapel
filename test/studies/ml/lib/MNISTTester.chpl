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

proc forward(ref net,x: Tensor(?), lb: int) {
    const output = net.forwardProp(x);
    const loss = -Math.log(output[lb]);
    const acc = tn.argmax(output.data) == lb;
    return (output,loss,acc);
}

proc train(ref net, data: [] (Tensor(3),int), lr: real = 0.005) {
    // writeln("Training on ",data.domain.size," images");
    const size = data.domain.size;

    var loss = 0.0;
    var acc = 0;

    net.resetGradients();
    var gradients: [0..#size] Tensor(1,real);

    forall ((im,lb),i) in zip(data,0..) with (ref net,+ reduce loss, + reduce acc) {
        const (output,l,a) = forward(net,im,lb);
        var gradient = tn.zeros(10);
        gradient[lb] = -1.0 / output[lb];
        
        gradients[i] = gradient;
        // net.backwardProp(im,gradient);

        loss += l;
        acc += if a then 1 else 0;
    }
    const inputs = [im in data] im[0];
    net.backwardPropBatch(inputs,gradients);

    net.optimize(lr / size);

    return (loss,acc);
}



// config const numTrainImages = 50000;
// config const numTestImages = 1000;

// config const learnRate = 0.03; // 0.05;
// config const batchSize = 50;
// config const numEpochs = 60;

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
            // writeln("[",i + 1," of ", trainingData.size / batchSize, "] Loss ", loss / batchSize," Accuracy ", acc ," / ", batchSize);
            if watch then IO.stdout.write("\r","[",i + 1," of ",trainingData.size / batchSize,"] (loss: ", loss / batchSize, ", accuracy: ", acc, " / ", batchSize, ")");
            IO.stdout.flush();
        }
        IO.stdout.write("\n");
        IO.stdout.flush();

        writeln("Evaluating...");

        var loss = 0.0;
        var numCorrect = 0;

        forall (im,lb) in testingData with (+ reduce loss, + reduce numCorrect,ref network) {
            const (o,l,a) = forward(network,im,lb);
            loss += l;
            numCorrect += a;
        }

        writeln("End of epoch ", epoch + 1, " Loss ", AutoMath.floor(loss / testingData.size * 10000) / 10000, " Accuracy ", numCorrect, " / ", testingData.size);

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

    forall (im,lb) in testingData with (+ reduce loss, + reduce acc,ref network) {
        const (output,loss_,acc_) = forward(network,im,lb);
        loss += loss_;
        acc += if acc_ then 1 else 0;
    }

    loss /= numImages;

    writeln("Loss: ",AutoMath.floor(10000 * loss) / 10000," Accuracy: ",acc ," / ", numImages, " ", (acc * 100):real / (numImages:real), " %");

    t.stop();

    if perfTest then writeln("time: ", t.elapsed());
}
