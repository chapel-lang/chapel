"""
Test basic functionality when `--resolver` is used.

Note: when this becomes the default, these tests should be folded into `basic.py`
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
        server_command=[sys.executable, CLS_PATH(), "--resolver"],
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
async def test_go_to_record_def(client: LanguageClient):
    """
    Ensure that 'go to definition' on a type actually works.
    """

    file = """
           record myRec {}
           var x: myRec;
           var y = new myRec();
           var z = y;
           """

    async with source_file(client, file) as doc:
        await check_goto_decl_def(client, doc, pos((0, 7)), pos((0, 7)))
        await check_goto_decl_def(client, doc, pos((1, 7)), pos((0, 7)))
        await check_goto_decl_def(client, doc, pos((2, 12)), pos((0, 7)))
        await check_goto_type_def(client, doc, pos((2, 4)), pos((0, 7)))
        await check_goto_type_def(client, doc, pos((3, 9)), pos((0, 7)))


@pytest.mark.asyncio
async def test_go_to_call_basic(client: LanguageClient):
    """
    Ensure that 'go to definition' works for concrete called functions.
    """

    file = """
           proc foo(x: int) {}
           proc foo(x: bool) {}
           foo(1);
           foo(true);
           """

    async with source_file(client, file) as doc:
        await check_goto_decl_def(client, doc, pos((2, 0)), pos((0, 5)))
        await check_goto_decl_def(client, doc, pos((3, 0)), pos((1, 5)))


@pytest.mark.asyncio
async def test_go_to_call_generic(client: LanguageClient):
    """
    Ensure that 'go to definition' works for generic functions and calls
    made from within them.
    """

    file = """
           proc foo(x: int) {}
           proc foo(x: bool) {}
           proc bar(x) do foo(x);
           bar(1);
           bar(true);
           """
    lenses = [(pos((2, 5)), 3)]

    async with source_file(client, file) as doc:
        got_lenses = await check_generic_code_lenses(client, doc, lenses)
        lenses_for_bar = got_lenses[(2, 5)]

        async def select_inst(n, expect_title):
            assert lenses_for_bar[n].command is not None
            assert lenses_for_bar[n].command.title == expect_title
            await execute_command(client, lenses_for_bar[n].command)

        # Before selecting instantiation, should not know which call it is
        await check_goto_decl_def(client, doc, pos((2, 15)), None)
        await check_goto_decl_def(client, doc, pos((2, 19)), pos((2, 9)))

        # Select the first instantiation
        await select_inst(1, "Show Instantiation")
        await check_goto_decl_def(client, doc, pos((2, 15)), pos((0, 5)))
        await check_goto_decl_def(client, doc, pos((2, 19)), pos((2, 9)))

        # Select the second instantiation
        await select_inst(2, "Show Instantiation")
        await check_goto_decl_def(client, doc, pos((2, 15)), pos((1, 5)))
        await check_goto_decl_def(client, doc, pos((2, 19)), pos((2, 9)))

        # Clear instantiation; call to 'foo' should be unclickable again.
        await select_inst(0, "Show Generic")
        await check_goto_decl_def(client, doc, pos((2, 15)), None)
        await check_goto_decl_def(client, doc, pos((2, 19)), pos((2, 9)))


@pytest.mark.asyncio
@pytest.mark.xfail
async def test_string(client: LanguageClient):
    """
    Ensure that goto-type works on a string.
    This should work, but currently crashes dyno.
    """

    file = """
           var x = "hello";
           """

    async with source_file(client, file) as doc:
        string_loc = internal_module("String")
        await check_goto_type_def(
            client, doc, pos((0, 4)), string_loc, "record _string"
        )
        await check_goto_type_def(
            client, doc, pos((0, 12)), string_loc, "record _string"
        )


@pytest.mark.asyncio
async def test_error_location_specific(client: LanguageClient):
    """
    Lock down that LSP errors for failed call resolution occur at the
    failed actual, if there is one.
    """

    file = """
           use IO except ioMode as iomode;

           proc foo(ok1: int, ok2: int, x: int, ok3: int) {}
           proc bar(ok1: int, ok2: int, x: int, ok3: int) {}
           proc bar(ok1: int, ok2: int, x: bool, ok3: int) {}
           foo(0, 0, 1.0, 0);
           bar(0, 0, 1.0, 0);

           proc baz(a: real, b: int) {}
           proc baz(a: int, b: real) {}
           baz(1.0, 1.0);

           foo(ok1=(...(1,)), 1, 1, 1);
           foo((...(1)), 1, 1, 1);

           proc int.beep(ok1: int, ok2: int, x: int, ok3: int) {}
           42.beep(0, 0, 1.0, 0);

           proc int.test() {
             beep(0, 0, 1.0, 0);
           }
           """

    def check_location(diagnostic, name, start_pos, end_pos):
        assert "[{}]".format(name) in diagnostic.message
        assert diagnostic.range.start.line == start_pos[0]
        assert diagnostic.range.start.character == start_pos[1]
        assert diagnostic.range.end.line == end_pos[0]
        assert diagnostic.range.end.character == end_pos[1]

    async with source_file(client, file, num_errors=8) as doc:
        await save_file(client, doc)
        diags = client.diagnostics[doc.uri]
        diags = sorted(diags, key=lambda d: d.range.start.line)

        # Using check_location
        check_location(diags[0], "AsWithUseExcept", (0, 14), (0, 30))
        check_location(diags[1], "NoMatchingCandidates", (5, 10), (5, 13))
        check_location(diags[2], "NoMatchingCandidates", (6, 10), (6, 13))
        check_location(diags[3], "NoMatchingCandidates", (10, 0), (10, 13))
        check_location(diags[4], "TupleExpansionNamedArgs", (12, 8), (12, 17))
        check_location(diags[5], "TupleExpansionNonTuple", (13, 4), (13, 12))
        check_location(diags[6], "NoMatchingCandidates", (16, 14), (16, 17))
        check_location(diags[7], "NoMatchingCandidates", (19, 13), (19, 16))
