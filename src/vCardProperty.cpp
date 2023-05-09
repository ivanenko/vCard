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
vCardProperty::vCardProperty(const std::string& name, const std::vector<std::string>& values) {
    m_name = name;
    m_values = values;
}

vCardProperty::vCardProperty(const std::string& name, const std::vector<std::string>& values, const vCardParamMap params) {
    m_name = name;
    m_values = values;
    if (!isValidParamMap(params)) {
        throw std::invalid_argument("invalid param map");
    }
    m_params = params;
}

void vCardProperty::setParams(vCardParamMap params)
{
    if (!isValidParamMap(params)) {
        throw std::invalid_argument("invalid param map");
    }
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

bool vCardProperty::hasAltId() {
    bool hasAltId = false;

    for (auto & param : m_params) {
        if (param.first == "ALTID") {
            hasAltId = true;
        }
    }

    return hasAltId;
}

vCardProperty & vCardProperty::operator << (const vCardParamMap &p)
{
    if (!isValidParamMap(p)) {
        throw std::invalid_argument("invalid param map");
    }
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

    vCardProperty cardProperty = vCardProperty(VC_ADDRESS, values);
    cardProperty.setParams(params);

    return cardProperty;
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

    vCardProperty cardProperty = vCardProperty(VC_NAME, values);
    cardProperty.setParams(params);

    return cardProperty;
}

vCardProperty vCardProperty::createOrganization(const std::string& name, const std::vector<std::string>& levels,
                                                const vCardParamMap& params)
{
    std::vector<std::string> values;
    values.push_back(name);
    values.insert(values.end(), levels.begin(), levels.end());

    vCardProperty cardProperty = vCardProperty(VC_ORGANIZATION, values);
    cardProperty.setParams(params);

    return cardProperty;
}

vCardProperty vCardProperty::createBirthday(const int year, const int month, const int day)
{
    char buffer[15];
    sprintf(buffer, "%4d-%2d-%2d", year, month, day);
    return vCardProperty(VC_BIRTHDAY, buffer);
}

bool vCardProperty::isValidParamMap(vCardParamMap params) {
    bool isValid = true;

    std::vector<std::string> disallowed;

    if (m_name == VC_ADDRESS) {
        disallowed = {
                "MEDIATYPE", "CALSCALE", "SORT-AS", "LEVEL"
        };
    }
    else if (m_name == VC_ANNIVERSARY) {
        disallowed = {
                "LANGUAGE", "PREF", "PID", "TYPE", "MEDIATYPE", "SORT-AS",
                "GEO", "TZ", "LABEL", "CC", "INDEX", "LEVEL"
        };
    }
    else if (m_name == VC_BIRTHDAY) {
        disallowed = {
                "PREF", "PID", "TYPE", "MEDIATYPE", "SORT-AS", "GEO", "TZ",
                "LABEL", "CC", "INDEX", "LEVEL"
        };
    }
    else if (m_name == VC_BIRTHPLACE) {
        disallowed = {
                "PREF", "PID", "TYPE", "MEDIATYPE", "CALSCALE", "SORT-AS",
                "GEO", "TZ", "LABEL", "CC", "INDEX", "LEVEL"
        };
    }
    else if (m_name == VC_CALURI || m_name == VC_CALADRURI) {
        disallowed = {
                "LANGUAGE", "CALSCALE", "SORT-AS", "GEO", "TZ", "LABEL",
                "CC", "LEVEL"
        };
    }
    else if (m_name == VC_CATEGORIES) {
        disallowed = {
                "LANGUAGE", "MEDIATYPE", "CALSCALE", "SORT-AS", "GEO",
                "TZ", "LABEL", "CC", "LEVEL"
        };
    }
    else if (m_name == VC_CONTACT_URI) {
        disallowed = {
                "LANGUAGE", "ALTID", "PID", "TYPE", "MEDIATYPE", "CALSCALE",
                "SORT-AS", "GEO", "TZ", "LABEL", "CC", "LEVEL"
        };
    }
    else if (m_name == VC_CLIENT_PID_MAP) {
        disallowed = {
                "LANGUAGE", "VALUE", "PREF", "ALTID", "PID", "TYPE", "MEDIATYPE",
                "CALSCALE", "SORT-AS", "GEO", "TZ", "LABEL", "CC", "INDEX", "LEVEL"
        };
    }
    else if (m_name == VC_DEATH_DATE) {
        disallowed = {
                "PREF", "PID", "TYPE", "MEDIATYPE", "SORT-AS", "GEO", "TZ", "LABEL",
                "CC", "INDEX", "LEVEL"
        };
    }
    else if (m_name == VC_DEATH_PLACE) {
        disallowed = {
                "PREF", "PID", "TYPE", "MEDIATYPE", "SORT-AS", "GEO", "TZ", "LABEL",
                "CC", "INDEX", "LEVEL", "CALSCALE"
        };
    }
    else if (m_name == VC_EMAIL){
        disallowed = {
                "LANGUAGE", "MEDIATYPE", "CALSCALE", "SORT-AS", "GEO", "TZ", "LABEL",
                "CC", "LEVEL"
        };
    }
    else if (m_name == VC_EXPERTISE) {
        disallowed = {
                "VALUE", "PID", "MEDIATYPE", "CALSCALE", "SORT-AS", "GEO", "TZ", "LABEL",
                "CC"
        };
    }
    else if (m_name == VC_FORMATTED_NAME) {
        disallowed = {
                "TYPE", "MEDIATYPE", "CALSCALE", "SORT-AS", "GEO", "TZ", "LABEL", "CC",
                "LEVEL"
        };
    }
    else if (m_name == VC_FREEBUSY_URL) {
        disallowed = {
                "LANGUAGE", "CALSCALE", "SORT-AS", "GEO", "TZ", "LABEL", "CC", "LEVEL"
        };
    }
    else if (m_name == VC_GENDER) {
        disallowed = {
                "LANGUAGE", "PREF", "ALTID", "PID", "TYPE", "MEDIATYPE", "CALSCALE",
                "SORT-AS", "GEO", "TZ", "LABEL", "CC", "INDEX", "LEVEL"
        };
    }
    else if (m_name == VC_GEOGRAPHIC_POSITION) {
        disallowed = {
                "LANGUAGE", "CALSCALE", "SORT-AS", "TZ", "LABEL", "CC", "LEVEL"
        };
    }
    else if (m_name == VC_HOBBY) {
        disallowed = {
                "VALUE", "PID", "MEDIATYPE", "CALSCALE", "SORT-AS", "GEO", "TZ", "LABEL",
                "CC"
        };
    }
    else if (m_name == VC_IMPP) {
        disallowed = {
                "LANGUAGE", "CALSCALE", "SORT-AS", "GEO", "TZ", "LABEL", "CC", "LEVEL"
        };
    }
    else if (m_name == VC_INTEREST) {
        disallowed = {
                "VALUE", "PID", "TYPE", "MEDIATYPE", "CALSCALE", "SORT-AS", "GEO", "TZ",
                "LABEL", "CC"
        };
    }
    else if (m_name == VC_KEY) {
        disallowed = {
                "LANGUAGE", "CALSCALE", "SORT-AS", "GEO", "TZ", "LABEL", "CC", "LEVEL"
        };
    }
    else if (m_name == VC_KIND) {
        disallowed = {
            "LANGUAGE", "PREF", "ALTID", "PID", "TYPE", "MEDIATYPE", "CALSCALE",
            "SORT-AS", "GEO", "TZ", "LABEL", "CC", "INDEX", "LEVEL"
        };
    }
    else if (m_name == VC_LANG) {
        disallowed = {
                "LANGUAGE", "MEDIATYPE", "CALSCALE", "SORT-AS", "GEO", "TZ", "LABEL",
                "CC", "LEVEL"
        };
    }
    else if (m_name == VC_LOGO) {
        disallowed = {
                "CALSCALE", "SORT-AS", "GEO", "TZ", "LABEL", "CC", "INDEX", "LEVEL"
        };
    }
    else if (m_name == VC_MEMBER) {
        disallowed = {
                "LANGUAGE", "TYPE", "CALSCALE", "SORT-AS", "GEO", "TZ", "LABEL", "CC",
                "LEVEL"
        };
    }
    else if (m_name == VC_NAME) {
        disallowed = {
                "PREF", "PID", "TYPE", "MEDIATYPE", "CALSCALE", "SORT-AS", "GEO", "TZ",
                "LABEL", "CC", "INDEX", "LEVEL"
        };
    }
    else if (m_name == VC_NICKNAME) {
        disallowed = {
                "MEDIATYPE", "CALSCALE", "SORT-AS", "GEO", "TZ", "LABEL", "CC", "LEVEL"
        };
    }
    else if (m_name == VC_NOTE) {
        disallowed = {
                "MEDIATYPE", "CALSCALE", "SORT-AS", "GEO", "TZ", "LABEL", "CC", "LEVEL"
        };
    }
    else if (m_name == VC_ORGANIZATION) {
        disallowed = {
                "MEDIATYPE", "CALSCALE", "GEO", "TZ", "LABEL", "CC", "LEVEL"
        };
    }
    else if (m_name == VC_ORG_DIRECTORY) {
        disallowed = {
                "VALUE", "MEDIATYPE", "CALSCALE", "SORT-AS", "GEO", "TZ", "LABEL", "CC",
                "LEVEL"
        };
    }
    else if (m_name == VC_PHOTO) {
        disallowed = {
                "LANGUAGE", "CALSCALE", "SORT-AS", "GEO", "TZ", "LABEL", "CC", "LEVEL"
        };
    }
    else if (m_name == VC_PRODUCT_IDENTIFIER) {
        disallowed = {
                "LANGUAGE", "PREF", "ALTID", "PID", "TYPE", "MEDIATYPE", "CALSCALE",
                "SORT-AS", "GEO", "TZ", "LABEL", "CC", "INDEX", "LEVEL"
        };
    }
    else if (m_name == VC_RELATED) {
        disallowed = {
                "CALSCALE", "SORT-AS", "GEO", "TZ", "LABEL", "CC", "LEVEL"
        };
    }
    else if (m_name == VC_REVISION) {
        disallowed = {
                "LANGUAGE", "PREF", "ALTID", "PID", "TYPE", "MEDIATYPE", "CALSCALE",
                "SORT-AS", "GEO", "TZ", "LABEL", "CC", "INDEX", "LEVEL"
        };
    }
    else if (m_name == VC_ROLE) {
        disallowed = {
                "MEDIATYPE", "CALSCALE", "SORT-AS", "GEO", "TZ", "LABEL", "CC", "INDEX",
                "LEVEL"
        };
    }
    else if (m_name == VC_SOUND) {
        disallowed = {
                "CALSCALE", "SORT-AS", "GEO", "TZ", "LABEL", "CC", "LEVEL"
        };
    }
    else if (m_name == VC_SOURCE) {
        disallowed = {
                "LANGUAGE", "PID", "TYPE", "CALSCALE", "SORT-AS", "GEO", "TZ", "LABEL",
                "CC", "LEVEL"
        };
    }
    else if (m_name == VC_TELEPHONE) {
        disallowed = {
                "LANGUAGE", "CALSCALE", "SORT-AS", "GEO", "TZ", "LABEL", "CC", "LEVEL"
        };
    }
    else if (m_name == VC_TIME_ZONE) {
        disallowed = {
                "LANGUAGE", "CALSCALE", "SORT-AS", "GEO", "TZ", "LABEL", "CC", "LEVEL"
        };
    }
    else if (m_name == VC_TITLE) {
        disallowed = {
                "MEDIATYPE", "CALSCALE", "SORT-AS", "GEO", "TZ", "LABEL", "CC", "LEVEL"
        };
    }
    else if (m_name == VC_UID) {
        disallowed = {
                "LANGUAGE", "PREF", "ALTID", "PID", "TYPE", "MEDIATYPE", "CALSCALE",
                "SORT-AS", "GEO", "TZ", "LABEL", "CC", "INDEX", "LEVEL"
        };
    }
    else if (m_name == VC_URL) {
        disallowed = {
                "LANGUAGE", "CALSCALE", "SORT-AS", "GEO", "TZ", "LABEL", "CC", "LEVEL"
        };
    }
    else if (m_name == VC_VERSION) {
        disallowed = {
                "LANGUAGE", "PREF", "ALTID", "PID", "TYPE", "MEDIATYPE", "CALSCALE",
                "SORT-AS", "GEO", "TZ", "LABEL", "CC", "INDEX", "LEVEL"
        };
    }
    else if (m_name == VC_XML) {
        disallowed = {
                "LANGUAGE", "PREF", "PID", "TYPE", "MEDIATYPE", "CALSCALE", "SORT-AS",
                "GEO", "TZ", "LABEL", "CC", "INDEX", "LEVEL"
        };
    }

    for (const auto& s : disallowed) {
        if (params.contains(s)) {
            isValid = false;
        }
    }

    return isValid;
}

