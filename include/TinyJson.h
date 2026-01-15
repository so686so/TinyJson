#ifndef _TINY_JSON_H_
#define _TINY_JSON_H_

/**
 * TinyJson: A Simple & Powerful C++ JSON Parser
 * -----------------------------------------------------------------------------
 * Based on work by : So Byung Jun
 * Refactored by    : Gemini
 * Last Update      : 2026.01.15
 * License          : MIT
 * Reference        : https://github.com/amir-s/jute
 * -----------------------------------------------------------------------------
 */

#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <exception>
#include <type_traits>
#include <utility> // for std::pair, std::move

namespace TinyJson {

// =============================================================================
// [Exception Class]
// =============================================================================

/**
 * @brief Exception thrown when JSON parsing fails or invalid access occurs.
 */
class TinyJsonException : public std::exception
{
public:
    explicit TinyJsonException( const std::string& msg ) : message( msg ) {}
    virtual const char* what() const noexcept override { return message.c_str(); }

private:
    std::string message;
};

// =============================================================================
// [Forward Declarations & Types]
// =============================================================================

class Json;

/**
 * @brief Represents the data type of a JSON element.
 */
enum class JsonType {
    UNKNOWN,
    OBJECT,
    ARRAY,
    STRING,
    BOOLEAN,
    INT,
    DOUBLE,
    NULL_TYPE
};

/**
 * @brief Formatting options for JSON serialization.
 */
enum class ToStringType {
    Strip,  ///< Minimal string   (no whitespace)
    Pretty  ///< Formatted string (indented)
};

using JsonObjects = std::vector<std::pair<std::string, Json>>;
using JsonArrays  = std::vector<Json>;

// =============================================================================
// [Parser Class]
// =============================================================================

/**
 * @brief Static helper class for parsing JSON strings and files.
 */
class Parser
{
public:
    /**
     * @brief Parses a JSON string (C-style) into a Json object.
     * @param str Null-terminated JSON string.
     * @return Parsed Json object.
     * @throws TinyJsonException if parsing fails.
     */
    static Json parse( const char* str );

    /**
     * @brief Parses a JSON string (std::string) into a Json object.
     * @param str JSON string.
     * @return Parsed Json object.
     * @throws TinyJsonException if parsing fails.
     */
    static Json parse( const std::string& str );

    /**
     * @brief Reads a JSON file and parses it.
     * @param fileName Path to the JSON file.
     * @return Parsed Json object.
     * @throws TinyJsonException if file open fails or parsing errors occur.
     */
    static Json parseFile( const std::string& fileName );

    // -------------------------------------------------------------------------
    // Utility Methods
    // -------------------------------------------------------------------------
    static bool isObject( const char* str ) noexcept;
    static bool isObject( const std::string& str ) noexcept;

private:
    enum class TokenType {
        UNKNOWN,
        STRING, INT, DOUBLE, BOOLEAN, // real data types
        CBRACE_OPEN, CBRACE_CLOSE,    // { }
        BRACKET_OPEN, BRACKET_CLOSE,  // [ ]
        COMMA, COLON,                 // , :
        NULL_TYPE
    };

    struct Token {
        // constructor
        Token( std::string _value = "", TokenType _type = TokenType::UNKNOWN )
            : value( _value ), type( _type )
        {}

        // members
        std::string value;
        TokenType   type;
    };

    inline static bool isWhiteSpace( const char c );

    static int nextWhiteSpace ( const std::string& src, const int startPos ) noexcept;
    static int skipWhiteSpaces( const std::string& src, const int pos      ) noexcept;

    static std::vector<Token> tokenize( const std::string& src );

    static Json jsonParse( const std::vector<Token>& v, int& curPos );
};

// =============================================================================
// [Json Class]
// =============================================================================

/**
 * @brief The core class representing a JSON element (Object, Array, Value, etc.).
 */
class Json
{
    friend class Parser;

public:
    // Forward declarations for iterators
    class iterator;
    class const_iterator;

    // =========================================================================
    // [Inner Classes for Iteration]
    // =========================================================================

    /**
     * @brief A proxy class that adapts keys to act as both string and integer.
     * Useful for unified access to Object keys (string) and Array indices (size_t).
     */
    class JsonKey {
    public:
        JsonKey( std::string k, size_t i, bool isArr )
            : keyStr( std::move( k ) ), indexVal( i ), isArray( isArr )
        {}

        // operator override : type cast
        operator std::string() const { return keyStr; }
        operator size_t()      const { return indexVal; }
        operator int()         const { return static_cast<int>( indexVal ); }

        // operator override : stream
        friend std::ostream& operator<<( std::ostream& os, const JsonKey& k )
        {
            if( k.isArray ) os << k.indexVal;
            else            os << k.keyStr;
            return os;
        }

        // operator override : compare
        bool operator==( const std::string& s ) const { return keyStr == s; }
        bool operator==( size_t i ) const { return indexVal == i; }
        bool operator==( int i ) const { return static_cast<int>( indexVal ) == i; }

    private:
        std::string keyStr;
        size_t      indexVal;
        bool        isArray;
    };

    /**
     * @brief A proxy object returned by items() iterator.
     * Provides .key() and .value() accessors.
     */
    template<typename IteratorType>
    class ItemProxy {
    public:
        ItemProxy( IteratorType it )
            : it( it )
        {}

        JsonKey key() const { return it.key(); }
        typename IteratorType::reference value() const { return it.value(); }

    private:
        IteratorType it;
    };

    /**
     * @brief Iterator for Json objects and arrays.
     */
    class iterator
    {
        friend class Json;

    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type        = Json;
        using difference_type   = std::ptrdiff_t;
        using pointer           = Json*;
        using reference         = Json&;

        iterator( JsonType t, JsonArrays::iterator ait, JsonArrays::iterator aStart, JsonObjects::iterator oit )
            : type( t ), arrIt( ait ), arrStart( aStart ), objIt( oit )
        {}

        // operator override : get value from ref.
        reference operator*() const
        {
            // Array
            if( type == JsonType::ARRAY )
                return *arrIt;
            // Object
            return objIt->second;
        }

        pointer operator->() const { return &( operator*() ); }

        JsonKey key() const
        {
            if( type == JsonType::ARRAY ){
                size_t idx = std::distance( arrStart, arrIt );
                return JsonKey( std::to_string( idx ), idx, true );
            }
            // Object
            return JsonKey( objIt->first, 0, false );
        }

        reference value() const { return operator*(); }

        // operator++ with ref.
        iterator& operator++()
        {
            if( type == JsonType::ARRAY ) arrIt++;
            else                          objIt++;
            return *this;
        }

        // operator++ with copy.
        iterator operator++( int )
        {
            iterator tmp = *this;
            ++( *this );
            return tmp;
        }

        bool operator!=( const iterator& other ) const
        {
            if( type == JsonType::ARRAY )
                return arrIt != other.arrIt;
            // Object
            return objIt != other.objIt;
        }

        bool operator==( const iterator& other ) const { return !( *this != other ); }

    private:
        JsonType              type;
        JsonArrays::iterator  arrIt;
        JsonArrays::iterator  arrStart;
        JsonObjects::iterator objIt;
    };

    /**
     * @brief Const Iterator for Json objects and arrays.
     */
    class const_iterator
    {
        friend class Json;

    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type        = const Json;
        using difference_type   = std::ptrdiff_t;
        using pointer           = const Json*;
        using reference         = const Json&;

        const_iterator( JsonType t, JsonArrays::const_iterator ait, JsonArrays::const_iterator aStart, JsonObjects::const_iterator oit )
            : type( t ), arrIt( ait ), arrStart( aStart ), objIt( oit )
        {}

        reference operator*() const
        {
            if( type == JsonType::ARRAY )
                return *arrIt;
            // Object
            return objIt->second;
        }

        pointer operator->() const { return &( operator*() ); }

        JsonKey key() const
        {
            if( type == JsonType::ARRAY ){
                size_t idx = std::distance( arrStart, arrIt );
                return JsonKey( std::to_string( idx ), idx, true );
            }
            // Object
            return JsonKey( objIt->first, 0, false );
        }

        reference value() const { return operator*(); }

        const_iterator& operator++()
        {
            if( type == JsonType::ARRAY ) arrIt++;
            else                          objIt++;
            return *this;
        }

        const_iterator operator++( int )
        {
            const_iterator tmp = *this;
            ++( *this );
            return tmp;
        }

        bool operator!=( const const_iterator& other ) const
        {
            if( type == JsonType::ARRAY )
                return arrIt != other.arrIt;
            return objIt != other.objIt;
        }

        bool operator==( const const_iterator& other ) const { return !( *this != other ); }

    private:
        JsonType                    type;
        JsonArrays::const_iterator  arrIt;
        JsonArrays::const_iterator  arrStart;
        JsonObjects::const_iterator objIt;
    };

    /**
     * @brief Helper for range-based items() iteration.
     */
    template<typename IteratorType>
    class ItemsRange
    {
    public:
        class items_iterator : public IteratorType
        {
        public:
            using base = IteratorType;

            items_iterator( const base& it )
                : base( it )
            {}

            ItemProxy<IteratorType> operator*() const
            {
                return ItemProxy<IteratorType>( *this );
            }

            items_iterator& operator++()
            {
                base::operator++();
                return *this;
            }

            bool operator!=( const items_iterator& other ) const
            {
                return base::operator!=( other );
            }
        };

        ItemsRange( IteratorType begin, IteratorType end )
            : m_begin( begin ), m_end( end )
        {}

        items_iterator begin() const { return items_iterator( m_begin ); }
        items_iterator end()   const { return items_iterator( m_end   ); }

    private:
        IteratorType m_begin;
        IteratorType m_end;
    };

    // =========================================================================
    // [Constructors & Destructor]
    // =========================================================================
    Json( const JsonType type ) noexcept;

    // String
    Json( const char* initVal );
    Json( const std::string& initVal );
    Json( char initVal );

    // Numeric
    Json( int       initVal );
    Json( long      initVal );
    Json( long long initVal );
    Json( double    initVal );
    Json( float     initVal );

    // Boolean
    Json( bool initVal ) noexcept;

    // Copy & Move
    Json( const Json& other );
    Json( Json&& other ) noexcept;

    // Destructor
    ~Json();

    // =========================================================================
    // [Operators]
    // =========================================================================
    Json& operator=( const Json& other );
    Json& operator=( Json&& other ) noexcept;

    // Assignment operators for value setting
    Json& operator=( const int value );
    Json& operator=( const double value );
    Json& operator=( const std::string& value );
    Json& operator=( const char* value );
    Json& operator=( const bool value );

    // Access operators
    Json& operator[]( const int arrIdx );
    Json& operator[]( const std::string& key );
    const Json& operator[]( const int arrIdx ) const;
    const Json& operator[]( const std::string& key ) const;

    // =========================================================================
    // [Type Checks]
    // =========================================================================
    inline bool isNull()   const { return this->jType == JsonType::NULL_TYPE; }
    inline bool isObject() const { return this->jType == JsonType::OBJECT;    }
    inline bool isArray()  const { return this->jType == JsonType::ARRAY;     }
    inline bool isString() const { return this->jType == JsonType::STRING;    }
    inline bool isDouble() const { return this->jType == JsonType::DOUBLE;    }
    inline bool isInt()    const { return this->jType == JsonType::INT;       }
    inline bool isBool()   const { return this->jType == JsonType::BOOLEAN;   }
    inline bool isValid()  const { return this->jType != JsonType::UNKNOWN;   }

    // =========================================================================
    // [Exist Checks]
    // =========================================================================
    /**
     * @brief Checks if a specific key exists in the JSON object.
     * @param key The key to search for.
     * @return true if the key exists, false otherwise (or if not an object).
     */
    bool contains( const std::string& key ) const;

    // =========================================================================
    // [Value Accessors]
    // =========================================================================

    /**
     * @brief Converts the JSON value to a specific C++ type.
     * @tparam T Target type (int, double, bool, std::string).
     */
    template <typename T> T getAs() const;

    /**
     * @brief Attempts to convert the JSON value to type T safely.
     * @return true if type matches and conversion succeeds, false otherwise.
     */
    template <typename T>
    bool tryGetAs( T& ref ) const
    {
        if( ( std::is_same<T, std::string>::value && this->isString() ) ||
            ( std::is_same<T, double>::value      && this->isDouble() ) ||
            ( std::is_same<T, int>::value         && this->isInt()    ) ||
            ( std::is_same<T, bool>::value        && this->isBool()   ) )
        {
            try {
                ref = getAs<T>();
                return true;
            } catch( ... ) { return false; }
        }
        return false;
    }

    // =========================================================================
    // [Utility Methods]
    // =========================================================================
    std::size_t size() const noexcept;
    std::vector<std::string> keys() const;

    /**
     * @brief Serializes the JSON object to a string.
     * @param type ToStringType::Strip (default) or ToStringType::Pretty.
     */
    std::string toString( ToStringType type = ToStringType::Strip ) const noexcept;

    /**
     * @brief Saves the JSON object to a file.
     */
    bool saveFile( const char* fileName ) const;

    // =========================================================================
    // [Data Modification (Chaining Supported)]
    // =========================================================================

    /**
     * @brief Adds a Key-Value pair (Object) or an Element (Array).
     * Automatically adapts based on the current JSON type (Object/Array).
     */
    template <typename T>
    Json& addObject( const std::string& key, const T& value )
    {
        if( this->jType == JsonType::ARRAY ){
            Json tempObj = Json( JsonType::OBJECT );
            tempObj.addProperty( key, Json( value ) );
            this->addElement( tempObj );
        } else {
            this->setType( JsonType::OBJECT );
            this->addProperty( key, Json( value ) );
        }
        return ( *this );
    }

    /**
     * @brief Adds a JSON object (Merge for Object, Push for Array).
     */
    Json& addObject( const Json& objectValue );

    /**
     * @brief Adds multiple elements to an Array.
     */
    template <typename T, typename... Types>
    Json& addElementToArray( const T& element, Types... elements )
    {
        this->addElement( Json( element ) );
        this->addElementToArray( elements... );
        return ( *this );
    }

    template <typename T>
    Json& addElementToArray( const T& element )
    {
        this->addElement( Json( element ) );
        return ( *this );
    }

    // =========================================================================
    // [Iteration Methods]
    // =========================================================================
    iterator begin();
    iterator end();

    const_iterator begin() const;
    const_iterator end()   const;

    ItemsRange<iterator>       items();
    ItemsRange<const_iterator> items() const;

private:
    Json() noexcept; // Default constructor hidden

    std::string strValue;
    JsonType    jType;
    JsonObjects properties;
    JsonArrays  arr;
    std::unordered_map<std::string, std::size_t> mapIndex;

    // Private Helpers
    std::string makeSpace( const unsigned int space ) const noexcept;

    std::string toStringPretty( const unsigned int space ) const noexcept;
    std::string toStringStrip() const noexcept;

    static std::string deserialize( const std::string& src ) noexcept;
    static std::string serialize  ( const std::string& src ) noexcept;

    // Low-level manipulators
    Json& setType( const JsonType type );
    void  setString( const std::string& s ) noexcept;
    void  addProperty( const std::string& key, const Json& value );
    void  addElement( const Json& value );
};

// =============================================================================
// [Template Specializations]
// =============================================================================
template <> int         Json::getAs<int>()         const;
template <> double      Json::getAs<double>()      const;
template <> std::string Json::getAs<std::string>() const;
template <> bool        Json::getAs<bool>()        const;

// =============================================================================
// [Global Helper Functions]
// =============================================================================

/** @brief Creates an empty JSON Object. */
Json JsonObject();

/** @brief Creates a JSON Object with an initial key-value pair. */
template <typename T>
Json JsonObject( const std::string& key, const T& value )
{
    return JsonObject().addObject( key, value );
}

/** @brief Specialization for C-string to prevent bool conversion issues. */
inline Json JsonObject( const std::string& key, const char* value )
{
    return JsonObject().addObject( key, std::string( value ) );
}

/** @brief Creates an empty JSON Array. */
Json JsonArray();

/** @brief Creates a JSON Array with initial elements. */
template <typename T, typename... Types>
Json JsonArray( const T& element, Types... elements )
{
    return JsonArray().addElementToArray( element, elements... );
}

template <typename T>
Json JsonArray( const T& element )
{
    return JsonArray().addElementToArray( element );
}

/** @brief Creates a JSON Null. */
Json JsonNULL();

} // namespace TinyJson

#endif // _TINY_JSON_H_