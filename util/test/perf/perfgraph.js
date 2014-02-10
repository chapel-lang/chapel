// Release branch info
// For now, I'm not using the release or revision fields
var branchInfo = [
                  { "release" : "0.9",
                    "branchDate" : "2009-04-08", // no actual branch
                    "revision" : "r15461" },
                  { "release" : "1.0",
                    "branchDate" : "2009-10-16",
                    "revision" : "r16229" },
                  { "release" : "1.01",
                    "branchDate" : "2009-10-30",
                    "revision" : "r16330" },
                  { "release" : "1.02",
                    "branchDate" : "2009-11-13",
                    "revision" : "r16423" },
                  { "release" : "1.1",
                    "branchDate" : "2010-04-09",
                    "revision" : "r17087"},
                  { "release" : "1.2",
                    "branchDate" : "2010-10-14",
                    "revision" : "r17926"},
                  { "release" : "1.3",
                    "branchDate" : "2011-04-12",
                    "revision" : "r18701"},
                  { "release" : "1.4",
                    "branchDate" : "2011-10-11",
                    "revision" : "r19320"},
                  { "release" : "1.5",
                    "branchDate" : "2012-04-10",
                    "revision" : "r19961"},
                  { "release" : "1.6",
                    "branchDate" : "2012-10-08",
                    "revision" : "r20667"},
                  { "release" : "1.7",
                    "branchDate" : "2013-04-05",
                    "revision" : "r21260"},
                  { "release" : "1.8",
                    "branchDate" : "2013-10-04",
                    "revision" : "r22086"}
                  ];

// stuff for dygraph
var gs = []; // array of current graphs
var blockRedraw = false;
var numGraphsReady = 0;

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
    logToggle.className = 'logToggle';
    logToggle.value = 'log';
    logToggle.style.visibility = 'hidden';
    gspacer.appendChild(logToggle);

    return {
        div: div,
        ldiv: ldiv,
        logToggle: logToggle
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
        dateWindow: [Dygraph.dateParser(graphInfo.startdate),
                     Dygraph.dateParser(graphInfo.enddate)],
        // sync graphs anytime we pan, zoom, or at initial draw
        drawCallback: customDrawCallback,
        // mark the release dates on the graph before the chart gets drawn
        underlayCallback: markReleaseDates
    }

    if (expandInfo) {
        graphOptions.visibility = expandInfo.visibility;
    }

    // actually create the dygraph
    var g = new Dygraph(div, 'CSVfiles/'+graphInfo.datfname, graphOptions);
    g.sort = graphInfo.sort;
    setupSeriesLocking(g);

    // The dygraph is now setting up and rendering. Once the graph is fully
    // drawn the ready state gets fired. We don't want to start synchronizing
    // graph zooms until all the graphs have been fully rendered and drawn,
    // or we will be modifying properties that don't actually exist yet.
    // We use numGraphsReady to handle that. Also make the log button visible
    // after the graph is drawn. Expansion occurs here. For expansion we need
    // the graph to be mostly initialized since we grab its labels and change
    // is visibility. We might be able to move expandgraphs to right after
    // sorting, but we would need to pass the divs and graphinfo to the
    // callback
    g.ready(function() {
        setupLogToggle(g, graphInfo, logToggle);
        numGraphsReady += 1;
        expandGraphs(graphInfo, g, div, ldiv);
    });

    gs.push(g);
}

// Function to expand an existing graph. graphInfo and graph are the existing
// graph and div/ldiv are the divs for that graph
function expandGraphs(graphInfo, graph, div, ldiv) {
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

    // modify the current graphs visibility if only some of the items are being
    // expanded
    var visibility = graph.visibility();
    for (var i = expandNum; i < labels.length; i++) {
      visibility[i] = false;
    }
    graph.updateOptions({visibility: visibility});

    // for each expanded graph
    for (var index = expandNum - 1;  index >= 0 ; index--) {
        // copy the graphInfo and add the key to the title
        var newInfo = {};
        for (info in graphInfo) {
            newInfo[info] = graphInfo[info];
        }
        newInfo.title = newInfo.title + ": " + labels[index + 1];
        newInfo.expand = 0;
        // gen the expanded graph with visibility set for the current graph
        for (var i = 0; i < expandNum; i++) { visibility[i] = false; }
        visibility[index] = true;
        expandInfo = {
            afterDiv: div,
            afterLDiv: ldiv,
            visibility: visibility.slice()
        }
        genDygraph(newInfo, expandInfo);
    }
}

// Setup the logging button
function setupLogToggle(g, graphInfo, logToggle) {
    if (graphInfo.unloggable) {
        logToggle.style.color='red';
    }
    logToggle.style.visibility = 'visible';

    logToggle.onclick = function() {
        var useLog = !g.getOption('logscale');
        if (useLog) logToggle.value = 'linear';
        else        logToggle.value = 'log';
        g.updateOptions({ logscale: useLog });
    }
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
    // to parse the data ourselves would be a real pain.
    if (initial && g.sort) {
        g.rawData_ = sortData(g);
        g.setAnnotations(g.annotations());
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

    // if this isn't the initial draw, and all our graphs have been fully
    // created then synchronize our graphs along the x-axis
    if (!initial && numGraphsReady === gs.length) {
        var range = g.xAxisRange();
        for (var j = 0; j < gs.length; j++) {
            if (gs[j] === g || range.toString() ===
                gs[j].xAxisRange().toString()) continue;
            gs[j].updateOptions({ dateWindow: range });
        }
    }
    blockRedraw = false;
}


// Mark all the release dates on the canvas with vertical lines
function markReleaseDates (canvas, area, g) {
    function markReleaseDate(date) {
        var xval = g.toDomXCoord(Dygraph.dateParser(date));
        canvas.beginPath();
        canvas.moveTo(xval, area.y);
        canvas.lineTo(xval, area.y + area.h);
        canvas.strokeStyle = '#e0af1b';
        canvas.stroke();
    }
    for (var i = 0; i < branchInfo.length; i++) {
        markReleaseDate(branchInfo[i].branchDate);
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
        if (allGraphs[i].suite === suite) {
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

    // reset the ready state for the graphs
    numGraphsReady = 0;

    // generate the dygraph(s) for the currently selected graphs
    for (var i = 0; i < allGraphs.length; i++) {
        var checkbox = document.getElementById('graph' + i);
        if (checkbox.checked) {
            genDygraph(allGraphs[i]);
        }
    }
}

