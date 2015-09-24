// Release branch info
// For now, I'm not using the release fields
var branchInfo = [
                  { "release" : "0.9",
                    "releaseDate": "2009-04-16",
                    "branchDate" : "2009-04-08", // no actual branch
                    "revision" : 15461 },
                  { "release" : "1.0",
                    "releaseDate": "2009-10-15",
                    "branchDate" : "2009-10-16",
                    "revision" : 16229 },
                  { "release" : "1.01",
                    "releaseDate": "2009-10-30",
                    "branchDate" : "2009-10-30",
                    "revision" : 16330 },
                  { "release" : "1.02",
                    "releaseDate": "2009-11-12",
                    "branchDate" : "2009-11-13",
                    "revision" : 16423 },
                  { "release" : "1.1",
                    "releaseDate": "2010-04-15",
                    "branchDate" : "2010-04-09",
                    "revision" : 17087},
                  { "release" : "1.1.1",
                    "releaseDate": "2010-07-08",
                    // TODO: Remove this branchDate; there was no branch for
                    //       1.1.1. (thomasvandoren, 2015-04-09)
                    "branchDate" : "2010-04-09",
                    "revision" : -1},
                  { "release" : "1.2",
                    "releaseDate": "2010-10-21",
                    "branchDate" : "2010-10-14",
                    "revision" : 17926},
                  { "release" : "1.3",
                    "releaseDate": "2011-04-21",
                    "branchDate" : "2011-04-12",
                    "revision" : 18701},
                  { "release" : "1.4",
                    "releaseDate": "2011-10-20",
                    "branchDate" : "2011-10-11",
                    "revision" : 19320},
                  { "release" : "1.5",
                    "releaseDate": "2012-04-19",
                    "branchDate" : "2012-04-10",
                    "revision" : 19961},
                  { "release" : "1.6",
                    "releaseDate": "2012-10-18",
                    "branchDate" : "2012-10-08",
                    "revision" : 20667},
                  { "release" : "1.7",
                    "releaseDate": "2013-04-18",
                    "branchDate" : "2013-04-05",
                    "revision" : 21260},
                  { "release" : "1.8",
                    "releaseDate": "2013-10-17",
                    "branchDate" : "2013-10-04",
                    "revision" : 22086},
                  { "release" : "1.9",
                    "releaseDate": "2014-04-17",
                    "branchDate" : "2014-04-07",
                    "revision" : 23144},
                  { "release" : "1.10",
                    "releaseDate": "2014-10-02",
                    "branchDate" : "2014-09-22",
                    "revision" : -1},
                  { "release" : "1.11",
                    "releaseDate": "2015-04-02",
                    "branchDate" : "2015-03-25",
                    "revision" : -1},
                  { "release" : "1.12",
                    "releaseDate": "2015-10-01",
                    "branchDate" : "2015-09-24",
                    "revision" : -1}
                  ];

var rebootDates = [
    "2014-06-21",
    "2014-07-19",
    "2014-08-16",
    "2014-09-20",
    "2014-10-18",
    "2014-11-15",
    "2014-12-20",
    "2015-01-17",
    "2015-02-21",
    "2015-03-21",
];

// NOTE: I wonder if it makes sense to calculate these rebootDates using
//       something like Datejs. (thomasvandoren, 2015-04-08)
//
//  https://cdnjs.com/libraries/datejs
//
/* E.g.
// Find the third Saturday of every month starting with rebootStartMonth and
// ending with today.
var rebootDates = [],

    // Starting with June 2014 (months are 0 based in JS).
    rebootStartMonth = new Date(2014, 5, 1),

    // Set curThirdDate to the third Saturday of the starting month.
    curThirdDate = rebootStartMonth.moveToNthOccurrence(6, 3);

while (curThirdDate.isBefore(Date.today())) {
    rebootDates.push(curThirdDate.toString("yyyy-MM-dd"));
    curThirdDate.addMonths(1).moveToNthOccurrence(6, 3);
}
*/

// array of currently displayed graphs
var gs = [];
// used to prevent multiple redraws of graphs when syncing x-axis zooms
var globalBlockRedraw = false;

// The main elements that all the graphs and graph legends will be put in
var parent = document.getElementById('graphdisplay');
var legend = document.getElementById('legenddisplay');

// setup the default configuration even if it's not multi-conf
var multiConfs = configurations.length != 0;
if (!multiConfs) { configurations = ['']; }
var defaultConfiguration = configurations[0];

// Experimental: used to toggle how stroke pattern and line colors are used for
// multi-configs. Default to using it for 16 node xc in a hacky way
var diffColorForEachConfig = pageTitle.indexOf("16 node XC") >= 0;

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

  // create a screenshot button and put it next to the annotation button
  var screenshotToggle = document.createElement('input');
  screenshotToggle.type = 'button';
  screenshotToggle.className = 'toggle';
  screenshotToggle.value = 'screenshot';
  screenshotToggle.style.visibility = 'hidden';
  gspacer.appendChild(screenshotToggle);

  return {
    div: div,
      ldiv: ldiv,
      logToggle: logToggle,
      annToggle: annToggle,
      screenshotToggle: screenshotToggle
  }
}


// Gen a new dygraph, if an existing graph is being expanded then expandInfo
// will contain the expansion information, else it is null
function genDygraph(graphInfo, graphDivs, graphData, graphLabels, expandInfo) {

  var div = graphDivs.div;
  var ldiv = graphDivs.ldiv;
  var logToggle = graphDivs.logToggle;
  var annToggle = graphDivs.annToggle;
  var screenshotToggle = graphDivs.screenshotToggle;

  var startdate = getDateFromURL(OptionsEnum.STARTDATE, graphInfo.startdate);
  var enddate = getDateFromURL(OptionsEnum.ENDDATE, graphInfo.enddate);
  startdate = parseDate(startdate);
  enddate = parseDate(enddate);

  // setup our options
  var graphOptions = {
    title: graphInfo.title,
    ylabel: graphInfo.ylabel,
    axes: {
      x: {
        drawGrid: false
      },
      y: {
        drawGrid: true,
        // So y values don't overlap with the y label
        axisLabelWidth: 80,
        valueFormatter: customValueFormatter,
        axisLabelFormatter: customAxisLabelFormatter
      }
    },
    includeZero: true,
    connectSeparatedPoints: true,
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
    labels: graphLabels,
    labelsDiv: ldiv,
    labelsSeparateLines: true,
    dateWindow: [startdate, enddate],
    // sync graphs anytime we pan, zoom, or at initial draw
    drawCallback: customDrawCallback,
    // mark the release dates on the graph before the chart gets drawn
    underlayCallback: markReleaseDates
  }

  if (multiConfs) {
    // grab just the series, ignoring 'Date'
    var graphSeries = graphLabels.slice(1);

    // make it so that the graph's colors and stroke pattern are the same for
    // each configuration of a series. e.g. 'series (conf1) series(conf2)' are
    // the same color and have the same stroke pattern
    graphOptions.colors = genSeriesColors(graphSeries);
    graphOptions.series = genPerSeriesStrokePattern(graphSeries, configurations);

    // set the initial visibility based on which configs are selected
    var disabledConfs = getCheckedConfigurations(false);
    var visibility = getVisibilityForConfigurations(graphLabels, disabledConfs);
    graphOptions.visibility = visibility;
  }

  if (expandInfo) {
    graphOptions.colors = expandInfo.colors;
  }

  // actually create the dygraph
  var g = new Dygraph(div, graphData, graphOptions);
  g.isReady = false;
  setupSeriesLocking(g);

  // The dygraph is now setting up and rendering. Once the graph is fully
  // drawn this ready state gets fired. We don't want to synchronize this
  // grpahs x-axis until it has been fully rendered, or we will be modifying
  // properties that don't exist yet. We use the isReady state to handle
  // that. We also make our buttons visible here that way they don't show up
  // before the graph does.
  g.ready(function() {
    g.divs = graphDivs;
    g.graphInfo = graphInfo;

    setupLogToggle(g, graphInfo, logToggle);
    setupAnnToggle(g, graphInfo, annToggle);
    setupScreenshotToggle(g, graphInfo, screenshotToggle);

    g.isReady = true;

    expandGraphs(g, graphInfo, graphDivs, graphData, graphLabels);

  });

  gs.push(g);
}

// Function to expand an existing graph. This leaves the original graph
// unchanged, and creates a new graph for each series in the original. Each new
// graph has all the configurations for the series.
//
// TODO remove the dependence on 'graph.getPropertiesForSeries' so this can be
// called without the original graph having to be fully rendered.
function expandGraphs(graph, graphInfo, graphDivs, graphData, graphLabels) {

  // if we don't need to expand just return
  if (!graphInfo.defaultexpand) { return; }

  // get a transposed version of the data, so we can easily grab series from it
  var transposedData = transpose(graphData);

  // expand graphs in reverse order. Allows us to  keep expanding after the
  // original graph's div instead of updating the div to place this graph after
  for (var i = graphLabels.length-1; i >= 0; i--) {
    // ignore non default confs, we grab them when we find the default conf
    if (graphLabels[i].endsWith(defaultConfiguration) == false) { continue; }

    // copy the graphInfo and add the key to the title (stripping the
    // configuration if we have multiple configurations.)
    var newInfo = $.extend({}, graphInfo);
    newInfo.title += ": " + graphLabels[i].replace(defaultConfiguration, '');

    // The new graph cannot be expanded
    newInfo.defaultexpand = false;

    // Grab the Date label and all the dates. Then grab the series name (label)
    // and the data for each config. Afterwards un-transpose the data so it's
    // formatted correctly. Also add the colors for the expanded graph as well
    var newLabels = graphLabels.slice(0, 1);
    var newData = transposedData.slice(0,1);
    var newColors = [];
    for (var j = 0; j < configurations.length; j++) {
      var confLabel = graphLabels[i].replace(defaultConfiguration, configurations[j]);
      var confIndex = graphLabels.indexOf(confLabel);
      if (confIndex >= 0) {
        newLabels.push(graphLabels[confIndex]);
        newData = newData.concat(transposedData.slice(confIndex, confIndex+1));
        newColors.push(graph.getPropertiesForSeries(graphLabels[confIndex]).color);
      } else {
        console.log('Warning: expected to find label "' + confLabel + '" for ' +
                    'graph "' + graphInfo.title + '" but it was missing');
      }
    }
    newData = transpose(newData);

    var newDivs = getNextDivs(graphDivs.div, graphDivs.ldiv);
    expandInfo = { colors: newColors }
    genDygraph(newInfo, newDivs, newData, newLabels, expandInfo);
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
      updateAnnotationsSeries(g);
      g.setAnnotations(g.graphInfo.annotations);
    } else {
      g.setAnnotations([]);
    }
  }
}


// Setup the screenshot button
function setupScreenshotToggle(g, graphInfo, screenshotToggle) {
  screenshotToggle.style.visibility = 'visible';

  screenshotToggle.onclick = function() {
    captureScreenshot(g, graphInfo);
  }
}


// Function to capture a screenshot of a graph and open the image in a new
// window.
//
// TODO: A nicer alternative would be to open a new window, and have that
// window create and render the screenshot and also have boxes to change the
// size of the rendered image. Right now it just defaults to making an image
// that is the same size as the actual graph.
//
// TODO: right now our graph and legend are in 2 separate divs so this is a
// little clunky because it renders each div separately and then combines them
// into a single canvas. It would be cleaner to have a div that wraps the graph
// and legend and just render that one.
function captureScreenshot(g, graphInfo) {

  var gWidth = g.divs.div.clientWidth + g.divs.ldiv.clientWidth;
  var gHeight = g.divs.div.clientHeight;

  var captureCanvas = document.createElement('canvas');
  captureCanvas.width = gWidth;
  captureCanvas.height = gHeight;
  var ctx = captureCanvas.getContext('2d');

  // html2canvas doesn't render transformed ccs3 text (like our ylabel.) We
  // make the label inivisible and we also hide the roll button box since
  // theres no point in capturing it in a screenshot
  g.updateOptions({showRoller: false, ylabel:''});
  var label = graphInfo.ylabel;

  // generate the graph
  html2canvas(g.divs.div, {
    // once the graph is rendered
    onrendered: function(graphCanvas) {
      // genenerate the legend
       html2canvas(g.divs.ldiv, {
        // once the legend is rendered
        onrendered: function(legendCanvas) {
          // draw the graph and legend canvas on a combined canvas.
          ctx.drawImage(graphCanvas, 0, 0);
          ctx.drawImage(legendCanvas, g.divs.div.clientWidth, 0);

          // get the graphs ylabel font properties
          var fontSize = g.getOption('axisLabelFontSize');
          ctx.font = '16px Arial';

          // rotate the canvas and draw the title
          ctx.translate(0, gHeight/2);
          ctx.rotate(-0.5*Math.PI);
          ctx.textAlign = 'center';
          ctx.fillText(label, 0, fontSize);

          // open the screenshot in a new window
          window.open(captureCanvas.toDataURL());

          // restore the roll box and ylabel
          g.updateOptions({showRoller: true, ylabel:label});
        }
      });
    }
  });
}


// Update which series the annotations for a graph are attached to based on the
// current configurations. Checks against disabled configs so we don't change
// annotations for graphs that aren't using multi-configs. This only changes
// the annotations in graphInfo, it does _not_ update the graph annotations
function updateAnnotationsSeries(g) {
  // if only one config or there's no annotations, nothing to update
  var annotations = g.graphInfo.annotations;
  var annLength = annotations.length;
  if (multiConfs === false || annLength === 0) return;

  // if all configs are hidden or all configs are visible, nothing to update
  var enabledConfs = getCheckedConfigurations();
  var disabledConfs = getCheckedConfigurations(false);
  if (enabledConfs.length === 0 || disabledConfs.length == 0) return;

  // note that all annotations attach themselves to the same series
  var firstAnn = annotations[0].series;

  // if ann series is a disabled config, replace it with an enabled one
  for (var i = 0, len = disabledConfs.length; i < len; i++) {
    if (firstAnn.endsWith(disabledConfs[i])) {
      var newSeries = firstAnn.replace(disabledConfs[i], enabledConfs[0]);
      for (var j = 0; j < annLength; j++) {
        annotations[j].series = newSeries;
      }
      break; // replaced the series, break out of disabledConfigs loop
    }
  }
}




// generate an object with an element for each series whose value is the stroke
// pattern for that series. Takes graphsSeries, which is the list of series for
// the graph and should not contain the 'Date'. Tries to use a different
// pattern for each configuration, but wraps around if there are more
// configurations than patterns. There's no need to use this function if
// multi-confs aren't being used, but it will work (all solid lines) if it is.
function genPerSeriesStrokePattern(graphSeries, configs) {

  // available stroke patterns for multi-conf, null means solid line
  var SOLID_LINE = null;
  var strokePatterns = [SOLID_LINE, Dygraph.DASHED_LINE, Dygraph.DOT_DASH_LINE, Dygraph.DOTTED_LINE ];

  var seriesOptions = {};
  // generate per series options
  if (multiConfs) {
    // same stroke pattern for each series (color differentiates configs)
    if (diffColorForEachConfig) {
      var counter = 0;
      for (var i = 0; i < graphSeries.length; i++) {
        if (configs.length > 0 && graphSeries[i].endsWith(configs[0])) {
          for (var j = 0; j < configs.length; j++) {
            var confLabel = graphSeries[i].replace(configs[0], configs[j]);
            var confIndex = graphSeries.indexOf(confLabel);
            if (confIndex >= 0) {
              var strokePattern = strokePatterns[counter%strokePatterns.length];
              seriesOptions[confLabel] = {'strokePattern': strokePattern};
            } else {console.log(confLabel);}
          }
          counter++
        }
      }
    // different stroke pattern for each config (color differentiates series)
    } else {
      for (var i = 0; i < graphSeries.length; i++) {
        seriesOptions[graphSeries[i]] = {'strokePattern': SOLID_LINE};
        for (var j = 0; j < configs.length; j++) {
          if (graphSeries[i].endsWith(configs[j])) {
            var strokePattern = strokePatterns[j%strokePatterns.length];
            seriesOptions[graphSeries[i]] = {'strokePattern': strokePattern};
          }
        }
      }
    }
  }
  return seriesOptions;
}


// generate a list of colors to use for multi-conf graphs. Takes graphsSeries
// which is the list of series for the graph and should not contain the 'Date'.
function genSeriesColors(graphSeries) {
  var colors = [];

  // attempts to produce colors that won't be offensive to the eyes but easy to
  // tell apart. Takes the current series and total number of series to make
  // sure similar colors aren't adjacent. Based on Dygraph's color generator
  function calcColor(cur, numSeries) {
    // decent defaults for saturation and value
    var sat = 1.0;
    var val = 0.5;

    // calculate a good hue by alternating series
    var half = Math.ceil(numSeries / 2);
    var idx = cur % 2 ? (half + (cur + 1)/ 2) : Math.ceil((cur + 1) / 2);
    var hue = (1.0 * idx / (1 + numSeries));

    // convert to an rgb value
    var colorStr = Dygraph.hsvToRGB(hue, sat, val);
    return colorStr;
  }

  // generate initial color, needed if multi-conf isn't being used, or if a
  // particular graph doesn't have series that are one of the configurations.
  for (var i = 0; i < graphSeries.length; i++) {
    colors[i] = calcColor(i, graphSeries.length);
  }

  // generate colors for multi-conf graphs
  if (multiConfs) {
    // Give each config a unique color (stroke pattern differentiates series)
    if (diffColorForEachConfig) {
      var configColors = [];
      for (var i = 0; i < graphSeries.length; i++) {
        for (var j = 0; j < configurations.length; j++) {
          if (graphSeries[i].endsWith(configurations[j])) {
            var colorStr = calcColor(j, configurations.length);
            colors[i] = colorStr
          }
        }
      }
    // same color for all configs (stroke pattern differentiates configs)
    } else {
      var counter = 0;
      for (var i = 0; i < graphSeries.length; i++) {
        if (graphSeries[i].endsWith(defaultConfiguration)) {
          var numColors = Math.ceil(graphSeries.length / configurations.length);
          var colorStr = calcColor(counter, numColors);
          for (var j = 0; j < configurations.length; j++) {
            var confLabel = graphSeries[i].replace(defaultConfiguration, configurations[j]);
            var confIndex = graphSeries.indexOf(confLabel);
            if (confIndex >= 0) {
              colors[confIndex] = colorStr;
            }
          }
          counter++
        }
      }
    }
  }
  return colors;
}


// We use a custom value formatter so that we can adjust the number of digits
// displayed based on min and max y values. This makes the graphs look a lot
// cleaner, especially since many of our graphs have widely varying y axis
// ranges. e.g. you don't care if a test takes 500.21 vs 500.29 seconds, but do
// care about 0.21 vs 0.29 seconds.
//
// Previously we did this in the zoom callback, but that forced us to re-render
// the dygraph which is slow. This adds some overhead to updating the value
// displayed in the label and legend, but there doesn't appear to be any
// performance issues.
function customValueFormatter(val, opts, series_name, dygraph) {

  // Find the range we're displaying and adjust digits accordingly
  var yRange = dygraph.yAxisRange();
  var yDiff = yRange[1] - yRange[0];
  var digits = 0;
  if (yDiff < 1.0) {
    digits = 4;
  } else if (yDiff < 100.0) {
    digits = 2;
  } else if (yDiff < 1000.0) {
    digits = 1;
  } else if (yDiff < 1000000.0) {
    digits = 0;
  } else {
    digits = 2;
  }

  // update digits, but do NOT redraw. Then use the default value formatter
  dygraph.updateOptions({digitsAfterDecimal: digits}, true);
  return Dygraph.numberValueFormatter(val, opts);
}

// custom formatter for the y axis labels, calls the legend value formatter
function customAxisLabelFormatter(val, granularity, opts, dygraph) {
  return customValueFormatter(val, opts, '', dygraph);
}


// synchronize our graphs along the x-axis and check if we should warn that
// using a log scale will result in wonky behavior.
function customDrawCallback(graph, initial) {
  if (globalBlockRedraw) return;

  // if a user has explicitly zoomed in on zero or negative value and they
  // attempt to take the log the graph will not render. This is a known
  // limitation of dygraphs. If the user has not zoomed, the graph will
  // automatically adjust the y display range, but if they have explicitly
  // requested a range, it will keep the same range for the log scale and
  // will attempt to take the log of zero.
  if (!initial) {
    var yRange = graph.yAxisRange();
    if (yRange[0] <= 0 && graph.isZoomed('y')) {
      graph.divs.logToggle.style.color = 'red';
    } else {
      graph.divs.logToggle.style.color = 'black';
    }
  }

  // if this isn't the initial draw, and this graph is fully rendered then
  // sync this graphs x-axis with all other ready graphs along the x-axis
  if (!initial && graph.isReady) {
    var range = graph.xAxisRange().slice();
    range[0] = roundDate(range[0], false);
    range[1] = roundDate(range[1], true);

    setURLFromDate(OptionsEnum.STARTDATE, Dygraph.dateString_(range[0]));
    setURLFromDate(OptionsEnum.ENDDATE, Dygraph.dateString_(range[1]));

    applyFnToAllGraphs(function(g) {
      if (g.isReady && differentDateRanges(range, g.xAxisRange())) {
        g.updateOptions({ dateWindow: range });
      }
    });
  }
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
    // TODO: Check that branchDate is a member of the object. (thomasvandoren, 2015-04-09)
    // E.g. with lodash.js or Underscore.js:
    //
    // if (_.has(branchInfo[i], "branchDate")) { ...
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

  var todayDate = getTodaysDate();

  // if the graphs weren't synced today let the user know
  var dateElem= document.getElementById('dateElem');
  if(parseDate(runDate) < parseDate(todayDate)) {
    dateElem.innerHTML = 'Graphs Last Updated on ' + runDate;
    dateElem.style.color = "RED";
  }

  // generate the multi configuration menu and toggle options
  var toggleConf = document.getElementById('toggleConf');
  if (multiConfs) {
    var queryStringConf = getOption(OptionsEnum.CONFIGURATIONS)
      if (queryStringConf) {
        var setConfigurations = queryStringConf.split(',');
      } else {
        var setConfigurations = configurationsVis;
        for (var j = 0; j < setConfigurations.length; j++) {
          setConfigurations[j] = normalizeForURL(setConfigurations[j]);
        }

      }

    for (var i = 0; i < configurations.length; i++) {
      var elem = document.createElement('div');
      var configuration = configurations[i];
      elem.className = 'graph';
      elem.innerHTML = '<input id="hide' + i + '"type="checkbox">' + configuration;
      toggleConf.appendChild(elem);
      var checkBox = document.getElementById('hide' + i);
      if (setConfigurations.indexOf(normalizeForURL(configuration)) >= 0) {
        checkBox.checked = true;
      } else {
        checkBox.checked = false;
      }
      checkBox.onchange = function() {
        // even if the checked configs match the 'default' configs put them in
        // the URL since the default configs could change over time.
        var configsURL = normalizeArrayForURL(getCheckedConfigurations());
        setQueryStringFromOption(OptionsEnum.CONFIGURATIONS, configsURL);

        applyFnToAllGraphs(function(g) {
          // attach annotations to visible series, if not already.
          // suppressDraw, setConfigurationVisibility will draw the graph
          if (g.annotations().length > 0) {
            updateAnnotationsSeries(g)
            g.setAnnotations(g.graphInfo.annotations, true);
          }
          setConfigurationVisibility(g);
        });
      };
    }

    // Experimental: Add buttons to reset stroke patters and to invert stroke
    // pattern / colors
    addExperimentalButtons(toggleConf);
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
    var suiteName = perfSuites[i].suite;
    if (suiteName.trim() === '<empty>') {
      suiteName = '';
      o.disabled = true;
    }
    o.innerHTML = suiteName.replace(/ /g, '&nbsp');
    o.setAttribute('value', suiteName);
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

  setupGraphSelectionPane();

  setGraphsFromURL();
  displaySelectedGraphs();
}


// We use 'fixed' css positioning to keep the graph selection pane always
// visible (scrolls when the page scrolls.) However we don't want it to scroll
// horizontally to so we move it when horizontal scross occur. Since it scrolls
// veritcally we also need to make sure it fits in the page height. This sets
// the initial dimensions and then listens for scrolling and resizes
function setupGraphSelectionPane() {
  $(window).scroll(function(){
    setGraphSelectionPanePos();
  });

  $(window).ready(function(){
    setDygraphPanePos();
    setGraphListHeight();
    setGraphSelectionPanePos();
  });

  $(window).resize(function(){
    setDygraphPanePos();
    setGraphListHeight();
    setGraphSelectionPanePos();
  });

  // We don't know the width of the graph selection pane until the list of
  // graph names are added. Once that is done figure out the position of pane
  // that holds the dygraphs. This is needed since the graph selection pane
  // is 'fixed' meaning it's outside the normal flow and other elements act
  // like it doesn't exist so we have to manually move the graph display to
  // avoid overlap. After the page is setup, the graph selection pane size
  // only changes if the browser zoom changes.
  function setDygraphPanePos() {
    var selectPaneWidth = parseInt($("#graphSelectionPane").outerWidth());
    $('#graphdisplay').css({ 'margin-left': selectPaneWidth });
  }

  // set the selection pane's horizontal positional based on the scroll so the
  // selection pane isn't always visible when scrolling horizontally. Some
  // browsers allow scrolling past the edge of the screen so we limit the
  // scroll amount to prevent weird overlaps.
  function setGraphSelectionPanePos() {
    // number of pixels hidden from view to the left visible window
    var scrollLeft = $(window).scrollLeft();
    // number of pixels hidden from view ('true' page size - visible amount)
    var numHiddenPixels = $(document).width() - $(window).width();

    scrollLeft = Math.max(0, scrollLeft);
    scrollLeft = Math.min(numHiddenPixels, scrollLeft);

    // move pane the opposite of scrolling so it moves out of view
    $('#graphSelectionPane').css({ 'left': -scrollLeft });
  }

  // determine the height to use for the graphlist. We want it to use most of
  // the rest of the page. We use 90% of the height between the top of the
  // graphlist and the bottom of the page (with a min of 100 pixels.)
  function setGraphListHeight() {
    var topPos = parseInt($("#graphlist").offset().top) - $(window).scrollTop();
    var w = $(window).height();
    var height = (w-topPos)*.9;
    if (height < 100) { height = 100;}
    $('#graphlist').css({ 'height': height });
  }
}


// Sets which configurations should be visible when there are multiple
// configurations available. e.g. --local vs --no-local
function setConfigurationVisibility(graph) {
  var labels = graph.getLabels().slice();
  var disabledConfs = getCheckedConfigurations(false);
  var visibility = getVisibilityForConfigurations(labels, disabledConfs);
  graph.updateOptions({visibility: visibility});
}

function getCheckedConfigurations(checked_status) {
  checked_status = defaultFor(checked_status, true);

  var configs = [];
  for (var i = 0; i < configurations.length; i++) {
    var checkBox = document.getElementById('hide' + i);
    if (checkBox.checked === checked_status) {
      configs.push(configurations[i]);
    }
  }
  return configs;
}

function getVisibilityForConfigurations(graphLabels, disabledConfigurations) {
  var graphSeries = graphLabels.slice(1);
  var visibility = [];
  for (var i = 0; i < graphSeries.length; i++) {
    visibility[i] = true;
    for (var j = 0; j < disabledConfigurations.length; j++) {
      if (graphSeries[i].endsWith(disabledConfigurations[j])) {
        visibility[i] = false;
      }
    }
  }
  return visibility;
}


// simple wrapper to set the date in the URL from a supplied date.
function setURLFromDate(whichDate, date) {
  date = defaultFor(date, '');
  if (whichDate !== OptionsEnum.STARTDATE && whichDate !== OptionsEnum.ENDDATE) {
    console.log('setURLFromDate can only take STARTDATE and ENDDATE');
  }

  // If the date was the current date, use the sentinel 'today' instead.
  // NOTE: Currently disabled, we're not sure if we like this policy
  /*if (date && (parseDate(date) == parseDate(getTodaysDate()))) {
    date = 'today';
  }*/

  setQueryStringFromOption(whichDate, date);
}


// simple wrapper to get the date from the URL. Accepts a defaultDate if the
// date isn't in the URL. Also looks for sentinel dates such as 'today'
function getDateFromURL(whichDate, defaultDate) {
  defaultDate = defaultFor(defaultDate, '');
  if (whichDate !== OptionsEnum.STARTDATE && whichDate !== OptionsEnum.ENDDATE) {
    console.log('getDateFromURL can only be asked for STARTDATE and ENDDATE');
    return '';
  }

  var dateString = getOption(whichDate);

  if (dateString === '')
    return defaultDate;

  if (dateString === normalizeForURL('today'))
    return getTodaysDate();

  return dateString;
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
    var curGraphs = [];
    for (var i = 0; i < allGraphs.length; i++) {
      var checkBox = document.getElementById('graph' + i);
      if (checkBox.checked) {
        curGraphs.push(allGraphs[i].title);
      }
    }

    // special case for if all graphs are selected. probably not needed
    // but this is the most common case where a lot of graphs are selected
    // and for some browsers we might be exceeding a url length limit
    if (allGraphs.length === curGraphs.length) {
      curGraphs = ['all'];
    }

    var curGraphsURL = normalizeArrayForURL(curGraphs);
    setQueryStringFromOption(OptionsEnum.GRAPHS, curGraphsURL);
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
  setURLFromDate(OptionsEnum.STARTDATE, '');
  setURLFromDate(OptionsEnum.ENDDATE, '');

  // Reset the display range for each graph
  applyFnToAllGraphs(function(g) {
    var start = parseDate(g.graphInfo.startdate);
    var end = parseDate(g.graphInfo.enddate);
    var range = [start, end];
    if (differentDateRanges(range, g.xAxisRange)) {
      g.updateOptions({ dateWindow: range });
    }
  });
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


function selectSuite(suite) {
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
      getDataAndGenGraph(allGraphs[i]);
    }
  }

  // update the url for the displayed graphs
  setURLFromGraphs(normalizeForURL(findSelectedSuite()));
  // set the dropdown box selection
  document.getElementsByName('jumpmenu')[0].value = findSelectedSuite();
}


// Load the data, and create a new dygraphs
function getDataAndGenGraph(graphInfo) {
  var dataFile = 'CSVfiles/'+graphInfo.datfname;

  // convert annotations to millis since epoch since we're using a native
  // array. We could do this in genGraphs, but then we have to think about
  // timezones and all that stuff, it's fast enough and far easier to just let
  // dygraphs do it
  if (!graphInfo.loadedAnnotations) {
    var ann = graphInfo.annotations;
    for (var i=0; i<ann.length; i++) {
      ann[i].x = parseDate(ann[i].x);
    }
    graphInfo.loadedAnnotations = true;
  }

  // need to get the divs before the async call to get the json so graphs are
  // displayed in the order they are listed, regardless of the order they are
  // loaded.
  var graphDivs = getNextDivs();
  var json = $.getJSON(dataFile)
    .done( function(json) {
      var graphData = json.data;
      var graphLabels = json.labels;
      for (var j = 0; j < graphData.length; j++) {
        graphData[j][0] = new Date(parseDate(graphData[j][0]));
      }
      genDygraph(graphInfo, graphDivs, graphData, graphLabels);
    })
    .fail( function(jqxhr, textStatus, error) {
      var err = textStatus + ', ' + error;
      console.log( 'Request for ' + dataFile + ' Failed: ' + err );
    });
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

  // if no graphs were selected, return no suite sentinel
  if (displayedGraphs.length === 0) { return NO_SUITE; }

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
  queryString = decodeURIComponent(queryString);
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


// Apply an arbitrary function to all graphs. The function being applied can
// only have a single graph as it's argument. Redraws are blocked by default.
// Useful so you don't forget to block redraws, which can kill performance.
function applyFnToAllGraphs(fnToApply, blockRedraw) {
  blockRedraw = defaultFor(blockRedraw, true);
  var oldGlobalBlockRedraw = globalBlockRedraw;

  globalBlockRedraw = blockRedraw;
  var gsLength = gs.length;
  for (var i = 0; i < gsLength; i++) {
    fnToApply(gs[i]);
  }
  globalBlockRedraw = oldGlobalBlockRedraw;
}


// Transpose a 2 dimensional array
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


// returns todays date formatted as 'YYYY<delimiter>MM<delimiter>DD'. Defaults
// to 'YYYY-MM-DD' if a delimiter isn't specified.
function getTodaysDate(delimiter) {
  delimiter = defaultFor(delimiter, '-');
  var d = new Date();
  return  d.getFullYear() + delimiter + (d.getMonth()+1) + delimiter + d.getDate();
}


// simple wrapper to check if two date ranges are different
function differentDateRanges(rangeOne, rangeTwo) {
  return rangeOne.toString() !== rangeTwo.toString();
}


// provides a semi clean way to allow default function arguments
function defaultFor(arg, defaultVal) {
  return typeof arg !== 'undefined' ? arg : defaultVal;
}


// normalizes each element of the array and converts the array to a comma
// separated string. Does not modify original array
function normalizeArrayForURL(arr) {
  // slice, don't modify original array
  var copy = arr.slice()
  for (var i = 0 ; i < copy.length; i++) {
    copy[i] = normalizeForURL(copy[i]);
  }
  return copy.toString();
}


// removes all characters that are not alphanumeric and converts to lowercase
function normalizeForURL(str) {
  // pull regex out of replace so it gets precompiled
  var nonAlphaNumRegex = /[^a-z0-9]/g;
  // convert to lower case first so regex doesn't have to be case insensitive
  return str.toLowerCase().replace(nonAlphaNumRegex, '');
}



////////////////////////////
// Experimental functions //
////////////////////////////


// Experimental: Swap how color and stroke pattern are used to differentiate
// different configs vs. different series on a graph
function invertMultiConfigStrokeAndColor() {

  diffColorForEachConfig = !diffColorForEachConfig;

  var curConfigs = getCheckedConfigurations();
  applyFnToAllGraphs(function(g) {
    var graphSeries = g.getLabels().slice(1);
    var seriesOpts = genPerSeriesStrokePattern(graphSeries, curConfigs);
    var colors = genSeriesColors(graphSeries);
    g.updateOptions({ series: seriesOpts, colors: colors });
  });
}


// Experimental: reset the stoke pattern for all graphs based on the currently
// displayed configurations
function resetStrokePattern() {
  var curConfigs = getCheckedConfigurations();
  applyFnToAllGraphs(function(g) {
    var graphSeries = g.getLabels().slice(1);
    var seriesOpts = genPerSeriesStrokePattern(graphSeries, curConfigs);
    g.updateOptions({ series: seriesOpts });
  });
}

// Experimental: Add buttons to reset stroke patters and to invert stroke
// pattern / colors
function addExperimentalButtons(toggleConf) {
  if (configurations.length >= 3) {
    var strokePatternToggle = document.createElement('input');
    strokePatternToggle.type = 'button';
    strokePatternToggle.value = 'Reset Stoke Patterns';
    toggleConf.appendChild(strokePatternToggle);
    strokePatternToggle.onclick = function() {
      resetStrokePattern();
    }

    var linebreak = document.createElement("br");
    toggleConf.appendChild(linebreak);

    var invertStrokeColorToggle = document.createElement('input');
    invertStrokeColorToggle.type = 'button';
    invertStrokeColorToggle.value = 'Invert Line Stroke/Color';
    toggleConf.appendChild(invertStrokeColorToggle);
    invertStrokeColorToggle.onclick = function() {
      invertMultiConfigStrokeAndColor();
    }
  }
}
