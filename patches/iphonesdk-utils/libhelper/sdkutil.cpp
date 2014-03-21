#include "helper.h"
#include <string>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <strings.h>

using namespace std;

string get_sdk_full_path()
{
 enterpath:
  
  cout<<"Please input your iPhone SDK absolute path : ";
  string sdk_fullpath;
  cin>>sdk_fullpath;
  if(!sdk_fullpath.empty()) {
    //I do not understand what you input.
    if(::access(sdk_fullpath.c_str(),R_OK) != 0) {
      cout<<RED "Wrong iPhone SDK path, Please enter the SDK path again!" NONE<<endl;
      goto enterpath;
    }
    //not absolute path
    if(!beginWith(sdk_fullpath, "/")) {
      cout<<RED "Wrong iPhone SDK path, Please enter the SDK path again!" NONE<<endl;
      goto enterpath;
    }

    //is dir?
    struct stat st;
    stat(sdk_fullpath.c_str(), &st);
    if(S_ISDIR(st.st_mode)) {
      //SDKSettings.plist should always be exists.
      //otherwise, this is must be wrong.
      string sdk_file = sdk_fullpath + "/SDKSettings.plist";
      if(::access(sdk_file.c_str(),R_OK) == 0)
        return sdk_fullpath;
      else {
        cout<<RED "Wrong iPhone SDK path, " GREEN "SDKSettings.plist" RED " not exists"<<endl 
            <<"Please enter the SDK path again!" NONE<<endl;
        goto enterpath;
      }
    }
  }
  return "";//never here, just for avoid warning.
}

string get_sdk_version_from_sdk_file(string sdk_file)
{

  char version[10];
  bzero(version,10);
  if(get_value_of_key_from_plist(version, "Version", sdk_file.c_str()))
    return version;
  else
    return "";
}


void detect_sdk_and_write_configfile(string configfile)
{
    string sdk_fullpath;
    string version;

    cout << LIGHT_BLUE "iPhone SDK Setup" NONE <<endl;
    // (2) detect iPhoneSDK
    cout << "\nThis is the first time you use iPhone toolchain for linux." <<endl;
    cout << "\n" <<endl;

    sdk_fullpath = find_more_dirs_in_dir("/usr/share", "iPhoneOS", ".sdk");
    if(sdk_fullpath.empty())
      sdk_fullpath = get_sdk_full_path();
    else
      sdk_fullpath = "/usr/share/" + sdk_fullpath;

    string sdk_file = sdk_fullpath + "/SDKSettings.plist";

    version = get_sdk_version_from_sdk_file(sdk_file);
    if(!version.empty())
      cout<<GREEN "Find \"iPhoneOS"<<version<<".sdk\" in "
      << "\""+sdk_fullpath+"\"" << NONE
      << endl;
    else
      version = "5.0";

    ofstream config(configfile.c_str());
    config << "SDK_FULL_PATH=" << sdk_fullpath <<endl;
    config << "SDK_VERSION=" << version <<endl;
    config.close();
}

string read_sdkpath_from_configfile(string configfile)
{
    string sdk_fullpath;
    ifstream fin(configfile.c_str());
    string line;
    while(getline(fin,line)) {
      if(beginWith(trim(line),"SDK_FULL_PATH="))
            sdk_fullpath = trim(line.substr(14,line.length()));
    }
    fin.close();
    return sdk_fullpath;
}

string read_sdkversion_from_configfile(string configfile)
{
    string version;
    ifstream fin(configfile.c_str());
    string line;
    while(getline(fin,line)) {
      if(beginWith(trim(line), "SDK_VERSION="))
            version = trim(line.substr(12,line.length()));
    }
    fin.close();
    return version;
}


