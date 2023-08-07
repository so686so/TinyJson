#include "TinyJson.h"

namespace TinyJson
{
using namespace std;

// About JsonObject & JsonArray ============================================
JsonValue JsonObject()
{
    return JsonValue( JsonType::OBJECT );
}

JsonValue JsonObject( const string& key, const JsonValue& jsValue )
{
    return JsonObject().addObject( key, jsValue );
}

JsonValue JsonArray()
{
    return JsonValue( JsonType::ARRAY );
}

JsonValue JsonNULL()
{
    return JsonValue( JsonType::NUL );
}

// About JsonValue ==========================================================
JsonValue::JsonValue() noexcept
    : strValue( "" )
    , jType( JsonType::UNKNOWN )
{
    //
}

JsonValue::JsonValue( const JsonType type ) noexcept
    : strValue( "" )
    , jType( type )
{
    if( this->isNull() ) this->setString( "null" );
}

/**Notation:
 * ---------
 * The C++ standard does not allow explicit specialization of class members at class scope.
 * The getAs<T>() template specials getAs<bool>() and getAs<string>() are defined outside the class.
 */
template <>
bool JsonValue::getAs()
{
    return ( this->strValue == "true" ) ? true : false;
}

template <>
string JsonValue::getAs()
{
    return this->deserialize( this->strValue );
}

string JsonValue::deserialize( const string& src ) noexcept
{
    string out = "";
    for( size_t i = 0; i < src.length(); ++i ) {
        if( src[i] == '\\' && i+1 < src.length() ) {
            if     ( src[i+1] == '\"' ) out += '"' ;
            else if( src[i+1] == '\\' ) out += '\\';
            else if( src[i+1] == '/'  ) out += '/' ;
            else if( src[i+1] == 'b'  ) out += '\b';
            else if( src[i+1] == 'f'  ) out += '\f';
            else if( src[i+1] == 'n'  ) out += '\n';
            else if( src[i+1] == 'r'  ) out += '\r';
            else if( src[i+1] == 't'  ) out += '\t';
            i++;
            continue;
        }
        out += src[i];
    }
    return out;
}

string JsonValue::serialize( const string& src ) noexcept
{
    string out = "";
    for( size_t i = 0; i < src.length(); ++i ) {
        if( src[i] == '\n' ) out += "\\n";
        if( src[i] == '\t' ) out += "\\t";
        if( src[i] == '\b' ) out += "\\b";
        if( src[i] == '\r' ) out += "\\r";
        if( src[i] == '\f' ) out += "\\f";
        else                 out += src[i];
    }
    return out;
}

inline string JsonValue::makeSpace( const unsigned int space ) noexcept 
{
    string s = "";
    unsigned int n = space;
    while( n-- > 0 ) s += " ";
    return s;
}

const string JsonValue::toStringPretty( const unsigned int space = 1 ) noexcept
{
    switch( this->jType )
    {
    case JsonType::STRING:  return string("\"") + this->strValue + string("\"");
    case JsonType::INT:     return strValue;
    case JsonType::DOUBLE:  return strValue;
    case JsonType::BOOLEAN: return strValue;
    case JsonType::NUL:     return "null";

    case JsonType::OBJECT:
    {
        int index = 0;
        string s = string("{\n");
        for( auto& prop : this->properties ) {
            s += makeSpace( space ) 
              +  string("\"") 
              +  prop.first  // key
              +  string("\": ") 
              +  prop.second.toStringPretty( space + 1 ) // value
              +  string( ( ++index == this->properties.size() ) ? "" : "," )
              +  string("\n");
        }
        s += makeSpace( space - 1 ) + string("}");
        return s;
    }

    case JsonType::ARRAY:
    {
        int index = 0;
        string s = string("[ ");
        for( auto& elem : this->arr ) {
            if( index++ > 0 ) s += ", ";
            s += elem.toStringPretty( space + 1 );
        }
        s += string(" ]");
        return s;
    }

    default:
    case JsonType::UNKNOWN:
        break;
    }
    return "#UNKNOWN#";
}

void JsonValue::addProperty( const string& k, const JsonValue& v ) noexcept
{
    // add new key:value
    if( this->mapIndex.find( k ) == this->mapIndex.end() ) {
        this->mapIndex[k] = this->properties.size();
        this->properties.emplace_back( k, v );
    } 
    // if key already exist, replace existing key:value
    else {
        JsonValue& src = const_cast<JsonValue&>(v);
        JsonValue& dst = this->properties[this->mapIndex[k]].second;

        if     ( src.isBool()   ) dst = src.getAs<bool>();
        else if( src.isInt()    ) dst = src.getAs<int>();
        else if( src.isString() ) dst = src.getAs<string>();
        else if( src.isDouble() ) dst = src.getAs<double>();
        else if( src.isNull()   ) dst = JsonNULL(); 
        else if( src.isObject() ) dst = src;
        else if( src.isArray()  ) dst = src;
    }
}

void JsonValue::addElement( const JsonValue& v ) noexcept
{
    if( !this->isArray() ) {
        cout << "addArray() canceled, target is not array type" << endl;
        return;
    }
    this->arr.emplace_back(v);
}

void JsonValue::setString( const string& s ) noexcept
{
    assert( !s.empty() );
    this->strValue = this->serialize( s );
}

// Returns how many elements the JsonValue has if it is Objects or Arrays
const size_t JsonValue::size() noexcept
{
    switch( this->jType )
    {
    case JsonType::ARRAY:  return this->arr.size();
    case JsonType::OBJECT: return this->properties.size();
    default:               return 0;
    }
}

JsonValue& JsonValue::operator[]( const int i )
{
    try {
        if( this->jType != JsonType::ARRAY )
            throw this->jType;

        if( i >= this->arr.size() )
            throw std::out_of_range("Json get value failed, you try operator[Index] but out of range");

        return this->arr[i];
    }
    catch( JsonType exType ) {
        cout << "Json get value failed, you try operator[Index] but that json type not array, " 
             << static_cast<int>(exType) << endl;
        exit( -1 );
    }
    catch( const std::exception& error ) {
        cerr << error.what() << endl;
        exit( -1 );
    }
}

JsonValue& JsonValue::operator[]( const string& key )
{
    if( this->mapIndex.find( key ) == this->mapIndex.end() ) {
        this->addObject( key, JsonNULL() );
    }
    return this->properties[this->mapIndex[key]].second;
}

JsonValue& JsonValue::operator=( const int value )
{
    stringstream ss; ss << value;
    this->setType( JsonType::INT ).setString( ss.str() );
    return (*this);
}

JsonValue& JsonValue::operator=( const double value )
{
    stringstream ss; ss << value;
    this->setType( JsonType::DOUBLE ).setString( ss.str() );
    return (*this);
}

JsonValue& JsonValue::operator=( const string& value )
{
    JsonValue js = Parser::parse( value, false );

    if( js.isArray() || js.isObject() )
        *this = js;
    else 
        this->setType( JsonType::STRING ).setString( value );

    return (*this);
}

JsonValue& JsonValue::operator=( const char* value )
{
    JsonValue js = Parser::parse( string(value), false );

    if( js.isArray() || js.isObject() )
        *this = js;
    else 
        this->setType( JsonType::STRING ).setString( value );

    return (*this);
}

JsonValue& JsonValue::operator=( const bool value )
{
    this->setType( JsonType::BOOLEAN ).setString( value ? "true" : "false" );
    return (*this);
}

JsonValue& JsonValue::operator=( JsonValue other )
{
    this->jType      = other.jType;
    this->strValue   = other.strValue;
    this->arr        = other.arr;
    this->properties = other.properties;
    this->mapIndex   = other.mapIndex;

    return (*this);
}


string JsonValue::toString() noexcept
{
    return this->toStringPretty();
}

const bool JsonValue::saveFile( const char* filename )
{
    ofstream file(filename);
    if( !file.is_open() ) return false;

    file << this->toString();
    file.close();

    cout << "File save done : " << filename << endl;
    return true;
}

// About Parser ==========================================================
inline const bool Parser::isWhiteSpace( const char c )
{
    return isspace(c);
}

const int Parser::nextWhiteSpace( const string& src, const int startPos ) noexcept
{
    int i = startPos;
    const int len = static_cast<int>( src.length() );

    while( i < len ) {
        if( src[i] == '"' ) {
            ++i;
            while( i < len && ( src[i] != '"' || src[i-1] == '\\' ) ) ++i;
        }
        if( src[i] == '\'' ) {
            ++i;
            while( i < len && ( src[i] != '\'' || src[i-1] == '\\' ) ) ++i;
        }
        if( isWhiteSpace( src[i] ) ) return i;
        ++i;
    }
    return len;
}

const int Parser::skipWhiteSpaces( const string& src, const int pos ) noexcept
{
    int i = pos;
    const int len = static_cast<int>( src.length() );

    while( i < len ) {
        if( !isWhiteSpace( src[i] ) ) return i;
        ++i;
    }
    return -1;
}

vector<Parser::Token> Parser::tokenize( const string& src, const bool validation_on = true )
{
    string source(src);
    vector<Token> tokens;

    // Check brackets pair
    static int check_cbrace  = 0;
    static int check_bracket = 0;

    int idx = Parser::skipWhiteSpaces( source, 0 );
    while( idx >= 0 ) {
        const int    next = Parser::nextWhiteSpace( source, idx );
        string       str  = source.substr( idx, next - idx );
        const size_t len  = str.length();

        size_t k = 0;
        while( k < len ) {
            if( str[k] == '"' ) {
                size_t tmp_k = k+1;
                while( tmp_k < len && ( str[tmp_k] != '"' || str[tmp_k-1] == '\\' ) ) ++tmp_k;
                tokens.emplace_back( str.substr( k+1, tmp_k-k-1), TokenType::STRING );
                k = tmp_k+1;
                continue;
            }
            else if( str[k] == '\'' ) {
                size_t tmp_k = k+1;
                while( tmp_k < len && ( str[tmp_k] != '\'' || str[tmp_k-1] == '\\' ) ) ++tmp_k;
                tokens.emplace_back( str.substr( k+1, tmp_k-k-1 ), TokenType::STRING );
                k = tmp_k+1;
                continue;                
            }
            else if( str[k] == ',' ) {
                tokens.emplace_back( ",", TokenType::COMMNA );
                ++k;
                continue;
            }
            else if( str[k] == 't' && k+3 < len && str.substr( k, 4 ) == "true" ) {
                tokens.emplace_back( "true", TokenType::BOOLEAN );
                k += 4;
                continue;
            }
            else if( str[k] == 'f' && k+4 < len && str.substr( k, 5 ) == "false" ) {
                tokens.emplace_back( "false", TokenType::BOOLEAN );
                k += 5;
                continue;                
            }
            else if( str[k] == 'n' && k+3 < len && str.substr( k, 4 ) == "null" ) {
                tokens.emplace_back( "null", TokenType::NUL );
                k += 4;
                continue;
            }
            else if( str[k] == '{' ) {
                tokens.emplace_back( "{", TokenType::CBRACE_OPEN );
                ++k;
                ++check_cbrace;
                continue;
            }
            else if( str[k] == '}' ) {
                tokens.emplace_back( "}", TokenType::CBRACE_CLOSE );
                ++k;
                --check_cbrace;
                continue;
            }
            else if( str[k] == '[' ) {
                tokens.emplace_back( "[", TokenType::BRACKET_OPEN );
                ++k;
                check_bracket++;
                continue;
            }
            else if( str[k] == ']' ) {
                tokens.emplace_back( "]", TokenType::BRACKET_CLOSE );
                ++k;
                check_bracket--;
                continue;
            }
            else if( str[k] == ':' ) {
                tokens.emplace_back( ":", TokenType::COLON );
                ++k;
                continue;
            }
            else if( str[k] == '-' || ( str[k] <= '9' && str[k] >= '0' ) ) {
                size_t tmp_k = k;
                if( str[tmp_k] == '-' ) ++tmp_k;

                TokenType currentType = TokenType::INT;
                while( tmp_k < len && ( ( str[tmp_k] <= '9' && str[tmp_k] >= '0' ) || str[tmp_k] == '.' ) ) {
                    if( str[tmp_k] == '.' ) currentType = TokenType::DOUBLE;
                    ++tmp_k;
                }
                tokens.emplace_back( str.substr( k, tmp_k-k ), currentType );
                k = tmp_k;
                continue;
            }
            // else
            tokens.emplace_back( str.substr( k ), TokenType::UNKNOWN );
            k = len;
        } // while( k < len )
        idx = Parser::skipWhiteSpaces( source, next );
    }

    // simple bracket pair validation
    if( validation_on ) {
        assert( check_bracket == 0 && "Mismatched pairs of [ , ]" );
        assert( check_cbrace  == 0 && "Mismatched pairs of { , }" );
    }

    return tokens;
}

JsonValue Parser::jsonParse( const vector<Token>& v, int& curPos, const bool validation_on = true )
 {
    JsonValue current = JsonObject().setType( JsonType::UNKNOWN );

    const int i = curPos;
    const TokenType tokenType = v[i].type;

    static stack<TokenType> bracket_check {};

    switch( tokenType )
    {
    case TokenType::CBRACE_OPEN:
    {
        current.setType( JsonType::OBJECT );
        bracket_check.push( TokenType::CBRACE_OPEN );

        int k = i+1;
        while( v[k].type != TokenType::CBRACE_CLOSE ) {
            string key = v[k].value;

            k += 2; // k+1 should be ':'
            JsonValue object = Parser::jsonParse( v, k );
            current.addProperty( key, object );

            if( v[k].type == TokenType::COMMNA ) 
                ++k;
        }

        TokenType check = bracket_check.top(); 
        bracket_check.pop();

        if( validation_on )
            assert( check == TokenType::CBRACE_OPEN && "Parenthesis Ordered Pair Validation Error Occurred" );

        curPos = k+1;
        break;
    }

    case TokenType::BRACKET_OPEN:
    {
        current.setType( JsonType::ARRAY );
        bracket_check.push(TokenType::BRACKET_OPEN);
        
        int k = i+1;
        while( v[k].type != TokenType::BRACKET_CLOSE ) {
            JsonValue element = Parser::jsonParse( v, k );
            current.addElement( element );

            if( v[k].type == TokenType::COMMNA )
                ++k;
        }

        TokenType check = bracket_check.top(); 
        bracket_check.pop();

        if( validation_on )
            assert( check == TokenType::BRACKET_OPEN && "Parenthesis Ordered Pair Validation Error Occurred" );

        curPos = k+1;
        break;
    }

    case TokenType::INT:
    {
        current.setType( JsonType::INT ).setString( v[i].value );
        curPos = i+1;
        break;
    }

    case TokenType::DOUBLE:
    {
        current.setType( JsonType::DOUBLE ).setString( v[i].value );
        curPos = i+1;
        break;
    }

    case TokenType::STRING:
    {
        current.setType( JsonType::STRING ).setString( v[i].value );
        curPos = i+1;
        break;
    }

    case TokenType::BOOLEAN:
    {
        current.setType( JsonType::BOOLEAN ).setString( v[i].value );
        curPos = i+1;
        break;
    }

    case TokenType::NUL:
    {
        current.setType( JsonType::NUL ).setString( v[i].value );
        curPos = i+1;
        break;
    }

    default:
        break;
    }

    if( validation_on )
        assert( ( v.size() != curPos || bracket_check.empty() ) && "Parenthesis Ordered Pair Validation Error Occurred Last" );
    else
        while( !bracket_check.empty() ) bracket_check.pop();

    return current;
}

JsonValue Parser::parse( const string& str, const bool valid_on = true )
{
    int startPos = 0;
    return Parser::jsonParse( tokenize( str, valid_on ), startPos, valid_on );
}

JsonValue Parser::parseFile( const string& fileName )
{
    ifstream ifs( fileName.c_str() );

    if( !ifs.good() ) {
        cout << "file read failed." << endl;
        return JsonObject();
    }

    string str {};
    string tmp {};

    while( getline( ifs, tmp ) ) 
        str += tmp;

    ifs.close();

    return Parser::parse( str );
}

}; // nsp:TinyJson