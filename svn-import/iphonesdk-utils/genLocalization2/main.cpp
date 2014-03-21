#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>

#include "getLocalizedStringFromFile.h"
#include "plist/plist.h"
#include "helper.h"

using namespace std;

void usage()
{
  cout <<BLUE "ios-genLocalization" NONE ": iOS App localization tool based on clang lexer" <<endl
       <<"By cjacker <cjacker@gmail.com> 2012"<<endl
       <<RED "\nUsage:" NONE<<endl
       <<"\tRun 'ios-genLocalization' in Project folder>"<<endl;
  exit(0);
}

vector<string> alldirs;

vector<string> allfiles;

void scan_dir_for_sources(string dirpath)
{
    
  DIR *dirptr=NULL;
  struct dirent *entry;
  if((dirptr = opendir(dirpath.c_str()))==NULL)
    {
      printf("opendir failed!");
      return;
    }
  while((entry=readdir(dirptr)))
    {
      string filename = entry->d_name;
      if(filename == "." || filename == "..")
        continue;
      filename = dirpath +"/"+filename;
      struct stat st;
      stat(filename.c_str(), &st);
      if(S_ISDIR(st.st_mode))
	scan_dir_for_sources(filename);
      if(endWith(filename,".m") || endWith(filename,".mm")) {
        allfiles.push_back(filename);
      }
    }
  closedir(dirptr); 
}


void scan_dir_for_dir(string dirpath)
{

  DIR *dirptr=NULL;
  struct dirent *entry;
  if((dirptr = opendir(dirpath.c_str()))==NULL)
    {
      printf("opendir failed!");
      return;
    }
  while((entry=readdir(dirptr)))
    {
      string filename = entry->d_name;
      if(filename == "." || filename == "..")
        continue;
      filename = dirpath +"/"+filename;
      struct stat st;
      stat(filename.c_str(), &st);
      if(S_ISDIR(st.st_mode)) {
	alldirs.push_back(filename);
	scan_dir_for_dir(filename);
      }
    }
  closedir(dirptr);
}

string config_file = string(getenv("HOME")) + "/.iphonesdk";

vector<string> getAllTranslatableString()
{
  string as = find_command("arm-apple-darwin11-as","arm-apple","as");

  if(as.empty()) {
    cout <<"Can not find proper toolchain commands."<<endl;
    cout <<"You may need install cctools" <<endl;
    cout <<"And setup PATH environment variable according to your installation" <<endl;
    exit(0);
  }

  string sdk_fullpath = "/usr/share/iPhoneOS5.0.sdk";
  //if ~/.iphonesdk not exists, detect it.  
  if(::access(config_file.c_str(),R_OK) != 0 )
    detect_sdk_and_write_configfile(config_file);

  sdk_fullpath = read_sdkpath_from_configfile(config_file);
  //check sdk path.
  if(::access(sdk_fullpath.c_str(), R_OK) != 0) {
    detect_sdk_and_write_configfile(config_file);
    sdk_fullpath = read_sdkpath_from_configfile(config_file);
  }


  string standard_include = sdk_fullpath + "/usr/include";   

  string cxxinclude = find_dir_in_dir(standard_include + "/c++","4.2.1" , "4", "");
  cxxinclude = standard_include + "/c++/" + cxxinclude;

  string cxxinclude1 = find_dir_in_dir(cxxinclude,"tr1" , "tr", "");
  cxxinclude1 = cxxinclude + "/" + cxxinclude1;
  
  string tripleinclude = find_dir_in_dir(cxxinclude, "arm-apple-darwin11", "armv", "");
  tripleinclude = cxxinclude + "/" + tripleinclude; 
 
  string triple = as.substr(as.find("arm"), as.length()-3-as.find("arm"));

  vector<string> includepaths;
  includepaths.push_back(standard_include);
  includepaths.push_back(cxxinclude);
  includepaths.push_back(cxxinclude1);
  includepaths.push_back(tripleinclude);

  //local headers in local dirs.
  scan_dir_for_dir(".");
  for(int i = 0; i < alldirs.size(); i++)
    includepaths.push_back(alldirs.at(i));
  includepaths.push_back(".");
  //find all sourcefiles. 
  scan_dir_for_sources(".");

  vector<string> allstrs;  
  for(int i = 0; i < allfiles.size(); i++) {
    vector<string> trans_list = getLocalizedStringFromFile(allfiles.at(i), triple, includepaths);
    for(int i = 0; i < trans_list.size(); i++)
      if(!trans_list.at(i).empty())
        allstrs.push_back(trans_list.at(i));
  }
  return allstrs;
}

void handleInfoPlist(string output)
{
  string project_info_plist = find_file_in_dir(".", "Info.plist", "", "Info.plist");
  project_info_plist = "./" + project_info_plist;
  if(!project_info_plist.empty()) {
    char *plist_info = NULL;
    uint32_t size_out = 0;
    plist_t pl_dict = plist_new_dict();
    char bundletype[10];
    bzero(bundletype,10);
    get_value_of_key_from_plist(bundletype, "CFBundlePackageType", project_info_plist.c_str());
    char cfbundlename[10];
    char cfbundledisplayname[10];
    bzero(cfbundlename,10);
    bzero(cfbundledisplayname, 10);
    if(get_value_of_key_from_plist(cfbundlename, "CFBundleName", project_info_plist.c_str()) &&
       get_value_of_key_from_plist(cfbundledisplayname, "CFBundleDisplayName", project_info_plist.c_str())) {
       if(strcmp(bundletype,"APPL")==0) {// normal package.
         plist_t name = plist_new_string(cfbundlename);
         plist_dict_insert_item(pl_dict, "CFBundleName", name);
         plist_t dname = plist_new_string(cfbundledisplayname);
         plist_dict_insert_item(pl_dict, "CFBundleDisplayName", dname);
       } else { //BNDL.
         plist_t dname = plist_new_string(cfbundledisplayname);
         plist_dict_insert_item(pl_dict, cfbundledisplayname, dname);
       }
    }
    plist_to_xml(pl_dict, &plist_info, &size_out);
    FILE *f = fopen(output.c_str(), "w");
    fprintf(f, "%s\n", plist_info);
    fclose(f);
  }
}

void handleStrings(vector<string> strings, string output)
{
  char *plist_xml = NULL;
  uint32_t size_out = 0;
  plist_t pl_dict= plist_new_dict();

  for(int i = 0; i < strings.size(); i++) {
    plist_t string = plist_new_string(strings.at(i).c_str());
    plist_dict_insert_item(pl_dict, strings.at(i).c_str(), string);
  }
  plist_to_xml(pl_dict, &plist_xml, &size_out);
  FILE *f = fopen(output.c_str(), "w");
  fprintf(f, "%s\n", plist_xml);
  fclose(f);
}

const int lang_count = 9;
string lang[] = {"English", "zh_CN", "zh_TW", "ko", "Japanese", "German", "French", "Italian", "Spanish"};
string lang_desc[] = {"English", "Simp. Chinese", "Trad. Chinese", "Korean", "Japanese", "German", "French", "Italian", "Spanish"};

int main(int argc, char **argv)
{
  if(argc > 1)
    usage();

  cout << "Localization tool for iOs App" <<endl<<endl
       << "Languages" <<endl;
  for(int i = 0; i < lang_count; i++) {
    printf("[%2d.] %s\t(%s)\n", i, lang[i].c_str(), lang_desc[i].c_str());
  }
  short input;
  do 
  { 
        cout<<"Enter your number : "; 
        cin>>input; 
        if (cin.fail())
        {
            cin.clear();
            cin.sync();
        }
 
  } while(input > 8 || input < 0);
  string localization = "./Localizations";
  string lang_choice = localization +"/"+lang[input] + ".lproj";
  
  mkdir(localization.c_str(),0755); 
  mkdir(lang_choice.c_str(),0755);

  string info_output = lang_choice + "/InfoPlist.strings";
  string string_output = lang_choice + "/Localizable.strings";
 
  handleInfoPlist(info_output);
  handleStrings(getAllTranslatableString(), string_output);
  cout <<"Finished, below files need to be translated" <<endl
       <<info_output<<endl
       <<string_output<<endl;
}

