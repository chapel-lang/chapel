import utils
import os
import glob
import json
import chpl_locale_model
import chpl_llvm
import chpl_compiler
import re
import chpl_tasks
from utils import error, warning, memoize, run_command, which, is_ver_in_range

def _validate_cuda_version():
    return _validate_cuda_version_impl()

def _validate_rocm_version():
    return _validate_rocm_version_impl()

class gpu_type:
    def __init__(self, sdk_path_env, compiler, default_arch, llvm_target,
                 runtime_impl, version_validator, llvm_validator):
        self.sdk_path_env = sdk_path_env
        self.compiler = compiler
        self.default_arch = default_arch
        self.llvm_target = llvm_target
        self.runtime_impl = runtime_impl
        self.version_validator = version_validator
        self.llvm_validator = llvm_validator

    def validate_sdk_version(self):
        return self.version_validator()

    def validate_llvm(self):
        return self.llvm_validator(self)

def _validate_cuda_llvm_version(gpu: gpu_type):
    return _validate_cuda_llvm_version_impl(gpu)

def _validate_rocm_llvm_version(gpu: gpu_type):
    return _validate_rocm_llvm_version_impl(gpu)


GPU_TYPES = {
    "nvidia": gpu_type(sdk_path_env="CHPL_CUDA_PATH",
                       compiler="nvcc",
                       default_arch="sm_60",
                       llvm_target="NVPTX",
                       runtime_impl="cuda",
                       version_validator=_validate_cuda_version,
                       llvm_validator=_validate_cuda_llvm_version),
    "amd": gpu_type(sdk_path_env="CHPL_ROCM_PATH",
                    compiler="hipcc",
                    default_arch="",
                    llvm_target="AMDGPU",
                    runtime_impl="rocm",
                    version_validator=_validate_rocm_version,
                    llvm_validator=_validate_rocm_llvm_version),
    "cpu": gpu_type(sdk_path_env="",
                    compiler="",
                    default_arch="",
                    llvm_target="",
                    runtime_impl="cpu",
                    version_validator=lambda: None,
                    llvm_validator=lambda: None),
}


def _reportMissingGpuReq(msg, allowExempt=True, suggestNone=True):
    if suggestNone:
        msg += " To avoid this issue, you can have GPU code run on the CPU "
        msg += "by setting 'CHPL_GPU=cpu'."

    if allowExempt and os.environ.get('CHPLENV_GPU_REQ_ERRS_AS_WARNINGS'):
        warning(msg)
        return

    if allowExempt:
        msg += " To turn this error into a warning set "
        msg += "CHPLENV_GPU_REQ_ERRS_AS_WARNINGS."
    error(msg)


def determine_gpu_type():
    typesFound = [val for (val, gpu_type) in GPU_TYPES.items() if which(gpu_type.compiler)]
    if len(typesFound) == 1:
      return typesFound[0]

    error("Unable to determine GPU type%s, assign 'CHPL_GPU' to one of: [%s]" %
      ("" if len(typesFound) == 0 else " (detected: [%s])" %  ", ".join(typesFound),
       ", ".join(GPU_TYPES.keys())))
    return None;

def get_llvm_override():
    if get() == 'amd':
        major_version = get_sdk_version().split('.')[0]
        if major_version == '5':
            return '{}/llvm/bin/llvm-config'.format(get_sdk_path('amd'))
        pass
    return 'none'

@memoize
def get():
    if chpl_locale_model.get() != 'gpu':
        return 'none'

    chpl_gpu_env = os.environ.get("CHPL_GPU")
    if chpl_gpu_env:
        if chpl_gpu_env not in GPU_TYPES:
            error("Only {} supported for 'CHPL_GPU'".format(list(GPU_TYPES.keys())))
        else:
            return chpl_gpu_env
    else:
        return determine_gpu_type();

@memoize
def get_arch():
    gpu_type = get()

    # No arch if GPU is not being used.
    if gpu_type == 'none' or gpu_type == 'cpu':
        return 'none'

    # Check if user is overriding the arch.
    arch = os.environ.get("CHPL_GPU_ARCH")
    if arch:
        # arch might be specified in arch1,arch2 format, which is only supported
        # on nvidia.
        if len(arch.split(",")) > 1 and gpu_type != "nvidia":
            error("Multi-target builds are only supported for the 'nvidia' GPU type.")
            arch = 'error'

        return arch

    # Return vendor-specific default architecture
    arch = GPU_TYPES[gpu_type].default_arch
    if arch != "":
        return arch
    else:
        error("CHPL_GPU={} requires also setting CHPL_GPU_ARCH. "
              "Please check the GPU programming technote "
              "<https://chapel-lang.org/docs/technotes/gpu.html> "
              "for more information.".format(gpu_type))
        return 'error'

@memoize
def get_sdk_path(for_gpu):
    gpu_type = get()

    # No SDK path if GPU is not being used.
    if gpu_type == 'cpu':
        return 'none'

    # Check vendor-specific environment variable for SDK path
    gpu = GPU_TYPES[for_gpu]
    chpl_sdk_path = os.environ.get(gpu.sdk_path_env)
    if chpl_sdk_path:
        return chpl_sdk_path

    # try to find the SDK by running `which` on a vendor-specific program.
    exists, returncode, my_stdout, my_stderr = utils.try_run_command(["which",
                                                                      gpu.compiler])

    if exists and returncode == 0:
        # Walk up from directories from the one containing the gpu compiler
        # (e.g.  `nvcc` or `hipcc`) until we find a directory that starts with
        # `runtime_impl` (e.g `cuda` or `rocm`)
        # TODO: this logic does not seem to work for spack
        real_path = os.path.realpath(my_stdout.strip()).strip()
        path_parts = real_path.split("/")
        chpl_sdk_path = "/"
        for part in path_parts:
            if len(part) == 0: continue
            chpl_sdk_path += part
            if not part.startswith(gpu.runtime_impl):
                chpl_sdk_path += "/"
            else:
                break
        
        # validate the SDK path found
        if not (os.path.exists(chpl_sdk_path) and os.path.isdir(chpl_sdk_path)):
            _reportMissingGpuReq(
                "Can't infer {} toolkit from '{}'. Try setting {}."
                .format(get(), real_path, gpu.sdk_path_env)
            )
            return 'error'

        return chpl_sdk_path
    elif gpu_type == for_gpu:
        _reportMissingGpuReq("Can't find {} toolkit.".format(get()))
        return 'error'
    else:
        return ''

def get_gpu_mem_strategy():
    memtype = os.environ.get("CHPL_GPU_MEM_STRATEGY")
    if memtype:
        valid_options = ["array_on_device", "unified_memory"]
        if memtype not in valid_options:
            error("CHPL_GPU_MEM_STRATEGY must be set to one of: %s" %
                 ", ".join(valid_options))
        return memtype
    return "array_on_device"


def get_cuda_libdevice_path():
    if get() == 'nvidia':
        # TODO this only makes sense when we are generating for nvidia
        chpl_cuda_path = get_sdk_path('nvidia')

        # there can be multiple libdevices for multiple compute architectures. Not
        # sure how realistic that is, nor I see multiple instances in the systems I
        # have access to. They are always named `libdevice.10.bc`, but I just want
        # to be sure here.
        path_part = "/nvvm/libdevice/libdevice*.bc"
        libdevices = glob.glob(chpl_cuda_path+path_part)
        if len(libdevices) == 0:
            _reportMissingGpuReq("Can't find libdevice. Please make sure your CHPL_CUDA_PATH is "
                  "set such that CHPL_CUDA_PATH{} exists.".format(path_part))
            return 'error'
        else:
            return libdevices[0]

    return "none"

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


def _validate_cuda_llvm_version_impl(gpu: gpu_type):
    if not validateLlvmBuiltForTgt(gpu.llvm_target):
        _reportMissingGpuReq(
            "LLVM not built for %s, consider setting CHPL_LLVM to 'bundled'." %
            gpu.llvm_target, allowExempt=False
        )

def _validate_rocm_llvm_version_impl(gpu: gpu_type):
    major_version = get_sdk_version().split('.')[0]

    if major_version in ('4', '5') and chpl_llvm.get() == 'bundled':
        error("Cannot target AMD GPUs with CHPL_LLVM=bundled")
    elif major_version == '6' and chpl_llvm.get() != 'bundled':
        # once https://github.com/llvm/llvm-project/issues/65188 is resolved,
        # ths branch check can removed
        # assert(int(chpl_llvm.get_llvm_version()) >= 18)
        error("Cannot target AMD GPUs with ROCm 6.x without CHPL_LLVM=bundled")
    elif (
        major_version == "6"
        and chpl_llvm.get() == "system"
        and int(chpl_llvm.get_llvm_version()) < 18
    ):
        # this branch will never be hit, because we only allow
        # CHPL_LLVM=bundled which is 18+
        # but this will be the correct check once the above issue is resolved
        error("Cannot target AMD GPUs with ROCm 6.x without LLVM 18+")
    elif not validateLlvmBuiltForTgt(gpu.llvm_target):
        _reportMissingGpuReq(
            "LLVM not built for %s." % gpu.llvm_target, allowExempt=False
        )

def _validate_cuda_version_impl():
    """Check that the installed CUDA version is >= MIN_REQ_VERSION and <
       MAX_REQ_VERSION"""
    MIN_REQ_VERSION = "7"
    MAX_REQ_VERSION = "13"

    cuda_version = get_sdk_version()

    if cuda_version is None:
        _reportMissingGpuReq("Unable to determine CUDA version.")
        return False

    if not is_ver_in_range(cuda_version, MIN_REQ_VERSION, MAX_REQ_VERSION):
      _reportMissingGpuReq(
            "Chapel requires a CUDA version between %s and %s, "
            "detected version %s on system." %
            (MIN_REQ_VERSION, MAX_REQ_VERSION, cuda_version))
      return False

    # CUDA 12 requires the bundled LLVM or the major LLVM version must be >15
    if is_ver_in_range(cuda_version, "12", "13"):
        llvm = chpl_llvm.get()
        if llvm == "system":
            llvm_config = chpl_llvm.find_system_llvm_config()
            llvm_ver_str = chpl_llvm.get_llvm_config_version(llvm_config).strip()
            if is_ver_in_range(llvm_ver_str, "0", "16"):
                _reportMissingGpuReq(
                        "LLVM versions before 16 do not support CUDA 12. "
                        "Your LLVM (CHPL_LLVM=system) version is {}. "
                        "You can use CUDA 11, or set CHPL_LLVM=bundled to use "
                        "CUDA 12.".format(llvm_ver_str), suggestNone=False,
                        allowExempt=False)
                return False

    return True

def get_sdk_version():
    if get() == 'amd':
        chpl_rocm_path = get_sdk_path('amd')
        files_to_try = ['%s/.info/version-hiprt' % chpl_rocm_path,
            '%s/.info/version-libs' % chpl_rocm_path]

        version_filename = None
        for fname in files_to_try:
           if os.path.exists(fname):
               version_filename = fname
               break

        rocm_version = None
        if version_filename is not None:
            rocm_version = open(version_filename).read()
        else:
            exists, returncode, my_stdout, my_stderr = utils.try_run_command(
                ["hipcc", "--version"])
            if exists and returncode == 0:
                match = re.search(r"rocm?-([\d\.]+)", my_stdout)
                if match:
                    rocm_version = match.group(1)
                else:
                    match = re.search(r"llvm-amdgpu-([\d\.]+)", my_stdout)
                    if match:
                        rocm_version = match.group(1)
        return rocm_version

    if get() == 'nvidia':
        chpl_cuda_path = get_sdk_path('nvidia')
        version_file_json = '%s/version.json' % chpl_cuda_path
        version_file_txt = '%s/version.txt' % chpl_cuda_path
        cuda_version = None
        if os.path.exists(version_file_json):
            f = open(version_file_json)
            version_json = json.load(f)
            f.close()
            cuda_version = version_json["cuda"]["version"]
        elif os.path.exists(version_file_txt):
            txt = open(version_file_txt).read()
            match = re.search(r'\d+\.\d+\.\d+', txt)
            if match:
                cuda_version = match.group()
        if cuda_version is None:
            exists, returncode, my_stdout, my_stderr = utils.try_run_command(
                ["nvcc", "--version"])
            if exists and returncode == 0:
                pattern = r"Cuda compilation tools, release ([\d\.]+)"
                match = re.search(pattern, my_stdout)
                if match:
                    cuda_version = match.group(1)
        return cuda_version


def _validate_rocm_version_impl():
    """Check that the installed ROCM version is >= MIN_REQ_VERSION and <
       MAX_REQ_VERSION"""
    MIN_REQ_VERSION = "4"
    MAX_REQ_VERSION = "5.5"
    MIN_ROCM6_REQ_VERSION = "6"
    MAX_ROCM6_REQ_VERSION = "6.2"

    rocm_version = get_sdk_version()

    if rocm_version is None:
        _reportMissingGpuReq("Unable to determine ROCm version.")
        return False

    if not is_ver_in_range(rocm_version, MIN_REQ_VERSION, MAX_REQ_VERSION) and not is_ver_in_range(rocm_version, MIN_ROCM6_REQ_VERSION, MAX_ROCM6_REQ_VERSION):
        _reportMissingGpuReq(
            "Chapel requires ROCm to be a version between %s and %s or between %s and %s, "
            "detected version %s on system." %
            (MIN_REQ_VERSION, MAX_REQ_VERSION, MIN_ROCM6_REQ_VERSION, MAX_ROCM6_REQ_VERSION, rocm_version))
        return False

    return True

@memoize
def validate(chplLocaleModel):
    if chplLocaleModel != "gpu":
        return True

    gpu = GPU_TYPES[get()]

    # Run function to validate that we have a satisfactory version of our SDK
    # (e.g. CUDA or ROCm)
    gpu.validate_sdk_version()

    if chpl_tasks.get() == 'fifo':
        error("The 'fifo' tasking model is not supported with GPU support")

    if get() == 'cpu':
        return True

    if chpl_compiler.get('target') != 'llvm':
        error("The 'gpu' locale model can only be used with "
              "CHPL_TARGET_COMPILER=llvm.")

    gpu.validate_llvm()

    for depr_env in ("CHPL_GPU_CODEGEN", "CHPL_GPU_RUNTIME"):
        if os.environ.get(depr_env):
            warning(depr_env + " is deprecated and now ignored. Please use " +
                    "'CHPL_GPU=[nvidia|amd|cpu]' to choose a GPU target " +
                    "explicitly.")

    return True
