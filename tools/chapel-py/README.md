# `chapel-py`

Python bindings for Chapel's frontend library, Dyno.

The purpose of this library is to allow programmers to clearly create tools
for the Chapel programming languages that would be more difficult to write
using C++ (the native language in which the Chapel frontend library is written).

For example, here's a toy program that prints the names of all things declared
anywhere in `myfile.chpl`.

```Python
from chapel import *
from chapel.core import *

ctx = Context()
ast = ctx.parse("myfile.chpl")

all_names = set()
for node in postorder(ast):
    if isinstance(node, NamedDecl):
        all_names.add(node.name())

for name in all_names:
    print(name)
```

If `myfile.chpl` contains the following:

```Chapel
module myfile {
    var x = 1+1;
    var y = x+1;

    record R {

    }
}
```

Then the program will print out something like the following:

```
R
y
x
myfile
```

## Installation
Make sure that you have a from-source build of Chapel available in your
`CHPL_HOME`, and that the Dyno compiler library has been built (this usually
happens if you build the compiler, or run `make test-frontend`). Currently,
the build script also requires having LLVM available in your path. With
those constraints met, you can just run `pip install`:

```Bash
python3 -m pip install -e .
```

## Usage
Check the `chplcheck.py` file in the root directory to see the library
in action.

At compile-time, `chpl-py` generates a class hierarhcy of Python objects,
with base class `AstNode`. A class is created for each of the AST nodes understood
by Dyno.

More documentation to come...
