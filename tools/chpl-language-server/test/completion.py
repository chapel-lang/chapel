"""
Test that completion works properly
"""

import sys

from lsprotocol.types import ClientCapabilities
from lsprotocol.types import Position, TextDocumentIdentifier
from lsprotocol.types import CompletionItem, CompletionList, CompletionParams
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


async def check_completion_items(
    client: LanguageClient,
    doc: TextDocumentIdentifier,
    pos: Position,
    expected: typing.List[str],
) -> typing.List[CompletionItem]:
    """
    Check that the names returned by the completion items match what is expected.

    Expected is a list strings of length N that should match the first N completion items.
    """
    items = await client.text_document_completion_async(
        params=CompletionParams(text_document=doc, position=pos)
    )
    assert items is not None
    items = items.items if isinstance(items, CompletionList) else items
    assert len(items) >= len(expected)

    sorted_items = sorted(items, key=lambda x: (x.sort_text or x.label).lower())
    print("     ")
    for expect, item in zip(expected, sorted_items):
        print(f"Expected: {expect}, Actual: {item.label}")
        assert item.label == expect
    return sorted_items


@pytest.mark.asyncio
async def test_empty(client: LanguageClient):
    """
    Test that an empty file returns something
    """
    test = ";"
    async with source_file(client, test) as doc:
        items = await check_completion_items(client, doc, pos((0, 0)), [])
        assert len(items) > 0


@pytest.mark.asyncio
async def test_basic_completion(client: LanguageClient):
    """
    Test basic features
    """
    file = """
            var myGlobal: int;
            record R {
              var abc: int;
              proc foo() { }
              proc bar(myFormal) {
                var localVar = 2;
                forall myIndex in 1..10
                  with (var myTaskPrivate = abc, var myGlobal = 1) {

                }
                begin {

                }
              }
            }
            var a = new R();
            """

    async with source_file(client, file) as doc:
        global_scope = (pos((0, 0)), ["a", "myGlobal", "R"])
        r_scope = (pos((2, 0)), ["abc", "bar", "foo"] + global_scope[1])
        bar_scope = (pos((5, 0)), ["localVar", "myFormal", "this"] + r_scope[1])

        # forall_scope changes where myGlobal is
        forall_parent_scope_elms = bar_scope[1].copy()
        forall_parent_scope_elms.remove("myGlobal")
        forall_scope = (
            pos((8, 0)),
            ["myGlobal", "myIndex", "myTaskPrivate"] + forall_parent_scope_elms,
        )

        begin_scope = (pos((11, 0)), bar_scope[1])

        expected = [global_scope, r_scope, bar_scope, forall_scope, begin_scope]
        for p, exp in expected:
            await check_completion_items(client, doc, p, exp)


@pytest.mark.asyncio
async def test_std_lib(client: LanguageClient):
    """
    Test that modules can be imported and used
    """
    A = """
        use IO;
        use B;
        var mySymbol = 1;
        """
    B = """
        proc foo() { }
        """

    async with source_files(client, A=A, B=B) as docs:
        a_scope = (pos((0, 0)), ["mySymbol", "foo"])
        b_scope = (pos((1, 0)), ["foo"])

        a_completion = await check_completion_items(
            client, docs("A"), a_scope[0], a_scope[1]
        )
        # a should have IO and writef somewhere in the list
        assert any(
            [x.label == "IO" or x.label == "writef" for x in a_completion]
        )

        await check_completion_items(client, docs("B"), b_scope[0], b_scope[1])

        await save_file(client, docs("A"), docs("B"))
        assert len(client.diagnostics[docs("A").uri]) == 0
        assert len(client.diagnostics[docs("B").uri]) == 0


@pytest.mark.asyncio
async def test_use_in_module(client: LanguageClient):
    """
    Test that modules can be imported and used inside of a module
    """

    file = """
            module file {
              var mySymbol = 1;
              module M {
                use Random;
              }
            }
            """

    async with source_file(client, file) as doc:
        file_scope = (pos((0, 0)), ["M", "mySymbol"])
        m_scope = (pos((3, 0)), ["M", "mySymbol"])

        await check_completion_items(client, doc, file_scope[0], file_scope[1])
        m_scope_items = await check_completion_items(
            client, doc, m_scope[0], m_scope[1]
        )
        assert any([x.label == "Random" for x in m_scope_items])


@pytest.mark.asyncio
async def test_use_in_scope(client: LanguageClient):
    """
    Test that modules can be imported and used inside of an arbitrary scope
    """

    file = """
            proc bar() {
              use Sort;
            }
            ;
            """

    async with source_file(client, file) as doc:
        # bar and Sort are at the same "depth", so we can't rely on ordering
        items = await check_completion_items(client, doc, pos((0, 0)), [])
        assert any([x.label == "bar" or x.label == "sort" for x in items])

        # outside of the scope, we should only see bar
        items = await check_completion_items(client, doc, pos((3, 0)), ["bar"])
        assert all([x.label != "sort" for x in items])
