import utils
import os
import glob
import chpl_locale_model
from utils import error, memoize

def get():
    if chpl_locale_model.get() == 'gpu':
        return 'cuda'
    else:
        return 'none'

@memoize
def get_cuda_path():
    chpl_cuda_path = os.environ.get("CHPL_CUDA_PATH")
    if chpl_cuda_path:
        return chpl_cuda_path

    exists, returncode, my_stdout, my_stderr = utils.try_run_command(["which",
                                                                      "nvcc"])

    if exists and returncode == 0:
        chpl_cuda_path = "/".join(my_stdout.strip().split("/")[:-2])
        return chpl_cuda_path
    else:
        return ""

def get_cuda_libdevice_path():
    chpl_cuda_path = get_cuda_path()

    # there can be multiple libdevices for multiple compute architectures. Not
    # sure how realistic that is, nor I see multiple instances in the systems I
    # have access to. They are always named `libdevice.10.bc`, but I just want
    # to be sure here.
    libdevices = glob.glob(chpl_cuda_path+"/nvvm/libdevice/libdevice*.bc")
    if len(libdevices) == 0:
        return ""
    else:
        return libdevices[0]

def get_runtime():
    return "cuda"

def validate(chplLocaleModel, chplComm):
    pass
