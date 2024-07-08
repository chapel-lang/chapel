/*
  This program is used to train a number of different networks on the MNIST. It generates
    a number of different networks, trains them, and saves them to disk. The networks are
    then used by the MNISTTester program to test their accuracy, later in the correctness tests. 
*/

import Chai as chai;
import Tensor as tn;
use Tensor;

import MNISTTester;

tn.seedRandom(0);

config const numTrainImages = 50000;
config const numTestImages = 5000;


var net1 = new chai.Network(
    (
        new chai.Conv(1,12,3,stride=2),
        new chai.Conv(12,16,4),
        new chai.MaxPool(),
        new chai.SoftMax(10)
    )
);

writeln("Training network 1: ", net1.signature());

MNISTTester.train(
    network=net1,
    numTrainImages=numTrainImages,
    numTestImages=numTestImages,
    learnRate=0.03,
    batchSize=100,
    numEpochs=100,
    savePath="../lib/models/mnist" + net1.signature() + ".model",
    watch=true
);




var net2 = new chai.Network(
    (
        new chai.Conv(1,32,5,stride=2),
        new chai.Conv(32,64,5,stride=1),
        new chai.MaxPool(),
        new chai.SoftMax(10)
    )
);

writeln("Training network 2: ", net2.signature());

MNISTTester.train(
    network=net2,
    numTrainImages=numTrainImages,
    numTestImages=numTestImages,
    learnRate=0.03,
    batchSize=500,
    numEpochs=100,
    savePath="../lib/models/mnist" + net2.signature() + ".model",
    watch=true
);




var net3 = new chai.Network(
    (
        new chai.Conv(1,8,7),
        new chai.Conv(8,12,5),
        new chai.MaxPool(),
        new chai.SoftMax(10)
    )
);

writeln("Training network 3: ", net3.signature());

MNISTTester.train(
    network=net3,
    numTrainImages=numTrainImages,
    numTestImages=numTestImages,
    learnRate=0.05,
    batchSize=20,
    numEpochs=100,
    savePath="../lib/models/mnist" + net3.signature() + ".model",
    watch=true
);





var net4 = new chai.Network(
    (
        new chai.Conv(1,64,3,stride=2),
        new chai.Conv(64,128,3),
        new chai.MaxPool(),
        new chai.SoftMax(10)
    )
);

writeln("Training network 4: ", net4.signature());

MNISTTester.train(
    network=net4,
    numTrainImages=numTrainImages,
    numTestImages=numTestImages,
    learnRate=0.03,
    batchSize=500,
    numEpochs=100,
    savePath="../lib/models/mnist" + net4.signature() + ".model",
    watch=true
);




var net5 = new chai.Network(
    (
        new chai.Conv(1,8,4,stride=2),
        new chai.Conv(8,12,5),
        new chai.MaxPool(),
        new chai.SoftMax(10)
    )
);

writeln("Training network 5: ", net5.signature());

MNISTTester.train(
    network=net5,
    numTrainImages=numTrainImages,
    numTestImages=numTestImages,
    learnRate=0.03,
    batchSize=100,
    numEpochs=100,
    savePath="../lib/models/mnist" + net5.signature() + ".model",
    watch=true
);

