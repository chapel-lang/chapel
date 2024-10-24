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
        await select_inst(1, "Show instantiation")
        await check_goto_decl_def(client, doc, pos((2, 15)), pos((0, 5)))
        await check_goto_decl_def(client, doc, pos((2, 19)), pos((2, 9)))

        # Select the second instantiation
        await select_inst(2, "Show instantiation")
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
