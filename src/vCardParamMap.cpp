//
// Created by renier on 22/04/23.
//

#include <vector>
#include "vCardParamMap.h"
#include "utils.h"

bool vCardParamMap::contains(std::string name) {
    bool found = false;

    for (const auto& p : m_params) {
        if (p.first == name) {
            found = true;
            break;
        }
    }

    return found;
}

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
