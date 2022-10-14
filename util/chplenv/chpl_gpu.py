import utils
import os
import glob
import chpl_locale_model
import chpl_llvm
from utils import error, memoize, run_command

# Format:
#   environment variable
#   program to locate SDK folder
#   depth of program within SDK folder
GPU_TYPES = {
    "cuda": ("CHPL_CUDA_PATH", "nvcc", 2),
    "amd": ("CHPL_ROCM_PATH", "hipcc", 3)
}

def get():
    if chpl_locale_model.get() != 'gpu':
        return 'none'

    chpl_gpu_codegen = os.environ.get("CHPL_GPU_CODEGEN")
    if chpl_gpu_codegen:
        if chpl_gpu_codegen not in GPU_TYPES:
            error("Only {} supported for 'CHPL_GPU_CODEGEN'".format(GPU_TYPES.keys()))
        else:
            return chpl_gpu_codegen
    else:
        return 'cuda'

@memoize
def get_sdk_path():
    gpu_type = get()

    # No SDK path if GPU is not being used.
    if gpu_type == 'none':
        return ''

    # Check vendor-specific environment variable for SDK path, or
    # try to find the SDK by running `which` on a vendor-specific program.
    gpu_variable, gpu_program, gpu_bin_depth = GPU_TYPES[gpu_type]
    chpl_sdk_path = os.environ.get(gpu_variable)
    if chpl_sdk_path:
        return chpl_sdk_path

    exists, returncode, my_stdout, my_stderr = utils.try_run_command(["which",
                                                                      gpu_program])

    if exists and returncode == 0:
        real_path = os.path.realpath(my_stdout.strip()).strip()
        chpl_sdk_path = "/".join(real_path.split("/")[:-gpu_bin_depth])
        return chpl_sdk_path
    else:
        error("Can't find {}".format(get()))

def get_gpu_mem_strategy():
    memtype = os.environ.get("CHPL_GPU_MEM_STRATEGY")
    if memtype:
        valid_options = ["array_on_device", "unified_memory"]
        if memtype not in valid_options:
            error("CHPL_GPU_MEM_STRATEGY must be set to one of: %s" %
                 ", ".join(valid_options));
        return memtype
    return "unified_memory"


def get_cuda_libdevice_path():
    if get() == 'cuda':
        # TODO this only makes sense when we are generating for nvidia
        chpl_cuda_path = get_sdk_path()

        # there can be multiple libdevices for multiple compute architectures. Not
        # sure how realistic that is, nor I see multiple instances in the systems I
        # have access to. They are always named `libdevice.10.bc`, but I just want
        # to be sure here.
        libdevices = glob.glob(chpl_cuda_path+"/nvvm/libdevice/libdevice*.bc")
        if len(libdevices) == 0:
            error("Can't find libdevice. Please make sure your CHPL_CUDA_PATH is "
                  "set such that CHPL_CUDA_PATH/nvmm/libdevice/libdevice*.bc exists.")
        else:
            return libdevices[0]

    return ""


def get_runtime():
    chpl_gpu_runtime = os.environ.get("CHPL_GPU_RUNTIME")
    if chpl_gpu_runtime:
        if chpl_gpu_runtime != "cuda" and chpl_gpu_runtime != "amd":
            error("Only 'cuda' or 'amd' supported for 'CHPL_GPU_RUNTIME'")
        else:
            return chpl_gpu_runtime
    return "cuda"

def validate(chplLocaleModel, chplComm):
    pass
