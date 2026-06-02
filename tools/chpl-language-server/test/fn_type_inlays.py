"""
Test that function return type inlay hints
"""

import sys
import typing
from dataclasses import dataclass
from enum import Enum
from typing import List, Tuple

from lsprotocol.types import ClientCapabilities
from lsprotocol.types import InitializeParams
import pytest
import pytest_lsp
from pytest_lsp import ClientServerConfig, LanguageClient

from util.utils import *
from util.config import CLS_PATH

# ---------------------------------------------------------------------------
# Function variant testing infrastructure
# ---------------------------------------------------------------------------


class _FnVariant(Enum):
    PROC = "proc"
    ITER = "iter"
    METHOD = "method"
    SECONDARY = "secondary"


@dataclass
class FnDecl:
    """
    Describes a function signature, which will be rendered in multiple
    variants to robustly test return type inlays.
    """

    name: str
    return_expr: str
    params: str = ""  # parameters without enclosing parens
    return_intent: str = ""
    throws: bool = False
    where_expr: str = ""
    yield_expr: str = ""  # defaults to return_expr
    expected_type: str = ""  # expected inlay if any
    explicit_return_type: str = (
        ""  # if set, fn has explicit annotation (expect no inlay)
    )
    do_body: bool = False  # use 'do' instead of '{..}'
    parenless: bool = False  # omit parameter list entirely (parenless function)

    def _render_header(
        self, variant: _FnVariant, record_name: str
    ) -> tuple[str, int]:
        is_iter = variant == _FnVariant.ITER
        kw = "iter" if is_iter else "proc"
        intent = "" if is_iter else self.return_intent
        name = (
            f"{record_name}.{self.name}"
            if variant == _FnVariant.SECONDARY
            else self.name
        )
        prefix = "  " if variant == _FnVariant.METHOD else ""
        param_str = "" if self.parenless else f"({self.params})"
        parts = [f"{prefix}{kw} {name}{param_str}"]
        if self.explicit_return_type:
            parts.append(f": {self.explicit_return_type}")
        if intent:
            parts.append(f" {intent}")
        if self.throws:
            parts.append(" ")

        # Return type goes here. Length so far is where the inlay column should
        # go, so compute it.
        inlay_col = len("".join(parts))

        if self.throws:
            parts.append("throws")
        if self.where_expr:
            parts.append(f" where {self.where_expr}")
        return ("".join(parts), inlay_col)

    def _render_body(self, variant: _FnVariant) -> str:
        is_iter = variant == _FnVariant.ITER
        kw = "yield" if is_iter else "return"
        if is_iter:
            expr = self.yield_expr or self.return_expr
        else:
            expr = self.return_expr
        if self.do_body:
            return f" do {kw} {expr};"
        return " { " + f"{kw} {expr};" + " }"

    def _render(self, variant: _FnVariant, record_name: str) -> tuple[str, int]:
        header, inlay = self._render_header(variant, record_name)
        return (header + self._render_body(variant), inlay)


@dataclass
class FnCall:
    name: str
    args: str = ""  # call arguments without enclosing parens

    def _render(self, variant: _FnVariant, parenless: bool = False) -> str:
        args_str = "" if parenless else f"({self.args})"
        if variant == _FnVariant.ITER:
            return f"for {self.name}{args_str} do {{}}"
        elif variant in (_FnVariant.METHOD, _FnVariant.SECONDARY):
            return f"_r.{self.name}{args_str};"
        else:
            return f"{self.name}{args_str};"


FnPart = typing.Union[FnDecl, FnCall]


def _render_fn_variant(
    parts: List[FnPart],
    variant: _FnVariant,
    record_name: str = "R",
    preamble: List[str] = [],
) -> Tuple[str, list]:
    """
    Render a parts list into (source_string, inlay_list) for the given variant.

    All FnDecl items are rendered as declarations; all FnCall items follow as
    calls. For METHOD, declarations are wrapped inside 'record R { ... }' with
    a 2-space indent. For others (for simplicity), a bare 'record R {}' precedes
    the declarations. Calls to methods get a '_r.' receiver.
    """
    decls = [p for p in parts if isinstance(p, FnDecl)]
    calls = [p for p in parts if isinstance(p, FnCall)]

    # Build a lookup from function name to its FnDecl for call rendering.
    decl_by_name = {d.name: d for d in decls}
    inlays: list = []
    lines: List[str] = preamble.copy()

    def render():
        nonlocal inlays, decls, lines

        for d in decls:
            decl, inlay_col = d._render(variant, record_name)
            lines.append(decl)
            if d.expected_type:
                type_text = d.expected_type + (" " if d.throws else "")
                inlays.append((pos((len(lines) - 1, inlay_col)), type_text))

    lines.append(f"record {record_name} {{")
    if variant == _FnVariant.METHOD:
        render()
    lines.append("}")
    if variant != _FnVariant.METHOD:
        render()

    lines.append(f"var _r: {record_name};")
    for c in calls:
        assert c.name in decl_by_name
        pl = decl_by_name[c.name].parenless
        lines.append(c._render(variant, pl))

    source = "\n".join(lines) + "\n"
    return source, inlays


async def check_fn_variants(
    client: LanguageClient,
    parts: List[FnPart],
    record_name: str = "R",
    preamble=[],
) -> None:
    """
    Run check_type_inlay_hints for all 4 function variants:
    proc, iter, primary method (inside record), and secondary method.
    """
    for variant in _FnVariant:
        source, inlays = _render_fn_variant(
            parts, variant, record_name, preamble
        )
        async with source_file(client, source) as doc:
            await check_type_inlay_hints(
                client, doc, rng((0, 0), endpos(source)), inlays
            )


@pytest_lsp.fixture(
    config=ClientServerConfig(
        server_command=[
            sys.executable,
            CLS_PATH(),
            "--resolver",
            "--return-type-inlays",
            "--generic-fn-type-inlays",
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
    await check_fn_variants(
        client,
        [
            FnDecl(
                "test", return_expr="42", expected_type="int(64)", do_body=True
            )
        ],
    )


@pytest.mark.asyncio
async def test_fn_type_inlay_concrete_real(client: LanguageClient):
    """
    Ensure that a concrete function returning real gets an inlay.
    """
    await check_fn_variants(
        client,
        [FnDecl("foo", return_expr="42.0", expected_type="real(64)")],
    )


@pytest.mark.asyncio
async def test_fn_type_inlay_explicit_no_inlay(client: LanguageClient):
    """
    Ensure that a function with an explicit return type annotation gets no inlay.
    """
    await check_fn_variants(
        client,
        [
            FnDecl(
                "bar", return_expr="1", explicit_return_type="int", do_body=True
            )
        ],
    )


@pytest.mark.asyncio
async def test_fn_type_inlay_generic_identity(client: LanguageClient):
    """
    Ensure that a generic identity function gets a ': x.type' inlay.
    """
    await check_fn_variants(
        client,
        [FnDecl("idk1", params="x", return_expr="x", expected_type="x.type")],
    )


@pytest.mark.asyncio
async def test_fn_type_inlay_generic_const_return(client: LanguageClient):
    """
    Ensure that a generic function with a concrete return value gets the
    common inlay when called with multiple types.
    """
    await check_fn_variants(
        client,
        [
            FnDecl(
                "idk2", params="x", return_expr="42", expected_type="int(64)"
            ),
            FnCall("idk2", "10"),
            FnCall("idk2", "10.0"),
        ],
    )


@pytest.mark.asyncio
async def test_fn_type_inlay_generic_pair(client: LanguageClient):
    """
    Ensure that a generic function returning a tuple of formals gets the right inlay.
    """
    await check_fn_variants(
        client,
        [
            FnDecl(
                "mkPair",
                params="x, y",
                return_expr="(x, y)",
                expected_type="(x.type, y.type)",
                do_body=True,
            )
        ],
    )


@pytest.mark.asyncio
async def test_fn_type_inlay_type_query_list(client: LanguageClient):
    """
    Ensure that a generic function with a type query in a formal annotation
    gets the right inlay using the type query name.
    """
    await check_fn_variants(
        client,
        [
            FnDecl(
                "foo",
                params="x: list(?eltType)",
                return_expr="x.first",
                expected_type="eltType",
                do_body=True,
            )
        ],
        preamble=["use List;"],
    )


@pytest.mark.asyncio
async def test_fn_type_inlay_type_query_via_type_proc(client: LanguageClient):
    """
    Ensure that a generic function whose formal type uses a type proc with a
    type query gets the right inlay (exercises usePlaceholders propagation).
    """
    await check_fn_variants(
        client,
        [
            FnDecl(
                "weird",
                params="x: id(?hmm)",
                return_expr="x",
                expected_type="hmm",
                do_body=True,
            )
        ],
        preamble=["proc id(type x) type : ? do return x;"],
    )


@pytest.mark.asyncio
async def test_fn_type_inlay_return_intent_type(client: LanguageClient):
    """
    Ensure return type annotations for functions with explicit return intents.
    """
    await check_fn_variants(
        client,
        [
            FnDecl(
                "bloop",
                return_intent="const",
                return_expr="42",
                expected_type="int(64)",
            ),
            FnDecl(
                "baz",
                return_intent="type",
                return_expr="int",
                yield_expr="42",
                expected_type="int(64)",
            ),
        ],
    )


@pytest.mark.asyncio
async def test_fn_type_inlay_where_clause(client: LanguageClient):
    """
    Ensure that a function with a 'where' clause gets an inlay in the right
    spot.
    """
    await check_fn_variants(
        client,
        [
            FnDecl(
                "bloop",
                return_intent="const",
                where_expr="true",
                return_expr="42",
                expected_type="int(64)",
            ),
            FnDecl(
                "boop",
                return_intent="type",
                where_expr="true",
                return_expr="int",
                yield_expr="42",
                expected_type="int(64)",
            ),
        ],
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

    proc_generic_inlays: EXPECTED_INLAYS = [(pos((0, 12)), ": x.type", None)]
    proc_int_inlays: EXPECTED_INLAYS = [(pos((0, 12)), ": int(64)", None)]
    proc_real_inlays: EXPECTED_INLAYS = [(pos((0, 12)), ": real(64)", None)]

    iter_generic_inlays: EXPECTED_INLAYS = [(pos((0, 13)), ": x.type", None)]
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
    More cases for return inlays: param intent, throws, where, and combinations.
    """
    # For 'throws', CLS inserts a trailing space to sidestep formatting issues.
    await check_fn_variants(
        client,
        [
            # param intent, braces body
            FnDecl(
                "f1",
                return_intent="param",
                return_expr="42",
                expected_type="int(64)",
            ),
            # throws, braces body
            FnDecl(
                "f2", throws=True, return_expr="42", expected_type="int(64)"
            ),
            # throws + where, do body
            FnDecl(
                "f3",
                throws=True,
                where_expr="true",
                return_expr="42",
                expected_type="int(64)",
                do_body=True,
            ),
            # where, do body
            FnDecl(
                "f4",
                where_expr="true",
                return_expr="42",
                expected_type="int(64)",
                do_body=True,
            ),
            # param intent + throws + where, do body
            FnDecl(
                "f5",
                return_intent="param",
                throws=True,
                where_expr="true",
                return_expr="42",
                expected_type="int(64)",
                do_body=True,
            ),
            # param intent + where, do body
            FnDecl(
                "f6",
                return_intent="param",
                where_expr="true",
                return_expr="42",
                expected_type="int(64)",
                do_body=True,
            ),
        ],
    )


@pytest.mark.asyncio
async def test_fn_type_inlay_parenless(client: LanguageClient):
    """
    Ensure that parenless functions get an inlay at the right spot
    (right after the function name, with no parentheses in the declaration).
    """
    await check_fn_variants(
        client,
        [
            FnDecl(
                "foo", return_expr="42", expected_type="int(64)", parenless=True
            )
        ],
    )
