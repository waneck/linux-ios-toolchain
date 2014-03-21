#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <libxml/parser.h> 
#include <libxml/tree.h> 


#include "helper.h"

using namespace std;

//default arch to armv7
string default_arch = "armv7";
//config file location 

extern char ** environ;


int main(int argc, char **argv)
{

  string config_file = string(getenv("HOME")) + "/.iphonesdk";

  // (1) detect toolchain.
  string clang = find_command("clang","clang","clang");
  string ldid = find_command("ldid","ldid","ldid");
  string as = find_command("arm-apple-darwin9-as","arm-apple","as");
  string target = as.substr(as.find("arm"), as.length()-3-as.find("arm"));

  
  if(clang.empty() || ldid.empty() || as.empty()) {
    cout <<"Can not find proper toolchain commands."<<endl;
    cout <<"You may need install clang, ldid, cctools" <<endl;
    cout <<"And setup PATH environment variable according to your installation" <<endl;
    exit(0);
  }


  string sdk_fullpath = "/usr/share/iPhoneOS5.0.sdk";
  string version = "5.0";

  int should_init = 0;
  if(endWith(string(argv[0]),"wrapper"))
    should_init = 1;

  if(endWith(string(argv[0]),"ios-switchsdk"))
    should_init = 1;
  //if ~/.iphonesdk not exists, detect it.  
  if(::access(config_file.c_str(),R_OK) != 0 || should_init)
    detect_sdk_and_write_configfile(config_file);

  sdk_fullpath = read_sdkpath_from_configfile(config_file);
  version = read_sdkversion_from_configfile(config_file);

  //check sdk path.
  if(::access(sdk_fullpath.c_str(), R_OK) != 0) {
    detect_sdk_and_write_configfile(config_file);
    sdk_fullpath = read_sdkpath_from_configfile(config_file);
    version = read_sdkversion_from_configfile(config_file);
  }


  // for SDK 4.x, set default arch to armv6
  if(beginWith(version,"4."))
       default_arch = "armv6";
    
  string command = "clang";
  string caller = argv[0];
  if(endWith(caller,"ios-clang"))
    command = clang;
  else if(endWith(caller,"ios-clang++"))
    command = clang + "++";
 
  //look in argv, if -arch had been setted, just use the settings.
  for(int i = 0; i < argc; i++) {
    if(strcmp(argv[i],"-arch") == 0 && (i+1) < argc) {
        string arch = argv[i+1];
        if(beginWith(arch,"arm"))
            default_arch = arch;
        break;
    }
  } 
     
  // cmd args for execvpe;
  char **cmd = (char **)malloc((8 + argc)*sizeof(char*));
  cmd[0] = strdup(command.c_str());
  cmd[1] = strdup("-target");
  cmd[2] = strdup(target.c_str());
  cmd[3] = strdup("-arch");
  cmd[4] = strdup(default_arch.c_str());
  cmd[5] = strdup("-isysroot");
  cmd[6] = strdup(sdk_fullpath.c_str());
  cmd[7] = strdup("-mlinker-version=134.9");
  for (int i = 1; i < argc; i ++) {
    cmd[7+i] = argv[i];
  }
  cmd[7+argc] = (char *)0;

  // env for execvpe
  int count = 0;
  for(int i = 0; environ[i] != NULL; i++ ) {
    count++;
  }

  char ** env_l = (char **)malloc((count+2) * sizeof(char*));
  for(int i = 0; environ[i] != NULL; i++ ) {
    env_l[i] = environ[i];
  }
  string iphone_env = "IPHONEOS_DEPLOYMENT_TARGET=" + version;
  string sign_code_when_build = "IOS_SIGN_CODE_WHEN_BUILD=1";
  env_l[count] = (strdup(iphone_env.c_str()));
  env_l[count+1] = (strdup(sign_code_when_build.c_str()));
  env_l[count+2] = (char *)0;

  //run command.
  execvpe(command.c_str(), cmd, env_l);
  return 0;
}
