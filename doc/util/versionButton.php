
<style>
#versionButton {
  padding: 4px;
  border: none;
  cursor: pointer;
}

/* Blue */
.currentVersion {
  background-color: #36acf9;
  color: white;
}
.currentVersion:hover, .currentVersion:focus {
  background-color: #2f95d8;
}

/* Orange */
.oldVersion {
  background-color: #f49542;
  color: white;
}
.oldVersion:hover, .oldVersion:focus {
  background-color: #d68137;
}

/* Yellow */
.preRelease {
  background-color: #f4df42;
  color: black;
}
.preRelease:hover, .preRelease:focus {
  background-color: #d3c139;
  color: black;
}

#dropContainer {
  position: relative;
  display: inline-block;
  margin-bottom:10px;
}

#dropDiv {
  display: none;
  position: absolute;
  background-color: #f9f9f9;
  min-width: 160px;
  overflow: auto;
  box-shadow: 0px 8px 16px 0px rgba(0,0,0,0.2);
  z-index: 1;
  height: 150px;
  right: 0;
}

#dropDiv a {
  color: black;
  padding: 12px 16px;
  text-decoration: none;
  display: block;
}

.dropdown a:hover {background-color: #f1f1f1}

</style>
<div id="dropContainer">
  <button id="versionButton" onclick="toggleDropDown()"></button>
  <div id="dropDiv">
  </div>
</div>
<script type="text/javascript">

var dropDiv = document.getElementById("dropDiv");

function toggleDropDown() {
  var dsp = dropDiv.style.display;
  if (dsp != "block") {
    dropDiv.style.display = "block";
  } else {
    dropDiv.style.display = "none";
  }
}

var chplTitle = "<?php echo "$chplTitle";?>";

// Note: assumes second element is most-recent release
var chplVersions = [
  "1.18 pre-release",
  "1.17",
  "1.16",
  "1.15",
  "1.14",
  "1.13",
  "1.12",
  "1.11"
];

var pagePath = "<?php echo "$pagename";?>";
if (pagePath == "") {
  // If no path is given, default to root of other docs
  pagePath = "index.html";
}
function dropSetup() {
  var button = document.getElementById("versionButton");
  // Uses unicode down-pointing triangle
  button.innerHTML = chplTitle + " &#9660;";

  // Choose button color
  if (chplTitle.includes("pre-release")) {
    button.classList.add("preRelease");
  } else if (chplTitle != "1.17") {
    button.classList.add("oldVersion");
  } else {
    button.classList.add("currentVersion");
  }

  // Clear old links (if any)
  while (dropDiv.firstChild) {
    dropDiv.removeChild(dropDiv.firstChild);
  }

  // Add links to chapel-lang.org/docs/###/
  for (var i = 0; i < chplVersions.length; i++) {
    var ver = chplVersions[i];
    if (ver != chplTitle) {
      var link = document.createElement("a");
      link.innerHTML = ver;
      if (ver.includes("pre-release")) {
        ver = "master";
      }
      link.href = "http://chapel-lang.org/docs/" + ver + "/" + pagePath + ".html";
      dropDiv.append(link);
    }
  }
}
dropSetup();

// Close the dropdown if the user clicks outside of it
window.onclick = function(event) {
  if (!event.target.matches('#versionButton')) {
    dropDiv.style.display = "none";
  }
}
</script>
