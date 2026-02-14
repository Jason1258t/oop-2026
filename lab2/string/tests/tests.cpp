#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

#include "HtmlDecode.hpp"

#include <sstream>
#include <string>

TEST_CASE("HtmlDecode decodes HTML entities back to characters", "[HtmlDecode]")
{
    SECTION("Empty string returns empty string")
    {
        REQUIRE(HtmlDecode("") == "");
    }

    SECTION("String without entities returns unchanged")
    {
        REQUIRE(HtmlDecode("Hello World") == "Hello World");
        REQUIRE(HtmlDecode("Just regular text 123!@#") == "Just regular text 123!@#");
        REQUIRE(HtmlDecode("No &entities here") == "No &entities here"); 
    }

    SECTION("Decodes &quot; entity")
    {
        REQUIRE(HtmlDecode("&quot;") == "\"");
        REQUIRE(HtmlDecode("Hello &quot;World&quot;") == "Hello \"World\"");
        REQUIRE(HtmlDecode("&quot;Hello&quot; World") == "\"Hello\" World");
        REQUIRE(HtmlDecode("Three &quot;quotes&quot; here&quot;") == "Three \"quotes\" here\"");
        REQUIRE(HtmlDecode("&quot;&quot;") == "\"\"");
    }

    SECTION("Decodes &apos; entity")
    {
        REQUIRE(HtmlDecode("&apos;") == "'");
        REQUIRE(HtmlDecode("It&apos;s a test") == "It's a test");
        REQUIRE(HtmlDecode("&apos;Start&apos; and &apos;end&apos;") == "'Start' and 'end'");
        REQUIRE(HtmlDecode("&apos;&apos;") == "''");
    }

    SECTION("Decodes &lt; entity")
    {
        REQUIRE(HtmlDecode("&lt;") == "<");
        REQUIRE(HtmlDecode("a &lt; b") == "a < b");
        REQUIRE(HtmlDecode("&lt;&lt;") == "<<");
    }

    SECTION("Decodes &gt; entity")
    {
        REQUIRE(HtmlDecode("&gt;") == ">");
        REQUIRE(HtmlDecode("a &gt; b") == "a > b");
        REQUIRE(HtmlDecode("&lt;tag&gt;") == "<tag>");
        REQUIRE(HtmlDecode("&gt;&gt;") == ">>");
    }

    SECTION("Decodes &amp; entity")
    {
        REQUIRE(HtmlDecode("&amp;") == "&");
        REQUIRE(HtmlDecode("M&amp;M") == "M&M");
        REQUIRE(HtmlDecode("Rock &amp; Roll") == "Rock & Roll");
        REQUIRE(HtmlDecode("&amp;amp;") == "&amp;");
        REQUIRE(HtmlDecode("&amp;&amp;") == "&&");
    }

    SECTION("Decodes mixed entities in complex strings")
    {
        REQUIRE(HtmlDecode("Cat &lt;says&gt; &quot;Meow&quot;. M&amp;M&apos;s") == 
                "Cat <says> \"Meow\". M&M's");
        
        REQUIRE(HtmlDecode("&lt;html&gt;&lt;head&gt;&lt;title&gt;Test&lt;/title&gt;&lt;/head&gt;&lt;/html&gt;") == 
                "<html><head><title>Test</title></head></html>");
        
        REQUIRE(HtmlDecode("&lt;div class=&quot;container&quot;&gt;Content &amp; stuff&lt;/div&gt;") == 
                "<div class=\"container\">Content & stuff</div>");
        
        REQUIRE(HtmlDecode("&amp;lt; &amp;gt; &amp;amp; &amp;quot; &amp;apos;") == 
                "&lt; &gt; &amp; &quot; &apos;");
    }

    SECTION("Handles entities without semicolon (should not decode)")
    {
        // Согласно условию, сущности должны быть с ';'
        REQUIRE(HtmlDecode("&quot") == "&quot");
        REQUIRE(HtmlDecode("&apos") == "&apos");
        REQUIRE(HtmlDecode("&lt") == "&lt");
        REQUIRE(HtmlDecode("&gt") == "&gt");
        REQUIRE(HtmlDecode("&amp") == "&amp");
        REQUIRE(HtmlDecode("Hello &quotWorld&quot") == "Hello &quotWorld&quot");
    }

    SECTION("Handles partial matches correctly")
    {
        REQUIRE(HtmlDecode("&quot; and &quot") == "\" and &quot");
        REQUIRE(HtmlDecode("&amp; &amp &amp;") == "& &amp &");
        REQUIRE(HtmlDecode("&lt;&lt") == "<&lt");
    }

    SECTION("Handles long sequences without false positives")
    {
        std::string longText(1000, 'a');
        REQUIRE(HtmlDecode(longText) == longText);
        
        std::string textWithAmp = "a&a";
        REQUIRE(HtmlDecode(textWithAmp) == "a&a"); // & без ; не декодируется
    }

    SECTION("Handles entity-like strings that exceed max length")
    {
        // &abcdef; - длина 7 (больше maxEntityLength=6)
        REQUIRE(HtmlDecode("&abcdef;") == "&abcdef;");
        REQUIRE(HtmlDecode("&abcdefgh;") == "&abcdefgh;");
        REQUIRE(HtmlDecode("&quot; and &abcdef;") == "\" and &abcdef;");
    }

    SECTION("Handles multiple entities in sequence")
    {
        REQUIRE(HtmlDecode("&lt;&gt;&amp;&quot;&apos;") == "<>&\"'");
        REQUIRE(HtmlDecode("&lt;&lt;&lt;&lt;") == "<<<<");
        REQUIRE(HtmlDecode("&amp;&amp;&amp;&amp;") == "&&&&");
    }

    SECTION("Handles entities at boundaries (start, end)")
    {
        REQUIRE(HtmlDecode("&quot;Start") == "\"Start");
        REQUIRE(HtmlDecode("End&apos;") == "End'");
        REQUIRE(HtmlDecode("&lt;") == "<");
        REQUIRE(HtmlDecode("&gt;") == ">");
    }

    SECTION("Handles text around entities correctly")
    {
        REQUIRE(HtmlDecode("Text before &lt;tag&gt; text after") == "Text before <tag> text after");
        REQUIRE(HtmlDecode("&lt;start&gt;middle&lt;end&gt;") == "<start>middle<end>");
        REQUIRE(HtmlDecode("Before &amp; after") == "Before & after");
    }

    SECTION("Handles non-entity ampersands")
    {
        REQUIRE(HtmlDecode("A & B") == "A & B");
        REQUIRE(HtmlDecode("&&") == "&&");
        REQUIRE(HtmlDecode("& & &") == "& & &");
    }
}

TEST_CASE("HtmlDecode complexity is O(N)", "[HtmlDecode][complexity]")
{
    // Этот тест проверяет, что функция работает эффективно с большими строками
    SECTION("Processes long string efficiently")
    {
        std::string input;
        std::string expected;
        
        // Создаем строку с множеством сущностей
        for (int i = 0; i < 10000; ++i) {
            input += " &quot;&apos;&lt;&gt;&amp; ";
            expected += " \"'<>& ";
        }
        
        auto start = std::chrono::steady_clock::now();
        auto result = HtmlDecode(input);
        auto end = std::chrono::steady_clock::now();
        
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        REQUIRE(result == expected);
        // Проверяем, что время выполнения разумное (например, < 100 мс для 100к символов)
        REQUIRE(duration.count() < 100);
    }
}

TEST_CASE("HtmlDecode handles edge cases", "[HtmlDecode][edge]")
{
    SECTION("Handles string with only ampersand")
    {
        REQUIRE(HtmlDecode("&") == "&");
    }

    SECTION("Handles string with only semicolon")
    {
        REQUIRE(HtmlDecode(";") == ";");
    }

    SECTION("Handles string with ampersand at end")
    {
        REQUIRE(HtmlDecode("Hello &") == "Hello &");
    }

    SECTION("Handles string with ampersand and then text")
    {
        REQUIRE(HtmlDecode("&amp") == "&amp");
        REQUIRE(HtmlDecode("&amp and text") == "&amp and text");
    }

    SECTION("Handles very long entity name")
    {
        std::string longEntity = "&" + std::string(100, 'a') + ";";
        REQUIRE(HtmlDecode(longEntity) == longEntity); // Не должно декодироваться
    }

    SECTION("Handles nested entity patterns")
    {
        REQUIRE(HtmlDecode("&amp;quot;") == "&quot;"); // &amp;quot; -> &quot;
        REQUIRE(HtmlDecode("&amp;amp;") == "&amp;");  // &amp;amp; -> &amp;
    }
}