import unittest

from utils import is_ver_in_range


class UtilsTests(unittest.TestCase):
    def test_is_ver_in_range(self):
        # Various tests for versions in range: [2, 4)
        for min_ver in ["2", "2.0", "2.0.0"]:
            for max_ver in ["4", "4.0", "4.0.0"]:
                # Versions too low
                for ver in ["0", "0.0", "1", "1.0", "1.0-alpha"]:
                    self.assertFalse(is_ver_in_range(ver, min_ver, max_ver))

                # Versions in bound
                for ver in [
                    "2",
                    "2.0",
                    "2.5",
                    "3",
                    "3-alpha",
                    "3.1",
                    "3.1.1",
                    "3.9",
                    "3.9.9.9.9",
                ]:
                    self.assertTrue(is_ver_in_range(ver, min_ver, max_ver))

                # Versions too high
                for ver in ["4", "4.0", "4.1", "4.0.1", "5", "5-alpha"]:
                    self.assertFalse(is_ver_in_range(ver, min_ver, max_ver))

        # Various tests for versions in range: [2.5, 4.5)
        for min_ver in ["2.5", "2.5.0"]:
            for max_ver in ["4.5", "4.5.0"]:
                # Versions too low
                for ver in ["2", "2.4", "2.4.0", "2.4.0.0", "2.4.9.9.9.9"]:
                    self.assertFalse(is_ver_in_range(ver, min_ver, max_ver))

                # Versions in bound
                for ver in ["2.5", "2.5.0", "2.6", "4.4", "4.4.0", "4.4.9"]:
                    self.assertTrue(is_ver_in_range(ver, min_ver, max_ver))

                # Versions too high
                for ver in ["4.5", "4.5.0", "4.5.9", "4.6", "5", "5.0"]:
                    self.assertFalse(is_ver_in_range(ver, min_ver, max_ver))
