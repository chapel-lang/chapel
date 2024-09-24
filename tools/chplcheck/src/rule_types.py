#
# Copyright 2024 Hewlett Packard Enterprise Development LP
# Other additional copyright holders may be indicated within.
#
# The entirety of this work is licensed under the Apache License,
# Version 2.0 (the "License"); you may not use this file except
# in compliance with the License.
#
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

import typing

import chapel
from abc import ABCMeta, abstractmethod
from fixits import Fixit, Edit


def _build_ignore_fixit(
    anchor: chapel.AstNode, lines: typing.List[str], rule_name: str
) -> Fixit:
    # TODO: how should this handle multiple ignores?
    loc = anchor.location()
    text = chapel.range_to_text(loc, lines)
    indent_amount = max(loc.start()[1] - 1, 0)
    indent = " " * indent_amount
    text = f'@chplcheck.ignore("{rule_name}")\n' + indent + text
    ignore = Fixit.build(Edit.build(loc, text))
    ignore.description = "Ignore this warning"
    return ignore


class BasicRuleResult:
    """
    Result type for basic rules. Rules can also return a plain boolean to
    represent a simple pass/fail result, with no fixit.
    """

    def __init__(
        self,
        node: chapel.AstNode,
        ignorable: bool = False,
        fixits: typing.Optional[typing.Union[Fixit, typing.List[Fixit]]] = None,
        data: typing.Optional[typing.Any] = None,
    ):
        self.node = node
        self.ignorable = ignorable
        if fixits is None:
            self._fixits = []
        elif isinstance(fixits, Fixit):
            self._fixits = [fixits]
        else:
            self._fixits = fixits
        self.data = data

    def add_fixits(self, *fixits: Fixit):
        """
        Add fixits to the result.
        """
        self._fixits.extend(fixits)

    def fixits(self, context: chapel.Context, name: str) -> typing.List[Fixit]:
        """
        Get the fixits associated with this result. Also builds an ignore fixit, if necessary.
        """
        to_return = self._fixits
        if self.ignorable:
            lines = chapel.get_file_lines(context, self.node)
            ignore = _build_ignore_fixit(self.node, lines, name)
            to_return.append(ignore)
        return to_return


_BasicRuleResult = typing.Union[bool, BasicRuleResult]
"""The type of values that can be returned by basic rule functions"""


BasicRuleCheck = typing.Callable[
    [chapel.Context, chapel.AstNode], _BasicRuleResult
]
"""Function type for basic rules; (context, node) -> _BasicRuleResult"""


class AdvancedRuleResult:
    """
    Result type for advanced rules. Advanced rules can also return a plain
    boolean to represent a simple pass/fail result, with no fixit. Having an anchor implies that it is ignorable
    """

    def __init__(
        self,
        node: chapel.AstNode,
        anchor: typing.Optional[chapel.AstNode] = None,
        fixits: typing.Optional[typing.Union[Fixit, typing.List[Fixit]]] = None,
        data: typing.Optional[typing.Any] = None,
    ):
        self.node = node
        self.anchor = anchor
        if fixits is None:
            self._fixits = []
        elif isinstance(fixits, Fixit):
            self._fixits = [fixits]
        else:
            self._fixits = fixits
        self.data = data

    def fixits(self, context: chapel.Context, name: str) -> typing.List[Fixit]:
        """
        Get the fixits associated with this result. Also builds an ignore fixit, if necessary.
        """
        to_return = self._fixits
        if self.anchor is not None:
            lines = chapel.get_file_lines(context, self.anchor)
            ignore = _build_ignore_fixit(self.anchor, lines, name)
            to_return.append(ignore)
        return to_return


_AdvancedRuleResult = typing.Iterator[
    typing.Union[chapel.AstNode, AdvancedRuleResult]
]
"""Internal type for advanced rule results"""


AdvancedRuleCheck = typing.Callable[
    [chapel.Context, chapel.AstNode], _AdvancedRuleResult
]
"""Function type for advanced rules"""


RuleResult = typing.Union[_BasicRuleResult, _AdvancedRuleResult]
"""Union type for all rule results"""

CheckResult = typing.Tuple[chapel.AstNode, str, typing.List[Fixit]]


VarResultType = typing.TypeVar("VarResultType")

FixitHook = typing.Callable[
    [chapel.Context, VarResultType],
    typing.Optional[typing.Union[Fixit, typing.List[Fixit]]],
]
"""
Function type for fixits; (context, data) -> None or Fixit or List[Fixit]
"""


class Rule(typing.Generic[VarResultType], metaclass=ABCMeta):
    # can't specify type of driver due to circular import
    def __init__(self, driver, name: str) -> None:
        self.driver = driver
        self.name = name
        self.fixit_funcs: typing.List[FixitHook[VarResultType]] = []

    def _fixup_description_for_fixit(
        self, fixit: Fixit, fixit_func: FixitHook
    ) -> None:
        if fixit.description is not None:
            return
        if fixit_func.__doc__ is not None:
            fixit.description = fixit_func.__doc__.strip()

    def run_fixit_hooks(
        self, context: chapel.Context, result: VarResultType
    ) -> typing.List[Fixit]:
        fixits_from_hooks = []
        for fixit_func in self.fixit_funcs:
            extra_fixes = fixit_func(context, result)
            if extra_fixes is None:
                continue

            if isinstance(extra_fixes, Fixit):
                extra_fixes = [extra_fixes]

            for f in extra_fixes:
                self._fixup_description_for_fixit(f, fixit_func)
                fixits_from_hooks.append(f)
        return fixits_from_hooks

    @abstractmethod
    def check(
        self, context: chapel.Context, root: chapel.AstNode
    ) -> typing.Iterable[CheckResult]:
        pass


class BasicRule(Rule[BasicRuleResult]):
    """
    Class containing all information for the driver about basic rules
    """

    def __init__(
        self, driver, name: str, pattern: typing.Any, check_func: BasicRuleCheck
    ) -> None:
        super().__init__(driver, name)
        self.pattern = pattern
        self.check_func = check_func

    def _check_single(
        self, context: chapel.Context, node: chapel.AstNode
    ) -> typing.Optional[CheckResult]:
        result = self.check_func(context, node)
        check, fixits = None, []

        # unwrap the result from the check function, and wrap it back
        # into a BasicRuleResult so we can feed it to the fixit hooks
        if isinstance(result, BasicRuleResult):
            check = False
            fixits = result.fixits(context, self.name)
        else:
            check = result
            result = BasicRuleResult(node)

        if check:
            return None

        # if we are going to warn, check for fixits from fixit hooks (in
        # addition to the fixits in the result itself)
        # add the fixits from the hooks to the fixits from the rule
        fixits = self.run_fixit_hooks(context, result) + fixits
        return (node, self.name, fixits)

    def check(
        self, context: chapel.Context, root: chapel.AstNode
    ) -> typing.Iterable[CheckResult]:
        for node, _ in self.driver.each_matching(root, self.pattern):
            if not self.driver.should_check_rule(self.name, node):
                continue

            checked = self._check_single(context, node)
            if checked is not None:
                yield checked


class AdvancedRule(Rule[AdvancedRuleResult]):
    """
    Class containing all information for the driver about advanced
    """

    def __init__(
        self, driver, name: str, check_func: AdvancedRuleCheck
    ) -> None:
        super().__init__(driver, name)
        self.check_func = check_func

    def check(
        self, context: chapel.Context, root: chapel.AstNode
    ) -> typing.Iterable[CheckResult]:
        for result in self.check_func(context, root):
            if isinstance(result, AdvancedRuleResult):
                node, anchor = result.node, result.anchor
                fixits = result.fixits(context, self.name)
                if anchor is not None and not self.driver.should_check_rule(
                    self.name, anchor
                ):
                    continue
            else:
                node = result
                fixits = []
                result = AdvancedRuleResult(node)

            # For advanced rules, the traversal of the AST is out of our hands,
            # so we can't stop it from going into unstable modules. Instead,
            # once the rule emits a warning, check by traversing the AST
            # if the warning target should be skipped.
            if self.driver.config.skip_unstable and chapel.in_unstable_module(
                node
            ):
                continue

            # if we are going to warn, check for fixits from fixit hooks (in
            # addition to the fixits in the result itself)
            # add the fixits from the hooks to the fixits from the rule
            fixits = self.run_fixit_hooks(context, result) + fixits
            yield (node, self.name, fixits)
