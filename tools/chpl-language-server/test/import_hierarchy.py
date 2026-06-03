"""
Test the import hierarchy feature, which re-uses the call hierarchy LSP
feature to show module use/import chains.

Outgoing: for a given module, which modules does it import?
Incoming: for a given module, which modules import it?
"""

import sys

from lsprotocol.types import ClientCapabilities
from lsprotocol.types import (
    CallHierarchyPrepareParams,
    CallHierarchyIncomingCallsParams,
    CallHierarchyOutgoingCallsParams,
    CallHierarchyItem,
    SymbolKind,
)
from lsprotocol.types import InitializeParams
import pytest
import pytest_lsp
import typing
from pytest_lsp import ClientServerConfig, LanguageClient

from util.utils import *
from util.config import CLS_PATH


@pytest_lsp.fixture(
    config=ClientServerConfig(
        server_command=[
            sys.executable,
            CLS_PATH(),
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


async def prepare_module_hierarchy(
    client: LanguageClient,
    doc: TextDocumentIdentifier,
    position: Position,
) -> typing.Optional[CallHierarchyItem]:
    """
    Ask for the call hierarchy item at the given position.  Returns the item
    only when it represents a module (data[0] == "module").
    """
    items = await client.text_document_prepare_call_hierarchy_async(
        CallHierarchyPrepareParams(text_document=doc, position=position)
    )
    if items is None or len(items) == 0:
        return None
    item = items[0]
    assert isinstance(item.data, list) and len(item.data) == 4
    assert item.data[0] == "module"
    assert item.kind == SymbolKind.Module
    return item


@pytest.mark.asyncio
async def test_import_hierarchy_prepare_on_module_decl(client: LanguageClient):
    """Placing the cursor on a module declaration returns a module item."""
    file = """
           module Foo {
           }
           """
    async with source_file(client, file) as doc:
        item = await prepare_module_hierarchy(client, doc, pos((0, 7)))
        assert item is not None
        assert item.name == "Foo"


@pytest.mark.asyncio
async def test_import_hierarchy_prepare_on_use_stmt(client: LanguageClient):
    """Placing the cursor on the module name in a use statement returns a module item."""
    fileA = """
            module A {
            }
            """
    fileB = """
            module B {
              use A;
            }
            """
    async with source_files(client, A=fileA, B=fileB) as docs:
        item = await prepare_module_hierarchy(client, docs("B"), pos((1, 6)))
        assert item is not None
        assert item.name == "A"


@pytest.mark.asyncio
async def test_import_hierarchy_outgoing(client: LanguageClient):
    """Outgoing calls for a module shows the modules it imports."""
    fileA = """
            module A {
            }
            """
    fileB = """
            module B {
              use A;
            }
            """
    async with source_files(client, A=fileA, B=fileB) as docs:
        item = await prepare_module_hierarchy(client, docs("B"), pos((0, 7)))
        assert item is not None
        assert item.name == "B"

        outgoing = await client.call_hierarchy_outgoing_calls_async(
            CallHierarchyOutgoingCallsParams(item)
        )
        assert outgoing is not None
        assert len(outgoing) == 1
        names = {call.to.name for call in outgoing}
        assert names == {"A"}


@pytest.mark.asyncio
async def test_import_hierarchy_incoming(client: LanguageClient):
    """Incoming calls for a module shows which modules import it."""
    fileA = """
            module A {
            }
            """
    fileB = """
            module B {
              use A;
            }
            """
    async with source_files(client, A=fileA, B=fileB) as docs:
        item = await prepare_module_hierarchy(client, docs("A"), pos((0, 7)))
        assert item is not None
        assert item.name == "A"

        incoming = await client.call_hierarchy_incoming_calls_async(
            CallHierarchyIncomingCallsParams(item)
        )
        assert incoming is not None
        assert len(incoming) == 1
        names = {call.from_.name for call in incoming}
        assert names == {"B"}


@pytest.mark.asyncio
async def test_import_hierarchy_chain(client: LanguageClient):
    """Multi-level import chains: A imports B, B imports C."""
    fileC = """
            module C {
            }
            """
    fileB = """
            module B {
              use C;
            }
            """
    fileA = """
            module A {
              use B;
            }
            """

    async with source_files(client, A=fileA, B=fileB, C=fileC) as docs:
        item_a = await prepare_module_hierarchy(client, docs("A"), pos((0, 7)))
        assert item_a is not None
        outgoing_a = await client.call_hierarchy_outgoing_calls_async(
            CallHierarchyOutgoingCallsParams(item_a)
        )
        assert outgoing_a is not None
        assert len(outgoing_a) == 1
        b_items = [c for c in outgoing_a if c.to.name == "B"]
        assert len(b_items) == 1

        item_b = b_items[0].to
        outgoing_b = await client.call_hierarchy_outgoing_calls_async(
            CallHierarchyOutgoingCallsParams(item_b)
        )
        assert outgoing_b is not None
        assert len(outgoing_b) == 1
        names_b = {c.to.name for c in outgoing_b}
        assert names_b == {"C"}


@pytest.mark.asyncio
async def test_import_hierarchy_multiple_imports(client: LanguageClient):
    """A module that imports several others shows all of them as outgoing."""
    fileA = """
            module A {
            }
            """
    fileB = """
            module B {
            }
            """
    fileC = """
            module C {
              use A;
              use B;
            }
            """
    async with source_files(client, A=fileA, B=fileB, C=fileC) as docs:
        item = await prepare_module_hierarchy(client, docs("C"), pos((0, 7)))
        assert item is not None

        outgoing = await client.call_hierarchy_outgoing_calls_async(
            CallHierarchyOutgoingCallsParams(item)
        )
        assert outgoing is not None
        assert len(outgoing) == 2
        names = {c.to.name for c in outgoing}
        assert names == {"A", "B"}


@pytest.mark.asyncio
async def test_import_hierarchy_multiple_importers(client: LanguageClient):
    """A module imported by several others shows all of them as incoming."""
    fileA = """
            module A {
            }
            """
    fileB = """
            module B {
              use A;
            }
            """
    fileC = """
            module C {
              use A;
            }
            """
    async with source_files(client, A=fileA, B=fileB, C=fileC) as docs:
        # 'A' is at (0, 7)
        item = await prepare_module_hierarchy(client, docs("A"), pos((0, 7)))
        assert item is not None

        incoming = await client.call_hierarchy_incoming_calls_async(
            CallHierarchyIncomingCallsParams(item)
        )
        assert incoming is not None
        assert len(incoming) == 2
        names = {c.from_.name for c in incoming}
        assert names == {"B", "C"}
