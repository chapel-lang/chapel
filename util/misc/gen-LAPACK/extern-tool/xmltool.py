import fnmatch
import os
import sys
reload(sys)
sys.setdefaultencoding("utf-8")

import re
import string

import xml.etree.ElementTree as ET
from xml.etree.ElementTree import Element, SubElement, dump, ElementTree
import xml.dom.minidom

def stringtoxml( string ):
  return xml.dom.minidom.parseString( string )

def prettystringxml( element ):
  return xml.dom.minidom.parseString( ET.tostring(element) ).toprettyxml()

def prettyprintxml( element ):
  print prettystringxml( element );

def prettywritexml( element, filename ):
  file = open( filename, "w" );
  file.write( prettystringxml(element) )

def writexml( element, filename ):
  ET.ElementTree( element ).write( filename )
  
def loadxml( filename ):
  return ET.parse( filename ).getroot()
  
def SubElementUnique( parent, child ):
  if parent.find( child ) == None:
    return SubElement( parent, child )
  else:
    return parent.find( child )
    
def unset( self, name ):
  del self.attrib[name]
  
Element.unset = unset  