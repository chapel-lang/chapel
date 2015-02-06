import socket, time
import json
from collections import defaultdict

import yaml
try:
  # Use CLoader if available for the speed boost
  from yaml import CLoader as Loader
except ImportError:
  # Fall back to the python Loader otherwise
  from yaml import Loader

_hostname = socket.gethostname().split('.')[0]
_dat_date_format = '%m/%d/%y'
_csv_date_format = '%Y-%m-%d'
# doubled up braces are the escape in .format
_annotation_format = """{{\
 series: "{series}",\
 x: "{x}",\
 shortText: {shortText},\
 text: {text},\
 cssClass: "blackAnnotation",\
 tickHeight: 2, \
 attachAtBottom: true\
}}"""


class InputError(Exception):
  pass


def load(path):
  with open(path, 'r') as source:
    data = yaml.load(source, Loader=Loader)

  # replace all of the date strings with actual time structs
  try:
    for group in data:
      for date in data[group].keys():
        data[group][time.strptime(date, _dat_date_format)] = data[group][date]
        del data[group][date]
  except AttributeError:
    raise InputError("Invalid annotation format in {0}".format(path))

  return data


def get(data, graph, series, start, end):
  matches = defaultdict(list)
  _find_annotations('all', matches, data, start, end)
  _find_annotations(graph, matches, data, start, end)

  formatted = []
  for i, date in enumerate(sorted(matches.keys()), start=1):
    date_string = time.strftime(_csv_date_format, date)
    ann_text = json.dumps('{0}: {1}'.format(date_string,
      r'\n'.join(matches[date])))
    formatted.append(_annotation_format.format(
        series = series,
        x = date_string,
        shortText = i,
        text = ann_text,
    ))

  return formatted


def _find_annotations(graph, matches, data, start, end):
  if graph in data:
    for date, annotations in data[graph].iteritems():
      if start <= date and date <= end:
        for ann in annotations:
          if isinstance(ann, dict):
            if _hostname in ann['host']:
              matches[date].append(ann['text'])
          else:
            matches[date].append(ann)

