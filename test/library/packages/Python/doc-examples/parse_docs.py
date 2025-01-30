import sys
import os
import argparse as ap
import typing
from dataclasses import dataclass, field
import shutil


@dataclass
class File:
    filename: str = ""
    file_contents: typing.List[str] = field(default_factory=list)

    def basename(self):
        return os.path.splitext(self.filename)[0]

    def write(self, directory: str):

        # find the first non-empty line, if it has leading whitespace, remove it
        # then, remove the same amount of whitespace from all lines
        whitespace = 0
        for line in self.file_contents:
            if line.strip() != "":
                whitespace = len(line) - len(line.lstrip())
                break
        self.file_contents = [
            line[whitespace:] if len(line) >= whitespace else line
            for line in self.file_contents
        ]

        with open(os.path.join(directory, self.filename), "w") as f:
            for line in self.file_contents:
                f.write(line + "\n")


@dataclass
class TestCase:
    test_file: File = field(default_factory=File)
    good_file: File = field(default_factory=File)
    compopts: typing.Optional[File] = None
    execopts: typing.Optional[File] = None
    noexec: bool = False

    def write(self, directory: str):
        self.test_file.write(directory)
        self.good_file.write(directory)
        if self.compopts:
            self.compopts.write(directory)
        if self.execopts:
            self.execopts.write(directory)
        if self.noexec:
            noexec_file = os.path.join(
                directory, self.test_file.basename() + ".noexec"
            )
            with open(noexec_file, "w"):
                pass


def parse(lines: typing.List[str]) -> typing.List[TestCase]:

    cur_line = 0

    def has_more():
        return cur_line < len(lines)

    tests = []
    test = None
    test_file = None
    good_file = None

    while has_more():
        # check for the start
        if (
            lines[cur_line].strip() == ".."
            and lines[cur_line + 1].strip() == "START_TEST"
        ):
            test = TestCase()
            cur_line += 2
            continue

        # check for the end
        if (
            test
            and lines[cur_line].strip() == ".."
            and lines[cur_line + 1].strip() == "END_TEST"
        ):
            if test_file is None:
                raise ValueError("No test file found")
            test.test_file = test_file
            tests.append(test)
            test = None
            cur_line += 2
            continue

        # check for the good file begin
        if (
            test
            and test_file
            and lines[cur_line].strip().startswith("START_GOOD")
        ):
            good_file = File(test_file.basename() + ".good")
            cur_line += 1
            continue

        # check for the good file end
        if (
            test
            and good_file
            and lines[cur_line].strip().startswith("END_GOOD")
        ):
            test.good_file = good_file
            good_file = None
            cur_line += 1
            continue

        # check for the filename
        if test and lines[cur_line].strip().startswith("FILENAME:"):
            filename = lines[cur_line].strip().split(":", maxsplit=1)[1].strip()
            test_file = File(filename)
            cur_line += 1
            continue
        # set the compopts file
        if (
            test
            and test_file
            and lines[cur_line].strip().startswith("COMPOPTS:")
        ):
            mylines = [lines[cur_line].strip().split(":", maxsplit=1)[1].strip()]
            test.compopts = File(test_file.basename() + ".compopts", mylines)
            cur_line += 1
            continue
        # set the execopts file
        if (
            test
            and test_file
            and lines[cur_line].strip().startswith("EXECOPTS:")
        ):
            mylines = [lines[cur_line].strip().split(":", maxsplit=1)[1].strip()]
            test.execopts = File(test_file.basename() + ".execopts", mylines)
            cur_line += 1
            continue

        # set the noexec flag
        if test and test_file and lines[cur_line].strip() == "NOEXEC":
            test.noexec = True
            cur_line += 1
            continue

        # skip the '.. code-block:: chapel'
        if test and lines[cur_line].strip().startswith(
            ".. code-block:: chapel"
        ):
            cur_line += 1
            continue

        # add the file lines to the good file
        if test and good_file:
            good_file.file_contents.append(lines[cur_line].rstrip())
            cur_line += 1
            continue
        # add the file lines to the test file
        if test and not good_file and test_file:
            test_file.file_contents.append(lines[cur_line].rstrip())
            cur_line += 1
            continue

        cur_line += 1

    return tests


def main():
    a = ap.ArgumentParser()
    a.add_argument("file")
    a.add_argument("--output-dir", required=True)
    args = a.parse_args()

    with open(args.file, "r") as f:
        lines = f.readlines()
        parsed = parse(lines)

    if not os.path.exists(args.output_dir):
        os.makedirs(args.output_dir)

    for test in parsed:
        test.write(args.output_dir)


if __name__ == "__main__":
    main()
