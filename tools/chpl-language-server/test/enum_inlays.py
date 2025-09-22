"""
Test that enum value inlays show up properly, Requires `--resolver`.
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
        server_command=[
            sys.executable,
            CLS_PATH(),
            "--resolver",
            "--enum-inlays",
        ],
        client_factory=get_base_client,
    )
)
async def client(lsp_client: LanguageClient):
    # Setup
    params = InitializeParams(capabilities=ClientCapabilities())
    await lsp_client.initialize_session(params)

    yield

    # Teardown
    await lsp_client.shutdown_session()


@pytest.mark.asyncio
async def test_enum_inlays(client: LanguageClient):
    """
    Ensure that type inlays are shown for primitive types.
    """

    file = """
            enum color {
              red = 1,
              green,
              blue,
              cyan = green : int + 10,
              magenta,
              yellow
            }
           """

    inlays = [
        (pos((2, 7)), " = 2"),
        (pos((3, 6)), " = 3"),
        (pos((4, 25)), " /* 12 */"),
        (pos((5, 9)), " = 13"),
        (pos((6, 8)), " = 14"),
    ]

    async with source_file(client, file) as doc:
        await check_enum_inlay_hints(
            client, doc, rng((0, 0), endpos(file)), inlays
        )

        # check that specifying a range works
        await check_enum_inlay_hints(
            client, doc, rng((0, 0), (4, 0)), inlays[:2]
        )
