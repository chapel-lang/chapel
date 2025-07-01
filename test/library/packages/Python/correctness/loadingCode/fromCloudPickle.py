import cloudpickle

class MyClass:
    def __init__(self, value):
        self.value = value

    def print(self):
        print(self.value)

def my_function():
    return MyClass(42)


with open('my_function.pkl', 'wb') as f:
    cloudpickle.dump(my_function, f)

with open('MyClass.pkl', 'wb') as f:
    cloudpickle.dump(MyClass, f)
