import Chai as chai;
import MNISTTester;

var net = new chai.Network(
    (
        new chai.Conv(1,8,7),
        new chai.Conv(8,12,5),
        new chai.MaxPool(),
        new chai.SoftMax(10)
    )
);

config const numTrainImages = 250;
config const numTestImages = 100;

config const learnRate = 0.05; // 0.05;
config const batchSize = 100;
config const numEpochs = 2;


MNISTTester.train(
    network=net,
    numTrainImages=numTrainImages,
    numTestImages=numTestImages,
    learnRate=learnRate,
    batchSize=batchSize,
    numEpochs=numEpochs,
    savePath="../lib/models/test_cnn" + net.signature() + ".model",
    expectedAccuracy= 40.0 / 100.0,
    expectedLoss= 2.25
);
