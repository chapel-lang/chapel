import Chai as chai;
import MNISTTester;

var net = new chai.Network(
    (
        new chai.Conv(1,64,5,stride=2),
        new chai.Conv(64,128,3),
        new chai.MaxPool(),
        new chai.SoftMax(10)
    )
);

config const numTrainImages = 80;
config const numTestImages = 50;

config const learnRate = 0.3; // 0.05;
config const batchSize = 10;
config const numEpochs = 1;


MNISTTester.train(
    network=net,
    numTrainImages=numTrainImages,
    numTestImages=numTestImages,
    learnRate=learnRate,
    batchSize=batchSize,
    numEpochs=numEpochs,
    savePath="../lib/models/test_cnn" + net.signature() + ".model",
    expectedAccuracy= 3.0 / 50.0,
    expectedLoss= 2.28
);
