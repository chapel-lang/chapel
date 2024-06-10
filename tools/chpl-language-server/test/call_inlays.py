"""
Test call inlays for literals in call expressions. Requires `--resolver`.
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
            "--literal-arg-inlays",
            "--no-type-inlays",
            "--no-param-inlays",
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
async def test_call_inlays(client: LanguageClient):
    """
    Ensure that call inlays are shown for literals in call expressions.
    """

    file = """
           proc foo(a: int, b: int) {}

           foo(42, 43);
           foo(a=42, b=43);
           foo(a=42,
               43);

           foo(b=42, 43);
           foo(42, b=43);

           var y = 10;
           foo(42, y);
           foo(a=y, 33+9);
           """

    inlays = [
        (pos((2, 4)), "a = ", None),
        (pos((2, 8)), "b = ", None),
        (pos((5, 4)), "b = ", None),
        (pos((7, 10)), "a = ", None),
        (pos((8, 4)), "a = ", None),
        (pos((11, 4)), "a = ", None),
    ]

    async with source_file(client, file) as doc:
        await check_inlay_hints(client, doc, rng((0, 0), endpos(file)), inlays)


@pytest.mark.asyncio
async def test_call_inlays_complex(client: LanguageClient):
    """
    Ensure that call inlays are shown for complex literals in call expressions.
    """

    file = """
           proc foo(a) {}
           foo(3i+10);
           foo(3i+10.0);
           foo(3.0i+10.0);
           foo(3.0i+10);
           foo(10+3i);
           foo(10.0+3i);
           foo(10.0+3.0i);
           foo(10+3.0i);

           // Not 'literals' in our sense:
           foo(3i + 4i);
           foo(1 + 1);
           foo((-1) - (-i));
           """

    inlays = [(pos((i, 4)), "a = ", None) for i in range(1, 9)]

    async with source_file(client, file) as doc:
        await check_inlay_hints(client, doc, rng((0, 0), endpos(file)), inlays)


@pytest.mark.asyncio
async def test_call_inlays_negative(client: LanguageClient):
    """
    Ensure that call inlays are shown for negative literals in call expressions.
    """

    file = """
        proc foo(a) {}
        foo(-42);
        foo(-42.0);
        foo(-42i);
        foo(-42.0i);

        // Not 'literals' in our sense:
        foo(- -42);
        """

    inlays = [(pos((i, 4)), "a = ", None) for i in range(1, 5)]

    async with source_file(client, file) as doc:
        await check_inlay_hints(client, doc, rng((0, 0), endpos(file)), inlays)


@pytest.mark.asyncio
async def test_nested_call_inlays(client: LanguageClient):
    """
    Ensure that call inlays are shown for literals in nested call expressions.
    """

    file = """
           proc foo(a: int, b: int, c: int) {}
           proc bar(a: int, b: int) do return a + b;

           foo(bar(42, 43),
               44,
               b=bar(b=45, 46));
           """

    inlays = [
        (pos((3, 8)), "a = ", None),
        (pos((3, 12)), "b = ", None),
        (pos((4, 4)), "c = ", None),
        (pos((5, 16)), "a = ", None),
    ]

    async with source_file(client, file) as doc:
        await check_inlay_hints(client, doc, rng((0, 0), endpos(file)), inlays)


@pytest.mark.asyncio
async def test_call_inlays_generic(client: LanguageClient):
    """
    Ensure call inlays work for all literal types
    """

    file = """
           proc foo(a, b, c) {}

           foo(1, 2.0, true);
           foo(1_00, c=2, 3+2);
           foo(10i, a=0.5, false);
           foo(b="hello", " ", c="world");
           """

    inlays = [
        (pos((2, 4)), "a = ", None),
        (pos((2, 7)), "b = ", None),
        (pos((2, 12)), "c = ", None),
        (pos((3, 4)), "a = ", None),
        (pos((4, 4)), "b = ", None),
        (pos((4, 16)), "c = ", None),
        (pos((5, 15)), "a = ", None),
    ]

    async with source_file(client, file) as doc:
        await check_inlay_hints(client, doc, rng((0, 0), endpos(file)), inlays)


@pytest.mark.asyncio
@pytest.mark.xfail
async def test_call_inlays_in_range(client: LanguageClient):
    """
    Call inlays should only be generated in the requested range
    """

    file = """
           proc foo(a) {}

           foo(1);
           foo(2);
           """

    inlays = [(pos((2, 4)), "a = ", None), (pos((3, 4)), "a = ", None)]

    async with source_file(client, file) as doc:
        # check all inlays
        await check_inlay_hints(client, doc, rng((0, 0), endpos(file)), inlays)
        # check only the first inlay
        await check_inlay_hints(
            client, doc, rng((2, 0), pos((2, 5))), inlays[:1]
        )
        # check only the second inlay
        await check_inlay_hints(
            client, doc, rng((3, 0), pos((3, 5))), inlays[1:]
        )
        # check no inlays
        await check_inlay_hints(client, doc, rng((0, 0), pos((1, 0))), [])
