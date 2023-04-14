import os, glob, json
import chpl_locale_model, chpl_llvm
import utils
from utils import error, memoize, run_command, which, report_unmet_runtime_req, is_ver_in_range

def _validate_cuda_version():
    return _validate_cuda_version_impl()

def _validate_rocm_version():
    return _validate_rocm_version_impl()

# Format:
#   SDK path environment variable
#   program to locate SDK folder
#   depth of program within SDK folder
#   Default GPU architecure for the vendor
#   LLVM target
#   Function to validate version
GPU_TYPES = {
    "cuda": ("CHPL_CUDA_PATH", "nvcc",  2,"sm_60",  "NVPTX",
             _validate_cuda_version),

    "rocm": ("CHPL_ROCM_PATH", "hipcc", 3,"gfx906", "AMDGPU",
            _validate_rocm_version)
}

def determineGpuType():
    typesFound = [gpuType for gpuType in GPU_TYPES.keys() if which(GPU_TYPES[gpuType][1])]
    if len(typesFound) == 1:
      return typesFound[0]

    error("Unable to determine GPU type%s, assign 'CHPL_GPU_CODEGEN' to one of: [%s]" %
      ("" if len(typesFound) == 0 else " (detected: [%s])" %  ", ".join(typesFound),
       ", ".join(GPU_TYPES.keys())))
    return None;

@memoize
def get():
    if chpl_locale_model.get() != 'gpu':
        return 'none'

    chpl_gpu_codegen = os.environ.get("CHPL_GPU_CODEGEN")
    if chpl_gpu_codegen:
        if chpl_gpu_codegen not in GPU_TYPES:
            error("Only {} supported for 'CHPL_GPU_CODEGEN'".format(
                GPU_TYPES.keys()))
        else:
            return chpl_gpu_codegen
    else:
        return determineGpuType();

@memoize
def get_arch():
    gpu_type = get()

    # No arch if GPU is not being used.
    if gpu_type == 'none':
        return ''

    # Check if user is overriding the arch.
    arch = os.environ.get("CHPL_GPU_ARCH")
    if arch:
        return arch

    # Return vendor-specific default architecture
    _, _, _, gpu_default_arch, _, _ = GPU_TYPES[gpu_type]
    return gpu_default_arch

@memoize
def get_sdk_path(for_gpu):
    gpu_type = get()

    # No SDK path if GPU is not being used.
    if gpu_type == 'none':
        return ''

    # Check vendor-specific environment variable for SDK path
    gpu_variable, gpu_program, gpu_bin_depth, _, _, _ = GPU_TYPES[for_gpu]
    chpl_sdk_path = os.environ.get(gpu_variable)
    if chpl_sdk_path:
        return chpl_sdk_path

    # try to find the SDK by running `which` on a vendor-specific program.
    exists, returncode, my_stdout, my_stderr = \
        utils.try_run_command(["which", gpu_program])

    if exists and returncode == 0:
        real_path = os.path.realpath(my_stdout.strip()).strip()
        chpl_sdk_path = "/".join(real_path.split("/")[:-gpu_bin_depth])
        return chpl_sdk_path
    elif gpu_type == for_gpu:
        report_unmet_runtime_req("Can't find {}".format(get()))
    else:
        return ''

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
        chpl_cuda_path = get_sdk_path('cuda')
        if chpl_cuda_path is None:
          # We will already print a warning if we're unable to find the sdk path
          return ""

        # there can be multiple libdevices for multiple compute architectures.
        # Not sure how realistic that is, nor I see multiple instances in the
        # systems I have access to. They are always named `libdevice.10.bc`,
        # but I just want to be sure here.
        libdevices = glob.glob(chpl_cuda_path+"/nvvm/libdevice/libdevice*.bc")
        if len(libdevices) == 0:
            report_unmet_runtime_req(
                "Can't find libdevice. Please make sure your $CHPL_CUDA_PATH "
                "is set such that CHPL_CUDA_PATH/nvmm/libdevice/libdevice*.bc "
                "exists.")
        else:
            return libdevices[0]

    return ""


def get_runtime():
    # For now, the runtime and codegen targets match one-for-one.
    chpl_gpu_runtime = os.environ.get("CHPL_GPU_RUNTIME")
    if chpl_gpu_runtime:
        valid_runtimes = list(GPU_TYPES.keys()) + ['none']
        if chpl_gpu_runtime not in valid_runtimes:
            error("Only {} supported for 'CHPL_GPU_RUNTIME'".format(
                valid_runtimes))
        else:
            return chpl_gpu_runtime
    return get()

def validateLlvmBuiltForTgt(expectedTgt):
    # If we're using the bundled LLVM, llvm-config may not have been built
    # before we call chplenv. It seems safe to assume the bundled LLVM has been
    # built with whatever requirements we have for Chapel so we just return
    # that it's been validated.
    if chpl_llvm.get() == 'bundled':
        return True

    exists, returncode, my_stdout, my_stderr = utils.try_run_command(
        [chpl_llvm.get_llvm_config(), "--targets-built"])

    if not exists or returncode != 0:
        return False

    targets = my_stdout.strip().split(" ")
    return expectedTgt in targets


def _validate_cuda_version_impl():
    """Check that the installed CUDA version is >= MIN_REQ_VERSION and <
       MAX_REQ_VERSION"""
    MIN_REQ_VERSION = "11"
    MAX_REQ_VERSION = "12"

    chpl_cuda_path = get_sdk_path('cuda')
    version_file = '%s/version.json' % chpl_cuda_path
    if not os.path.exists(version_file):
      report_unmet_runtime_req("Unable to find file %s." % version_file)
      return False

    f = open(version_file)
    version_json = json.load(f)
    f.close()
    cudaVersion = version_json["cuda"]["version"]

    if not is_ver_in_range(cudaVersion, MIN_REQ_VERSION, MAX_REQ_VERSION):
        report_unmet_runtime_req(
            "Chapel requires a CUDA version between %s and %s, "
            "detected version %s on system." %
            (MIN_REQ_VERSION, MAX_REQ_VERSION, cudaVersion))
        return False

    return True
    

def _validate_rocm_versionImpl():
    """Check that the installed CUDA version is >= MIN_REQ_VERSION and <
       MAX_REQ_VERSION"""
    MIN_REQ_VERSION = "4"
    MAX_REQ_VERSION = "6"

    chpl_rocm_path = get_sdk_path('rocm')
    version_file = '%s/.info/version-hiprt' % chpl_rocm_path
    if not os.path.exists(version_file):
      report_unmet_runtime_req("Unable to find file %s." % version_file)
      return False

    rocmVersion = open(version_file).read()

    if not is_ver_in_range(rocmVersion, MIN_REQ_VERSION, MAX_REQ_VERSION):
        report_unmet_runtime_req(
            "Chapel requires a HIP runtime version between %s and %s, "
            "detected version %s on system." %
            (MIN_REQ_VERSION, MAX_REQ_VERSION, rocmVersion))
        return False

    return True

    
@memoize
def validate(chplLocaleModel, chplComm):
    if chplLocaleModel != "gpu":
        return True

    # Run function to validate that we have a satisfactory version of our SDK
    # (e.g. cuda or rocm)
    GPU_TYPES[get()][5]()

    llvmTgt = GPU_TYPES[get()][4]
    if not validateLlvmBuiltForTgt(llvmTgt):
        report_unmet_runtime_req("LLVM not built for %s, " +
        "consider setting CHPL_LLVM to 'bundled'" % llvmTgt)

    return True
