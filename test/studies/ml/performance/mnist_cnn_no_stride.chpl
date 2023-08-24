import Chai as torch;
import MNISTTester;

var net = new chai.Network(
    (
        new chai.Conv(1,8,7),
        new chai.Conv(8,12,5),
        new chai.MaxPool(),
        new chai.SoftMax(10)
    )
);

config const numTrainImages = 20000;
config const numTestImages = 1000;

config const learnRate = 0.03; // 0.05;
config const batchSize = 100;
config const numEpochs = 10;


MNISTTester.train(
    network=net,
    numTrainImages=numTrainImages,
    numTestImages=numTestImages,
    learnRate=learnRate,
    batchSize=batchSize,
    numEpochs=numEpochs,
    savePath="../lib/models/test_cnn" + net.signature() + ".model"
);