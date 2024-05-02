import sys
import os

from lsprotocol.types import ClientCapabilities
from lsprotocol.types import CompletionList
from lsprotocol.types import CompletionParams
from lsprotocol.types import InitializeParams
from lsprotocol.types import Position
from lsprotocol.types import TextDocumentIdentifier

import pytest
import pytest_lsp
from pytest_lsp import ClientServerConfig
from pytest_lsp import LanguageClient

CHPL_HOME = os.environ.get("CHPL_HOME")
if not CHPL_HOME:
    raise ValueError("The Language Server tests require the CHPL_HOME environment variable to be set.")

@pytest_lsp.fixture(
    config=ClientServerConfig(server_command=[sys.executable, os.path.join(CHPL_HOME, "tools", "chpl-language-server", "src", "chpl-language-server.py")]),
)
async def client(lsp_client: LanguageClient):
    # Setup
    params = InitializeParams(capabilities=ClientCapabilities())
    await lsp_client.initialize_session(params)

    yield

    # Teardown
    await lsp_client.shutdown_session()

def test_initialize(client: LanguageClient):
    pass
