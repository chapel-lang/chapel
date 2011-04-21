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

#include <iostream>
#include <sstream>
#include <string>
#include <string.h>
#include "config.hpp"
#include "os.hpp"
#include <string.h>
#include <stdlib.h>
#include <algorithm>
#include <ext/functional>
#include <functional>
#include "debug.hpp"
#include "functors.hpp"
#include <map>

using namespace nanos;


Config::NanosHelp *Config::_nanosHelp = NULL;

void Config::NanosHelp::addHelpString ( const std::string &section, const HelpTriplet& ht )
{
   _helpSections[section].push_back( ht );
}

void Config::NanosHelp::addSectionDescription ( const std::string &section, const std::string &description )
{
   _sectionDescriptions[section] = description;
}

void Config::NanosHelp::buildSectionHelp( std::stringstream &helpString, const std::string &sectionName, HelpStringList &optionsHelpList, size_t helpLength)
{
      std::string formattedSectionName;
      formattedSectionName.assign( helpLength+8, ' ' );
      formattedSectionName.replace( 0, sectionName.size()+1, sectionName + ":" );
      helpString << formattedSectionName;

      SectionDescriptionsMap::iterator desc = _sectionDescriptions.find ( sectionName );
      if ( desc != _sectionDescriptions.end() ) {
         helpString << desc->second;
      }

      helpString << std::endl;

      std::stringstream argHelpString;
      std::stringstream envHelpString;

      for ( HelpStringList::iterator it = optionsHelpList.begin(); it != optionsHelpList.end(); it++ ) {
         HelpTriplet& ht = *it;
         std::string argHelp = ht.getArgHelp(helpLength);
         std::string envHelp = ht.getEnvHelp(helpLength);

         if ( argHelp != "" ) argHelpString << "      " << argHelp << std::endl;
         if ( envHelp != "" ) envHelpString << "      " << envHelp << std::endl;
      }

      if ( argHelpString.str() != "" ) {
         helpString << "   NX_ARGS options" << std::endl;
         helpString << argHelpString.str();
      }
      if ( envHelpString.str() != "" ) {
         helpString << "   Environment variables" << std::endl;
         helpString << envHelpString.str();
      }
}

const std::string Config::NanosHelp::getHelp()
{
   std::stringstream helpString;
   size_t helpLength=0;

   for ( SectionsMap::iterator it = _helpSections.begin(); it != _helpSections.end(); it++ ) {
      HelpStringList &optionsHelpList = it->second;
      for ( HelpStringList::iterator it = optionsHelpList.begin(); it != optionsHelpList.end(); it++ ) {
         size_t length = it->getHelpLength();
         if ( length > helpLength ) helpLength = length;
      }
   }

   buildSectionHelp( helpString, "Core", _helpSections["Core"], helpLength );

   for ( SectionsMap::iterator it = _helpSections.begin(); it != _helpSections.end(); it++ ) {
      helpString << std::endl;
      helpString << std::endl;

      std::string sectionName = it->first;
      if (sectionName != "Core" )
         buildSectionHelp( helpString, sectionName, it->second, helpLength );
   }

   return helpString.str();
}

const std::string Config::getNanosHelp()
{
   return _nanosHelp->getHelp();
}

void Config::setDefaults()
{
}

void Config::parseFiles ()
{
}

void Config::registerEnvOption ( const std::string &option, const std::string &envVar )
{
	if ( _configOptions[option]->getEnvVar() != "" ) {
      message0("WARNING: EnvOption '" << envVar << "' overwrites '" << _configOptions[option]->getEnvVar()
         << "' previously defined for the config option '" << option << "'"
         << std::endl << "Try using 'Config::registerAlias() instead.");
   }
   _configOptions[option]->setEnvVar( envVar );
}

void Config::registerArgOption ( const std::string &option, const std::string &arg )
{
	if ( _configOptions[option]->getArg() != "" ) {
      message0("WARNING: ArgOption '" << arg << "' overwrites '" << _configOptions[option]->getArg()
         << "' previously defined for the config option '" << option << "'"
         << std::endl << "Try using 'Config::registerAlias() instead.");
   }
   _configOptions[option]->setArg( arg );
   _argOptionsMap[arg] = _configOptions[option];
}

void Config::registerConfigOption ( const std::string &optionName, Option *option, const std::string &helpMessage )
{
   ConfigOption *configOption = NEW ConfigOption( optionName, *option, helpMessage, _currentSection );
   _configOptions[optionName] = configOption;
}

void Config::registerAlias ( const std::string &optionName, const std::string &alias, const std::string &helpMessage )
{
   ConfigOption *option = _configOptions[optionName];
   ConfigOption *aliasOption = NEW ConfigOption( alias, option->getOption(), helpMessage, _currentSection );
   _configOptions[alias] = aliasOption;
}

void Config::parseEnvironment ()
{
   for ( ConfigOptionMap::iterator it = _configOptions.begin(); it != _configOptions.end(); it++ ) {
      ConfigOption &confOpt = * ( it->second );
      if ( confOpt.getEnvVar() != "" ) {
         Option &opt = confOpt.getOption();
         opt.setName( confOpt.getEnvVar() );

         const char *env = OS::getEnvironmentVariable( confOpt.getEnvVar() );

         if ( !env ) continue;

         try {
            opt.parse( env );
         } catch ( InvalidOptionException &exception ) {
            std::cerr << "WARNING:" << exception.what() << std::endl;
         }
      }
   }
}

// C-strings are used in this function because std::string substring
// and character removal are O(N) compared to the possible O(1)
// with C style pointer manipulation.
// Even so, a std::string needs to be constructed to access the argument map
// so it's not clear if its worth it.
void Config::parseArguments ()
{
   const char *tmp = OS::getEnvironmentVariable( "NX_ARGS" );

   if ( tmp == NULL ) return;

   char env[ strlen(tmp) + 1 ];
   strcpy( &env[0], tmp );
   char *arg = strtok( &env[0], " " );

   while ( arg != NULL) {
      char * value=0;
      bool needValue=true;
      bool hasNegation=false;

      if ( arg[0] != '-' ) {
         arg = strtok( NULL, " " );
         continue;
      }

      arg++;

      // support --args
      if ( arg[0] == '-' ) arg++;

      // Since this skips the negation prefix no other arguments
      // are allowed to start with the negation prefix.
      if ( strncmp( arg, "no-", 3 ) == 0) {
         hasNegation=true;
         arg+=3;
      }

      if ( ( value = strchr( arg,'=' ) ) != NULL ) {
         // -arg=value form
         *value = 0; // sepparate arg from value
         value++; // point to the beginning of value
         needValue = false;
      } else {
         // -arg value form
      }

      ConfigOptionMap::iterator obj = _argOptionsMap.find( std::string( arg ) );

      if ( obj != _argOptionsMap.end() ) {
         Option &opt = ( *obj ).second->getOption();

         if ( needValue && opt.getType() != Option::FLAG ) {
            value = strtok( NULL, " " );
            if ( value == NULL)
               throw InvalidOptionException( opt,"" );
         }
         char yes[] = "yes";
         char no[] = "no";

         if ( opt.getType() == Option::FLAG ) {
            if ( hasNegation )
               value = no;
            else
               value = yes;
         }

         try {
            opt.setName( std::string( arg ) );
            opt.parse( value );
         } catch ( InvalidOptionException &exception ) {
            std::cerr << "WARNING:" << exception.what() << std::endl;
         }
      }
      arg = strtok( NULL, " " );
   }
}

void Config::init ()
{
   setDefaults();
   parseFiles();
   parseEnvironment();
   parseArguments();

   if ( _nanosHelp == NULL ) {
      _nanosHelp = NEW NanosHelp();
   }

   for ( ConfigOptionMap::iterator it = _configOptions.begin(); it != _configOptions.end(); it++ ) {
      _nanosHelp->addHelpString ( (*it).second->getSection(), (*it).second->getHelp() );
   }
}

void Config::setOptionsSection( const std::string &sectionName, const std::string &sectionDescription )
{
   _currentSection = sectionName;

   if ( _nanosHelp == NULL ) {
     _nanosHelp = NEW NanosHelp();
   }
   _nanosHelp->addSectionDescription ( sectionName, sectionDescription );
}

//TODO: move to utility header

void Config::clear ()
{
   std::for_each( _configOptions.begin(),_configOptions.end(),pair_deleter2<ConfigOption> );
   _configOptions.clear();
   _argOptionsMap.clear();
}

//TODO: generalize?

class map_copy
{
      Config::ConfigOptionMap& dest;

   public:

      map_copy( Config::ConfigOptionMap &d ) : dest( d ) {}

      void operator()( Config::ConfigOptionMap::value_type pair ) {  dest[pair.first] = pair.second->clone(); }
};

void Config::copy ( const Config &cfg )
{
   std::for_each( cfg._configOptions.begin(), cfg._configOptions.end(), map_copy( _configOptions ) );
   for ( ConfigOptionMap::iterator it = _configOptions.begin(); it != _configOptions.end(); it++ ) {
      _argOptionsMap[it->first] = it->second;
   }
}

Config::Config ( const Config &cfg )
{
   copy( cfg );
}

const Config & Config::operator= ( const Config &cfg )
{
   // handle self-assignment
   if ( this == &cfg ) return *this;

   clear();

   copy( cfg );

   return *this;
}

Config::~Config ()
{
   clear();
}

unsigned int Config::ConfigOption::getHelpLength()
{
   std::string farg="";
   std::string fenv="";
   
   if ( _argOption != "" ) {
      farg = "--" + _argOption + " ";
      std::string argval = _option.getArgHelp();
      if ( argval.compare( "no" ) == 0 ) {
         farg += "--no-"+_argOption;
      } else {
         farg += "[=]" + argval;
      }
   }

   if ( _envOption != "" ) {
      fenv = _envOption + " = " + _option.getEnvHelp();
   }

   if ( fenv.size() > farg.size() ) return fenv.size();
   else return farg.size();
}

Config::HelpTriplet Config::ConfigOption::getHelp()
{
   std::string fenv="";
   std::string farg="";

// argument
   if ( _argOption != "" ) {
      farg = "--" + _argOption + " ";
      std::string argval = _option.getArgHelp();
      if ( argval.compare( "no" ) == 0 ) {
         farg += "--no-"+_argOption;
      } else {
         farg += "[=]" + argval;
      }
   }

// Env vars
   if ( _envOption != "" ) {
      fenv = _envOption + " = " + _option.getEnvHelp();
   }

   return HelpTriplet( fenv, farg, _message );

}


size_t Config::HelpTriplet::getHelpLength()
{
   size_t envHelpSize = _envHelp.size();
   size_t argHelpSize = _argHelp.size();

   if ( envHelpSize > argHelpSize ) return envHelpSize;
   return argHelpSize;
}

std::string Config::HelpTriplet::getEnvHelp( size_t size )
{
   if ( _envHelp == "" ) return "";
   std::string help = "";
   std::string formattedEnv = "";
   formattedEnv.assign( size+2, ' ' );
   formattedEnv.replace( 0, _envHelp.size(), _envHelp );
   help += formattedEnv + _message;
   return help;
}

std::string Config::HelpTriplet::getArgHelp( size_t size )
{
   if ( _argHelp == "" ) return "";
   std::string help = "";
   std::string formattedArg = "";
   formattedArg.assign( size+2, ' ' );
   formattedArg.replace( 0, _argHelp.size(), _argHelp );
   help += formattedArg + _message;
   return help;
}

