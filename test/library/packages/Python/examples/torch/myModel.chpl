use Python;
use List;


proc main() {
  var interp = new Interpreter();

  // imports from mymodel
  var myModelModule = interp.importModule("mymodel");
  var myModelClass = myModelModule.get("MyModel");

  // imports from torch
  var torch = interp.importModule("torch");
  var tensor = torch.get("tensor");
  var nn = interp.importModule("torch.nn");
  var MSELoss = nn.get("MSELoss");

  // create the model
  var model = myModelClass();
  var input_tensor = tensor([[1.0,]], kwargs=["requires_grad" => false]);
  writeln("Input tensor: ", input_tensor);

  // init model weights to 0.9
  {
    var params = model.call(list(owned Value?), "parameters");
    for p in params {
      var data_ = p!.get("data");
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
    var data = p!.get("data");
    var grad = p!.get("grad");
    var temp = grad.call("__mul__", learning_rate);
    var temp2 = data.call("__sub__", temp);
    p!.set("data", temp2);
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
