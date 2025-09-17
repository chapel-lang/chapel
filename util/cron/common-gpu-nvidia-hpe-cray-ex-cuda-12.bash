# common settings for running GPU nightly testing on the HPE Cray EX system with CUDA 12

# We need 12.8 for the stream test because the CUDA driver on pinoak
# only supports PTX for 12.8, until the driver is updated, we need to
# stick with 12.8 instead of 12.9
module load cuda/12.8  # default is CUDA 12.9
