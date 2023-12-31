#ifndef _TINY_JSON_PARSER_H_
#define _TINY_JSON_PARSER_H_

/**
 * Simple & tiny json parser
 * ------------------------------------------
 * Author      : So Byung Jun
 * Last Update : 2024.01.09
 * ------------------------------------------
 * Reference : https://github.com/amir-s/jute
 */

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <stack>
#include <type_traits>
#include <cassert>

namespace TinyJson {
using namespace std;

// Json data parser class
class JsonValue;
class Parser
{
public:
    /**
     * @brief  parse json from in-code string format ( const char* )
     * @param  str target string for parsing with std::string type
     * @param  validation_on If the order of parentheses is not correct, the assert error occurs.
     */
    static JsonValue parse( const char* str, const bool validation_on );
    static JsonValue parse( const string& str, const bool validation_on );
    
    static JsonValue parseFile( const string& fileName );

    static const bool isObject( const char* str ) noexcept;
    static const bool isObject( const string& str ) noexcept;

// define struct & enum class
private:
    enum class TokenType
    {
        UNKNOWN,
        STRING,
        INT,
        DOUBLE,
        CBRACE_OPEN,   // { , curl brace
        CBRACE_CLOSE,  // }
        BRACKET_OPEN,  // [ , square bracket
        BRACKET_CLOSE, // ]
        COMMNA,        // ,
        COLON,         // :
        BOOLEAN,
        NULL_TYPE      // NULL
    };

    struct Token
    {
        string    value;
        TokenType type;

        Token( string _value = "", TokenType _type = TokenType::UNKNOWN )
            : value(_value)
            , type(_type)
        {}
    };

// functions
private:
    inline static const bool isWhiteSpace( const char c );

    static const int nextWhiteSpace ( const string& src, const int startPos ) noexcept;
    static const int skipWhiteSpaces( const string& src, const int pos ) noexcept;

    static vector<Token> tokenize( const string& src, const bool validation_on );
    static JsonValue jsonParse( const vector<Token>& v, int& curPos, const bool validation_on = true );
}; // Class:Parser

// You can get JsonValue only by using JsonObject() and JsonArray() functions.
// ---------------------------------------------------------------------------
JsonValue JsonObject();
JsonValue JsonObject( const string& key, const JsonValue& jsValue );
JsonValue JsonNULL();
JsonValue JsonArray();
// ---------------------------------------------------------------------------

// Set using alias
using JsonObjects = vector<pair<string, JsonValue>>; // pair<key, value>
using JsonArrays  = vector<JsonValue>;

enum class JsonType {
    UNKNOWN,
    OBJECT,
    ARRAY,
    STRING,
    BOOLEAN,
    INT,
    DOUBLE,
    NULL_TYPE // Cannot be used 'NULL' because it is a reserved keyword
};

enum class ToStringType {
    Strip,
    Pretty,
};

// Json data unit class
class JsonValue
{
private:
    // Hiding constructors in private:
    // can only be created with 'JsonObject()' or 'JsonArray()'
    JsonValue() noexcept;

    // Constructor : Initialize value ( int, double, string )
    template <typename T>
    JsonValue( const T& initVal ) : jType( JsonType::UNKNOWN )
    {
        // SetType
        if      ( is_floating_point<T>::value ) this->setType( JsonType::DOUBLE );
        else if ( is_integral<T>::value )       this->setType( JsonType::INT );
        else                                    this->setType( JsonType::STRING );

        try {
            // Convert T type to string
            stringstream ss;
            ss << initVal;

            assert( ss.good() );
            string strVal = ss.str();

            // Set Value
            this->setString( strVal );
        }
        catch ( ... ) {
            // cout << "JsonValue Set Failed, Type: " << static_cast<int>(this->jType) << ", Value: " << initVal << endl;
            this->setType( JsonType::NULL_TYPE );
            this->setString( "null" );
        }
    }

    // Bool value function specialize because value
    // must be stored as true -> "true", false -> "false"
    JsonValue( const bool initVal ) noexcept : jType( JsonType::BOOLEAN )
    {
        ( initVal == true ) ? this->setString( "true" ) : this->setString( "false" );
    }

    static string deserialize( const string& src ) noexcept;
    static string serialize( const string& src ) noexcept;

    inline string makeSpace( const unsigned int space ) noexcept;
    const  string toStringPretty( const unsigned int space ) noexcept;
    const  string toStringStrip() noexcept;

public:
    // Constructor : Initialize with JsonType only, not set value
    JsonValue( const JsonType type = JsonType::OBJECT ) noexcept;

    // Initialize with json binray (const char*)
    JsonValue( const char* initVal ) noexcept
    {
        if( Parser::isObject(initVal) == true )
            *this = Parser::parse( string{initVal}, true );
        else
            *this = JsonValue(initVal);
    }

    // Initialize with json binray (string)
    JsonValue( const string& initVal ) noexcept
    {
        if( Parser::isObject(initVal) == true )
            *this = Parser::parse( initVal, true );
        else
            *this = JsonValue(initVal);
    }

    // Destructor
    ~JsonValue() {}

    JsonValue& setType( const JsonType type )
    {
        this->jType = type;
        return (*this);
    };

    // get current json object key list
    vector<string> keys();

    // get current json values if JsonArray type
    vector<JsonValue>& list();

// Region :: JsonValue struct insertion functions after parsing in Parser
// ---------------------------------------------------------------------------------------
    void addProperty( const string& key, const JsonValue& value ) noexcept; // +JsonObject
    void addElement ( const JsonValue& value ) noexcept;                    // +JsonArray
// ---------------------------------------------------------------------------------------

// Region :: Functions used to create json in code
// ---------------------------------------------------------------------------------------
    template <typename T>
    JsonValue& addObject( const string& key, const T& value )
    {
        this->setType( JsonType::OBJECT );
        this->addProperty( key, JsonValue( value ) );
        return (*this);
    }

    JsonValue& addObject( const string& key, const JsonValue& jsonValue )
    {
        this->setType( JsonType::OBJECT );
        this->addProperty( key, jsonValue );
        return (*this);
    }

    // variable length argument template
    template <typename T, typename... Types>
    JsonValue& addArray( const T& element, Types... elements )
    {
        // Handling T
        this->addElement( JsonValue(element) );

        // Handling Types : recursive
        this->addArray( elements... );

        return (*this);
    }

    template <typename T>
    JsonValue& addArray( const T& element )
    {
        // recursive end
        this->addElement( JsonValue( element ) );

        return (*this);
    }
// ---------------------------------------------------------------------------------------

    // Set the string s value to the corresponding JsonValue actual value
    // All json values are stored as strings inside JsonValue
    void setString( const string& s ) noexcept;

    // Inline functions to check what type a JsonValue is
    inline const bool isObject() { return this->jType == JsonType::OBJECT;    };
    inline const bool isArray()  { return this->jType == JsonType::ARRAY;     };
    inline const bool isString() { return this->jType == JsonType::STRING;    };
    inline const bool isDouble() { return this->jType == JsonType::DOUBLE;    };
    inline const bool isInt()    { return this->jType == JsonType::INT;       };
    inline const bool isNull()   { return this->jType == JsonType::NULL_TYPE; };
    inline const bool isBool()   { return this->jType == JsonType::BOOLEAN;   };
    inline const bool isValid()  { return this->jType != JsonType::UNKNOWN;   };

    /**Notation:
     * ---------
     * The C++ standard does not allow explicit specialization of class members at class scope.
     * The getAs<T>() template specials getAs<bool>() and getAs<string>() are defined outside the class.
     */

    // A function to call each type of value stored in JsonValue ( ex: doouble value = js.getAs<double>(); )
    template <typename T>
    T getAs()
    {
        T ret;
        stringstream ss( this->strValue );
        ss >>  ret;
        return ret;
    }

    template <typename T>
    const bool tryGetAs( T& ref )
    {
        try {
            if( std::is_same<T, string>::value && this->isString() ) {
                ref = getAs<T>();
            }
            else if( std::is_same<T, double>::value && this->isDouble() ) {
                ref = getAs<T>();
            }
            else if( std::is_same<T, int>::value && this->isInt() ) {
                ref = getAs<T>();
            }
            else if( std::is_same<T, bool>::value && this->isBool() ) {
                ref = getAs<T>();
            }
            else {
                return false;
            }
        }
        catch( ... ) {
            return false;
        }
        return true;
    }

    // Returns how many elements the JsonValue has if it is Objects or Arrays
    const size_t size() noexcept;

    // get operator
    JsonValue& operator[]( const int arrIdx );
    JsonValue& operator[]( const string& key );

    // set operator
    JsonValue& operator=( JsonValue other );
    JsonValue& operator=( const int value );
    JsonValue& operator=( const double value );
    JsonValue& operator=( const string& value );
    JsonValue& operator=( const char* value );
    JsonValue& operator=( const bool value );

    // Convert Json -> string
    string toString( ToStringType type = ToStringType::Strip ) noexcept;

    // Save File
    const bool saveFile( const char* fileName );

// values
private:
    string   strValue; // The actual value of that JsonValue
    JsonType jType;    // The value type of that JsonValue

    JsonObjects properties; // A vector that stores key:value pairs of JsonObjects.
    JsonArrays  arr;        // A vector that stores the elements of JsonArray

    unordered_map<string, size_t> mapIndex; // for properties' <key, idx>
}; // Class:JsonValue

// Region :: template functions used to create json in Code
// ---------------------------------------------------------------------------------------
template <typename T>
JsonValue JsonObject( const string& key, const T& value ) {
    return JsonObject().addObject( key, value );
}

template <typename T, typename... Types>
JsonValue JsonArray( const T& element, Types... elements ) {
    return JsonArray().addArray( element, elements... );
}

template <typename T>
JsonValue JsonArray( const T& element ) {
    return JsonArray().addArray( element );
}
// ---------------------------------------------------------------------------------------
}; // nsp:SimpleTool
#endif