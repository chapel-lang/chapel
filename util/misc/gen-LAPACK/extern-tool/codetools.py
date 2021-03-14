#!/usr/bin/python

class InvalidCharacterException ( Exception ):
  def __str__(self):
    return "Line cannot contain carriage return character"

class TypeMismatchException (Exception):
  def __init__(self, actual, expected):
    self.actual = actual
    self.expected = expected
    
  def __str__(self):
    return "Expected " + self.expected + " but got " + self.actual

class CodeProducer:
  tab = " " * 2
  def generate( self ):
    raise NotImplementedError
  
  def scope( self ):
    raise NotImplementedError

class SegmentProducer ( CodeProducer ):
  def __init__( self, string ):
    self.value = str( string )
    
  def generate( self ):
    return self.value;
  
  def scope(self):
    pass
  
  def __add__(self, other):
    if not isinstance( other, CodeProducer ):
      raise TypeMismatchException( other.__class__.__name__, CodeProducer )
    return SegmentProducer( self.value + other.generate() )

class LineProducer ( SegmentProducer ):
  def __init__( self, string ):
    
    self.value = string
  
  def generate( self ):
    return self.value + "\n"
    
class CommentProducer ( LineProducer ):
  def __init__( self, string ):
    self.value = string
    
  def generate( self ):
    return "/* " + self.value + " */\n"
    
class SequenceOfProducers ( CodeProducer ):
  def __init__( self ):
    self.producers = []

  def append(self, producer ):
    self.producers.append( producer )
    return self
  
  def prepend( self, producer ):
    temp = [ producer ]
    temp.extend( self.producers )
    self.producers = temp
    return self
  
  def insert( self, idx, producer ):
    self.producers.insert( idx, producer )
    return self

  def len( self ):
    return len( self.producers )
    
  def scope(self ):
    for producer in self.producers:
      producer.scope()
    
  def generate( self ):
    string = str()
    for producer in self.producers:
      string += producer.generate()
    return string;

class ListProducer ( SequenceOfProducers ):
  def __init__( self, sep=", ", prefix = "", suffix = "" ):
    self.producers = []
    self.sep = sep
    self.prefix = prefix
    self.suffix = suffix
    
  def generate( self ):
    string = str( self.prefix )
    count_producers = len( self.producers )
    for i in range( count_producers ):
      producer = self.producers[i]
      string += producer.generate() + (self.sep if i < count_producers - 1 else "")
    string += self.suffix
    return string;

class ScopeProducer ( SequenceOfProducers ):

  def __init__ (self):
    self.producers = [] # list of CodeProducer objects
    self.depth = 0
    
  def append(self, producer ):
    if isinstance( producer, ScopeProducer ):
      for i in range( 1 + self.depth): producer.scope()
    self.producers.append( producer )
    return self
  
  def prepend( self, producer ):
    if isinstance( producer, ScopeProducer ):
      for i in range( 1 + self.depth): producer.scope()
    temp = [ producer ]
    temp.extend( self.producers )
    self.producers = temp
    return self
  
  def insert( self, idx, producer ):
    if isinstance( producer, ScopeProducer ):
      for i in range( 1 + self.depth): producer.scope()
    self.producers.insert( idx, producer )
    return self
  
  def len( self ):
    return len( self.producers )
    
  def scope( self ):
    self.depth += 1
    for producer in self.producers:
      producer.scope()
      
  def generate( self ):
    string = str()
    string += CodeProducer.tab*(self.depth) +LineProducer("{").generate()
    for producer in self.producers:
      if isinstance( producer, ScopeProducer ):
        string += producer.generate()
      else:
        string += CodeProducer.tab*(self.depth+1) + producer.generate()
    string += CodeProducer.tab*(self.depth) + LineProducer("}").generate()
    return string;
    
  

