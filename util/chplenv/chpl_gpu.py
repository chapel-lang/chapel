import utils
import os


def get_cuda_path():
    chpl_cuda_path = os.environ.get("CHPL_CUDA_PATH")
    if chpl_cuda_path:
        return chpl_cuda_path

    exists, returncode, my_stdout, my_stderr = utils.try_run_command(["which",
                                                                      "nvcc"])

    if exists and returncode == 0:
        chpl_cuda_path = "/".join(my_stdout.strip().split("/")[:-2]) + "/lib64"
        return chpl_cuda_path
    else:
        return ""
