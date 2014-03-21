#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <list>
#include <sstream>

#include <stdio.h>
#include <libgen.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>

#include "libxcodeutils/pbxprojdef.h"

using namespace std;

//current supported build type.
enum targetType {
  APP,
  STATICLIB,
  EXEC,
  FRAMEWORK
};

enum buildPhase {
  SOURCES,
  HEADERS,
  FRAMEWORKS,
  RESOURCES
};
//this is a wrapper for system, if error happened when compilation, then exit.
void runCommand(const char * command)
{
  int ret = system(command);
  if(ret != 0)
    exit(1);
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

  for(i=0; i<szStr; i++) {
    pos=str.find(pattern,i);

    if(std::string::npos == pos)
      break;
    if(pos < szStr) {
      std::string s=str.substr(i,pos-i);
      retStr += s;
      retStr += dstPattern;
      i=pos+pattern.length()-1;
      if(++l_count >= count) {
	i++;
	break;
      }
    }
  }
  retStr += str.substr(i);
  return retStr;
}


class File
{
public:
  string isa;
  string lastKnownFileType;
  string name;
  string path;
  string cflag;
  int is_public_header;
};

class PBXNativeTarget
{
public:
  //from target;
  string name;
  string productName;
  string productType;

  //from buildPhases
  vector<File> sources;
  vector<File> headers;
  vector<File> headerpaths;
  vector<File> frameworks;
  vector<File> resources;

  //from productReference;
  string result;
  string resulttype;
  
  //from buildConfigurationList;
  string buildargs;
  string infoplist;

  //for frame work;
  string dylib_compatibility_version;
  string dylib_current_version;
};

class PBXProj {
public:
  PBXProj();
  void loadProj(string path);
  int getTargetCount();
  vector<PBXNativeTarget> getTargets();

private:
  //extract all files project contains to allFiles;
  //main porpose it to construct the currect pathes for every files.
  //since file reference did not contains any paht infomations.
  void getAllFilesFromMainGroup(const PBXBlock * block, string current_path);
  
  //get common build args from buildSettings.
  string getBuildSettings(const PBXBlock *block);
  //get productname from buildSettings.
  string getProductName(const PBXBlock *block);
  //get the infoplist file from buildSettings, 
  //especially for App.
  string getInfoPlist(const PBXBlock *block);
  //get dylib compatibility version and current version from buildSettings.
  //especially for framework build.
  string getDylibCompatibilityVersion(const PBXBlock *block);
  string getDylibCurrentVersion(const PBXBlock *block);

  //get every files from target buildPhases
  //and construct corrent member field of target class.
  void getFilesFromFileArray(const PBXArray *file_arr, buildPhase bptype, PBXNativeTarget *target);

  //start parse target.
  //called by constructor of PBXProj.loadProj();
  void initTargets();

  //contains targets count.
  int targetcount;

  //contains all targets.
  vector<PBXNativeTarget> targets;

  //contains all files in project.
  map<string,string> allFiles;
 
  PBXFile *pDoc;
};

PBXProj::PBXProj()
{
  pDoc = NULL;
  allFiles.clear();
  targetcount = 0;
  targets.clear();
}

void PBXProj::loadProj(string path)
{
  string projectDir(path);
  string projectFile(projectDir);
  loadProject(projectFile.c_str(), &pDoc);

  //loading all files in project when opened.
  const PBXValueRef* ref = dynamic_cast<const PBXValueRef*>(pDoc->valueForKeyPath("rootObject.mainGroup"));
  const PBXValue *value = pDoc->deref(ref);
  const PBXBlock *block = PBXBlock::cast(value);

  getAllFilesFromMainGroup(block, ".");
  initTargets();
}

int PBXProj::getTargetCount()
{
  return this->targetcount;
}

vector<PBXNativeTarget> PBXProj::getTargets()
{
  return this->targets;
}

void PBXProj::initTargets()
{
  const PBXArray *target_arr = dynamic_cast<const PBXArray*>(pDoc->valueForKeyPath("rootObject.targets"));
  if(!target_arr)
    return;

  //get target count from "Project object"
  this->targetcount =  target_arr->count();


  PBXValueList::const_iterator itor = target_arr->begin();
  PBXValueList::const_iterator end  = target_arr->end();
  for (; itor != end; ++itor) {
    const PBXValueRef* ref     = dynamic_cast<const PBXValueRef*>(*itor);
    const PBXValue *value = pDoc->deref(ref);
    const PBXBlock *target_blk = PBXBlock::cast(value);
    
    if(!target_blk)
      return;

    PBXNativeTarget target_detail;

    //ignore PBXAggregateTarget, aggregate target means a target group
    //we did not want to support it.
    const PBXText * target_isa =  dynamic_cast<const PBXText*>(target_blk->valueForKey("isa"));
    if(target_isa->text() == string("PBXAggregateTarget")) {
      this->targetcount -= 1; 
      continue;  
    }
    //ignore everything until it is a PBXNativeTarget.
    if(target_isa->text() != string("PBXNativeTarget")) {
      this->targetcount -= 1;
      continue;
    }
    //target name
    const PBXText * name = dynamic_cast<const PBXText*> (target_blk->valueForKey("name"));
    if(name)
      target_detail.name = m_replace(name->text(), "\"", "", -1);
    //prodcutName in target sometimes meaningless
    // 
    /*const PBXText *productName = dynamic_cast<const PBXText*> (target_blk->valueForKey("productName"));
      if(productName)
      target_detail.productName = productName->text();
    */
    const PBXText *productType = dynamic_cast<const PBXText*> (target_blk->valueForKey("productType"));
    if(productType)
      target_detail.productType = m_replace(productType->text(), "\"", "", -1);
      
    const PBXValueRef *bcl_ref = dynamic_cast<const PBXValueRef*>(target_blk->valueForKey("buildConfigurationList"));
    const PBXValue * bcl_value = dynamic_cast<const PBXValue *>(pDoc->deref(bcl_ref));
    const PBXBlock *bcl_blk = dynamic_cast<const PBXBlock*>(bcl_value);

    // a target should always had a buildConfigurationList contains Release/Debug build configuration
    // is it is missing, ignore this target.
    if(!bcl_blk) {
      this->targetcount -= 1;
      continue;
    }
   
    //get enough infomations from build configurations
    //here we only get things from "Release" configuration.
    target_detail.buildargs = getBuildSettings(bcl_blk);
    target_detail.infoplist = getInfoPlist(bcl_blk);
    target_detail.productName =getProductName(bcl_blk);
    target_detail.dylib_compatibility_version = getDylibCompatibilityVersion(bcl_blk);
    target_detail.dylib_current_version = getDylibCurrentVersion(bcl_blk);

    //Sometimes productName in buildconfigurations points to TARGET_NAME;
    if(target_detail.productName == "$(TARGET_NAME)")
      target_detail.productName = target_detail.name;


    //productReference point to the final result. 
    const PBXValueRef *pr_ref = dynamic_cast<const PBXValueRef*>(target_blk->valueForKey("productReference"));
    const PBXValue *pr_value = pDoc->deref(pr_ref);
    const PBXBlock *pr_blk = PBXBlock::cast(pr_value);
    
    const PBXText * explicitFileType =  dynamic_cast<const PBXText*>(pr_blk->valueForKey("explicitFileType"));
    if(explicitFileType)
      target_detail.resulttype = m_replace(explicitFileType->text(),"\"","",-1);
    
    const PBXText * path = dynamic_cast<const PBXText*>(pr_blk->valueForKey("path"));
    if(path) {
      string result = path->text();
      result = m_replace(result," ", "_", -1);
      result = m_replace(result,"\"", "", -1);
      target_detail.result = result;//path->text();
    }

    //buildPhases in target block defines all resources in this target.
    //mostly include Sources/Frameworks/Headers/Resources
    const PBXArray *bp_arr = dynamic_cast<const PBXArray*>(target_blk->valueForKey("buildPhases"));

    if(bp_arr) {
      PBXValueList::const_iterator bp_itor = bp_arr->begin();
      PBXValueList::const_iterator bp_end  = bp_arr->end();
      for(; bp_itor != bp_end; bp_itor++){
	const PBXValueRef* bp_ref     = dynamic_cast<const PBXValueRef*>(*bp_itor);
	const PBXValue *bp_value = pDoc->deref(bp_ref);
	const PBXBlock *bp_blk = PBXBlock::cast(bp_value);
	    
	//record bp_blk_type here. 
	string bp_blk_type = "";	  
	const PBXText* bp_isa = dynamic_cast<const PBXText*>(bp_blk->valueForKey("isa"));
	if(!bp_isa) {
	  this->targetcount -= 1;
	  continue;
	}
	
	bp_blk_type = bp_isa->text();

	const PBXArray* files_arr = dynamic_cast<const PBXArray*>(bp_blk->valueForKey("files"));
	if(!files_arr) {
	  this->targetcount -= 1;
	  continue;
	}
	if(bp_blk_type == "PBXFrameworksBuildPhase")
	  getFilesFromFileArray(files_arr, FRAMEWORKS, &target_detail);
	else if(bp_blk_type == "PBXSourcesBuildPhase")
	  getFilesFromFileArray(files_arr, SOURCES, &target_detail);
	else if(bp_blk_type == "PBXResourcesBuildPhase")
	  getFilesFromFileArray(files_arr, RESOURCES, &target_detail);
	else if(bp_blk_type == "PBXHeadersBuildPhase")
	  getFilesFromFileArray(files_arr, HEADERS, &target_detail);
      }
    }
    this->targets.push_back(target_detail);
  }
}

void PBXProj::getFilesFromFileArray(const PBXArray *files_arr, buildPhase bptype, PBXNativeTarget *target) 
{
  PBXValueList::const_iterator files_itor = files_arr->begin();
  PBXValueList::const_iterator files_end  = files_arr->end();
  for(; files_itor != files_end; files_itor++){
    const PBXValueRef * files_ref = dynamic_cast<const PBXValueRef*>(*files_itor);
    const PBXValue *files_value = pDoc->deref(files_ref);
    const PBXBlock *files_blk = PBXBlock::cast(files_value);

    string cflag = "";
    int is_public_header = 0;
    const PBXBlock * file_settings = dynamic_cast<const PBXBlock*>(files_blk->valueForKey("settings"));
    // files contains a assignment named settings, it will contains special compiler flag for source file
    // or is/is not public for header files.
    // these info are we needed.
    if(file_settings) {
      const PBXText * compiler_flags = dynamic_cast<const PBXText *> (file_settings->valueForKey("COMPILER_FLAGS"));
      if(compiler_flags)
	cflag = m_replace(compiler_flags->text(),"\"","",-1);
      const PBXArray * file_attr =  dynamic_cast<const PBXArray *> (file_settings->valueForKey("ATTRIBUTES"));
      if(file_attr){
	PBXValueList::const_iterator attr_itor = file_attr->begin();
	PBXValueList::const_iterator attr_end = file_attr->end();
	for(; attr_itor != attr_end; attr_itor++) {
	  const PBXText * is_public = dynamic_cast<const PBXText *> (*attr_itor);
	  if(is_public && (is_public->text() == string("Public")))
	    is_public_header = 1;
	}
      }
    }

    const PBXValueRef *file_ref = dynamic_cast<const PBXValueRef*>(files_blk->valueForKey("fileRef") );

    const PBXValue *file_value = pDoc->deref(file_ref);
    const PBXBlock *file_blk = PBXBlock::cast(file_value);

    const PBXText * file_isa = dynamic_cast<const PBXText*>(file_blk->valueForKey("isa"));
    string isa = file_isa->text();
    File file;

    if(isa == "PBXVariantGroup") {
      //Group in resources seems mean a directory that should be keep its structure.
      //So we try to get the directory name;
      const PBXArray * children_arr = dynamic_cast<const PBXArray*>(file_blk->valueForKey("children"));
      PBXValueList::const_iterator children_itor = children_arr->begin();
      PBXValueList::const_iterator children_end  = children_arr->end();
      for(; children_itor != children_end; children_itor++) {
	const PBXValueRef * c_ref = dynamic_cast<const PBXValueRef*>(*children_itor);
	const PBXValue * c_value = pDoc->deref(c_ref);
	const PBXBlock * c_blk = PBXBlock::cast(c_value);
	const PBXText * file_isa = dynamic_cast<const PBXText*>(c_blk->valueForKey("isa"));
	if(file_isa)
	  file.isa = file_isa->text();
	const PBXText * file_lastKnownFileType = dynamic_cast<const PBXText*>(c_blk->valueForKey("lastKnownFileType"));
	if(file_lastKnownFileType)
	  file.lastKnownFileType = file_lastKnownFileType->text();
	const PBXText * file_name = dynamic_cast<const PBXText*>(c_blk->valueForKey("name"));
	if(file_name)
	  file.name = file_name->text();

	const PBXText * file_path = dynamic_cast<const PBXText*>(c_blk->valueForKey("path"));
	if(file_path) {
	  string local_path = file_path->text();
	  string full_path = local_path;
	  if(this->allFiles.find(local_path) != this->allFiles.end())
	    full_path = this->allFiles.find(local_path)->second;
	  full_path = ::dirname(strdup(full_path.c_str()));
	  file.path = full_path;
	}
      }
    }
    else if(isa == "PBXFileReference") {
      file.isa = file_isa->text();
      file.cflag = cflag;
      file.is_public_header = is_public_header;
      const PBXText * file_lastKnownFileType = dynamic_cast<const PBXText*>(file_blk->valueForKey("lastKnownFileType"));
      if(file_lastKnownFileType)
	file.lastKnownFileType = file_lastKnownFileType->text();
      const PBXText * file_name = dynamic_cast<const PBXText*>(file_blk->valueForKey("name"));
      if(file_name)
	file.name = file_name->text();

      const PBXText * file_path = dynamic_cast<const PBXText*>(file_blk->valueForKey("path"));
      if(file_path) {
	string local_path = file_path->text();
	string full_path =local_path;
	if(this->allFiles.find(local_path) != this->allFiles.end())
	  full_path = this->allFiles.find(local_path)->second;
	full_path = m_replace(full_path, "\"", "", -1);
	file.path = full_path;
      }
    }
    else if(isa == "PBXReferenceProxy") {
      file.isa = file_isa->text();
      const PBXText * file_path = dynamic_cast<const PBXText*>(file_blk->valueForKey("path"));
      if(file_path) {
	string local_path = file_path->text();
	string full_path =local_path;
	if(this->allFiles.find(local_path) != this->allFiles.end())
	  full_path = this->allFiles.find(local_path)->second;
	full_path = m_replace(full_path, "\"", "", -1);
	file.path = full_path;
      }
    }
    if(bptype == FRAMEWORKS && !file.path.empty()) {
      string framework = file.path;
      char * frameworkpath = strdup(framework.c_str());
      framework = ::basename(frameworkpath);
      framework = framework.substr(0,framework.find(".framework"));
      file.path = framework;
      target->frameworks.push_back(file);
    } 
    else if(bptype == RESOURCES && !file.path.empty())
      target->resources.push_back(file);
    else if(bptype == SOURCES && !file.path.empty())
      target->sources.push_back(file);
    else if(bptype == HEADERS && !file.path.empty()){
      target->headers.push_back(file);
      string headerpath = file.path;
      char * headerpath_c = strdup(headerpath.c_str());
      headerpath = ::dirname(headerpath_c);
      file.path = m_replace(headerpath,"\"","",-1);
      target->headerpaths.push_back(file);
    }
  }
}



void PBXProj::getAllFilesFromMainGroup(const PBXBlock *block, string current_path)
{
  string local_path = current_path;
  string block_type;

  const PBXText *isa =dynamic_cast<const PBXText*>(block->valueForKey("isa"));
  if(isa)
    block_type = isa->text();
  
  const PBXText * path = dynamic_cast<const PBXText*>(block->valueForKey("path"));
  if(path)
    local_path = local_path+"/"+path->text();

  if(block_type == "PBXFileReference") {
    // cout<<local_path<<endl;
    allFiles.insert(pair<string,string>(path->text(), local_path));
    //    this->allFiles.push_back(local_path);
  } else if (block_type == "PBXGroup" || block_type == "PBXVariantGroup") {
    const PBXArray *arr = dynamic_cast<const PBXArray *>(block->valueForKey("children"));
    if(!arr)
      return;
    PBXValueList::const_iterator itor = arr->begin();
    PBXValueList::const_iterator end  = arr->end();
    for(; itor != end; itor++){
      const PBXValueRef * ref = dynamic_cast<const PBXValueRef*>(*itor);
      const PBXValue *value = pDoc->deref(ref);
      const PBXBlock *blk = PBXBlock::cast(value);
      if(!blk)
        return;
      this->getAllFilesFromMainGroup(blk, local_path);
    }
  }
}

string PBXProj::getProductName(const PBXBlock *block)
{
  string productname;
  const PBXText * type = dynamic_cast<const PBXText *>(block->valueForKey("isa"));
  string btype = type->text();
  if(!type || btype != "XCConfigurationList")
    return productname;
  
  const PBXText * defaults = dynamic_cast<const PBXText *>(block->valueForKey("defaultConfigurationName"));
  if(!defaults)
    return productname;
  string bdefaults = defaults->text();
  const PBXArray * confs = dynamic_cast<const PBXArray *>(block->valueForKey("buildConfigurations"));
  if(!confs)
    return productname;

  PBXValueList::const_iterator itor = confs->begin();
  PBXValueList::const_iterator end  = confs->end();  

  for(; itor != end; itor++) {
    const PBXValueRef * ref = dynamic_cast<const PBXValueRef*>(*itor);
    const PBXValue *value = pDoc->deref(ref);
    const PBXBlock *blk = PBXBlock::cast(value); 
    const PBXText * name = dynamic_cast<const PBXText *>(blk->valueForKey("name"));
    string bname = name->text();
    if(bname != bdefaults )
      continue;
    const PBXBlock * settings = dynamic_cast<const PBXBlock *>(blk->valueForKey("buildSettings"));
    if(!settings)
      continue;
    const PBXText * pName = dynamic_cast<const PBXText *>(settings->valueForKey("PRODUCT_NAME"));
    if(pName){
      productname = pName->text();
      m_replace(productname, " ", "_", -1);
      m_replace(productname, "\"", "", -1);
      return productname;
    }
  }  
  return productname;
}

string PBXProj::getDylibCompatibilityVersion(const PBXBlock *block)
{
  string version;
  const PBXText * type = dynamic_cast<const PBXText *>(block->valueForKey("isa"));
  string btype = type->text();
  if(!type || btype != "XCConfigurationList")
    return version;
  const PBXText * defaults = dynamic_cast<const PBXText *>(block->valueForKey("defaultConfigurationName"));
  if(!defaults)
    return version;
  string bdefaults = defaults->text();
  const PBXArray * confs = dynamic_cast<const PBXArray *>(block->valueForKey("buildConfigurations"));
  if(!confs)
    return version;
  PBXValueList::const_iterator itor = confs->begin();
  PBXValueList::const_iterator end  = confs->end();
  for(; itor != end; itor++) {
    const PBXValueRef * ref = dynamic_cast<const PBXValueRef*>(*itor);
    const PBXValue *value = pDoc->deref(ref);
    const PBXBlock *blk = PBXBlock::cast(value);
    const PBXText * name = dynamic_cast<const PBXText *>(blk->valueForKey("name"));
    string bname = name->text();
    if(bname != bdefaults )
      continue;
    const PBXBlock * settings = dynamic_cast<const PBXBlock *>(blk->valueForKey("buildSettings"));
    if(!settings)
      continue;
    
    //lexer treat number as integer but float number as text.
    const PBXInteger * compVersion = dynamic_cast<const PBXInteger *>(settings->valueForKey("DYLIB_COMPATIBILITY_VERSION"));
    if(!compVersion){
      const PBXText * compVersionText = dynamic_cast<const PBXText *>(settings->valueForKey("DYLIB_COMPATIBILITY_VERSION"));
      if(compVersionText) {
        version = compVersionText->text();
        return version;
      }
    }
    else {
      stringstream ss;
      ss<< compVersion->intValue();
      return ss.str();
    }
  }
  return version;
}


string PBXProj::getDylibCurrentVersion(const PBXBlock *block)
{
  string version;
  const PBXText * type = dynamic_cast<const PBXText *>(block->valueForKey("isa"));
  string btype = type->text();
  if(!type || btype != "XCConfigurationList")
    return version;
  const PBXText * defaults = dynamic_cast<const PBXText *>(block->valueForKey("defaultConfigurationName"));
  if(!defaults)
    return version;
  string bdefaults = defaults->text();
  const PBXArray * confs = dynamic_cast<const PBXArray *>(block->valueForKey("buildConfigurations"));
  if(!confs)
    return version;
  PBXValueList::const_iterator itor = confs->begin();
  PBXValueList::const_iterator end  = confs->end();
  for(; itor != end; itor++) {
    const PBXValueRef * ref = dynamic_cast<const PBXValueRef*>(*itor);
    const PBXValue *value = pDoc->deref(ref);
    const PBXBlock *blk = PBXBlock::cast(value);
    const PBXText * name = dynamic_cast<const PBXText *>(blk->valueForKey("name"));
    string bname = name->text();
    if(bname != bdefaults )
      continue;
    const PBXBlock * settings = dynamic_cast<const PBXBlock *>(blk->valueForKey("buildSettings"));
    if(!settings)
      continue;
    //lexer treat number as integer but float number as text.
    const PBXInteger * currVersion = dynamic_cast<const PBXInteger *>(settings->valueForKey("DYLIB_CURRENT_VERSION"));
    if(!currVersion){
      const PBXText * currVersionText = dynamic_cast<const PBXText *>(settings->valueForKey("DYLIB_CURRENT_VERSION"));
      if(currVersionText) {
        version = currVersionText->text();
        return version;
      }
    }
    else {
      stringstream ss;
      ss<< currVersion->intValue();
      return ss.str();
    }
  }
  return version;
}


string PBXProj::getInfoPlist(const PBXBlock *block)
{
  string infoplist;
  const PBXText * type = dynamic_cast<const PBXText *>(block->valueForKey("isa"));
  string btype = type->text();
  if(!type || btype != "XCConfigurationList")
    return infoplist;
  const PBXText * defaults = dynamic_cast<const PBXText *>(block->valueForKey("defaultConfigurationName"));
  if(!defaults)
    return infoplist;
  string bdefaults = defaults->text();
  const PBXArray * confs = dynamic_cast<const PBXArray *>(block->valueForKey("buildConfigurations"));
  if(!confs)
    return infoplist;
  PBXValueList::const_iterator itor = confs->begin();
  PBXValueList::const_iterator end  = confs->end();  
  for(; itor != end; itor++) {
    const PBXValueRef * ref = dynamic_cast<const PBXValueRef*>(*itor);
    const PBXValue *value = pDoc->deref(ref);
    const PBXBlock *blk = PBXBlock::cast(value); 
    const PBXText * name = dynamic_cast<const PBXText *>(blk->valueForKey("name"));
    string bname = name->text();
    if(bname != bdefaults )
      continue;
    const PBXBlock * settings = dynamic_cast<const PBXBlock *>(blk->valueForKey("buildSettings"));
    if(!settings)
      continue;
    const PBXText * pList = dynamic_cast<const PBXText *>(settings->valueForKey("INFOPLIST_FILE"));
    if(pList){
      infoplist = pList->text();
      return m_replace(infoplist, "\"", "", -1);
    }
  }  
  return infoplist;
}

string PBXProj::getBuildSettings(const PBXBlock *block)
{
  string buildargs = "";
  const PBXText * type = dynamic_cast<const PBXText *>(block->valueForKey("isa"));
  string btype = type->text();
  if(!type || btype != "XCConfigurationList")
    return buildargs;
  
  const PBXText * defaults = dynamic_cast<const PBXText *>(block->valueForKey("defaultConfigurationName"));
  if(!defaults)
    return buildargs;
  string bdefaults = defaults->text();
  const PBXArray * confs = dynamic_cast<const PBXArray *>(block->valueForKey("buildConfigurations"));
  if(!confs)
    return buildargs;

  PBXValueList::const_iterator itor = confs->begin();
  PBXValueList::const_iterator end  = confs->end();  

  for(; itor != end; itor++) {
    const PBXValueRef * ref = dynamic_cast<const PBXValueRef*>(*itor);
    const PBXValue *value = pDoc->deref(ref);
    const PBXBlock *blk = PBXBlock::cast(value); 
    const PBXText * name = dynamic_cast<const PBXText *>(blk->valueForKey("name"));
    string bname = name->text();
    if(bname != bdefaults )
      continue;
    const PBXBlock * settings = dynamic_cast<const PBXBlock *>(blk->valueForKey("buildSettings"));
    if(!settings)
      continue;
    const PBXText * arc = dynamic_cast<const PBXText *>(settings->valueForKey("CLANG_ENABLE_OBJC_ARC"));
    if(arc && arc->text() == string("YES"))
      buildargs = buildargs + " -fobjc-arc";
    const PBXText * pch = dynamic_cast<const PBXText *>(settings->valueForKey("GCC_PRECOMPILE_PREFIX_HEADER"));
    if(pch && pch->text() == string("YES")) {
      const PBXText *pch_path = dynamic_cast<const PBXText *>(settings->valueForKey("GCC_PREFIX_HEADER"));
      if(pch_path)
	buildargs = buildargs + " -include " + pch_path->text();
    }
    const PBXArray *hsp_arr = dynamic_cast<const PBXArray*>(settings->valueForKey("HEADER_SEARCH_PATHS"));
    if(hsp_arr) {
      PBXValueList::const_iterator hsp_itor = hsp_arr->begin();
      PBXValueList::const_iterator hsp_end  = hsp_arr->end();
      for(; hsp_itor != hsp_end; hsp_itor++){
        const PBXText *hsp_path = dynamic_cast<const PBXText *> (*hsp_itor);
        if(hsp_path && hsp_path->text())
          buildargs = buildargs + " -I" + hsp_path->text();
      }
    }
  }
  
  vector<string> local_header_path;
  //search allFiles, find .h, add the path to buildargs;
  map <string, string>::iterator allFiles_Iter;
  for ( allFiles_Iter = allFiles.begin( ); allFiles_Iter != allFiles.end( ); allFiles_Iter++ ) {
    string file = allFiles_Iter->first;
    if(endWith(file, ".h")){
      string fullpath = allFiles_Iter->second;
      fullpath = ::dirname(strdup(fullpath.c_str()));
      local_header_path.push_back(fullpath);
    }
  }
  //remove duplicated one
  sort(local_header_path.begin(), local_header_path.end());
  local_header_path.erase(unique(local_header_path.begin(), local_header_path.end()), local_header_path.end()); 
  
  for(int i = 0; i < local_header_path.size(); i++) {
    buildargs = buildargs + " -I" + local_header_path[i];
  }
  return buildargs;
}

void convertMakefile(PBXNativeTarget target, targetType type)
{
  string compiler = "ios-clang";
  string buildargs = target.buildargs;
  string output = target.result.substr(0,target.result.find(".app"));

  if(type == FRAMEWORK)
    output = m_replace(output, ".framework", "", -1); 

  vector<string> headerpaths;
  for(int i = 0; i < target.headerpaths.size(); i++) {
    headerpaths.push_back(target.headerpaths[i].path);
  }
  sort(headerpaths.begin(), headerpaths.end());
  headerpaths.erase(unique(headerpaths.begin(), headerpaths.end()), headerpaths.end());


  for(int i = 0; i < headerpaths.size(); i++) {
    buildargs = buildargs + " -I" + headerpaths[i];
  }
  buildargs += " -I.";

  ofstream makefile("./Makefile");
  if(type == APP)
    makefile << "IPHONE_IP:=" <<endl;
 
  makefile << "PROJECTNAME:="<<output<<endl;

  if(type == APP) {
    makefile << "APPFOLDER:=xcbuild/$(PROJECTNAME).app" <<endl;
    makefile << "INSTALLFOLDER:=$(PROJECTNAME).app"<<endl;
  }
  
  makefile << endl;
  makefile << "CC:=ios-clang" <<endl;
  makefile << "CPP:=ios-clang++" <<endl;
  makefile << endl;
  makefile << "CFLAGS +="<<buildargs<<endl;
  makefile << endl;
  makefile << "CPPFLAGS +="<<buildargs<<endl;
  makefile << endl;
  for(int i = 0; i < target.frameworks.size(); i++) {
    //handle dylib and static lib in frameworks;
    string framework = target.frameworks[i].path;
    if(endWith(framework, ".dylib") && beginWith(framework, "lib")) {
      framework = framework.substr(0,framework.find(".dylib"));
      framework = framework.substr(3, framework.length());
      makefile <<"LDFLAGS += -l" <<framework<<endl;
    } else if(endWith(framework, ".a") && beginWith(framework, "lib")) {
      makefile <<"LDFLAGS += "<<framework<<endl;
    }else 
      makefile <<"LDFLAGS += -framework " + framework <<endl;
  }

  if(type == FRAMEWORK) {
    makefile <<"LDFLAGS += -dynamiclib" <<endl;
    makefile <<"LDFLAGS += -install_name /Library/Frameworks/$(PROJECTNAME).framework/$(PROJECTNAME)"<<endl;
    if(!target.dylib_compatibility_version.empty())
      makefile <<"LDFLAGS += -compatibility_version "<<target.dylib_compatibility_version <<endl;
    if(!target.dylib_current_version.empty())
      makefile <<"LDFLAGS += -current_version "<<target.dylib_current_version <<endl;
  }
  makefile << endl;
 
  vector<string> sourcepaths;
  for(int i = 0; i < target.sources.size(); i++) {
    sourcepaths.push_back(target.sources[i].path);
  }
  
  /*  sort(sourcepaths.begin(), sourcepaths.end());
      sourcepaths.erase(unique(sourcepaths.begin(), sourcepaths.end()), sourcepaths.end());
  */
  makefile << endl;
  if(type == STATICLIB)
    makefile << "all: $(PROJECTNAME) headers"<<endl<<endl;
  else if(type == FRAMEWORK)
    makefile << "all: $(PROJECTNAME) framework"<<endl<<endl;
  else
    makefile << "all: $(PROJECTNAME)"<<endl<<endl;

  makefile << "OBJS+=  \\"<<endl;
  for(int i = 0; i < sourcepaths.size(); i++) {
    string object = m_replace(sourcepaths[i], ".m", ".o", -1);
    object = m_replace(object, ".c", ".o", -1);
    object = m_replace(object, ".cpp", ".o", -1);
    if(i == sourcepaths.size()-1)
      makefile << "\t"<<object<<endl;
    else
      makefile << "\t"<<object<<" \\"<<endl;
  }
  makefile << endl;
  makefile << "$(PROJECTNAME): \\"<<endl;
  for(int i = 0; i < sourcepaths.size(); i++) {
    string object = m_replace(sourcepaths[i], ".m", ".o", -1);
    object = m_replace(object, ".c", ".o", -1);
    object = m_replace(object, ".cpp", ".o", -1);
    if(i == sourcepaths.size()-1)
      makefile << "\t"<<object<<endl;
    else
      makefile << "\t"<<object<<" \\"<<endl;
  }

  if(type == APP || type == EXEC) { 
    makefile << "\tmkdir -p xcbuild"<<endl;
    makefile << "\t$(CC) $(CFLAGS) $(LDFLAGS) $(filter %.o,$^) -o xcbuild/$@"<<endl<<endl;
  } else if(type == FRAMEWORK) {
    makefile << "\tmkdir -p xcbuild/$(PROJECTNAME).framework"<<endl;
    makefile << "\t$(CC) $(CFLAGS) $(LDFLAGS) $(filter %.o,$^) -o xcbuild/$(PROJECTNAME).framework/$@"<<endl<<endl;
  } else if(type == STATICLIB) {
    makefile << "\tmkdir -p xcbuild"<<endl;
    makefile << "\tarm-apple-darwin11-ar cr xcbuild/"<<output<<" $(filter %.o,$^)"<<endl<<endl;
  }

  for(int i = 0; i < sourcepaths.size(); i++) {
    string object = m_replace(sourcepaths[i], ".m", ".o", -1);
    object = m_replace(object, ".c", ".o", -1);
    object = m_replace(object, ".cpp", ".o", -1);
    makefile << object <<": "<<sourcepaths[i]<<endl;
    if(endWith(sourcepaths[i],".cpp"))
      makefile << "\t$(CPP) -c $(CPPFLAGS) "<< target.sources[i].cflag << " $< -o $@"<<endl<<endl;
    else
      makefile << "\t$(CC) -c $(CFLAGS) "<<target.sources[i].cflag<< " $< -o $@"<<endl<<endl;
  }

  if(type == STATICLIB) {
    makefile << "headers:"<<endl;
    makefile << "\tmkdir -p xcbuild/headers/private" <<endl;
    
    for(int i = 0; i < target.headers.size(); i++) {
      if(target.headers[i].is_public_header)
        makefile << "\tcp -r " << target.headers[i].path << " xcbuild/headers" <<endl;
      else
        makefile << "\tcp -r " << target.headers[i].path << " xcbuild/headers/private" <<endl;
    }
    makefile <<endl;
  }

  if(type == FRAMEWORK) {
    makefile << "framework:"<<endl;
    makefile << "\tmkdir -p xcbuild/$(PROJECTNAME).framework/Headers/private" <<endl;
    for(int i = 0; i < target.headers.size(); i++) {
      if(target.headers[i].is_public_header)
        makefile << "\tcp -r " << target.headers[i].path << " xcbuild/$(PROJECTNAME).framework/Headers" <<endl;
      else
        makefile << "\tcp -r " << target.headers[i].path << " xcbuild/$(PROJECTNAME).framework/Headers/private" <<endl;
        
    }
    makefile <<endl;
  }

  if(type == APP) { // App need this.
    makefile << "INFOPLIST:=" <<target.infoplist<<endl <<endl;

    vector<string> resources;
    for(int i = 0; i < target.resources.size(); i++) {
      resources.push_back(target.resources[i].path);
    }
    sort(resources.begin(), resources.end());
    resources.erase(unique(resources.begin(), resources.end()), resources.end());

    makefile << "RESOURCES += \\" <<endl;
    for(int i = 0; i < resources.size(); i++) {
      if(i == resources.size()-1)
        makefile << "\t"<< resources[i] <<endl;
      else
        makefile << "\t"<< resources[i] <<" \\"<<endl;
    }
    makefile << endl;

    makefile << "dist: $(PROJECTNAME)"<<endl;
    makefile << "\tmkdir -p $(APPFOLDER)"<<endl;
    makefile << "\tcp -r $(RESOURCES) $(APPFOLDER)"<<endl;
    makefile << "\tcp $(INFOPLIST) $(APPFOLDER)/Info.plist"<<endl;
    makefile << "\tcp xcbuild/$(PROJECTNAME) $(APPFOLDER)"<<endl;
    makefile << "\tsed -i 's|$${EXECUTABLE_NAME}|" << output << "|g' " << "$(APPFOLDER)/Info.plist"<<endl;
    makefile << "\tsed -i 's|$${PRODUCT_NAME}|" << output << "|g' " << "$(APPFOLDER)/Info.plist"<<endl;
    transform(output.begin(), output.end(), output.begin(), ::tolower);
    makefile << "\tsed -i 's|$${PRODUCT_NAME:identifier}|" << output << "|g' " << "$(APPFOLDER)/Info.plist"<<endl;
    makefile << "\tsed -i 's|$${PRODUCT_NAME:rfc1034identifier}|" << output << "|g' " << "$(APPFOLDER)/Info.plist"<<endl;
    makefile << "\tfind $(APPFOLDER) -name \\*.png|xargs ios-pngcrush -c"<<endl;
    makefile << "\tfind $(APPFOLDER) -name \\*.plist|xargs ios-plutil -c"<<endl;
    makefile << "\tfind $(APPFOLDER) -name \\*.strings|xargs ios-plutil -c"<<endl;
    makefile << endl;
    makefile << "langs:"<<endl;
    makefile << "\tios-genLocalization"<<endl<<endl;

    makefile << "install: dist"<<endl;
    makefile << "ifeq ($(IPHONE_IP),)"<<endl;
    makefile << "\techo \"Please set IPHONE_IP\""<<endl;
    makefile << "else"<<endl;
    makefile << "\tssh root@$(IPHONE_IP) 'rm -fr /Applications/$(INSTALLFOLDER)'"<<endl;
    makefile << "\tscp -r $(APPFOLDER) root@$(IPHONE_IP):/Applications/$(INSTALLFOLDER)"<<endl;
    makefile << "\techo \"Application $(INSTALLFOLDER) installed\""<<endl;
    makefile << "\tssh mobile@$(IPHONE_IP) 'uicache'"<<endl;
    makefile << "endif" <<endl <<endl;

    makefile << "uninstall:"<<endl;
    makefile << "ifeq ($(IPHONE_IP),)"<<endl;
    makefile << "\techo \"Please set IPHONE_IP\""<<endl;
    makefile << "else"<<endl;
    makefile << "\tssh root@$(IPHONE_IP) 'rm -fr /Applications/$(INSTALLFOLDER)'"<<endl;
    makefile << "\techo \"Application $(INSTALLFOLDER) uninstalled\"" <<endl;
    makefile << "endif" <<endl <<endl;
  } //end APP

  makefile << "clean:"<<endl;
  makefile << "\tfind . -name \\*.o|xargs rm -rf"<<endl;
  makefile << "\trm -rf xcbuild"<<endl;

  makefile.close();  
  cout <<"Makefile generated."<<endl;
  cout <<"Build result will be in ./xcbuild" <<endl;
}


void convertTarget(PBXNativeTarget target, int compile)
{
  if(target.resulttype == "archive.ar")
    convertMakefile(target, STATICLIB);
  else if(target.resulttype == "wrapper.application")
    convertMakefile(target, APP);
  else if(target.resulttype == "compiled.mach-o.executable")
    convertMakefile(target, EXEC);
  else if(target.resulttype == "wrapper.framework")
    convertMakefile(target, FRAMEWORK);
  else
    cout <<"Not supported yet."<<endl;
  if(compile)
    runCommand("make");
}

void printHelp(const char* cmd)
{
  printf("Usage: %s -c <project.pbxproj> : convert xcodeproj to make\n", cmd);
  printf("       %s -b <project.pbxproj> : build xcodeproj directly\n", cmd);
  exit(0);
}

int main(int argc, char* argv[])
{
  
  const char * cmd = argv[0]; 
  int willcompile = 0;
  if(argc == 1)
    printHelp(cmd);

  char optflag;
  while((optflag = getopt(argc, argv, "hcb")) != -1) {
    switch(optflag) {
    case 'c':
      willcompile = 0;
      break;
    case 'b':
      willcompile = 1;
      break;
    case 'h':
    default:
      printHelp(cmd);
    }
  }

  argc = argc - optind;
  argv = argv + optind;

  if(::access(argv[0],R_OK) != 0)
    printHelp(cmd);

  PBXProj *pbx = new PBXProj();
  pbx->loadProj(argv[0]);
  int target_count = pbx->getTargetCount();
  vector<PBXNativeTarget> targets = pbx->getTargets();
  if(target_count <= 0)
    cout <<"Error, no target found in project" <<endl;

  if(target_count >1) {
    cout <<"There are more than one targets in project files"<<endl;
    for(int i = 0; i < target_count; i++) {
      printf("%d, %s, %s, %s\n", i, targets[i].name.c_str(), targets[i].result.c_str(), targets[i].resulttype.c_str());
    }
    short input;
    do {
      cout <<"Please choose one:";
      cin>>input;
      if (cin.fail()) {
	cin.clear();
	cin.sync();
      }
    } while(input > target_count || input < 0);
    convertTarget(targets[input], willcompile);
  } else 
    convertTarget(targets[0], willcompile);
  
  return 0;
}



