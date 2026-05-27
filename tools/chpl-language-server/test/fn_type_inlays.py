"""
Test that function return type inlay hints
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
        server_command=[
            sys.executable,
            CLS_PATH(),
            "--resolver",
            "--fn-type-inlays",
            "--no-type-inlays",
            "--no-param-inlays",
            "--no-literal-arg-inlays",
            "--end-markers=none",
        ],
        client_factory=get_base_client,
    )
)
async def client(lsp_client: LanguageClient):
    params = InitializeParams(capabilities=ClientCapabilities())
    await lsp_client.initialize_session(params)
    yield
    await lsp_client.shutdown_session()


@pytest.mark.asyncio
async def test_fn_type_inlay_concrete_int(client: LanguageClient):
    """
    Ensure that a concrete function returning int gets an inlay.
    """
    file = """
            proc test() do return 42;
            iter testI() do yield 42;
           """
    inlays = [
        (pos((0, 11)), "int(64)"),
        (pos((1, 12)), "int(64)"),
    ]
    async with source_file(client, file) as doc:
        await check_type_inlay_hints(
            client, doc, rng((0, 0), endpos(file)), inlays
        )


@pytest.mark.asyncio
async def test_fn_type_inlay_concrete_real(client: LanguageClient):
    """
    Ensure that a concrete function returning real gets an inlay.
    """
    file = """
            proc foo() {
              return 42.0;
            }
            iter fooI() {
              yield 42.0;
            }
           """
    inlays = [
        (pos((0, 10)), "real(64)"),
        (pos((3, 11)), "real(64)"),
    ]
    async with source_file(client, file) as doc:
        await check_type_inlay_hints(
            client, doc, rng((0, 0), endpos(file)), inlays
        )


@pytest.mark.asyncio
async def test_fn_type_inlay_explicit_no_inlay(client: LanguageClient):
    """
    Ensure that a function with an explicit return type annotation gets no inlay.
    """
    file = """
            proc bar(): int do return 1;
            iter barI(): int do yield 1;
           """
    async with source_file(client, file) as doc:
        await check_type_inlay_hints(client, doc, rng((0, 0), endpos(file)), [])


@pytest.mark.asyncio
async def test_fn_type_inlay_generic_const_return(client: LanguageClient):
    """
    Ensure that a generic function with a concrete return value gets the
    common inlay when called with multiple types.
    """
    file = """
            proc idk2(x) { return 42; }
            iter idk3(x) { yield 42; }
            idk2(10);
            idk2(10.0);
            for z in idk3(10) do {}
            for z in idk3(10.0) do {}
           """
    inlays = [
        (pos((0, 12)), "int(64)"),
        (pos((1, 12)), "int(64)"),
    ]
    async with source_file(client, file) as doc:
        await check_type_inlay_hints(
            client, doc, rng((0, 0), endpos(file)), inlays
        )


@pytest.mark.asyncio
async def test_fn_type_inlay_return_intent_type(client: LanguageClient):
    """
    Ensure return type annotations for functions with explicit return intents.
    """
    file = """
            proc baz() type { return int; }
            iter bazI() { yield 42; }
           """
    inlays = [
        (pos((0, 15)), "int(64)"),
        (pos((1, 11)), "int(64)"),
    ]
    async with source_file(client, file) as doc:
        await check_type_inlay_hints(
            client, doc, rng((0, 0), endpos(file)), inlays
        )


@pytest.mark.asyncio
async def test_fn_type_inlay_where_clause(client: LanguageClient):
    """
    Ensure that a function with a 'where' clause gets an inlay in the right
    spot.
    """
    file = """
            proc boop() type where true { return int; }
            iter boopI() where true { yield 42; }
           """
    inlays = [
        (pos((0, 16)), "int(64)"),
        (pos((1, 12)), "int(64)"),
    ]
    async with source_file(client, file) as doc:
        await check_type_inlay_hints(
            client, doc, rng((0, 0), endpos(file)), inlays
        )


@pytest.mark.asyncio
async def test_fn_type_inlay_per_instantiation(client: LanguageClient):
    """
    For functions whose return type depends on the instantiation (i.e.,
    is not common), ensure that clicking a particular instantiation shows
    an appropriate inlay for that instantiation.
    """
    proc_file = """
            proc idk1(x) { return x; }
            idk1(10);
            idk1(10.0);
           """
    iter_file = """
            iter idk1I(x) { yield x; }
            for z in idk1I(10) do {}
            for z in idk1I(10.0) do {}
           """
    # 3 lenses: "Show Generic" + 2 instantiations
    proc_lens = (pos((0, 5)), 3)
    iter_lens = (pos((0, 5)), 3)

    proc_generic_inlays: EXPECTED_INLAYS = []
    proc_int_inlays: EXPECTED_INLAYS = [(pos((0, 12)), ": int(64)", None)]
    proc_real_inlays: EXPECTED_INLAYS = [(pos((0, 12)), ": real(64)", None)]

    iter_generic_inlays: EXPECTED_INLAYS = []
    iter_int_inlays: EXPECTED_INLAYS = [(pos((0, 13)), ": int(64)", None)]
    iter_real_inlays: EXPECTED_INLAYS = [(pos((0, 13)), ": real(64)", None)]

    await click_lenses_and_check_inlays(
        client,
        proc_lens,
        [proc_generic_inlays, proc_int_inlays, proc_real_inlays],
        A=proc_file,
    )
    await click_lenses_and_check_inlays(
        client,
        iter_lens,
        [iter_generic_inlays, iter_int_inlays, iter_real_inlays],
        A=iter_file,
    )


@pytest.mark.asyncio
async def test_fn_type_inlay_header_variants(client: LanguageClient):
    """
    More cases for return inlays.
    """
    file = """
            proc f1() param { return 42; }
            iter f1I() { yield 42; }
            proc f2() throws { return 42; }
            iter f2I() throws { yield 42; }
            proc f3() throws where true do return 42;
            iter f3I() throws where true do yield 42;
            proc f4() where true do return 42;
            iter f4I() where true do yield 42;
            proc f5() param throws where true do return 42;
            iter f5I() throws where true do yield 42;
            proc f6() param where true do return 42;
            iter f6I() where true do yield 42;
           """

    # For 'throws' CLS inserts spaces to sidestep tedious formatting cases.
    inlays = [
        (pos((0, 15)), "int(64)"),
        (pos((1, 10)), "int(64)"),
        (pos((2, 10)), "int(64) "),
        (pos((3, 11)), "int(64) "),
        (pos((4, 10)), "int(64) "),
        (pos((5, 11)), "int(64) "),
        (pos((6, 9)), "int(64)"),
        (pos((7, 10)), "int(64)"),
        (pos((8, 16)), "int(64) "),
        (pos((9, 11)), "int(64) "),
        (pos((10, 15)), "int(64)"),
        (pos((11, 10)), "int(64)"),
    ]
    async with source_file(client, file) as doc:
        await check_type_inlay_hints(
            client, doc, rng((0, 0), endpos(file)), inlays
        )
