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

#ifndef CONTACTS_COMMANDER_VCARD_H
#define CONTACTS_COMMANDER_VCARD_H

#include <string>
#include <vector>
#include <map>

#define VC_SEPARATOR_TOKEN ';'
#define VC_END_LINE_TOKEN "\r\n"
#define VC_BEGIN_TOKEN "BEGIN:VCARD"
#define VC_END_TOKEN "END:VCARD"
#define VC_ASSIGNMENT_TOKEN ':'
#define VC_TYPE_SEP_TOKEN '='

enum vCardVersion
{
    VC_VER_2_1,
    VC_VER_3_0,
    VC_VER_4_0
};

#define VC_ADDRESS              "ADR"
#define VC_AGENT                "AGENT"
#define VC_BIRTHDAY             "BDAY"
#define VC_CATEGORIES           "CATEGORIES"
#define VC_CLASS                "CLASS"
#define VC_DELIVERY_LABEL       "LABEL"
#define VC_EMAIL                "EMAIL"
#define VC_FORMATTED_NAME       "FN"
#define VC_GEOGRAPHIC_POSITION  "GEO"
#define VC_KEY                  "KEY"
#define VC_LOGO                 "LOGO"
#define VC_MAILER               "MAILER"
#define VC_NAME                 "N"
#define VC_NICKNAME             "NICKNAME"
#define VC_NOTE                 "NOTE"
#define VC_ORGANIZATION         "ORG"
#define VC_PHOTO                "PHOTO"
#define VC_PRODUCT_IDENTIFIER   "PRODID"
#define VC_REVISION             "REV"
#define VC_ROLE                 "ROLE"
#define VC_SORT_STRING          "SORT-STRING"
#define VC_SOUND                "SOUND"
#define VC_TELEPHONE            "TEL"
#define VC_TIME_ZONE            "TZ"
#define VC_TITLE                "TITLE"
#define VC_URL                  "URL"
#define VC_VERSION              "VERSION"

typedef std::multimap<std::string, std::string>::iterator param_iterator;

/**
 * @brief Property parameters holder. We can have multiple parameters with the same name
 * Based on std::multimap
 */
class vCardParamMap {
public:
    vCardParamMap() {}
    ~vCardParamMap() {}

    /**
     * Returns the total number of parameters
     */
    int size() const { return m_params.size(); }

    bool is_empty() const { return m_params.empty(); }
    void clear() { m_params.clear(); }

    /**
     * Returns the number of parameters with specified name
     * @param name The name of the parameter
     * @return The count of parameters
     */
    int count(std::string name) const { return m_params.count(name); }

    param_iterator begin() {return m_params.begin(); }
    param_iterator end() {return m_params.end(); }

    void addParam(std::string name, std::string value);
    void setParam(std::string name, std::string value);
    void removeParam(std::string name);

    /**
     * We can have multiple parameters with the same name, so return
     * begin_iterator and end_iterator for the range of parameters
     * @param name The name of the parameter
     * @return std::pair of begin and end iterators
     */
    std::pair<param_iterator, param_iterator> getParams(std::string name);

    /**
     * We can have multiple parameters with the same name, so [] operator returns
     * first found parameter's value with specified name
     * @param name The name of the parameter
     * @return string value of the parameter
     * @throws std::runtime_error Is thrown if parameter not found
     */
    std::string& operator[] (std::string name);

protected:
    std::multimap<std::string, std::string> m_params;
};

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
    vCardProperty(const std::string& name, const std::vector<std::string>& values): m_name(name), m_values(values) {}
    vCardProperty(const std::string& name, const std::vector<std::string>& values, const vCardParamMap params):
                    m_name(name), m_values(values), m_params(params) {}
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
    std::vector<std::string>& values() {return m_values; }

    void setParams(vCardParamMap params);
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
};

/**
 * @brief vCard object
 */
class vCard {
public:
    vCard(vCardVersion v = VC_VER_4_0): m_version(v)  {}
    ~vCard() {}

    /**
     * Returns the number of properties
     */
    int count() const { return m_properties.size(); };

    void addProperty(const vCardProperty& property);

    /**
     * Removes all found properties with the specified name
     */
    void removeProperty(std::string name);

    std::vector<vCardProperty>& properties() { return m_properties; }

    vCardVersion getVersion() { return m_version; }
    std::string getVersionStr();
    void setVersion(vCardVersion v) { m_version = v; }

    /**
     * Returns first found property with the specified name
     */
    vCardProperty& operator[] (std::string name);

    /**
     * Shortcut for adding property to the vCard
     */
    vCard & operator << (const vCardProperty &p);

protected:
    std::vector<vCardProperty> m_properties;
    vCardVersion m_version;
};


#endif //CONTACTS_COMMANDER_VCARD_H
