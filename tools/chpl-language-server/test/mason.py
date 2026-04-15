"""
Tests for mason project support in the language server.
"""

import os
import shutil
import stat
import sys
import tempfile
from pathlib import Path
from typing import Dict, List, Optional

from lsprotocol.types import ClientCapabilities
from lsprotocol.types import InitializeParams
import pytest
import pytest_lsp
from pytest_lsp import ClientServerConfig, LanguageClient

from util.utils import *
from util.config import CLS_PATH


def _write_fake_mason(directory: str, modules_output: str) -> None:
    """Write a small shell script called ``mason`` into *directory*.

    When invoked as ``mason modules`` it prints *modules_output* to stdout.
    All other sub-commands exit 0 with no output.
    """
    script = os.path.join(directory, "mason")
    with open(script, "w") as f:
        f.write("#!/usr/bin/env python3\n")
        f.write("import sys\n")
        f.write('if len(sys.argv) >= 2 and sys.argv[1] == "modules":\n')
        f.write(f'  print("{{\\"modules\\": [\\"{modules_output}\\"]}}")\n')
    os.chmod(
        script,
        os.stat(script).st_mode | stat.S_IEXEC | stat.S_IXGRP | stat.S_IXOTH,
    )


def make_fake_mason_env(modules_output: str) -> tuple:
    """Create a temporary directory containing a fake ``mason`` and return
    ``(tmpdir, server_env)`` where *server_env* is suitable for passing to
    ``ClientServerConfig(server_env=...)``.

    The caller is responsible for cleaning up *tmpdir* (it is a
    ``tempfile.TemporaryDirectory``).
    """
    tmpdir = tempfile.TemporaryDirectory()
    fake_mason_dep = os.path.join(tmpdir.name, modules_output)
    _write_fake_mason(tmpdir.name, fake_mason_dep)
    env = os.environ.copy()
    env["PATH"] = tmpdir.name + os.pathsep + env.get("PATH", "")
    return tmpdir, env, fake_mason_dep


_fake_mason_dir, _fake_mason_env, fake_mason_dep = make_fake_mason_env(
    "in/a/weird/far/away/place/MyDep.chpl"
)


def teardown_module():
    _fake_mason_dir.cleanup()


@pytest_lsp.fixture(
    config=ClientServerConfig(
        server_command=[sys.executable, CLS_PATH()],
        server_env=_fake_mason_env,
        client_factory=get_base_client,
    ),
)
async def client(lsp_client: LanguageClient):
    # Setup
    params = InitializeParams(capabilities=ClientCapabilities())
    await lsp_client.initialize_session(params)

    yield

    # Teardown
    await lsp_client.shutdown_session()


@pytest.mark.asyncio
async def test_mason(client: LanguageClient):
    """
    Test that the mock_get_mason_modules context manager correctly patches
    """

    filePackage = """
            module Package {
              import MyDep;
              var x = 42;
              var z = MyDep.y;
              proc main() {
                writeln(x);
                writeln(z);
              }
            }
            """
    fileTest = """
            use UnitTest;
            import Package;
            import MyDep;

            proc myTest(test: borrowed Test) throws {
              test.assertEqual(Package.x, 42);
              test.assertEqual(Package.z, MyDep.y);
            }

            UnitTest.main();
            """
    fileExample = """
            import Package;
            import MyDep;
            proc main() {
              writeln(Package.x);
              writeln(MyDep.y);
              writeln(Package.z);
            }
            """

    fileMyDep = """
            module MyDep {
              var y = 99;
            }
            """
    toml = """
            [brick]
            chplVersion = "2.9.0"
            license = "None"
            name = "Package"
            type = "application"
            version = "0.1.0"

            [dependencies]
            MyDep = "0.1.0"
            """

    async with with_file_structure(
        client,
        {
            "src/Package.chpl": filePackage,
            "test/Test.chpl": fileTest,
            "example/Example.chpl": fileExample,
            "Mason.toml": toml,
            fake_mason_dep: fileMyDep,
        },
    ) as docs:

        x_def = (docs("src/Package"), pos((2, 6)))
        y_def = (docs(fake_mason_dep), pos((1, 6)))
        z_def = (docs("src/Package"), pos((3, 6)))
        myDep_def = (docs(fake_mason_dep), pos((0, 7)))

        x_use = [
            (docs("src/Package"), pos((5, 12))),
            (docs("test/Test"), pos((5, 27))),
            (docs("example/Example"), pos((3, 18))),
        ]
        y_use = [
            (docs("src/Package"), pos((3, 16))),
            (docs("test/Test"), pos((6, 36))),
            (docs("example/Example"), pos((4, 16))),
        ]
        z_use = [
            (docs("src/Package"), pos((6, 12))),
            (docs("test/Test"), pos((6, 27))),
            (docs("example/Example"), pos((5, 18))),
        ]
        myDep_use = [
            (docs("src/Package"), pos((1, 9))),
            (docs("src/Package"), pos((3, 10))),
            (docs("test/Test"), pos((2, 7))),
            (docs("test/Test"), pos((6, 30))),
            (docs("example/Example"), pos((1, 7))),
            (docs("example/Example"), pos((4, 10))),
        ]

        to_check = [
            (x_def, x_use),
            (y_def, y_use),
            (z_def, z_use),
            (myDep_def, myDep_use),
        ]
        for def_loc, use_locs in to_check:
            for use_loc in use_locs:
                await check_goto_decl_def(
                    client, use_loc[0], use_loc[1], def_loc
                )

        await save_file(
            client,
            docs("src/Package"),
            docs("test/Test"),
            docs("example/Example"),
            docs(fake_mason_dep),
        )
        assert len(client.diagnostics[docs("src/Package").uri]) == 0
        assert len(client.diagnostics[docs("test/Test").uri]) == 0
        assert len(client.diagnostics[docs("example/Example").uri]) == 0
        assert (
            len(client.diagnostics[docs(fake_mason_dep).uri])
            == 0
        )
