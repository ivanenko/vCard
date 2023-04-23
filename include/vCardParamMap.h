//
// Created by renier on 22/04/23.
//

#ifndef VCARD_VCARDPARAMMAP_H
#define VCARD_VCARDPARAMMAP_H

#include <map>
#include <string>

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

    param_iterator begin() { return m_params.begin(); }
    param_iterator end() { return m_params.end(); }

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

#endif //VCARD_VCARDPARAMMAP_H
