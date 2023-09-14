# How to use chpl_plot:
#
# chpl_plot is intended to be used in conjunction with chplExperiment. The idea
# is chplExperiment will run a number of experiments by calling out to 'gather'
# scripts that will dump '.dat' files into a 'logs/' directory. The .dat files
# in these directories contain tables of data.
#
# The simplest use of chpl_plot looks like this:
#
#    tables = load_tables()  # construct a dictionary of Table objects (one entry for .dat under 'logs/')
#    paint(data)             # produce '.png' files for each Table
#
# Other useful things to know:
#
# - If you want to print the tables in `.md` format you can use
# `print(tables.md())`.
#
# - Often times, you'll want to rearrange data before plotting it. The `join`
# function is particularly useful for this. It takes two Table objects that
# have the same row labels (but different column labels) and joins them into a
# new Table
#
# So for example, say, you had the following DAT files:
#
#       logs/feature_on.dat         lots/feature_off.dat
#
#          time_feature_on            time_feaure_off
#    2     12.2                   2   124.1
#    4     25.3                   4   258.9
#    8     40.2                   8   514.2
#    16    89.5                  16   1025.3
#    32    93.2                  32   2049.1
#
# and you wanted to make a single plot or table to compare the times in both
# files. You could do the following:
#
#     tables = load_tables()
#     jointTable = join(tables['feature_on'], tables['feature_off'])
#     print(jointTable.md())
#
# Which, if run would result in:
#
#     ### logs/feature_on.dat:logs/feature_off.dat
#     |      | time_feature_on | time_feaure_off |
#     | ---- | --------------- | --------------- |
#     |  2.0 |            12.2 |           124.1 |
#     |  4.0 |            25.3 |           258.9 |
#     |  8.0 |            40.2 |           514.2 |
#     | 16.0 |            89.5 |          1025.3 |
#     | 32.0 |            93.2 |          2049.1 |
#
# In cases of duplicate entries, column labels are uniquified by appending
# a number. So for example, the join of these three tables:
#
#         table_a            table_b           table_c
#
#     |      | time |    |      | time |    |      | time |
#     | ---- | ---- |    | ---- | ---- |    | ---- | ---- |
#     |  2.0 | 10.0 |    |  2.0 | 20.0 |    |  2.0 | 30.0 |
#     |  4.0 | 11.0 |    |  4.0 | 21.0 |    |  4.0 | 31.0 |
#     |  8.0 | 12.0 |    |  8.0 | 22.0 |    |  8.0 | 32.0 |
#     | 16.0 | 13.0 |    | 16.0 | 23.0 |    | 16.0 | 33.0 |
#     | 32.0 | 14.0 |    | 32.0 | 24.0 |    | 32.0 | 34.0 |
#
# would be this:
#
#     |      | time  | time_1 | time_2 |
#     | ---- | ----- | ------ | ------ |
#     |  2.0 |  10.0 |   20.0 |   30.0 |
#     |  4.0 |  11.0 |   21.0 |   31.0 |
#     |  8.0 |  12.0 |   22.0 |   32.0 |
#     | 16.0 |  13.0 |   23.0 |   33.0 |
#     | 32.0 |  14.0 |   24.0 |   34.0 |
#
# In such situations you may want to consider relabeling the columns before
# conducting the joins or you can use the relabel_col_w_tbl_title on the tables
# before conducting the join.
#
#
# F.A.Q.
#
#  (Q) The way my plot is being visualized isn't right. I need to tweak it,
#      what can I do?
#  (A) First look through this FAQ to see if another answer can give more
#      direct help. If not, consider storing the plot object to a variable
#      without saving it yet like so: `plot = tbl.plot(..., save=False)`
#      (where ... is the set of arguments you would pass to the `plot`
#      function otherwise). Once you have this, you can call various
#      functions on to modify it, and then call plot.save() to save it once
#      you're done.
#      
#      Take a look at the Plot API first to see if it has what you need.
#      If not you can access the underlying matplotlib objects `plot.ax` or
#      `plot.fig`.
#
#  (Q) While debugging, how can I see the content in my table object? Doing
#      `print(tbl)` shows me its a table object but doesn't list the data?
#  (A) Do: `print(tbl.md())`, "md" here means markdown and returns a string
#      of the table in markdown format.
#
#  (Q) Parts of my plot are clipped in my image (labels, etc.). What can I do?
#  (A) Get the plot object without saving it by calling
#      `p = plot(...., save=False)`. You can then forward 'tight' to
#       matplotlib's 'bbobx_unches' argument when saving the plot like so:
#       `p.save(bbox_inches='tight')`. If the ends up cutting other things
#       off you can try adding extra paddingl ike this:
#       `p.save(bbox_inches='tight', pad_inches=0.5)`
#
#  (Q) My legend isn't placed very nicely. I want to move it; what can I do?
#  (A) Separate out plotting and saving (as discussed in the previous question)
#      and you can set `plot.mpl_legend_args` to a dictionary for the kwargs
#      that would get passed to matplotlib's 'Axes.legend' function. Keep in
#      mind that this script will pass `legend` arguments for 'loc', 'fontsize',
#      and 'ncol', so if you do not want these to be passed you'll need to
#      overwrite them with 'None'. See here for more info about the aguments
#      that can be passed to Axes.legend:
#      https://matplotlib.org/stable/api/_as_gen/matplotlib.axes.Axes.legend.html
#      As an example, the following will give the legend 3 columns and center it
#      above and outside of the chart:
#
#        p.mpl_legend_args = { 'loc': 'center',
#                              'ncol': 3,
#                              'bbox_to_anchor': (0.5, 1.3) }



import os, re, codecs, shlex, copy, numbers, sys
from functools import reduce

import numpy as np
import matplotlib as mpl
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
import pathlib
from pprint import pprint
from enum import Enum

mpl.rcParams['text.usetex'] = False
mpl.rcParams['font.family'] = 'sans-serif'
mpl.rcParams['savefig.dpi'] = '300'

basefontsize = 24
titlepadding = 20
labelpadding = 20
markersize = 10
markeredgewidth = 3

def style(color, marker, linestyle):
  return {'color':color, 'linewidth':3, 'marker':marker, 'linestyle':linestyle }

COLOR_GREEN          = '#00A000'
COLOR_LIGHT_BLUE     = '#00A6DD'
COLOR_MED_BLUE       = '#3984C3'
COLOR_DARK_BLUE      = '#005696'
COLOR_VERY_DARK_BLUE = '#001FAB'
COLOR_DARK_RED       = '#A00000'

_chpl_plot_linestyles = {
    0 : {'color': 'gray'},

    1 : style( COLOR_GREEN,          'x', 'solid'),
    2 : style( COLOR_LIGHT_BLUE,     's', 'solid'),
    3 : style( COLOR_MED_BLUE,       'o', 'solid'),
    4 : style( COLOR_DARK_BLUE,      'D', 'solid'),
    40: style( COLOR_VERY_DARK_BLUE, '^', 'solid'),
    5 : style( COLOR_DARK_RED,       'x', 'solid'),

    11: style( COLOR_GREEN,          'x', 'dashed'),
    12: style( COLOR_LIGHT_BLUE,     's', 'dashed'),
    13: style( COLOR_MED_BLUE,       'o', 'dashed'),
    14: style( COLOR_DARK_BLUE,      'D', 'dashed'),
    140:style( COLOR_VERY_DARK_BLUE, '^', 'dashed'),
    15: style( COLOR_DARK_RED,       'x', 'dashed'),

    21: style( COLOR_GREEN,          'x', 'dotted'),
    22: style( COLOR_LIGHT_BLUE,     's', 'dotted'),
    23: style( COLOR_MED_BLUE,       'o', 'dotted'),
    24: style( COLOR_DARK_BLUE,      'D', 'dotted'),
    25: style( COLOR_DARK_RED,       'x', 'dotted')
}

def line_style(num):
  if num in _chpl_plot_linestyles:
    return _chpl_plot_linestyles[num]
  return _chpl_plot_linestyles[0]

class Arrow:
  """ Add an arrow to the plot indicating whether up is or down is better """
  def __init__(self, position, direction, color, text):
    if position not in ("right"):
      print(("Only 'right', arrows can be added. "
             "Defaulting to 'right'"))
      self.position = "right"
    else:
      self.position = position

    if direction not in ("up", "down"):
      print(("Only 'up', and 'down' arrows can be added. "
             "Defaulting to 'up'"))
      self.direction = "up"
    else:
      self.direction = direction

    if color not in ("green", "red", "blue"):
      print(("Only 'green', 'red' and 'blue' arrows can be added. "
             "Defaulting to 'blue'"))
      self.color = "blue"
    else:
      self.color = color

    self.text = text

  def gen_kwargs_dict(self):
    if self.direction == "up":
      boxstyle = "rarrow,pad=0.5"
    else:
      boxstyle = "larrow,pad=0.5"

    if self.color == "green":
      facecolor, edgecolor = "limegreen", "darkgreen"
    elif self.color == "red":
      facecolor, edgecolor = "red", "darkred"
    elif self.color == "blue":
      facecolor, edgecolor = "royalblue", "navy"

    bbox_args = dict(boxstyle=boxstyle,
                     facecolor=facecolor,
                     edgecolor=edgecolor,
                     linewidth=2)

    return dict(s=self.text,
                color="white",
                fontweight="bold",
                horizontalalignment="center",
                verticalalignment="center",
                rotation=90,
                size=basefontsize-4,
                bbox=bbox_args)


def _computePngFilename(table):
  """ Given a table, compute a filename to save for a plot. Specifically, use
  the same filename Name the '.png' file with the same as the filename the
  table came from (but with .dat changed to .png). If the table is joined from
  several files join filenames together with '__'. """
  png_file_name = ""
  cameFromLogsDir = False
  first = True
  for name in table.filename.split(':'):
    if name.startswith('logs/') or name.startswith('./logs/'):
      cameFromLogsDir = True
    short_name = os.path.basename(pathlib.Path(name).stem)
    if not first:
      png_file_name += "__"
    png_file_name += short_name
    first = False
  png_file_name += ".png"
  if cameFromLogsDir:
    png_file_name = "logs/" + png_file_name
    
  return png_file_name


PlotKind = Enum('PlotKind', ['BASIC', 'BAR'])
class Plot:
  """ Construct by passing the constructor a name for the plot as well as a
      list of values to use along the x axis.  If this list is numbers then we will
      produce a bar chart (otherwise a basic plot).

      To populate with data call the add_y_data function.

      This class has the following data members:

        self.plotKind
        self.fig
        self.ax
        self.mpl_legend_args
        self.legend_alphabetical

      There's also the following "private" members. If you need to add data
      use the 'add_y_data' function.

        self._x_data
        self._y_datas
        self._png_file_name 
        self._include_legend
        self._explicit_ylim
        self._arrows
        self._bars
  """

  def __init__(self, table, title=None, xlabel=None, ylabel=None, kind=None, includeLegend=True, better=None, shiftLineStyles=0, mpl_legend_args=None):
    self._x_data = table.xData
    self._y_datas = []
    self._png_file_name = _computePngFilename(table)
    self._include_legend = includeLegend
    self._explicit_ylim = False
    self._arrows = []
    self._bars = []
    self.mpl_legend_args = mpl_legend_args
    self.legend_alphabetical = False

    # If plot kind wasn't specified assume we're a basic plot unless x_data is
    # non numeric in which case we're a bar chart
    if kind is not None:
      self.plotKind = kind
    else:
      self.plotKind = PlotKind.BASIC
      if len(table.xData) > 0 and not isinstance(table.xData[0], numbers.Number):
        self.plotKind = PlotKind.BAR

    # set figure size, margins, title
    self.fig = plt.figure(figsize=(10,6))
    self.ax = self.fig.add_axes((.15,.15,.7,.7))
    self.ax.set_axisbelow(True)

    # Set title, xlabel, ylabel
    if title is None:
      self.set_title(table.title)
    else:
      self.set_title(title)

    if xlabel is not None:
      self.set_xlabel(xlabel)

    if ylabel is not None:
      self.set_ylabel(ylabel)

    # set borders
    self.ax.spines['bottom'].set_linewidth(4)
    self.ax.spines['bottom'].set_color('#808080')
    self.ax.spines['left'].set_linewidth(4)
    self.ax.spines['left'].set_color('#808080')
    self.ax.spines['top'].set_linewidth(0)
    self.ax.spines['right'].set_linewidth(0)

    # configuration specific to the kind of plot (basic or bar)
    if self.plotKind is PlotKind.BASIC:
      self.ax.grid(axis='y', linestyle='dashed', linewidth=3, zorder=1)
      self.ax.set_xlim((self._x_data[0], self._x_data[-1]))
      self.ax.xaxis.set_major_locator(ticker.FixedLocator(self._x_data))
      self.ax.set_xticklabels([str(x) for x in self._x_data])
      tickAxis = 'both'
    elif self.plotKind is PlotKind.BAR:
      plt.setp(self.ax.get_xticklabels(), rotation=45, horizontalalignment='right')
      tickAxis = 'y'
    else:
      assert("Unknown PlotKind")

    # ticks (remove them, then readd them according to the kind of chart)
    self.ax.tick_params(axis='both',which='major',direction='in',bottom=True,
                        length=0)
    self.ax.tick_params(axis=tickAxis,which='major',direction='in',bottom=True,
                        length=10, colors='#808080', labelsize=20,
                        width=4, pad=10, zorder=20)
    
    # add data
    i = 1
    for colname in table.yData:
      self.add_y_data(table.yData[colname], colname, linestyle=i)
      i += 1

    # add arrow if specified
    if better is not None:
      self._arrows.append(Arrow(position='right', direction=better, text='Better', color='green'))


  def add_y_data(self, column, label, linestyle=0, zorder=None):
    # Sanitize data
    data = column
    for i in range(0, len(data)):
      if data[i] is None:
          data[i] = np.nan
    self._y_datas.append(data)

    if self.plotKind == PlotKind.BASIC:
      zorder = 100 if zorder is None else zorder
      self.ax.plot(self._x_data,
                   data,
                   label=label,
                   zorder=zorder,
                   clip_on=False,
                   **line_style(linestyle))

    elif self.plotKind == PlotKind.BAR:
      barNum = len(self._bars)
      barWidth = 0.9 if barNum == 0 else 0.4 if barNum == 1 else 1.0/(len(self._bars)+2)

      # Adjust width of all pre-existing bars so we can fit the new bar in
      i = 0
      for b in self._bars:
        b['x'] = np.arange(len(self._x_data)) + (barWidth*i)
        b['width'] = barWidth
        i += 1

      x = np.arange(len(self._x_data)) + (barWidth*barNum)
      kwargs = line_style(linestyle)
      kwargs.pop('linestyle', None)
      kwargs.pop('marker', None)
      kwargs.pop('markersize', None)
      kwargs.pop('markeredgewidth', None)
      newBars = { \
        'x':       x,
        'height':  data,
        'width':   barWidth,
        'label':   label,
        'zorder':  1 if zorder is None else zorder,
        'clip_on': True}
      newBars.update(kwargs)
      self._bars.append(newBars)

      # Remove old bars and repaint. This is making self.plot O(N^2), which
      # isn't great but I think the added interactivity is worth it and in
      # practice the number of bars is pretty limitted.
      #for c in self.ax.containers:
      #  if isinstance(c, mpl.container.BarContainer):
      #    c.remove()
      #for b in self._bars:
      #  self.ax.bar(**b)

    else:
      assert("Unknown plot kind")

  def add_arrow(self, position, direction, color, text):
    self.arrows.append(Arrow(position, direction, color, text))

  def set_title(self, title):
    self.ax.set_title(title, pad=titlepadding, size=basefontsize)

  def set_xlabel(self, label):
    self.ax.set_xlabel(label, size=basefontsize)

  def set_xticks(self, *args, **kwargs):
    self.ax.set_xticks(*args, **kwargs)

  def set_xticklabels(self, *args, **kwargs):
    self.ax.set_xticklabels(*args, **kwargs)

  def set_ylabel(self, label):
    self.ax.set_ylabel(label, size=basefontsize)

  def set_ylim(self, *args, **kwargs):
    self.ax.set_ylim(*args, **kwargs)
    self._explicit_ylim = True

    # If there's a barchart adjust the bars to the new axis (so they don't
    # spill out of the bottom)
    newBottom = self.ax.get_ylim()[0]
    for b in self._bars:
      b['height'] = list(map(lambda x: x-newBottom, b['height']))
      b['bottom'] = newBottom

  def show(self):
    self._finalize()
    plt.show()

  def save(self, filename=None, **mpl_save_fig_args):
    self._finalize()
    if filename is None:
      filename = self._png_file_name
    plt.savefig(filename, **mpl_save_fig_args)

  def _finalize(self):
    # We defer drawing bars until finalization so we can adjust the width
    # as we ad them
    first = True
    for b in self._bars:
      self.ax.bar(**b)
      self.ax.set_xticks(b['x'], self._x_data, fontsize=12)
      # Put asterisks above any bars that are extending beyond the axis
      for i, v in enumerate(b['height']):
        y = v + self.ax.get_ylim()[0]
        if y > self.ax.get_ylim()[1]:
          self.ax.text(b['x'][i], self.ax.get_ylim()[1], "*", color='blue', fontweight='bold', horizontalalignment='center')
      first = False

    if not self._explicit_ylim:
      self.ax.set_ylim((0, max(np.nanmax(d) for d in self._y_datas)))

    xleft, xright = self.ax.get_xlim()
    ybottom, ytop = self.ax.get_ylim()
    xlen = xright-xleft
    ylen = ytop-ybottom

    for arrow in self._arrows:
      if arrow.position == "right":
        x, y = xright+xlen*0.1, ybottom+(ylen/2)
      else:
        print("I dunno how to put an arrow there yet. Ignoring")
      self.ax.text(x, y, **arrow.gen_kwargs_dict())

    # From https://jdhao.github.io/2017/06/03/change-aspect-ratio-in-mpl/
    ratio = 0.4
    self.ax.set_aspect(abs((xright-xleft)/(ybottom-ytop))*ratio)

    if self._include_legend:
      legend_args = { \
        'loc': 'best',
        'fontsize': basefontsize-2,
        'ncol': 1 }
      if self.mpl_legend_args:
        for arg in self.mpl_legend_args:
          legend_args[arg] = self.mpl_legend_args[arg]
      handles, labels = self.ax.get_legend_handles_labels()
      self.ax.legend(handles, labels, **legend_args)

# -----------------------------------------------------------------------------
def _maxLen(l, fmt="%s"):
  """ Utility function that returns the maximum string length among all items
      in iterable object 'l' """
  return reduce(lambda x,y: max(x,len(str(fmt % y))), l, 0)

class Table:
  """ Represents data read in from a .dat file.  Dat files contain tabular data
      where the first column corresponds to 'xData' and all other columns correspond
      to 'yData'. 

      Each column of data has a label, these labels may be reassigned using the
      'relabel' function.

      For most uses cases rather than constructing this object directly you'll
      probably want to use the free function 'loadDatFile' (which loads a specific
      .dat file) or loadDat (which returns a dictionary of Table objects for all
      .dat files under the logs/ directory)

      To extract a particular column of data use the `[]` operators. The result will
      be a new Table object containing the old 'xData' and just the 'yData' for
      the specified column.
  """
  def __init__(self, title=None, filename=None, config_vars=None, results=None, kvPairs=None):
    self.title = title
    self.filename = filename
    self.xData = config_vars
    self.yData = results
    self.kvPairs = kvPairs

    if title is None and filename is not None:
      self.title = os.path.basename(pathlib.Path(filename).stem)

  def __getitem__(self, key):
    newDat = Table(self.title, self.filename, self.xData, {key: self.yData[key]}, self.kvPairs)
    return newDat

  def __repr__(self):
    return "<Table object %s with columns %s>" % (self.filename, list(self.yData.keys()))

  def relabel(self, *labels):
    newYData = {}
    i = 0
    for key in self.yData:
      newYData[labels[i]] = self.yData[key]
      i += 1
    self.yData = newYData

  def md(self, dataFmt="%f"):
    def _compute_col_length(col_title):
      return max(len(col_title), _maxLen(self.yData[col_title], dataFmt))

    res = f"### {self.title}\n"

    max_row_label_len = _maxLen(self.xData)
    col_label_len = list(map(_compute_col_length, self.yData.keys()))

    res += f"| %{max_row_label_len}s |" % ""
    for col_num in range(len(self.yData)):
      res += f" %{col_label_len[col_num]}s |" % list(self.yData.keys())[col_num]

    res += f"\n| %s |" % ("-" * max_row_label_len)
    for col_num in range(len(self.yData)):
      res += f" %s |" % ("-" * col_label_len[col_num])

    for row_num in range(len(self.xData)):
      res += "\n"
      res += f"| %{max_row_label_len}s |" % self.xData[row_num]
      for col_num in range(len(self.yData)):
        cur_col = list(self.yData.keys())[col_num]
        res += f" %{col_label_len[col_num]}s |" % str(dataFmt % self.yData[cur_col][row_num])

    return res

  def plot(self, **plotArgs):
    return plot(self, **plotArgs)

  def select(self, *cols):
    """ Return a new table containing only the specified columns from this
        table. If you wish to do this for a single column you can also use the
        indexing (i.e. '[]') operator. For example: tbl['columnName'] """
    newYData = {}
    for col in cols:
      newYData[col] = self.yData[col]
    return Table(self.title, self.filename, self.xData, newYData, self.kvPairs)

  def with_col_as_tbl_title(self):
    """ If this table consists of a single data column, relabel that column
        to be whatever the title of the table is. This is useful in situations
        where you have several tables that have columns with the same name
        and you want to be able to distinguish them """
    assert(len(self.yData) == 1)
    old_name = list(self.yData.keys())[0]
    return Table(self.title, self.filename, self.xData, {self.title: self.yData[old_name]}, self.kvPairs)

  def with_cols_relabeled(self, relabelMap):
    """ given a dictionary of oldName->newName pairs for columns, produce a
        new table with the columns relabeled """ 
    yData = {}
    for key in self.yData:
      if key in relabelMap:
        yData[relabelMap[key]] = self.yData[key]
      else:
        yData[key] = self.yData[key]
    return Table(self.title, self.filename, self.xData, yData, self.kvPairs)

  def with_title(self, title):
    return Table(title, self.filename, self.xData, self.yData, self.kvPairs)

  def normalize_to(self, col_name):
    """ Remove 'col_name' from the table and express all other values in terms
    of the ratio of that value to that value in the specified column. """
    self._verify_valid_col(col_name)
    newData = copy.deepcopy(self.yData)
    del(newData[col_name])
    for col in newData:
      for i in range(len(newData[col])):
        newData[col][i] = newData[col][i] / self.yData[col_name][i]
    return Table(self.title + " normalized to " + col_name, self.filename, self.xData, newData, self.kvPairs)

  def _verify_valid_col(self, col_name):
    if col_name not in self.yData:
      raise Exception(
        "%s not a valid column. Valid columns are: %s" % (
        col_name, list(self.yData.keys())))

def loadDatFile(filename):
  """ Returns a dictionary of Table objects for all .dat files under the
      logs/ directory. """
  areYValuesNumbers = True
  with open(filename, 'r') as f:
    datanames = []
    config_vars = []
    results = {}
    kvPairs = {'filename': os.path.basename(filename)}
    for l in f:
      if len(l.strip()) == 0:
          continue
      if l.strip()[0] == '#':
        m = re.match(r"#(\w+):\s*(.*)$", l.strip())
        if m:
          decodedVal = codecs.escape_decode(bytes(
            m.groups()[1], "utf-8"))[0].decode("utf-8")
          kvPairs[m.groups()[0]] = decodedVal
        continue
      if len(datanames) == 0:
        # read the first line with datanames
        datanames = shlex.split(l)
        for dn in datanames:
          results[dn] = []
      else:
        # read one config var and the rest are data
        if not l.split()[0].isnumeric():
            areYValuesNumbers = False
        
        if areYValuesNumbers:
            config_vars.append(float(l.split()[0]))
        else:
            config_vars.append(l.split()[0])

        for dn, data in zip(datanames, l.split()[1:]):
          results[dn].append(float(data))

  return Table(None, filename, config_vars, results, kvPairs)


class TableCollection():
  def __init__(self, tables):
    self.tables = tables

  def __repr__(self):
    return "collection:" + repr(self.tables)

  def __getitem__(self, name):
    return self.tables[name]

  def __contains__(self, key):
    return key in self.tables

  def keys(self):
    return self.tables.keys()

  def md(self, *args):
    """ Print all tables in .md """
    return "\n\n".join(map(lambda tbl: tbl.md(*args), self.tables.values()))

  def plot(self, **kwArgs):
    """ Join all tables and plot them """
    return self.join().plot(**kwArgs)

  def join(self):
    """ Join all tables and return the resulting table """
    return join(*self.tables.values())

  def select(self, *col_labels):
    """ Return a new table collection, where each table is filtered to only
        include the specified columns """
    res = {}
    for entry in self.tables:
      res[entry] = self.tables[entry].select(*col_labels)
    return TableCollection(res)

  def with_col_as_tbl_title(self):
    res = {}
    for entry in self.tables:
      res[entry] = self.tables[entry].with_col_as_tbl_title()
    return TableCollection(res)

# -----------------------------------------------------------------------------
def _valOrDefault(kvPairs, key, default):
  return default if key not in kvPairs else f"{kvPairs[key]}"

def _embedMetadata(datFilename, imgFilename):
  from PIL import Image
  from PIL.PngImagePlugin import PngInfo

  datContent = open(datFilename).read()
  img = Image.open(imgFilename)

  metadata = PngInfo()
  metadata.add_text("datContent", datContent)

  img.save(imgFilename, pnginfo=metadata)

def _plotImpl(table, save, filename=None, **plotArgs):
  p = Plot(table, **plotArgs)
  if save:
    if filename is None:
      filename = p._png_file_name
    #_embedMetadata(chplDat.filename, "logs/" + plot.baseName + '.png')
    p.save(filename)
  return p

def computeUniqColName(existingCols, newCol):
  proposedNewCol = newCol
  num = 0
  while proposedNewCol in existingCols:
    num += 1
    proposedNewCol = newCol + "_" + str(num)
  return proposedNewCol

# -----------------------------------------------------------------------------

def join(*tables):
  res = copy.deepcopy(tables[0])
  res.title = "join of: (" + res.title

  for data in tables[1:]:
    assert(res.xData == data.xData)
    res.filename += ":" + data.filename
    res.title += ", " + data.title
    for col in data.yData:
      colName = computeUniqColName(res.yData.keys(), col)
      res.yData[colName] = data.yData[col]
    res.kvPairs.update(data.kvPairs)
  res.title += ")"

  return res


def plot(tables, save=True, filename=None, **plotArgs):
  """ This function is polymorphic (including its return type). If it is passed
  a single table it returns the plot for that single table. If it is passed a
  TableCollection then it returns a list of plots for those tables. """
  res = None
  # If we're passed a collection of tables, plot each individually
  if isinstance(tables, TableCollection):
    res = []
    for name in tables.tables:
      res.append(plot(tables.tables[name], save, **plotArgs))
  else:
    res = _plotImpl(tables, save, filename=filename, **plotArgs)
  return res


def load_tables(path="logs", filterFn=None):
  res = {}
  for filename in sorted(os.listdir(path)):
    fullPath = path + "/" + filename
    if filename.endswith(".dat"):
      if filterFn and not filterFn(fullPath):
        continue
      res[os.path.basename(fullPath)[0:-4]] = loadDatFile(fullPath)
  return TableCollection(res)
