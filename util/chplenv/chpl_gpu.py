import utils
import os
import chpl_locale_model

def get():
    if chpl_locale_model.get() == 'gpu':
        return 'cuda'
    else:
        return 'none'

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
