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

#include <string>
#include <vector>

std::vector<std::string> split(const std::string s, char separator, bool skip_slashed)
{
    // "\\" / "\," / "\;" / "\n" - ;, Backslashes, commas, and newlines must be encoded.
    std::vector<std::string> result;

    int prev_pos = 0, pos = 0;
    while(pos <= s.size()){
        if(skip_slashed)
            if((pos+1)<=s.size() && s[pos] == '\\' && s[pos+1] == separator){
                pos += 2;
                continue;
            }

        if(s[pos] == separator){
            result.push_back(s.substr(prev_pos, pos-prev_pos));
            prev_pos = pos+1;
        }
        pos++;
    }

    result.push_back(s.substr(prev_pos, pos-prev_pos));

    return result;
}



