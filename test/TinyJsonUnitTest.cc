#include "gtest/gtest.h"
#include "include/TinyJson.h"

using namespace TinyJson;

class JsonParseFixture : public ::testing::Test
{
protected:
    JsonParseFixture()
        : js( new JsonValue(JsonType::UNKNOWN) )
    {
    }

    ~JsonParseFixture()
    {
        delete js;
    }

    void SetUp() override
    {
        (*js) = Parser::parseFile("../data/Data.json");
    }

    JsonValue* js;
};

class JsonCreateFixture : public ::testing::Test
{
protected:
    JsonCreateFixture()
        : js( new JsonValue{ "{ \"StringKey\" : \"StringVal\", \"NullKey\": null, \"IntKey\": 123 }" } )
    {
    }

    ~JsonCreateFixture()
    {
        delete js;
    }

    void SetUp() override
    {
        //
    }

    JsonValue* js;
};

TEST( ConstructTest, Create )
{
    JsonValue jsn = JsonValue{"{\"A\":123}"};
    EXPECT_TRUE(jsn.isValid());
}

TEST( ConstructTest, TypeInt )
{
    JsonValue jsn = JsonValue(JsonType::INT);
    EXPECT_TRUE(jsn.isInt());
}

TEST( ConstructTest, TypeDouble )
{
    JsonValue jsn = JsonValue(JsonType::DOUBLE);
    EXPECT_TRUE(jsn.isDouble());
}

TEST( ConstructTest, TypeString )
{
    JsonValue jsn = JsonValue(JsonType::STRING);
    EXPECT_TRUE(jsn.isString());
}

TEST( ConstructTest, TypeBool )
{
    JsonValue jsn = JsonValue(JsonType::BOOLEAN);
    EXPECT_TRUE(jsn.isBool());
}


TEST_F( JsonParseFixture, Parse_Check_Int )
{
    EXPECT_EQ( 42, (*js)["life"]["mean"]["is"].getAs<int>() );
}


TEST_F( JsonParseFixture, Parse_Check_String )
{
    EXPECT_EQ( std::string("TinyJson"), (*js)["repo_name"].getAs<string>() );
}

TEST_F( JsonParseFixture, Parse_Check_Array )
{
    EXPECT_TRUE( (*js)["examples"].isArray() );
}

TEST_F( JsonParseFixture, Parse_Check_Object )
{
    EXPECT_TRUE( (*js)["examples"][0].isObject() );
    EXPECT_TRUE( (*js)["life"].isObject() );
    EXPECT_TRUE( (*js)["life"]["mean"].isObject() );
}

TEST_F( JsonCreateFixture, Create_Check_Int )
{
    EXPECT_EQ( 123, (*js)["IntKey"].getAs<int>() );
}

TEST_F( JsonCreateFixture, Create_Check_String )
{
    EXPECT_EQ( std::string("StringVal"), (*js)["StringKey"].getAs<string>() );
}

int main( int argc, char** argv )
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}