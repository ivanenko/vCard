//
// Created by renier on 22/04/23.
//

#include <sstream>
#include <vector>
#include "vCardProperty.h"
#include "utils.h"

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

