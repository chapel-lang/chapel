import Chai as chai;
import Tensor as tn;
use Tensor only Tensor;
import Math;

tn.seedRandom(0);

config const epochs = 1000;
config const learnRate = 0.05;

// var network = new shared chai.Network(
//     (
//         new shared chai.Dense(2),
//         // new chai.ReLU(0.1),
//         // new chai.Sigmoid(),
//         // new chai.Dense(3),
//         // new chai.ReLU(0.1),
//         new shared chai.Sigmoid(),
//         new shared chai.Dense(3),
//         new shared chai.Sigmoid(),
//         new shared chai.Dense(2),
//         new shared chai.Sigmoid()
//         // new chai.Sigmoid()
//         // new chai.ReLU(0.1)
//     )
// );
var net = new shared chai.Network(
    (
        new shared chai.Dense(3),
        new shared chai.Sigmoid(),
        new shared chai.Dense(4),
        new shared chai.Sigmoid(),
        new shared chai.Dense(2),
        new shared chai.Sigmoid()
    )
);




proc forward(batch: [] (Tensor(1),Tensor(1))) {
    var outputs: [0..#batch.size] Tensor(1);
    var losses: [0..#batch.size] real;
    var lossesGrad: [0..#batch.size] Tensor(1);

    forall ((input,expected),idx) in zip(batch,0..) with (ref net) {
        const output = net.forwardProp(input);
        outputs[idx] = output;
        losses[idx] = (+ reduce ((expected - output).data ** 2.0)) / output.domain.size;
        lossesGrad[idx] = expected - output;
    }
    return (losses, lossesGrad, outputs);
}

proc train(batch: [] (Tensor(1),Tensor(1)), lr:real) {

    var (losses,deltas,outputs) = forward(batch);
    var inputs = [(i,o) in batch] i;


    net.resetGradients();
    net.backwardPropBatch(inputs,deltas);
    net.optimize(lr);

    const loss = (+ reduce losses) / batch.size;

    return loss;
}

proc test(batch: [] (Tensor(1),Tensor(1))) {
    const (losses,lossesGrad, outputs) = forward( batch);
    for ((input,expected),output) in zip(batch,outputs) {
        writeln(input.data, " -> ", output.data, " [", tn.argmax(expected.data) ,"]");
    }
}


var inputs: [0..#4] Tensor(1);
inputs[0] = [0.0,0.0];
inputs[1] = [0.0,1.0];
inputs[2] = [1.0,0.0];
inputs[3] = [1.0,1.0];

var outputs: [0..#4] Tensor(1);
outputs[0] = [1.0,0.0];
outputs[1] = [0.0,1.0];
outputs[2] = [0.0,1.0];
outputs[3] = [1.0,0.0];

var t = tn.randn(3,2);
writeln(t, " ",t.shape);
writeln(t.transpose(), " ",t.transpose().shape);

var dy = tn.randn(3);
var dx = t.transpose() * dy;
writeln(dx, " ",dx.shape);



// halt(0);

const batch = for a in zip(inputs,outputs) do a;
net.forwardPropBatch(inputs);


for e in 1..epochs {
    const loss = train(batch,learnRate);
    writeln("Epoch: ", e, " Loss: ", loss);
}

test(batch);
