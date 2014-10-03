// Release branch info
// For now, I'm not using the release fields
var branchInfo = [
                  { "release" : "0.9",
                    "branchDate" : "2009-04-08", // no actual branch
                    "revision" : 15461 },
                  { "release" : "1.0",
                    "branchDate" : "2009-10-16",
                    "revision" : 16229 },
                  { "release" : "1.01",
                    "branchDate" : "2009-10-30",
                    "revision" : 16330 },
                  { "release" : "1.02",
                    "branchDate" : "2009-11-13",
                    "revision" : 16423 },
                  { "release" : "1.1",
                    "branchDate" : "2010-04-09",
                    "revision" : 17087},
                  { "release" : "1.2",
                    "branchDate" : "2010-10-14",
                    "revision" : 17926},
                  { "release" : "1.3",
                    "branchDate" : "2011-04-12",
                    "revision" : 18701},
                  { "release" : "1.4",
                    "branchDate" : "2011-10-11",
                    "revision" : 19320},
                  { "release" : "1.5",
                    "branchDate" : "2012-04-10",
                    "revision" : 19961},
                  { "release" : "1.6",
                    "branchDate" : "2012-10-08",
                    "revision" : 20667},
                  { "release" : "1.7",
                    "branchDate" : "2013-04-05",
                    "revision" : 21260},
                  { "release" : "1.8",
                    "branchDate" : "2013-10-04",
                    "revision" : 22086},
                  { "release" : "1.9",
                    "branchDate" : "2014-04-07",
                    "revision" : 23144},
                  { "release" : "1.10",
                    "branchDate" : "2014-09-22",
                    "revision" : -1}  // TODO: This was a git commit.
                  ];

// stuff for dygraph
var gs = []; // array of current graphs
var blockRedraw = false;

// hack to use the previous compiler performance keys to set
// colors/dashed lines for various configurations. Will be
// updated in the near term.
if (descriptions[0])
    var primaryString = descriptions[0];
else
    var primaryString = ' (all)';
if (descriptions[1])
    var secondaryString =  descriptions[1];
else
    var secondaryString = ' (examples)';

// The main elements that all the graphs and graph legends will be put in
var parent = document.getElementById('graphdisplay');
var legend = document.getElementById('legenddisplay');

// This is used to get the next div for the graph and legend. This is important
// for graph expansion because we need to be able to add the expanded graphs
// after the graph that is being expanded and there may be other graphs
// after it so we don't just want to put the expanded graphs at the end.
function getNextDivs(afterDiv, afterLDiv) {

    // if divs were specified, create new divs that follow those, else just put
    // these divs at the end
    var beforeDiv = null;
    var beforeLDiv = null;
    if (afterDiv && afterLDiv &&
        afterDiv.nextSibling && afterLDiv.nextSibling) {
        beforeDiv = afterDiv.nextSibling.nextSibling;
        beforeLDiv = afterLDiv.nextSibling.nextSibling;
    }

    // create the graph/legend divs and spacers
    var div = document.createElement('div');
    div.className = 'perfGraph';
    parent.insertBefore(div, beforeDiv);

    var gspacer = document.createElement('div');
    gspacer.className = 'gspacer';
    parent.insertBefore(gspacer, beforeDiv);

    var ldiv = document.createElement('div');
    ldiv.className = 'perfLegend';
    legend.insertBefore(ldiv, beforeLDiv);

    var lspacer = document.createElement('div');
    lspacer.className = 'lspacer';
    legend.insertBefore(lspacer, beforeLDiv);

    // create a log button and put it in the gspacer
    var logToggle = document.createElement('input');
    logToggle.type = 'button';
    logToggle.className = 'toggle';
    logToggle.value = 'log';
    logToggle.style.visibility = 'hidden';
    gspacer.appendChild(logToggle);

    // create an annotation button and put it next to the log button in gspacer
    var annToggle = document.createElement('input');
    annToggle.type = 'button';
    annToggle.className = 'toggle';
    annToggle.value = 'annotations';
    annToggle.style.visibility = 'hidden';
    gspacer.appendChild(annToggle);

    return {
        div: div,
        ldiv: ldiv,
        logToggle: logToggle,
        annToggle: annToggle
    }
}

// Gen a new dygraph, if an existing graph is being expanded then expandInfo
// will contain the expansion information, else it is null
function genDygraph(graphInfo, expandInfo) {

    // setup the divs
    var afterDiv = null;
    var afterLDiv = null;
    if (expandInfo) {
        afterDiv = expandInfo.afterDiv;
        afterLDiv = expandInfo.afterLDiv;
    }
    var divs = getNextDivs(afterDiv, afterLDiv);
    var div = divs.div;
    var ldiv = divs.ldiv;
    var logToggle = divs.logToggle;
    var annToggle = divs.annToggle;

    var startdate = graphInfo.startdate;
    var enddate = graphInfo.enddate;
    // if our x axis values are actually dates, parse them as such
    if (!numericX) {
      startdate= Dygraph.dateParser(startdate);
      enddate= Dygraph.dateParser(enddate);
    }

    // setup our options
    var graphOptions = {
        title: graphInfo.title,
        ylabel: graphInfo.ylabel,
        drawXGrid: false,
        drawYGrid: true,
        includeZero: true,
        showRoller: true,
        legend: 'always',
        customBars: graphInfo.displayrange,
        highlightSeriesOpts: {
            strokeWidth: 2,
            strokeBorderWidth: 0,
            highlightCircleSize: 4
        },
        // don't "dim" the  series when one is highlighted
        highlightSeriesBackgroundAlpha: 1,
        // So it's easier to zoom in on the right side
        rightGap: 15,
        // So y values don't overlap with the y label
        yAxisLabelWidth: 80,
        labelsDiv: ldiv,
        labelsSeparateLines: true,
        dateWindow: [startdate, enddate],
        // sync graphs anytime we pan, zoom, or at initial draw
        drawCallback: customDrawCallback,
        // mark the release dates on the graph before the chart gets drawn
        underlayCallback: markReleaseDates
    }

    if (expandInfo) {
        graphOptions.visibility = expandInfo.visibility;
        graphOptions.colors = expandInfo.colors;
    }

    // actually create the dygraph
    var g = new Dygraph(div, 'CSVfiles/'+graphInfo.datfname, graphOptions);
    g.isReady = false;
    setupSeriesLocking(g);

    // we use options in graphinfo in dygraph callbacks that we can't pass
    // arguments to so we add it to the graph to be able to pass it around
    g.divs = divs;
    g.graphInfo = graphInfo;

    // The dygraph is now setting up and rendering. Once the graph is fully
    // drawn this ready state gets fired. We don't want to synchronize this
    // grpahs x-axis until it has been fully rendered, or we will be modifying
    // properties that don't exist yet. We use the isReady state to handle
    // that. We also make our buttons visible here that way they don't show up
    // before the graph does.
    g.ready(function() {
        setupLogToggle(g, graphInfo, logToggle);
        setupAnnToggle(g, graphInfo, annToggle);
        g.isReady = true;
    });

    gs.push(g);
}

// This is a small helper function to see if a string ends with a certain
// string. Do not redefine if a browser has a built in version
if (typeof String.prototype.endsWith !== 'function') {
    String.prototype.endsWith = function(suffix) {
        return this.indexOf(suffix, this.length - suffix.length) !== -1;
    };
}

// Function to expand an existing graph. This will potentially change the
// visibility of the current graph and will create multiple graphs from the
// same data just with certain ones hidden. For instance this is used with the
// compiler performance graphs to display only the top 10 series in a graph and
// to create an individual graph for each series. Note that if a there are two
// series that have the same name but one ends with ' (examples)' and the other
// ' (all') They will both be shown on the expanded graph but the examples one
// will not be shown on the graph containing all of them.
function expandGraphs(graph) {

    var graphInfo = graph.graphInfo;
    var expandAllSentinel = -1;
    var expandNum = graphInfo.expand;
    var labels = graph.getLabels();

    // if we don't need to expand just return
    if (!expandNum || expandNum === 0) {
        return;
    }

    // check for expand all sentinel, or expansion too high
    if (expandNum === expandAllSentinel || expandNum >= labels.length) {
        expandNum = labels.length - 1;
    }

    // modify the current graph's visibility if only some of the series are to
    // be expanded
    var visibility = graph.visibility();
    for (var i = 0; i < visibility.length; i++) { visibility[i] = false; }
    var i = 0;
    var j = 0;
    while (i < expandNum && j < labels.length -1 ) {
        j++;
        if (labels[j].endsWith(secondaryString)) {
            continue;
        }
        visibility[j-1] = true;
        i++;
     }
     graph.updateOptions({visibility: visibility});

    // figure out the starting series for expansion. we expand graphs in
    // reverse order so we need to figure out which is the last one we will
    // expand and start expanding at that one.
    var i = 0;
    var j = 1;
    while ( i < expandNum && j < labels.length) {
        if (!labels[j].endsWith(secondaryString)) {
            i++;
        }
        j++;
    }

    // for each expanded graph
    var i = 0;
    while (i < expandNum && j > 1 ) {
        j--;
        if (labels[j].endsWith(secondaryString)) {
            continue;
        }

        // copy the graphInfo and add the key to the title
        var newInfo = {};
        for (info in graphInfo) {
            newInfo[info] = graphInfo[info];
        }
        newInfo.title = newInfo.title + ": " + labels[j].replace(' (all)', '');
        newInfo.expand = 0;

        // gen the expanded graph with visibility set for the current series
        for (var k = 0; k < visibility.length; k++) { visibility[k] = false; }
        var exampleLabel = labels[j].replace(primaryString, secondaryString);
        var exampleIndex = graph.getPropertiesForSeries(exampleLabel).column;
        visibility[j-1] = true;
        visibility[exampleIndex-1] = true;

        // make sure the colors for the series in the expanded graph match the
        // colors for the series in the original graph
        var colors = graph.getColors().slice();
        for ( var k = 0; k < colors.length; k++) {
          colors[k] = graph.getPropertiesForSeries(labels[j]).color;
        }

        expandInfo = {
            afterDiv: graph.divs.div,
            afterLDiv: graph.divs.ldiv,
            visibility: visibility.slice(),
            colors: colors
        }
        genDygraph(newInfo, expandInfo);
        i++;
    }
}

// Setup the log button
function setupLogToggle(g, graphInfo, logToggle) {
    logToggle.style.visibility = 'visible';

    logToggle.onclick = function() {
        var useLog = !g.getOption('logscale');
        if (useLog) logToggle.value = 'linear';
        else        logToggle.value = 'log';
        g.updateOptions({ logscale: useLog });
    }
}

// Setup the annotation button
function setupAnnToggle(g, graphInfo, annToggle) {
    annToggle.style.visibility = 'visible';

    annToggle.onclick = function() {
        if (g.annotations().length === 0) {
            g.setAnnotations(graphInfo.annotations);
        } else {
            g.setAnnotations([]);
        }
    }
}

// Because we let dygraphs parse the data and setup and then sort the series
// order, funky things happen with the colors because they don't get sorted
// with the series. This just resets the order of the colors back to the
// original. This way we don't have multiple series with the same colors next
// to each other, which would make the graph hard to read.
function setColors(g, origColors, blockRedraw) {
    var labels = g.getLabels();
    var visibility = g.visibility();
    var colors = origColors.slice();

    // We need to create a map between the index of a label and it's index in
    // the colors array. The colors array just has the colors for the visible
    // series while the label array has the names for all the series. This map
    // is just so we can set the color of a particular series.
    var curColor = 0;
    var labelToColorMap = {};
    for (var i = 1; i < labels.length; i++) {
      if( visibility[i-1] ) {
        labelToColorMap[i] = curColor;
        curColor += 1;
      }
    }

    // Reset the colors to the original ones
    var curColor = 0;
    for (var i = 1; i < labels.length; i++) {
        if(!labels[i].endsWith(secondaryString) && visibility[i-1]) {
            colors[labelToColorMap[i]] = origColors[curColor];
            curColor += 1;
        }
    }

    // This is for compiler performance graphs only. If a series ends with
    // secondaryString we want to make sure it has the same colors as the '(all)
    // series and that it has a dashed line to make it easier to distinguish.
    for (var i = 1; i < labels.length; i++) {
        if(labels[i].endsWith(secondaryString)) {
            allLabel = labels[i].replace(secondaryString, primaryString);
            allIndex = g.indexFromSetName(allLabel);
            if (!visibility[allIndex-1]) continue;
            var color = colors[labelToColorMap[allIndex]];
            colors[labelToColorMap[i]] = color;
            var series = {};
            series[labels[i]] = { 'strokePattern' : Dygraph.DASHED_LINE };
            g.updateOptions({ 'series' : series }, true);
       }
    }

    // update the colors array with the new colors
    g.updateOptions({ 'colors' : colors }, blockRedraw);
}

// Function to sort the data after it has been converted to an array
// Note that this sorts based on the most recent day available of all
// data. The sort order does not change based on zoom or anything.
function sortData(g) {

    var customBars = g.getOption('customBars');
    var myLabels = g.getLabels();
    var dataAsArray = g.rawData_;

    // if there is no data or if we only have the date and a
    // single key, the data is already "sorted"
    if (!dataAsArray || !dataAsArray[0] || dataAsArray[0].length <= 2) {
        return dataAsArray;
    }

    // transpose the data so that was can take advantage of built in sorting
    var transposedData = transpose(dataAsArray);

    // add the labels to the array so that the labels get sorted too
    for (var i = 0; i < transposedData.length; i++) {
        transposedData[i].splice(0, 0, myLabels[i].slice());
    }

    // the first row is now all the dates (and the date label.) They will
    // screw up sorting, but we need to save them so that we can add them
    // back in after sorting
    var firstRow = transposedData.shift();

    // sort our data based on value of the most recent day (we could also
    // easily sort based on the average of the last x days or something too.)
    transposedData.sort(function(a, b) {
        if (customBars) {
            return b[b.length-1][1] - a[a.length-1][1];
        } else {
            return b[b.length-1] - a[a.length-1];
        }
    });

    //add the dates and date label back in
    transposedData.splice(0, 0, firstRow);

    // take the labels out and store them in the myLabels array
    for (var i = 0; i < transposedData.length; i++) {
        myLabels[i] = transposedData[i].shift();
    }

    // update or labels with the now sorted ones
    g.updateOptions({labels: myLabels}, true);

    // undo our original transpose and return the sorted data
    dataAsArray  = transpose(transposedData);
    return dataAsArray;
}

// helper function to transpose a 2 dim array (to make sorting a lot easier.)
// assumes array is a non-empty rows x cols array
function transpose(array) {
    var temp = [];
    var cols = array.length;
    var rows = array[0].length;
    if (cols === 0 || rows === 0) { return temp; }

    for (var r = 0; r < rows; r++) {
        temp[r] = [];
        for (var c = 0; c < cols; c++) {
            temp[r][c] = array[c][r];
        }
    }
    return temp;
}

// synchronize our graphs along the x-axis and update the number of decimals
// being displayed per graph based on the range of data being displayed
function customDrawCallback(g, initial) {
    if (blockRedraw) return;
    blockRedraw = true;

    // We let dygraphs handle reading the data and parsing it into an array. We
    // then sort that data on the first draw. This is a little weird because
    // we're creating a graph, and while it's rendering we sort it but having
    // to parse the data ourselves would be a real pain. Since the series
    // colors don't get sorted with the data we save the original and then
    // reset so that multiple series that are next to each other don't have the
    // same color. After sorting is done, we may expand the graph.
    if (initial) {
        if (g.graphInfo.sort) {
            var origColors = g.getColors().slice();
            g.rawData_ = sortData(g);
            g.setAnnotations(g.annotations());
            setColors(g, origColors, true);
        }
        if (descriptions.length > 0) {
          setConfigurationVisibility(g, true);
        }

        g.setAnnotations(g.annotations());
        expandGraphs(g);
    }

    // Find the range we're displaying and adjust the number of decimals
    // accordingly. setAnnotations() is used to redraw the graph. Normally
    // updateOptions should redraw the graph, but it doesn't always work as
    // expected in a callback.
    var oldNumDigits = g.getOption('digitsAfterDecimal');
    var newNumDigits = 0;
    var yRange = g.yAxisRange();
    var yDiff = yRange[1] - yRange[0];

    if (yDiff < 1.0) {
        newNumDigits = 4;
    } else if (yDiff < 100.0) {
        newNumDigits = 2;
    } else if (yDiff < 1000000.0) {
        newNumDigits = 0;
    } else {
        newNumDigits = 2;
    }

    if(newNumDigits !== oldNumDigits) {
        g.updateOptions({digitsAfterDecimal: newNumDigits}, true);
        g.setAnnotations(g.annotations());
    }

    // if a user has explicitly zoomed in on zero or negative value and they
    // attempt to take the log the graph will not render. This is a known
    // limitation of dygraphs. If the user has not zoomed, the graph will
    // automatically adjust the y display range, but if they have explicitly
    // requested a range, it will keep the same range for the log scale and
    // will attempt to take the log of zero.
    if (yRange[0] <= 0 && g.isZoomed('y')) {
        g.divs.logToggle.style.color = 'red';
    } else {
        g.divs.logToggle.style.color = 'black';
    }

    // if this isn't the initial draw, and this graph is fully rendered then
    // sync this graphs x-axis with all other ready graphs along the x-axis
    if (!initial && g.isReady) {
        var range = g.xAxisRange();
        for (var j = 0; j < gs.length; j++) {
            if (gs[j] === g || !gs[j].isReady ||
                range.toString() === gs[j].xAxisRange().toString()) continue;
            gs[j].updateOptions({ dateWindow: range });
        }
    }
    blockRedraw = false;
}

// Mark all the release dates on the canvas with vertical lines
function markReleaseDates (canvas, area, g) {
    function markReleaseDate(date) {
        var xval = g.toDomXCoord(date);
        canvas.beginPath();
        canvas.moveTo(xval, area.y);
        canvas.lineTo(xval, area.y + area.h);
        canvas.strokeStyle = '#e0af1b';
        canvas.stroke();
    }
    for (var i = 0; i < branchInfo.length; i++) {
        if (numericX) {
            markReleaseDate(branchInfo[i].revision);
        } else {
            markReleaseDate(Dygraph.dateParser(branchInfo[i].branchDate));
        }
    }
}

// If you click on a graph, focus on the currently highlighted series
// (highlighting isn't automatically switched, and don't change values
// back to newest on unhighlight.) This is nice for following a particular
// series. You can't do this as a regular option when the dygraph is created
// because the dygraph isn't passed as an argument to the callback function
function setupSeriesLocking(g) {
    var onclick = function (ev) {
        if (g.isSeriesLocked()) {
            g.clearSelection();
        } else {
            g.setSelection(g.getSelection(), g.getHighlightSeries(), true);
        }
    };
    g.setSelection(false);
    g.updateOptions({ clickCallback: onclick }, true);
}

function perfGraphInit() {
    // set the title
    var titleElem = document.getElementById('titleElem');
    titleElem.innerHTML = document.title;

    var d = new Date();
    var todayDate = d.getFullYear() + '-' + (d.getMonth()+1) + '-' + d.getDate();

    // if the graphs weren't synced today let the user know
    var dateElem= document.getElementById('dateElem');
    if(Dygraph.dateParser(runDate) < Dygraph.dateParser(todayDate)) {
        dateElem.innerHTML = 'Graphs Last Updated on ' + runDate;
        dateElem.style.color = "RED";
    }

    // generate the configuration menu and toggle options
    var toggleConf = document.getElementById('toggleConf');
    if (descriptions.length > 0) {
      for (var i = 0; i < descriptions.length; i++) {
        var elem = document.createElement('div');
        var description = descriptions[i];
        elem.className = 'graph';
        elem.innerHTML = '<input id="hide' + i + '"type="checkbox">' + description;
        toggleConf.appendChild(elem);
        var checkBox = document.getElementById('hide' + i);
        if (i === 0) {
          checkBox.checked = true;
        } else {
          checkBox.checked = false;
        }
        checkBox.onchange = function() {
          for (var i = 0; i < gs.length; i++) {
            setConfigurationVisibility(gs[i], false);
          }
        };
      }
    } else {
      toggleConf.textContent = '';
    }

    // generate the suite menu
    var suiteMenu = document.getElementById('suiteMenu');
    var f = document.createElement('form');
    f.name = "form1";
    var fselect = document.createElement('select');
    fselect.setAttribute('name', "jumpmenu");
    fselect.setAttribute('onChange', "jumpto(document.form1.jumpmenu.options[document.form1.jumpmenu.options.selectedIndex].value)");
    f.appendChild(fselect);
    var o = document.createElement('option');
    o.innerHTML = "None";
    o.setAttribute('value', "");
    fselect.appendChild(o);
    for (var i = 0; i < perfSuites.length; i++) {
        var o = document.createElement('option');
        o.innerHTML = perfSuites[i].suite;
        o.setAttribute('value', perfSuites[i].suite);
        fselect.appendChild(o);
    }
    suiteMenu.appendChild(f);

    // generate the graph list
    var graphlist = document.getElementById('graphlist');
    for (var i = 0; i < allGraphs.length; i++) {
        var elem = document.createElement('div');
        elem.className = 'graph';
        elem.innerHTML = '<input id="graph' + i + '" type="checkbox">' + allGraphs[i].title;
        graphlist.appendChild(elem);
    }

    setCheckBoxesFromURL();
    displaySelectedGraphs();
}

function setConfigurationVisibility(graph, blockRedraw) {
  var checked = {};
  for (var i = 0; i<descriptions.length; i++) {
    var checkBox = document.getElementById('hide' + i);
      checked[descriptions[i]] = checkBox.checked;
  }

  var labels = graph.getLabels();
  var visibility = graph.visibility();
  for (var j = 1; j < labels.length; j++) {
    var prop = graph.getPropertiesForSeries(labels[j]);
    for (var check in checked) {
      if (checked.hasOwnProperty(check)) {
        if (labels[j].endsWith(check)) {
          visibility[j-1] = checked[check];
        }
      }
    }

    graph.updateOptions({visibility: visibility}, blockRedraw);
  }
}


// This function parses the query string of the url and sets check boxes
// accordingly. All check boxes are set to not checked by default but if the
// query string contains <boxnumber>=1 then the box is set to be initially
// checked.
function setCheckBoxesFromURL() {
    var queryString = document.location.search.slice(1);
    var hashes = queryString.split('&');
    for(var i = 0; i < hashes.length; i++)  {
        var hash = hashes[i].split('=');
        if (hash[1] === '1') {
            var checkBox = document.getElementById('graph' + hash[0]);
            checkBox.checked = true;
        }
    }
}

// Update the query string of the url based on the current set of checkboxes
// that are checked.
function setURLFromCheckBoxes() {
    var baseURL = document.location.href.split('?')[0];
    var queryString = '?';
    for (var i = 0; i < allGraphs.length; i++) {
        var checkBox  = document.getElementById('graph' + i);
        if (checkBox.checked) {
            queryString += i + '=1&'
        }
    }
    queryString = queryString.slice(0, -1);
    history.replaceState(null, null, baseURL + queryString);
}

function selectAllGraphs() {
    checkAll(true);
}

function unselectAllGraphs() {
    checkAll(false);
}

function checkAll(val) {
    for (var i = 0; i < allGraphs.length; i++) {
        var elem = document.getElementById('graph' + i);
        elem.checked = val;
    }
}

function selectSuite(suite) {
    for (var i = 0; i < allGraphs.length; i++) {
        var elem = document.getElementById('graph' + i);
        if (allGraphs[i].suites.indexOf(suite) >= 0) {
            elem.checked = true;
        } else {
            elem.checked = false;
        }
    }
    displaySelectedGraphs();
}

function displaySelectedGraphs() {
    // Clean up divs
    while (parent.childNodes.length > 0) {
        parent.removeChild(parent.childNodes[0]);
        legend.removeChild(legend.childNodes[0]);
    }

    // clean up all the dygraphs
    while (gs.length > 0) {
        gs.pop().destroy();
    }

    // generate the dygraph(s) for the currently selected graphs
    for (var i = 0; i < allGraphs.length; i++) {
        var checkbox = document.getElementById('graph' + i);
        if (checkbox.checked) {
            genDygraph(allGraphs[i]);
        }
    }

    setURLFromCheckBoxes();
}

