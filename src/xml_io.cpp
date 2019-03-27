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

#include "xml_io.h"

XmlWriter & XmlWriter::operator << (vCard & vCard)
{
    return *this;
}

XmlWriter & XmlWriter::operator << (std::vector<vCard> & cards)
{
    return *this;
}

XmlWriter & XmlWriter::operator << (vCardProperty & prop)
{
    return *this;
}

XmlWriter & XmlWriter::operator << (vCardParamMap & p)
{
    if(p.size() == 0)
        return *this;

    *m_os << "<parameters>";
    auto it = p.begin();
    while(it != p.end()){
        *m_os << "<" << it->first << ">";
        *m_os << "<text>" << it->second << "</text>";
        *m_os << "</" << it->first << ">";
        it++;
    }

    *m_os << "</parameters>";

    return *this;
}