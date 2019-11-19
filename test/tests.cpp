/**
 * Tests for vCard simple library.
 *
 * Copyright (C) 2019, Ivanenko Danil.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <iostream>
#include <sstream>
#include "catch.hpp"
#include "../src/vcard.h"
#include "../src/text_io.h"
#include "../src/xml_io.h"

using namespace Catch;

TEST_CASE( "vCardParamMap", "[params]" ) {

    SECTION("test param map"){
        vCardParamMap params;
        params.addParam("TYPE", "home");
        params.addParam("TYPE", "work");
        params.addParam("LANGUAGE", "en");
        CHECK(params.size() == 3);
        CHECK(params["LANGUAGE"] == "en");
        CHECK(params.count("TYPE") == 2);

        params.setParam("TYPE", "fax");
        CHECK(params.size() == 2);

        params.addParam("TYPE", "voice");
        params.removeParam("TYPE");
        CHECK(params.size() == 1);

        params.clear();
        CHECK(params.is_empty());
    }
    SECTION("parse input string to params"){
        std::string str("TYPE=work;PREF=1;zz=zz");
        vCardParamMap params = TextReader::parseParams(str);
        REQUIRE(params.size() == 3);

        // TYPE and SORT-AS special cases
        // see https://www.rfc-editor.org/errata/eid3488
        str = "TYPE=\"work,home\";PREF=1";
        params = TextReader::parseParams(str);
        CHECK(params.size() == 3);
        CHECK(params.count("TYPE") == 2);

        for(param_iterator it = params.begin(); it != params.end(); ++it){
            std::cout << it->first << "=" << it->second << std::endl;
        }

        str = "VALUE=\"Name, 1\",name2,\"name; three, two3\";PREF=2";
        params = TextReader::parseParams(str);
        CHECK(params.size() == 4);

        TextWriter tw(std::cout);
        tw << params;
    }
}

TEST_CASE( "vCardProperty", "[property]" ) {
    SECTION("test property parsing"){
        std::string str = "grp.TEL;VALUE=uri;TYPE=home:tel:+33-01-23-45-67";
        vCardProperty prop = TextReader::parseProperty(str);
        CHECK(prop.getName() == "TEL");
        CHECK(prop.getGroup() == "grp");

        str = "grp.TEL;VALUE=\"bla : bla\";TYPE=home:tel:+33-01-23-45-67";
        prop = TextReader::parseProperty(str);
        CHECK(prop.params().size() == 2);
        CHECK(prop.params()["VALUE"] == "bla : bla");

        str = "ADR;TYPE=dom,home,postal,parcel:;;123 Main Street;Any Town;Region;91921\\;-1234;USA";
        prop = TextReader::parseProperty(str);
        CHECK(prop.values().at(vCardProperty::PostalCode) == "91921\\;-1234");
    }
    SECTION("property stream operator"){
        vCardProperty prop("grp", "TEL", "tel:+111-22-33");
        std::stringstream s;
        TextWriter tw(s);
        tw << prop;
        REQUIRE_THAT(s.str(), StartsWith("grp.TEL"));
    }
    SECTION("property creation"){
        vCardProperty p = vCardProperty::createAddress("Lane 5 ", "Detroit", "Nevada", "112233", "USA");
        vCardParamMap params;
        params.addParam("TYPE", "home");
        std::stringstream s;
        TextWriter tw(s);
        p << params;
        tw << p;
        REQUIRE_THAT(s.str(), EndsWith("Detroit;Nevada;112233;USA"));

        p = vCardProperty::createBirthday(2000, 12, 30);
        s.str(std::string());
        tw << p;
        REQUIRE(s.str() == "BDAY:2000-12-30");

        std::vector<std::string> levels {"HR", "ssk"};
        p = vCardProperty::createOrganization("Gooogle", levels);
        s.str(std::string());
        tw << p;
        REQUIRE(s.str() == "ORG:Gooogle;HR;ssk");
    }
}

TEST_CASE( "vCard", "[vcard]"){
    std::string str =
        "BEGIN:VCARD\r\n"
        "VERSION:4.0\r\n"
        "N:Gump;Forrest;;Mr.;\r\n"
        "FN:Forrest Gump\r\n"
        "ADR;TYPE=HOME;LABEL=\"42 Plantation St.\\nBaytown\\, LA 30314\\nUnited States of America\":;;42\n"
        " Plantation St.;Baytown;LA;30314;United States of America\r\n"
        "END:VCARD";

    std::stringstream s;
    s << str;
    TextReader tr(s);
    std::vector<vCard> cards = tr.parseCards();

    REQUIRE(cards.size() == 1);
    REQUIRE(cards[0].count() == 3);
    REQUIRE(cards[0]["ADR"].values().at(vCardProperty::Region) == "LA");
}

TEST_CASE("vCard object", "[vcard]"){
    vCardParamMap params;
    params.addParam("TYPE", "home");
    params.addParam("TYPE", "work");
    params.addParam("LANGUAGE", "en");

    vCardProperty p = vCardProperty::createAddress("Street", "City", "Nevada", "112233", "USA");
    p << params;

    vCard card;
    card << p;

    std::stringstream s;
    TextWriter tw(s);
    tw << card;

    REQUIRE_THAT(s.str(), StartsWith("BEGIN:VCARD"));
}

TEST_CASE("xCard parsing", "[xcard]"){
    std::stringstream s("<?xxx ?><vcard dd='ss'></vcard><param></param></aaa><bbb/>");
    XmlReader xr;
    std::vector<vCard> z = xr.parseCards(&s);
}

TEST_CASE("xCard params generate", "[xcard]"){
    vCardParamMap params;
    params.addParam("TYPE", "home");
    params.addParam("TYPE", "work");
    params.addParam("LANGUAGE", "en");
    params.addParam("PREF", "1");

    std::stringstream s;
    XmlWriter xw(s);
    xw << params;

    REQUIRE_THAT(s.str(), StartsWith("<parameters>"));
    REQUIRE_THAT(s.str(), Contains("<PREF><integer>1</integer></PREF>"));
}

TEST_CASE("xCard property generate", "[xcard]"){
    vCardProperty p = vCardProperty::createAddress("Street", "City", "Nevada", "112233", "USA");

    vCardParamMap params;
    params.addParam("TYPE", "home");
    params.addParam("PREF", "1");

    p << params;

    std::stringstream s;
    XmlWriter xw(s);
    xw << p;

    INFO(s.str());

    REQUIRE_THAT(s.str(), Contains("<pobox/><ext/><street>Street</street>"));

    s.str(std::string());
    vCardProperty n = vCardProperty::createName("fname", "lname");
    xw << n;
    REQUIRE_THAT(s.str(), Contains("<N><surname>lname</surname><given>fname</given><additional/><prefix/><suffix/></N>"));
}

TEST_CASE("xCard generate", "[xcard]") {
    vCardProperty p = vCardProperty::createAddress("Street", "City", "Nevada", "112233", "USA");

    vCard card;
    card << p;

    std::stringstream s;
    XmlWriter tw(s);
    tw << card;

    REQUIRE_THAT(s.str(), StartsWith("<vcard><ADR>"));
}
