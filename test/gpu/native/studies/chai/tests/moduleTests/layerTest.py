# Reference script to generate the correct answers for the module tests
# You will need to install pytorch to run
import torch
import torch.nn as nn
import sys
import os

sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '..', '..', 'ChAI', 'scripts')))
import chai


class Dummy(nn.Module):
    def __init__(self):
        super(Dummy, self).__init__()

        self.model = nn.Linear(100, 10)

    def forward(self, x):
        return self.model(x)


def test_dummy():
    print("Testing dummy model")
    model = Dummy()
    model.chai_dump('./savedTensors/dummyModel', 'dummyModel', with_json=False, verbose=False, dtype=torch.float64)
    print("Model dumped")
    x = torch.randn(100)
    x.chai_save('./savedTensors', 'dummyInput', with_json=False, verbose=False, dtype=torch.float64)
    print("Input saved")
    y = model(x)
    y.chai_save('./savedTensors', 'dummyAnswer')
    print("Answer saved")


def test_load():
    x = torch.arange(10, dtype=torch.float)
    # x = torch.randn(10)
    x.chai_save('./savedTensors', 'loadInput', with_json=True, verbose=True, dtype=torch.float64)
    print(x)

def test_linear():
    with torch.no_grad():
        print("Testing linear layer")
        model = nn.Linear(10, 5)
        x = torch.arange(10, dtype=torch.float)
        # x = torch.randn(10)
        print(x)
        x.chai_save('./savedTensors', 'linearInput', with_json=False, verbose=False, dtype=torch.float64)
        y = model(x)
        y.chai_save('./savedTensors', 'linearAnswer', with_json=False, verbose=False, dtype=torch.float64)
        print(y)

def test_conv2d():
    with torch.no_grad():

        print("Testing Conv2d layer")
        # Small problem size
        model = nn.Conv2d(3, 5, 3)
        x = torch.arange(27, dtype=torch.float)
        x = x.reshape(3, 3, 3)
        # x = torch.randn(3, 3, 3)
        x.chai_save('./savedTensors', 'conv2dInput', with_json=False, verbose=False, dtype=torch.float64)
        print(x)

        y = model(x)
        y.chai_save('./savedTensors', 'conv2dAnswer', with_json=False, verbose=False, dtype=torch.float64)
        print(y)


def test_maxpool2d():
    print("Testing MaxPool2d layer")
    # Small problem size
    model = nn.MaxPool2d(3)
    x = torch.arange(125, dtype=torch.float)
    x = x.reshape(5, 5, 5)
    # x = torch.randn(3, 3, 3)
    print(x)
    x.chai_save('./savedTensors', 'maxPool2dInput', with_json=False, verbose=False, dtype=torch.float64)
    y = model(x)
    print(y)
    y.chai_save('./savedTensors', 'maxPool2dAnswer', with_json=False, verbose=False, dtype=torch.float64)

def test_adaptiveavgpool2d():
    print("Testing AdaptiveAvgPool2d layer")
    # Small problem size
    model = nn.AdaptiveAvgPool2d(2)
    x = torch.arange(125, dtype=torch.float)
    x = x.reshape(5, 5, 5)
    # x = torch.randn(5, 5, 5)
    print(x)
    x.chai_save('./savedTensors', 'adaptiveAvgPool2dInput', with_json=False, verbose=False, dtype=torch.float64)
    y = model(x)
    print(y)
    y.chai_save('./savedTensors', 'adaptiveAvgPool2dAnswer', with_json=False, verbose=False, dtype=torch.float64)

def test_flatten():
    print("Testing Flatten layer")
    # Small problem size
    model = nn.Flatten(0) # 0 is the start_dim, by default it is 1, we should fix chai to match the default
    x = torch.arange(27, dtype=torch.float)
    x = x.reshape(3, 3, 3)
    # x = torch.randn(3, 3, 3)
    print(x)
    x.chai_save('./savedTensors', 'flattenInput', with_json=False, verbose=False, dtype=torch.float64)
    y = model(x)
    print(y)
    y.chai_save('./savedTensors', 'flattenAnswer', with_json=False, verbose=False, dtype=torch.float64)

def test_relu():
    print("Testing ReLU layer")
    # Small problem size
    model = nn.ReLU()
    x = torch.arange(27, dtype=torch.float)
    x = x.reshape(3, 3, 3)
    # x = torch.randn(3, 3, 3)
    print(x)
    x.chai_save('./savedTensors', 'reluInput', with_json=False, verbose=False, dtype=torch.float64)
    y = model(x)
    print(y)
    y.chai_save('./savedTensors', 'reluAnswer', with_json=False, verbose=False, dtype=torch.float64)

def test_softmax():
    print("Testing Softmax layer")
    # Small problem size
    model = nn.Softmax(dim=1)
    x = torch.arange(9, dtype=torch.float)
    x = x.reshape(3, 3)
    # x = torch.randn(3, 3)
    print(x)
    x.chai_save('./savedTensors', 'softmaxInput', with_json=False, verbose=False, dtype=torch.float64)
    y = model(x)
    print(y)
    y.chai_save('./savedTensors', 'softmaxAnswer', with_json=False, verbose=False, dtype=torch.float64)

def test_dropout():
    print("Testing Dropout layer")
    # Small problem size
    model = nn.Dropout(p=0.5)
    x = torch.arange(9, dtype=torch.float)
    x = x.reshape(3, 3)
    # x = torch.randn(3, 3)
    print(x)
    x.chai_save('./savedTensors', 'dropoutInput', with_json=False, verbose=False, dtype=torch.float64)
    y = model(x)
    print(y)
    y.chai_save('./savedTensors', 'dropoutAnswer', with_json=False, verbose=False, dtype=torch.float64)


def main():
    # torch.manual_seed(5)
    # test_dummy()
    # test_load()
    # test_linear()
    # test_conv2d()
    # test_maxpool2d()
    # test_adaptiveavgpool2d()
    # test_flatten()
    # test_relu()
    # test_softmax()
    test_dropout()

if __name__ == "__main__":
    main()

