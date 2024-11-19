/*
  /!\ /!\ /!\ /!\ /!\ /!\ /!\ /!\ /!\ /!\ /!\ /!\ /!\ /!\ /!\ /!\
  /!\ /!\ /!\ /!\ /!\ /!\ /!\ /!\ /!\ /!\ /!\ /!\ /!\ /!\ /!\ /!\

  ATTENTION:

      If this test fails it means we're running a new version of LLVM.  Before
      updating the test so it no longer fails, use the following checklist to
      see if there are other changes we need to make.

      If you don't want to address all these items yourself, create a GH issue
      about addressing them and assign that to someone who's familiar with GPU
      support and who will follow through on it.

  /!\ /!\ /!\ /!\ /!\ /!\ /!\ /!\ /!\ /!\ /!\ /!\ /!\ /!\ /!\ /!\
  /!\ /!\ /!\ /!\ /!\ /!\ /!\ /!\ /!\ /!\ /!\ /!\ /!\ /!\ /!\ /!\

  CHECKLIST:

    - [ ] In the technote search for any mentions of the previous version of
          LLVM and replace them with the newer version.

    - [ ] Check to see what version of CUDA and ROCm this version of LLVM
          supports for its NVPTX and AMDGPU backend.  If (compared to our
          prior supported version) things have changed.  Determine if we want to
          have Chapel support this newer version of CUDA/ROCm.  If so then:

        - [ ] Make sure we have nightly testing set up to work with this newer
              version.

        - [ ] Look at util/chplenv/chpl_gpu.py, in here we have code that
              checks to see if you're using a compatible version of CUDA/ROCm
              (as of writing this comment this is in the
              `_validate_cuda_version_impl` and `_validate_rocm_version_impl`
              functions).  Update this code.

        - [ ] Check to see if there are any mentions of these version
              requirements in the tech note and update as needed.

    - [ ] Take a moment to think if there are other items that should belong on
          this checklist and add them if appropriate.

    - [ ] Update the EXPECTED_LLVM_VERSION constant so this test no longer
          fails.
*/

//-----------------------------------------------------------------------------

// After addressing the checklist items (or creating a GH issue to do so)
// modify the following const so that this test no longer fails.
const EXPECTED_LLVM_VERSION = 19;

//-----------------------------------------------------------------------------

config const actualLlvmVersion : int; // Passed in by .execopts

if actualLlvmVersion > EXPECTED_LLVM_VERSION {
  writef("For GPU support, Chapel officially supports using LLVM %s but " +
    "found version %s, so this test has failed. ",
    EXPECTED_LLVM_VERSION, actualLlvmVersion);
  writef("Look at this test and its included checklist to see how to update" +
         "GPU support for a newer version of LLVM.");
}
