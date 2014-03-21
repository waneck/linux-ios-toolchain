#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlerror.h>

#include "plist/plist.h"

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>

#include <string>
using namespace std;
void _nullGenericErrorFunc(void* ctxt, char* msg, ...)
{
  //disable all warnings and errors of libxml2.
}

bool is_xml_plist(string plistpath)
{
  if(::access(plistpath.c_str(), R_OK) != 0)
    return 0;
  xmlSetGenericErrorFunc(NULL, (xmlGenericErrorFunc)_nullGenericErrorFunc);

  xmlDocPtr doc;
  xmlNodePtr cur;
  doc = xmlReadFile(plistpath.c_str(), "UTF-8", 0);
  if (doc == NULL)
    return 0;

  cur = xmlDocGetRootElement(doc);
  if(cur == NULL) {
    xmlFreeDoc(doc);
    return 0;
  }
  if (xmlStrcmp(cur->name, (const xmlChar *) "plist")) {
    xmlFreeDoc(doc);
    return 0;
  }
  return 1;
}

int is_binary_plist(const char *plistfile)
{
  if(access(plistfile, R_OK) != 0)
    return 0;
  char *plist_entire = NULL;
  FILE *fp = fopen(plistfile, "rb");
  if(!fp)
    return 0;
  struct stat st;
  stat(plistfile, &st);
  plist_entire = (char *) malloc(sizeof(char) * (st.st_size + 1));
  int read_size = fread(plist_entire, sizeof(char), st.st_size, fp);
  fclose(fp);
  if (memcmp(plist_entire, "bplist00", 8) == 0) {
    free(plist_entire);
    return 1;
  }
  free(plist_entire);
  return 0;
}
int get_value_of_key_from_plist(char *valuestr, const char *keystr, const char *plistfile)
{
  if(!(is_xml_plist(plistfile) || is_binary_plist(plistfile)))
    return 0;

  xmlSetGenericErrorFunc(NULL, (xmlGenericErrorFunc)_nullGenericErrorFunc);

  xmlDocPtr doc;   
  xmlNodePtr cur; 
  
  if(is_binary_plist(plistfile)) {
    FILE *file_in = fopen(plistfile, "rb");
    if(!file_in)
      return 0;
    struct stat st;
    stat(plistfile, &st);
    char * plist_entire = (char *) malloc(sizeof(char) * (st.st_size + 1));
    int read_size = fread(plist_entire, sizeof(char), st.st_size, file_in);
    fclose(file_in);

    plist_t root_node = NULL;
    char *plist_out = NULL;
    uint32_t size;

    plist_from_bin(plist_entire, read_size, &root_node);
    plist_to_xml(root_node, &plist_out, &size);
    doc = xmlReadMemory(plist_out, size, NULL, "UTF-8", XML_PARSE_RECOVER); 
  }
  else 
    doc = xmlReadFile(plistfile, "UTF-8",XML_PARSE_RECOVER ); 

  if (doc == NULL ) { 
    fprintf(stderr,"Document not parsed successfully. \n"); 
    return 0; 
  } 
  cur = xmlDocGetRootElement(doc); 

  if (cur == NULL) { 
    fprintf(stderr,"empty document\n"); 
    xmlFreeDoc(doc); 
    return 0; 
  } 

  if (xmlStrcmp(cur->name, (const xmlChar *) "plist")) { 
    fprintf(stderr,"document of the wrong type, root node != plist"); 
    xmlFreeDoc(doc); 
    return 0; 
  } 
  
  cur = cur->xmlChildrenNode;


  while(cur != NULL) {
    if ((!xmlStrcmp(cur->name, (const xmlChar *)"dict"))) {
      
      xmlNodePtr keyNode = cur->xmlChildrenNode;
      xmlNodePtr valuePtr = NULL;
      xmlChar *key; 
      
      while (keyNode != NULL) {
    if((!xmlStrcmp(keyNode->name, (const xmlChar *)"key"))) {
      key = xmlNodeGetContent(keyNode);

      if(!strcmp((const char *)key, keystr)) {
        valuePtr = keyNode->next;
        while(xmlStrcmp(valuePtr->name, (const xmlChar *)"string")){
          valuePtr = valuePtr->next;
        }
        if(valuePtr) {
          xmlChar * value = xmlNodeGetContent(valuePtr);
          strcpy(valuestr, (const char *)value);
          xmlFree(value);
        }
      }
      xmlFree(key);
    }
    keyNode = keyNode->next;
      }
    } 
    cur = cur->next;
  }
  xmlFreeDoc(doc); 
  return 1;
}

/*int main(int argc, char **argv)
{
  isXmlPlist(argv[1]);
  return 0;
}*/

