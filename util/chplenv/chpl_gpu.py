import utils
import os
import glob
import chpl_locale_model
import chpl_llvm
from utils import error, memoize, run_command

def get():
    if chpl_locale_model.get() == 'gpu':
        return 'cuda'
    else:
        return 'none'

@memoize
def get_cuda_path():
    if chpl_locale_model.get() == 'gpu':
        chpl_cuda_path = os.environ.get("CHPL_CUDA_PATH")
        if chpl_cuda_path:
            return chpl_cuda_path

        exists, returncode, my_stdout, my_stderr = utils.try_run_command(["which",
                                                                          "nvcc"])

        if exists and returncode == 0:
            chpl_cuda_path = "/".join(os.path.realpath(my_stdout).strip().split("/")[:-2])
            return chpl_cuda_path
        else:
            error("Can't find cuda")

    return ""

def get_cuda_libdevice_path():
    if chpl_locale_model.get() == 'gpu':
        # TODO this only makes sense when we are generating for nvidia
        chpl_cuda_path = get_cuda_path()

        # there can be multiple libdevices for multiple compute architectures. Not
        # sure how realistic that is, nor I see multiple instances in the systems I
        # have access to. They are always named `libdevice.10.bc`, but I just want
        # to be sure here.
        libdevices = glob.glob(chpl_cuda_path+"/nvvm/libdevice/libdevice*.bc")
        if len(libdevices) == 0:
            error("Can't find libdevice. Please make sure your CHPL_CUDA_PATH is "
                  "set such that CHPL_CUDA_PATH/bin/nvcc exists.")
        else:
            return libdevices[0]

    return ""


def get_runtime():
    chpl_gpu_runtime = os.environ.get("CHPL_GPU_RUNTIME")
    if chpl_gpu_runtime:
        if chpl_gpu_runtime != "cuda":
            error("Only 'cuda' supported for 'CHPL_GPU_RUNTIME'")
        else:
            return chpl_gpu_runtime
    return "cuda"

def validate(chplLocaleModel, chplComm):
    pass
