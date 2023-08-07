# TinyJson

---

- ### Simple Json Parser using C++ ( Reference by [jute](https://github.com/amir-s/jute) )
    > - [x] [Add] Can create json format within source code  
    > - [x] [Add] Simple json format validation
    > - [x] [Add] Save json data to file
    > - [x] [Add] Distinguish between integers and real numbers
    > - [x] [Add] Exception handling when invalid key and index values are referenced
    > - [x] [Add] Adding json data in a natural way in code ( json["key"] = value )
    > - [ ] [Add] foreach : key, value 

---

<br>

## Environment
- It works ``Linux``
- No library dependencies

<br>


## Compile & Run
> **[Download]** git clone https://github.com/so686so/SimpleTool.git  // TODO
>  
> _Direcotry : SimpleTool/JsonParser_  //TODO
>   
> **[Compile]** g++ -o runFile JsonParser.h JsonParser.cpp main.cpp  
>  
> **[Run]** ./runFile  
---

<br>

## HOW TO USE

### 1. Load and Parse Json
- [ Sample Json File ] [**test_file_to.json**](./test_file_to_json.json)
```cpp
//main.cpp

#include "JsonParser.h"

using namespace std;
using namespace SimpleTool;

int main( void )
{
    // Load file & parse
    JsonValue js = Parser::parseFile("./test_file_to_json.json");

    // print Console
    cout << js.toString() << endl;

    return 0;
}
```
```shell
# Console Stdout
{
 "examples": [{
   "tag_name": "a",
   "attr": [{
     "key": "href",
     "value": "http://amir.saboury.net"
    }, {
     "key": "target",
     "value": "_blank"
    }]
  }, {
   "this_is": ["array", "of", "strings"],
   "number_array": [1, 2, 4, 8, 16],
   "pie": 3.14,
   "boolean": true,
   "bug": null,
   "mixed": [1, 2, {
     "test1": -1.2345,
     "test2": false
    }, null, 0.4, ["nested", ["array", true]], "end of story!"]
  }, {
   "done": true
  }, {
   "control_chars": "here is \\n: \nand it is tab: \t!\nsupports \" and other control chars\n"
  }]
}
```

<br>

### 2. Get Value From Key or Index
```cpp
//main.cpp

#include "JsonParser.h"

using namespace std;
using namespace SimpleTool;

int main( void )
{
    // Load file & parse
    JsonValue js = Parser::parseFile("./test_file_to_json.json");

    // use like, 'js["examples"][0]["tag_name"].getAs<string>()'

    // Get Value From Key
    cout << "a    ?= " << js["examples"][0]["tag_name"].getAs<string>()     << endl;
    cout << "3.14 ?= " << js["examples"][1]["pie"].getAs<double>()          << endl;
    cout << "16   ?= " << js["examples"][1]["number_array"][4].getAs<int>() << endl;

    return 0;
}
```
```shell
# Console Stdout
a    ?= a
3.14 ?= 3.14
16   ?= 16
```

<br>

### 3. Make Json(Simple) & File Save
```cpp
//main.cpp

#include "JsonParser.h"

using namespace std;
using namespace SimpleTool;

int main( void )
{
    JsonValue jsSmp = JsonObject("Key", "value"); // Insert key:value directly while creating JSON format

    // Save
    jsSmp.saveFile("test_json_to_file_smp.json");

    return 0;
}
```
```json
// ./test_json_to_file_smp.json
{
 "Key": "value"
}
```

<br>

### 4. Make Json(more complex) & File Save
```cpp
//main.cpp

#include "JsonParser.h"

using namespace std;
using namespace SimpleTool;

int main( void )
{
    JsonValue jsCpx = JsonObject(); // make empty json format

    // insert data by chain method
    jsCpx.addObject( "First",  1 )
         .addObject( "Second", 4.56 )
         .addObject( "Array",  JsonArray ( "1", 3.45, "Test", true ) )
         .addObject( "Child",  JsonObject( "ch1", 11 )
                              .addObject ( "ch2", 22 )
                              .addObject ( "GrandChild", JsonArray( "1", 3.45, "Test", true ) ) )
         .addObject( "Third",  123 )
         .addObject( "Last",   false );

    // Save
    jsCpx.saveFile("test_json_to_file_cpx.json");

    return 0;
}
```
```json
// ./test_json_to_file_cpx.json
{
 "First": 1,
 "Second": 4.56,
 "Array": ["1", 3.45, "Test", true],
 "Child": {
  "ch1": 11,
  "ch2": 22,
  "GrandChild": ["1", 3.45, "Test", true]
 },
 "Third": 123,
 "Last": false
}
```

<br>

---  

# License: [MIT](https://opensource.org/licenses/MIT)
