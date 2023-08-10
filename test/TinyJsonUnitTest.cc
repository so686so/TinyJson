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


TEST_F( JsonParseFixture, Parse )
{
    EXPECT_EQ( 42, (*js)["life"]["mean"]["is"].getAs<int>() );
}


TEST_F( JsonParseFixture, Parse2 )
{
    EXPECT_EQ( std::string("TinyJson"), (*js)["repo_name"].getAs<string>() );
}

int main( int argc, char** argv )
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}