"""
Test that the linter is working properly from CLS. These tests are not meant to be exhaustive of the linter's capabilities, rather they are meant to ensure that the integration between CLS and chplcheck is working properly.
"""

import sys

from lsprotocol.types import ClientCapabilities
from lsprotocol.types import InitializeParams
from lsprotocol.types import CodeActionParams, CodeActionContext
import pytest
import pytest_lsp
from pytest_lsp import ClientServerConfig, LanguageClient

from util.utils import *
from util.config import CLS_PATH


@pytest_lsp.fixture(
    config=ClientServerConfig(
        server_command=[sys.executable, CLS_PATH(), "--chplcheck"],
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
async def test_lint_warnings(client: LanguageClient):
    """
    Test that lint warnings are reported properly
    """
    file = """
            for i in {1..10} do { }
           """
    async with source_file(client, file, 3) as doc:
        pass


@pytest.mark.asyncio
async def test_lint_fixits(client: LanguageClient):
    """
    Test that fixits work properly
    """
    file = """
            for i in {1..10} { i; }
           """
    async with source_file(client, file, None) as doc:
        await save_file(client, doc)
        assert len(client.diagnostics[doc.uri]) == 1

        diagnostics = client.diagnostics[doc.uri]
        actions = await client.text_document_code_action_async(
            CodeActionParams(
                doc, rng((0, 0), endpos(file)), CodeActionContext(diagnostics)
            )
        )
        assert actions is not None
        # there should 2 actions, one to fixit and one to ignore. make sure that both exists
        # TODO: check that the fixits are valid and what we expect for a given input string
        # TODO: this should also be made into a helper function
        assert len(actions) == 2
        assert (
            "Apply Fix for" in actions[0].title
            and "Ignore" not in actions[0].title
        )
        assert "Ignore" in actions[1].title
        # TODO: check that applying the fixits actually resolves the warning
