#include "helper.h"
#include <string>

using namespace std;
string trim(string s) {
  if (s.length() == 0) return s;
  size_t beg = s.find_first_not_of(" \a\b\f\n\r\t\v");
  size_t end = s.find_last_not_of(" \a\b\f\n\r\t\v");
  if (beg == string::npos) return "";
  return string(s, beg, end - beg + 1);
}

bool beginWith(const string str,const string needle) {
        return (!str.compare(0,needle.length(),needle));
}

bool endWith(const string str,const string needle) {
   if (str.length() >= needle.length()) {
      return (0 == str.compare (str.length() - needle.length(), needle.length(), needle));
    }
    return false;
}

std::string m_replace(std::string str,std::string pattern,std::string dstPattern,int count)
{
    std::string retStr="";
    string::size_type pos;

    int szStr=str.length();
    int szPattern=pattern.size();
    int i=0;
    int l_count=0;
    if(-1 == count) // replace all
        count = szStr;

    for(i=0; i<szStr; i++)
    {
        pos=str.find(pattern,i);

        if(std::string::npos == pos)
            break;
        if(pos < szStr)
        {
            std::string s=str.substr(i,pos-i);
            retStr += s;
            retStr += dstPattern;
            i=pos+pattern.length()-1;
            if(++l_count >= count)
            {
                i++;
                break;
            }
        }
    }
    retStr += str.substr(i);
    return retStr;
}

