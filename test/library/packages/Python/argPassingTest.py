
def no_args():
    print("called no_args")

def one_arg(a):
    print("called one_arg with {}".format(a))

def two_args(a, b):
    print("called two_args with {} and {}".format(a, b))

def three_args(a, b, c):
    print("called three_args with {}, {}, and {}".format(a, b, c))

def varargs(*args):
    print("called varargs")
    print("  args: " + ", ".join([str(a) for a in args]))

def one_arg_with_default(a=1):
    print("called one_arg_with_default with {}".format(a))

def three_args_with_default(a, b=2, c=3):
    print("called three_args_with_default with {}, {}, and {}".format(a, b, c))

def three_args_with_default_and_kwargs(a, b=2, c=3, **kwargs):
    print("called three_args_with_default_and_kwargs with {}, {}, and {}".format(a, b, c))
    print("  kwargs: " + ", ".join(["{}={}".format(k, v) for k, v in kwargs.items()]))

def varargs_and_kwargs(*args, **kwargs):
    print("called varargs_and_kwargs")
    print("  args: " + ", ".join([str(a) for a in args]))
    print("  kwargs: " + ", ".join(["{}={}".format(k, v) for k, v in kwargs.items()]))
