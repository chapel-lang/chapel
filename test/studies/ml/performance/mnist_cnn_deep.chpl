import Chai as chai;
import MNISTTester;

var net = new chai.Network(
    (
        new chai.Conv(1,64,5,stride=2),
        new chai.Conv(64,128,3),
        new chai.Conv(128,256,3),
        new chai.MaxPool(),
        new chai.SoftMax(10)
    )
);

config const numTrainImages = 50;
config const numTestImages = 10;

config const learnRate = 0.003; // 0.05;
config const batchSize = 100;
config const numEpochs = 1;


MNISTTester.train(
    network=net,
    numTrainImages=numTrainImages,
    numTestImages=numTestImages,
    learnRate=learnRate,
    batchSize=batchSize,
    numEpochs=numEpochs,
    savePath="../lib/models/test_cnn" + net.signature() + ".model",
    expectedAccuracy=0.0 / 10.0,
    expectedLoss=2.3
);
