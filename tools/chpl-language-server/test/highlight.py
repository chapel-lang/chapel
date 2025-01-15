"""
Test that symbol highlights work properly.
"""

import sys

from lsprotocol.types import ClientCapabilities
from lsprotocol.types import Position, Range, TextDocumentIdentifier
from lsprotocol.types import DocumentHighlightParams
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
    )
)
async def client(lsp_client: LanguageClient):
    # Setup
    params = InitializeParams(capabilities=ClientCapabilities())
    await lsp_client.initialize_session(params)

    yield

    # Teardown
    await lsp_client.shutdown_session()


async def check_symbol_highlight(
    client: LanguageClient,
    doc: TextDocumentIdentifier,
    pos: Position,
    expected: typing.List[Range],
):
    highlights = await client.text_document_document_highlight_async(
        params=DocumentHighlightParams(text_document=doc, position=pos)
    )
    assert highlights is not None
    assert len(highlights) == len(expected)
    for highlight, expect in zip(highlights, expected):
        assert highlight.range == expect


@pytest.mark.asyncio
async def test_symbol_highlight(client: LanguageClient):
    """
    Test that symbols are properly highlighted.
    """
    file = """
            var symbol = 10;
            var x = 10 + symbol;
            proc foo(y) {
              if symbol then
                return y + symbol;
              else
                return y;
            }
            foo(symbol);
            """

    async with source_file(client, file) as doc:
        expected = [
            rng((0, 4), (0, 10)),
            rng((1, 13), (1, 19)),
            rng((3, 5), (3, 11)),
            rng((4, 15), (4, 21)),
            rng((8, 4), (8, 10)),
        ]
        await check_symbol_highlight(client, doc, pos((0, 4)), expected)
        await check_symbol_highlight(client, doc, pos((0, 7)), expected)
        await check_symbol_highlight(client, doc, pos((0, 10)), expected)


@pytest.mark.asyncio
async def test_symbol_highlight_ifvar(client: LanguageClient):
    """
    Test that symbols are properly highlighted.
    """
    file = """
            proc getMaybeNil() { }
            if var x = getMaybeNil() { x; }
            if const y = getMaybeNil() { y; }
            """

    async with source_file(client, file) as doc:
        expected_getMaybeNil = [
            rng((0, 5), (0, 16)),
            rng((1, 11), (1, 22)),
            rng((2, 13), (2, 24)),
        ]

        await check_symbol_highlight(
            client, doc, pos((0, 6)), expected_getMaybeNil
        )
        await check_symbol_highlight(
            client, doc, pos((1, 17)), expected_getMaybeNil
        )
        await check_symbol_highlight(
            client, doc, pos((2, 19)), expected_getMaybeNil
        )

        expected_x = [rng((1, 7), (1, 8)), rng((1, 27), (1, 28))]
        await check_symbol_highlight(client, doc, pos((1, 7)), expected_x)
        await check_symbol_highlight(client, doc, pos((1, 27)), expected_x)

        expected_y = [rng((2, 9), (2, 10)), rng((2, 29), (2, 30))]
        await check_symbol_highlight(client, doc, pos((2, 10)), expected_y)
        await check_symbol_highlight(client, doc, pos((2, 30)), expected_y)


@pytest.mark.asyncio
async def test_symbol_highlight_catch(client: LanguageClient):
    """
    Test that symbols are properly highlighted.
    """
    file = """
            try { }
            catch err: Error { writeln(err); }
            """

    async with source_file(client, file) as doc:
        expected_Error = [rng((1, 11), (1, 16))]
        await check_symbol_highlight(client, doc, pos((1, 11)), expected_Error)
        await check_symbol_highlight(client, doc, pos((1, 13)), expected_Error)

        expected_err = [rng((1, 6), (1, 9)), rng((1, 27), (1, 30))]
        await check_symbol_highlight(client, doc, pos((1, 6)), expected_err)
        await check_symbol_highlight(client, doc, pos((1, 9)), expected_err)
        await check_symbol_highlight(client, doc, pos((1, 28)), expected_err)
