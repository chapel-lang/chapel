import unittest
from contextlib import ExitStack
from unittest.mock import patch

import chpl_gpu


class RocmVersionTests(unittest.TestCase):
    def test_supported_versions(self):
        for version in (
            "5.0", "5.4.3", "6.0", "6.3.4", "7.0", "7.2",
            "10.0", "10.0.0", "10.1", "10.1.0", "10.1.9",
        ):
            with self.subTest(version=version):
                with patch.object(
                    chpl_gpu, "get_sdk_version", return_value=version
                ):
                    with patch.object(chpl_gpu, "warning") as warning:
                        self.assertTrue(chpl_gpu._validate_rocm_version_impl())
                        warning.assert_not_called()

    def test_unsupported_versions(self):
        for version in (
            "4.9", "5.5", "6.4", "8.0", "9.0", "9.9", "10.2", "11.0",
        ):
            with self.subTest(version=version):
                with patch.object(
                    chpl_gpu, "get_sdk_version", return_value=version
                ):
                    with patch.object(chpl_gpu, "warning") as warning:
                        self.assertFalse(chpl_gpu._validate_rocm_version_impl())
                        warning.assert_called_once()
                        self.assertIn("10.0 to 10.1.x", warning.call_args.args[0])

    def test_missing_version(self):
        with patch.object(chpl_gpu, "get_sdk_version", return_value="none"):
            with patch.object(chpl_gpu, "_reportMissingGpuReq") as report:
                self.assertFalse(chpl_gpu._validate_rocm_version_impl())
                report.assert_called_once_with(
                    "Unable to determine ROCm version."
                )

    def test_rocm_llvm_versions(self):
        for rocm in ("7.1.0", "10.0.0", "10.1.0"):
            for llvm in (21, 22, 23, 24):
                supported = (
                    llvm in (21, 22) if rocm.startswith("7.") else llvm == 23
                )
                for llvm_kind in ("system", "bundled"):
                    with self.subTest(
                        rocm=rocm, llvm=llvm, kind=llvm_kind
                    ), ExitStack() as stack:
                        stack.enter_context(patch.multiple(
                            chpl_gpu,
                            get_sdk_version=lambda: rocm,
                            validateLlvmBuiltForTgt=lambda target: True,
                        ))
                        stack.enter_context(patch.multiple(
                            chpl_gpu.chpl_llvm,
                            get=lambda: llvm_kind,
                            get_llvm_version=lambda: str(llvm),
                        ))
                        error = stack.enter_context(patch.object(
                            chpl_gpu, "error", side_effect=ValueError
                        ))
                        gpu = chpl_gpu.GPU_TYPES["amd"]
                        if supported:
                            chpl_gpu._validate_rocm_llvm_version_impl(gpu)
                            error.assert_not_called()
                        else:
                            with self.assertRaises(ValueError):
                                chpl_gpu._validate_rocm_llvm_version_impl(gpu)


if __name__ == "__main__":
    unittest.main()
