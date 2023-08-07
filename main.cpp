#include "TinyJson.h"

using namespace std;
using namespace TinyJson;

// Compile : g++ -o runFile TinyJson.h TinyJson.cpp main.cpp
// TestRun : ./runFile

int main()
{
    cout << "[#001] Load & Parse ------------------------------------------------------------" << endl;

    // Parse target json file
    JsonValue js = Parser::parseFile("./Data.json");
    
    // Print Console
    cout << js.toString() << endl;


    cout << "[#002] Get value from key : object ---------------------------------------------" << endl;

    // Use like, 'std::string value = js["repo_name"].getAs<string>();'
    auto repo = js["repo_name"].getAs<string>();
    cout << "key: repo_name, value: " << repo << endl;

    cout << js["thanks"].getAs<string>() << ", " 
         << js["life"]["mean"]["is"].getAs<int>() << endl;


    cout << "[#003] Get value from index : array --------------------------------------------" << endl;

    auto bool_value = js["examples"][1]["boolean"].getAs<bool>();
    if( bool_value == true ) {
        cout << "number is " << js["examples"][1]["number_array"][3].getAs<int>() << endl;
        cout << "pie value " << js["examples"][1]["pie"].getAs<double>() << endl;
    }


    cout << "[#004] Check value type --------------------------------------------------------" << endl;

    // get subset of json data
    JsonValue jsChild = js["examples"][1]["mixed"];

    cout << "isInt?    true  == " << boolalpha << jsChild[0].isInt()    << endl; // 1
    cout << "isInt?    false == " << boolalpha << jsChild[2].isInt()    << endl; // Object
    cout << "isDouble? true  == " << boolalpha << jsChild[4].isDouble() << endl; // 0.17171771
    cout << "isDouble? false == " << boolalpha << jsChild[0].isDouble() << endl; // 1
    cout << "isBool?   true  == " << boolalpha << jsChild[5].isBool()   << endl; // true
    cout << "isBool?   false == " << boolalpha << jsChild[3].isBool()   << endl; // null
    cout << "isString? true  == " << boolalpha << jsChild[7].isString() << endl; // "end list!"
    cout << "isObject? true  == " << boolalpha << jsChild[2].isObject() << endl; // object
    cout << "isArray?  true  == " << boolalpha << jsChild[6].isArray()  << endl; // array 
    cout << "isNull?   true  == " << boolalpha << jsChild[3].isNull()   << endl; // null


    cout << "[#005] Set new value -----------------------------------------------------------" << endl;

    // js["NotYet"] is not set yet
    cout << js["NotYet"].toString() << ", isNull: " << boolalpha << js["NotYet"].isNull() << endl;

    // just add
    js["NotYet"] = "Hello World!";
    cout << js.toString() << endl;


    cout << "[#006] Replace value -----------------------------------------------------------" << endl;

    // 42
    cout << "Before(int): " << js["life"]["mean"]["is"].getAs<int>() << endl;

    // A value can be replaced with a different type than the existing one.
    js["life"]["mean"]["is"] = "The reason for living isn't just coding...";

    // Do not enter the conditional statement
    if( js["life"]["mean"]["is"].isInt() ) 
        cout << "After(int):  " << js["life"]["mean"]["is"].getAs<int>() << endl;
        
    // Enter here : changed type string!
    if( js["life"]["mean"]["is"].isString() ) 
        cout << "After(str):  " << js["life"]["mean"]["is"].getAs<string>() << endl;


    cout << "[#007] Replace value 2 ---------------------------------------------------------" << endl;

    // Change value, Object
    js["life"]["mean"]["is"] = JsonObject("Language", "C++");
    // print pretty
    cout << js["life"]["mean"]["is"].toString() << endl;

    // Change value, null
    js["life"]["mean"]["is"] = JsonNULL();
    // print pretty
    cout << js["life"]["mean"]["is"].toString() << endl;

    // Change value, array
    js["life"]["mean"]["is"] = JsonArray(1, JsonNULL(), true, -3.5, "ABC");
    // print pretty
    cout << js["life"]["mean"]["is"].toString() << endl;


    // TODO
    // make json ojbect : simple
    // make json object : complex
    // make json object : using string -> json
    // save file
    return 0;

    // Get Value From Key
    cout << "Invalid ?= " << js["examples"][0]["tag_name"].getAs<string>()     << endl;
    cout << "3.14159 ?= " << js["examples"][1]["pie"].getAs<double>()          << endl;
    cout << "16      ?= " << js["examples"][1]["number_array"][4].getAs<int>() << endl;
    cout << "=============================================================" << endl;

    js["examples"][1]["pie"] = JsonObject("change", 12345);
    js["examples"][0] = JsonArray( "1", 3.45, "Test", true );
    js["examples"][1]["number_array"][4] = JsonNULL();
    js["examples"][1]["this_is"] = "changeValue!";
    js["new"] = 3;
    js["chain"]["method"] = "created";
    js["autoParse"] = "{ \"KEY\": 42, \"LIST\": [ 1, 2, 3.4, 5, null ] }";

    cout << js.toString() << endl;
    cout << "=============================================================" << endl;

    js.saveFile("./test_json_to_file_echo.json");

// Test : Json -> File ( simple )
    JsonValue jsSmp = JsonObject("Key", "value"); // JSON 포맷 만들면서 바로 key:value 집어넣기

    // Print Console
    cout << jsSmp.toString() << endl;
    cout << "=============================================================" << endl;

    // Save
    jsSmp.saveFile("test_json_to_file_smp.json");
    cout << "=============================================================" << endl;

// Test : Json -> File ( more complex )
    JsonValue jsCpx = JsonObject(); // 빈 JSON 포맷 만들기

    // 체인 메소드 방식으로 데이터 집어넣기
    jsCpx.addObject( "First",  1 )
         .addObject( "Second", 4.56 )
         .addObject( "Array",  JsonArray ( "1", 3.45, "Test", true ) )
         .addObject( "Child",  JsonObject( "ch1", 11 )
                              .addObject ( "ch2", 22 )
                              .addObject ( "GrandChild", JsonArray( "1", 3.45, "Test", true ) ) )
         .addObject( "Third",  JsonNULL() )
         .addObject( "Last",   false );

    // Print Console
    cout << jsCpx.toString() << endl;
    cout << "=============================================================" << endl;

    // Save
    jsCpx.saveFile("test_json_to_file_cpx.json");
    cout << "=============================================================" << endl;

    return 0;
}