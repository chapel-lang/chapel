
import ast
import re
from typing import Dict, Tuple, Optional

class PyiSignatures:

    def __init__(self, filename: str):
        self.filename = filename
        self.regex = re.compile(r'def\s+([^ ]+)(\(.*\))(?:\s*->\s*(.+?))?:')
        # (classname, methodname) -> (args, return)
        self.types: Dict[Tuple[str, str], Tuple[str, str]] = dict()
        self._process()

    def _process(self):
        with open(self.filename, 'r') as f:
            tree = ast.parse(f.read())

        for cls in ast.walk(tree):
            if isinstance(cls, ast.ClassDef):
                classname = cls.name
                for node in ast.walk(cls):
                    if isinstance(node, ast.FunctionDef):
                        methodname = node.name
                        node.body = node.body[2:]
                        s = ast.unparse(node)
                        m = self.regex.search(s)
                        if m:
                            args = m.group(2)
                            ret = m.group(3)
                            self.types[(classname, methodname)] = (args, ret)


    def get(self, classname: str, methodname: str) -> Optional[Tuple[str, str]]:
        return self.types.get((classname, methodname))
