"""
Test param inlays for param expressions. Requires `--resolver`.
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
            "--param-inlays",
            "--no-type-inlays",
            "--no-literal-arg-inlays",
            "--end-markers=none",
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
async def test_param_inlays_prim(client: LanguageClient):
    """
    Ensure that param inlays are shown.
    """

    file = '''
            param a = 10;
            param b = (a + 10) * 20;
            param c = sqrt(((a + 10) * 20): real):int;

            proc foo(type t) param {
              return t == int;
            }
            param d = if foo(int) then 1 else 10;
            param e = if foo(string) then 1 else 10;

            param f = "hello";

            param g = false;
            param h = true;

            param i = "hello\\nworld";
            param j = b"Lots of weird bytes in here \\t\\n\\r";
            param k = """this is a long
            long string""";
           '''

    inlays = [
        (pos((0, 7)), "10"),
        (pos((1, 7)), "400"),
        (pos((2, 7)), "20"),
        (pos((7, 7)), "1"),
        (pos((8, 7)), "10"),
        (pos((10, 7)), '"hello"'),
        (pos((12, 7)), "false"),
        (pos((13, 7)), "true"),
        (pos((15, 7)), '"hello\\nworld"'),
        (pos((16, 7)), 'b"Lots of weird bytes in here \\t\\n\\r"'),
        (pos((17, 7)), '"this is a long\\nlong string"'),
    ]

    async with source_file(client, file) as doc:
        await check_param_inlay_hints(
            client, doc, rng((0, 0), endpos(file)), inlays
        )

        # check that specifying a range works
        await check_param_inlay_hints(
            client, doc, rng((0, 0), (1, 0)), [inlays[0]]
        )


@pytest.mark.asyncio
async def test_param_inlays_split_init(client: LanguageClient):
    """
    Ensure that param inlays are shown properly for split init
    """

    file = """
            param a;
            if false then
              a = 10;
            else
              a = 20;
           """

    inlays = [(pos((0, 7)), "20")]
    async with source_file(client, file) as doc:
        await check_param_inlay_hints(
            client, doc, rng((0, 0), endpos(file)), inlays
        )
