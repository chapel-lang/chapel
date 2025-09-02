"""
Tests basic functionality, including autocompletion, go-to-definition, hover,
and references
"""

import sys

from lsprotocol.types import ClientCapabilities
from lsprotocol.types import CompletionList, CompletionParams
from lsprotocol.types import ReferenceParams, ReferenceContext
from lsprotocol.types import InitializeParams
from lsprotocol.types import HoverParams, Hover, MarkupContent
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
async def test_global_completion(client: LanguageClient):
    """
    Test top-level autocompletion.

    All completion is based entirely on global symbols right now; thus,
    anywhere you try, you should get the same results.
    """

    positions = [pos((n, 0)) for n in range(1, 5)]
    file = """
           for i in 1..10 {
             for j in 1..10 {
               writeln("Hello, world!", (i, j));
             }
           }
           """

    global_symbols = ["here", "strideKind", "boundKind", "Locales"]

    async with source_file(client, file) as doc:
        for position in positions:
            results = await client.text_document_completion_async(
                params=CompletionParams(position=position, text_document=doc)
            )

            # spec: it could be a completion item list, or a CompletionList,
            # or null. We care about the list of items.
            assert results is not None
            if isinstance(results, CompletionList):
                results = results.items
            result_names = [r.label for r in results]

            for symbol in global_symbols:
                assert symbol in result_names


@pytest.mark.asyncio
async def test_go_to_definition_simple(client: LanguageClient):
    """
    Basic test cases for go-to-definition.
    """

    file = """

           var x: int = 5;
           var y: int = x;
           var z: int = x + y;
           {
             var x = 10;
             var y = x;
           }
           """

    async with source_file(client, file) as doc:
        # Definitions link to themselves
        await check_goto_decl_def(client, doc, pos((0, 4)), pos((0, 4)))
        await check_goto_decl_def(client, doc, pos((1, 4)), pos((1, 4)))
        await check_goto_decl_def(client, doc, pos((2, 4)), pos((2, 4)))
        await check_goto_decl_def(client, doc, pos((4, 6)), pos((4, 6)))
        await check_goto_decl_def(client, doc, pos((5, 6)), pos((5, 6)))

        # References link to their definitions
        await check_goto_decl_def(client, doc, pos((1, 13)), pos((0, 4)))
        await check_goto_decl_def(client, doc, pos((2, 13)), pos((0, 4)))
        await check_goto_decl_def(client, doc, pos((2, 17)), pos((1, 4)))
        await check_goto_decl_def(client, doc, pos((5, 10)), pos((4, 6)))

        # Other things don't go anywhere
        # 'var' tokens
        await check_goto_decl_def(client, doc, pos((0, 2)), None)
        await check_goto_decl_def(client, doc, pos((1, 2)), None)
        await check_goto_decl_def(client, doc, pos((2, 2)), None)
        await check_goto_decl_def(client, doc, pos((4, 4)), None)
        await check_goto_decl_def(client, doc, pos((5, 4)), None)
        # numbers
        await check_goto_decl_def(client, doc, pos((0, 13)), None)
        await check_goto_decl_def(client, doc, pos((4, 10)), None)


@pytest.mark.asyncio
async def test_go_to_definition_use_standard(client: LanguageClient):
    """
    Ensure that go-to-definition works on standard module symbols
    that are used or imported.
    """

    file = """
           use IO;
           use List, Map;
           import Time;
           """

    mod_IO = standard_module("IO")
    mod_List = standard_module("List")
    mod_Map = standard_module("Map")
    mod_Time = standard_module("Time")

    async with source_file(client, file) as doc:
        await check_goto_decl_def_module(client, doc, pos((0, 4)), mod_IO)
        await check_goto_decl_def_module(client, doc, pos((0, 4)), mod_IO)
        await check_goto_decl_def_module(client, doc, pos((1, 4)), mod_List)
        await check_goto_decl_def_module(client, doc, pos((1, 10)), mod_Map)
        await check_goto_decl_def_module(client, doc, pos((2, 8)), mod_Time)


@pytest.mark.asyncio
async def test_go_to_definition_use_across_modules(client: LanguageClient):
    """
    Ensure that go-to-definition works on symbols that reference other modules
    """

    fileA = """
            module A {
              var x = 42;
            }
            """
    fileB = """
            module B {
              use A;
              var y = x;
            }
            """

    async def check(docs):
        docA = docs("A")
        docB = docs("B")

        await check_goto_decl_def_module(client, docB, pos((1, 6)), docA)
        await check_goto_decl_def(
            client, docB, pos((2, 10)), (docA, pos((1, 6)))
        )

    async with source_files(client, A=fileA, B=fileB) as docs:
        await check(docs)

    async with unrelated_source_files(client, A=fileA, B=fileB) as docs:
        await check(docs)


@pytest.mark.asyncio
async def test_go_to_definition_standard_rename(client: LanguageClient):
    """
    Ensure that go-to-definition works with using/importing symbols,
    even if renaming is in use.
    """

    file = """
           use IO as OI;
           import IO.{ioMode as im};
           use List only list;
           """

    mod_IO = standard_module("IO")
    mod_List = standard_module("List")

    async with source_file(client, file) as doc:
        await check_goto_decl_def_module(client, doc, pos((0, 4)), mod_IO)
        await check_goto_decl_def_module(client, doc, pos((0, 10)), mod_IO)
        await check_goto_decl_def_module(client, doc, pos((1, 7)), mod_IO)
        await check_goto_decl_def(
            client, doc, pos((1, 11)), mod_IO, expect_str="enum ioMode"
        )
        await check_goto_decl_def(
            client, doc, pos((1, 21)), mod_IO, expect_str="enum ioMode"
        )
        await check_goto_decl_def_module(client, doc, pos((2, 4)), mod_List)
        await check_goto_decl_def(
            client, doc, pos((2, 14)), mod_List, expect_str="record list"
        )


@pytest.mark.asyncio
async def test_go_to_record_def(client: LanguageClient):
    """
    Ensure that 'go to definition' on a type actually works.
    """

    file = """
           record myRec {}
           var x: myRec;
           var y = new myRec();
           """

    async with source_file(client, file) as doc:
        await check_goto_decl_def(client, doc, pos((0, 7)), pos((0, 7)))
        await check_goto_decl_def(client, doc, pos((1, 7)), pos((0, 7)))
        await check_goto_decl_def(client, doc, pos((2, 12)), pos((0, 7)))


@pytest.mark.asyncio
async def test_go_to_string_type(client: LanguageClient):
    """
    Ensure that 'go to definition' on a type actually works.
    """

    file = """
           var z: string;
           """

    mod_String = internal_module("String")

    async with source_file(client, file) as doc:
        await check_goto_decl_def(
            client, doc, pos((0, 7)), mod_String, expect_str="record _string"
        )


@pytest.mark.asyncio
async def test_list_references(client: LanguageClient):
    """
    Basic 'list references' test with shadowing and scopes.
    """

    file = """
           var x = 42;
           var y = x;
           for i in 1..10 {
                var z = x;
                var y = 42 + i;

                while true {
                    var i = 0;
                    var j = i + 1;
                }
           }
           """

    async with source_file(client, file) as doc:
        # 'find references' on definitions;
        # the cross checking will also validate the references.
        await check_references_and_cross_check(
            client, doc, pos((0, 4)), [pos((0, 4)), pos((1, 8)), pos((3, 13))]
        )
        await check_references_and_cross_check(
            client, doc, pos((1, 4)), [pos((1, 4))]
        )
        await check_references_and_cross_check(
            client, doc, pos((2, 4)), [pos((2, 4)), pos((4, 18))]
        )
        await check_references_and_cross_check(
            client, doc, pos((3, 9)), [pos((3, 9))]
        )
        await check_references_and_cross_check(
            client, doc, pos((4, 9)), [pos((4, 9))]
        )
        await check_references_and_cross_check(
            client, doc, pos((7, 13)), [pos((7, 13)), pos((8, 17))]
        )
        await check_references_and_cross_check(
            client, doc, pos((8, 13)), [pos((8, 13))]
        )


@pytest.mark.asyncio
async def test_list_references_across_files(client: LanguageClient):
    """
    Ensure that list-references works across multiple user-defined files.
    """

    fileA = """
            module A {
              var x = 42;
            }
            """
    fileB = """
            module B {
              use A;

              var y = x;
            }
            """
    fileC = """
            module C {
              import A.x;

              var z = x;
            }
            """
    async with source_files(client, A=fileA, B=fileB, C=fileC) as docs:
        all_refs = [
            (docs("A"), pos((1, 6))),
            (docs("B"), pos((3, 10))),
            (docs("C"), pos((3, 10))),
        ]

        await check_references_and_cross_check(
            client, docs("A"), pos((1, 6)), all_refs
        )

        await save_file(client, docs("A"), docs("B"), docs("C"))
        assert len(client.diagnostics[docs("A").uri]) == 0
        assert len(client.diagnostics[docs("B").uri]) == 0
        assert len(client.diagnostics[docs("C").uri]) == 0


@pytest.mark.asyncio
@pytest.mark.xfail
async def test_list_references_standard(client: LanguageClient):
    """
    Doesn't seem to work now; would require loading and processing entire
    standard library to find all references to a standard symbol.
    """

    file = """
           use IO;
           type myType = ioMode;
           """

    async with source_file(client, file) as doc:
        references = await client.text_document_references_async(
            params=ReferenceParams(
                text_document=doc,
                position=pos((1, 14)),
                context=ReferenceContext(include_declaration=True),
            )
        )

        assert references is not None
        assert len(references) > 10


@pytest.mark.asyncio
async def test_go_to_def_inherit(client: LanguageClient):
    """
    Test cases for go-to-definition on inherit expression.
    """

    file = """
            module foo {
            interface II { }
            record R: II { }

            class CC { }
            class C: CC, II { }

            module M {
              class CC {}
            }
            class C2: M.CC { }
            }
           """

    async with source_file(client, file) as doc:
        # Definitions link to themselves
        await check_goto_decl_def(client, doc, pos((0, 7)), pos((0, 7)))
        await check_goto_decl_def(client, doc, pos((1, 10)), pos((1, 10)))
        await check_goto_decl_def(client, doc, pos((2, 7)), pos((2, 7)))
        await check_goto_decl_def(client, doc, pos((4, 6)), pos((4, 6)))
        await check_goto_decl_def(client, doc, pos((5, 6)), pos((5, 6)))
        await check_goto_decl_def(client, doc, pos((7, 7)), pos((7, 7)))
        await check_goto_decl_def(client, doc, pos((8, 8)), pos((8, 8)))
        await check_goto_decl_def(client, doc, pos((10, 6)), pos((10, 6)))

        # check the inherit expressions
        await check_goto_decl_def(client, doc, pos((2, 10)), pos((1, 10)))
        await check_goto_decl_def(client, doc, pos((5, 9)), pos((4, 6)))
        await check_goto_decl_def(client, doc, pos((5, 13)), pos((1, 10)))
        await check_goto_decl_def(client, doc, pos((10, 10)), pos((7, 7)))
        await check_goto_decl_def(client, doc, pos((10, 12)), pos((8, 8)))


@pytest.mark.asyncio
async def test_go_to_enum(client: LanguageClient):
    """
    Test cases for go-to-definition on enums.
    """

    file = """
            proc bar param do return 1;
            enum A {
              a = bar,
              b = 1,
            }
            var e = A.a;
           """

    async with source_file(client, file) as doc:
        # Definitions link to themselves
        await check_goto_decl_def(client, doc, pos((0, 5)), pos((0, 5)))
        await check_goto_decl_def(client, doc, pos((1, 5)), pos((1, 5)))
        await check_goto_decl_def(client, doc, pos((2, 2)), pos((2, 2)))
        await check_goto_decl_def(client, doc, pos((3, 2)), pos((3, 2)))
        await check_goto_decl_def(client, doc, pos((5, 4)), pos((5, 4)))

        # check bar
        await check_goto_decl_def(client, doc, pos((2, 6)), pos((0, 5)))
        # check A.a
        await check_goto_decl_def(client, doc, pos((5, 8)), pos((1, 5)))
        await check_goto_decl_def(client, doc, pos((5, 10)), pos((2, 2)))


@pytest.mark.asyncio
async def test_hover(client: LanguageClient):
    """
    Ensure that `: string` as a type inlay does not break
    """

    file = """
            module M {
              iter myIter() { }
              proc myFunc(myFormal: int) { }
              operator +(x, y) do return 1;
              var myVar: int;
              config const myConfig: int;

              record myRec {
                iter myMethodIter() { }
                proc myMethod(myFormal: real(32)) { }
                operator *(x, y) do return 1;
                var myField: int;
              }

              class myClass {
                proc init() { }
                proc deinit() { }
                proc init=(other) { }
                operator :(x, type t) { }
              }

              proc myRec.secondary(myFormal) { }
              proc getType() type { return myRec; }
              proc (getType()).secondary2(myFormal) { }

              proc takesTuples(myTup: (int, real) = (1, 2.0)) { }
              proc takesTuples2(myTup: 2 * int) { }
            }
           """

    hovers = [
        ("module M", rng(pos((0, 7)), pos((0, 8))), pos((0, 7))),
        ("iter myIter()", rng(pos((1, 7)), pos((1, 13))), pos((1, 8))),
        (
            "proc myFunc(myFormal: int)",
            rng(pos((2, 7)), pos((2, 13))),
            pos((2, 9)),
        ),
        ("myFormal: int", rng(pos((2, 14)), pos((2, 22))), pos((2, 15))),
        ("operator +(x, y)", rng(pos((3, 11)), pos((3, 12))), pos((3, 11))),
        ("var myVar: int", rng(pos((4, 6)), pos((4, 11))), pos((4, 8))),
        (
            "config const myConfig: int",
            rng(pos((5, 15)), pos((5, 23))),
            pos((5, 15)),
        ),
        ("record myRec", rng(pos((7, 9)), pos((7, 14))), pos((7, 10))),
        ("iter myMethodIter()", rng(pos((8, 9)), pos((8, 21))), pos((8, 18))),
        (
            "proc myMethod(myFormal: real(32))",
            rng(pos((9, 9)), pos((9, 17))),
            pos((9, 11)),
        ),
        ("myFormal: real(32)", rng(pos((9, 18)), pos((9, 26))), pos((9, 22))),
        ("operator *(x, y)", rng(pos((10, 13)), pos((10, 14))), pos((10, 13))),
        ("var myField: int", rng(pos((11, 8)), pos((11, 15))), pos((11, 8))),
        ("class myClass", rng(pos((14, 8)), pos((14, 15))), pos((14, 8))),
        ("proc init()", rng(pos((15, 9)), pos((15, 13))), pos((15, 12))),
        ("proc deinit()", rng(pos((16, 9)), pos((16, 15))), pos((16, 12))),
        ("proc init=(other)", rng(pos((17, 9)), pos((17, 14))), pos((17, 12))),
        (
            "operator :(x, type t)",
            rng(pos((18, 13)), pos((18, 14))),
            pos((18, 14)),
        ),
        (
            "proc myRec.secondary(myFormal)",
            rng(pos((21, 13)), pos((21, 22))),
            pos((21, 13)),
        ),
        # myRec is a use, so the hover gives us the location of the def
        ("record myRec", rng(pos((7, 9)), pos((7, 14))), pos((21, 12))),
        ("myFormal", rng(pos((21, 23)), pos((21, 31))), pos((21, 31))),
        (
            "proc getType() type",
            rng(pos((22, 7)), pos((22, 14))),
            pos((22, 9)),
        ),
        (
            "proc (getType()).secondary2(myFormal)",
            rng(pos((23, 19)), pos((23, 29))),
            pos((23, 19)),
        ),
        # getType() is a use, so the hover gives us the location of the def
        ("proc getType() type", rng(pos((22, 7)), pos((22, 14))), pos((23, 9))),
        # (getType()) is just an expression, so it just shows the text
        ("(getType())", rng(pos((23, 7)), pos((23, 18))), pos((23, 7))),
        (
            "takesTuples(myTup: (int, real) = (1, 2.0))",
            rng(pos((25, 7)), pos((25, 18))),
            pos((25, 9)),
        ),
        (
            "myTup: (int, real) = (1, 2.0)",
            rng(pos((25, 19)), pos((25, 24))),
            pos((25, 24)),
        ),
        (
            "takesTuples2(myTup: 2 * int)",
            rng(pos((26, 7)), pos((26, 19))),
            pos((26, 9)),
        ),
        ("myTup: 2 * int", rng(pos((26, 20)), pos((26, 25))), pos((26, 25))),
    ]

    async with source_file(client, file) as doc:

        for expected_text, expected_rng, p in hovers:
            # try to hover over the inlay/inlay location, make sure it does something
            actual = await client.text_document_hover_async(
                params=HoverParams(doc, p)
            )
            print(actual)
            assert actual is not None
            assert expected_rng == actual.range
            assert isinstance(actual.contents, MarkupContent)
            assert expected_text in actual.contents.value
