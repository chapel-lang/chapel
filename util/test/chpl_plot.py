import os, re, codecs, shlex, copy

import numpy as np
import matplotlib as mpl
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
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

def width(x):
  return x+2;

chpl_plot_linestyles = {
0 : {'color': 'gray'}, # let matplotlib decide the rest
1 : {'color':'#00A000', 'linewidth':width(1), 'marker':'x', 'markersize':markersize, 'markeredgewidth':markeredgewidth, 'linestyle':'solid' }, # green
2 : {'color':'#00A6DD', 'linewidth':width(1), 'marker':'s', 'markersize':markersize, 'markeredgewidth':markeredgewidth, 'linestyle':'solid' }, # light blue
3 : {'color':'#3984C3', 'linewidth':width(1), 'marker':'o', 'markersize':markersize, 'markeredgewidth':markeredgewidth, 'linestyle':'solid' }, # med blue
4 : {'color':'#005696', 'linewidth':width(1), 'marker':'D', 'markersize':markersize, 'markeredgewidth':markeredgewidth, 'linestyle':'solid' }, # dark blue
40: {'color':'#001FAB', 'linewidth':width(1), 'marker':'^', 'markersize':markersize, 'markeredgewidth':markeredgewidth, 'linestyle':'solid'}, # very dark blue
5 : {'color':'#A00000', 'linewidth':width(1), 'marker':'x', 'markersize':markersize, 'markeredgewidth':markeredgewidth, 'linestyle':'solid' }, # dark red
11: {'color':'#00A000', 'linewidth':width(1), 'marker':'x', 'markersize':markersize, 'markeredgewidth':markeredgewidth, 'linestyle':'dashed'}, # green
12: {'color':'#00A6DD', 'linewidth':width(1), 'marker':'s', 'markersize':markersize, 'markeredgewidth':markeredgewidth, 'linestyle':'dashed'}, # light blue
13: {'color':'#3984C3', 'linewidth':width(1), 'marker':'o', 'markersize':markersize, 'markeredgewidth':markeredgewidth, 'linestyle':'dashed'}, # med blue
14: {'color':'#005696', 'linewidth':width(1), 'marker':'D', 'markersize':markersize, 'markeredgewidth':markeredgewidth, 'linestyle':'dashed'}, # dark blue
140:{'color':'#001FAB', 'linewidth':width(1), 'marker':'^', 'markersize':markersize, 'markeredgewidth':markeredgewidth, 'linestyle':'dashed'}, # very dark blue
15: {'color':'#A00000', 'linewidth':width(1), 'marker':'x', 'markersize':markersize, 'markeredgewidth':markeredgewidth, 'linestyle':'dashed'}, # dark red
21: {'color':'#00A000', 'linewidth':width(1), 'marker':'x', 'markersize':markersize, 'markeredgewidth':markeredgewidth, 'linestyle':'dotted'}, # green
22: {'color':'#00A6DD', 'linewidth':width(1), 'marker':'s', 'markersize':markersize, 'markeredgewidth':markeredgewidth, 'linestyle':'dotted'}, # light blue
23: {'color':'#3984C3', 'linewidth':width(1), 'marker':'o', 'markersize':markersize, 'markeredgewidth':markeredgewidth, 'linestyle':'dotted'}, # med blue
24: {'color':'#005696', 'linewidth':width(1), 'marker':'D', 'markersize':markersize, 'markeredgewidth':markeredgewidth, 'linestyle':'dotted'}, # dark blue
25: {'color':'#A00000', 'linewidth':width(1), 'marker':'x', 'markersize':markersize, 'markeredgewidth':markeredgewidth, 'linestyle':'dotted'}, # dark red
}

class Arrow:
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

PlotKind = Enum('PlotKind', ['PLOT', 'BAR'])

class Plot:
  def __init__(self, name, x_data):
    self.x_data = x_data
    self.name = name
    self.plotKind = PlotKind.PLOT

    if len(x_data) > 0 and not x_data[0].isnumeric():
      self.plotKind = PlotKind.BAR

    # set figure size and margins
    self.fig = plt.figure(figsize=(10,6))
    self.ax = self.fig.add_axes((.15,.15,.7,.7))

    # set borders
    self.ax.spines['bottom'].set_linewidth(width(2))
    self.ax.spines['bottom'].set_color('#808080')
    self.ax.spines['left'].set_linewidth(width(2))
    self.ax.spines['left'].set_color('#808080')
    self.ax.spines['top'].set_linewidth(0)
    self.ax.spines['right'].set_linewidth(0)

    # ticks
    self.ax.tick_params(axis='both',which='major',direction='in',bottom=True,
                        length=10, colors='#808080', labelsize=20,
                        width=width(2), pad=10, zorder=20)

    self.create_legend = False
    self.y_datas = []
    self.y_lines = []
    self.explicit_ylim = False
    self.arrows = []

    self.legend_font_size = basefontsize-2
    self.legend_position = None
    self.legend_ncol = 1
    self.legend_alphabetical = False

    if self.plotKind is PlotKind.PLOT:
      #grid
      self.ax.grid(axis='y', linestyle='dashed',
                   linewidth=width(1), zorder=1)

      # adjustments based on x_data
      self.ax.set_xlim((self.x_data[0], self.x_data[-1]))
      self.ax.set_xticklabels([str(x) for x in self.x_data])
      self.ax.xaxis.set_major_locator(ticker.FixedLocator(self.x_data))
    if self.plotKind is PlotKind.BAR:
      plt.setp(self.ax.get_xticklabels(), rotation=45, horizontalalignment='right')
    else:
      assert("Unknown PlotKind")

  def add_y_data(self, data, label='', linestyle=0, zorder=1):
    if label != '':
      self.create_legend = True

    for i in range(0, len(data)):
      if data[i] is None:
          data[i] = np.nan

    self.y_datas.append(data)

    if self.plotKind == PlotKind.PLOT:
      self.y_lines.append(
        self.ax.plot(self.x_data,
                     data,
                     label=label,
                     zorder=zorder,
                     clip_on=False,
                     **chpl_plot_linestyles[linestyle]))

    elif self.plotKind == PlotKind.BAR:
      barNum = len(self.y_datas)-1
      print("plot", barNum)
      x = np.arange(len(self.x_data)) + (0.25*barNum)
      #rects = ax.bar(x + (width*i), data[i], width, label=barLabels[i])

      kwargs = chpl_plot_linestyles[linestyle]
      del(kwargs['linestyle'])
      del(kwargs['marker'])
      del(kwargs['markersize'])
      del(kwargs['markeredgewidth'])
      self.y_lines.append(
        self.ax.bar(x,
                    data,
                    width=0.25,
                    label=label,
                    zorder=zorder,
                    clip_on=False,
                    **kwargs))
      self.ax.set_xticks(x, self.x_data)
    else:
      assert("Unknown plot kind")

  def add_dummy(self, label=''):
    self.ax.plot(self.x_data,
                 [None for x in self.x_data],
                 label=label,
                 marker='None',
                 linestyle='None')

  def add_arrow(self, position, direction,
                color, text):
    self.arrows.append(Arrow(position, direction, color, text))

  def set_title(self, title):
    self.ax.set_title(title, pad=10, size=basefontsize)

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
    self.explicit_ylim = True

  def set_linestyle(self, lineNum, styleNum):
    for prop in chpl_plot_linestyles[styleNum]:
      setFunc = getattr(self.y_lines[lineNum][0], 'set_' + prop)
      setFunc(chpl_plot_linestyles[styleNum][prop])
    
  def show(self):
    self.__finalize()
    plt.show()

  def save(self, filename=None, *extensions, **mpl_args):
    self.__finalize()

    if filename is None:
      filename = "logs/" + self.name

    if len(extensions) == 0:
      _extensions = ('png',)
    else:
      _extensions = extensions

    for ext in _extensions:
      plt.savefig('{}.{}'.format(filename, ext), **mpl_args)

  def __finalize(self):
    # assume that we have at least one y_data
    if not self.explicit_ylim:
      self.ax.set_ylim((0, max(np.nanmax(d) for d in self.y_datas)))

    xleft, xright = self.ax.get_xlim()
    ybottom, ytop = self.ax.get_ylim()
    xlen = xright-xleft
    ylen = ytop-ybottom

    for arrow in self.arrows:
      if arrow.position == "right":
        x, y = xright+xlen*0.1, ybottom+(ylen/2)
      else:
        print("I dunno how to put an arrow there yet. Ignoring")

      self.ax.text(x, y, **arrow.gen_kwargs_dict())

    # From https://jdhao.github.io/2017/06/03/change-aspect-ratio-in-mpl/
    ratio = 0.4
    self.ax.set_aspect(abs((xright-xleft)/(ybottom-ytop))*ratio)

    if self.create_legend:
      handles, labels = self.ax.get_legend_handles_labels()
      # sort both labels and handles by labels
      if self.legend_alphabetical:
          labels, handles = zip(*sorted(zip(labels, handles), key=lambda t: t[0]))

      # else:
      self.ax.legend(handles, labels, loc=self.legend_position,
                     handlelength=16*3.6/self.legend_font_size/2,
                     fontsize=self.legend_font_size,
                     ncol=self.legend_ncol)

# -----------------------------------------------------------------------------

class ChplDat:
  def __init__(self, filename=None, config_vars=None, results=None, kvPairs=None):
    self.filename = filename
    self.xData = config_vars
    self.yData = results
    self.kvPairs = kvPairs

  def __getitem__(self, key):
    newDat = ChplDat(self.filename, self.xData, {key: self.yData[key]}, self.kvPairs)
    return newDat

  def __str__(self):
    return "<ChplDat object %s with columns %s>" % (self.filename, list(self.yData.keys()))

  def relabel(self, labels):
    newYData = {}
    i = 0
    for key in self.yData:
      newYData[labels[i]] = self.yData[key]
      i += 1
    self.yData = newYData

def loadDatFile(filename):
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

  return ChplDat(filename, config_vars, results, kvPairs)

def load(filename):
  (xData, yData, kvPairs) = loadDat(filename)

  baseName = os.path.basename(filename)[:-4]
  title = _valOrDefault(kvPairs, 'title', baseName)
  xlabel = _valOrDefault(kvPairs, 'xlabel', '')
  ylabel = _valOrDefault(kvPairs, 'ylabel', '')

  p = Plot(name=title, x_data=xData)
  p.set_title(title)
  p.set_xlabel(xlabel)
  p.set_ylabel(ylabel)
  p.legend_font_size = 18
  p.baseName = baseName

  if 'better' in kvPairs:
    p.add_arrow(position='right', direction=kvPairs['better'], text='Better', color='green')

  n = 0
  for lines in yData:
    p.add_y_data(yData[lines], linestyle=11+n, label=lines)
    n += 1
 
  return p

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

def plot(name, chplDat):
  baseName = os.path.basename(chplDat.filename)[:-4]
  title = _valOrDefault(chplDat.kvPairs, 'title', name)
  xlabel = _valOrDefault(chplDat.kvPairs, 'xlabel', '')
  ylabel = _valOrDefault(chplDat.kvPairs, 'ylabel', '')

  p = Plot(name=title, x_data=chplDat.xData)
  p.set_title(title)
  p.set_xlabel(xlabel)
  p.set_ylabel(ylabel)
  p.legend_font_size = 18

  if 'better' in chplDat.kvPairs:
    p.add_arrow(position='right', direction=kvPairs['better'], text='Better', color='green')

  n = 0
  for lines in chplDat.yData:
    p.add_y_data(chplDat.yData[lines], linestyle=11+n, label=lines)
    n += 1

  return p
  
def paint(name, chplDat):
  p = plot(name, chplDat)
  p.save()
  #_embedMetadata(chplDat.filename, "logs/" + plot.baseName + '.png')

def loadDat(path, filterFn=None):
  res = {}
  for filename in os.listdir(path):
    fullPath = path + "/" + filename
    if filename.endswith(".dat"):
      if filterFn and not filterFn(fullPath):
        continue
      res[os.path.basename(fullPath)[0:-4]] = loadDatFile(fullPath)
  return res

def computeUniqColName(existingCols, newCol):
  proposedNewCol = newCol
  num = 0
  while proposedNewCol in existingCols:
    num += 1
    proposedNewCol = newCol + "_" + str(num)
  return proposedNewCol

def joinDat(dataList):
  res = copy.deepcopy(dataList[0])

  for data in dataList[1:]:
    assert(res.xData == data.xData)
    res.filename += ":" + data.filename
    for col in data.yData:
      colName = computeUniqColName(res.yData.keys(), col)
      res.yData[colName] = data.yData[col]
    res.kvPairs.update(data.kvPairs)

  return res

# Tips for using processFn (taking an argument p)
#
# Want to change the title?
#     p.set_title('something else')
#
# Want to change the label on a line?
#     p.y_lines[0][0].set_label('new label')
#
# How about the line style?
#     p.set_linestyle(lineNum, styleNum)

#def paintDatFiles(processFn=None, filterFn=None, inDir="logs"):
#      filename = f"{inDir}/{shortFilename}"
#        
#      plot = load(filename)
#      if processFn:
#        processFn(plot)
#      paint(filename, plot)
