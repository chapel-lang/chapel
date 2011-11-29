/*************************************************************************************/
/*      Copyright 2009 Barcelona Supercomputing Center                               */
/*                                                                                   */
/*      This file is part of the NANOS++ library.                                    */
/*                                                                                   */
/*      NANOS++ is free software: you can redistribute it and/or modify              */
/*      it under the terms of the GNU Lesser General Public License as published by  */
/*      the Free Software Foundation, either version 3 of the License, or            */
/*      (at your option) any later version.                                          */
/*                                                                                   */
/*      NANOS++ is distributed in the hope that it will be useful,                   */
/*      but WITHOUT ANY WARRANTY; without even the implied warranty of               */
/*      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                */
/*      GNU Lesser General Public License for more details.                          */
/*                                                                                   */
/*      You should have received a copy of the GNU Lesser General Public License     */
/*      along with NANOS++.  If not, see <http://www.gnu.org/licenses/>.             */
/*************************************************************************************/
#ifndef __NANOS_INSTRUMENTOR_H
#define __NANOS_INSTRUMENTOR_H

#include "instrumentation_decl.hpp"
#include "system.hpp"
#include "allocator_decl.hpp"

using namespace nanos;

#ifdef NANOS_INSTRUMENTATION_ENABLED

inline nanos_event_value_t InstrumentationValueDescriptor::getId ( void )
{
   return _id;
}

inline const std::string InstrumentationValueDescriptor::getDescription ( void )
{
   return _description;
}


/** INSTRUMENTOR KEY DESCRIPTOR **/

inline nanos_event_key_t InstrumentationKeyDescriptor::getId ( void )
{
   return _id;
}

inline const std::string InstrumentationKeyDescriptor::getDescription ( void )
{
   return _description;
}

inline nanos_event_value_t InstrumentationKeyDescriptor::registerValue ( const std::string &value, const std::string &description,
                                                                      bool abort_when_registered )
{
   return registerValue( value.c_str(), description.c_str(), abort_when_registered );
}

inline nanos_event_value_t InstrumentationKeyDescriptor::registerValue ( const char *value, const char *description, bool abort_when_registered )
{
   InstrumentationValueDescriptor *valueDescriptor = NULL;

   ValueMapIterator it = _valueMap.find( value );

   if ( it == _valueMap.end() ) {
      {
         LockBlock lock( _lock );
         it = _valueMap.find( value );
         if ( it == _valueMap.end() ) {
            valueDescriptor = NEW InstrumentationValueDescriptor ( (nanos_event_value_t) _totalValues++, description );
            _valueMap.insert( std::make_pair( value, valueDescriptor ) );
         }
         else {
            if ( abort_when_registered ) fatal0("Event Value was already registered (lock taken)\n");
            valueDescriptor = it->second;
         }
      }
   }
   else {
      if ( abort_when_registered ) fatal0("Event Value was already registered (lock not taken)\n");
      valueDescriptor = it->second;
   }

   return valueDescriptor->getId();
}


inline void InstrumentationKeyDescriptor::registerValue ( const std::string &value, nanos_event_value_t val,
                                                          const std::string &description, bool abort_when_registered )
{
   registerValue( value.c_str(), val, description.c_str(), abort_when_registered );
}

inline void InstrumentationKeyDescriptor::registerValue ( const char *value, nanos_event_value_t val,
                                                          const char *description, bool abort_when_registered )
{
   InstrumentationValueDescriptor *valueDescriptor = NULL;

   ValueMapIterator it = _valueMap.find( value );

   if ( it == _valueMap.end() ) {
      {
         LockBlock lock( _lock );
         // Checking if val was already used
         if ( abort_when_registered ) {
            for ( ValueMapIterator it2 = _valueMap.begin(); it2 != _valueMap.end(); it2++ ) {
               InstrumentationValueDescriptor *vD = it2->second;
               if ( vD->getId() == val ) fatal("Event Value 'id' was already registered");
            }
         }
         it = _valueMap.find( value );
         if ( it == _valueMap.end() ) {
            _totalValues++; // keeping total values counter, although it is not used as 'val'
            valueDescriptor = new InstrumentationValueDescriptor ( val, description );
            _valueMap.insert( std::make_pair( value, valueDescriptor ) );
         }
         else {
            if ( abort_when_registered ) fatal0("Event Value was already registered (lock taken)\n");
         }
      }
   }
   else {
      if ( abort_when_registered ) fatal0("Event Value was already registered (lock not taken)\n");
   }
}

inline nanos_event_value_t InstrumentationKeyDescriptor::getValue ( const std::string &value )
{
   return getValue( value.c_str() );
}

inline nanos_event_value_t InstrumentationKeyDescriptor::getValue ( const char *value )
{
   ValueMapIterator it = _valueMap.find( value );

   if ( it == _valueMap.end() ) return (nanos_event_value_t) 0; 
   else return it->second->getId();
}

inline InstrumentationKeyDescriptor::ConstValueMapIterator InstrumentationKeyDescriptor::beginValueMap ( void )
{
   return _valueMap.begin();
}

inline InstrumentationKeyDescriptor::ConstValueMapIterator InstrumentationKeyDescriptor::endValueMap ( void )
{
   return _valueMap.end();
}

inline const std::string InstrumentationKeyDescriptor::getValueDescription ( nanos_event_value_t val )
{
   bool found = false;
   ValueMapIterator it;
   
   for ( it = _valueMap.begin(); ( (it != _valueMap.end()) && !found ); /* no-increment */ ) {
      if ( it->second->getId() == val ) found = true;
      else it++;
   }
   
   if (found == true) return it->second->getDescription();
   else return "";
}
/** INSTRUMENTOR DICTIONARY **/

inline nanos_event_key_t InstrumentationDictionary::registerEventKey ( const std::string &key, const std::string &description, bool abort_when_registered  )
{
   return registerEventKey( key.c_str(), description.c_str(), abort_when_registered );
}

inline nanos_event_key_t InstrumentationDictionary::registerEventKey ( const char *key, const char *description, bool abort_when_registered  )
{
   InstrumentationKeyDescriptor *keyDescriptor = NULL;

   KeyMapIterator it = _keyMap.find( key );

   if ( it == _keyMap.end() ) {
      {
         LockBlock lock( _lock );
         it = _keyMap.find( key );
         if ( it == _keyMap.end() ) {
            keyDescriptor = NEW InstrumentationKeyDescriptor ( (nanos_event_key_t) _totalKeys++, description );
            _keyMap.insert( std::make_pair( key, keyDescriptor ) );
         }
         else {
            if ( abort_when_registered ) fatal0("Event Key was already registered (lock taken)\n");
            keyDescriptor = it->second;
         }
      }
   }
   else {
      if ( abort_when_registered ) fatal0("Event Key was already registered (lock not taken)\n");
      keyDescriptor = it->second;
   }

   return keyDescriptor->getId();
}

inline nanos_event_key_t InstrumentationDictionary::getEventKey ( const std::string &key )
{
   return getEventKey( key.c_str() );
}

inline nanos_event_key_t InstrumentationDictionary::getEventKey ( const char *key )
{
   KeyMapIterator it = _keyMap.find( key );

   if ( it == _keyMap.end() ) return (nanos_event_key_t) 0;
   else return it->second->getId();
}

inline nanos_event_value_t InstrumentationDictionary::registerEventValue ( const std::string &key, const std::string &value, const std::string &description, bool abort_when_registered )
{
   return registerEventValue ( key.c_str(), value.c_str(), description.c_str(), abort_when_registered );
}

inline nanos_event_value_t InstrumentationDictionary::registerEventValue ( const char *key, const char *value, const char *description, bool abort_when_registered )
{
   InstrumentationKeyDescriptor *keyDescriptor = NULL;

   KeyMapIterator it = _keyMap.find( key );

   if ( it == _keyMap.end() ) {
      {
         LockBlock lock( _lock );
         it = _keyMap.find( key );
         if ( it == _keyMap.end() ) {
            keyDescriptor = NEW InstrumentationKeyDescriptor ( (nanos_event_key_t) _totalKeys++, "" );
            _keyMap.insert( std::make_pair( key, keyDescriptor ) );
         }
         else {
            keyDescriptor = it->second;
         }
      }
   }
   else keyDescriptor = it->second;

   return keyDescriptor->registerValue( value, description, abort_when_registered );
}

inline void InstrumentationDictionary::registerEventValue ( const std::string &key, const std::string &value,
                                                            nanos_event_value_t val,
                                                            const std::string &description, bool abort_when_registered )
{
   return registerEventValue ( key.c_str(), value.c_str(), val, description.c_str(), abort_when_registered );
}

inline void InstrumentationDictionary::registerEventValue ( const char *key, const char *value,
                                                            nanos_event_value_t val,
                                                            const char *description, bool abort_when_registered )
{
   InstrumentationKeyDescriptor *keyDescriptor = NULL;

   KeyMapIterator it = _keyMap.find( key );

   if ( it == _keyMap.end() ) {
      {
         LockBlock lock( _lock );
         it = _keyMap.find( key );
         if ( it == _keyMap.end() ) {
            keyDescriptor = new InstrumentationKeyDescriptor ( (nanos_event_key_t) _totalKeys++, "" );
            _keyMap.insert( std::make_pair( key, keyDescriptor ) );
         }
         else {
            keyDescriptor = it->second;
         }
      }
   }
   else keyDescriptor = it->second;

   return keyDescriptor->registerValue( value, val, description, abort_when_registered );
}

inline nanos_event_value_t InstrumentationDictionary::getEventValue ( const std::string &key, const std::string &value )
{
   return getEventValue ( key.c_str(), value.c_str() );
}

inline nanos_event_value_t InstrumentationDictionary::getEventValue ( const char *key, const char *value )
{
   KeyMapIterator it = _keyMap.find( key );

   if ( it == _keyMap.end() ) return (nanos_event_value_t) 0;
   else return it->second->getValue(value);
}

inline InstrumentationDictionary::ConstKeyMapIterator InstrumentationDictionary::beginKeyMap ( void )
{
   return _keyMap.begin();
}

inline InstrumentationDictionary::ConstKeyMapIterator InstrumentationDictionary::endKeyMap ( void )
{
   return _keyMap.end();
}


inline const std::string InstrumentationDictionary::getKeyDescription ( nanos_event_key_t key )
{
   bool found = false;
   KeyMapIterator it;
   
   for ( it = _keyMap.begin(); ( (it != _keyMap.end()) && !found ); /* no-increment */ ) {
      if ( it->second->getId() == key ) found = true;
      else it++;
   }
   
   if (found == true) return it->second->getDescription();
   else return "";
}

inline const std::string InstrumentationDictionary::getValueDescription ( nanos_event_key_t key, nanos_event_value_t val )
{
   bool found = false;
   KeyMapIterator it;
   
   for ( it = _keyMap.begin(); ( (it != _keyMap.end()) && !found ); /* no-increment */ ) {
      if ( it->second->getId() == key ) found = true;
      else it++;
   }
   
   if (found == true) return it->second->getValueDescription( val );
   else return "";
}

/** INSTRUMENTOR **/

inline InstrumentationDictionary * Instrumentation::getInstrumentationDictionary ( void ) { return &_instrumentationDictionary; }

inline nanos_event_type_t Instrumentation::Event::getType () const { return _type; }

inline nanos_event_state_value_t Instrumentation::Event::getState () { return _state; }

inline unsigned int Instrumentation::Event::getNumKVs () const { return _nkvs; }

inline Instrumentation::Event::ConstKVList Instrumentation::Event::getKVs () const { return _kvList; }

inline unsigned int Instrumentation::Event::getDomain ( void ) const { return _ptpDomain; }

inline unsigned int Instrumentation::Event::getId( void ) const { return _ptpId; }

inline void Instrumentation::Event::reverseType ( )
{
   switch ( _type )
   {
      case NANOS_PTP_START: _type = NANOS_PTP_END; break;
      case NANOS_PTP_END: _type = NANOS_PTP_START; break;
      case NANOS_BURST_START: _type = NANOS_BURST_END; break;
      case NANOS_BURST_END: _type = NANOS_BURST_START; break;
      default: break;
   }
}

#endif

#endif
