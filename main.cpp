/**
 * TinyJson Example Usage
 * -----------------------------------------------------------------------------
 * Demonstrates 100% coverage of TinyJson features including:
 * - Parsing (File/String)
 * - Serialization (Pretty/Strip)
 * - Object/Array Construction & Chaining
 * - Deep Access & Safety Checks
 * - Iteration (Standard & Key-Value)
 * -----------------------------------------------------------------------------
 */

#include <iostream>
#include <string>
#include <vector>
#include "TinyJson.h"

using namespace std;
using namespace TinyJson;

// Compile : g++ -o TinyJsonExample -I./include src/TinyJson.cpp main.cpp
// Run     : ./TinyJsonExample

// =============================================================================
// [Console UI Helpers]
// =============================================================================
namespace Color {
    const string RESET   = "\033[0m";
    const string RED     = "\033[31m";
    const string GREEN   = "\033[32m";
    const string YELLOW  = "\033[33m";
    const string BLUE    = "\033[34m";
    const string MAGENTA = "\033[35m";
    const string CYAN    = "\033[36m";
    const string BOLD    = "\033[1m";
}

void printHeader( const string& title )
{
    cout << "\n" << Color::CYAN << Color::BOLD
         << "================================================================================" << endl;
    cout << "[ " << title << " ]" << endl;
    cout << "================================================================================" << Color::RESET << endl;
}

void printDesc( const string& desc )
{
    cout << Color::BLUE << " >> " << desc << Color::RESET << endl;
}

void printCode( const string& code )
{
    cout << Color::YELLOW << "    Code: " << code << Color::RESET << endl;
}

// =============================================================================
// [Main Function]
// =============================================================================
int main()
{
    try
    {
        // =====================================================================
        // [#001] Parsing & Serialization
        // =====================================================================
        printHeader( "#001 Load, Parse & Print" );

        printDesc( "Load JSON from file and print as Minified string." );
        Json js = Parser::parseFile( "data/Data.json" );
        cout << js.toString() << endl; // Default: Strip

        printDesc( "Print as Pretty string (Indented)." );
        cout << js.toString( ToStringType::Pretty ) << endl;


        // =====================================================================
        // [#002] Deep Access & Type Casting
        // =====================================================================
        printHeader( "#002 Deep Access & Get Value" );

        printDesc( "Access nested objects using [] operator." );
        printCode( "js[\"life\"][\"mean\"][\"is\"].getAs<int>();" );

        int mean = js["life"]["mean"]["is"].getAs<int>();
        cout << "    -> Value: " << mean << endl;

        printDesc( "Access array by index." );
        printCode( "js[\"examples\"][1][\"pie\"].getAs<double>();" );

        double pie = js["examples"][1]["pie"].getAs<double>();
        cout << "    -> Value: " << pie << endl;


        // =====================================================================
        // [#003] Safety (tryGetAs)
        // =====================================================================
        printHeader( "#003 Safety Check (tryGetAs)" );

        printDesc( "Attempt to get an Integer as a String (Should Fail)." );
        string refStr;
        if( js["life"]["mean"]["is"].tryGetAs<string>( refStr ) ) {
            cout << Color::RED << "    [Fail] It is not a string." << Color::RESET << endl;
        } else {
            cout << Color::GREEN << "    [Pass] Type mismatch detected safely." << Color::RESET << endl;
        }

        printDesc( "Attempt to get an Integer as an Integer (Should Success)." );
        int refInt;
        if( js["life"]["mean"]["is"].tryGetAs<int>( refInt ) ) {
            cout << Color::GREEN << "    [Pass] Value retrieved: " << refInt << Color::RESET << endl;
        }

        printDesc( "Check if a key exists using .contains()." );
        if( js["life"]["mean"].contains( "is" ) ) {
            cout << Color::GREEN << "    [Pass] Key 'is' exists." << Color::RESET << endl;
        }
        if( !js.contains( "GhostKey" ) ) {
            cout << Color::GREEN << "    [Pass] Key 'GhostKey' does not exist." << Color::RESET << endl;
        }


        // =====================================================================
        // [#004] Constructing Objects (Chaining)
        // =====================================================================
        printHeader( "#004 Constructing Objects (Chaining)" );

        printDesc( "Build complex objects using method chaining." );
        Json myObj = JsonObject();

        myObj.addObject( "project", "TinyJson" )
             .addObject( "version", 2.5 )
             .addObject( "active",  true )
             .addObject( "meta",    JsonObject( "author", "Gemini" )
                                      .addObject( "year",   2026 ) );

        cout << myObj.toString( ToStringType::Pretty ) << endl;


        // =====================================================================
        // [#005] Constructing Arrays (Mixed)
        // =====================================================================
        printHeader( "#005 Constructing Arrays" );

        printDesc( "Build arrays with mixed types using addElementToArray." );
        Json myArr = JsonArray();

        myArr.addElementToArray( 1, 2, 3 )           // Add Integers
             .addElementToArray( "Start", "End" )    // Add Strings
             .addObject( "id", 99 );                 // Smart Add: Push Object {id:99}

        cout << myArr.toString() << endl;


        // =====================================================================
        // [#006] Iteration: Standard Loop (Values Only)
        // =====================================================================
        printHeader( "#006 Standard Iteration (Values Only)" );

        printDesc( "Loop over an Array (Read & Modify via Reference)." );
        Json numbers = JsonArray( 10, 20, 30 );

        cout << "    [Before] " << numbers.toString() << endl;

        for( auto& val : numbers ) {
            // val is Json&
            val = val.getAs<int>() + 1; // Modify value
        }

        cout << "    [After]  " << numbers.toString() << endl;


        // =====================================================================
        // [#007] Iteration: Items Loop (Key & Value)
        // =====================================================================
        printHeader( "#007 Items Iteration (Key & Value)" );

        printDesc( "Loop over an Object using .items()." );

        for( auto item : myObj.items() ) {
            // item.key() returns JsonKey (convertible to string)
            // item.value() returns Json&
            cout << "    Key: " << item.key() << ", Value: " << item.value().toString() << endl;
        }

        printDesc( "Loop over an Array using .items() (Key is Index)." );

        for( auto item : numbers.items() ) {
            // item.key() acts as size_t index here!
            size_t idx = item.key();
            cout << "    Index [" << idx << "] = " << item.value().getAs<int>() << endl;
        }


        // =====================================================================
        // [#008] Modification & File Save
        // =====================================================================
        printHeader( "#008 Modify & Save" );

        printDesc( "Modify existing keys and save to file." );

        js["repo_name"] = "TinyJson-Refactored"; // Change String
        js["new_field"] = JsonArray( 1, 2, 3 );  // Add New Array
        js["char_type"] = "!?|@#$%^&*(){}[]<>-=_+;'\",.";
        js["emoticons"] = "📄✨📖✅❌🛠️😊";

        if( js.saveFile( "data/output_test.json" ) ) {
            cout << Color::GREEN << "    [Success] Saved to 'data/output_test.json'" << Color::RESET << endl;
        }

        printHeader( "Example Finished" );

    }
    catch( const TinyJsonException& e ) {
        cerr << Color::RED << "\n[TinyJson Error] " << e.what() << Color::RESET << endl;
        return -1;
    }
    catch( const std::exception& e ) {
        cerr << Color::RED << "\n[Standard Error] " << e.what() << Color::RESET << endl;
        return -1;
    }

    return 0;
}