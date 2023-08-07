#include "TinyJson.h"

using namespace std;
using namespace TinyJson;

// Compile : g++ -o runFile TinyJson.h TinyJson.cpp main.cpp
// TestRun : ./runFile

int main()
{
// Test : File -> Json
    JsonValue js = Parser::parseFile("./Data.json");
    cout << "=============================================================" << endl;

    // Print Console
    cout << js.toString() << endl;
    cout << "=============================================================" << endl;

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