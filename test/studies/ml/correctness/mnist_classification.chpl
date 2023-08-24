
import Chai as chai;
import Tensor as tn;
use Tensor;
import Math;
import MNIST;
import Random;
import IO;
import BinaryIO;
import MNISTTester;


config param networkNum = 1;

tn.seedRandom(0);

config const modelDir = "../lib/models/";
config const numImages = 30000;

if networkNum == 1 {
    var net1 = new chai.Network(
        (
            new chai.Conv(1,12,3,stride=2),
            new chai.Conv(12,16,4),
            new chai.MaxPool(),
            new chai.SoftMax(10)
        )
    );

    MNISTTester.test(
        network=net1,
        numImages=numImages,
        modelPath= modelDir + "mnist" + net1.signature() + ".model"
    );
}

if networkNum == 2 {
    var net2 = new chai.Network(
        (
            new chai.Conv(1,32,5,stride=2),
            new chai.Conv(32,64,5,stride=1),
            new chai.MaxPool(),
            new chai.SoftMax(10)
        )
    );

    MNISTTester.test(
        network=net2,
        numImages=numImages,
        modelPath=modelDir + "mnist" + net2.signature() + ".model" // this is the lenet output
    );

    MNISTTester.test(
        network=net2,
        numImages=numImages,
        modelPath=modelDir + "mnist_cnn_epoch_60.model" // this is the lenet output
    );
}




if networkNum == 3 {
    var net3 = new chai.Network(
        (
            new chai.Conv(1,8,7),
            new chai.Conv(8,12,5),
            new chai.MaxPool(),
            new chai.SoftMax(10)
        )
    );

    MNISTTester.test(
        network=net3,
        numImages=numImages,
        modelPath=modelDir + "mnist" + net3.signature() + ".model"
    );
}



if networkNum == 4 {
    var net4 = new chai.Network(
        (
            new chai.Conv(1,64,3,stride=2),
            new chai.Conv(64,128,3),
            new chai.MaxPool(),
            new chai.SoftMax(10)
        )
    );

    MNISTTester.test(
        network=net4,
        numImages=numImages,
        modelPath=modelDir + "mnist" + net4.signature() + ".model"
    );
}




if networkNum == 5 {
    var net5 = new chai.Network(
        (
            new chai.Conv(1,8,4,stride=2),
            new chai.Conv(8,12,5),
            new chai.MaxPool(),
            new chai.SoftMax(10)
        )
    );

    MNISTTester.test(
        network=net5,
        numImages=numImages,
        modelPath=modelDir + "mnist" + net5.signature() + ".model"
    );
}

