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
from dataclasses import dataclass

import chapel
from fixits import Fixit


@dataclass
class FullBasicRuleResult:
    fixit: typing.Union[Fixit, typing.List[Fixit]]


BasicRuleResult = typing.Union[bool, FullBasicRuleResult]
BasicRule = typing.Callable[[chapel.Context, chapel.AstNode], BasicRuleResult]


@dataclass
class FullAdvancedRuleResult:
    node: chapel.AstNode
    anchor: typing.Optional[chapel.AstNode] = None
    fixit: typing.Optional[
        typing.Union[Fixit, typing.List[Fixit]]
    ] = None


AdvancedRuleResult = typing.Iterator[
    typing.Union[chapel.AstNode, FullAdvancedRuleResult]
]
AdvancedRule = typing.Callable[
    [chapel.Context, chapel.AstNode], AdvancedRuleResult
]

RuleResult = typing.Union[BasicRuleResult, AdvancedRuleResult]
Rule = typing.Union[BasicRule, AdvancedRule]
