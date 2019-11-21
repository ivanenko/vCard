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

#include <algorithm>
#include "json_io.h"
#include "utils.h"


JsonWriter & JsonWriter::operator << (vCard & vCard)
{
    *m_os << "[\"vcard\", [";

    *m_os << "[\"version\", {}, \"text\", \"4.0\"],";
    for(auto it = vCard.properties().begin(); it != vCard.properties().end(); ++it){
        *this << *it;
    }

    *m_os << "]]";
    return *this;
}

JsonWriter & JsonWriter::operator << (std::vector<vCard> & cards)
{
    *m_os << "[";

    for(auto card: cards){
        *this << card;
    }

    *m_os << "]";

    return *this;
}

JsonWriter & JsonWriter::operator << (vCardProperty & p)
{
    *m_os << "[";

    std::string name(p.getName());
    tolower(name);

    *m_os << "\"" << name << "\", ";
    *this << p.params();
    *m_os << ", ";

    *m_os << "\"" << get_property_type(name) << "\", ";

    if(p.values().size() > 1)
        *m_os << "[";

    auto it = p.values().begin();
    while(it != p.values().end()){
        *m_os << "\"" << *it << "\"";

        it++;

        if(it != p.values().end())
            *m_os << ", ";
    }

    if(p.values().size() > 1)
        *m_os << "]";

    *m_os << "]";
    return *this;
}

std::string JsonWriter::get_property_type(std::string property_name)
{
    std::string type("text");
    tolower(property_name);

    if(vCardProperty::property_types.find(property_name) != vCardProperty::property_types.end())
        type = vCardProperty::property_types.at(property_name);

    return type;
}

JsonWriter & JsonWriter::operator << (vCardParamMap & p)
{
    *m_os << "{";

    auto it = p.begin();
    while(it != p.end()){
        *m_os << "\"" << it->first << "\": \"" << it->second << "\"";

        it++;
        if(it != p.end())
            *m_os << ", ";
    }

    *m_os << "}";
    return *this;
}

// ================================================================================

std::vector<vCard> JsonReader::parseCards(std::istream *is)
{
    std::vector<vCard> t;
    return t;
}