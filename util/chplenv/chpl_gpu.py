import utils
import os


def get_cuda_path():
    chpl_cuda_path = os.environ.get("CHPL_CUDA_PATH")
    if chpl_cuda_path:
        return chpl_cuda_path

    res = utils.run_command("which nvcc".split())

    chpl_cuda_path = "/".join(res.strip().split("/")[:-2]) + "/lib64"



    return chpl_cuda_path

