
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

/* Red */
.oldVersion {
  background-color: #ff0000;
  color: white;
}
.oldVersion:hover, .oldVersion:focus {
  background-color: #ff0000;
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

var pagePath = "<?php echo "$pagename";?>";
if (pagePath == "") {
  // If no path is given, default to root of other docs
  pagePath = "index";
}
function dropSetup() {
  var currentRelease = "1.22"; // what does the public have?
  var stagedRelease = "1.23";  // is there a release staged but not yet public?
  var nextRelease = "1.23";    // what's the next release? (on docs/master)
  var button = document.getElementById("versionButton");
  // Uses unicode down-pointing triangle
  var arrow = " &#9660;";
  button.innerHTML = "version " + chplTitle + arrow;

  // Choose button color
  if (chplTitle == stagedRelease || chplTitle == nextRelease) {
    // add pre-release label using a non-breaking hyphen
    button.innerHTML = "version " + chplTitle + " (pre&#8209;release)" + arrow;
    button.classList.add("preRelease");
  } else if (chplTitle == currentRelease) {
    button.classList.add("currentVersion");
  } else {
    // add old release label using a non-blocking space
    button.innerHTML = "version " + chplTitle + " (old&nbsp;release)" + arrow;
    button.classList.add("oldVersion");
  }

  // Clear old links (if any)
  while (dropDiv.firstChild) {
    dropDiv.removeChild(dropDiv.firstChild);
  }

  if (chplTitle != currentRelease) {
    // Add links to current version of docs
    var link = document.createElement("a");
    link.innerHTML = "version "+currentRelease+"<br>(latest release)";
    link.href = "https://chapel-lang.org/docs/" + pagePath + ".html";
    dropDiv.append(link);
  }

  if (!chplTitle.includes(nextRelease)) {
    // Add links to master version of docs
    var link = document.createElement("a");
    link.innerHTML = "version " + nextRelease + "<br>(pre-release)";
    link.href = "https://chapel-lang.org/docs/master/" + pagePath + ".html";
    dropDiv.append(link);
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
