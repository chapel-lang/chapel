import torch

class MyModel(torch.nn.Module):
    def __init__(self):
        super(MyModel, self).__init__()
        self.linear = torch.nn.Linear(1, 1)

    def forward(self, x):
        return self.linear(x)

def main():
    # create the model
    model = MyModel()
    input_tensor = torch.tensor([[1.0]], requires_grad=False)
    print("Input tensor:", input_tensor)

    # init model weights to 0.9
    for p in model.parameters():
        p.data.fill_(0.9)

    # run model
    pred = model(input_tensor)

    # compute the loss
    loss_fn = torch.nn.MSELoss()
    target = torch.tensor([[2.0]])
    print("Target:", target)
    loss = loss_fn(pred, target)
    loss.backward()

    # update the model's parameters
    learning_rate = 0.01
    # we don't have context managers threaded through the code yet
    # so we need to manually zero the gradients
    for p in model.parameters():
        p.data -= learning_rate * p.grad # workaround, .data
    # ideal
    # with torch.no_grad():
    #     for p in model.parameters():
    #         p -= learning_rate * p.grad

    print(f"Prediction: {pred.item():.3g}")
    print(f"Loss: {loss.item():.3g}")
    print("Weights:")
    for p in model.parameters():
        print(f"  {p.item():.3g}")

if __name__ == '__main__':
    main()
