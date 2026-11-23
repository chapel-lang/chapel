import unittest
from contextlib import ExitStack
from unittest.mock import patch

import chpl_gpu


class CudaVersionTests(unittest.TestCase):
    def test_cuda_11_llvm_versions(self):
        for cuda in ("11.0", "11.6", "11.7", "11.8"):
            for llvm_version in (22, 23, 24):
                for llvm_kind in ("system", "bundled"):
                    with self.subTest(
                        cuda=cuda, llvm=llvm_version, kind=llvm_kind
                    ), ExitStack() as stack:
                        stack.enter_context(
                            patch.object(
                                chpl_gpu, "get_sdk_version", return_value=cuda
                            )
                        )
                        stack.enter_context(
                            patch.multiple(
                                chpl_gpu.chpl_llvm,
                                get=lambda: llvm_kind,
                                get_llvm_version=lambda: str(llvm_version),
                            )
                        )
                        report = stack.enter_context(
                            patch.object(chpl_gpu, "_reportMissingGpuReq")
                        )
                        warning = stack.enter_context(
                            patch.object(chpl_gpu, "warning")
                        )

                        valid = chpl_gpu._validate_cuda_version_impl()
                        if llvm_version >= 23:
                            self.assertFalse(valid)
                            report.assert_called_once()
                            message = report.call_args.args[0]
                            self.assertIn("CUDA 11", message)
                            self.assertIn("LLVM", message)
                            self.assertIn(str(llvm_version), message)
                            self.assertEqual(
                                report.call_args.kwargs,
                                {"allowExempt": False, "suggestNone": False},
                            )
                            warning.assert_not_called()
                        else:
                            self.assertEqual(valid, cuda in ("11.7", "11.8"))
                            report.assert_not_called()

    def test_newer_cuda_with_llvm_23(self):
        for cuda in ("12.0", "12.9", "13.0"):
            for llvm_kind in ("system", "bundled"):
                with self.subTest(
                    cuda=cuda, kind=llvm_kind
                ), ExitStack() as stack:
                    stack.enter_context(
                        patch.object(
                            chpl_gpu, "get_sdk_version", return_value=cuda
                        )
                    )
                    stack.enter_context(
                        patch.multiple(
                            chpl_gpu.chpl_llvm,
                            get=lambda: llvm_kind,
                            get_llvm_version=lambda: "23",
                            find_system_llvm_config=lambda: "llvm-config",
                            get_llvm_config_version=lambda config: "23.0.0",
                        )
                    )
                    report = stack.enter_context(
                        patch.object(chpl_gpu, "_reportMissingGpuReq")
                    )
                    self.assertTrue(chpl_gpu._validate_cuda_version_impl())
                    report.assert_not_called()

    def test_cuda_11_with_unbuilt_bundled_llvm(self):
        with patch.object(chpl_gpu, "get_sdk_version", return_value="11.8"):
            with patch.multiple(
                chpl_gpu.chpl_llvm,
                get=lambda: "bundled",
                get_llvm_version=lambda: 0,
            ):
                with patch.object(chpl_gpu, "_reportMissingGpuReq") as report:
                    self.assertTrue(chpl_gpu._validate_cuda_version_impl())
                    report.assert_not_called()

    def test_cuda_11_without_llvm_backend(self):
        for llvm_kind in ("none", "unset"):
            with self.subTest(kind=llvm_kind), ExitStack() as stack:
                stack.enter_context(
                    patch.object(
                        chpl_gpu, "get_sdk_version", return_value="11.8"
                    )
                )
                stack.enter_context(
                    patch.object(
                        chpl_gpu.chpl_llvm, "get", return_value=llvm_kind
                    )
                )
                llvm_version = stack.enter_context(
                    patch.object(
                        chpl_gpu.chpl_llvm,
                        "get_llvm_version",
                        return_value="23",
                    )
                )
                self.assertTrue(chpl_gpu._validate_cuda_version_impl())
                llvm_version.assert_not_called()

    def test_cuda_11_llvm_error_cannot_be_downgraded(self):
        with ExitStack() as stack:
            stack.enter_context(
                patch.dict(
                    "os.environ", {"CHPLENV_GPU_REQ_ERRS_AS_WARNINGS": "1"}
                )
            )
            stack.enter_context(
                patch.multiple(
                    chpl_gpu,
                    get=lambda: "nvidia",
                    get_sdk_version=lambda: "11.0",
                    validateLlvmBuiltForTgt=lambda target: True,
                )
            )
            stack.enter_context(
                patch.multiple(
                    chpl_gpu.chpl_llvm,
                    get=lambda: "system",
                    get_llvm_version=lambda: "23",
                )
            )
            stack.enter_context(
                patch.object(
                    chpl_gpu.chpl_tasks, "get", return_value="qthreads"
                )
            )
            stack.enter_context(
                patch.object(chpl_gpu.chpl_compiler, "get", return_value="llvm")
            )
            stack.enter_context(patch.object(chpl_gpu, "warning"))
            stack.enter_context(
                patch.object(chpl_gpu, "error", side_effect=ValueError)
            )
            with self.assertRaisesRegex(ValueError, "LLVM.*CUDA 11"):
                chpl_gpu.validate("gpu")


class RocmVersionTests(unittest.TestCase):
    def test_supported_versions(self):
        for version in (
            "5.0",
            "5.4.3",
            "6.0",
            "6.3.4",
            "7.0",
            "7.2",
            "10.0",
            "10.0.0",
            "10.1",
            "10.1.0",
            "10.1.9",
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
            "4.9",
            "5.5",
            "6.4",
            "8.0",
            "9.0",
            "9.9",
            "10.2",
            "11.0",
        ):
            with self.subTest(version=version):
                with patch.object(
                    chpl_gpu, "get_sdk_version", return_value=version
                ):
                    with patch.object(chpl_gpu, "warning") as warning:
                        self.assertFalse(chpl_gpu._validate_rocm_version_impl())
                        warning.assert_called_once()
                        self.assertIn(
                            "10.0 to 10.1.x", warning.call_args.args[0]
                        )

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
                        stack.enter_context(
                            patch.multiple(
                                chpl_gpu,
                                get_sdk_version=lambda: rocm,
                                validateLlvmBuiltForTgt=lambda target: True,
                            )
                        )
                        stack.enter_context(
                            patch.multiple(
                                chpl_gpu.chpl_llvm,
                                get=lambda: llvm_kind,
                                get_llvm_version=lambda: str(llvm),
                            )
                        )
                        error = stack.enter_context(
                            patch.object(
                                chpl_gpu, "error", side_effect=ValueError
                            )
                        )
                        gpu = chpl_gpu.GPU_TYPES["amd"]
                        if supported:
                            chpl_gpu._validate_rocm_llvm_version_impl(gpu)
                            error.assert_not_called()
                        else:
                            with self.assertRaises(ValueError):
                                chpl_gpu._validate_rocm_llvm_version_impl(gpu)


if __name__ == "__main__":
    unittest.main()
