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
#include "vCardProperty.h"

enum vCardVersion
{
    VC_VER_2_1,
    VC_VER_3_0,
    VC_VER_4_0
};


typedef std::vector<vCardProperty>::iterator prop_iterator;

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

    //iterator
    prop_iterator begin() { return m_properties.begin(); }
    prop_iterator end() { return m_properties.end(); }

    /**
     * Returns first found property with the specified name
     */
    vCardProperty& operator[] (std::string name);

    vCardProperty& operator[] (int index);
    /**
     * Shortcut for adding property to the vCard
     */
    vCard & operator << (const vCardProperty &p);

protected:
    std::vector<vCardProperty> m_properties;
    vCardVersion m_version;

    bool checkCardinality(vCardProperty prop);
};


#endif //CONTACTS_COMMANDER_VCARD_H
