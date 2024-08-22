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
