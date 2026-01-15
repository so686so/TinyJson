/**
 * TinyJson Unit Tests using Catch2
 * ------------------------------------------
 * Framework : Catch2 v3.5.0
 * Target    : TinyJson::Json class
 * Focus     : Context-Aware API, Strict Parsing, Type Deduction
 * ------------------------------------------
 */

// Catch Lib
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp> // float 비교용

// STL
#include <cstdio>    // for remove()
#include <stdlib.h>  // for malloc()
#include <memory>    // for make_shared, make_unique
#include <algorithm> // find_if

// TinyJson
#include "TinyJson.h"

using namespace TinyJson;

// ======================================================================================
// [Test 1] 기본 타입 파싱 및 getAs<T> 검증
// ======================================================================================
TEST_CASE( "Primitive Types Parsing & getAs Specialization", "[primitive]" )
{
    // Parser::parse() 함수는 인자로 문자열 및 문자열 관련 변수만 받기 때문에
    // 문자열로 이루어진 문장을 타입에 맞게 추론하여 변환한다.

    SECTION( "Boolean Type Parsing (True)" )
    {
        // ------------------------------------------------------------------------------
        // Json create with std::string {}
        Json js_with_string_construct = Parser::parse( std::string{ "true" } );

        // 정석적인 방식의 파싱
        REQUIRE( js_with_string_construct.isBool() == true );
        REQUIRE( js_with_string_construct.getAs<bool>() == true );

        // 모든 JSON 원소 객체는 문자열 타입이 아니어도
        // 강제 변환이 가능하긴 하다.
        REQUIRE( js_with_string_construct.isString() == false );
        REQUIRE( js_with_string_construct.getAs<std::string>() == "true" );

        // Json create with std::string (2)
        std::string js_string = "true";
        Json js_with_string_construct_2 = Parser::parse( js_string );

        REQUIRE( js_with_string_construct_2.isBool() == true );
        REQUIRE( js_with_string_construct_2.getAs<bool>() == true );

        REQUIRE( js_with_string_construct_2.isString() == false );
        REQUIRE( js_with_string_construct_2.getAs<std::string>() == "true" );

        // ------------------------------------------------------------------------------
        // Json create with const char*
        const char* char_ptr = "true";
        Json js_with_const_char_ptr = Parser::parse( char_ptr );

        REQUIRE( js_with_const_char_ptr.isBool() == true );
        REQUIRE( js_with_const_char_ptr.getAs<bool>() == true );

        REQUIRE( js_with_const_char_ptr.isString() == false );
        REQUIRE( js_with_const_char_ptr.getAs<std::string>() == "true" );

        // Json create with const char* (2)
        Json js_with_const_char_ptr_2 = Parser::parse( "true" );

        REQUIRE( js_with_const_char_ptr_2.isBool() == true );
        REQUIRE( js_with_const_char_ptr_2.getAs<bool>() == true );

        REQUIRE( js_with_const_char_ptr_2.isString() == false );
        REQUIRE( js_with_const_char_ptr_2.getAs<std::string>() == "true" );

        // ------------------------------------------------------------------------------
        // Json create with char[]
        char char_array[] = "true";
        Json js_with_char_array = Parser::parse( char_array );

        REQUIRE( js_with_char_array.isBool() == true );
        REQUIRE( js_with_char_array.getAs<bool>() == true );

        REQUIRE( js_with_char_array.isString() == false );
        REQUIRE( js_with_char_array.getAs<std::string>() == "true" );

        // ------------------------------------------------------------------------------
        // Json create with char* malloc
        const size_t string_size = 4 + 1;
        char* string_ptr = (char*)malloc(string_size * sizeof(char));
        snprintf( string_ptr, string_size, "true" );

        Json js_with_malloc_string = Parser::parse( string_ptr );

        REQUIRE( js_with_malloc_string.isBool() == true );
        REQUIRE( js_with_malloc_string.getAs<bool>() == true );

        REQUIRE( js_with_malloc_string.isString() == false );
        REQUIRE( js_with_malloc_string.getAs<std::string>() == "true" );

        free( string_ptr );

        std::string* string_ptr_2 = new std::string{ "true" };

        Json js_with_malloc_string_2 = Parser::parse( *string_ptr_2 );

        REQUIRE( js_with_malloc_string_2.isBool() == true );
        REQUIRE( js_with_malloc_string_2.getAs<bool>() == true );

        REQUIRE( js_with_malloc_string_2.isString() == false );
        REQUIRE( js_with_malloc_string_2.getAs<std::string>() == "true" );

        delete string_ptr_2;

        // ------------------------------------------------------------------------------
        // Json create with shared_ptr
        std::shared_ptr<std::string> sptr_string = std::make_shared<std::string>( "true" );

        Json js_with_sptr = Parser::parse( *sptr_string );

        REQUIRE( js_with_sptr.isBool() == true );
        REQUIRE( js_with_sptr.getAs<bool>() == true );

        REQUIRE( js_with_sptr.isString() == false );
        REQUIRE( js_with_sptr.getAs<std::string>() == "true" );

        // ------------------------------------------------------------------------------
        // Json create with unique_ptr
        std::unique_ptr<std::string> uptr_string { new std::string{ "true" } };

        Json js_with_uptr = Parser::parse( *uptr_string );

        REQUIRE( js_with_uptr.isBool() == true );
        REQUIRE( js_with_uptr.getAs<bool>() == true );

        REQUIRE( js_with_uptr.isString() == false );
        REQUIRE( js_with_uptr.getAs<std::string>() == "true" );
    }

    SECTION( "Boolean Type Parsing (False)" )
    {
        // ------------------------------------------------------------------------------
        // Json create with std::string {}
        Json js_with_string_construct = Parser::parse( std::string{ "false" } );

        // 정석적인 방식의 파싱
        REQUIRE( js_with_string_construct.isBool() == true );
        REQUIRE( js_with_string_construct.getAs<bool>() == false );

        // 모든 JSON 원소 객체는 문자열 타입이 아니어도
        // 강제 변환이 가능하긴 하다.
        REQUIRE( js_with_string_construct.isString() == false );
        REQUIRE( js_with_string_construct.getAs<std::string>() == "false" );

        // Json create with std::string (2)
        std::string js_string = "false";
        Json js_with_string_construct_2 = Parser::parse( js_string );

        REQUIRE( js_with_string_construct_2.isBool() == true );
        REQUIRE( js_with_string_construct_2.getAs<bool>() == false );

        REQUIRE( js_with_string_construct_2.isString() == false );
        REQUIRE( js_with_string_construct_2.getAs<std::string>() == "false" );

        // ------------------------------------------------------------------------------
        // Json create with const char*
        const char* char_ptr = "false";
        Json js_with_const_char_ptr = Parser::parse( char_ptr );

        REQUIRE( js_with_const_char_ptr.isBool() == true );
        REQUIRE( js_with_const_char_ptr.getAs<bool>() == false );

        REQUIRE( js_with_const_char_ptr.isString() == false );
        REQUIRE( js_with_const_char_ptr.getAs<std::string>() == "false" );

        // Json create with const char* (2)
        Json js_with_const_char_ptr_2 = Parser::parse( "false" );

        REQUIRE( js_with_const_char_ptr_2.isBool() == true );
        REQUIRE( js_with_const_char_ptr_2.getAs<bool>() == false );

        REQUIRE( js_with_const_char_ptr_2.isString() == false );
        REQUIRE( js_with_const_char_ptr_2.getAs<std::string>() == "false" );

        // ------------------------------------------------------------------------------
        // Json create with char[]
        char char_array[] = "false";
        Json js_with_char_array = Parser::parse( char_array );

        REQUIRE( js_with_char_array.isBool() == true );
        REQUIRE( js_with_char_array.getAs<bool>() == false );

        REQUIRE( js_with_char_array.isString() == false );
        REQUIRE( js_with_char_array.getAs<std::string>() == "false" );

        // ------------------------------------------------------------------------------
        // Json create with char* malloc
        const size_t string_size = 5 + 1;
        char* string_ptr = (char*)malloc(string_size * sizeof(char));
        snprintf( string_ptr, string_size, "false" );

        Json js_with_malloc_string = Parser::parse( string_ptr );

        REQUIRE( js_with_malloc_string.isBool() == true );
        REQUIRE( js_with_malloc_string.getAs<bool>() == false );

        REQUIRE( js_with_malloc_string.isString() == false );
        REQUIRE( js_with_malloc_string.getAs<std::string>() == "false" );

        free( string_ptr );

        std::string* string_ptr_2 = new std::string{ "false" };

        Json js_with_malloc_string_2 = Parser::parse( *string_ptr_2 );

        REQUIRE( js_with_malloc_string_2.isBool() == true );
        REQUIRE( js_with_malloc_string_2.getAs<bool>() == false );

        REQUIRE( js_with_malloc_string_2.isString() == false );
        REQUIRE( js_with_malloc_string_2.getAs<std::string>() == "false" );

        delete string_ptr_2;

        // ------------------------------------------------------------------------------
        // Json create with shared_ptr
        std::shared_ptr<std::string> sptr_string = std::make_shared<std::string>( "false" );

        Json js_with_sptr = Parser::parse( *sptr_string );

        REQUIRE( js_with_sptr.isBool() == true );
        REQUIRE( js_with_sptr.getAs<bool>() == false );

        REQUIRE( js_with_sptr.isString() == false );
        REQUIRE( js_with_sptr.getAs<std::string>() == "false" );

        // ------------------------------------------------------------------------------
        // Json create with unique_ptr
        std::unique_ptr<std::string> uptr_string { new std::string{ "false" } };

        Json js_with_uptr = Parser::parse( *uptr_string );

        REQUIRE( js_with_uptr.isBool() == true );
        REQUIRE( js_with_uptr.getAs<bool>() == false );

        REQUIRE( js_with_uptr.isString() == false );
        REQUIRE( js_with_uptr.getAs<std::string>() == "false" );
    }

    SECTION( "Integer Type Parsing" )
    {
        // ------------------------------------------------------------------------------
        // Json create with std::string {}
        Json js_with_string_construct = Parser::parse( std::string{ "12345" } );

        // 정석적인 방식의 파싱
        REQUIRE( js_with_string_construct.isInt() == true );
        REQUIRE( js_with_string_construct.getAs<int>() == 12345 );

        REQUIRE( js_with_string_construct.isString() == false );
        REQUIRE( js_with_string_construct.getAs<std::string>() == "12345" );

        REQUIRE( js_with_string_construct.isDouble() == false );
        REQUIRE_THAT( js_with_string_construct.getAs<double>(), Catch::Matchers::WithinRel( 12345.0, 0.0001 ) );

        // Json create with std::string (2)
        std::string js_string = "12345";
        Json js_with_string_construct_2 = Parser::parse( js_string );

        REQUIRE( js_with_string_construct_2.isInt() == true );
        REQUIRE( js_with_string_construct_2.getAs<int>() == 12345 );

        REQUIRE( js_with_string_construct_2.isString() == false );
        REQUIRE( js_with_string_construct_2.getAs<std::string>() == "12345" );

        REQUIRE( js_with_string_construct_2.isDouble() == false );
        REQUIRE_THAT( js_with_string_construct_2.getAs<double>(), Catch::Matchers::WithinRel( 12345.0, 0.0001 ) );

        // Json create with std::string (3)
        std::string js_string_with_to_string = std::to_string( 12345 );
        Json js_with_string_construct_3 = Parser::parse( js_string_with_to_string );

        REQUIRE( js_with_string_construct_3.isInt() == true );
        REQUIRE( js_with_string_construct_3.getAs<int>() == 12345 );

        REQUIRE( js_with_string_construct_3.isString() == false );
        REQUIRE( js_with_string_construct_3.getAs<std::string>() == "12345" );

        REQUIRE( js_with_string_construct_3.isDouble() == false );
        REQUIRE_THAT( js_with_string_construct_3.getAs<double>(), Catch::Matchers::WithinRel( 12345.0, 0.0001 ) );

        // ------------------------------------------------------------------------------
        // Json create with const char*
        const char* char_ptr = "12345";
        Json js_with_const_char_ptr = Parser::parse( char_ptr );

        REQUIRE( js_with_const_char_ptr.isInt() == true );
        REQUIRE( js_with_const_char_ptr.getAs<int>() == 12345 );

        REQUIRE( js_with_const_char_ptr.isString() == false );
        REQUIRE( js_with_const_char_ptr.getAs<std::string>() == "12345" );

        REQUIRE( js_with_const_char_ptr.isDouble() == false );
        REQUIRE_THAT( js_with_const_char_ptr.getAs<double>(), Catch::Matchers::WithinRel( 12345.0, 0.0001 ) );

        // Json create with const char* (2)
        Json js_with_const_char_ptr_2 = Parser::parse( "12345" );

        REQUIRE( js_with_const_char_ptr_2.isInt() == true );
        REQUIRE( js_with_const_char_ptr_2.getAs<int>() == 12345 );

        REQUIRE( js_with_const_char_ptr_2.isString() == false );
        REQUIRE( js_with_const_char_ptr_2.getAs<std::string>() == "12345" );

        REQUIRE( js_with_const_char_ptr_2.isDouble() == false );
        REQUIRE_THAT( js_with_const_char_ptr_2.getAs<double>(), Catch::Matchers::WithinRel( 12345.0, 0.0001 ) );

        // ------------------------------------------------------------------------------
        // Json create with char[]
        char char_array[] = "12345";
        Json js_with_char_array = Parser::parse( char_array );

        REQUIRE( js_with_char_array.isInt() == true );
        REQUIRE( js_with_char_array.getAs<int>() == 12345 );

        REQUIRE( js_with_char_array.isString() == false );
        REQUIRE( js_with_char_array.getAs<std::string>() == "12345" );

        REQUIRE( js_with_char_array.isDouble() == false );
        REQUIRE_THAT( js_with_char_array.getAs<double>(), Catch::Matchers::WithinRel( 12345.0, 0.0001 ) );

        // ------------------------------------------------------------------------------
        // Json create with char* malloc
        const size_t string_size = 5 + 1;
        char* string_ptr = (char*)malloc(string_size * sizeof(char));
        snprintf( string_ptr, string_size, "%d", 12345 );

        Json js_with_malloc_string = Parser::parse( string_ptr );

        REQUIRE( js_with_malloc_string.isInt() == true );
        REQUIRE( js_with_malloc_string.getAs<int>() == 12345 );

        REQUIRE( js_with_malloc_string.isString() == false );
        REQUIRE( js_with_malloc_string.getAs<std::string>() == "12345" );

        REQUIRE( js_with_malloc_string.isDouble() == false );
        REQUIRE_THAT( js_with_malloc_string.getAs<double>(), Catch::Matchers::WithinRel( 12345.0, 0.0001 ) );

        free( string_ptr );

        std::string* string_ptr_2 = new std::string{ "12345" };

        Json js_with_malloc_string_2 = Parser::parse( *string_ptr_2 );

        REQUIRE( js_with_malloc_string_2.isInt() == true );
        REQUIRE( js_with_malloc_string_2.getAs<int>() == 12345 );

        REQUIRE( js_with_malloc_string_2.isString() == false );
        REQUIRE( js_with_malloc_string_2.getAs<std::string>() == "12345" );

        REQUIRE( js_with_malloc_string_2.isDouble() == false );
        REQUIRE_THAT( js_with_malloc_string_2.getAs<double>(), Catch::Matchers::WithinRel( 12345.0, 0.0001 ) );

        delete string_ptr_2;

        // ------------------------------------------------------------------------------
        // Json create with shared_ptr
        std::shared_ptr<std::string> sptr_string = std::make_shared<std::string>( "12345" );

        Json js_with_sptr = Parser::parse( *sptr_string );

        REQUIRE( js_with_sptr.isInt() == true );
        REQUIRE( js_with_sptr.getAs<int>() == 12345 );

        REQUIRE( js_with_sptr.isString() == false );
        REQUIRE( js_with_sptr.getAs<std::string>() == "12345" );

        REQUIRE( js_with_sptr.isDouble() == false );
        REQUIRE_THAT( js_with_sptr.getAs<double>(), Catch::Matchers::WithinRel( 12345.0, 0.0001 ) );

        // ------------------------------------------------------------------------------
        // Json create with unique_ptr
        std::unique_ptr<std::string> uptr_string { new std::string{ "12345" } };

        Json js_with_uptr = Parser::parse( *uptr_string );

        REQUIRE( js_with_uptr.isInt() == true );
        REQUIRE( js_with_uptr.getAs<int>() == 12345 );

        REQUIRE( js_with_uptr.isString() == false );
        REQUIRE( js_with_uptr.getAs<std::string>() == "12345" );

        REQUIRE( js_with_uptr.isDouble() == false );
        REQUIRE_THAT( js_with_uptr.getAs<double>(), Catch::Matchers::WithinRel( 12345.0, 0.0001 ) );
    }

    SECTION( "Double Type Parsing" )
    {
        // ------------------------------------------------------------------------------
        // Json create with std::string {}
        Json js_with_string_construct = Parser::parse( std::string{ "3.14159" } );

        // 정석적인 방식의 파싱
        REQUIRE( js_with_string_construct.isDouble() == true );
        REQUIRE_THAT( js_with_string_construct.getAs<double>(), Catch::Matchers::WithinRel( 3.14159, 0.0001 ) );

        // Json create with std::string (2)
        std::string js_string = "3.14159";
        Json js_with_string_construct_2 = Parser::parse( js_string );

        REQUIRE( js_with_string_construct_2.isDouble() == true );
        REQUIRE_THAT( js_with_string_construct_2.getAs<double>(), Catch::Matchers::WithinRel( 3.14159, 0.0001 ) );

        // Json create with std::string (3)
        std::string js_string_with_to_string = std::to_string( 3.14159 );
        Json js_with_string_construct_3 = Parser::parse( js_string_with_to_string );

        REQUIRE( js_with_string_construct_3.isDouble() == true );
        REQUIRE_THAT( js_with_string_construct_3.getAs<double>(), Catch::Matchers::WithinRel( 3.14159, 0.0001 ) );

        // ------------------------------------------------------------------------------
        // Json create with const char*
        const char* char_ptr = "3.14159";
        Json js_with_const_char_ptr = Parser::parse( char_ptr );

        REQUIRE( js_with_const_char_ptr.isDouble() == true );
        REQUIRE_THAT( js_with_const_char_ptr.getAs<double>(), Catch::Matchers::WithinRel( 3.14159, 0.0001 ) );

        // Json create with const char* (2)
        Json js_with_const_char_ptr_2 = Parser::parse( "3.14159" );

        REQUIRE( js_with_const_char_ptr_2.isDouble() == true );
        REQUIRE_THAT( js_with_const_char_ptr_2.getAs<double>(), Catch::Matchers::WithinRel( 3.14159, 0.0001 ) );

        // ------------------------------------------------------------------------------
        // Json create with char[]
        char char_array[] = "3.14159";
        Json js_with_char_array = Parser::parse( char_array );

        REQUIRE( js_with_char_array.isDouble() == true );
        REQUIRE_THAT( js_with_char_array.getAs<double>(), Catch::Matchers::WithinRel( 3.14159, 0.0001 ) );

        // ------------------------------------------------------------------------------
        // Json create with char* malloc
        const size_t string_size = 8 + 1;
        char* string_ptr = (char*)malloc(string_size * sizeof(char));
        snprintf( string_ptr, string_size, "%.5f", 3.14159 );

        Json js_with_malloc_string = Parser::parse( string_ptr );

        REQUIRE( js_with_malloc_string.isDouble() == true );
        REQUIRE_THAT( js_with_malloc_string.getAs<double>(), Catch::Matchers::WithinRel( 3.14159, 0.0001 ) );

        free( string_ptr );

        std::string* string_ptr_2 = new std::string{ "3.14159" };

        Json js_with_malloc_string_2 = Parser::parse( *string_ptr_2 );

        REQUIRE( js_with_malloc_string_2.isDouble() == true );
        REQUIRE_THAT( js_with_malloc_string_2.getAs<double>(), Catch::Matchers::WithinRel( 3.14159, 0.0001 ) );

        delete string_ptr_2;

        // ------------------------------------------------------------------------------
        // Json create with shared_ptr
        std::shared_ptr<std::string> sptr_string = std::make_shared<std::string>( "3.14159" );

        Json js_with_sptr = Parser::parse( *sptr_string );

        REQUIRE( js_with_sptr.isDouble() == true );
        REQUIRE_THAT( js_with_sptr.getAs<double>(), Catch::Matchers::WithinRel( 3.14159, 0.0001 ) );

        // ------------------------------------------------------------------------------
        // Json create with unique_ptr
        std::unique_ptr<std::string> uptr_string { new std::string{ "3.14159" } };

        Json js_with_uptr = Parser::parse( *uptr_string );

        REQUIRE( js_with_uptr.isDouble() == true );
        REQUIRE_THAT( js_with_uptr.getAs<double>(), Catch::Matchers::WithinRel( 3.14159, 0.0001 ) );
    }

    SECTION( "String Type Parsing with Escapes" )
    {
        std::string jsonStr = "\"Hello\\nWorld\"";
        Json js = Parser::parse( jsonStr );

        REQUIRE( js.isString() == true );
        REQUIRE( js.getAs<std::string>() == "Hello\nWorld" );
    }

    SECTION( "Null Type Parsing" )
    {
        std::string jsonStr = "null";
        Json js = Parser::parse( jsonStr );

        REQUIRE( js.isNull() == true );
    }
}

// ======================================================================================
// [Test 2] 객체(Object) 생성, 조작 및 병합(Merge) 테스트
// ======================================================================================
TEST_CASE( "Json Object Manipulation & Merging", "[object]" )
{
    Json js = JsonObject();

    SECTION( "Add Properties (Basic)" )
    {
        js.addObject( "name", "TinyJson" )
          .addObject( "version", 2.0 )
          .addObject( "string_true", "true" )
          .addObject( "boolean_true", true );

        REQUIRE( js.isObject() == true );
        REQUIRE( js.size() == 4 );

        REQUIRE( js["name"].isString() == true );
        REQUIRE( js["name"].getAs<std::string>() == "TinyJson" );

        REQUIRE( js["version"].isDouble() == true );
        REQUIRE_THAT( js["version"].getAs<double>(), Catch::Matchers::WithinRel( 2.0, 0.0001 ) );

        REQUIRE( js["string_true"].isString() == true );
        REQUIRE( js["string_true"].getAs<std::string>() == "true" );

        REQUIRE( js["boolean_true"].isBool() == true );
        REQUIRE( js["boolean_true"].getAs<bool>() == true );
    }

    SECTION( "Nested Objects" )
    {
        js["parent"] = JsonObject( "child", 99 );

        REQUIRE( js["parent"].isObject() == true );
        REQUIRE( js["parent"]["child"].getAs<int>() == 99 );

        js["deep"] = JsonObject( "out", JsonObject( "mid", JsonObject( "in", 88 ) ) );

        REQUIRE( js["deep"].isObject() == true );
        REQUIRE( js["deep"]["out"].isObject() == true );
        REQUIRE( js["deep"]["out"]["mid"].isObject() == true );
        REQUIRE( js["deep"]["out"]["mid"]["in"].isInt() == true );
        REQUIRE( js["deep"]["out"]["mid"]["in"].getAs<int>() == 88 );
    }

    // Object Merging Test
    SECTION( "Object Merging (addObject with Json Object)" )
    {
        Json user    = JsonObject( "name", "Alice" );
        Json details = JsonObject( "age", 30 ).addObject( "city", "Seoul" );

        // details의 내용을 user에 병합
        user.addObject( details );

        REQUIRE( user.size() == 3 ); // name, age, city
        REQUIRE( user["name"].getAs<std::string>() == "Alice" );
        REQUIRE( user["age"].getAs<int>() == 30 );
        REQUIRE( user["city"].getAs<std::string>() == "Seoul" );
    }

    // Exception on Invalid Add
    SECTION( "Exception: Adding Value without Key to Object" )
    {
        Json obj = JsonObject();
        // Object에 Key 없이 단순 정수(123)를 넣으려 하면 예외 발생
        REQUIRE_THROWS_AS( obj.addObject( Json(123) ), TinyJsonException );
    }
}

// ======================================================================================
// [Test 3] 배열(Array) 조작 및 스마트 addObject 테스트
// ======================================================================================
TEST_CASE( "Json Array Manipulation & Smart Add", "[array]" )
{
    Json jsArr = JsonArray();

    SECTION( "Add Elements (addElementToArray)" )
    {
        jsArr.addElementToArray( 1, 2, 3 );
        jsArr.addElementToArray( "four" );

        REQUIRE( jsArr.isArray() == true );
        REQUIRE( jsArr.size() == 4 );
        REQUIRE( jsArr[0].getAs<int>() == 1 );
    }

    // Array Context: addObject(Key, Value)
    SECTION( "Smart Add: addObject(Key, Value) on Array" )
    {
        Json arr = JsonArray();

        // 배열에 "id": 1을 추가 -> [ { "id": 1 } ] 로 변환되어야 함
        arr.addObject( "id", 1 );

        REQUIRE( arr.size() == 1 );
        REQUIRE( arr[0].isObject() ); // 원소가 Object여야 함
        REQUIRE( arr[0]["id"].getAs<int>() == 1 );

        arr.addObject( "Ar", JsonArray(1,2,3) );
        REQUIRE( arr.size() == 2 );
        REQUIRE( arr[1].isObject() );
        REQUIRE( arr[1]["Ar"].isArray() );
        REQUIRE( arr[1]["Ar"].size() == 3 );
    }

    // Array Context: addObject(Array)
    SECTION( "Smart Add: addObject(Array) on Array" )
    {
        Json arr_root = JsonArray();
        Json arr_in_1 = JsonArray();
        Json arr_in_2 = JsonArray( 1.23, true, "string" );

        arr_in_1.addElementToArray( 1, 2, 3, 4, 5 );

        arr_root.addObject( arr_in_1 );
        arr_root.addObject( arr_in_2 );

        REQUIRE( arr_root.size() == 2 );

        REQUIRE( arr_root[0].isArray() );
        REQUIRE( arr_root[1].isArray() );

        REQUIRE( arr_root[0].size() == 5 );
        REQUIRE( arr_root[1].size() == 3 );

        Json& arr_in_root = arr_root[1];

        REQUIRE_THAT( arr_in_root[0].getAs<double>(), Catch::Matchers::WithinRel( 1.23, 0.0001 ) );
        REQUIRE( arr_in_root[1].getAs<bool>() == true );
        REQUIRE( arr_in_root[2].getAs<std::string>() == "string" );
    }

    // Array Context: addObject(Json Object)
    SECTION( "Smart Add: addObject(JsonObject) on Array" )
    {
        Json arr  = JsonArray();
        Json item = JsonObject( "name", "Item1" );

        // 정상적인 JSON 문자열인지 확인 (이스케이프 문자 포함)
        REQUIRE( item.toString() == "{ \"name\": \"Item1\" }" );

        // 배열에 완성된 객체 추가 -> [ { "name": "Item1" } ]
        arr.addObject( item );

        REQUIRE( arr.size() == 1 );
        REQUIRE( arr[0]["name"].getAs<std::string>() == "Item1" );
    }

    SECTION( "Mixed Chaining (addElementToArray + addObject)" )
    {
        Json arr = JsonArray();

        // 정수 추가 -> 객체 추가 -> 정수 추가
        arr.addElementToArray( 10 )
           .addObject( "key", "value" ) // 자동으로 {key:value} 객체 생성됨
           .addElementToArray( 20 );

        REQUIRE( arr.size() == 3 );
        REQUIRE( arr[0].getAs<int>() == 10 );
        REQUIRE( arr[1]["key"].getAs<std::string>() == "value" );
        REQUIRE( arr[2].getAs<int>() == 20 );
    }
}

// ======================================================================================
// [Test 4] 예외 처리 및 안정성 테스트
// ======================================================================================
TEST_CASE( "Exception Handling & Stability", "[exception]" )
{
    SECTION( "Invalid Json String" )
    {
        std::string invalidJson = "{ \"key\": 123 "; // 닫는 괄호 누락
        REQUIRE_THROWS_AS( Parser::parse( invalidJson ), TinyJsonException );
    }

    SECTION( "Out of Range Access (Array)" )
    {
        Json js = JsonArray( 1, 2 );
        REQUIRE_THROWS_AS( js[5], TinyJsonException );
    }

    SECTION( "Invalid Key Access (Const Object)" )
    {
        const Json js = JsonObject( "exist", 1 );
        REQUIRE_THROWS_AS( js["empty"], TinyJsonException );
    }

    SECTION( "Type Mismatch Access" )
    {
        Json js = JsonArray( 1, 2 );
        // 배열에 문자열 키로 접근 시도
        REQUIRE_THROWS_AS( js["key"], TinyJsonException );
    }

    SECTION( "Strict Parsing: Missing Comma" )
    {
        // 콤마 누락
        REQUIRE_THROWS_AS( Parser::parse( "{ \"a\": 1 \"b\": 2 }" ), TinyJsonException );
        // 콜론 누락
        REQUIRE_THROWS_AS( Parser::parse( "{ \"a\" 1 }" ), TinyJsonException );
    }
}

// ======================================================================================
// [Test 5] tryGetAs 안전한 값 추출 테스트
// ======================================================================================
TEST_CASE( "Safe Value Retrieval (tryGetAs)", "[utility]" )
{
    Json js = JsonObject();
    js["pi"] = 3.14;

    double dVal = 0.0;
    bool bVal = false;
    int iVal = 0;

    REQUIRE( js["pi"].tryGetAs<double>( dVal ) == true );
    REQUIRE_THAT( dVal, Catch::Matchers::WithinRel( 3.14, 0.001 ) );

    REQUIRE( js["pi"].tryGetAs<bool>( bVal ) == false );
    REQUIRE( js["pi"].tryGetAs<int>( iVal ) == false ); // Strict check
}

// ======================================================================================
// [Test 6] File I/O Integration
// ======================================================================================
TEST_CASE( "File I/O Integration", "[io]" )
{
    const char* filename = "test_temp.json";

    Json writeJs = JsonObject();
    writeJs["app"] = "TinyJson";
    writeJs["ver"] = 1.0;

    REQUIRE( writeJs.saveFile( filename ) == true );

    Json readJs = Parser::parseFile( filename );
    REQUIRE( readJs["app"].getAs<std::string>() == "TinyJson" );
    REQUIRE( readJs["ver"].getAs<double>() == 1.0 );

    std::remove( filename );
}

// ======================================================================================
// [Test 7] C++ Semantics (Copy & Move)
// ======================================================================================
TEST_CASE( "C++ Copy and Move Semantics", "[memory]" )
{
    SECTION( "Deep Copy Verification" )
    {
        Json original = JsonObject( "key", "original" );
        Json copy     = original;

        copy["key"] = "modified";

        REQUIRE( original["key"].getAs<std::string>() == "original" );
        REQUIRE( copy["key"].getAs<std::string>() == "modified" );
    }

    SECTION( "Move Semantics Verification" )
    {
        Json source = JsonArray( 1, 2, 3 );
        Json dest = std::move( source );

        REQUIRE( dest.isArray() );
        REQUIRE( dest.size() == 3 );
        REQUIRE( source.isNull() ); // Moved state
    }
}

// ======================================================================================
// [Test 8] Constructor Type Deduction (Strict Check)
// "Item1"이 1로 변환되거나, bool이 int로 변환되는 등의 문제를 방지하기 위한 테스트.
// ======================================================================================
TEST_CASE( "Constructor Type Deduction & Overload Resolution", "[deduction]" )
{
    SECTION( "String Literal vs Bool" )
    {
        Json j_str("Item1");
        // 이것이 실패하면 const char*가 bool로 변환.
        REQUIRE( j_str.isString() );
        REQUIRE_FALSE( j_str.isBool() );
        REQUIRE_FALSE( j_str.isInt() );
        REQUIRE( j_str.getAs<std::string>() == "Item1" );
    }

    SECTION( "Char vs Int" )
    {
        Json j_char('A');
        // 이것이 실패하면 char가 int로 변환.
        REQUIRE( j_char.isString() );
        REQUIRE_FALSE( j_char.isInt() );
        REQUIRE( j_char.getAs<std::string>() == "A" );
    }

    SECTION( "Bool vs Int" )
    {
        Json j_bool(true);
        // 이것이 실패하면 bool이 int로 변환.
        REQUIRE( j_bool.isBool() );
        REQUIRE_FALSE( j_bool.isInt() );
        REQUIRE( j_bool.getAs<bool>() == true );

        Json j_bool2(false);
        REQUIRE( j_bool2.isBool() );
        REQUIRE( j_bool2.getAs<bool>() == false );
    }

    SECTION( "Int vs Double" )
    {
        Json j_int(42);
        REQUIRE( j_int.isInt() );
        REQUIRE_FALSE( j_int.isDouble() );

        Json j_dbl(3.14);
        REQUIRE( j_dbl.isDouble() );
        REQUIRE_FALSE( j_dbl.isInt() );
    }

    SECTION( "Explicit String Construction" )
    {
        std::string s = "test";
        Json j_std_str(s);
        REQUIRE( j_std_str.isString() );
        REQUIRE( j_std_str.getAs<std::string>() == "test" );
    }
}

// ======================================================================================
// [Test 9] Smart Key Iteration (Int for Array, String for Object)
// ======================================================================================
TEST_CASE( "Smart Key Iteration", "[iterator]" )
{
    // 1. Array: .key() as Integer
    SECTION( "Array Key as Integer" )
    {
        Json arr = JsonArray();
        arr.addElementToArray( 10, 20, 30 );

        int idxSum = 0;

        for( auto item : arr.items() )
        {
            // [핵심] item.key()가 자동으로 size_t(int)로 변환됨!
            size_t idx = item.key();
            idxSum += idx;

            // 값 검증
            if( idx == 0 ) REQUIRE( item.value().getAs<int>() == 10 );
            if( idx == 1 ) REQUIRE( item.value().getAs<int>() == 20 );
        }
        // 0 + 1 + 2 = 3
        REQUIRE( idxSum == 3 );
    }

    // 2. Object: .key() as String
    SECTION( "Object Key as String" )
    {
        Json obj = JsonObject();
        obj.addObject("A", 1).addObject("B", 2);

        std::string keyConcat = "";

        for( auto item : obj.items() )
        {
            // [핵심] item.key()가 자동으로 string으로 변환됨!
            std::string k = item.key();
            keyConcat += k;
        }

        REQUIRE( keyConcat == "AB" );
    }

    // 3. Mixed Usage (cout support)
    SECTION( "Key printing directly" )
    {
        Json arr = JsonArray();
        arr.addElementToArray(99);

        // JsonKey 객체는 ostream << 연산자가 오버로딩되어 있어 바로 출력 가능
        for( auto item : arr.items() )
        {
            std::stringstream ss;
            ss << item.key(); // "0" (숫자지만 텍스트로 출력됨)
            REQUIRE( ss.str() == "0" );
        }
    }
}

// ======================================================================================
// [Test 10] Raw Iterator Behavior & STL Compatibility
// items() 헬퍼 함수 없이, 순수 iterator의 동작과 표준 준수 여부를 테스트합니다.
// ======================================================================================
TEST_CASE( "Raw Iterator Behavior", "[iterator][core]" )
{
    // 1. Array Iterator 수동 조작 검증
    SECTION( "Array Iterator Manual Manipulation" )
    {
        Json arr = JsonArray();
        arr.addElementToArray( 10, 20, 30 );

        auto it = arr.begin();
        auto end = arr.end();

        // [Dereference] *it -> Json&
        REQUIRE( (*it).isInt() );
        REQUIRE( (*it).getAs<int>() == 10 );

        // [Arrow Operator] it-> -> Json*
        // Json 객체의 멤버 함수(getAs)를 화살표 연산자로 호출 가능해야 함
        REQUIRE( it->getAs<int>() == 10 );

        // [Increment] ++it
        ++it;
        REQUIRE( it->getAs<int>() == 20 );

        // [Post-Increment] it++
        auto oldIt = it++;
        REQUIRE( oldIt->getAs<int>() == 20 );
        REQUIRE( it->getAs<int>() == 30 );

        // [Equality]
        ++it;
        REQUIRE( it == end );
    }

    // 2. Object Iterator 수동 조작 검증
    SECTION( "Object Iterator Manual Manipulation" )
    {
        Json obj = JsonObject();
        obj.addObject("A", 100).addObject("B", 200);

        auto it = obj.begin();

        // 1. 첫 번째 요소 확인 (순서는 삽입 순서 보장)
        // iterator 자체의 .key() 메서드 확인
        REQUIRE( it.key() == "A" );
        REQUIRE( it.value().getAs<int>() == 100 );
        REQUIRE( (*it).getAs<int>() == 100 ); // *it는 Value를 반환

        // 2. 이동
        ++it;
        REQUIRE( it.key() == "B" );
        REQUIRE( it->getAs<int>() == 200 );

        // 3. 종료 확인
        ++it;
        REQUIRE( it == obj.end() );
    }

    // 3. 값 수정 검증 (Modifiable Reference)
    SECTION( "Modification via Iterator" )
    {
        Json arr = JsonArray();
        arr.addElementToArray( 1, 2, 3 );

        // Range-based for loop (reference)
        for( auto& val : arr )
        {
            // 값을 10배로 수정
            val = val.getAs<int>() * 10;
        }

        REQUIRE( arr[0].getAs<int>() == 10 );
        REQUIRE( arr[1].getAs<int>() == 20 );
        REQUIRE( arr[2].getAs<int>() == 30 );
    }

    // 4. STL 알고리즘 호환성 테스트
    // TinyJson의 iterator가 표준 iterator spec을 잘 따르는지 확인
    SECTION( "STL Algorithm Compatibility (std::find_if)" )
    {
        Json arr = JsonArray();
        arr.addElementToArray( 10, 20, 999, 40 );

        // std::find_if를 사용하여 999 찾기
        auto found = std::find_if( arr.begin(), arr.end(), [](const Json& j) {
            return j.isInt() && j.getAs<int>() == 999;
        });

        REQUIRE( found != arr.end() );
        REQUIRE( found->getAs<int>() == 999 );

        // 인덱스 확인 (distance 사용)
        // iterator 내부에서 arrStart를 관리하므로 distance 계산 가능해야 함
        // (단, std::forward_iterator_tag 이므로 distance는 O(N)일 수 있음)
        REQUIRE( std::distance(arr.begin(), found) == 2 );
    }

    // 5. Const Iterator 검증
    SECTION( "Const Iterator (Read-Only)" )
    {
        const Json obj = JsonObject("x", 1).addObject("y", 2);

        // const 객체의 begin()은 const_iterator를 반환해야 함
        auto it = obj.begin();

        REQUIRE( it.key() == "x" );
        REQUIRE( it->getAs<int>() == 1 );

        // it->value() = 5; // 컴파일 에러가 나야 정상 (Read-Only)

        ++it;
        REQUIRE( it.key() == "y" );
    }

    // 6. Empty Container Iterator
    SECTION( "Empty Container Safety" )
    {
        Json emptyArr = JsonArray();
        REQUIRE( emptyArr.begin() == emptyArr.end() );

        Json emptyObj = JsonObject();
        REQUIRE( emptyObj.begin() == emptyObj.end() );

        // 루프 진입 안 하는지 확인
        int count = 0;
        for( auto& val : emptyArr ) { count++; }
        for( auto& val : emptyObj ) { count++; }
        REQUIRE( count == 0 );
    }

    // 7. Iterator Key Access Consistency
    // iterator.key()가 Array일 때 인덱스, Object일 때 키를 잘 반환하는지
    SECTION( "Iterator Key Access Consistency" )
    {
        // Array
        Json arr = JsonArray(100, 200);
        auto arrIt = arr.begin();
        REQUIRE( (size_t)arrIt.key() == 0 ); // "0" -> 0 (JsonKey 변환)
        ++arrIt;
        REQUIRE( (std::string)arrIt.key() == "1" ); // "1"

        // Object
        Json obj = JsonObject("first", 1);
        auto objIt = obj.begin();
        REQUIRE( (std::string)objIt.key() == "first" );
        // REQUIRE( (int)objIt.key() ); // 문자열은 숫자로 변환 시 0이 되거나 예외 (구현 의존적이나 여기선 stoi 아님)
        // 현재 JsonKey 구현 상 문자열 키를 int로 변환 시도하면 0이 됨 (indexVal 초기화값)
        // 하지만 논리적으로 Object 키를 int로 쓰는건 말이 안되므로 테스트 생략 또는 0 확인
    }
}

// ======================================================================================
// [Test 11] Iteration Behavior Comparison (Standard vs .items())
// 1. Standard Loop: 오직 값(Value)만 순회하며, 참조를 통해 수정 가능함.
// 2. .items() Loop: Key(Index)와 Value 쌍을 순회하며, Value를 수정 가능함.
// ======================================================================================
TEST_CASE( "Iteration Behavior Comparison", "[iterator][final]" )
{
    // ----------------------------------------------------------------------------------
    // Case A: Object Iteration
    // ----------------------------------------------------------------------------------
    SECTION( "Object: Standard vs items()" )
    {
        Json obj = JsonObject();
        obj.addObject("A", 10).addObject("B", 20);

        // [1] Standard Loop: for( auto& val : obj )
        // -> Key는 알 수 없고, Value(Json&)만 나옵니다.
        for( auto& val : obj )
        {
            // val은 Json Object가 아니라 내부의 int 값(Json&)입니다.
            REQUIRE( val.isInt() );

            // 값을 수정하면 원본이 바뀝니다.
            val = val.getAs<int>() + 5;
        }

        // 수정 확인 (10->15, 20->25)
        REQUIRE( obj["A"].getAs<int>() == 15 );
        REQUIRE( obj["B"].getAs<int>() == 25 );


        // [2] Items Loop: for( auto item : obj.items() )
        // -> Key와 Value 모두 접근 가능합니다.
        for( auto item : obj.items() )
        {
            // Key 접근 (std::string)
            std::string k = item.key();

            // Value 접근 (Json&) 및 수정
            if( k == "A" ) {
                REQUIRE( item.value().getAs<int>() == 15 );
                item.value() = 100; // 수정
            }
            else if( k == "B" ) {
                REQUIRE( item.value().getAs<int>() == 25 );
                item.value() = 200; // 수정
            }
        }

        // items()를 통한 수정 확인
        REQUIRE( obj["A"].getAs<int>() == 100 );
        REQUIRE( obj["B"].getAs<int>() == 200 );
    }

    // ----------------------------------------------------------------------------------
    // Case B: Array Iteration
    // ----------------------------------------------------------------------------------
    SECTION( "Array: Standard vs items()" )
    {
        Json arr = JsonArray();
        arr.addElementToArray( 1, 2, 3 );

        // [1] Standard Loop: for( auto& val : arr )
        // -> 인덱스는 알 수 없고, 순서대로 Value(Json&)만 나옵니다.
        int sum = 0;
        for( auto& val : arr )
        {
            sum += val.getAs<int>();

            // 값 수정 (모든 원소를 0으로 초기화)
            val = 0;
        }
        REQUIRE( sum == 6 ); // 1+2+3
        REQUIRE( arr[0].getAs<int>() == 0 );
        REQUIRE( arr[2].getAs<int>() == 0 );


        // [2] Items Loop: for( auto item : arr.items() )
        // -> 인덱스(Key)와 Value 모두 접근 가능합니다.
        // -> 데이터 재설정 (0, 0, 0 -> 10, 20, 30)
        for( auto item : arr.items() )
        {
            // Array의 key()는 인덱스를 반환합니다. (자동 형변환: size_t)
            size_t idx = item.key();

            // 인덱스를 이용해 값 수정
            item.value() = (int)(idx + 1) * 10;
        }

        REQUIRE( arr[0].getAs<int>() == 10 );
        REQUIRE( arr[1].getAs<int>() == 20 );
        REQUIRE( arr[2].getAs<int>() == 30 );
    }

    // ----------------------------------------------------------------------------------
    // Case C: Key Type Check (String vs Int conversion)
    // ----------------------------------------------------------------------------------
    SECTION( "Key Type Deduction Check" )
    {
        Json arr = JsonArray(10);
        Json obj = JsonObject("k", 10);

        // Array의 키는 숫자로 변환 가능
        auto arrIt = arr.items().begin();
        REQUIRE( (int)(*arrIt).key() == 0 );
        REQUIRE( (std::string)(*arrIt).key() == "0" );

        // Object의 키는 문자열
        auto objIt = obj.items().begin();
        REQUIRE( (std::string)(*objIt).key() == "k" );
    }
}