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

#ifndef VCARD_JSON_IO_H
#define VCARD_JSON_IO_H

#include <istream>
#include "vcard.h"

class JsonWriter {
public:
    JsonWriter(std::ostream& os /*, vCvCardEncoding code = UTF_8*/): m_os(&os) {}
    ~JsonWriter() {}

    JsonWriter & operator << (vCard & vCard);
    JsonWriter & operator << (std::vector<vCard> & cards);
    JsonWriter & operator << (vCardProperty & prop);
    JsonWriter & operator << (vCardParamMap & param);

protected:
    std::ostream *m_os;

    std::string get_property_type(std::string property_name);
};

class JsonReader {
public:
    JsonReader() {}
    ~JsonReader() {}

    std::vector<vCard> parseCards(std::istream *is);
};

#endif //VCARD_JSON_IO_H
