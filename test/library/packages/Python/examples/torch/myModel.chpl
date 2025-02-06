use Python;
use List;


proc main() {
  var interp = new Interpreter();

  // imports from mymodel
  var myModelModule = new Module(interp, "mymodel");
  var myModelClass = new Class(myModelModule, "MyModel");

  // imports from torch
  var torch = new Module(interp, "torch");
  var tensor = new Function(torch, "tensor");
  var nn = new Module(interp, "torch.nn");
  var MSELoss = new Class(nn, "MSELoss");

  // create the model
  var model = myModelClass();
  var input_tensor = tensor([[1.0,]], kwargs=["requires_grad" => false]);
  writeln("Input tensor: ", input_tensor);

  // init model weights to 0.9
  {
    var params = model.call(list(owned Value?), "parameters");
    for p in params {
      var data_ = p!.getAttr("data");
      data_.call("fill_", 0.9);
    }
  }

  // run model
  var pred = model(input_tensor);

  // compute the loss
  var loss_fn = MSELoss();
  var target = tensor([[2.0,]]);
  writeln("Target: ", target);
  var loss = loss_fn(pred, target);
  loss.call("backward");

  // update the model's parameters
  var learning_rate = 0.01;
  var params = model.call(list(owned Value?), "parameters");
  for p in params {
    var data = p!.getAttr("data");
    var grad = p!.getAttr("grad");
    var temp = grad.call("__mul__", learning_rate);
    var temp2 = data.call("__sub__", temp);
    p!.setAttr("data", temp2);
  }

  writeln("Prediction: ", pred.call(real, "item"));
  writeln("Loss: ", loss.call(real, "item"));
  writeln("Weights:");
  {
    var params = model.call(list(owned Value?), "parameters");
    for p in params {
      writeln("  ", p!.call(real, "item"));
    }
  }
}
