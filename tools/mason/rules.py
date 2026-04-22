#
# Copyright 2026-2026 Hewlett Packard Enterprise Development LP
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

import chapel
from rule_types import BasicRuleResult
from fixits import Fixit, Edit


def rules(driver):

    @driver.basic_rule(chapel.StringLikeLiteral)
    def OnlyDoubleQuote(_, node: chapel.StringLikeLiteral):
        if node.quote_style() != '"' and node.quote_style() != '"""':
            # if the string contains a double quote, its ok
            # single quotes used to avoid escaping
            if node.value().find('"') != -1:
                return True
            return BasicRuleResult(node, ignorable=False)
        else:
            return True

    @driver.fixit(OnlyDoubleQuote)
    def OnlyDoubleQuoteFixit(context: chapel.Context, result: BasicRuleResult):
        if result.node.quote_style() == '"""':
            return None
        loc = result.node.location()
        s = (
            result.node.value()
            .replace('"', '\\"')
            .replace("\n", "\\n")
            .replace("\t", "\\t")
        )
        fixit = Fixit.build(Edit.build(loc, '"' + s + '"'))
        return fixit
