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
#include <algorithm>
#include "vcard.h"

vCardProperty& vCard::operator[] (std::string name) {
    for(vCardProperty& p: m_properties) {
        if (p.getName() == name)
            return p;
    }

    throw std::runtime_error("Property not found");
}

vCardProperty& vCard::operator[](int index) {
    if (index > m_properties.size() || index < 0) {
        throw std::runtime_error("Index outside bounds of array");
    }

    return m_properties[index];
}

vCard & vCard::operator << (const vCardProperty &p)
{
    if (!checkCardinality(p)) {
        throw std::invalid_argument("property already present");
    }

    m_properties.push_back(p);
    return *this;
}

void vCard::addProperty(const vCardProperty& property)
{
    if (!checkCardinality(property)) {
        throw std::invalid_argument("property already present");
    }
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

bool vCard::checkCardinality(vCardProperty prop) {
    bool isValid = true;

    std::vector<std::string> noMultiples = {
            VC_ANNIVERSARY, VC_BIRTHDAY, VC_GENDER, VC_KIND, VC_NAME,
            VC_PRODUCT_IDENTIFIER, VC_REVISION, VC_UID, VC_FORMATTED_NAME
    };

    // check if prop can have multiples
    if (std::find(noMultiples.begin(), noMultiples.end(), prop.getName()) != noMultiples.end()) {
        // check if it's already present and if they have altids
        bool isPresent = false,
            hasAltid;
        for (auto & p : m_properties) {
            if (p.getName() == prop.getName()) {
                isPresent = true;
                hasAltid = p.hasAltId();
            }
        }
        if (isPresent && (!hasAltid || !prop.hasAltId())) {
           isValid = false;
        }
    }

    return isValid;
}
