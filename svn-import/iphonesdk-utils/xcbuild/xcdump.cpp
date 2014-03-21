//
//  main.cpp
//  xcodeutils
//
//  Created by Yu Yingshen on 11-7-29.
//  Copyright 2011年 HOME. All rights reserved.
//

#include "pbxprojdef.h"
#include <string>
#include <iostream>
#include <assert.h>

static void printHelp();
static bool shouldCompact(const PBXAssignment* pAssignment);
static void dumpDocument(const PBXFile* pDoc);
static void dumpBlock(const PBXBlock* pBlock, bool compact = false);
static void dumpArray(const PBXArray* pArray, bool compact = false);
static void dumpStatement(const PBXItem* pStmt);
static void dumpValue(const PBXValue* pValue);
static void dumpAssignment(const PBXAssignment* pAssignment);
static void dumpCommentStatement(const PBXCommentItem* pCommentStmt);

static int ident = 0;
static void tab() 
{
    ++ident;
}
static void unTab()
{
    --ident;
}
static const std::string space()
{
    static std::string space_var;
    space_var = std::string(ident, '\t');
    return space_var;
}


int main (int argc, const char * argv[])
{
    if (argc < 2) {
        printHelp();
        return -1;
    }
    
    std::string projectDir(argv[1]);
    std::string projectFile(projectDir );
    
    PBXFile * pDoc = NULL;
    if (!loadProject(projectFile.c_str(), &pDoc)) {
        std::cout<<"failed to load "<<projectFile<<std::endl;
        return -1;
    }
    
    dumpDocument(pDoc);
    
    delete pDoc;
    return 0;
}


void printHelp()
{
    std::string help = "xcdump MyProject.xcodeproj";
    std::cout << help << std::endl;
}

void dumpDocument(const PBXFile* pDoc)
{
    std::cout << pDoc->preamble() << std::endl;
    dumpBlock(pDoc->block());
    std::cout << std::endl;
}




void dumpBlock(const PBXBlock* pBlock, bool compact)
{
    PBXItemList::const_iterator itor = pBlock->begin();
    PBXItemList::const_iterator end  = pBlock->end();
    
    if (!compact) {
        std::cout << "{" << std::endl;
        tab();
        for (; itor != end; ++itor) {
            dumpStatement(*itor);
            std::cout << std::endl;
        }
        unTab();
        std::cout << space() << "}";
    }
    else {
        std::cout << "{";
        for (; itor != end; ++itor) {
            dumpStatement(*itor);
            std::cout << " ";
        }
        std::cout << "}";
    }
}

void dumpArray(const PBXArray* pArray, bool compact)
{
    PBXValueList::const_iterator itor = pArray->begin();
    PBXValueList::const_iterator end  = pArray->end();
    
    if (!compact) {
        std::cout << "(" << std::endl;
        tab();
        for (; itor != end; ++itor) {
            std::cout << space();
            dumpValue(*itor);
            std::cout << "," << std::endl;
        }
        unTab();
        std::cout << space() << ")";
    }
    else {
        std::cout << "(";
        for (; itor != end; ++itor) {
            dumpValue(*itor);
            std::cout << ", ";
        }
        std::cout << ")";
    }
}

void dumpStatement(const PBXItem* pStmt)
{
    const PBXAssignment* assignment = dynamic_cast<const PBXAssignment*>(pStmt);
    if (assignment) {
        dumpAssignment(assignment);
    }
    const PBXCommentItem* comment = dynamic_cast<const PBXCommentItem*>(pStmt);
    if (comment) {
      dumpCommentStatement(comment);
    }
}

void dumpValue(const PBXValue* pValue)
{
    const PBXInteger*     integer = dynamic_cast<const PBXInteger*>(pValue);
    const PBXText*        text    = dynamic_cast<const PBXText*>(pValue);
    const PBXValueRef*    ref     = dynamic_cast<const PBXValueRef*>(pValue);
    const PBXArray*       array   = dynamic_cast<const PBXArray*>(pValue);
    const PBXBlock*       block   = dynamic_cast<const PBXBlock*>(pValue);
    
    if (integer) {
        std::cout << integer->intValue();
    }
    else if (text) {
        std::cout << text->text();
    }
    else if (ref) {
        std::cout << ref->id();
    }
    else if (array) {
        dumpArray(array);
    }
    else if (block) {
        dumpBlock(block);
    }
    else {
        assert(false);
    }
    
    if (pValue->hasComment()) {
      std::cout << " " << pValue->comment();
    }
}

void dumpAssignment(const PBXAssignment* pAssignment)
{
    std::string key = pAssignment->key();
    std::string keyComment = pAssignment->keyComment();

    if (pAssignment->hasKeyComment()) {
        std::cout << space() << key << " " << keyComment << " = ";
    }
    else {
        std::cout << space() << key << " = ";
    }
    
    dumpValue(pAssignment->value());
    std::cout << ";";
}

void dumpCommentStatement(const PBXCommentItem* pCommentStmt)
{
    std::string comment = pCommentStmt->comment();
    bool        found   = comment.find("Begin") != std::string::npos;
    if (found) {
        std::cout << std::endl;
    }
    std::cout << pCommentStmt->comment();
}