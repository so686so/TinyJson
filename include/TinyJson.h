#ifndef _TINY_JSON_PARSER_H_
#define _TINY_JSON_PARSER_H_

/**
 * Simple & tiny json parser (Refactored)
 * ------------------------------------------
 * Based on work by : So Byung Jun
 * Refactored by    : Gemini
 * Last Update      : 2026.01.14
 * ------------------------------------------
 * Reference : https://github.com/amir-s/jute
 */

#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <type_traits>
#include <exception>
#include <utility> // for std::pair, std::move

namespace TinyJson {

// ======================================================================================
// [예외 클래스 정의]
// 파싱 실패나 잘못된 접근 시 프로세스를 종료하지 않고
// std::exception 클래스를 상속받은 예외 메세지를 던집니다.
// ======================================================================================
class TinyJsonException : public std::exception
{
private:
    std::string message;

public:
    explicit TinyJsonException( const std::string& msg ) : message( msg ) {}
    virtual const char* what() const noexcept override { return message.c_str(); }
};

// ======================================================================================
// [전방 선언] : TinyJson::Json
// ======================================================================================
class Json;

// ======================================================================================
// [타입 정의]
// JSON 내부에서 사용할 데이터 타입과 열거형을 정의합니다.
// ======================================================================================
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

enum class ToStringType {
    Strip, // 공백 없는 문자열
    Pretty // 들여쓰기가 적용된 문자열
};

// ======================================================================================
// 별칭 정의
// ======================================================================================
using JsonObjects = std::vector<std::pair<std::string, Json>>;
using JsonArrays  = std::vector<Json>;

// ======================================================================================
// [Parser 클래스]
// 문자열이나 파일을 읽어 Json 객체로 변환하는 정적 메서드를 제공합니다.
// ======================================================================================
class Parser
{
public:
    // ----------------------------------------------------------------------------------
    // 문자열을 파싱하여 Json 객체로 반환합니다.
    // 실패 시 TinyJsonException을 throw 합니다.
    // ----------------------------------------------------------------------------------
    /**
     * @brief JSON 형식의 문자열을 파싱하여 Json 객체로 변환합니다.
     * @param str JSON 형식의 문자열 ( char* 문자열 형식 )
     * @return 파싱된 Json 객체
     * @throws TinyJsonException 파싱 실패(문법 오류) 시 예외 발생
     */
    static Json parse( const char* str );

    /**
     * @brief JSON 형식의 문자열을 파싱하여 Json 객체로 변환합니다.
     * @param str JSON 형식의 문자열 (std::string)
     * @return 파싱된 Json 객체
     * @throws TinyJsonException 파싱 실패(문법 오류) 시 예외 발생
     */
    static Json parse( const std::string& str );

    // ----------------------------------------------------------------------------------
    // 파일을 읽어 파싱합니다.
    // ----------------------------------------------------------------------------------
    /**
     * @brief 파일 경로에서 JSON 파일을 읽어 파싱합니다.
     * @param fileName 읽을 파일의 경로. 실행 파일 기준 상대 경로 혹은 절대 경로 사용.
     * @return 파싱된 Json 객체
     * @throws TinyJsonException 파일이 없거나 읽기 실패, 또는 파싱 실패 시 예외 발생
     * @note 파일 경로가 상대 경로일 경우, 컴파일 된 실행 바이너리의 경로를 기준으로 함.
     */
    static Json parseFile( const std::string& fileName );

    // ----------------------------------------------------------------------------------
    // 해당 문자열이 유효한 JSON 객체 형태인지 검사합니다.
    // ----------------------------------------------------------------------------------
    static bool isObject( const char* str ) noexcept;
    static bool isObject( const std::string& str ) noexcept;

// 내부 구조체 및 열거형
private:
    enum class TokenType
    {
        UNKNOWN,
        STRING,
        INT,
        DOUBLE,
        CBRACE_OPEN,   // {
        CBRACE_CLOSE,  // }
        BRACKET_OPEN,  // [
        BRACKET_CLOSE, // ]
        COMMA,         // ,
        COLON,         // :
        BOOLEAN,
        NULL_TYPE
    };

    struct Token
    {
        std::string value;
        TokenType   type;

        Token( std::string _value = "", TokenType _type = TokenType::UNKNOWN )
            : value( _value )
            , type ( _type  )
        {
            //
        }
    };

// 내부 함수
private:
    inline static bool isWhiteSpace( const char c );

    static int nextWhiteSpace ( const std::string& src, const int startPos ) noexcept;
    static int skipWhiteSpaces( const std::string& src, const int pos      ) noexcept;

    // 토큰화 함수
    static std::vector<Token> tokenize( const std::string& src );

    // 재귀적 파싱 함수
    static Json jsonParse( const std::vector<Token>& v, int& curPos );
};

// ======================================================================================
// [Helper 함수]
// 사용자가 쉽게 객체나 배열을 생성할 수 있도록 돕는 전역 함수들입니다.
// ======================================================================================
/**
 * @brief 빈 JSON 객체(Object)를 생성합니다.
 * @return JsonType::OBJECT 타입의 빈 Json 인스턴스
 * @code
 * Json js = JsonObject(); // {}
 * @endcode
 */
Json JsonObject();

/**
 * @brief 초기 키-값을 가진 JSON 객체를 생성합니다.
 * @param key 속성 키 (std::string)
 * @param value 속성 값 (Json으로 변환 가능한 모든 타입)
 * @return 초기값이 설정된 Json 인스턴스
 * @code
 * Json js = JsonObject("name", "TinyJson"); // { "name": "TinyJson" }
 * @endcode
 */
Json JsonObject( const std::string& key, const Json& jsValue );

// 내부 로직에서 주로 사용하는 함수.
// 해당 함수 대신 JsonObject / JsonArray 로 최초 객체 생성 권장.
Json JsonNULL();

/**
 * @brief 빈 JSON 배열(Array)을 생성합니다.
 * @return JsonType::ARRAY 타입의 빈 Json 인스턴스
 * @code
 * Json arr = JsonArray(); // []
 * @endcode
 */
Json JsonArray();

// ======================================================================================
// [Json 클래스]
// JSON 데이터의 최소 단위입니다. 객체, 배열, 문자열, 숫자 등을 모두 표현합니다.
// ======================================================================================
class Json
{
    // Parser가 private 메서드(setType, addProperty 등)에 접근할 수 있도록 허용
    friend class Parser;

public:
    // Iterator & Items implements for range-based loop
    // 전방 선언
    class iterator;
    class const_iterator;

private:
    // 기본 생성자 (private으로 숨김, JsonObject() 등 사용 유도)
    Json() noexcept;

public:
    Json( const JsonType type ) noexcept;

    // 1. 문자열 계열 (String)
    Json( const char*        initVal );
    Json( const std::string& initVal );
    Json( char               initVal );

    // 2. 정수 계열 (Integer)
    Json( int       initVal );
    Json( long      initVal );
    Json( long long initVal );

    // 3. 실수 계열 (Double)
    Json( double initVal );
    Json( float  initVal );

    // 4. 불리언 계열 (Boolean)
    Json( bool initVal ) noexcept;

    // ----------------------------------------------------------------------------------
    // [복사 생성자]
    // ----------------------------------------------------------------------------------
    Json( const Json& other );

    // ----------------------------------------------------------------------------------
    // [이동 생성자] (Move Semantics)
    // ----------------------------------------------------------------------------------
    Json( Json&& other ) noexcept;

    // ----------------------------------------------------------------------------------
    // [소멸자]
    // ----------------------------------------------------------------------------------
    ~Json();

    // ----------------------------------------------------------------------------------
    // [대입 연산자] 복사 및 이동
    // ----------------------------------------------------------------------------------
    Json& operator=( const Json& other );
    Json& operator=( Json&& other ) noexcept;

    // ----------------------------------------------------------------------------------
    // [값 설정 연산자]
    // ----------------------------------------------------------------------------------
    Json& operator=( const int value );
    Json& operator=( const double value );
    Json& operator=( const std::string& value );
    Json& operator=( const char* value );
    Json& operator=( const bool value );

    // ----------------------------------------------------------------------------------
    // [접근 연산자] 배열 인덱스 및 객체 키 접근
    // ----------------------------------------------------------------------------------
    /**
     * @brief 배열(Array)에서 인덱스(Index)를 사용하여 값에 접근합니다.
     * @param arrIdx 0부터 시작하는 인덱스
     * @return 해당 인덱스에 위치한 Json 값의 참조(Reference)
     * @throws TinyJsonException 현재 객체가 ARRAY 타입이 아니거나, 인덱스 범위를 초과한 경우 예외 발생
     */
    Json& operator[]( const int arrIdx );

    /**
     * @brief 객체(Object)에서 키(Key)를 사용하여 값에 접근합니다.
     * @note 해당 키가 존재하지 않을 경우,
     * JSON이 Object 타입이라면 자동으로 JsonNULL()을 생성하여 삽입합니다.
     * JSON이 Object 타입이 아니라면 예외를 반환합니다.
     * @param key 찾을 키 이름
     * @return 해당 키에 매핑된 Json 값의 참조(Reference)
     * @throws TinyJsonException 현재 객체가 OBJECT 타입이 아닐 경우 예외 발생
     */
    Json& operator[]( const std::string& key );

    const Json& operator[]( const int arrIdx ) const;
    const Json& operator[]( const std::string& key ) const;

    // ----------------------------------------------------------------------------------
    // [타입 확인 함수]
    // ----------------------------------------------------------------------------------
    inline bool isNull()   const { return this->jType == JsonType::NULL_TYPE; };
    inline bool isObject() const { return this->jType == JsonType::OBJECT;    };
    inline bool isArray()  const { return this->jType == JsonType::ARRAY;     };
    inline bool isString() const { return this->jType == JsonType::STRING;    };
    inline bool isDouble() const { return this->jType == JsonType::DOUBLE;    };
    inline bool isInt()    const { return this->jType == JsonType::INT;       };
    inline bool isBool()   const { return this->jType == JsonType::BOOLEAN;   };

    /**
     * @brief 현재 Json 객체가 유효한지(UNKNOWN이 아닌지) 확인합니다.
     * @return 유효하면 true, 아니면 false
     */
    inline bool isValid() const { return this->jType != JsonType::UNKNOWN; };

    // ----------------------------------------------------------------------------------
    // [값 추출 함수] getAs<T>()
    // ----------------------------------------------------------------------------------
    /**
     * @brief Json 값을 특정 C++ 타입으로 변환하여 반환합니다.
     * @tparam T 변환할 목표 타입 (int, double, bool, std::string)
     * @return 변환된 값
     * @note 변환에 실패할 경우 0, false, 또는 빈 문자열 등 기본값을 반환할 수 있습니다.
     */
    template <typename T> T getAs() const;

    // ----------------------------------------------------------------------------------
    // [안전한 값 추출] tryGetAs<T>()
    // ----------------------------------------------------------------------------------
    /**
     * @brief Json 값을 안전하게 특정 타입으로 변환을 시도합니다.
     * @tparam T 변환할 목표 타입
     * @param[out] ref 변환 성공 시 값을 저장할 참조 변수
     * @return 변환 성공 시 true, 실패(타입 불일치 등) 시 false
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
            }
            catch(...) { return false; }
        }
        return false;
    }

    // ----------------------------------------------------------------------------------
    // [유틸리티 함수]
    // ----------------------------------------------------------------------------------
    /**
     * @brief JSON 객체(Object) 또는 배열(Array)이 포함하고 있는 요소의 개수를 반환합니다.
     * @return 요소의 개수 (std::size_t)
     * @note
     * Array : 배열에 들어있는 항목(Element)의 총 개수.
     * Object: 객체가 가지고 있는 키-값(Key-Value) 쌍의 개수.
     * 그 외(String, Int, Null 등): 0 반환.
     */
    std::size_t size() const noexcept;

    /**
     * @brief 객체(Object)가 가진 모든 키(Key)의 목록을 반환합니다.
     * @return 키 문자열들을 담은 vector
     */
    std::vector<std::string> keys() const;

    // -------------------------------------------------------------------------
    // [JsonKey] 키 값을 문자열 또는 숫자로 자유롭게 변환해주는 래퍼 클래스
    // -------------------------------------------------------------------------
    class JsonKey {
    public:
        JsonKey(std::string k, size_t i, bool isArr)
            : keyStr(std::move(k)), indexVal(i), isArray(isArr) {}

        // 1. 문자열로 변환 (Object의 Key 또는 Array의 인덱스 문자열)
        operator std::string() const { return keyStr; }

        // 2. 정수로 변환 (Array의 인덱스)
        operator size_t() const { return indexVal; }
        operator int()    const { return static_cast<int>(indexVal); }

        // 3. cout 출력을 위한 연산자
        friend std::ostream& operator<<(std::ostream& os, const JsonKey& k) {
            if (k.isArray) os << k.indexVal;
            else           os << k.keyStr;
            return os;
        }

        // 비교 연산자 지원
        bool operator==(const std::string& s) const { return keyStr == s; }
        bool operator==(size_t i) const { return indexVal == i; }
        bool operator==(int i) const { return static_cast<int>(indexVal) == i; }

    private:
        std::string keyStr;
        size_t      indexVal;
        bool        isArray;
    };

    // -------------------------------------------------------------------------
    // [Item Proxy] items() 루프의 요소. key()가 JsonKey 객체를 반환함
    // -------------------------------------------------------------------------
    template<typename IteratorType>
    class ItemProxy {
    public:
        ItemProxy(IteratorType it) : it(it) {}

        // [핵심] Key가 JsonKey 래퍼를 반환
        JsonKey key() const {
            return it.key_wrapper(); // Iterator가 래퍼를 생성해서 줌
        }

        typename IteratorType::reference value() const { return it.value(); }

    private:
        IteratorType it;
    };

    // -------------------------------------------------------------------------
    // [Iterator]
    // -------------------------------------------------------------------------
    class iterator
    {
        friend class Json;
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type        = Json;
        using difference_type   = std::ptrdiff_t;
        using pointer           = Json*;
        using reference         = Json&;

        iterator(JsonType t, JsonArrays::iterator ait, JsonArrays::iterator aStart, JsonObjects::iterator oit)
            : type(t), arrIt(ait), arrStart(aStart), objIt(oit) {}

        reference operator*() const {
            if (type == JsonType::ARRAY) return *arrIt;
            return objIt->second;
        }

        pointer operator->() const { return &(operator*()); }

        // 내부적으로 JsonKey 객체 생성
        JsonKey key_wrapper() const {
            if (type == JsonType::ARRAY) {
                size_t idx = std::distance(arrStart, arrIt);
                return JsonKey(std::to_string(idx), idx, true);
            }
            return JsonKey(objIt->first, 0, false);
        }

        // 기존 문자열 전용 key() (하위 호환성 및 명시적 호출용)
        JsonKey key() const { return key_wrapper(); }

        reference value() const { return operator*(); }

        iterator& operator++() {
            if (type == JsonType::ARRAY) arrIt++;
            else                         objIt++;
            return *this;
        }

        iterator operator++(int) {
            iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        bool operator!=(const iterator& other) const {
            if (type == JsonType::ARRAY) return arrIt != other.arrIt;
            return objIt != other.objIt;
        }

        bool operator==(const iterator& other) const { return !(*this != other); }

    private:
        JsonType type;
        JsonArrays::iterator arrIt;
        JsonArrays::iterator arrStart;
        JsonObjects::iterator objIt;
    };

    // -------------------------------------------------------------------------
    // [Const Iterator]
    // -------------------------------------------------------------------------
    class const_iterator
    {
        friend class Json;
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type        = const Json;
        using difference_type   = std::ptrdiff_t;
        using pointer           = const Json*;
        using reference         = const Json&;

        const_iterator(JsonType t, JsonArrays::const_iterator ait, JsonArrays::const_iterator aStart, JsonObjects::const_iterator oit)
            : type(t), arrIt(ait), arrStart(aStart), objIt(oit) {}

        reference operator*() const {
            if (type == JsonType::ARRAY) return *arrIt;
            return objIt->second;
        }

        pointer operator->() const { return &(operator*()); }

        JsonKey key_wrapper() const {
            if (type == JsonType::ARRAY) {
                size_t idx = std::distance(arrStart, arrIt);
                return JsonKey(std::to_string(idx), idx, true);
            }
            return JsonKey(objIt->first, 0, false);
        }

        JsonKey key() const { return key_wrapper(); }
        reference value() const { return operator*(); }

        const_iterator& operator++() {
            if (type == JsonType::ARRAY) arrIt++;
            else                         objIt++;
            return *this;
        }

        const_iterator operator++(int) {
            const_iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        bool operator!=(const const_iterator& other) const {
            if (type == JsonType::ARRAY) return arrIt != other.arrIt;
            return objIt != other.objIt;
        }

        bool operator==(const const_iterator& other) const { return !(*this != other); }

    private:
        JsonType type;
        JsonArrays::const_iterator arrIt;
        JsonArrays::const_iterator arrStart;
        JsonObjects::const_iterator objIt;
    };

    // -------------------------------------------------------------------------
    // [Items Range Helper]
    // -------------------------------------------------------------------------
    template<typename IteratorType>
    class ItemsRange {
    public:
        class items_iterator : public IteratorType {
        public:
            using base = IteratorType;
            items_iterator(const base& it) : base(it) {}

            // 여기서 ItemProxy를 반환합니다.
            ItemProxy<IteratorType> operator*() const {
                return ItemProxy<IteratorType>(*this);
            }

            items_iterator& operator++() { base::operator++(); return *this; }
            bool operator!=(const items_iterator& other) const { return base::operator!=(other); }
        };

        ItemsRange(IteratorType begin, IteratorType end) : m_begin(begin), m_end(end) {}
        items_iterator begin() const { return items_iterator(m_begin); }
        items_iterator end() const   { return items_iterator(m_end); }

    private:
        IteratorType m_begin;
        IteratorType m_end;
    };

    iterator begin() {
        return iterator(this->jType, this->arr.begin(), this->arr.begin(), this->properties.begin());
    }
    iterator end() {
        return iterator(this->jType, this->arr.end(), this->arr.begin(), this->properties.end());
    }
    const_iterator begin() const {
        return const_iterator(this->jType, this->arr.begin(), this->arr.begin(), this->properties.begin());
    }
    const_iterator end() const {
        return const_iterator(this->jType, this->arr.end(), this->arr.begin(), this->properties.end());
    }

    ItemsRange<iterator> items() {
        return ItemsRange<iterator>(begin(), end());
    }
    ItemsRange<const_iterator> items() const {
        return ItemsRange<const_iterator>(begin(), end());
    }

    /**
     * @brief Json 객체를 문자열로 직렬화(Serialization)합니다.
     * @param type 직렬화 방식 (ToStringType::Strip: 공백 제거, ToStringType::Pretty: 들여쓰기 포함)
     * @return JSON 형식의 문자열
     */
    std::string toString( ToStringType type = ToStringType::Strip ) const noexcept;

    /**
     * @brief Json 객체를 파일로 저장합니다. (Pretty Print 적용됨)
     * @param fileName 저장할 파일 경로
     * @return 저장 성공 시 true, 실패 시 false
     */
    bool saveFile( const char* fileName ) const;

    // Chain Method 지원
    /**
     * @brief 객체에 키-값을 추가하거나, 배열에 새로운 객체를 추가합니다. (Context-Aware)
     * * @details
     * - **Parent가 Object일 때**: 해당 키(key)와 값(value)을 속성으로 추가합니다.
     * - **Parent가 Array일 때**: `{ key: value }` 형태의 새로운 객체를 생성하여 배열의 원소로 추가합니다.
     * * @tparam T 추가할 값의 타입
     * @param key 추가할 키 이름
     * @param value 추가할 값
     * @return 자기 자신의 참조(*this)를 반환하여 메서드 체이닝 지원
     * * @code
     * // Case 1: Object에 속성 추가
     * jsObj.addObject("name", "Kim"); // { "name": "Kim" }
     * * // Case 2: Array에 객체 추가
     * jsArr.addObject("id", 1);       // [ { "id": 1 } ]
     * @endcode
     */
    template <typename T>
    Json& addObject( const std::string& key, const T& value )
    {
        // 1. 부모가 이미 배열(ARRAY)인 경우 -> { key: value } 객체를 만들어 배열에 넣음
        if ( this->jType == JsonType::ARRAY ) {
            Json tempObj = Json( JsonType::OBJECT );
            tempObj.addProperty( key, Json(value) );
            this->addElement( tempObj );
        }
        // 2. 그 외(UNKNOWN, OBJECT) -> 내 자신을 Object로 만들고 속성 추가
        else {
            this->setType( JsonType::OBJECT );
            this->addProperty( key, Json( value ) );
        }
        return (*this);
    }

    /**
     * @brief 완성된 Json 객체를 추가합니다. (Context-Aware: Push or Merge)
     * * @details
     * - **Parent가 Array일 때**: 인자로 전달된 객체를 배열의 원소로 추가합니다. (Push)
     * - **Parent가 Object일 때**: 인자로 전달된 객체의 모든 속성(Key-Value)을 현재 객체에 병합합니다. (Merge)
     * * @param objectValue 추가할 Json 객체
     * @return 자기 자신의 참조(*this)
     * @throws TinyJsonException Parent가 Object인데 인자가 Object가 아닐 경우 (Key가 없으므로)
     */
    Json& addObject( const Json& objectValue );

    /**
     * @brief 배열(Array)에 하나 이상의 원소를 순차적으로 추가합니다. (가변 인자 지원)
     * * @details
     * 입력된 순서대로 배열의 끝에 요소들이 추가됩니다.
     * * @tparam T 첫 번째 요소의 타입
     * @tparam Types 나머지 요소들의 타입
     * @param element 추가할 첫 번째 요소
     * @param elements 추가할 나머지 요소들
     * @return 자기 자신의 참조(*this)
     * * @code
     * // 정수, 실수, 문자열을 한 번에 추가
     * arr.addElementToArray( 1, 3.14, "text" ); // [ 1, 3.14, "text" ]
     * @endcode
     */
    template <typename T, typename... Types>
    Json& addElementToArray( const T& element, Types... elements )
    {
        this->addElement( Json(element) );
        this->addElementToArray( elements... );
        return (*this);
    }

    template <typename T>
    Json& addElementToArray( const T& element )
    {
        this->addElement( Json( element ) );
        return (*this);
    }

private:
    std::string      strValue; // 실제 값
    JsonType         jType;    // 데이터 타입

    JsonObjects properties; // Object용 저장소
    JsonArrays  arr;        // Array용 저장소

    std::unordered_map<std::string, std::size_t> mapIndex; // Key 검색용 인덱스

    // pretty print 헬퍼
    std::string makeSpace( const unsigned int space ) const noexcept;
    std::string toStringPretty( const unsigned int space ) const noexcept;
    std::string toStringStrip() const noexcept;

    // 내부 직렬화/역직렬화 헬퍼
    static std::string deserialize( const std::string& src ) noexcept;
    static std::string serialize  ( const std::string& src ) noexcept;

    // 데이터 조작 함수
    Json& setType( const JsonType type );
    void  setString( const std::string& s ) noexcept;

    // 내부 구현: Low-level 데이터 삽입
    void addProperty( const std::string& key, const Json& value );
    void addElement ( const Json& value );
};

// ======================================================================================
// [템플릿 특수화 선언]
// ======================================================================================
template <> int         Json::getAs<int>()         const;
template <> double      Json::getAs<double>()      const;
template <> std::string Json::getAs<std::string>() const;
template <> bool        Json::getAs<bool>()        const;

// ======================================================================================
// [전역 템플릿 함수 정의]
// ======================================================================================
template <typename T>
Json JsonObject( const std::string& key, const T& value )
{
    return JsonObject().addObject( key, value );
}

inline Json JsonObject( const std::string& key, const char* value )
{
    // 명시적으로 string으로 변환하여 addObject 호출
    return JsonObject().addObject( key, std::string(value) );
}

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

} // namespace TinyJson

#endif