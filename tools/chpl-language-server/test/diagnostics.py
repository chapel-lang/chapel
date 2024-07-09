"""
Test that various diagnostics are reported properly such as syntax errors,
deprecations, and scope resolution errors
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
async def test_incorrect_cross_file_diagnostics(client: LanguageClient):
    """
    Test that implicit module error only shows in correct file
    """
    fileA = """
            module A {
              use B;
              var x = 42;
            }
            """
    fileB = """
            module B {
              var x = 42;
            }
            writeln(x);
            """

    async with source_files(client, A=fileA, B=fileB) as docs:
        await save_file(client, docs("B"), docs("A"))
        assert len(client.diagnostics[docs("A").uri]) == 0
        assert len(client.diagnostics[docs("B").uri]) == 2

        # check if the expected warnings are present
        foundImplicitModule = False
        foundSameName = False
        for d in client.diagnostics[docs("B").uri]:
            if "ImplicitFileModule" in d.message:
                # Warning: [ImplicitFileModule]: an implicit module named 'B'
                # is being introduced to contain file-scope code
                foundImplicitModule = True
            if "ImplicitModuleSameName" in d.message:
                # Warning: [ImplicitModuleSameName]: module 'B' has
                # the same name as the implicit file module
                foundSameName = True

        assert foundImplicitModule and foundSameName


@pytest.mark.asyncio
async def test_deprecations(client: LanguageClient):
    """
    Test that deprecations are reported
    """
    file = """
           @deprecated("a is deprecated")
           var a = 10;
           var b = a;
           """
    async with source_file(client, file, None) as doc:
        await save_file(client, doc)
        assert len(client.diagnostics[doc.uri]) == 1
        assert "deprecated" in client.diagnostics[doc.uri][0].message


@pytest.mark.asyncio
async def test_syntax_errors(client: LanguageClient):
    """
    Test that syntax errors are reported
    """
    file = """
           var a = 10
           var b = a;
           """
    async with source_file(client, file, None) as doc:
        await save_file(client, doc)
        assert len(client.diagnostics[doc.uri]) == 1
        assert "Syntax: " in client.diagnostics[doc.uri][0].message


@pytest.mark.asyncio
async def test_redefinition_error_basic(client: LanguageClient):
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


@pytest.mark.asyncio
async def test_redefinition_error_fn(client: LanguageClient):
    """
    Ensure that redefinition errors are always issued, even if
    there are no identifiers to scope resolve for.
    """

    file = """
           proc foo() {
             var x = 42;
             var x = "";
           }
           """

    async with source_file(client, file, num_errors=1) as doc:
        pass


@pytest.mark.asyncio
async def test_redefinition_error_ident(client: LanguageClient):
    """
    Ensure that redefinition errors are always issued, even if
    there are no identifiers to scope resolve for.
    """

    file = """
           var x = 10;
           var x = 11;
           var y = x;
           """

    async with source_file(client, file, num_errors=1) as doc:
        pass
