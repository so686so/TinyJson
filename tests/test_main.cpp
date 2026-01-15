/**
 * TinyJson Unit Tests using Catch2
 * -----------------------------------------------------------------------------
 * Framework : Catch2 v3.5.0
 * Target    : TinyJson::Json class
 * Focus     : Context-Aware API, Strict Parsing, Type Deduction, Iterators
 * -----------------------------------------------------------------------------
 */

// Catch2 Library
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp> // for double comparison

// STL
#include <cstdio>    // for remove()
#include <string>
#include <vector>
#include <algorithm> // for std::find_if
#include <memory>

// TinyJson Header
#include "TinyJson.h"

using namespace TinyJson;

// =============================================================================
// [Test 1] Parser Input Compatibility & Primitive Types
// Verify that the Parser accepts various string types (std::string, char*,
// char[], smart pointers) and correctly parses primitive JSON types.
// =============================================================================
TEST_CASE( "Parser Input Compatibility", "[primitive][input]" )
{
    // 1. Boolean Parsing with various input types
    SECTION( "Input Type: std::string" )
    {
        // R-value
        Json js1 = Parser::parse( std::string( "true" ) );
        REQUIRE( js1.isBool() );
        REQUIRE( js1.getAs<bool>() == true );

        // L-value
        std::string s = "false";
        Json js2 = Parser::parse( s );
        REQUIRE( js2.isBool() );
        REQUIRE( js2.getAs<bool>() == false );
    }

    SECTION( "Input Type: const char*" )
    {
        const char* ptr = "true";
        Json js = Parser::parse( ptr );

        REQUIRE( js.isBool() );
        REQUIRE( js.getAs<bool>() == true );
    }

    SECTION( "Input Type: char array []" )
    {
        char arr[] = "false";
        Json js = Parser::parse( arr );

        REQUIRE( js.isBool() );
        REQUIRE( js.getAs<bool>() == false );
    }

    SECTION( "Input Type: Dynamic Allocation (malloc/new)" )
    {
        // 1. malloc (C-style)
        const size_t len = 5; // "true" + \0
        char* rawPtr = (char*)malloc( len * sizeof(char) );
        if( rawPtr ) {
            snprintf( rawPtr, len, "true" );

            Json js = Parser::parse( rawPtr );
            REQUIRE( js.getAs<bool>() == true );

            free( rawPtr );
        }

        // 2. new (C++ style string pointer)
        std::string* strPtr = new std::string( "false" );
        Json js2 = Parser::parse( *strPtr );

        REQUIRE( js2.getAs<bool>() == false );
        delete strPtr;
    }

    SECTION( "Input Type: Smart Pointers" )
    {
        // std::shared_ptr
        auto sptr = std::make_shared<std::string>( "true" );
        Json js1 = Parser::parse( *sptr );
        REQUIRE( js1.getAs<bool>() == true );

        // std::unique_ptr
        auto uptr = std::make_unique<std::string>( "false" );
        Json js2 = Parser::parse( *uptr );
        REQUIRE( js2.getAs<bool>() == false );
    }
}

TEST_CASE( "Primitive Value Parsing", "[primitive][value]" )
{
    SECTION( "Integer Parsing" )
    {
        Json js = Parser::parse( "12345" );
        REQUIRE( js.isInt() );
        REQUIRE( js.getAs<int>() == 12345 );
        REQUIRE( js.getAs<std::string>() == "12345" );
    }

    SECTION( "Double Parsing" )
    {
        Json js = Parser::parse( "3.14159" );
        REQUIRE( js.isDouble() );
        REQUIRE_THAT( js.getAs<double>(), Catch::Matchers::WithinRel( 3.14159, 0.0001 ) );
    }

    SECTION( "String Parsing (Escaped)" )
    {
        // Raw: "Line1\nLine2"
        Json js = Parser::parse( "\"Line1\\nLine2\"" );
        REQUIRE( js.isString() );
        REQUIRE( js.getAs<std::string>() == "Line1\nLine2" );
    }

    SECTION( "Null Parsing" )
    {
        Json js = Parser::parse( "null" );
        REQUIRE( js.isNull() );
    }
}

// =============================================================================
// [Test 2] Type Deduction & Constructor Overloading
// Verify that C++ types are correctly mapped to JSON types during construction.
// =============================================================================
TEST_CASE( "Constructor Type Deduction", "[deduction]" )
{
    SECTION( "String Literal vs Bool" )
    {
        // "Item1" is const char*, should NOT be converted to bool(true)
        Json j_str( "Item1" );

        REQUIRE( j_str.isString() );
        REQUIRE_FALSE( j_str.isBool() );
        REQUIRE( j_str.getAs<std::string>() == "Item1" );
    }

    SECTION( "Char vs Int" )
    {
        // 'A' is char, should NOT be converted to int(65)
        Json j_char( 'A' );

        REQUIRE( j_char.isString() );
        REQUIRE_FALSE( j_char.isInt() );
        REQUIRE( j_char.getAs<std::string>() == "A" );
    }

    SECTION( "Bool vs Int" )
    {
        Json j_bool( true );
        REQUIRE( j_bool.isBool() );
        REQUIRE_FALSE( j_bool.isInt() );
        REQUIRE( j_bool.getAs<bool>() == true );
    }

    SECTION( "Int vs Double" )
    {
        Json j_int( 42 );
        REQUIRE( j_int.isInt() );

        Json j_dbl( 3.14 );
        REQUIRE( j_dbl.isDouble() );
    }
}

// =============================================================================
// [Test 3] Object Manipulation
// Verify object creation, property addition, and merging.
// =============================================================================
TEST_CASE( "Json Object Manipulation", "[object]" )
{
    Json js = JsonObject();

    SECTION( "Add Properties (Method Chaining)" )
    {
        js.addObject( "name", "TinyJson" )
          .addObject( "version", 2.0 )
          .addObject( "stable", true );

        REQUIRE( js.isObject() );
        REQUIRE( js.size() == 3 );
        REQUIRE( js["name"].getAs<std::string>() == "TinyJson" );
        REQUIRE( js["version"].getAs<double>() == 2.0 );
        REQUIRE( js["stable"].getAs<bool>() == true );

        js["direct_in_bool"]    = true;
        js["direct_in_int"]     = 1234;
        js["direct_in_double"]  = 5.67;
        js["direct_in_string"]  = "message";
        js["direct_in_object"]  = JsonObject("key", "val");
        js["direct_in_array"]   = JsonArray( 11, "2", false );

        REQUIRE( js.contains( "direct_in_bool"   ) );
        REQUIRE( js.contains( "direct_in_int"    ) );
        REQUIRE( js.contains( "direct_in_double" ) );
        REQUIRE( js.contains( "direct_in_string" ) );
        REQUIRE( js.contains( "direct_in_object" ) );
        REQUIRE( js.contains( "direct_in_array"  ) );

        REQUIRE( js["direct_in_bool"].isBool() );
        REQUIRE( js["direct_in_bool"].getAs<bool>() == true );

        REQUIRE( js["direct_in_int"].isInt() );
        REQUIRE( js["direct_in_int"].getAs<int>() == 1234 );

        REQUIRE( js["direct_in_double"].isDouble() );
        REQUIRE_THAT( js["direct_in_double"].getAs<double>(),
            Catch::Matchers::WithinRel( 5.67, 0.0001 ) );

        REQUIRE( js["direct_in_string"].isString() );
        REQUIRE( js["direct_in_string"].getAs<std::string>() == "message" );

        REQUIRE( js["direct_in_object"].isObject() );
        REQUIRE( js["direct_in_object"]["key"].getAs<std::string>() == "val" );

        REQUIRE( js["direct_in_array"].isArray() );
        REQUIRE( js["direct_in_array"][2].getAs<bool>() == false );
    }

    SECTION( "Nested Objects" )
    {
        js["parent"] = JsonObject( "child", 99 );

        REQUIRE( js["parent"].isObject() );
        REQUIRE( js["parent"]["child"].getAs<int>() == 99 );

        js["deep"] = JsonObject( "out", JsonObject( "mid", JsonObject( "in", 88 ) ) );

        REQUIRE( js["deep"].isObject() == true );
        REQUIRE( js["deep"]["out"].isObject() == true );
        REQUIRE( js["deep"]["out"]["mid"].isObject() == true );
        REQUIRE( js["deep"]["out"]["mid"]["in"].isInt() == true );
        REQUIRE( js["deep"]["out"]["mid"]["in"].getAs<int>() == 88 );
    }

    SECTION( "Object Merging" )
    {
        Json user    = JsonObject( "name", "Alice" );
        Json details = JsonObject( "age", 30 ).addObject( "city", "Seoul" );

        // Merge 'details' into 'user'
        user.addObject( details );

        REQUIRE( user.size() == 3 );
        REQUIRE( user["name"].getAs<std::string>() == "Alice" );
        REQUIRE( user["age"].getAs<int>() == 30 );
        REQUIRE( user["city"].getAs<std::string>() == "Seoul" );
    }

    SECTION( "Exception: Invalid Addition" )
    {
        Json obj = JsonObject();
        // Cannot add a value without a key to an Object
        REQUIRE_THROWS_AS( obj.addObject( Json( 123 ) ), TinyJsonException );
    }
}

// =============================================================================
// [Test 4] Array Manipulation
// Verify array creation, element addition, and mixed chaining.
// =============================================================================
TEST_CASE( "Json Array Manipulation", "[array]" )
{
    Json jsArr = JsonArray();

    SECTION( "Add Elements (Variadic Template)" )
    {
        jsArr.addElementToArray( 1, 2, 3, "four" );

        REQUIRE( jsArr.isArray() );
        REQUIRE( jsArr.size() == 4 );
        REQUIRE( jsArr[0].getAs<int>() == 1 );
        REQUIRE( jsArr[3].getAs<std::string>() == "four" );
    }

    SECTION( "Smart Add: Key-Value Pair in Array Context" )
    {
        Json arr = JsonArray();

        // Adding "id": 1 to an Array creates an Object { "id": 1 } inside the Array
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

    SECTION( "Mixed Chaining (Array & Object Contexts)" )
    {
        Json arr = JsonArray();

        arr.addElementToArray( 10 )                 // [ 10 ]
           .addObject( "key", "value" )             // [ 10, { "key": "value" } ]
           .addElementToArray( 20 );                // [ 10, { "key": "value" }, 20 ]

        REQUIRE( arr.size() == 3 );
        REQUIRE( arr[0].getAs<int>() == 10 );
        REQUIRE( arr[1]["key"].getAs<std::string>() == "value" );
        REQUIRE( arr[2].getAs<int>() == 20 );
    }
}

// =============================================================================
// [Test 5] Iteration (nlohmann-style)
// Verify Standard Loop (Values) and items() Loop (Key-Value).
// =============================================================================
TEST_CASE( "Iteration Behavior", "[iterator]" )
{
    // -------------------------------------------------------------------------
    // 1. Standard Range-based for loop (Values only)
    // -------------------------------------------------------------------------
    SECTION( "Standard Loop: Object (Values)" )
    {
        Json obj = JsonObject();
        obj.addObject( "A", 10 ).addObject( "B", 20 );

        int sum = 0;
        // Should iterate over values (Json&)
        for( auto& val : obj ) {
            REQUIRE( val.isInt() );
            sum += val.getAs<int>();

            // Modification Check
            val = val.getAs<int>() + 5;
        }

        REQUIRE( sum == 30 ); // 10 + 20
        REQUIRE( obj["A"].getAs<int>() == 15 ); // Modified
    }

    SECTION( "Standard Loop: Array (Elements)" )
    {
        Json arr = JsonArray();
        arr.addElementToArray( 1, 2, 3 );

        int sum = 0;
        for( auto& val : arr ) {
            sum += val.getAs<int>();
        }
        REQUIRE( sum == 6 );
    }

    // -------------------------------------------------------------------------
    // 2. items() Loop (Key & Value)
    // -------------------------------------------------------------------------
    SECTION( "items() Loop: Object (Key & Value)" )
    {
        Json obj = JsonObject();
        obj.addObject( "Name", "Tiny" ).addObject( "Ver", 1 );

        std::string keys = "";

        for( auto item : obj.items() )
        {
            keys += (std::string)item.key();

            if( item.key() == "Ver" ) {
                // Modify value via item proxy
                item.value() = 2;
            }
        }

        REQUIRE( keys == "NameVer" );
        REQUIRE( obj["Ver"].getAs<int>() == 2 );
    }

    SECTION( "items() Loop: Array (Index & Value)" )
    {
        Json arr = JsonArray();
        arr.addElementToArray( 10, 20, 30 );

        for( auto item : arr.items() )
        {
            // JsonKey automatically converts to size_t (index)
            size_t idx = item.key();

            // Modify value based on index
            item.value() = (int)( idx + 1 ) * 100;
        }

        REQUIRE( arr[0].getAs<int>() == 100 );
        REQUIRE( arr[1].getAs<int>() == 200 );
        REQUIRE( arr[2].getAs<int>() == 300 );
    }
}

// =============================================================================
// [Test 6] Iterator Compatibility & STL
// Verify manual iterator manipulation and STL algorithm support.
// =============================================================================
TEST_CASE( "Iterator STL Compatibility", "[iterator][stl]" )
{
    Json arr = JsonArray();
    arr.addElementToArray( 10, 20, 99, 40 );

    SECTION( "std::find_if" )
    {
        auto it = std::find_if( arr.begin(), arr.end(), []( const Json& j ) {
            return j.getAs<int>() == 99;
        });

        REQUIRE( it != arr.end() );
        REQUIRE( it->getAs<int>() == 99 );
        REQUIRE( std::distance( arr.begin(), it ) == 2 );
    }

    SECTION( "Const Iterator" )
    {
        const Json c_arr = arr;
        int count = 0;
        for( auto it = c_arr.begin(); it != c_arr.end(); ++it ) {
            count++;
        }
        REQUIRE( count == 4 );
    }
}

// =============================================================================
// [Test 7] Safety & Exception Handling
// Verify error handling for invalid operations.
// =============================================================================
TEST_CASE( "Exception Handling", "[exception]" )
{
    SECTION( "Invalid Json String" )
    {
        std::string invalidJson = "{ \"key\": 123 "; // Missing brace
        REQUIRE_THROWS_AS( Parser::parse( invalidJson ), TinyJsonException );
    }

    SECTION( "Out of Range Access" )
    {
        Json js = JsonArray( 1, 2 );
        REQUIRE_THROWS_AS( js[5], TinyJsonException );
    }

    SECTION( "Type Mismatch Access" )
    {
        Json js = JsonArray( 1, 2 );
        // Accessing Array with String Key
        REQUIRE_THROWS_AS( js["key"], TinyJsonException );
    }

    SECTION( "Const Object Invalid Key" )
    {
        const Json js = JsonObject( "exist", 1 );
        // Const access to non-existent key should throw, not create
        REQUIRE_THROWS_AS( js["empty"], TinyJsonException );
    }
}

// =============================================================================
// [Test 8] Utility Functions (tryGetAs, File IO, Memory)
// =============================================================================
TEST_CASE( "Utilities", "[utility]" )
{
    SECTION( "tryGetAs (Safe Retrieval)" )
    {
        Json js = JsonObject( "pi", 3.14 );
        double dVal = 0.0;
        int iVal = 0;

        REQUIRE( js["pi"].tryGetAs<double>( dVal ) == true );
        REQUIRE_THAT( dVal, Catch::Matchers::WithinRel( 3.14, 0.001 ) );

        REQUIRE( js["pi"].tryGetAs<int>( iVal ) == false ); // Type mismatch
    }

    SECTION( "File I/O" )
    {
        const char* filename = "test_temp.json";
        Json writeJs = JsonObject( "app", "TinyJson" );

        REQUIRE( writeJs.saveFile( filename ) == true );

        Json readJs = Parser::parseFile( filename );
        REQUIRE( readJs["app"].getAs<std::string>() == "TinyJson" );

        std::remove( filename );
    }

    SECTION( "Copy & Move Semantics" )
    {
        // Copy
        Json original = JsonObject( "k", "v" );
        Json copy = original;
        copy["k"] = "mod";
        REQUIRE( original["k"].getAs<std::string>() == "v" );

        // Move
        Json source = JsonArray( 1, 2, 3 );
        Json dest = std::move( source );
        REQUIRE( dest.size() == 3 );
        REQUIRE( source.isNull() ); // Source should be null after move
    }

    SECTION( "Check Key Existence (contains)" )
    {
        Json js = JsonObject( "exist", 1 );

        // 1. 존재하는 키
        REQUIRE( js.contains( "exist" ) == true );

        // 2. 존재하지 않는 키
        REQUIRE( js.contains( "empty" ) == false );

        // 3. Object가 아닌 경우
        Json arr = JsonArray();
        REQUIRE( arr.contains( "key" ) == false );
    }
}