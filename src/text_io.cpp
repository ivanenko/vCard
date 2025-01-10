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

#include <algorithm>
#include "text_io.h"
#include "utils.h"

std::istream *safeGetline(std::istream *is, std::string &t)
{
    t.clear();

    // The characters in the stream are read one-by-one using a std::streambuf.
    // That is faster than reading them one-by-one using the std::istream.
    // Code that uses streambuf this way must be guarded by a sentry object.
    // The sentry object performs various tasks,
    // such as thread synchronization and updating the stream state.

    std::istream::sentry se(*is, true);
    std::streambuf *sb = is->rdbuf();

    for(;;)
    {
        int c = sb->sbumpc();
        switch(c)
        {
            case '\n':
                return is;
            case '\r':
                if(sb->sgetc() == '\n')
                {
                    sb->sbumpc();
                    // special case for vCard folding lines
                    if(sb->sgetc() == ' ' || sb->sgetc() == '\t' || sb->sgetc() == '=' || sb->sgetc() == ';')
                    {
                        sb->sbumpc();
                        continue;
                    }
                }

                return is;
            case std::streambuf::traits_type::eof():
                // Also handle the case when the last line has no line ending
                if(t.empty())
                    is->setstate(std::ios::eofbit);
                return is;
            default:
                t += (char) c;
        }
    }
}

vCardParamMap TextReader::parseParams(std::string input)
{
    vCardParamMap params;
    int i = 0, start_pos = 0;
    while(i < input.size())
    {
        // get params name
        while(input[i] != '=' && i < input.size())
            i++;

        std::string name = input.substr(start_pos, i - start_pos);
        i++; // skip '='
        start_pos = i;

        // get params values
        while(true)
        {
            while(i < input.size() && input[i] != ';' && input[i] != ',')
            {
                if(input[i] == '\"')
                {        // read everything inside ""
                    i++; // skip first '"'
                    start_pos = i;
                    while(input[i] != '\"' && i < input.size())
                        i++;

                    if(i >= input.size())
                        throw std::runtime_error("Parsing error: not closed DQUOTE");

                    break;
                }
                else
                {
                    i++;
                }
            }

            std::string value = input.substr(start_pos, i - start_pos);

            std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c) { return std::toupper(c); });
            if(name == "TYPE" || name == "SORT-AS")
            {
                std::vector<std::string> p = split(value, ',');
                for(auto &s : p)
                    params.addParam(name, s);
            }
            else
                params.addParam(name, value);

            if(input[i] == '\"')
                i++; // skip last '"'
            i++;     // skip ';' or ','
            start_pos = i;

            if(input[i - 1] == ';' || i >= input.size())
                break;
        }
    }

    return params;
}

vCardProperty TextReader::parseProperty(std::string line)
{
    // find first nonquoted ':', first ';' and first '.'
    int i = 0, colon_pos = -1, first_semicln_pos = -1, first_point_pos = -1;
    bool quoted = false;
    while(i < line.size())
    {
        if(line[i] == VC_ASSIGNMENT_TOKEN && !quoted)
            break;

        if(line[i] == ';' && first_semicln_pos < 0) // first ';' could not be quoted
            first_semicln_pos = i;

        if(line[i] == '.' && first_point_pos < 0)
            first_point_pos = i;

        if(line[i] == '\"')
            quoted = !quoted;

        i++;
    }

    colon_pos = i;
    if(colon_pos == line.size())
        throw std::runtime_error("Error parsing property, no ':' found");

    std::string group, name;
    vCardParamMap params;
    if(first_semicln_pos > 0 && first_semicln_pos < colon_pos)
    { // we have parameters in property
        if(first_point_pos > 0 && first_point_pos < first_semicln_pos)
        {
            group = line.substr(0, first_point_pos);
            name = line.substr(first_point_pos + 1, first_semicln_pos - first_point_pos - 1);
        }
        else
        {
            name = line.substr(0, first_semicln_pos);
        }

        if(name != VC_VERSION)
        {
            std::string params_string = line.substr(first_semicln_pos + 1, colon_pos - first_semicln_pos - 1);
            // Issue: Fix error on CELL or other single type.
            if(params_string.find("=") == std::string::npos)
            {
                constexpr auto TYPE_STR = "TYPE=";
                std::string validate {};
                i = 0;
                first_semicln_pos = 0;
                params_string+=';';
                while(i < params_string.length())
                {
                    if(params_string[i] == ';')
                    {
                        validate += TYPE_STR;
                        validate += params_string.substr(first_semicln_pos, i - first_semicln_pos);
                        first_semicln_pos=i+1;
                        validate += ';'; // Next
                    }
                    ++i;
                }

                params_string = validate;
            }

            params = TextReader::parseParams(params_string);
        }
    }
    else
    { // we do not have parameters
        if(first_point_pos > 0 && first_point_pos < colon_pos)
        {
            group = line.substr(0, first_point_pos);
            name = line.substr(first_point_pos + 1, colon_pos - first_point_pos);
        }
        else
        {
            name = line.substr(0, colon_pos);
        }
    }

    std::string value = line.substr(colon_pos + 1);
    vCardProperty prop(group, name, value);
    prop.setParams(params);
    return prop;
}

std::vector<vCard> TextReader::parseCards()
{
    std::vector<vCard> vcards;
    vCard current;
    bool started = false;
    bool trySkipContent = false;
    std::string line;
    while(!safeGetline(m_is, line)->eof())
    {
        if((line == VC_BEGIN_TOKEN) && !started)
        {
            started = true;
        }
        else if((line == VC_END_TOKEN) && started)
        {
            vcards.push_back(current);
            trySkipContent = false;
            started = false;
            // Empty the current card
            current = vCard();
        }
        else if((line.find("VERSION") != std::string::npos) && started)
        {
            size_t pos = line.find(":");
            if(pos != std::string::npos)
            {
                std::string version = line.substr(pos + 1);
                trim(version);
                if(version == "3.0")
                    current.setVersion(VC_VER_3_0);
                if(version == "2.1")
                    current.setVersion(VC_VER_2_1);
            }

            continue;
        }
        else if(started)
        {
            // Check data (PHOTO), (SOUND)
            if(line.find("X-CUSTOM") != std::string::npos || line.find("PHOTO") != std::string::npos || line.find("SOUND") != std::string::npos)
            {
                trySkipContent = true;
                continue;
            }
            if(line.empty() || trySkipContent && line.at(0) == ' ')
            {
                continue;
            }
            try
            {
                vCardProperty prop = TextReader::parseProperty(line);
                current.addProperty(prop);
            }
            catch(std::exception)
            {
                continue;
            }

            trySkipContent = false;
        }
    }

    return vcards;
}

TextReader &TextReader::operator>>(vCard &vCard)
{
    return *this;
}

TextReader &TextReader::operator>>(std::vector<vCard> &cards)
{
    return *this;
}

//============================================================================

TextWriter &TextWriter::operator<<(vCard &vCard)
{
    *m_os << VC_BEGIN_TOKEN << VC_END_LINE_TOKEN;
    *m_os << "VERSION:" << vCard.getVersionStr() << VC_END_LINE_TOKEN;

    for(auto it = vCard.properties().begin(); it != vCard.properties().end(); ++it)
    {
        // TODO make line folding (split lines with length > 75)
        *this << *it;
        *m_os << VC_END_LINE_TOKEN;
    }

    *m_os << VC_END_TOKEN << VC_END_LINE_TOKEN;
    return *this;
}

TextWriter &TextWriter::operator<<(std::vector<vCard> &cards)
{
    for(auto card : cards)
    {
        *this << card;
    }

    return *this;
}

TextWriter &TextWriter::operator<<(vCardProperty &p)
{
    if(!p.getGroup().empty())
        *m_os << p.getGroup() << '.';

    *m_os << p.getName();

    if(p.params().size() > 0)
    {
        *m_os << ';';
        *this << p.params();
    }

    *m_os << ':';

    auto it = p.values().begin();
    while(it != p.values().end())
    {
        *m_os << *it;

        it++;

        if(it != p.values().end())
            *m_os << ";";
    }

    return *this;
}

TextWriter &TextWriter::operator<<(vCardParamMap &p)
{
    auto it = p.begin();
    while(it != p.end())
    {
        *m_os << it->first << "=";
        size_t found = it->second.find_first_of(":;,");
        if(found == std::string::npos)
            *m_os << it->second;
        else
            *m_os << "\"" << it->second << "\"";

        it++;
        if(it != p.end())
            *m_os << ";";
    }

    return *this;
}
