#!/usr/bin/env python3

import sys

def verTup(v):
  return tuple(map(int, (v.split("."))))

if not sys.version_info >= (3, 7):
  print("chpl_plot requires Python 3.7 or above")
  print("Add a compatible version to your PATH")
  print("Alternatively, set the CHPL_EXP_SKIP_PYTHON_DEP_CHECK environment variable")
  print("to some value and accept that if chpl_plot is used it will error out.")
  sys.exit(1)

def is_module_available(module_name):
  # should work in python 3.4 and above
  import importlib.util
  loader = importlib.util.find_spec(module_name)
  return loader != None

def is_module_available_at_version(module_name, version):
  import importlib
  mod = importlib.import_module(module_name)

  if verTup(mod.__version__) < verTup(version):
    return mod.__version__

  return True


checkModules = [('numpy', '0'), ('matplotlib', "3.5.4")]
missingModules = []

for (mod, ver) in checkModules:
  if not is_module_available(mod):
    missingModules.append((mod, ver, "not installed"))
    continue
  avail = is_module_available_at_version(mod, ver)
  if avail != True:
    missingModules.append((mod, ver, avail))

if len(missingModules) > 0:
  print("The following module(s) are required by chpl_plot, but are not installed: ")
  print("")
  for (mod, expectVer, haveVer) in missingModules:
    print("  %s at version %s or above, you have: %s" % (mod, expectVer, haveVer))
  print("")
  print("You can install Python modules using 'pip'. Because machines may have multiple ")
  print("versions of pip and python installed I suggest invoking it as a module like:")
  print("'python3 -m pip'. If you want to install modules under your user account you")
  print("can pass the --user flag, for example:")
  print("")
  print("  python3 -m pip install " + " ".join(map(lambda x: x[0], missingModules)) + " --user")
  print("")
  print("If pip itself is not installed (and you don't have root access), you can install")
  print("under your user account by running the following (see https://pypi.org/project/pip/")
  print("for more info)")
  print("")
  print("  wget https://bootstrap.pypa.io/get-pip.py && python3 get-pip.py --user")
  print("  cd .local/bin")
  print("  # and then run your 'python3 -m pip --user' command from here")
  print("")
  print("")
  print("Alternatively, set the CHPL_EXP_SKIP_PYTHON_DEP_CHECK environment variable")
  print("to some value and accept that if chpl_plot is used it will error out.")
  print("You may want to copy any generated .dat files to another machine to process")
  print("them.")
  sys.exit(1)

sys.exit(0)
