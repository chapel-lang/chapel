import Chai as chai;
import MNISTTester;

var net = new chai.Network(
    (
        new chai.Conv(1,16,5),
        new chai.Conv(16,32,3),
        new chai.MaxPool(),
        new chai.Flatten(),
        new chai.Dense(200),
        new chai.Sigmoid(),
        new chai.SoftMax(10)
    )
);

config const numTrainImages = 125;
config const numTestImages = 100;

config const learnRate = 0.03; // 0.05;
config const batchSize = 20;
config const numEpochs = 2;


MNISTTester.train(
    network=net,
    numTrainImages=numTrainImages,
    numTestImages=numTestImages,
    learnRate=learnRate,
    batchSize=batchSize,
    numEpochs=numEpochs,
    savePath="../lib/models/test_cnn" + net.signature() + ".model",
    expectedAccuracy= 18.0 / 100.0,
    expectedLoss= 2.23
);
