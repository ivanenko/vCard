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

#ifndef VCARD_TEXT_IO_H
#define VCARD_TEXT_IO_H

#include <iosfwd>
#include <istream>
#include "vcard.h"

enum vCardEncoding {
    UTF_8 = 0,
    UTF_16
};

class TextReader {
public:
    TextReader(std::istream& is, vCardEncoding code = UTF_8): m_is(&is) {}
    ~TextReader() {}

    static vCardParamMap parseParams(std::string line);
    static vCardProperty parseProperty(std::string line);
    std::vector<vCard> parseCards();

    TextReader & operator >> (vCard & vCard);
    TextReader & operator >> (std::vector<vCard> & cards);
    //TextReader & operator >> (vCardProperty & prop);
    //TextReader & operator >> (vCardParamMap & param);

protected:
    std::istream *m_is;
};

class TextWriter {
public:
    TextWriter(std::ostream& os, vCardEncoding code = UTF_8): m_os(&os) {}
    ~TextWriter() {}

    TextWriter & operator << (vCard & vCard);
    TextWriter & operator << (std::vector<vCard> & cards);
    TextWriter & operator << (vCardProperty & prop);
    TextWriter & operator << (vCardParamMap & param);

protected:
    std::ostream *m_os;
};

#endif //VCARD_TEXT_IO_H
