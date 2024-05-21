"""
Tests that cls-commands.json works as expected, allowing nested directories to be resolved
"""

import sys

from lsprotocol.types import ClientCapabilities
from lsprotocol.types import InitializeParams
import pytest
import pytest_lsp
from pytest_lsp import ClientServerConfig, LanguageClient

from util.utils import *
from util.config import CLS_PATH


@pytest_lsp.fixture(
    config=ClientServerConfig(
        server_command=[sys.executable, CLS_PATH()],
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
async def test_list_references_across_dirs(client: LanguageClient):
    """
    Ensure that list-references works across multiple user-defined files in
    multiple directories
    """

    fileMain = """
            module Main {
              use A, B, C;

              proc main() {
                writeln(A.x);
                writeln(B.y);
                writeln(C.z);
              }
            }
            """
    fileA = """
            module A {
              var x = 42;
            }
            """
    fileB = """
            module B {
              use A;

              var y = x;
            }
            """
    fileC = """
            module C {
              import A.x;

              var z = x;
            }
            """
    async with source_files_dict(
        client,
        {
            "Main": fileMain,
            "subdir/A": fileA,
            "subdir/B": fileB,
            "subdir2/nested/C": fileC,
        },
    ) as docs:

        x_refs = [
            (docs("Main"), pos((4, 14))),
            (docs("subdir/A"), pos((1, 6))),
            (docs("subdir/B"), pos((3, 10))),
            (docs("subdir2/nested/C"), pos((3, 10))),
        ]
        y_refs = [
            (docs("Main"), pos((5, 14))),
            (docs("subdir/B"), pos((3, 6))),
        ]
        z_refs = [
            (docs("Main"), pos((6, 14))),
            (docs("subdir2/nested/C"), pos((3, 6))),
        ]

        await check_references_and_cross_check(
            client, x_refs[0][0], x_refs[0][1], x_refs
        )
        await check_references_and_cross_check(
            client, y_refs[0][0], y_refs[0][1], y_refs
        )
        await check_references_and_cross_check(
            client, z_refs[0][0], z_refs[0][1], z_refs
        )

        await save_file(
            client,
            docs("Main"),
            docs("subdir/A"),
            docs("subdir/B"),
            docs("subdir2/nested/C"),
        )
        assert len(client.diagnostics[docs("Main").uri]) == 0
        assert len(client.diagnostics[docs("subdir/A").uri]) == 0
        assert len(client.diagnostics[docs("subdir/B").uri]) == 0
        assert len(client.diagnostics[docs("subdir2/nested/C").uri]) == 0
