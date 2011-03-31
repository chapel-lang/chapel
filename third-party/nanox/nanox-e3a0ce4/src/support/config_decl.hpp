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

#ifndef _NANOS_CONFIG_DECL
#define _NANOS_CONFIG_DECL

#include <stdexcept>
#include <vector>
#include "compatibility.hpp"
#include <memory>
#include <sstream>
#include <string.h>

namespace nanos
{

   class Config
   {

      public:
         // Checking predicates
         // true predicate

         template<typename T> class CheckValue
         {

            public:
               virtual ~CheckValue() {}

               virtual bool operator() ( const T &value ) const;
         };

         // isPositive predicate

         template<typename T> class isPositive : public CheckValue<T>
         {

            public:
               virtual ~isPositive() {}

               virtual bool operator() ( const T &value ) const;
         };

         /** Configuration options */
         // Abstract Base Class
         class Option
         {

            public:
               typedef enum { FLAG, VALUE } OptType;

            private:
               std::string _name;
               OptType _type;

            public:
               // constructors
               Option( const std::string &n, const OptType t ) : _name( n ), _type( t ) {}

               Option( const char *n, const OptType t ) : _name( n ), _type( t ) {}

               Option( const OptType t ) : _name( "" ), _type( t ) {}

               // copy constructor
               Option( const Option &opt ) : _name( opt._name ),_type( opt._type ) {}

               // assignment operator
               const Option & operator= ( const Option &opt );
               // destructors
               virtual ~Option() {};

               const std::string &getName() const;

               void setName( const std::string &name );

               const OptType& getType() const;

               virtual void parse ( const char *value ) = 0;

               // clone idiom
               virtual Option * clone () = 0;

              /* \brief Formatted help for the argument option.
               */
               virtual const std::string getArgHelp () = 0;

              /* \brief Formatted help for the environment variable.
               */
               virtual const std::string getEnvHelp () = 0;
         };

         // Action Option, Base Class
         template<typename T, class helpFormat, typename checkT = CheckValue<T> >
         class ActionOption : public Option
         {

            private:
               checkT _check;
               helpFormat _helpFormat;

            public:
               // constructors
               ActionOption( const std::string &name ) :
                     Option( name,Option::VALUE ) {}

               ActionOption( const char *name ) :
                     Option( name,Option::VALUE ) {}

               ActionOption() :
                     Option( Option::VALUE ) {}

               // copy constructors
               ActionOption( const ActionOption& opt ) : Option( opt ), _check( opt._check ), _helpFormat( opt._helpFormat ) {}

               // assignment operator
               const ActionOption & operator= ( const ActionOption & opt );
               // destructor
               virtual ~ActionOption() {}

               virtual void parse ( const char *value );
               virtual void setValue ( const T &value ) = 0;

               bool checkValue ( const T &value ) const;

               virtual ActionOption * clone () = 0;

              /* \brief Formatted help for the argument option.
               */
               virtual const std::string getArgHelp ();

              /* \brief Formatted help for the environment variable.
               */
               virtual const std::string getEnvHelp ();
         };

         // VarOption: Option modifies a variable
         template<typename T, class helpFormat, typename checkT= CheckValue<T> >
         class VarOption : public ActionOption<T,helpFormat,checkT>
         {

            private:
               T &_var;
               // assignment operator
               const VarOption & operator= ( const VarOption &opt );

            public:
               //constructors
               VarOption( const std::string &name,T &ref ) :
                     ActionOption<T,helpFormat,checkT>( name ),_var( ref ) {}

               VarOption( const char *name, T &ref ) :
                     ActionOption<T,helpFormat,checkT>( name ),_var( ref ) {}

               VarOption( T &ref ) :
                     ActionOption<T,helpFormat,checkT>(), _var( ref ) {}

               // copy constructor
               VarOption( const VarOption &opt ) :
                     ActionOption<T,helpFormat,checkT>( opt ),_var( opt._var ) {}

               //destructor
               virtual ~VarOption() {}

               virtual void setValue ( const T &value );

               virtual VarOption * clone ();
         };

         // shortcuts for VarOptions and ActionOptions


         class HelpFormat
         {
            public:
               virtual ~HelpFormat() {}

               virtual std::string operator()();
         };

         class IntegerHelpFormat : public HelpFormat
         {
            public:
               virtual ~IntegerHelpFormat() {}

               virtual std::string operator()();
         };

         class BoolHelpFormat : public HelpFormat
         {
            public:
               virtual ~BoolHelpFormat() {}

               virtual std::string operator()();
         };

         class StringHelpFormat : public HelpFormat
         {
            public:
               virtual ~StringHelpFormat() {}

               virtual std::string operator()();
         };

         class PositiveHelpFormat : public HelpFormat
         {
            public:
               virtual ~PositiveHelpFormat() {}

               virtual std::string operator()();
         };

         typedef class VarOption<int,IntegerHelpFormat>                         IntegerVar;

         typedef class VarOption<bool,BoolHelpFormat>                           BoolVar;

         typedef class VarOption<std::string, StringHelpFormat>                 StringVar;

         typedef class VarOption<int,PositiveHelpFormat,isPositive<int> >       PositiveVar;
         typedef class VarOption<unsigned int,PositiveHelpFormat,isPositive<unsigned int> >
                                                                                UintVar;
         
         typedef class VarOption<size_t,PositiveHelpFormat,isPositive<size_t> > SizeVar;

         typedef class ActionOption<int,IntegerHelpFormat>                      IntegerAction;

         typedef class ActionOption<bool,BoolHelpFormat>                        BoolAction;

         typedef class ActionOption<std::string, StringHelpFormat>              StringAction;

         typedef class ActionOption<int,PositiveHelpFormat,isPositive<int> >    PositiveAction;

         template<typename T, class helpFormat, typename checkT= CheckValue<T> >
         class FuncOption : public ActionOption<T,helpFormat,checkT>
         {
            public:
               typedef void (*funptr) (const T &);

            private:
               funptr  _function;

            public:
               //constructors
               FuncOption( const std::string &name, funptr function ) :
                     ActionOption<T,helpFormat,checkT>( name ), _function( function ) {}

               FuncOption( const char *name, funptr function ) :
                     ActionOption<T,helpFormat,checkT>( name ), _function( function ) {}

               FuncOption( funptr function ) :
                     ActionOption<T,helpFormat,checkT>(), _function( function ) {}

               // copy constructor
               FuncOption( const FuncOption &opt ) :
                     ActionOption<T,helpFormat,checkT>( opt ), _function( opt._function ) {}

               //destructor
               virtual ~FuncOption() {}

               virtual void setValue ( const T& value );

               virtual FuncOption * clone ();
         };

         typedef class FuncOption<int,PositiveHelpFormat,isPositive<int> >      PositiveFunc;

         template<typename T>
         class MapAction : public Option
         {

            public:
               typedef std::pair<std::string,T> MapOption;
               typedef std::vector<MapOption> MapList;

            private:
               MapList _options;

               const std::string getHelp ();

            public:
               // constructors
               MapAction( const std::string &name, const MapList &opts ) :
                     Option( name,Option::VALUE ), _options( opts ) {}

               MapAction( const char *name, const MapList &opts ) :
                     Option( name,Option::VALUE ), _options( opts ) {}

               MapAction( const MapList &opts ) :
                     Option( Option::VALUE ), _options( opts ) {}

               MapAction( );

               // copy constructor
               MapAction( const MapAction &opt ) : Option( opt ),_options( opt._options ) {}

               // assignment operator
               const MapAction & operator= ( const MapAction &opt );
               // destructor
               virtual ~MapAction() {}

               MapAction & addOption ( std::string optionName, T value );

               virtual void parse ( const char *name );
               virtual void setValue ( const T &value ) = 0;
               virtual MapAction * clone () = 0;

              /* \brief Formatted help for the argument option.
               */
               virtual const std::string getArgHelp ();

              /* \brief Formatted help for the environment variable.
               */
               virtual const std::string getEnvHelp ();
         };

         template <typename T>
         class MapVar : public MapAction<T>
         {

            private:
               T &_var;
               // assignment operator
               const MapVar & operator= ( const MapVar & );

            public:
               typedef typename MapAction<T>::MapOption MapOption;
               typedef typename MapAction<T>::MapList MapList;

               //constructors
               MapVar( const std::string &name, T &ref, const MapList &opts ) :
                     MapAction<T>( name,opts ), _var( ref ) {}

               MapVar( const char *name, T &ref, const MapList &opts ) :
                     MapAction<T>( name,opts ), _var( ref ) {}

               MapVar( T &ref ) :
                     MapAction<T>(), _var( ref ) {}

               // copy constructor
               MapVar( const MapVar &opt ) : MapAction<T>( opt ), _var( opt._var ) {}

               // destructor
               virtual ~MapVar() {}

               virtual void setValue ( const T &value );

               virtual MapVar * clone ();
         };

         class ActionFlag : public Option
         {
            public:
               // constructors
               ActionFlag( const std::string &name ) :
                     Option( name,Option::FLAG ) {}

               ActionFlag( const char *name ) :
                     Option( name,Option::FLAG ) {}

               ActionFlag() :
                     Option( Option::FLAG ) {}

               // copy constructors
               ActionFlag( const ActionFlag& opt ) : Option( opt ) {}

               // assignment operator
               const ActionFlag & operator= ( const ActionFlag & opt );
               // destructor
               virtual ~ActionFlag() {}

               virtual void parse ( const char *value );
               virtual void setValue ( const bool &value ) = 0;

               virtual ActionFlag * clone () = 0;

              /* \brief Formatted help for the option.
               */
               virtual const std::string getArgHelp();
               virtual const std::string getEnvHelp();
         };

         class FlagOption : public ActionFlag
         {

            private:
               bool &_var;
               bool  _setTo;
               // assigment operator
               const FlagOption & operator= ( const FlagOption &opt );

            public:
               // constructors
               FlagOption ( const std::string &name, bool &ref, bool value=true ) :
                     ActionFlag( name ),_var( ref ),_setTo( value ) {}

               FlagOption ( const char *name, bool &ref, bool value=true ) :
                     ActionFlag( name ),_var( ref ),_setTo( value ) {}

               FlagOption ( bool &ref, bool value=true ) :
                     ActionFlag(),_var( ref ),_setTo( value ) {}

               // copy constructors
               FlagOption( const FlagOption &opt ) : ActionFlag( opt ), _var( opt._var ), _setTo( opt._setTo ) {}

               // destructor
               virtual ~FlagOption() {}

               virtual void setValue ( const bool &value );

               virtual FlagOption * clone ();
         };

         class HelpTriplet
         {
            private:
               std::string _envHelp;
               std::string _argHelp;
               std::string _message;

            public:
               HelpTriplet ( const std::string &envHelp, const std::string argHelp, const std::string message ) :
                  _envHelp( envHelp ), _argHelp( argHelp ), _message( message ) {}

               HelpTriplet ( const HelpTriplet& ht ) :
                  _envHelp( ht._envHelp ), _argHelp( ht._argHelp ), _message( ht._message ) {}

               const HelpTriplet& operator=( const HelpTriplet& ht );

               ~HelpTriplet() {}

               size_t getHelpLength();
               std::string getEnvHelp( size_t size );
               std::string getArgHelp( size_t size );
         };

        /* \brief High-Level Instances of configuration options
         */
         class ConfigOption
         {
            private:
               /**< Name that identifies the option. */
               std::string _optionName;

               /**< Environment variable option to set this option. */
               std::string _envOption;
               /**< Argument option for NANOS_ARGS to set this option. */
               std::string _argOption;

               /**< Option set by the ConfigOption*/
               Option &_option;

               /**< help message for this option. */
               std::string _message;

               /**< Section in which the ConfigOption is*/
               std::string _section;

            public:
              /* \brief Constructor
               * \param name: Name that identifies the option.
               * \param option: Option set by this ConfigOption.
               * \param helpMessage: Help message to be printed for this option.
               * \param section: section in which the ConfigOption is listed.
               */
               ConfigOption( const std::string &name, Option &option, const std::string &helpMessage, const std::string &section ) :
                  _optionName( name ), _envOption( "" ), _argOption( "" ), _option( option ), _message( helpMessage ), _section( section ) {}

              /* \brief Constructor
               * \param name: Name that identifies the option.
               * \param arg: Argument name to set the option.
               * \paran env: Environment variable name to set the option.
               * \param option: Option set by this ConfigOption.
               * \param helpMessage: Help message to be printed for this option.
               * \param section: section in which the ConfigOption is listed.
               */
               ConfigOption( const std::string &name, const std::string& env, const std::string &arg, Option &option, const std::string &helpMessage, const std::string &section ) :
                  _optionName( name ), _envOption( arg ), _argOption( env ), _option( option ), _message( helpMessage ), _section( section ) {}

               ConfigOption( const ConfigOption &co ) :
                  _optionName( co._optionName ), _envOption( co._envOption ), _argOption( co._argOption ),
                  _option( co._option ), _message( co._message ), _section( co._section ) {}

               const ConfigOption & operator= ( const ConfigOption &co );

               ~ConfigOption () { /* delete &_option; */ /*FIXME: mercurium tests complain this delete */ }

              /* \brief Returns the formatted help message for the ConfigOption's argument.
               */
               HelpTriplet getHelp();

              /* \breif Returns the option's help lenght
               */
               unsigned int getHelpLength();

              /* \brief Environment Option's getter method.
               */
               const std::string& getEnvVar();

              /* \brief Argument Option's getter method.
               */
               const std::string& getArg();

              /* \brief Environment Option's setter method.
               */
               void setEnvVar( const std::string envOption );

              /* \brief Argument Option's setter method.
               */
               void setArg( const std::string argOption );

              /* \brief Returns the option set by this ConfigOption
               */
               Option& getOption();

              /* \brief Returns the name of the section to which this ConfigOption belongs
               */
               const std::string getSection ();

              /* \brief cloner function
               */
               ConfigOption * clone ();
         };

        /* \brief Nanos help class, constructs the help text for the library
         */
         class NanosHelp
         {
             private:
//                /**< Help formatted help string for the argument and the environment variable of an option */
//                typedef std::pair<std::string, std::string> argAndEnvHelps;
                /**< List of helps in a section */
//                typedef std::vector<argAndEnvHelps> HelpStringList;
                typedef std::vector<HelpTriplet> HelpStringList;
                /**< Sections by name */
                typedef TR1::unordered_map<std::string, HelpStringList> SectionsMap;
                /**< Section descriptions by name */
                typedef TR1::unordered_map<std::string, std::string> SectionDescriptionsMap;

                /**< Help strings for all sections */
                SectionsMap _helpSections;
                /**< Section descriptions */
                SectionDescriptionsMap _sectionDescriptions;

                void buildSectionHelp( std::stringstream &helpString, const std::string &sectionName, HelpStringList &optionsHelpList, size_t helpLength);

             public:

               /* \brief Add the help string for an option
                * \param section section to which the option belogns
                * \param agrHelpString Help string for the argument option
                * \param envHelpString Help string for the environment variable
                */
                void addHelpString ( const std::string &section, const HelpTriplet &ht );

               /* \brief Set the description for a section
                * \param section Section name
                * \param description Section description
                */
                void addSectionDescription ( const std::string &section, const std::string &description );

               /* \brief Build and return the help text
                */
                const std::string getHelp();
         };

         /**< Stores ConfigOptions by name */
         typedef TR1::unordered_map<std::string, ConfigOption *> ConfigOptionMap;

      private:

         /**< List of all config options in this config */
         ConfigOptionMap _configOptions;
         /**< Shortcut to find argument options */
         ConfigOptionMap _argOptionsMap;

         /**< Current section where the options are being listed */
         std::string _currentSection;

         /**< Gathers help messages of all Configs used in the runtime */
         static NanosHelp *_nanosHelp;

      protected:

         virtual void setDefaults();
         void parseFiles();
         void parseArguments();
         void parseEnvironment();
         void clear();
         void copy( const Config &origin );

      public:
         // constructors
         Config() : _currentSection("Other options") {}

         // copy constructors
         Config( const Config &cfg );
         // assignment operator
         const Config & operator= ( const Config &cfg );
         // destructor
         virtual ~Config ();

        /* \brief initializes the config object
         */
         void init();

        /* \brief Sets the current section in which new ConfigOptions will be listed
         * \param sectionName name of the section to be set as current
         * \param sectionDescription If any, sets the description for the section
         */
         void setOptionsSection( const std::string &sectionName, const std::string &sectionDescription );

        /* \brief Register a Configuration Option
         * \param optionName Name to give to the option
         * \param option Option set by this ConfigOption
         * \param helpMessage Help information about the ConfigOption
         */
         void registerConfigOption ( const std::string &optionName, Option *option, const std::string &helpMessage );

        /* \brief Register alias for a ConfigOption
         * \param optionName Name of the ConfigOption to alias
         * \param aliasName Name for the new ConfigOption
         * \param helpMessage Help information about the new ConfigOption
         */
         void registerAlias ( const std::string &optionName, const std::string &aliasName, const std::string &helpMessage );

        /* \brief Register env var option
         * \param option Name of the ConfigOption
         * \param envVar Name for the environment variable to set the option
         */
         void registerEnvOption ( const std::string &option, const std::string &envVar );

        /* \brief Register a NX_ARGS argument option
         * \param option Name of the ConfigOption
         * \param arg Name for the argument
         */
         void registerArgOption ( const std::string &option, const std::string &arg );

        /* Returns the formatted help text for the nanox runtime library
         */
         static const std::string getNanosHelp();
   };

   /** exceptions */

  /* \brief Exception for invalid options
   */
   class InvalidOptionException : public  std::runtime_error
   {

      public:
        /* \brief Constructor
         * \param option Option detected invalid
         * \param value Invalid value
         */
         InvalidOptionException( const Config::Option &option,
                                 const std::string &value );

   };

};

#endif
