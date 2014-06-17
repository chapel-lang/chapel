
""" 
Set up the plot figures, axes, and items to be done for each frame.

This module is imported by the plotting routines and then the
function setplot is called to set the plot parameters.
    
""" 

#--------------------------
def setplot(plotdata):
#--------------------------
    
    """ 
    Specify what is to be plotted at each frame.
    Input:  plotdata, an instance of pyclaw.plotters.data.ClawPlotData.
    Output: a modified version of plotdata.
    
    """ 

    plotdata.clearfigures()  # clear any old figures,axes,items data

    # Figure for q[0]
    plotfigure = plotdata.new_plotfigure(name='q[0]', figno=1)

    # Set up for axes in this figure:
    plotaxes = plotfigure.new_plotaxes()
    plotaxes.title = 'Solution'
    plotaxes.xlimits = 'auto'
    plotaxes.ylimits = [-1.2, 1.2]

    # Set up for item on these axes:
    plotitem = plotaxes.new_plotitem(plot_type='1d_plot')
    plotitem.plot_var = 0
    plotitem.plotstyle = '-o'
    plotitem.color = 'b'
    plotitem.show = True       # show on plot?
    
    # Parameters used only when creating html and/or latex hardcopy
    # e.g., via pyclaw.plotters.frametools.printframes:

    plotdata.printfigs = True                # print figures
    plotdata.print_format = 'png'            # file format
    plotdata.print_framenos = 'all'          # list of frames to print
    plotdata.print_fignos = 'all'            # list of figures to print
    plotdata.html = True                     # create html files of plots?
    plotdata.html_homelink = '../README.html'   # pointer for top of index
    plotdata.latex = True                    # create latex file of plots?
    plotdata.latex_figsperline = 2           # layout of plots
    plotdata.latex_framesperline = 1         # layout of plots
    plotdata.latex_makepdf = False           # also run pdflatex?

    return plotdata

    
