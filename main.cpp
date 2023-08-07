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


    cout << "[#003] tryGetAs<T>() ---------------------------------------------------------" << endl;

    // Use like, if( {value}.tryGetAs<T>( T& ref ) )
    // If tryGetAs result true, you can get value by ref
    int refInt = 0;
    if( ( js["repo_name"] ).tryGetAs<int>( refInt ) )
        cout << "try get success, value(int): " << refInt << endl;
    else
        cout << "try get failed, [\"repo_name\"] value is not int" << endl;

    string refStr = "";
    if( ( js["repo_name"] ).tryGetAs<string>( refStr ) )
        cout << "try get success, value(str): " << refStr << endl;
    else
        cout << "try get failed, [\"repo_name\"] value is not string" << endl;


    cout << "[#004] Get value from index : array --------------------------------------------" << endl;

    auto bool_value = js["examples"][1]["boolean"].getAs<bool>();
    if( bool_value == true ) {
        cout << "number is " << js["examples"][1]["number_array"][3].getAs<int>() << endl;
        cout << "pie value " << js["examples"][1]["pie"].getAs<double>() << endl;
    }


    cout << "[#005] Check value type --------------------------------------------------------" << endl;

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


    cout << "[#006] Set new value -----------------------------------------------------------" << endl;

    // js["NotYet"] is not set yet
    cout << js["NotYet"].toString() << ", isNull: " << boolalpha << js["NotYet"].isNull() << endl;

    // just add
    js["NotYet"] = "Hello World!";
    cout << js.toString() << endl;


    cout << "[#007] Replace value -----------------------------------------------------------" << endl;

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


    cout << "[#008] Replace value 2 ---------------------------------------------------------" << endl;

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


    cout << "[#009] Make json ojbect : simple -----------------------------------------------" << endl;

    // Insert key:value directly while creating json format
    JsonValue jsSmp = JsonObject("Key", "value");
    // print pretty
    cout << jsSmp.toString() << endl;


    cout << "[#010] Make json ojbect : complex ----------------------------------------------" << endl;

    // make empty json format
    JsonValue jsCpx = JsonObject();

    // insert data using chain method
    jsCpx.addObject( "First",  1 )
         .addObject( "Second", 4.56 )
         .addObject( "Array",  JsonArray ( "1", 3.45, "Test", true ) )
         .addObject( "Child",  JsonObject( "ch1", 11 )
                              .addObject ( "ch2", 22 )
                              .addObject ( "GrandChild", JsonArray( "1", 3.45, "Test", true ) ) )
         .addObject( "Third",  JsonNULL() )
         .addObject( "Last",   false );

    // print pretty
    cout << jsCpx.toString() << endl;


    cout << "[#010] Make json ojbect : easy way ---------------------------------------------" << endl;

    JsonValue jsEasy = JsonObject();

    jsEasy["Hello"]    = "World";
    jsEasy["Count"]    = 777;
    jsEasy["Nullable"] = JsonNULL();
    jsEasy["List"]     = JsonArray( 1, 2, 3, 4 );
    jsEasy["List2"]    = "[ 4, 5, 6, 7, { \"inList\": [ true, null, -3, 0.123 ] }, false ]";
    jsEasy["Objects"]  = JsonObject( "child", "value" );
    jsEasy["Objects2"] = "{ \"Child2\" : \"value2\", \"My\": null }";

    // print pretty
    cout << jsEasy.toString() << endl;


    cout << "[#011] File save ---------------------------------------------------------------" << endl;

    js.saveFile( "test_sjon_to_file_echo.json" );

    return 0;
}