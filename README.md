# TinyJson

<p align="center"><img src="./asset/TinyJson.png"></p>

## C++를 이용한 간단하고 강력한 JSON Parser ( Reference by [jute](https://github.com/amir-s/jute) )  

- **단 두 개의 코드 파일**만으로 작동합니다. ( `TinyJson.h`, `TinyJson.cpp` )  
- 각 파일은 **1000줄 미만의 코드**로 구성되어 있습니다. 코드를 분석해 보세요!  
- 아래는 [jute](https://github.com/amir-s/jute)의 원본 JSON parser에서 개선된 사항입니다.  

> * [x] [Add] Catch2 v3.5.0을 이용한 **단위 테스트(Unit Tests)** 도입  
> * [x] [Add] **CMake** 빌드 시스템 지원 및 예시 제공  
> * [x] [Add] 엄격한 파싱 및 유효성 검사 로직 추가  
> * [x] [Add] Json 데이터를 파일로 저장  
> * [x] [Add] 정수와 실수 타입 구분  
> * [x] [Add] 유효하지 않은 키/인덱스 참조 또는 파싱 오류 시 예외 처리 (`TinyJsonException`)  
> * [x] [Add] 직관적인 코드 작성 문법 지원 ( `json["key"] = value` )  
> * [x] [Add] **Range-based for loop** 지원 ( e.g. `for( auto& object : json )` )  

---

## 개발 환경

* `Linux`, `Windows`, `macOS` 환경 지원
* **`C++14`** 필수
* `CMake` 사용 (권장, v3.14 이상)
* **<u>외부 라이브러리 의존성 없음</u>** (단위 테스트 실행 시에만 CPM을 통해 Catch2를 자동으로 가져옵니다)

---

## 다른 프로젝트에 `TinyJson`을 가져가 사용하는 방법

- **필요 파일** : `TinyJson.h`, `TinyJson.cpp`
- 두 파일을 프로젝트 내 적절한 위치에 복사 후 `#include "TinyJson.h"`

---

## 예제 코드 및 단위 테스트 실행 방법

### 방법 1. `CMake` 사용한 예제 및 단위 테스트 빌드

> **1. [설정]** `mkdir build && cd build && cmake ..`  
> **2. [빌드]** `cmake --build .`  
> **3. [단위 테스트 실행]** `./unit_tests`  
> **4. [예제 코드 실행]** `./TinyJsonExample` (프로젝트 루트 경로에 실행 파일이 생성됩니다)  

### 방법 2. `g++` 사용한 예제 코드 수동 컴파일

> **1. [컴파일]** `g++ -o runFile -I./include src/TinyJson.cpp main.cpp`  
> **2. [예제 코드 실행]** `./runFile`

---

## 참고 사항

- **적절한 오류 검사가 구현되어 있습니다.** 파서는 입력된 JSON 형식이 유효하지 않거나 잘못된 접근( 예: const 객체에서 존재하지 않는 키에 접근, 배열 인덱스 초과 등 )이 발생할 경우 `TinyJson::TinyJsonException` 예외를 던집니다. 따라서 `try-catch` 블록을 사용하여 예외를 처리하는 것을 권장합니다.  

- 해당 프로젝트는 **최대한 단순하게 C++ JSON Parser 구현을 연습해보기 위한 샘플 코드 프로젝트**입니다. 만약 실제 업무에 사용 등 더 엄밀한 JSON Parser가 필요하다면, **[nlohmann/json](https://github.com/nlohmann/json)** 을 사용하세요. 해당 코드는 **단일 헤더 파일**로 구현되어 있으며, 사실상 실무 환경에서 업계 표준입니다. 하지만 **25000줄 이상의 코드 라인**을 가집니다.

---

## HOW TO USE (사용법)

> **01.** [Json 로드 및 파싱](#1-load-and-parse-json)  
> **02.** [키를 사용하여 값 가져오기 : Object](#2-get-value-from-key--object)  
> **03.** [안전하게 값 가져오기 : tryGetAs<T>()](#3-trygetas)  
> **04.** [인덱스를 사용하여 값 가져오기 : Array](#4-get-value-from-index--array)  
> **05.** [값 타입 확인](#5-check-value-type)  
> **06.** [새로운 값 설정](#6-set-new-value)  
> **07.** [값 교체하기](#7-replace-value)  
> **08.** [값 교체하기 2 (타입 변경)](#8-replace-value-2)  
> **09.** [Json 객체 만들기 : 단순형](#9-make-json-object--simple)  
> **10.** [Json 객체 만들기 : 복합형](#10-make-json-object--complex)  
> **11.** [Json 객체 만들기 : 간편한 방법](#11-make-json-object--easy-way)  
> **12.** [파일로 저장](#12-file-save)  
> **13.** [키 목록 가져오기 및 순회(Iteration)](#13-get-keys-or-iteration)  

---

### 1. Load and Parse Json

* [ 예제 Json 파일 ] **[Data.json](https://www.google.com/search?q=./data/Data.json)**

```c++
// main.cpp
#include <iostream>
#include "TinyJson.h"

using namespace std;
using namespace TinyJson;

int main( void )
{
    try {
        // 대상 json 파일을 파싱합니다.
        // 실패 시 TinyJsonException을 던집니다.
        Json js = Parser::parseFile( "data/Data.json" );

        // 콘솔에 출력합니다.
        cout << js.toString(ToStringType::Pretty) << endl;
    }
    catch( const TinyJsonException& e ) {
        cerr << e.what() << endl;
    }

    return 0;
}

```

<details>
<summary> 결과 (Result) </summary>

```shell
# Console Stdout
{
 "repo_name": "TinyJson",
 "examples": [ {
   "author": "so686so",
   "name": "SoByungJun",
   "attr": [ {
     "key": "link",
     "value": "http://github.com/so686so/TinyJson"
    }, {
     "key": "target",
     "value": "_everyone_"
    } ]
  }, {
   "this_is": [ "array", "of", "strings" ],
   "number_array": [ 1, 2, 4, 8, 16 ],
   "pie": 3.14159,
   "boolean": true,
   "bug": null,
   "mixed": [ 1, 2, {
     "arg_1": -100.12345,
     "arg_2": false
    }, null, 0.17171771, true, [ "this", [ "in_array is", true ] ], "end list!" ]
  }, {
   "test_json": true
  }, {
   "control_chars": "hellow \\n worlds! \nice to meet you:tab:\tEND\bS\n"
  } ],
 "thanks": "amir-s",
 "reference_by": "https://github.com/amir-s/jute",
 "life": {
  "mean": {
   "is": 42
  }
 }
}

```

</details>

### 2. Get value from key : object

```c++
int main( void )
{
    Json js = Parser::parseFile( "data/Data.json" );

    // 사용법: 'std::string value = js["repo_name"].getAs<string>();'
    auto repo = js["repo_name"].getAs<string>();

    cout << "key: repo_name, value: " << repo << endl;

    cout << js["thanks"].getAs<string>() << ", "
         << js["life"]["mean"]["is"].getAs<int>() << endl;

    return 0;
}

```

<details>
<summary> 결과 (Result) </summary>

```shell
# Console Stdout
key: repo_name, value: TinyJson
amir-s, 42

```

</details>

### 3. tryGetAs<T>()

```cpp
int main( void )
{
    Json js = Parser::parseFile( "data/Data.json" );

    // 사용법: if( {value}.tryGetAs<T>( T& ref ) )
    // tryGetAs 결과가 true이면, ref 변수를 통해 값을 얻을 수 있습니다.
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

    return 0;
}

```

<details>
<summary> 결과 (Result) </summary>

```shell
# Console Stdout
try get failed, ["repo_name"] value is not int
try get success, value(str): TinyJson

```

</details>

### 4. Get value from index : array

```cpp
int main( void )
{
    Json js = Parser::parseFile( "data/Data.json" );

    auto bool_value = js["examples"][1]["boolean"].getAs<bool>();
    if( bool_value == true ) {
        cout << "number is " << js["examples"][1]["number_array"][3].getAs<int>() << endl;
        cout << "pie value " << js["examples"][1]["pie"].getAs<double>() << endl;
    }

    return 0;
}

```

<details>
<summary> 결과 (Result) </summary>

```shell
# Console Stdout
number is 8
pie value 3.14159

```

</details>

### 5. Check value type

```cpp
int main( void )
{
    Json js = Parser::parseFile( "data/Data.json" );
    // Json 데이터의 일부(subset)를 가져옵니다.
    Json jsChild = js["examples"][1]["mixed"];

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

    return 0;
}

```

<details>
<summary> 결과 (Result) </summary>

```shell
# Console Stdout
isInt?    true  == true
isInt?    false == false
isDouble? true  == true
isDouble? false == false
isBool?   true  == true
isBool?   false == false
isString? true  == true
isObject? true  == true
isArray?  true  == true
isNull?   true  == true

```

</details>

### 6. Set new value

```cpp
int main( void )
{
    Json js = Parser::parseFile( "data/Data.json" );

    // js["NotYet"]은 아직 설정되지 않았습니다.
    cout << js["NotYet"].toString() << ", isNull: " << boolalpha << js["NotYet"].isNull() << endl;

    // 간단하게 값을 추가합니다.
    js["NotYet"] = "Hello World!";
    cout << js.toString() << endl;

    return 0;
}

```

<details>
<summary> 결과 (Result) </summary>

```shell
# Console Stdout
null, isNull: true
{
 ...
 "NotYet": "Hello World!"
}

```

</details>

### 7. Replace value

```cpp
int main( void )
{
    Json js = Parser::parseFile( "data/Data.json" );
    // 42
    cout << "Before(int): " << js["life"]["mean"]["is"].getAs<int>() << endl;

    // 기존과 다른 타입의 값으로도 교체가 가능합니다.
    js["life"]["mean"]["is"] = "The reason for living isn't just coding...";

    // 조건문에 진입하지 않습니다.
    if( js["life"]["mean"]["is"].isInt() )
        cout << "After(int):  " << js["life"]["mean"]["is"].getAs<int>() << endl;

    // 조건문에 진입합니다 : 타입이 string으로 변경되었습니다!
    if( js["life"]["mean"]["is"].isString() )
        cout << "After(str):  " << js["life"]["mean"]["is"].getAs<string>() << endl;

    return 0;
}

```

<details>
<summary> 결과 (Result) </summary>

```shell
# Console Stdout
Before(int): 42
After(str):  The reason for living isn't just coding...

```

</details>

### 8. Replace value 2

```cpp
int main( void )
{
    Json js = Parser::parseFile( "data/Data.json" );

    // Object 값으로 변경
    js["life"]["mean"]["is"] = JsonObject("Language", "C++");
    // 보기 좋게 출력 (pretty print)
    cout << js["life"]["mean"]["is"].toString() << endl;

    // null 값으로 변경
    js["life"]["mean"]["is"] = JsonNULL();
    // 보기 좋게 출력
    cout << js["life"]["mean"]["is"].toString() << endl;

    // Array 값으로 변경
    js["life"]["mean"]["is"] = JsonArray(1, JsonNULL(), true, -3.5, "ABC");
    // 보기 좋게 출력
    cout << js["life"]["mean"]["is"].toString() << endl;

    return 0;
}

```

<details>
<summary> 결과 (Result) </summary>

```shell
# Console Stdout
{
 "Language": "C++"
}
null
[ 1, null, true, -3.5, "ABC" ]

```

</details>

### 9. Make json object : simple

```cpp
int main( void )
{
    // Json 포맷을 생성하면서 직접 key:value를 삽입합니다.
    Json jsSmp = JsonObject("Key", "value");
    // 보기 좋게 출력
    cout << jsSmp.toString() << endl;

    return 0;
}

```

<details>
<summary> 결과 (Result) </summary>

```shell
# Console Stdout
{
 "Key": "value"
}

```

</details>

### 10. Make json object : complex

```cpp
int main( void )
{
    // 빈 json 포맷을 만듭니다.
    Json jsCpx = JsonObject();

    // 체인 메서드를 사용하여 데이터를 삽입합니다.
    jsCpx.addObject( "First",  1 )
         .addObject( "Second", 4.56 )
         .addObject( "Array",  JsonArray ( "1", 3.45, "Test", true ) )
         .addObject( "Child",  JsonObject( "ch1", 11 )
                              .addObject ( "ch2", 22 )
                              .addObject ( "GrandChild", JsonArray( "1", 3.45, "Test", true ) ) )
         .addObject( "Third",  JsonNULL() )
         .addObject( "Last",   false );

    // 보기 좋게 출력
    cout << jsCpx.toString(ToStringType::Pretty) << endl;

    return 0;
}

```

<details>
<summary> 결과 (Result) </summary>

```shell
# Console Stdout
{
  "First": 1,
  "Second": 4.56,
  "Array": [ "1", 3.45, "Test", true ],
  "Child": {
    "ch1": 11,
    "ch2": 22,
    "GrandChild": [ "1", 3.45, "Test", true ]
  },
  "Third": null,
  "Last": false
}

```

</details>

### 11. Make json object : easy way

```cpp
int main( void )
{
    Json jsEasy = JsonObject();

    jsEasy["Hello"]    = "World";
    jsEasy["Count"]    = 777;
    jsEasy["Nullable"] = JsonNULL();
    jsEasy["List"]     = JsonArray( 1, 2, 3, 4 );
    jsEasy["List2"]    = "[ 4, 5, 6, 7, { \"inList\": [ true, null, -3, 0.123 ] }, false ]";
    jsEasy["Objects"]  = JsonObject( "child", "value" );
    jsEasy["Objects2"] = "{ \"Child2\" : \"value2\", \"My\": null }";

    // 보기 좋게 출력
    cout << jsEasy.toString(ToStringType::Pretty) << endl;

    return 0;
}

```

<details>
<summary> 결과 (Result) </summary>

```shell
# Console Stdout
{
 "Hello": "World",
 "Count": 777,
 "Nullable": null,
 "List": [ 1, 2, 3, 4 ],
 "List2": [ 4, 5, 6, 7, {
   "inList": [ true, null, -3, 0.123 ]
  }, false ],
 "Objects": {
  "child": "value"
 },
 "Objects2": {
  "Child2": "value2",
  "My": null
 }
}

```

</details>

### 12. File save

```cpp
int main( void )
{
    Json js = Parser::parseFile( "data/Data.json" );

    if (js.saveFile( "data/test_json_to_file_echo.json" )) {
        cout << "File save done." << endl;
    }

    return 0;
}

```

<details>
<summary> 결과 (Result) </summary>

```bash
# Console Stdout
'File save done.'

```

</details>

### 13. Get keys or iteration

```cpp
int main( void )
{
    Json js = Parser::parseFile( "data/Data.json" );

    // 모든 키(key)를 가져옵니다.
    for( auto& k : js.keys() ) cout << k << endl;

    // Range-based for loop를 사용하여 배열을 순회합니다.
    for( auto& eachValue : js["examples"] ) {
        cout << "----------------------------------------------" << endl;
        cout << eachValue.toString() << endl;
    }

    return 0;
}

```

<details>
<summary> 결과 (Result) </summary>

```bash
# Console Stdout
repo_name
examples
thanks
reference_by
life
NotYet
----------------------------------------------
{
 "author": "so686so",
 "name": "SoByungJun",
 "attr": [ {
   "key": "link",
   "value": "http://github.com/so686so/TinyJson"
  }, {
   "key": "target",
   "value": "_everyone_"
  } ]
}
----------------------------------------------
{
 "this_is": [ "array", "of", "strings" ],
 "number_array": [ 1, 2, 4, 8, 16 ],
 ...
}
...

```

</details>

---

## License: [MIT](https://opensource.org/licenses/MIT)