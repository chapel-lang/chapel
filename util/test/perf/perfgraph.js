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
                    "revision" : "r21260"}
                  ];
// stuff for dygraph
var gs = []; // array of current graphs
var blockRedraw = false;
var initialize = false;
var numGraphs = 0;

function genDygraph(graphInfo, parent, legend) {
    var gspacer = document.createElement('div');
    gspacer.className = 'gspacer';
    var div = document.createElement('div');
    parent.appendChild(gspacer);
    div.className = 'perfGraph';
    parent.appendChild(div);
    var lspacer = document.createElement('div');
    lspacer.className = 'lspacer';
    var ldiv = document.createElement('div');
    legend.appendChild(lspacer);
    ldiv.className = 'perfLegend';
    legend.appendChild(ldiv);
    // Date.parse with our date format (Subset of ISO 8601) will return UTC time
    // so we'll need to adjust the start/end time to our local time
    var date = new Date();
    var localOffset = date.getTimezoneOffset() * 60 * 1000;
    var startTime = Date.parse(graphInfo.startdate) + localOffset;
    var endTime = Date.parse(graphInfo.enddate) + localOffset;
    var g = new Dygraph(div,
                        'CSVfiles/'+graphInfo.datfname,
                        {
                            title: graphInfo.title,
                            ylabel: graphInfo.ylabel,
                            drawXGrid: false,
                            drawYGrid: true,
                            includeZero: true,
                            showRoller: true,
                            legend: 'always',
                            // the options for the highlighted series 
                            highlightSeriesOpts: {    
                              strokeWidth: 2,
                              strokeBorderWidth: 0,
                              highlightCircleSize: 4,
                            },
                            // don't "dim" the  series when one is highlighted
                            highlightSeriesBackgroundAlpha: 1, 
                            // So it's easier to zoom in on the right side
                            rightGap: 15, 
                            // Give yourself a little more space so the y axis
                            // values don't overlap with the y label
                            yAxisLabelWidth: 70,
                            labelsDiv: ldiv,
                            labelsSeparateLines: true,
                            dateWindow: [startTime, endTime],
                            drawCallback: function(me, initial) {
                                if (blockRedraw) return;
                                blockRedraw = true;
                                
                                // Find the range we're displaying and adjust 
                                // the number of decimals accordingly, the set
                                // annotations is just because it wasn't 
                                // actually updating the graph for some reason.
                                // In the dygraph docs, this is how I saw you
                                // could redraw the graph with no side effects, 
                                // but there might be a better solution 
                                var yRange = me.yAxisRange();
                                var yDiff = yRange[1] - yRange[0];
                                if (yDiff > 100.0) {
                                    me.updateOptions({digitsAfterDecimal: 0} );
                                    me.setAnnotations(me.annotations());
                                }
                                else if (yDiff > 1.0) {
                                    me.updateOptions({digitsAfterDecimal: 2} );
                                    me.setAnnotations(me.annotations());
                                }
                                else {
                                    me.updateOptions({digitsAfterDecimal: 4} );
                                    me.setAnnotations(me.annotations());
                                }
                                
                                if(!initial) {  
                                    var range = me.xAxisRange();
                                    <!-- var yrange = me.yAxisRange(); -->
                                    for (var j = 0; j < numGraphs; j++) {
                                        if (gs[j] == me) continue;
                                        gs[j].updateOptions( {
                                                dateWindow: range
                                                <!-- valueRange: yrange -->
                                                 } )
                                    }
                                }
                                blockRedraw = false;
                            },
                            underlayCallback: function(canvas, area, g) {
                                function markReleaseDate(date) {
                                    var mDate = Date.parse(date) + localOffset;
                                    var xval = g.toDomXCoord(new Date(mDate));
                                    canvas.beginPath();
                                    canvas.moveTo(xval, area.y);
                                    canvas.lineTo(xval, area.y+area.h);
                                    canvas.strokeStyle = '#e0af1b';
                                    canvas.stroke();
                                }
                                for (var i = 0; i < branchInfo.length; i++) {
                                    markReleaseDate(branchInfo[i].branchDate);
                                }
                            }
                        }
                        )
    
    // If you click on a graph, focus on the currently highlighted series 
    // (highlighting isn't automatically switched, and don't change values 
    // back to newest on unhighlight.) This is nice for following a particular 
    // series. You can't do this as a regular option when the dygraph is created
    // because the dygraph isn't passed as an argument to the callback function
    // brad pointed out that it would be nice if you could enable this by
    // clicking on the legend entry 
    var onclick = function(ev) {
      if (g.isSeriesLocked()) {
        g.clearSelection();
      } else {
        g.setSelection(g.getSelection(), g.getHighlightSeries(), true);
      }
    };
    g.setSelection(false);
    g.updateOptions({clickCallback: onclick}, true);
    
    gs.push(g)
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
    fselect.setAttribute('onChange',  "jumpto(document.form1.jumpmenu.options[document.form1.jumpmenu.options.selectedIndex].value)");
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
    checkAll(true)
}

function unselectAllGraphs() {
    checkAll(false)
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
        if (allGraphs[i].suite == suite)
            elem.checked = true;
        else
            elem.checked = false;
    }
    displaySelectedGraphs()
}

function displaySelectedGraphs() {
    var parent = document.getElementById('graphdisplay');
    var legend = document.getElementById('legenddisplay');
    // Clean up divs
    while (parent.childNodes.length > 0) {
        parent.removeChild(parent.childNodes[0]);
        legend.removeChild(legend.childNodes[0]);
    }

    // clean up dygraph stuff
    for (var i = 0; i < numGraphs; i++) {
        gs.pop();
    }
    numGraphs = 0;
    for (var i = 0; i < allGraphs.length; i++) {
        var checkbox = document.getElementById('graph' + i);
        if (checkbox.checked) {
            numGraphs++;
        }
    }

    for (var i = 0; i < allGraphs.length; i++) {
        var checkbox = document.getElementById('graph' + i);
        if (checkbox.checked) {
            var elem = genDygraph(allGraphs[i], parent, legend);
        }
    }
}

