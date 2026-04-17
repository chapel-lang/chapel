import ast
import re
from typing import Dict, Tuple, Optional, Set, List


class PyiSignatures:

    def __init__(self, filename: str):
        self.filename = filename
        self.regex = re.compile(r"def\s+([^ ]+)(\(.*\))(?:\s*->\s*(.+?))?:")
        # (classname, methodname) -> (args, return)
        self.methods: Dict[Tuple[str, str], Tuple[str, str]] = dict()
        # (classname) -> signature ('CLASSNAME(PARENT)')
        self.classes: Dict[str, Optional[str]] = dict()
        self._process()

    def _process(self):
        with open(self.filename, "r") as f:
            tree = ast.parse(f.read())

        for cls in ast.walk(tree):
            if isinstance(cls, ast.ClassDef):
                classname = cls.name
                # just pick the single base class, since chapel-py doesn't
                # do multiple inheritance.
                parent = cls.bases[0].id if cls.bases else None
                if parent:
                    self.classes[classname] = f"({parent})"
                for node in ast.walk(cls):
                    if isinstance(node, ast.FunctionDef):
                        methodname = node.name
                        node.body = node.body[2:]
                        s = ast.unparse(node)
                        m = self.regex.search(s)
                        if m:
                            args = m.group(2)
                            ret = m.group(3)
                            self.methods[(classname, methodname)] = (args, ret)

    def get_method(
        self, classname: str, methodname: str
    ) -> Optional[Tuple[str, str]]:
        return self.methods.get((classname, methodname))

    def get_class(self, classname: str) -> Optional[str]:
        return self.classes.get(classname)
