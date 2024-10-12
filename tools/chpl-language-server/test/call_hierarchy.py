"""
Tests basic functionality, including autocompletion, go-to-definition, hover,
and references
"""

import sys

from lsprotocol.types import ClientCapabilities
from lsprotocol.types import (
    CallHierarchyPrepareParams,
    CallHierarchyOutgoingCallsParams,
    CallHierarchyItem,
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
            "--resolver",
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


class CallTree:
    def __init__(self, item_id: str, children: typing.List["CallTree"]):
        self.item_id = item_id
        self.children = children


async def collect_call_tree(
    client: LanguageClient, item: CallHierarchyItem, depth: int
) -> typing.Optional[CallTree]:
    if depth <= 0:
        return None

    assert isinstance(item.data, list)
    assert len(item.data) == 3
    item_id = item.data[0]

    children = []
    outgoing = await client.call_hierarchy_outgoing_calls_async(
        CallHierarchyOutgoingCallsParams(item)
    )
    if outgoing is not None:
        for outgoing_call in outgoing:
            new_tree = await collect_call_tree(
                client, outgoing_call.to, depth - 1
            )
            if new_tree is not None:
                children.append(new_tree)

    return CallTree(item_id, children)


async def compute_call_hierarchy(
    client: LanguageClient,
    doc: TextDocumentIdentifier,
    position: Position,
    depth: int,
) -> typing.Optional[CallTree]:
    items = await client.text_document_prepare_call_hierarchy_async(
        CallHierarchyPrepareParams(text_document=doc, position=position)
    )
    if items is None:
        return None

    assert len(items) == 1
    return await collect_call_tree(client, items[0], depth)


def verify_call_hierarchy(tree: CallTree, expected: CallTree):
    assert tree.item_id == expected.item_id
    assert len(tree.children) == len(expected.children)
    for i in range(len(tree.children)):
        verify_call_hierarchy(tree.children[i], expected.children[i])


async def check_call_hierarchy(
    client: LanguageClient,
    doc: TextDocumentIdentifier,
    position: Position,
    expected: CallTree,
    depth: int = 10,
) -> typing.Optional[CallTree]:
    items = await client.text_document_prepare_call_hierarchy_async(
        CallHierarchyPrepareParams(text_document=doc, position=position)
    )
    assert items is not None
    assert len(items) == 1
    tree = await collect_call_tree(client, items[0], depth)
    assert tree is not None
    verify_call_hierarchy(tree, expected)
    return tree


@pytest.mark.asyncio
async def test_call_hierarchy_basic(client: LanguageClient):
    file = """
           proc foo() {}
           proc bar() do foo();
           bar();
           """

    async with source_file(client, file) as doc:
        expect = CallTree("main.bar", [CallTree("main.foo", [])])
        await check_call_hierarchy(client, doc, pos((2, 0)), expect)


@pytest.mark.asyncio
async def test_call_hierarchy_overloads(client: LanguageClient):
    file = """
           proc foo(arg: int) {}
           proc foo(arg: bool) {}
           foo(1);
           foo(true);
           """

    async with source_file(client, file) as doc:
        expect_int = CallTree("main.foo", [])
        await check_call_hierarchy(client, doc, pos((2, 0)), expect_int)
        expect_bool = CallTree("main.foo#1", [])
        await check_call_hierarchy(client, doc, pos((3, 0)), expect_bool)


@pytest.mark.asyncio
async def test_call_hierarchy_recursive(client: LanguageClient):
    file = """
           proc foo() do foo();
           foo();
           """

    async with source_file(client, file) as doc:
        expect = CallTree("main.foo", [CallTree("main.foo", [])])
        await check_call_hierarchy(client, doc, pos((1, 0)), expect, depth=2)


@pytest.mark.asyncio
async def test_call_hierarchy_across_files(client: LanguageClient):
    fileA = """
            module A {
              proc someImplementationDetail(arg: string) {}
            }
            """
    fileB = """
            module B {
              use A;

              proc toString(x: int): string do return "";
              proc toString(x: real): string do return "";

              proc doSomething(arg) {
                someImplementationDetail(toString(arg));
              }
            }
            """
    fileC = """
            module C {
              use B;

              doSomething(12);
              doSomething(12.0);
            }
            """

    expected_int = CallTree(
        "B.doSomething",
        [
            CallTree("A.someImplementationDetail", []),
            CallTree("B.toString", []),
        ],
    )
    expected_real = CallTree(
        "B.doSomething",
        [
            CallTree("A.someImplementationDetail", []),
            CallTree("B.toString#1", []),
        ],
    )

    async def check(docs):
        await check_call_hierarchy(client, docs("C"), pos((3, 2)), expected_int)
        await check_call_hierarchy(client, docs("C"), pos((4, 2)), expected_real)

    # Ensure that call hierarchy works without .cls-commands.json...
    async with unrelated_source_files(
        client, A=fileA, B=fileB, C=fileC
    ) as docs:
        await check(docs)

    # ...and with .cls-commands.json
    async with source_files(
        client, A=fileA, B=fileB, C=fileC
    ) as docs:
        await check(docs)
