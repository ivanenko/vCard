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

#include <istream>
#include <string>
#include <sstream>
#include <algorithm>
#include "vcard.h"
#include "utils.h"


//==============================================================================

void vCardParamMap::addParam(std::string name, std::string value)
{
    toupper(name);
    m_params.insert({name, value});
}

void vCardParamMap::setParam(std::string name, std::string value)
{
    toupper(name);
    m_params.erase(name);
    m_params.emplace(name, value);
}

void vCardParamMap::removeParam(std::string name)
{
    toupper(name);
    m_params.erase(name);
}

std::pair<param_iterator, param_iterator> vCardParamMap::getParams(std::string name)
{
    return m_params.equal_range(name);
}


std::string& vCardParamMap::operator[] (std::string name)
{
    auto search = m_params.find(name);
    if(search != m_params.end())
        return search->second;
    else
        throw std::runtime_error("param not found");
}

// =================================================================

const std::map<std::string, std::string> vCardProperty::property_types =
    {
        {"fn", "text"}, {"bday", "date"}, {"anniversary", "text"}, {"gender", "sex"},
        {"lang", "language-tag"}, {"tel", "uri"}, {"geo", "uri"}, {"key", "uri"},
        {"url", "uri"}, {"photo", "uri"}, {"impp", "uri"}, {"logo", "uri"},
        {"member", "uri"}, {"related", "uri"}, {"rev", "timestamp"}, {"sound", "uri"},
        {"uid", "uri"}, {"url", "uri"}, {"fburl", "uri"}, {"caladruri", "uri"},
        {"caluri", "uri"}, {"source", "uri"}, {"adr", "text"}, {"n", "text"}
    };

vCardProperty::vCardProperty(const std::string& name, const std::string& value)
{
    m_name = name;
    m_values = split(value, VC_SEPARATOR_TOKEN, true);
}

vCardProperty::vCardProperty(const std::string& group, const std::string& name, const std::string& value)
{
    m_name = name;
    m_group = group;
    m_values = split(value, VC_SEPARATOR_TOKEN, true);
}

void vCardProperty::setParams(vCardParamMap params)
{
    this->m_params = params;
}

std::string vCardProperty::getValue()
{
    std::stringstream s;

    auto it = m_values.begin();
    while(it != m_values.end()){
        s << *it;
        it++;
        if(it != m_values.end())
            s << ";";
    }

    return s.str();
}

vCardProperty & vCardProperty::operator << (const vCardParamMap &p)
{
    this->setParams(p);
    return *this;
}

vCardProperty vCardProperty::createAddress(const std::string& street, const std::string& locality,
        const std::string& region, const std::string& postal_code, const std::string& country,
        const std::string& post_office_box, const std::string& ext_address, const vCardParamMap& params)
{
    std::vector<std::string> values;
    values.push_back(post_office_box);
    values.push_back(ext_address);
    values.push_back(street);
    values.push_back(locality);
    values.push_back(region);
    values.push_back(postal_code);
    values.push_back(country);

    return vCardProperty(VC_ADDRESS, values, params);
}

vCardProperty vCardProperty::createName(const std::string& firstname, const std::string& lastname,
                                const std::string& additional, const std::string& prefix,
                                const std::string& suffix, const vCardParamMap& params)
{
    std::vector<std::string> values;
    values.push_back(lastname);
    values.push_back(firstname);
    values.push_back(additional);
    values.push_back(prefix);
    values.push_back(suffix);

    return vCardProperty(VC_NAME, values, params);
}

vCardProperty vCardProperty::createOrganization(const std::string& name, const std::vector<std::string>& levels,
        const vCardParamMap& params)
{
    std::vector<std::string> values;
    values.push_back(name);
    values.insert(values.end(), levels.begin(), levels.end());

    return vCardProperty(VC_ORGANIZATION, values, params);
}

vCardProperty vCardProperty::createBirthday(const int year, const int month, const int day)
{
    char buffer[15];
    sprintf(buffer, "%4d-%2d-%2d", year, month, day);
    return vCardProperty(VC_BIRTHDAY, buffer);
}



// ======================================================================

vCardProperty& vCard::operator[] (std::string name) {
    for(vCardProperty& p: m_properties) {
        if (p.getName() == name)
            return p;
    }

    throw std::runtime_error("Property not found");
}

vCard & vCard::operator << (const vCardProperty &p)
{
    m_properties.push_back(p);
    return *this;
}

void vCard::addProperty(const vCardProperty& property)
{
    m_properties.push_back(property);
}

void vCard::removeProperty(std::string name)
{
    auto it = m_properties.begin();
    while(it != m_properties.end()){
        if(it->getName() == name)
            m_properties.erase(it);
        else
            it++;
    }
}

std::string vCard::getVersionStr()
{
    switch (m_version){
        case VC_VER_2_1:
            return "2.1";

        case VC_VER_3_0:
            return "3.0";

        case VC_VER_4_0:
            return "4.0";
    }
}