/**
 * vCard simple library.
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
#include <algorithm>
#include <string>
#include "xml_io.h"
#include "utils.h"

std::map<std::string, std::string> param_types {
    {"geo", "uri"}, {"pref", "integer"}
};

std::vector<std::string> property_adr_fields {
    "pobox", "ext", "street", "locality", "region", "code", "country"
};

std::vector<std::string> property_name_fields {
        "surname", "given", "additional", "prefix", "suffix"
};

XmlWriter & XmlWriter::operator << (vCard & vCard)
{
    *m_os << "<vcard>";

    for(auto it = vCard.properties().begin(); it != vCard.properties().end(); ++it){
        *this << *it;
    }

    *m_os << "</vcard>";

    return *this;
}

XmlWriter & XmlWriter::operator << (std::vector<vCard> & cards)
{
    *m_os << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
    *m_os << "<vcards xmlns=\"urn:ietf:params:xml:ns:vcard-4.0\">";

    for(auto card: cards){
        *this << card;
    }

    *m_os << "</vcards>";

    return *this;
}

std::string XmlWriter::get_property_type(std::string property_name, int count)
{
    std::string type("text");
    tolower(property_name);

    if(property_name == "adr"){
        if(count < property_adr_fields.size())
            type = property_adr_fields[count];
    } else if(property_name == "n"){
        if(count < property_name_fields.size())
            type = property_name_fields[count];
    } else {
        if(vCardProperty::property_types.find(property_name) != vCardProperty::property_types.end())
            type = vCardProperty::property_types.at(property_name);
    }

    return type;
}

XmlWriter & XmlWriter::operator << (vCardProperty & prop)
{
    *m_os << "<" << prop.getName() << ">";

    if(prop.params().size() > 0){
        *this << prop.params();
    }

    int count=0;
    auto it = prop.values().begin();
    while(it != prop.values().end()){
        std::string type = get_property_type(prop.getName(), count);

        if(it->size() == 0){
            *m_os << "<" << type << "/>";
        } else {
            *m_os << "<" << type << ">" << *it << "</" << type << ">";
        }

        it++;
        count++;
    }

    *m_os << "</" << prop.getName() << ">";

    return *this;
}

XmlWriter & XmlWriter::operator << (vCardParamMap & p)
{
    if(p.size() == 0)
        return *this;

    *m_os << "<parameters>";
    auto it = p.begin();
    while(it != p.end()){
        *m_os << "<" << it->first << ">";

        std::string type("text");
        std::string name(it->first);
        tolower(name);
        if(param_types.find(name) != param_types.end())
            type = param_types[name];
        *m_os << "<" << type << ">" << it->second << "</" << type << ">";
        *m_os << "</" << it->first << ">";
        it++;
    }

    *m_os << "</parameters>";

    return *this;
}

// ================================================================================

std::vector<vCard> XmlReader::parseCards(std::istream *is)
{
    std::istream::sentry se(*is, true);
    std::streambuf* sb = is->rdbuf();

    //auto EOF = std::streambuf::traits_type::eof();
    int c;
    while(sb->sgetc() != EOF){
        c = sb->sbumpc();

        if(c == '<' && sb->sgetc() == '?'){ //skip <?...?>
            c = sb->sbumpc(); // c=?
            while(c!=EOF && !(c=='?' && sb->sgetc()=='>'))
                c = sb->sbumpc();

            // we are on '>' or EOF
            sb->sbumpc();
            continue;
        }

        if(c == '<' && sb->sgetc() == '/') { //close tag
            std::string tag_name;
            sb->sbumpc(); // skip '/'
            c = sb->sbumpc();
            while(c!=EOF && c!='>'){
                tag_name += c;
                c = sb->sbumpc();
            }
            std::cout << "{" << tag_name << "}, ";

            // change state
            continue;
        }

        if(c == '<'){
            // read tag name
            std::string tag_name;
            c = sb->sbumpc();
            while (c!=EOF && !std::isspace(c) && c!='/' && c!='>') {
                tag_name += c;
                c = sb->sbumpc();
            }

            std::cout << tag_name << ", ";

            // if c == EOF throw exception XML format error
            if(std::isspace(c)) { // skip to the end of tag - no need for attributes
                while(c!=EOF && c!='/' && c!='>')
                    c = sb->sbumpc();
            }

            // if c == '/' - closed tag without content
            // if c == '>' - change state to open tag

            continue;
        }

        //read text inside tag

    }

    std::vector<vCard> t;
    return t;
}