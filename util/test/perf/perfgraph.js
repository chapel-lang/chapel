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

// array of currently displayed graphs
var gs = [];
// used to prevent multiple redraws of graphs when syncing x-axis zooms
var blockRedraw = false;

// hack to use the previous compiler performance keys to set
// colors/dashed lines for various configurations.
if (descriptions[0]) {
  var primaryString = descriptions[0];
} else {
  var primaryString = ' (all)';
}
if (descriptions[1]) {
  var secondaryString =  descriptions[1];
} else {
  var secondaryString = ' (examples)';
}

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

  var startdate = getOption(OptionsEnum.STARTDATE) || graphInfo.startdate;
  var enddate = getOption(OptionsEnum.ENDDATE) || graphInfo.enddate;
  startdate = parseDate(startdate);
  enddate = parseDate(enddate);

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
    var graphInfo = g.graphInfo;
    var expandNum = graphInfo.expand;

    // if we're expanding a graph, or we have multiple configs, set new colors
    if ((expandNum !== undefined && expandNum !== 0) || descriptions.length > 0) {
      setColors(g, g.getColors().slice(), true);
      g.setAnnotations(g.annotations());
    }

    if (descriptions.length > 0) {
      setConfigurationVisibility(g, true);
      g.setAnnotations(g.annotations());
    }

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
    range[0] = roundDate(range[0], false);
    range[1] = roundDate(range[1], true);

    setQueryStringFromOption(OptionsEnum.STARTDATE, Dygraph.dateString_(range[0]));
    setQueryStringFromOption(OptionsEnum.ENDDATE, Dygraph.dateString_(range[1]));

    for (var j = 0; j < gs.length; j++) {
      if (gs[j].isReady && (g === gs[j] ||
            range.toString() !== gs[j].xAxisRange().toString())) {
              gs[j].updateOptions({ dateWindow: range });
            }
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
    markReleaseDate(parseDate(branchInfo[i].branchDate));
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

var NO_SUITE = "None";


// TODO split this up into logical functions (setup multi conf, setup suite
// dropdown, setup graph list, etc.)
function perfGraphInit() {
  // set the title
  var titleElem = document.getElementById('titleElem');
  titleElem.innerHTML = document.title;

  var d = new Date();
  var todayDate = d.getFullYear() + '-' + (d.getMonth()+1) + '-' + d.getDate();

  // if the graphs weren't synced today let the user know
  var dateElem= document.getElementById('dateElem');
  if(parseDate(runDate) < parseDate(todayDate)) {
    dateElem.innerHTML = 'Graphs Last Updated on ' + runDate;
    dateElem.style.color = "RED";
  }

  // generate the multi configuration menu and toggle options
  var toggleConf = document.getElementById('toggleConf');
  if (descriptions.length > 0) {
    var queryStringConf = getOption(OptionsEnum.CONFIGURATIONS)
      if (queryStringConf) {
        var setConfigurations = queryStringConf.split(',');
      } else {
        var setConfigurations = [descriptions[0].trim()];
      }

    for (var i = 0; i < descriptions.length; i++) {
      var elem = document.createElement('div');
      var description = descriptions[i];
      elem.className = 'graph';
      elem.innerHTML = '<input id="hide' + i + '"type="checkbox">' + description;
      toggleConf.appendChild(elem);
      var checkBox = document.getElementById('hide' + i);
      if (setConfigurations.indexOf(description.trim()) >= 0) {
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
  o.innerHTML = NO_SUITE;
  o.setAttribute('value', NO_SUITE);
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

  setGraphsFromURL();
  displaySelectedGraphs();
}


// Sets which configurations should be visible when there are multiple
// configurations available. e.g. --local vs --no-local
function setConfigurationVisibility(graph, blockRedraw) {
  var checked = {};
  var configs = ''
    for (var i = 0; i<descriptions.length; i++) {
      var checkBox = document.getElementById('hide' + i);
      if (checkBox.checked) {
        configs += i + ',';
      }
      checked[descriptions[i]] = checkBox.checked;
    }
  if (configs.removeTrailingChar() === descriptions[0].trim()) {
    configs = '';
  }
  setQueryStringFromOption(OptionsEnum.CONFIGURATIONS, configs.removeTrailingChar());
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


// This function parses the query string of the URL and finds out which graphs
// to check. It handles individual graphs, all graphs, and suites
function setGraphsFromURL() {
  var suiteToDisplay = getOption(OptionsEnum.SUITE);
  if (suiteToDisplay === '') {
    suiteToDisplay = NO_SUITE;
  }
  // if no suite was selected, set individual graphs
  if (normalizeForURL(suiteToDisplay) === normalizeForURL(NO_SUITE)) {
    var graphsToDisplay = getOption(OptionsEnum.GRAPHS);
    // special case for all graphs
    if (graphsToDisplay === 'all') {
      selectAllGraphs();
    } else if (graphsToDisplay.length) {
      // find all the graphs to display and check them
      var boxesToCheck = graphsToDisplay.split(',');
      for (var i = 0; i < allGraphs.length; i++) {
        if (boxesToCheck.indexOf(normalizeForURL(allGraphs[i].title)) >= 0) {
          var checkBox = document.getElementById('graph' + i);
          checkBox.checked = true;
        }
      }
    }
  }
  // if a suite was selected
  else {
    var suites = getSuites();
    for (var i = 0; i < suites.length; i++) {
      if (normalizeForURL(suites[i]) == suiteToDisplay) {
        selectSuite(suites[i], false);
        document.getElementsByName('jumpmenu')[0].value = suites[i];
      }
    }
  }
}


// Update the query string based on the current set of displayed graphs
function setURLFromGraphs(suite) {
  suite = normalizeForURL(suite);
  // if no suite was selected, mark individual graphs
  if (suite === normalizeForURL(NO_SUITE)) {
    var curGraphs = '';
    var allChecked = true;
    for (var i = 0; i < allGraphs.length; i++) {
      var checkBox = document.getElementById('graph' + i);
      if (checkBox.checked) {
        curGraphs += normalizeForURL(allGraphs[i].title) + ',';
      } else {
        allChecked = false;
      }
    }

    // remove the trailing ',' from the list of checked graphs
    curGraphs = curGraphs.removeTrailingChar();

    // special case for if all graphs are selected. probably not needed
    // but this is the most common case where a lot of graphs are selected
    // and for some browsers we might be exceeding a url length limit
    if (allChecked) {
      curGraphs = 'all';
    }

    setQueryStringFromOption(OptionsEnum.GRAPHS, curGraphs);
    setQueryStringFromOption(OptionsEnum.SUITE, '');
  }
  // if a suite was selected
  else {
    setQueryStringFromOption(OptionsEnum.SUITE, suite);
    setQueryStringFromOption(OptionsEnum.GRAPHS, '');
  }
}


// reset the date range
function clearDates() {
  // clear the query string
  setQueryStringFromOption(OptionsEnum.STARTDATE, '');
  setQueryStringFromOption(OptionsEnum.ENDDATE, '');

  // Reset the display range for each graph, blocking extra redraws
  blockRedraw = true;
  for (var i = 0; i < gs.length; i++) {
    var curGraph = gs[i];
    var start = parseDate(curGraph.graphInfo.startdate);
    var end = parseDate(curGraph.graphInfo.enddate);
    var range = [start, end];
    if (range.toString() !== curGraph.xAxisRange().toString()) {
      curGraph.updateOptions({ dateWindow: range });
    }
  }
  blockRedraw = false;
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


// returns an object of all the suites, whose values are initialized to ''
function getSuitesObject() {
  var suites = getSuites();
  var suitesObj = {};
  for (var i = 0; i < suites.length; i++) {
   suitesObj[suites[i]] = '';
  }
  return suitesObj;
}


// returns an array of all the suites
function getSuites() {
  var suites = [];
  for (var key in perfSuites) {
    suites.push(perfSuites[key].suite);
  }
  return suites;
}


function selectSuite(suite, display) {
  display = defaultFor(display, true);
  for (var i = 0; i < allGraphs.length; i++) {
    var elem = document.getElementById('graph' + i);
    if (allGraphs[i].suites.indexOf(suite) >= 0) {
      elem.checked = true;
    } else {
      elem.checked = false;
    }
  }
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

  // update the url for the displayed graphs
  setURLFromGraphs(normalizeForURL(findSelectedSuite()));
  // set the dropdown box selection
  document.getElementsByName('jumpmenu')[0].value = findSelectedSuite();
}


// Identify if a suite is selected based on the currently displayed graphs.
function findSelectedSuite() {
  var displayedGraphs = "";
  var graphsInSuite = getSuitesObject();

  // build a string representation of the currently selected graphs and
  // simultaneously build a string representation for graphs in each suite
  for (var i = 0; i < allGraphs.length; i++) {
    var curGraph = allGraphs[i];
    var elem = document.getElementById('graph' + i);
    if (elem.checked === true) {
      displayedGraphs += curGraph.title;
    }
    for (var suiteIndex in curGraph.suites) {
      graphsInSuite[curGraph.suites[suiteIndex]] += allGraphs[i].title;
    }
  }

  // see if the currently selected representation  matches any of the suites
  for (var suite in graphsInSuite) {
    if (displayedGraphs === graphsInSuite[suite]) {
      return suite;
    }
  }

  // if not, no suite was selected
  return NO_SUITE;
}




//////////////////////
// Start of options //
//////////////////////


// Options depict things like the current date range, which graphs are
// displayed, etc. The query string of the URL is used to store options so that
// you can easy share things with others. The OptionsEnum and the options
// functions are used to get options from the URL and write them back to the
// URL. These functions should be used for manipulating the query string rather
// than modifying it directly.

// "Enum" so that the string values don't have to be used throughout the code.
// The order of the enums doesn't really matter, but I put the date range first
// since a user might want to manually select a specific date range, and we
// don't provide any other easy way to do that right now.
OptionsEnum = {
  STARTDATE      : 'startdate',
  ENDDATE        : 'enddate',
  CONFIGURATIONS : 'configs',
  GRAPHS         : 'graphs',
  SUITE          : 'suite'
}


// get an object of options from the current query string
function getOptions() {
  var options = {};
  for (option in OptionsEnum) { options[OptionsEnum[option]] = '';}

  var queryString = document.location.search.slice(1);
  var queryStrings = queryString.split('&');
  for (var i = 0; i < queryStrings.length; i++) {
    var curOption = queryStrings[i].split('=');
    var curOptionName = curOption[0];
    var curOptionValue = curOption[1];
    options[curOptionName] = curOptionValue;
  }
  return options;
}


// get a specific option from the query string. returns empty string if option
// was not in the query string
function getOption(optionName) {
  var options = getOptions();
  if (optionName in options) {
    return options[optionName];
  }
  return '';
}


// set the query string based on the set of options passed in
function setQueryStringFromOptions(options) {
  var baseURL = document.location.href.split('?')[0];
  var queryString = '?';

  for (var option in options) {
    if (options[option]) {
      queryString += option+'='+options[option]+'&';
    }
  }

  queryString = queryString.removeTrailingChar();
  history.replaceState(null, null, baseURL + queryString);
}


// set the query string based on the particular option. This only
// overrides/adds the option specified without changing the rest of the options
function setQueryStringFromOption(option, optionValue) {
  var options = getOptions();
  options[option] = optionValue;
  setQueryStringFromOptions(options);
}



//////////////////////
// Helper functions //
//////////////////////


// Remove a trailing character from a string. Removes any character by default,
// or only the specified character if one is passed.
String.prototype.removeTrailingChar = function(charToRemove) {
  charToRemove = defaultFor(charToRemove, '.');
  var re = new RegExp(charToRemove +'$');
  return this.replace(re, '');
}


// Small helper function to implement string endsWith. Do not redefine if a
// browser has a (presumably faster) built in version
if (typeof String.prototype.endsWith !== 'function') {
  String.prototype.endsWith = function(suffix) {
    return this.indexOf(suffix, this.length - suffix.length) !== -1;
  };
}


// helper function to round a date up or down (for the purposes on zooming in
// on date boundaries only.) e.g. 01/01/01 and 2 hours will either round down
// to exactly 01/01/01 or up to 01/02/01 since our data comes in once a day
// only and this makes storing the date range easier in the URL. date is in
// milliseconds since epoch
function roundDate(date, roundUp) {
  if (numericX) {
    return date;
  } else {
    // Dygraph.dateString returns the date in the form "YYYY/MM/DD hh:mm::ss"
    // We simply split on the space, and re-parse just the YYYY/MM/DD part to
    // get an exact date boundary. If we're rounding up we add one day in
    // millis - 1 milli to the date in order to round it to the next day for
    // any time of the day unless it was already on a date boundary
    var nearlyADayInMillis = 24 * 60 * 60 * 1000 - 1;
    if (roundUp) {
      var dateString = Dygraph.dateString_(date+nearlyADayInMillis).split(' ');
    } else {
      var dateString = Dygraph.dateString_(date).split(' ');
    }
    return parseDate(dateString[0]);
  }
}


// helper function to parse a date (either use dygraph date parser, or do
// nothing for numericX)
function parseDate(date) {
  if (numericX) {
    return date;
  } else {
    return Dygraph.dateParser(date);
  }
}


// provides a semi clean way to allow default function arguments
function defaultFor(arg, defaultVal) {
  return typeof arg !== 'undefined' ? arg : defaultVal;
}


// removes all non-alphanumeric characters from a string and converts to
// lowercase
function normalizeForURL(str) {
  // pull regex out of replace so it gets precompiled
  var nonAlphaNumRegex = /[^a-z0-9]/g;
  // convert to lower case first so regex doesn't have to be case insensitive
  return str.toLowerCase().replace(nonAlphaNumRegex, '');
}
