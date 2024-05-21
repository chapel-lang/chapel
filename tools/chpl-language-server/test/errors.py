"""
Tests basic functionality, including autocompletion, go-to-definition, hover,
and references
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
async def test_redefinition_error(client: LanguageClient):
    """
    Ensure that redefinition errors are always issued, even if
    there are no identifiers to scope resolve for.
    """

    file = """
           var x = 42;
           var x = 42;
           """

    async with source_file(client, file, num_errors=1) as doc:
        pass
