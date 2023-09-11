#!/usr/bin/env python3

import sys

# Copied from <https://stackoverflow.com/questions/14050281/how-to-check-if-a-python-module-exists-without-importing-it>
def is_module_available(module_name):
    if sys.version_info < (3, 0):
        # python 2
        import importlib
        torch_loader = importlib.find_loader(module_name)
    elif sys.version_info <= (3, 3):
        # python 3.0 to 3.3
        import pkgutil
        torch_loader = pkgutil.find_loader(module_name)
    elif sys.version_info >= (3, 4):
        # python 3.4 and above
        import importlib
        torch_loader = importlib.util.find_spec(module_name)

    return torch_loader is not None

print(is_module_available('sys'))
print(is_module_available('numpy'))


