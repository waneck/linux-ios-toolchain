#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlerror.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>

#ifndef PATH_MAX
#include <linux/limits.h>
#endif


#include "plist/plist.h"
#include "plutil.h"


int willcompile = -1;

void usage() {
  printf("plist compiler/decompiler utility\n\n");
  printf("usage:\n");
  printf("-c <plists>: convert xml plist to binary format\n");
  printf("-d <plists>: convert binary plist to xml format\n");
  printf("-h         : show this message\n");
  printf("<plists>   : convert plist automatically\n");
  exit(0);
}

void _nullGenericErrorFunc(void* ctxt, char* msg, ...)
{
  //disable all warnings and errors of libxml2.
}

int is_xml_plist(char *plistfile)
{
  if(access(plistfile, R_OK) != 0)
    return 0;
  xmlSetGenericErrorFunc(NULL, (xmlGenericErrorFunc)_nullGenericErrorFunc);

  xmlDocPtr doc;
  xmlNodePtr cur;
  doc = xmlReadFile(plistfile, "UTF-8", 0);
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

int is_binary_plist(char *plistfile)
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

int is_plist(char *plistfile)
{
  return (is_binary_plist(plistfile) || is_xml_plist(plistfile));
}

void compile_stub(char *plistfile, int compile)
{
  if(!plistfile)
    return; 
  FILE *file_in = fopen(plistfile, "rb");
  if(!file_in)
    return;
  struct stat st;
  stat(plistfile, &st);
  char * plist_entire = (char *) malloc(sizeof(char) * (st.st_size + 1));
  int read_size = fread(plist_entire, sizeof(char), st.st_size, file_in);
  fclose(file_in);
 
  plist_t root_node = NULL;
  char *plist_out = NULL;

  int size;
  if(compile) {
    plist_from_xml(plist_entire, read_size, &root_node);
    plist_to_bin(root_node, &plist_out, &size);
  } else { 
    plist_from_bin(plist_entire, read_size, &root_node);
    plist_to_xml(root_node, &plist_out, &size);
  }
  plist_free(root_node);
  free(plist_entire);

  char * outfilename = (char *)malloc(PATH_MAX);
  sprintf(outfilename, "%s.tmp", plistfile);
  if (plist_out) {
    FILE *oplist = fopen(outfilename, "wb");
    if(!oplist)
      return;
    fwrite(plist_out, size, sizeof(char), oplist);
    fclose(oplist);
    free(plist_out);
  }
  unlink(plistfile);
  rename(outfilename, plistfile);
}

void decompile(char *plistfile)
{
  printf("convert %s to xml\n", plistfile);
  compile_stub(plistfile, 0);
} 

void compile(char *plistfile)
{
  printf("convert %s to binary\n", plistfile);
  compile_stub(plistfile, 1);
}

int main(int argc, char **argv)
{
  char *cmd_name = argv[0];
  if(argc == 1)
    usage();

  char optflag;
  while((optflag = getopt(argc, argv, "hcd")) != -1) {
    switch(optflag) {
    case 'c':
      willcompile = 1;
      break;
    case 'd':
      willcompile = 0;
      break;
    case 'h':
    default:
      usage();
    }
  }

  argc = argc - optind;
  argv = argv + optind;
  int i;
  for(i = 0; i < argc; i++) {
    if(willcompile == 1 && is_xml_plist(argv[i]) == 1) { 
      compile(argv[i]);
    }
    else if(willcompile == 0 && is_binary_plist(argv[i]) == 1) { 
      decompile(argv[i]);
    }
    else if(willcompile == -1 && is_plist(argv[i])){
      if(is_xml_plist(argv[i]))
        compile(argv[i]);
      else
        decompile(argv[i]);
    }
    else 
        continue;
  }
  return 0;
}
