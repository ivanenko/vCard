//
// Created by renier on 22/04/23.
//

#ifndef VCARD_VCARDPROPERTY_H
#define VCARD_VCARDPROPERTY_H

#include <vector>
#include <string>
#include "vCardParamMap.h"

#define VC_SEPARATOR_TOKEN ';'
#define VC_END_LINE_TOKEN "\r\n"
#define VC_BEGIN_TOKEN "BEGIN:VCARD"
#define VC_END_TOKEN "END:VCARD"
#define VC_ASSIGNMENT_TOKEN ':'
#define VC_TYPE_SEP_TOKEN '='

#define VC_ADDRESS              "ADR"
#define VC_ANNIVERSARY          "ANNIVERSARY"
#define VC_BIRTHDAY             "BDAY"
#define VC_BIRTHPLACE           "BIRTHPLACE"
#define VC_CALURI               "CALURI"
#define VC_CALADRURI            "CALADRURI"
#define VC_CATEGORIES           "CATEGORIES"
#define VC_CONTACT_URI          "CONTACT-URI"
#define VC_CLIENT_PID_MAP       "CLIENTPIDMAP"
#define VC_DEATH_DATE           "DEATHDATE"
#define VC_DEATH_PLACE          "DEATHPLACE"
#define VC_EMAIL                "EMAIL"
#define VC_EXPERTISE            "EXPERTISE"
#define VC_FORMATTED_NAME       "FN"
#define VC_FREEBUSY_URL         "FBURL"
#define VC_GENDER               "GENDER"
#define VC_GEOGRAPHIC_POSITION  "GEO"
#define VC_HOBBY                "HOBBY"
#define VC_IMPP                 "IMPP"
#define VC_INTEREST             "INTEREST"
#define VC_KEY                  "KEY"
#define VC_KIND                 "KIND"
#define VC_LANG                 "LANG"
#define VC_LOGO                 "LOGO"
#define VC_MEMBER               "MEMBER"
#define VC_NAME                 "N"
#define VC_NICKNAME             "NICKNAME"
#define VC_NOTE                 "NOTE"
#define VC_ORGANIZATION         "ORG"
#define VC_ORG_DIRECTORY        "ORG-DIRECTORY"
#define VC_PHOTO                "PHOTO"
#define VC_PRODUCT_IDENTIFIER   "PRODID"
#define VC_RELATED              "RELATED"
#define VC_REVISION             "REV"
#define VC_ROLE                 "ROLE"
#define VC_SOUND                "SOUND"
#define VC_SOURCE               "SOURCE"
#define VC_TELEPHONE            "TEL"
#define VC_TIME_ZONE            "TZ"
#define VC_TITLE                "TITLE"
#define VC_UID                  "UID"
#define VC_URL                  "URL"
#define VC_VERSION              "VERSION"
#define VC_XML                  "XML"

/**
 * @brief vCard line property
 */
class vCardProperty {
public:

    static const std::map<std::string, std::string> property_types;

    /// Used as indexes for ADR property values
    enum AddressFields
    {
        PostOfficeBox = 0,
        ExtendedAddress,
        Street,
        Locality,           // e.g. City.
        Region,             // e.g. State or province.
        PostalCode,
        Country
    };

    /// Used as indexes for N property values
    enum NameFields { Lastname = 0, Firstname, Additional, Prefix, Suffix };

    /// Used as indexes for GEO property values
    enum GeoPositionFields { Latitude = 0, Longitude };

    vCardProperty() {}
    vCardProperty(const std::string& group, const std::string& name, const std::string& value);
    vCardProperty(const std::string& name, const std::string& value);
    vCardProperty(const std::string& name, const std::vector<std::string>& values);
    vCardProperty(const std::string& name, const std::vector<std::string>& values, const vCardParamMap params);
    ~vCardProperty() {}

    std::string& getName() { return m_name; }
    std::string& getGroup() { return m_group; }

    /**
     * Returns joined values list in one string
     */
    std::string getValue();

    /**
     * Returns list of string values
     */
    std::vector<std::string>& values() { return m_values; }

    void setParams(vCardParamMap params);
    vCardParamMap getParams() { return m_params; }

    [[deprecated("Method bypasses validation")]]
    vCardParamMap& params() { return m_params; }

    /**
     * Shortcut for adding parameters to property
     * Example: prop << parameters;
     */
    vCardProperty & operator << (const vCardParamMap &p);

    /// Static property creators

    static vCardProperty createAddress(const std::string& street, const std::string& locality,
                                       const std::string& region, const std::string& postal_code, const std::string& country,
                                       const std::string& post_office_box = "", const std::string& ext_address = "",
                                       const vCardParamMap& params = vCardParamMap());
    static vCardProperty createBirthday(const int year, const int month, const int day);
    static vCardProperty createName(const std::string& firstname, const std::string& lastname,
                                    const std::string& additional = "", const std::string& prefix = "",
                                    const std::string& suffix = "", const vCardParamMap& params = vCardParamMap());
    static vCardProperty createOrganization(const std::string& name, const std::vector<std::string>& levels = std::vector<std::string>(), const vCardParamMap& params = vCardParamMap());

protected:
    std::string m_group;
    std::string m_name;
    std::vector<std::string> m_values;
    vCardParamMap m_params;

    bool isValidParamMap(vCardParamMap params);
};

#endif //VCARD_VCARDPROPERTY_H
