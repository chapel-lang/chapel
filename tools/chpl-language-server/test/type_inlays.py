"""
Test that type inlays show up properly, and that they can function as
clickable links. Requires `--resolver`.
"""

import sys

from lsprotocol.types import ClientCapabilities
from lsprotocol.types import HoverParams
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
            "--type-inlays",
            "--no-literal-arg-inlays",
            "--no-param-inlays",
            "--end-markers=none",
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


@pytest.mark.asyncio
async def test_type_inlays_prim(client: LanguageClient):
    """
    Ensure that type inlays are shown for primitive types.
    """

    file = """
            var a = 10;
            var b: int = 10;
            var c = a:int(32);
            var d = 0.0;

            var e = "hello";
            var f = true;

            var g;
            if false then
              g = 10;
            else
              g = 20.0;
           """

    inlays = [
        (pos((0, 5)), "int(64)"),
        (pos((2, 5)), "int(32)"),
        (pos((3, 5)), "real(64)"),
        (pos((5, 5)), "string"),
        (pos((6, 5)), "bool"),
        (pos((8, 5)), "real(64)"),
    ]

    async with source_file(client, file) as doc:
        await check_type_inlay_hints(
            client, doc, rng((0, 0), endpos(file)), inlays
        )

        # check that specifying a range works
        await check_type_inlay_hints(
            client, doc, rng((0, 0), (3, 0)), inlays[:2]
        )


@pytest.mark.asyncio
@pytest.mark.xfail
async def test_type_inlays_user_defined_types(client: LanguageClient):
    """
    Ensure that type inlays are shown properly for user-defined types.
    """
    file = """
            {at} Concrete {{
              var x: int;
            }}
            {at} Generic {{
              type t;
              var x: t;
              proc init(type t) {{
                this.t = t;
              }}
            }}
            var a = new Concrete();
            var b = a;

            var c = new Generic(int);
            var d = c;

            var e = new Generic(Concrete);
            var f = e;

            var g = new Generic(Generic(string));
            var h = g;
           """

    inlay_pos = [
        (pos((10, 5)), "{m}Concrete"),
        (pos((11, 5)), "{m}Concrete"),
        (pos((13, 5)), "{m}Generic(int)"),
        (pos((14, 5)), "{m}Generic(int)"),
        (pos((16, 5)), "{m}Generic({m}Concrete)"),
        (pos((17, 5)), "{m}Generic({m}Concrete)"),
        (pos((19, 5)), "{m}Generic({m}Generic(string))"),
        (pos((20, 5)), "{m}Generic({m}Generic(string))"),
    ]

    variants = [("record", ""), ("class", "owned ")]

    for at, management in variants:
        async with source_file(client, file.format(at=at)) as doc:
            inlays = [
                (pos, typename.format(m=management))
                for pos, typename in inlay_pos
            ]
            await check_type_inlay_hints(
                client, doc, rng((0, 0), endpos(file)), inlays
            )
            await save_file(client, doc)
            assert len(client.diagnostics[doc.uri]) == 0


@pytest.mark.asyncio
async def test_type_inlays_tuple(client: LanguageClient):
    """
    Ensure that type inlays are shown for tuples
    """

    file = """
            record R { }
            var a = (10, 20);
            var b = ((10:uint(8), ""), new R());
            var c = b;
            var (d, f) = (1, (2.0, new R()));
            var (e, (g, h)) = (false, (new R(), a));
           """

    inlays = [
        (pos((1, 5)), "(int(64), int(64))"),
        (pos((2, 5)), "((uint(8), string), R)"),
        (pos((3, 5)), "((uint(8), string), R)"),
        (pos((4, 6)), "int(64)"),
        (pos((4, 9)), "(real(64), R)"),
        (pos((5, 6)), "bool"),
        (pos((5, 10)), "R"),
        (pos((5, 13)), "(int(64), int(64))"),
    ]

    async with source_file(client, file) as doc:
        await check_type_inlay_hints(
            client, doc, rng((0, 0), endpos(file)), inlays
        )


@pytest.mark.asyncio
@pytest.mark.xfail
async def test_type_inlays_range(client: LanguageClient):
    """
    Ensure that type inlays are shown for ranges
    """

    file = """
            var r1 = 1..10;
            var r2 = 1..<10;
            var r3 = 1..;
            var r4 = ..10;
            var r5 = ..;

            var r6 = 1..10 by 10;
            var r7 = 1..10 by -10;
            var r8 = 1..10 by -1;
            var r9 = 1..10 by 1;

            var r10 = 1:int(32)..10:int(32);
           """

    inlays = [
        (pos((0, 6)), "range(int(64), boundKind.both, strideKind.one)"),
        (pos((1, 6)), "range(int(64), boundKind.both, strideKind.one)"),
        (pos((2, 6)), "range(int(64), boundKind.low, strideKind.one)"),
        (pos((3, 6)), "range(int(64), boundKind.high, strideKind.one)"),
        (pos((4, 6)), "range(int(64), boundKind.neither, strideKind.one)"),
        (pos((6, 6)), "range(int(64), boundKind.both, strideKind.positive)"),
        (pos((7, 6)), "range(int(64), boundKind.both, strideKind.negative)"),
        (pos((8, 6)), "range(int(64), boundKind.both, strideKind.negOne)"),
        (pos((9, 6)), "range(int(64), boundKind.both, strideKind.one)"),
        (pos((10, 7)), "range(int(32), boundKind.both, strideKind.one)"),
    ]

    async with source_file(client, file) as doc:
        await check_type_inlay_hints(
            client, doc, rng((0, 0), endpos(file)), inlays
        )


@pytest.mark.asyncio
@pytest.mark.xfail
async def test_type_inlays_arrays(client: LanguageClient):
    """
    Ensure that type inlays are shown for arrays
    """

    file = """
            var a = [1, 2, 3];
            var b: [1..10] real;
            var c = b;
           """

    inlays = [
        (pos((0, 5)), "[0..2] int(64)"),
        (pos((2, 5)), "[1..10] real(64)"),
    ]

    async with source_file(client, file) as doc:
        await check_type_inlay_hints(
            client, doc, rng((0, 0), endpos(file)), inlays
        )


@pytest.mark.asyncio
@pytest.mark.xfail
async def test_type_inlays_domains(client: LanguageClient):
    """
    Ensure that type inlays are shown for domains
    """

    file = """
            var a = {1..10};
            var b: domain(1, int);
            var c = b;
           """

    inlays = [
        (pos((0, 5)), "domain(1, int(64), strideKind.one)"),
        (pos((2, 5)), "domain(1, int(64), strideKind.one)"),
    ]

    async with source_file(client, file) as doc:
        await check_type_inlay_hints(
            client, doc, rng((0, 0), endpos(file)), inlays
        )


@pytest.mark.asyncio
async def test_type_inlays_loops(client: LanguageClient):
    """
    Ensure that type inlays are shown for loops
    """

    file = """
            for i in 1..10 { }
            forall i in 1..10 { }
            coforall i in 1..10 { }
            foreach i in 1..10 { }
            [i in 1..10] { ; }
           """

    inlays = [
        (pos((0, 5)), "int(64)", False),
        (pos((1, 8)), "int(64)", False),
        (pos((2, 10)), "int(64)", False),
        (pos((3, 9)), "int(64)", False),
        (pos((4, 2)), "int(64)", False),
    ]

    async with source_file(client, file) as doc:
        await check_type_inlay_hints(
            client, doc, rng((0, 0), endpos(file)), inlays
        )


@pytest.mark.asyncio
async def test_type_inlays_return(client: LanguageClient):
    """
    Ensure that type inlays are inferred from return types on simple functions.
    """

    file = """
            class C { }
            proc bar() do return new C?();
            var b = bar();

            proc foo() do return 1..10;
            for i in foo() { }
           """

    inlays = [
        (pos((2, 5)), "owned C?"),
        (pos((5, 5)), "int(64)", False),
    ]

    async with source_file(client, file) as doc:
        await check_type_inlay_hints(
            client, doc, rng((0, 0), endpos(file)), inlays
        )


@pytest.mark.asyncio
@pytest.mark.xfail
async def test_type_inlays_yield(client: LanguageClient):
    """
    Ensure that type inlays are inferred from yield types on simple functions.
    """

    file = """
            iter foo() {
              yield 1;
              yield 2;
              yield 3;
            };
            var a = foo();
           """

    inlays = [(pos((5, 5)), "[domain(1, int(64), strideKind.one)] int(64)")]

    async with source_file(client, file) as doc:
        await check_type_inlay_hints(
            client, doc, rng((0, 0), endpos(file)), inlays
        )


@pytest.mark.asyncio
async def test_type_inlays_clickable_def(client: LanguageClient):
    """
    Ensure that type inlays are clickable to their definitions.
    """

    file = """
            record R { }

            var x = new R();
           """

    async with source_file(client, file) as doc:
        inlays = await check_type_inlay_hints(
            client, doc, rng((0, 0), endpos(file)), [(pos((2, 5)), "R")]
        )

        assert len(inlays) == 1
        # the second inlay part is the definition and should be clickable
        assert isinstance(inlays[0].label, list) and len(inlays[0].label) == 2

        loc = inlays[0].label[1].location
        assert loc is not None
        assert loc.uri == doc.uri
        assert loc.range == rng((0, 7), (0, 8))


@pytest.mark.asyncio
async def test_type_inlays_hover_string(client: LanguageClient):
    """
    Ensure that `: string` as a type inlay does not break
    """

    file = """
            var s = "hello";
           """

    async with source_file(client, file) as doc:
        inlays = await check_type_inlay_hints(
            client, doc, rng((0, 0), endpos(file)), [(pos((0, 5)), "string")]
        )

        assert len(inlays) == 1
        # the second inlay part is the definition and should be clickable
        assert isinstance(inlays[0].label, list) and len(inlays[0].label) == 2

        loc = inlays[0].label[1].location
        assert loc is not None
        assert loc.uri == internal_module("String").uri

        # try to hover over the inlay/inlay location, make sure it does something
        hover = await client.text_document_hover_async(
            params=HoverParams(internal_module("String"), loc.range.start)
        )
        assert hover is not None


@pytest.mark.asyncio
async def test_type_implicit_this(client: LanguageClient):
    """
    Ensure that no type inlays are shown for implicit this
    """
    file = """
            {} C {{
              var x: int;
              proc init(x: int) {{
                this.x = x;
              }}
              proc foo() {{
                var y = this; // y should have a type inlay
              }}
            }}
           """

    y_inlay_pos = pos((6, 9))

    variants = [("record", "C"), ("class", "borrowed C")]

    for at, typename in variants:
        async with source_file(client, file.format(at)) as doc:
            await check_type_inlay_hints(
                client,
                doc,
                rng((0, 0), endpos(file)),
                [(y_inlay_pos, typename)],
            )


@pytest.mark.asyncio
@pytest.mark.xfail
async def test_split_init_type_inlays(client: LanguageClient):
    """
    Ensure that type inlays are shown properly for split init on generics
    """

    file = """
            record Generic {
              type t;
              var x: t;
              proc init(type t) {
                this.t = t;
              }
            }

            var x: Generic(?);
            x = new Generic(int);
            var y = x; // should be Generic(int)
           """

    y_inlay = (pos((10, 5)), "Generic(int)")
    async with source_file(client, file) as doc:
        await check_type_inlay_hints(
            client, doc, rng((0, 0), endpos(file)), [y_inlay]
        )


@pytest.mark.asyncio
async def test_type_inlay_type_variable(client: LanguageClient):
    """
    Ensure that type inlays are shown properly for type variables
    """

    file = """
            proc bar(type t) type do return t;
            type t = bar(int);
           """

    y_inlay = (pos((1, 6)), "int(64)")
    async with source_file(client, file) as doc:
        await check_type_inlay_hints(
            client, doc, rng((0, 0), endpos(file)), [y_inlay]
        )
