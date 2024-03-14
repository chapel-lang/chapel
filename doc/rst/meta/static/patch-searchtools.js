--- searchtools.js	2024-03-14 17:18:07.389035224 -0400
+++ searchtools.js	2024-03-14 17:55:58.410810922 -0400
@@ -64,9 +64,38 @@
   const showSearchSummary = DOCUMENTATION_OPTIONS.SHOW_SEARCH_SUMMARY;
   const contentRoot = document.documentElement.dataset.content_root;
 
-  const [docName, title, anchor, descr, score, _filename] = item;
+  const [docName, title, anchor, descr, score, _filename, showTitle] = item;
 
   let listItem = document.createElement("li");
+  let typestr = "";
+  if (docName.startsWith("language/spec/"))
+    typestr = "Language Specification";
+  else if (docName.startsWith("primers/"))
+    typestr = "primer";
+  else if (docName.startsWith("examples/"))
+    typestr = "example";
+  else if (docName.startsWith("users-guide/"))
+    typestr = "users guide";
+  else if (docName.startsWith("usingchapel/man"))
+    typestr = "man page";
+  else if (docName.startsWith("technotes"))
+    typestr = "technical note";
+  else if (docName.startsWith("developer/compiler-internals/"))
+    typestr = "compiler internals";
+  else if (docName.startsWith("modules/"))
+    typestr = "standard library";
+
+  let mydescr;
+  if (typestr || descr) {
+    mydescr = " ("; 
+    if (typestr != "") mydescr += typestr;
+    if (descr) {
+      if (typestr != "") mydescr += " ";
+      mydescr += descr;
+    }
+    mydescr += ")";
+  }
+
   let requestUrl;
   let linkUrl;
   if (docBuilder === "dirhtml") {
@@ -86,9 +115,11 @@
   linkEl.href = linkUrl + anchor;
   linkEl.dataset.score = score;
   linkEl.innerHTML = title;
+
+  if (mydescr)
+    listItem.appendChild(document.createElement("span")).innerHTML = mydescr;
+
   if (descr) {
-    listItem.appendChild(document.createElement("span")).innerHTML =
-      " (" + descr + ")";
     // highlight search terms in the description
     if (SPHINX_HIGHLIGHT_ENABLED)  // set in sphinx_highlight.js
       highlightTerms.forEach((term) => _highlightText(listItem, term, "highlighted"));
@@ -295,11 +326,12 @@
           let score = Math.round(100 * queryLower.length / title.length)
           results.push([
             docNames[file],
-            titles[file] !== title ? `${titles[file]} > ${title}` : title,
+            title,
             id !== null ? "#" + id : "",
             null,
             score,
             filenames[file],
+            titles[file] !== title ? `${title} in ${titles[file]}` : title,
           ]);
         }
       }
@@ -312,11 +344,12 @@
           let score = Math.round(100 * queryLower.length / entry.length)
           results.push([
             docNames[file],
-            titles[file],
+            entry,
             id ? "#" + id : "",
             null,
             score,
             filenames[file],
+            "<b>" + entry + "</b> in " + titles[file],
           ]);
         }
       }
@@ -432,6 +465,7 @@
         descr,
         score,
         filenames[match[0]],
+        null,
       ]);
     };
     Object.keys(objects).forEach((prefix) =>
@@ -539,6 +573,7 @@
         null,
         score,
         filenames[file],
+        null
       ]);
     }
     return results;
