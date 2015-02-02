/*
 * Copyright 2004-2015 Cray Inc.
 * Other additional copyright holders may be indicated within.
 *
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "processTokens.h"

#include "countTokens.h"
#include "misc.h"
#include "stringutil.h"

#include "yy.h"
#include "chapel.tab.h"

#include <cstring>
#include <cctype>
#include <string>

int   stringBuffLen = 0;
int   stringLen     = 0;
char* stringBuffer  = NULL;

void newString() {
  stringLen = 0;
  if (stringBuffLen) {
    stringBuffer[stringLen] = '\0';
  }
}

// Returns the hexadecimal character for 0-16.
static char toHex(char c)
{
  if (0 <= c && c <= 9) return '0' + c;
  else return 'A' + (c - 10);
}

static void addCharMaybeEscape(char c, bool canEscape) {
  int escape = canEscape && !(isascii(c) && isprint(c));
  int charlen = escape ? 4 : 1; // convert nonasci to \xNN

  if (stringLen+charlen+1 > stringBuffLen) {
    stringBuffLen = 2*(stringBuffLen + charlen);
    stringBuffer = (char*)realloc(stringBuffer, stringBuffLen*sizeof(char));
  }

  if (escape) {
    stringBuffer[stringLen++] = '\\';
    stringBuffer[stringLen++] = 'x';
    stringBuffer[stringLen++] = toHex(((unsigned char)c) >> 4);
    stringBuffer[stringLen++] = toHex(c & 0xf);
  } else {
    stringBuffer[stringLen++] = c;
  }
  stringBuffer[stringLen] = '\0';
}

void addCharString(char c) {
  addCharMaybeEscape(c, true);
}

void addChar(char c) {
  addCharMaybeEscape(c, false);
}

void addString(const char* str) {
  for (int i = 0; str[i]; i++)
    addChar(str[i]);
}

void processNewline(void) {
  chplLineno++;
  yylloc.first_column = yylloc.last_column = 0;
  yylloc.first_line = yylloc.last_line = chplLineno;
  countNewline();
}
