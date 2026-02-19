import os
import glob
import chpl_locale_model
import chpl_platform
import chpl_llvm
import chpl_compiler
import re
import chpl_tasks
import chpl_home_utils
import overrides
from utils import error, warning, memoize, try_run_command, which, is_ver_in_range, check_valid_var

def _validate_cuda_version():
    return _validate_cuda_version_impl()

def _validate_rocm_version():
    return _validate_rocm_version_impl()

class gpu_type:
    def __init__(self, sdk_path_env,
                        compiler,
                        default_arch,
                        llvm_target,
                        runtime_impl,
                        find_sdk_path,
                        find_version,
                        version_validator,
                        llvm_validator,
                        real_gpu):
        self.sdk_path_env = sdk_path_env
        self.compiler = compiler
        self.default_arch = default_arch
        self.llvm_target = llvm_target
        self.runtime_impl = runtime_impl
        self.find_sdk_path = find_sdk_path
        self.find_version = find_version
        self.version_validator = version_validator
        self.llvm_validator = llvm_validator
        self.real_gpu = real_gpu

    def validate_sdk_version(self):
        return self.version_validator()

    def validate_llvm(self):
        return self.llvm_validator(self)

def _validate_cuda_llvm_version(gpu: gpu_type):
    return _validate_cuda_llvm_version_impl(gpu)

def _validate_rocm_llvm_version(gpu: gpu_type):
    return _validate_rocm_llvm_version_impl(gpu)

@memoize
def gpu_compiler_basic_compile(compiler: str, lang: str):
    dummy_main = "int main() { return 0; }"
    _, _, stdout, _ = try_run_command([compiler, "-v", "-c", "-x", lang, "-", "-o", "/dev/null"], cmd_input=dummy_main, combine_output=True)
    # NOTE: this code does not check if the code compiled properly,
    # all we care about is the output
    return stdout

def _find_cuda_sdk_path(compiler: str):
    out = gpu_compiler_basic_compile(compiler, "cu")
    if not out:
        return None
    regex = r"^#\$ TOP=(.+)$"
    match = re.search(regex, out, re.MULTILINE)
    return match.group(1) if match else None

def find_cuda_libdevice_path(compiler: str):
    out = gpu_compiler_basic_compile(compiler, "cu")
    if not out:
        return None
    regex = r"^#\$ NVVMIR_LIBRARY_DIR=(.+)$"
    match = re.search(regex, out, re.MULTILINE)
    if not match:
        return None
    libdevice_path = match.group(1)
    # there can be multiple libdevices for multiple compute architectures. Not
    # sure how realistic that is, nor I see multiple instances in the systems I
    # have access to. They are always named `libdevice.10.bc`, but I just want
    # to be sure here.
    libdevices = glob.glob(os.path.join(libdevice_path, "libdevice.*.bc"))
    return libdevices[0] if len(libdevices) > 0 else None


def find_llvm_amd_bin_path(compiler: str):
    out = gpu_compiler_basic_compile(compiler, "hip")
    if not out:
        return None
    regex = r"^InstalledDir: (.+)$"
    match = re.search(regex, out, re.MULTILINE)
    return match.group(1) if match else None

def find_amdgcn_path(compiler: str):
    out = gpu_compiler_basic_compile(compiler, "hip")
    if not out:
        return None
    # find the builtin bitcode path
    # this will likely appear many times, we just take the first occurrence
    regex = r"-mlink-builtin-bitcode\s*(/.+?amdgcn/bitcode)"
    match = re.search(regex, out)
    if not match:
        return None
    full_path = match.group(1)
    # strip the 'bitcode' part (and trailing '/')
    return os.path.dirname(full_path)

def _find_hip_sdk_path(compiler: str):
    out = gpu_compiler_basic_compile(compiler, "hip")
    if not out:
        return None
    regex = r"^Found HIP installation: (.+),"
    match = re.search(regex, out, re.MULTILINE)
    return match.group(1) if match else None

def _find_cuda_version(compiler: str):
    # we can run 'nvcc --version'
    # 'Cuda compilation tools, release'
    regex = r"Cuda compilation tools, release ([\d\.]+)"

    exists, returncode, out, _ = try_run_command([compiler, "--version"])
    if not (exists and returncode == 0):
        return None
    
    match = re.search(regex, out)
    return match.group(1) if match else None

def _find_rocm_version(compiler: str):
    # hip/amd has less uniform version info, we have to guess
    # use one of the following regexes in the compiler output in this order
    sep = os.path.sep
    regexes = [
        r"\broc-([\d\.]+)\b",
        sep+r"rocm-([\d\.]+)"+sep,
        sep+r"hip-([\d\.]+)",
        sep+r"llvm-amdgpu-([\d\.]+)",
        sep+r"rocm"+sep+r"([\d\.]+)"+sep,
    ]

    out = gpu_compiler_basic_compile(compiler, "hip")
    if not out:
        return None

    for regex in regexes:
        match = re.search(regex, out)
        if match:
            return match.group(1)
    return None

GPU_TYPES = {
    "nvidia": gpu_type(sdk_path_env="CHPL_CUDA_PATH",
                       compiler="nvcc",
                       default_arch="sm_60",
                       llvm_target="NVPTX",
                       runtime_impl="cuda",
                       find_sdk_path=_find_cuda_sdk_path,
                       find_version=_find_cuda_version,
                       version_validator=_validate_cuda_version,
                       llvm_validator=_validate_cuda_llvm_version,
                       real_gpu=True),
    "amd": gpu_type(sdk_path_env="CHPL_ROCM_PATH",
                    compiler="hipcc",
                    default_arch="",
                    llvm_target="AMDGPU",
                    runtime_impl="rocm",
                    find_sdk_path=_find_hip_sdk_path,
                    find_version=_find_rocm_version,
                    version_validator=_validate_rocm_version,
                    llvm_validator=_validate_rocm_llvm_version,
                    real_gpu=True),
    "cpu": gpu_type(sdk_path_env="",
                    compiler="",
                    default_arch="",
                    llvm_target="",
                    runtime_impl="cpu",
                    find_sdk_path=lambda compiler: None,
                    find_version=lambda compiler: None,
                    version_validator=lambda: None,
                    llvm_validator=lambda: None,
                    real_gpu=False),
    "none": gpu_type(sdk_path_env="",
                     compiler="",
                     default_arch="",
                     llvm_target="",
                     runtime_impl="",
                     find_sdk_path=lambda compiler: None,
                     find_version=lambda compiler: None,
                     version_validator=lambda: None,
                     llvm_validator=lambda: None,
                     real_gpu=False)
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

    detected_str = (
        " (detected: [{}])".format(", ".join(typesFound)) if typesFound else ""
    )
    options_str = ", ".join(t for t in GPU_TYPES.keys() if t != "none")
    error(
        "Unable to determine GPU type{}, assign 'CHPL_GPU' to one of: [{}]".format(
            detected_str, options_str
        )
    )
    return None

@memoize
def get_llvm_override():
    if get() == 'amd':
        major_version = get_sdk_version().split('.')[0]
        if major_version == '5':
            llvm_path = find_llvm_amd_bin_path(get_gpu_compiler())
            return '{}/llvm-config'.format(llvm_path)
        pass
    return 'none'

@memoize
def get():
    if chpl_locale_model.get() != 'gpu':
        return 'none'

    chpl_gpu_env = overrides.get("CHPL_GPU")
    if chpl_gpu_env:
        if chpl_gpu_env not in GPU_TYPES:
            check_valid_var("CHPL_GPU", chpl_gpu_env, list(GPU_TYPES.keys()))
        else:
            return chpl_gpu_env
    else:
        return determine_gpu_type()

@memoize
def get_arch():
    gpu_type = get()

    # No arch if GPU is not being used.
    if gpu_type == 'none' or gpu_type == 'cpu':
        return 'none'

    # Check if user is overriding the arch.
    arch = overrides.get("CHPL_GPU_ARCH")
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
def get_gpu_compiler():
    gpu_type = get()
    sdk_path = get_sdk_path(gpu_type)
    if sdk_path == 'none':
        return 'none'
    
    name = GPU_TYPES[gpu_type].compiler
    bin_dir = os.path.join(sdk_path, 'bin')
    full_path = os.path.join(bin_dir, name)
    if not os.path.exists(full_path):
        _reportMissingGpuReq("Can't find {} in '{}'".format(name, bin_dir))
    return full_path


@memoize
def get_sdk_path(for_gpu):
    # No SDK path if GPU is not being used.
    if not GPU_TYPES[get()].real_gpu:
        return 'none'
    gpu = GPU_TYPES[for_gpu]

    def validate_path(p):
        # TODO: for now, just do a simple validation that checks if the path exists
        return (os.path.exists(p) and os.path.isdir(p))

    # use user specified if given
    chpl_sdk_path = overrides.get(gpu.sdk_path_env)
    if chpl_sdk_path:
        if for_gpu == get() and not validate_path(chpl_sdk_path):
            _reportMissingGpuReq(
                "{}='{}' does not exist. Make sure the toolkit path is correct."
                .format(gpu.sdk_path_env, chpl_sdk_path))
            return 'error'
        return chpl_sdk_path

    # find the sdk path from the compiler based on the one in PATH
    sdk_path = gpu.find_sdk_path(gpu.compiler)
    if sdk_path:
        if not validate_path(sdk_path):
            _reportMissingGpuReq(
                "Inferred {} toolkit is not valid. Try setting {}."
                .format(gpu.runtime_impl, gpu.sdk_path_env))
            return 'error'
        return sdk_path
    elif for_gpu == get():
        _reportMissingGpuReq("Can't infer {} toolkit from '{}'. Try setting {}."
              .format(gpu.runtime_impl, gpu.compiler, gpu.sdk_path_env))
        return 'error'
    else:
        return 'none'

@memoize
def is_sdk_path_user_specified(for_gpu):
    return overrides.get(GPU_TYPES[for_gpu].sdk_path_env) is not None


@memoize
def get_gpu_mem_strategy():
    memtype = overrides.get("CHPL_GPU_MEM_STRATEGY")
    if memtype:
        valid_options = ["array_on_device", "unified_memory"]
        if memtype not in valid_options:
            error("CHPL_GPU_MEM_STRATEGY must be set to one of: %s" %
                 ", ".join(valid_options))
        return memtype
    return "array_on_device"

def get_runtime_compile_args():
    if chpl_locale_model.get() != 'gpu':
        return [], []
    bundled = []
    system = []

    gpu_type = get()
    sdk_path = get_sdk_path(gpu_type)
    incl = chpl_home_utils.get_chpl_runtime_incl()

    # this -D is needed since it affects code inside of headers
    bundled.append("-DHAS_GPU_LOCALE")
    if gpu_type == "cpu":
        bundled.append("-DGPU_RUNTIME_CPU")

    # If compiling for GPU locales, add CUDA runtime headers to include path
    bundled.append("-I" + os.path.join(incl, "gpu", gpu_type))
    if gpu_type == "nvidia":
        # treat the CUDA SDK as a system include
        system.append("-idirafter" + os.path.join(sdk_path, "include"))

        # workaround an issue with __float128 not being supported by clang in device code
        system.append("-D__STRICT_ANSI__=1")

    elif gpu_type == "amd":
        # -isystem instead of -I silences warnings from inside these includes.
        system.append("-isystem" + os.path.join(sdk_path, "include"))
        system.append("-isystem" + os.path.join(sdk_path, "hip", "include"))

        major_version = get_sdk_version().split('.')[0]
        bundled.append("-DROCM_VERSION_MAJOR=" + major_version)
    
    return bundled, system

def get_runtime_link_args():
    if chpl_locale_model.get() != 'gpu':
        return [], []
    bundled = []
    system = []

    gpu_type = get()
    sdk_path = get_sdk_path(gpu_type)

    if gpu_type == "nvidia":
        system.append("-L" + os.path.join(sdk_path, "lib64"))
        system.append("-lcudart")
        if chpl_platform.is_wsl():
            # WSL needs to link with libcuda that belongs to the driver hosted in Windows
            system.append("-L" + os.path.join("/usr", "lib", "wsl", "lib"))
        system.append("-lcuda")
    elif gpu_type == "amd":
        paths = [sdk_path]
        for p in paths:
            lib_path = os.path.join(p, "lib")
            system.append("-L" + lib_path)
            system.append("-Wl,-rpath," + lib_path)
        system.append("-lamdhip64")

    return bundled, system

@memoize
def get_cuda_libdevice_path():
    if get() == 'nvidia':
        compiler = get_gpu_compiler()
        libdevice = find_cuda_libdevice_path(compiler)
        if not libdevice:
            _reportMissingGpuReq("Can't determine libdevice path from {}".format(compiler))
            return 'error'
        return libdevice
    return "none"

@memoize
def get_rocm_llvm_path():
    if get() == 'amd':
        compiler = get_gpu_compiler()
        llvm_path = find_llvm_amd_bin_path(compiler)
        if not llvm_path:
            _reportMissingGpuReq("Can't determine AMD LLVM path from {}".format(compiler))
            return 'error'
        # strip bin path component (and trailing /)
        return os.path.dirname(llvm_path)
    return 'none'

@memoize
def get_rocm_amdgcn_path():
    if get() == 'amd':
        compiler = get_gpu_compiler()
        amdgcn_path = find_amdgcn_path(compiler)
        if not amdgcn_path:
            _reportMissingGpuReq("Can't determine amdgcn path from {}".format(compiler))
            return 'error'
        return amdgcn_path
    return 'none'


def validateLlvmBuiltForTgt(expectedTgt):
    # If we're using the bundled LLVM, llvm-config may not have been built
    # before we call chplenv. It seems safe to assume the bundled LLVM has been
    # built with whatever requirements we have for Chapel so we just return
    # that it's been validated.
    if chpl_llvm.get() == 'bundled':
        return True

    exists, returncode, my_stdout, _ = try_run_command(
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
    major_version, minor_version = get_sdk_version().split('.')[:2]

    if major_version in ('5',) and chpl_llvm.get() == 'bundled':
        error("Cannot target AMD GPUs with CHPL_LLVM=bundled")
    elif major_version == '6':
        if int(minor_version) < 3:
            # it must be bundled LLVM or patched
            if chpl_llvm.get() != 'bundled' and os.environ.get("CHPL_LLVM_65188_PATCH", "0") != "1":
                error("Cannot target AMD GPUs with ROCm 6.0-6.2 without CHPL_LLVM=bundled")
        else:
            # it must be LLVM 21+ or the bundled LLVM
            if chpl_llvm.get() != 'bundled' and int(chpl_llvm.get_llvm_version()) < 21:
                error("Cannot target AMD GPUs with ROCm 6.3 without CHPL_LLVM=bundled or LLVM 21+")
    elif major_version == '7':
        # requires LLVM 21+
        if int(chpl_llvm.get_llvm_version()) < 21:
            error("Cannot target AMD GPUs with ROCm 7.x without LLVM 21+")
    elif not validateLlvmBuiltForTgt(gpu.llvm_target):
        _reportMissingGpuReq(
            "LLVM not built for %s." % gpu.llvm_target, allowExempt=False
        )

def _validate_cuda_version_impl():
    """Check that the installed CUDA version is >= MIN_REQ_VERSION and <
       MAX_REQ_VERSION"""
    MIN_REQ_VERSION = "11.7"
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

@memoize
def get_sdk_version():
    gpu = GPU_TYPES[get()]

    if os.environ.get('CHPL_GPU_SDK_VERSION') is not None:
        fixstr = "."
        if gpu.real_gpu:
            var = gpu.sdk_path_env
            fixstr = " and set {} to the desired version-specific path instead.".format(var)
        warning("Setting CHPL_GPU_SDK_VERSION has no effect. Unset CHPL_GPU_SDK_VERSION"+fixstr)

    if not gpu.real_gpu:
        return 'none'
    version = gpu.find_version(get_gpu_compiler())
    # TODO: add validation for if the compiler matches what the user set CHPL_GPU_SDK_VERSION?
    version = version.strip() if version is not None else 'none'
    return version


def _validate_rocm_version_impl():
    """Check that the installed ROCM version is >= MIN_REQ_VERSION and <
       MAX_REQ_VERSION"""
    MIN_REQ_VERSION = "5.0"
    MAX_REQ_VERSION = "5.5" # upper bound non-inclusive
    MAX_REQ_VERSION_NICE = "5.4.x"

    MIN_ROCM6_REQ_VERSION = "6.0"
    MAX_ROCM6_REQ_VERSION = "6.4" # upper bound non-inclusive
    MAX_ROCM6_REQ_VERSION_NICE = "6.3.x"

    MIN_ROCM7_REQ_VERSION = "7.0"
    MAX_ROCM7_REQ_VERSION = "8.0" # upper bound non-inclusive
    MAX_ROCM7_REQ_VERSION_NICE = "7.x"

    rocm_version = get_sdk_version()

    if rocm_version == 'none':
        _reportMissingGpuReq("Unable to determine ROCm version.")
        return False

    if (
        not is_ver_in_range(rocm_version, MIN_REQ_VERSION, MAX_REQ_VERSION)
        and not is_ver_in_range(
            rocm_version, MIN_ROCM6_REQ_VERSION, MAX_ROCM6_REQ_VERSION
        )
        and not is_ver_in_range(
            rocm_version, MIN_ROCM7_REQ_VERSION, MAX_ROCM7_REQ_VERSION
        )
    ):
        warning(
            "Chapel requires ROCm versions %s to %s, %s to %s, or %s to %s, "
            "detected version %s on system."
            % (
                MIN_REQ_VERSION,
                MAX_REQ_VERSION_NICE,
                MIN_ROCM6_REQ_VERSION,
                MAX_ROCM6_REQ_VERSION_NICE,
                MIN_ROCM7_REQ_VERSION,
                MAX_ROCM7_REQ_VERSION_NICE,
                rocm_version,
            )
        )
        return False

    return True

@memoize
def validate(chplLocaleModel):
    if chplLocaleModel != "gpu":
        return True

    gpu_val = get()
    if gpu_val == 'none':
        error("CHPL_GPU cannot be set to 'none' when using the 'gpu' locale model.")
    gpu = GPU_TYPES[gpu_val]

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

    return True
