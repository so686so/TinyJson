#include "TinyJson.h"

// 해당 라이브러리들은 헤더 파일에는 필요 없이
// 소스 코드에서만 필요한 라이브러리들
#include <iostream>
#include <fstream>
#include <stack>
#include <cassert>
#include <algorithm> // for std::swap

namespace TinyJson
{

// ======================================================================================
// [Helper 함수 구현]
// ======================================================================================
Json JsonObject()
{
    return Json( JsonType::OBJECT );
}

Json JsonObject( const std::string& key, const Json& jsValue )
{
    return JsonObject().addObject( key, jsValue );
}

Json JsonArray()
{
    return Json( JsonType::ARRAY );
}

Json JsonNULL()
{
    return Json( JsonType::NULL_TYPE );
}

// ======================================================================================
// [Json 생성자 및 소멸자]
// ======================================================================================
Json::Json() noexcept
    : strValue( "" )
    , jType   ( JsonType::UNKNOWN )
{
}

Json::Json( const JsonType type ) noexcept
    : strValue( "" )
    , jType   ( type )
{
    if( this->isNull() ) this->setString( "null" );
}

// 1. 문자열
Json::Json( const char* initVal ) {
    this->setType( JsonType::STRING );
    this->setString( initVal );
}

Json::Json( const std::string& initVal ) {
    this->setType( JsonType::STRING );
    this->setString( initVal );
}

Json::Json( char initVal ) {
    this->setType( JsonType::STRING );
    this->setString( std::string(1, initVal) );
}

// 2. 정수 (stringstream 사용하지 않고 성능 최적화)
Json::Json( int initVal ) {
    this->setType( JsonType::INT );
    this->setString( std::to_string(initVal) );
}

Json::Json( long initVal ) {
    this->setType( JsonType::INT );
    this->setString( std::to_string(initVal) );
}

Json::Json( long long initVal ) {
    this->setType( JsonType::INT );
    this->setString( std::to_string(initVal) );
}

// 3. 실수
Json::Json( double initVal ) {
    this->setType( JsonType::DOUBLE );
    this->setString( std::to_string(initVal) );
}

Json::Json( float initVal ) {
    this->setType( JsonType::DOUBLE );
    this->setString( std::to_string(initVal) );
}

// 4. 불리언
Json::Json( bool initVal ) noexcept {
    this->setType( JsonType::BOOLEAN );
    this->setString( initVal ? "true" : "false" );
}

// 복사 생성자
Json::Json( const Json& other )
{
    this->jType      = other.jType;
    this->strValue   = other.strValue;
    this->arr        = other.arr;
    this->properties = other.properties;
    this->mapIndex   = other.mapIndex;
}

// 이동 생성자 (Move Semantics)
Json::Json( Json&& other ) noexcept
    : strValue  ( std::move( other.strValue ) )
    , jType     ( other.jType )
    , properties( std::move( other.properties ) )
    , arr       ( std::move( other.arr ) )
    , mapIndex  ( std::move( other.mapIndex ) )
{
    other.jType = JsonType::NULL_TYPE;
}

Json::~Json()
{
    //
}

// ======================================================================================
// [연산자 오버로딩 구현]
// ======================================================================================
Json& Json::operator=( const Json& other )
{
    if( this != &other ){
        this->jType      = other.jType;
        this->strValue   = other.strValue;
        this->arr        = other.arr;
        this->properties = other.properties;
        this->mapIndex   = other.mapIndex;
    }
    return (*this);
}

Json& Json::operator=( Json&& other ) noexcept
{
    if( this != &other ){
        this->strValue   = std::move( other.strValue );
        this->jType      = other.jType;
        this->properties = std::move( other.properties );
        this->arr        = std::move( other.arr );
        this->mapIndex   = std::move( other.mapIndex );

        other.jType = JsonType::NULL_TYPE;
    }
    return (*this);
}

Json& Json::operator[]( const int i )
{
    if( this->jType != JsonType::ARRAY )
        throw TinyJsonException( "Invalid access: Operator[] int used on non-array type" );

    if( i < 0 || i >= (int)this->arr.size() )
        throw TinyJsonException( "Index out of range" );

    return this->arr[i];
}

Json& Json::operator[]( const std::string& key )
{
    if( this->jType != JsonType::OBJECT && this->jType != JsonType::UNKNOWN ){
        throw TinyJsonException( "Invalid access: Operator[] string used on non-object type (Type is " + std::to_string((int)this->jType) + ")" );
    }

    // UNKNOWN이면 Object로 초기화
    if( this->jType == JsonType::UNKNOWN ) {
        this->setType( JsonType::OBJECT );
    }

    if( this->mapIndex.find( key ) == this->mapIndex.end() ) {
        this->addObject( key, JsonNULL() );
    }
    return this->properties[this->mapIndex[key]].second;
}

const Json& Json::operator[]( const int i ) const
{
    if( this->jType != JsonType::ARRAY )
        throw TinyJsonException( "Invalid access: Operator[] int used on non-array type" );

    if( i < 0 || i >= (int)this->arr.size() )
        throw TinyJsonException( "Index out of range" );

    return this->arr[i];
}

const Json& Json::operator[]( const std::string& key ) const
{
    if( this->jType != JsonType::OBJECT )
        throw TinyJsonException( "Invalid access: Operator[] string used on non-object type" );

    auto it = this->mapIndex.find( key );
    if( it == this->mapIndex.end() )
        throw TinyJsonException( "Key not found: " + key );

    return this->properties[it->second].second;
}

Json& Json::operator=( const int value )
{
    std::stringstream ss; ss << value;
    this->setType( JsonType::INT ).setString( ss.str() );
    return (*this);
}

Json& Json::operator=( const double value )
{
    std::stringstream ss; ss << value;
    this->setType( JsonType::DOUBLE ).setString( ss.str() );
    return (*this);
}

Json& Json::operator=( const std::string& value )
{
    if( Parser::isObject( value ) )
        *this = Parser::parse( value );
    else
        this->setType( JsonType::STRING ).setString( value );
    return (*this);
}

Json& Json::operator=( const char* value )
{
    return this->operator=( std::string(value) );
}

Json& Json::operator=( const bool value )
{
    this->setType( JsonType::BOOLEAN ).setString( value ? "true" : "false" );
    return (*this);
}

// ======================================================================================
// [getAs 템플릿 특수화 구현]
// ======================================================================================

// Generic 템플릿
template <typename T>
T Json::getAs() const
{
    T ret;
    std::stringstream ss( this->strValue );
    ss >> ret;
    return ret;
}

template <>
bool Json::getAs<bool>() const
{
    return ( this->strValue == "true" );
}

template <>
std::string Json::getAs<std::string>() const
{
    return this->deserialize( this->strValue );
}

template <>
int Json::getAs<int>() const
{
    try {
        return std::stoi( this->strValue );
    }
    catch (...) { return 0; }
}

template <>
double Json::getAs<double>() const
{
    try {
        return std::stod( this->strValue );
    }
    catch (...) { return 0.0; }
}

// ======================================================================================
// [직렬화 및 유틸리티 구현]
// ======================================================================================
std::string Json::deserialize( const std::string& src ) noexcept
{
    std::string out = "";

    for( std::size_t i = 0; i < src.length(); ++i )
    {
        if( src[i] == '\\' && i + 1 < src.length() )
        {
            char next = src[i+1];
            switch( next ) {
                case '"':  out += '"';  break;
                case '\\': out += '\\'; break;
                case '/':  out += '/';  break;
                case 'b':  out += '\b'; break;
                case 'f':  out += '\f'; break;
                case 'n':  out += '\n'; break;
                case 'r':  out += '\r'; break;
                case 't':  out += '\t'; break;
                default:   out += next; break;
            }
            i++;
        }
        else
        {
            out += src[i];
        }
    }
    return out;
}

std::string Json::serialize( const std::string& src ) noexcept
{
    std::string out = "";

    for( char c : src )
    {
        switch( c ){
            case '\n': out += "\\n";  break;
            case '\t': out += "\\t";  break;
            case '\b': out += "\\b";  break;
            case '\r': out += "\\r";  break;
            case '\f': out += "\\f";  break;
            case '"':  out += "\\\""; break;
            case '\\': out += "\\\\"; break;
            default:   out += c;      break;
        }
    }
    return out;
}

void Json::setString( const std::string& s ) noexcept
{
    this->strValue = this->serialize( s );
}

Json& Json::setType( const JsonType type )
{
    this->jType = type;
    return (*this);
}

std::size_t Json::size() const noexcept
{
    if( this->jType == JsonType::ARRAY )  return this->arr.size();
    if( this->jType == JsonType::OBJECT ) return this->properties.size();
    return 0;
}

std::vector<std::string> Json::keys() const
{
    std::vector<std::string> k;
    if( this->jType == JsonType::OBJECT )
    {
        for( auto& pair : this->properties ){ k.push_back( pair.first ); }
    }
    return k;
}

Json& Json::addObject( const Json& objectValue )
{
    // [Case 1] 부모가 Array인 경우 -> 입력값이 무엇이든 원소로 추가 (Push)
    if( this->jType == JsonType::ARRAY )
    {
        this->addElement( objectValue );
        return (*this);
    }

    // [Case 2] 부모가 Object (혹은 UNKNOWN) 인 경우
    // 사용자 요청 반영: 입력값이 Object라면 내부 속성들을 합친다 (Merge)
    if( objectValue.isObject() )
    {
        this->setType( JsonType::OBJECT );

        // 들어온 객체의 모든 속성(Properties)을 순회하며 내 안에 복사
        // (같은 클래스이므로 private 멤버 properties에 직접 접근 가능)
        for( const auto& prop : objectValue.properties ){
            this->addProperty( prop.first, prop.second );
        }
        return (*this);
    }

    // [Exception] 부모는 Object인데, Key도 없는 단순 값(Int, String 등)을 넣으려 할 때
    throw TinyJsonException("Cannot add a non-object value to an Object without a key.");
}

void Json::addProperty( const std::string& k, const Json& v )
{
    if( this->mapIndex.find( k ) != this->mapIndex.end() )
    {
        this->properties[this->mapIndex[k]].second = v;
    }
    else
    {
        this->mapIndex[k] = this->properties.size();
        this->properties.emplace_back( k, v );
    }
}

void Json::addElement( const Json& v )
{
    this->arr.push_back( v );
}

std::string Json::makeSpace( const unsigned int space ) const noexcept
{
    return std::string( space, ' ' );
}

std::string Json::toStringStrip() const noexcept
{
    switch( this->jType )
    {
    case JsonType::STRING:    return "\"" + this->strValue + "\"";
    case JsonType::INT:       return strValue;
    case JsonType::DOUBLE:    return strValue;
    case JsonType::BOOLEAN:   return strValue;
    case JsonType::NULL_TYPE: return "null";

    case JsonType::OBJECT:
    {
        std::string s = "{ ";
        for( std::size_t i = 0; i < this->properties.size(); ++i )
        {
            s += "\"" + this->properties[i].first + "\": " + this->properties[i].second.toStringStrip();
            if( i < this->properties.size() - 1 ) { s += ", "; }
        }
        s += " }";
        return s;
    }

    case JsonType::ARRAY:
    {
        std::string s = "[ ";
        for( std::size_t i = 0; i < this->arr.size(); ++i )
        {
            s += this->arr[i].toStringStrip();
            if( i < this->arr.size() - 1 ) { s += ", "; }
        }
        s += " ]";
        return s;
    }
    default: return "";
    }
}

std::string Json::toStringPretty( const unsigned int space ) const noexcept
{
    switch( this->jType )
    {
    case JsonType::STRING:    return "\"" + this->strValue + "\"";
    case JsonType::INT:       return strValue;
    case JsonType::DOUBLE:    return strValue;
    case JsonType::BOOLEAN:   return strValue;
    case JsonType::NULL_TYPE: return "null";

    case JsonType::OBJECT:
    {
        std::string s = "{\n";
        for( std::size_t i = 0; i < this->properties.size(); ++i )
        {
            s += makeSpace( space )
               + "\"" + this->properties[i].first + "\": "
               + this->properties[i].second.toStringPretty( space + 2 );

            if( i < this->properties.size() - 1 ) { s += ",\n"; }
            else                                  { s += "\n";  }
        }
        s += makeSpace( space > 2 ? space - 2 : 0 ) + "}";
        return s;
    }

    case JsonType::ARRAY:
    {
        std::string s = "[ ";
        for( std::size_t i = 0; i < this->arr.size(); ++i )
        {
            s += this->arr[i].toStringPretty( space );
            if( i < this->arr.size() - 1 ) { s += ", "; }
        }
        s += " ]";
        return s;
    }
    default: return "";
    }
}

std::string Json::toString( ToStringType type ) const noexcept
{
    return ( type == ToStringType::Pretty )
        ? this->toStringPretty( 2 )
        : this->toStringStrip();
}

bool Json::saveFile( const char* filename ) const
{
    std::ofstream file( filename );

    if( !file.is_open() )
        return false;

    file << this->toString( ToStringType::Pretty );
    file.close();

    return true;
}

// ======================================================================================
// [Parser 구현]
// ======================================================================================
bool Parser::isObject( const std::string& str ) noexcept
{
    try
    {
        return !Parser::tokenize( str ).empty();
    }
    catch (...)
    {
        return false;
    }
}

bool Parser::isObject( const char* str ) noexcept
{
    return Parser::isObject( std::string(str) );
}

inline bool Parser::isWhiteSpace( const char c )
{
    return isspace( c );
}

int Parser::nextWhiteSpace( const std::string& src, const int startPos ) noexcept
{
    int       i   = startPos;
    const int len = (int)src.length();

    while( i < len )
    {
        if( src[i] == '"' )
        {
            ++i;
            while( i < len && ( src[i] != '"' || src[i-1] == '\\' ) ) { ++i; }
        }
        if( src[i] == '\'' )
        {
            ++i;
            while( i < len && ( src[i] != '\'' || src[i-1] == '\\' ) ) { ++i; }
        }

        if( isWhiteSpace( src[i] ) ) { return i; }
        ++i;
    }
    return len;
}

int Parser::skipWhiteSpaces( const std::string& src, const int pos ) noexcept
{
    int       i   = pos;
    const int len = (int)src.length();

    while( i < len )
    {
        if( !isWhiteSpace( src[i] ) ) { return i; }
        ++i;
    }

    return -1;
}

std::vector<Parser::Token> Parser::tokenize( const std::string& src )
{
    std::vector<Token> tokens;
    int idx = Parser::skipWhiteSpaces( src, 0 );

    int check_brace   = 0;
    int check_bracket = 0;

    while( idx >= 0 )
    {
        int next = Parser::nextWhiteSpace( src, idx );

        std::string str = src.substr( idx, next - idx );
        std::size_t len = str.length();
        std::size_t k   = 0;

        while( k < len )
        {
            char c = str[k];
            if( c == '"' )
            {
                std::size_t tmp = k + 1;

                while( tmp < len && ( str[tmp] != '"' || str[tmp-1] == '\\' ) )
                {
                    ++tmp;
                }

                tokens.emplace_back( str.substr( k + 1, tmp - k - 1 ), TokenType::STRING );
                k = tmp + 1;
            }
            else if( c == ',' )
            {
                tokens.emplace_back( ",", TokenType::COMMA );
                ++k;
            }
            else if( c == '{' )
            {
                tokens.emplace_back( "{", TokenType::CBRACE_OPEN );
                ++k; ++check_brace;
            }
            else if( c == '}' )
            {
                tokens.emplace_back( "}", TokenType::CBRACE_CLOSE );
                ++k; --check_brace;
            }
            else if( c == '[' )
            {
                tokens.emplace_back( "[", TokenType::BRACKET_OPEN );
                ++k; ++check_bracket;
            }
            else if( c == ']' )
            {
                tokens.emplace_back( "]", TokenType::BRACKET_CLOSE );
                ++k; --check_bracket;
            }
            else if( c == ':' )
            {
                tokens.emplace_back( ":", TokenType::COLON );
                ++k;
            }
            else if( c == 't' && k + 3 < len && str.substr( k, 4 ) == "true" )
            {
                tokens.emplace_back( "true", TokenType::BOOLEAN );
                k += 4;
            }
            else if( c == 'f' && k + 4 < len && str.substr( k, 5 ) == "false" )
            {
                tokens.emplace_back( "false", TokenType::BOOLEAN );
                k += 5;
            }
            else if( c == 'n' && k + 3 < len && str.substr( k, 4 ) == "null" )
            {
                tokens.emplace_back( "null", TokenType::NULL_TYPE );
                k += 4;
            }
            else if( c == '-' || ( c >= '0' && c <= '9' ) )
            {
                std::size_t tmp = k;
                if( str[tmp] == '-' ) { ++tmp; }

                TokenType type = TokenType::INT;
                while( tmp < len && ( ( str[tmp] >= '0' && str[tmp] <= '9' ) || str[tmp] == '.' ) )
                {
                    if( str[tmp] == '.' ) { type = TokenType::DOUBLE; }
                    ++tmp;
                }

                tokens.emplace_back( str.substr( k, tmp - k ), type );
                k = tmp;
            }
            else
            {
                ++k;
            }
        }
        idx = Parser::skipWhiteSpaces( src, next );
    }

    if( check_brace != 0 || check_bracket != 0 )
        throw TinyJsonException( "Mismatched brackets or braces in JSON" );

    return tokens;
}

Json Parser::jsonParse( const std::vector<Token>& v, int& curPos )
{
    if( curPos >= (int)v.size() )
        return JsonNULL();

    Token token = v[curPos];
    Json  current( JsonType::UNKNOWN );

    if( token.type == TokenType::CBRACE_OPEN )
    {
        current.setType( JsonType::OBJECT );

        int k = curPos + 1;

        while( k < (int)v.size() && v[k].type != TokenType::CBRACE_CLOSE )
        {
            std::string key = v[k].value;

            if( v[k+1].type != TokenType::COLON )
                throw TinyJsonException( "Expected ':' after key in object" );

            int nextPos = k + 2;

            Json val = Parser::jsonParse( v, nextPos );

            current.addProperty( key, val );
            k = nextPos;

            if( k < (int)v.size() )
            {
                // 콤마가 있으면 다음 요소로 진행
                if( v[k].type == TokenType::COMMA )
                {
                    ++k;
                }
                // 닫는 괄호면 루프 종료 (while 조건에서 처리됨)
                else if( v[k].type == TokenType::CBRACE_CLOSE )
                {
                    continue;
                }
                // 콤마도 아니고 닫는 괄호도 아니면 문법 오류!
                else
                {
                    throw TinyJsonException( "Expected ',' or '}' after property in object" );
                }
            }
        }
        curPos = k + 1;
    }
    else if( token.type == TokenType::BRACKET_OPEN )
    {
        current.setType( JsonType::ARRAY );

        int k = curPos + 1;

        while( k < (int)v.size() && v[k].type != TokenType::BRACKET_CLOSE )
        {
            int  nextPos = k;
            Json val     = Parser::jsonParse( v, nextPos );

            current.addElement( val );
            k = nextPos;

            if( k < (int)v.size() && v[k].type == TokenType::COMMA ) { ++k; }
        }
        curPos = k + 1;
    }
    else
    {
        switch( token.type )
        {
        // setString()은 내부적으로 serialize()를 호출하므로 이중 이스케이프가 발생함.
        // 토큰 값은 이미 포맷팅된 문자열이므로 strValue에 직접 대입해야 함.
        case TokenType::STRING:
            current.setType( JsonType::STRING );
            current.strValue = token.value;
            break;

        case TokenType::INT:
            current.setType( JsonType::INT ).setString( token.value );
            break;

        case TokenType::DOUBLE:
            current.setType( JsonType::DOUBLE ).setString( token.value );
            break;

        case TokenType::BOOLEAN:
            current.setType( JsonType::BOOLEAN ).setString( token.value );
            break;

        case TokenType::NULL_TYPE:
            current.setType( JsonType::NULL_TYPE ).setString( "null" );
            break;

        default:
            break;
        }
        curPos++;
    }
    return current;
}

Json Parser::parse( const char* str )
{
    return Parser::parse( std::string(str) );
}

Json Parser::parse( const std::string& str )
{
    try
    {
        std::vector<Token> tokens = Parser::tokenize( str );

        if( tokens.empty() )
            return JsonNULL();

        int pos = 0;
        return Parser::jsonParse( tokens, pos );
    }
    catch ( const std::exception& e )
    {
        throw TinyJsonException( std::string("Parse Error: ") + e.what() );
    }
}

Json Parser::parseFile( const std::string& fileName )
{
    std::ifstream ifs( fileName );
    if( !ifs.is_open() )
        throw TinyJsonException( "File open failed: " + fileName );

    std::stringstream buffer;
    buffer << ifs.rdbuf();
    return Parser::parse( buffer.str() );
}

} // namespace TinyJson