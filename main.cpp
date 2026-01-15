#include <iostream>
#include <string>
#include <vector>
#include "TinyJson.h"

using namespace std;
using namespace TinyJson;

// Compile : g++ -o runFile -I./include src/TinyJson.cpp main.cpp
// Run     : ./runFile

// ======================================================================================
// [Console Color Helper]
// 콘솔 출력을 꾸미기 위한 색상 상수 및 헬퍼 함수입니다.
// ======================================================================================
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

// ======================================================================================
// [Main Function]
// ======================================================================================
int main()
{
    // 예외 처리를 통해 안전하게 실행
    try
    {
        // ==============================================================================
        // [#001] Load & Parse
        // ==============================================================================
        printHeader("#001 Load & Parse JSON File");

        printDesc("Load JSON data from a file and parse it into a Json Object.");
        printCode("Json js = Parser::parseFile(\"data/Data.json\");");

        // 파일 파싱
        Json js = Parser::parseFile("data/Data.json");

        // ==============================================================================
        // [#002] Print Console
        // ==============================================================================
        printHeader("#002 Print Console");

        printDesc("Minified String (Default)");
        printCode("cout << js.toString() << endl;");

        cout << js.toString() << endl;

        printDesc("Pretty String (Indented)");
        printCode("cout << js.toString(ToStringType::Pretty) << endl;");

        cout << js.toString(ToStringType::Pretty) << endl;


        // ==============================================================================
        // [#003] Get Value from Key
        // ==============================================================================
        printHeader("#003 Access Object Values");

        printDesc("Retrieve values using key access operator [].");
        printCode("auto repo = js[\"repo_name\"].getAs<string>();");

        auto repo = js["repo_name"].getAs<string>();
        cout << "    -> repo_name: " << repo << endl;

        printDesc("Chained access for nested objects.");
        printCode("int meanOfLife = js[\"life\"][\"mean\"][\"is\"].getAs<int>();");

        int meanOfLife = js["life"]["mean"]["is"].getAs<int>();
        cout << "    -> life.mean.is: " << meanOfLife << endl;


        // ==============================================================================
        // [#004] Safe Access (tryGetAs)
        // ==============================================================================
        printHeader("#004 Safe Value Retrieval (tryGetAs)");

        printDesc("Check type safety before retrieving value. Returns false on type mismatch.");
        printCode("if( js[\"repo_name\"].tryGetAs<int>( refInt ) ) ... ");

        int refInt = 0;
        if( ( js["repo_name"] ).tryGetAs<int>( refInt ) )
            cout << Color::GREEN << "    Success! Value(int): " << refInt << Color::RESET << endl;
        else
            cout << Color::RED << "    Failed! 'repo_name' is not an Integer." << Color::RESET << endl;

        printDesc("Try getting as String:");
        printCode("if( js[\"repo_name\"].tryGetAs<string>( refStr ) ) ... ");

        string refStr = "";
        if( ( js["repo_name"] ).tryGetAs<string>( refStr ) )
            cout << Color::GREEN << "    Success! Value(str): " << refStr << Color::RESET << endl;
        else
            cout << Color::RED   << "    Failed! 'repo_name' is not a String." << Color::RESET << endl;


        // ==============================================================================
        // [#005] Array Access
        // ==============================================================================
        printHeader("#005 Access Array Elements");

        printDesc("Access array elements using index operator [index].");
        printCode("bool bVal = js[\"examples\"][1][\"boolean\"].getAs<bool>();");

        auto bool_value = js["examples"][1]["boolean"].getAs<bool>();
        if( bool_value == true ){
            cout << "    -> Boolean value is true." << endl;
        }

        printCode("int num = js[\"examples\"][1][\"number_array\"][3].getAs<int>();");
        cout << "    -> number_array[3]: " << js["examples"][1]["number_array"][3].getAs<int>() << endl;


        // ==============================================================================
        // [#006] Type Checking
        // ==============================================================================
        printHeader("#006 Check Value Types");

        printDesc("Inspect the type of each JSON element.");
        Json jsChild = js["examples"][1]["mixed"];

        // 람다 함수로 출력 간소화
        auto printType = [&](const string& label, bool result) {
            cout << "    " << label << ": " << (result ? Color::GREEN + "true" : Color::RED + "false") << Color::RESET << endl;
        };

        cout << jsChild.toString() << endl;
        printCode("jsChild[N].is{TYPE}();");
        printType("Is Index 0 Int?   ", jsChild[0].isInt());
        printType("Is Index 1 Double?", jsChild[1].isDouble()); // 2.0 or 2
        printType("Is Index 1 Int?   ", jsChild[1].isInt());
        printType("Is Index 2 Object?", jsChild[2].isObject());
        printType("Is Index 3 Null?  ", jsChild[3].isNull());
        printType("Is Index 4 Double?", jsChild[4].isDouble());
        printType("Is Index 5 Bool?  ", jsChild[5].isBool());
        printType("Is Index 6 Array? ", jsChild[6].isArray());
        printType("Is Index 7 String?", jsChild[7].isString());


        // ==============================================================================
        // [#007] Set New Value
        // ==============================================================================
        printHeader("#007 Set New Key-Value");

        printDesc("Accessing a non-existent key automatically creates a null object.");
        printCode("cout << js[\"NotYet\"].toString();");
        cout << "    Current: " << js["NotYet"].toString() << ", isNull: " << boolalpha << js["NotYet"].isNull() << endl;

        printDesc("Assign a value to the key.");
        printCode("js[\"NotYet\"] = \"Hello World!\";");
        js["NotYet"] = "Hello World!";

        cout << "    " << js.toString() << endl;


        // ==============================================================================
        // [#008] Type Replacement (Dynamic Typing)
        // ==============================================================================
        printHeader("#008 Dynamic Type Replacement");

        printDesc("A value can be replaced with a different type dynamically.");
        cout << "    Before (int) : " << js["life"]["mean"]["is"].getAs<int>() << endl;

        printCode("js[\"life\"][\"mean\"][\"is\"] = \"B612\";");
        js["life"]["mean"]["is"] = "B612";

        if( js["life"]["mean"]["is"].isString() )
            cout << "    After  (str) : " << js["life"]["mean"]["is"].getAs<string>() << endl;


        // ==============================================================================
        // [#009] Complex Replacement
        // ==============================================================================
        printHeader("#009 Complex Replacement");

        printDesc("Replacing value with Object:");
        printCode("js[\"target\"] = JsonObject(\"Language\", \"C++\");");
        js["life"]["mean"]["is"] = JsonObject("Language", "C++");
        cout << "    -> " << js["life"]["mean"]["is"].toString() << endl;

        printDesc("Replacing value with Array:");
        printCode("js[\"target\"] = JsonArray(1, JsonNULL(), true, -3.5);");
        js["life"]["mean"]["is"] = JsonArray(1, JsonNULL(), true, -3.5, "ABC");
        cout << "    -> " << js["life"]["mean"]["is"].toString() << endl;


        // ==============================================================================
        // [#010] Create Object (Simple)
        // ==============================================================================
        printHeader("#010 Create Simple Object");

        printCode("Json jsSmp = JsonObject(\"Key\", \"value\");");
        Json jsSmp = JsonObject("Key", "value");
        cout << jsSmp.toString(ToStringType::Pretty) << endl;


        // ==============================================================================
        // [#011] Create Object (Chaining) - Root : Object
        // ==============================================================================
        printHeader("#011 Create Complex Object (Chaining)");

        printDesc("Construct nested JSON using Method Chaining.");
        printCode("jsCpx.addObject(...).addObject(...);");

        Json jsCpx = JsonObject();
        jsCpx.addObject( "First",  1 )
             .addObject( "Second", 4.56 )
             .addObject( "Array",  JsonArray ( "1", 3.45, "Test", true ) )
             .addObject( "Child",  JsonObject( "ch1", 11 ).addObject ( "ch2", 22 ) )
             .addObject( "Last",   false );

        cout << jsCpx.toString(ToStringType::Pretty) << endl;


        // ==============================================================================
        // [#012] Create Complex Array (Mixed Methods)
        // ==============================================================================
        printHeader("#012 Create Complex Array (Mixed Methods)");

        printDesc("Demonstrating 'addElementToArray' (raw values) and 'addObject' (smart object insert) mixed chaining.");
        printCode("jsArr.addElementToArray(100).addObject(\"OBJ\", \"Created\").addElementToArray(...);");

        Json jsArr = JsonArray();

        jsArr.addElementToArray( 100 )                  // [ 100 ]
             .addObject( "OBJ", "Created" )             // [ 100, { "OBJ": "Created" } ]
             .addElementToArray( "Raw Element" )        // [ ..., "Raw Element" ]
             .addElementToArray( JsonArray(1, 2) )      // [ ..., [ 1, 2 ] ]
             .addObject( JsonObject("Nested", "Obj") ); // [ ..., { "Nested": "Obj" } ]

        cout << jsArr.toString(ToStringType::Pretty) << endl;


        // ==============================================================================
        // [#013] Create Object (Syntactic Sugar)
        // ==============================================================================
        printHeader("#013 Create Object (Assignment)");

        printDesc("Create JSON using easy assignment syntax.");
        Json jsEasy = JsonObject();

        jsEasy["Hello"]   = "World";
        jsEasy["List"]    = JsonArray( 1, 2, 3 );
        jsEasy["RawJson"] = "{ \"valid_json\": true }"; // 문자열로 된 JSON도 파싱하여 자동 삽입

        cout << jsEasy.toString(ToStringType::Pretty) << endl;


        // ==============================================================================
        // [#014] Save to File
        // ==============================================================================
        printHeader("#014 Save to File");

        printCode("js.saveFile(\"data/output_test.json\");");

        if( js.saveFile( "data/output_test.json" ) ) {
            cout << Color::GREEN << "    [Success] File saved to 'data/output_test.json'" << Color::RESET << endl;
        } else {
            cout << Color::RED   << "    [Error] File save failed." << Color::RESET << endl;
        }


        // ==============================================================================
        // [#015] Iteration
        // ==============================================================================
        printHeader("#015 Iteration (Keys & Values)");

        printDesc("1. Get all Keys:");
        vector<string> keys = js.keys();
        cout << "    Keys: [ ";
        for( size_t i=0; i<keys.size() && i<5; ++i ) cout << keys[i] << (i==keys.size()-1 ? "" : ", ");
        if(keys.size() > 5) cout << "... ";
        cout << "]" << endl;

        printDesc("2. Iterate Array using Range-based for loop:");
        printCode("for( auto& val : js[\"examples\"] ) { ... }");

        int count = 0;
        for( auto& eachValue : js["examples"] ) {
            cout << "    [" << count++ << "] " << eachValue.toString() << endl;
        }

        printHeader("End of Example");

    }
    catch ( const TinyJsonException& e ) {
        cerr << Color::RED << "\n[TinyJson Error] " << e.what() << Color::RESET << endl;
        return -1;
    }
    catch ( const std::exception& e ) {
        cerr << Color::RED << "\n[Standard Error] " << e.what() << Color::RESET << endl;
        return -1;
    }

    return 0;
}