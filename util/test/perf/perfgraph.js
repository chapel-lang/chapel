// Release branch info
// For now, I'm not using the release or revision fields
var branchInfo = [
                  { "release" : "1.1",
                    "branchDate" : "2010-04-09",
                    "revision" : "r17089"},
                  { "release" : "1.2",
                    "branchDate" : "2010-10-14",
                    "revision" : "r17927"},
                  { "release" : "1.3",
                    "branchDate" : "2011-04-12",
                    "revision" : "r18702"},
                  { "release" : "1.4",
                    "branchDate" : "2011-10-11",
                    "revision" : "r19321"},
                  { "release" : "1.5",
                    "branchDate" : "2012-04-10",
                    "revision" : "r19962"},
                  { "release" : "1.6",
                    "branchDate" : "2012-10-08",
                    "revision" : "r20668"},
                  { "release" : "1.7",
                    "branchDate" : "2013-04-05",
                    "revision" : "r21261"}
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
    gs.push(new Dygraph(div,
                        'CSVfiles/'+graphInfo.datfname,
                        {
                            title: graphInfo.title,
                            ylabel: graphInfo.ylabel,
                            drawXGrid: false,
                            drawYGrid: true,
                            includeZero: true,
                            showRoller: true,
                            legend: 'always',
                            labelsDiv: ldiv,
                            labelsSeparateLines: true,
                            dateWindow: [ Date.parse(graphInfo.startdate),
                                          Date.parse(graphInfo.enddate)],
                            drawCallback: function(me, initial) {
                                if (blockRedraw || initial) return;
                                blockRedraw = true;
                                var range = me.xAxisRange();
                                <!-- var yrange = me.yAxisRange(); -->
                                for (var j = 0; j < numGraphs; j++) {
                                    if (gs[j] == me) continue;
                                    gs[j].updateOptions( {
                                            dateWindow: range
                                            <!-- valueRange: yrange -->
                                             } )
                                }
                                blockRedraw = false;
                            },
                            underlayCallback: function(canvas, area, g) {
                                function markReleaseDate(date) {
                                    var xval = g.toDomXCoord(new Date(date));
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
            )
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

