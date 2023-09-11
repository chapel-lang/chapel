#!/usr/bin/env python

import sys

# Code for this function copied from:
# <https://stackoverflow.com/questions/14050281/how-to-check-if-a-python-module-exists-without-importing-it>
def is_module_available(module_name):
    if sys.version_info < (3, 0):
        # python 2
        import importlib
        loader = importlib.find_loader(module_name)
    elif sys.version_info <= (3, 3):
        # python 3.0 to 3.3
        import pkgutil
        loader = pkgutil.find_loader(module_name)
    elif sys.version_info >= (3, 4):
        # python 3.4 and above
        import importlib
        loader = importlib.util.find_spec(module_name)

    return loader is not None


checkModules = ['numpy', 'matplotlib']
missingModules = list(filter(lambda mod: not is_module_available(mod), checkModules))

if len(missingModules) > 0:
  print("The following module(s) are required by chpl_plot, but are not installed: ")
  print("")
  print("  ", missingModules)
  print("")
  print("You can install Python modules using 'pip', if you want to install these")
  print("under your user account you can pass the --user flag, for example:")
  print("")
  print("  pip install " + " ".join(missingModules) + " --user")
  print("")
  print("If pip itself is not installed (and you don't have root access), you can install")
  print("under your user account by running the following (see https://pypi.org/project/pip/")
  print("for more info)")
  print("")
  print("  wget https://bootstrap.pypa.io/get-pip.py && python get-pip.py --user")
  print("  cd .local/bin")
  print("  # and then run your 'pip --user' command from here")
  print("")
  print("")
  print("Alternativey, set the CHPL_EXP_SKIP_PYTHON_DEP_CHECK environment variable")
  print("to some value and accept that if chpl_plot is used it will error out.")
  print("You may want to copy any generated .dat files to another machine to process")
  print("them.")
  sys.exit(1)

sys.exit(0)
