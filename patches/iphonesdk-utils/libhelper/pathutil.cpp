#include <string>
#include <dirent.h>
#include <stdio.h>
#include "helper.h"
//for stat()
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <iostream>

using namespace std;

vector<string> fillpath(void)
{
    vector<string> pathdir;
    char *key=NULL, *tok=NULL, *pathcp, *path = getenv("PATH");
    int i = 0;

    if (!path)
        return pathdir;
    pathcp = strdup(path);

    for (tok = strtok_r(pathcp, ":", &key); tok;
         tok = strtok_r(NULL, ":", &key)) {
         pathdir.push_back(strdup(tok));
    }
    free(pathcp);
    return pathdir;
}


string find_cmd_in_dir(string dir, string begin, string end)
{
    string target_file;
    DIR *dirptr=NULL;
    struct dirent *entry;
    if((dirptr = opendir(dir.c_str()))==NULL)
        return "";
    while(entry=readdir(dirptr))
    {
        string filename = entry->d_name;
        if(endWith(filename, end) && beginWith(filename, begin)) {
            target_file = filename;
            break;
        }
    }
    closedir(dirptr);
    return target_file;
}


string find_command(string cmd, string begin, string end)
{
    string cmd_fullpath; 
    
    vector<string> pathdir = fillpath();
    for(int i = 0 ; i < pathdir.size(); i++) {
        string cmd_fullpath = pathdir[i] + "/" + cmd;
        if(access(cmd_fullpath.c_str(), X_OK) == 0)
            return cmd_fullpath;
    }
   
    for(int i = 0 ; i < pathdir.size(); i++) {
        cmd_fullpath = find_cmd_in_dir(pathdir[i], begin, end);
        if(!cmd_fullpath.empty())
            return pathdir[i] +"/"+cmd_fullpath;
    }
    return cmd_fullpath;
}

string find_file_in_dir(string dir, string file, string begin, string end)
{
  string target_file;
  DIR *dirptr=NULL;
  struct dirent *entry;
  if((dirptr = opendir(dir.c_str()))==NULL)
    {
      return "";
    }
  while((entry=readdir(dirptr)))
    {
      string filename = entry->d_name;
      if(filename == file) {
    target_file = filename;
    break;
      }
      if(endWith(filename,end) && beginWith(filename, begin)) {
    target_file = filename;
    break;
      }
    }
  closedir(dirptr);
  return target_file;
}

string find_dir_in_dir(string dir, string dirname, string begin, string end)
{
  string target_file = find_file_in_dir(dir, dirname, begin, end);
  struct stat st;
  stat( (dir + "/" +target_file).c_str(), &st );

  if(!target_file.empty() && S_ISDIR(st.st_mode))
    return target_file;

  return "";
}

string find_more_dirs_in_dir(string dir, string begin, string end)
{
  vector<string> dirs;
  string target_file;
  DIR *dirptr=NULL;
  struct dirent *entry;
  if((dirptr = opendir(dir.c_str()))==NULL)
      return "";
  
  while((entry=readdir(dirptr))) {
    string filename = entry->d_name;
    if(endWith(filename,end) && beginWith(filename, begin)) 
      dirs.push_back(filename);
  }
  closedir(dirptr);  

  if(dirs.size() == 0) //notfound;
    return "";
  
  if(dirs.size() == 1)
    return dirs[0];
  
  printf("There are more than one sdks found:\n");
  for(int i = 0; i < dirs.size(); i++) 
    printf("[%i] %s\n", i, dirs[i].c_str());
  short input;
  do {
      cout <<"Please choose one:";
      cin>>input;
      if (cin.fail()) {
        cin.clear();
        cin.sync();
      }
  } while(input > dirs.size() || input < 0);
  return dirs[input]; 
}
