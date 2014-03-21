//
//  parserdef.cpp
//  xcodeutils
//
//  Created by Yu Yingshen on 11-7-31.
//  Copyright 2011年 HOME. All rights reserved.
//

#include "pbxprojdef.h"
#include <iostream>
#include <sstream>


////////////////////////////////////////////////////////////////////////////////
PBXBlock::PBXBlock()
{
}

PBXBlock::~PBXBlock()
{
    mValueMap.clear();
    
    PBXItemList::iterator iter = mStatements.begin();
    PBXItemList::iterator end  = mStatements.end();
    for (; iter != end; ++iter) {
        delete *iter;
    }
}

void PBXBlock::addStatement(PBXItem* statement)
{
    mStatements.push_back(statement);
    const PBXAssignment* assign = dynamic_cast<const PBXAssignment*>(statement);
    if (assign) {
        std::pair<const std::string, const PBXValue*> valuePair(assign->key(), assign->value());
        mValueMap.insert(valuePair);
    }
}

const PBXValue* PBXBlock::valueForKey(const char* name) const
{
    PBXValueMap::const_iterator itor = mValueMap.find(name);
    if (itor != mValueMap.end()) {
        return  itor->second;
    }
    return NULL;
}
    
PBXItemList::const_iterator PBXBlock::begin() const
{
    return mStatements.begin();
}

PBXItemList::const_iterator PBXBlock::end()   const
{
    return mStatements.end();
}

const PBXBlock* PBXBlock::cast(const PBXValue* value)
{
    return dynamic_cast<const PBXBlock*> (value);
}

////////////////////////////////////////////////////////////////////////////////
PBXValue::PBXValue()
{
}

PBXValue::~PBXValue()
{
}
bool PBXValue::hasComment() const
{
    return !mComment.empty();
}
const char* PBXValue::comment() const
{
    return mComment.c_str();
}
void PBXValue::setComment(const char* newComment)
{
    mComment = newComment;
}

////////////////////////////////////////////////////////////////////////////////
PBXValueRef::PBXValueRef(const char* id)
{
    mId = id;
}
const char* PBXValueRef::id() const
{   
    return mId.c_str();
}

const PBXValueRef* PBXValueRef::cast(const PBXValue* value)
{
    return dynamic_cast<const PBXValueRef*> (value);
}

////////////////////////////////////////////////////////////////////////////////
PBXText::PBXText(const char* text)
{
    mText = text;
}
const char* PBXText::text() const
{
    return mText.c_str();
}

const PBXText* PBXText::cast(const PBXValue* value)
{
    return dynamic_cast<const PBXText*> (value);
}


////////////////////////////////////////////////////////////////////////////////
PBXInteger::PBXInteger(int intValue)
{
    mIntValue = intValue;
}
int PBXInteger::intValue() const
{
    return mIntValue;
}

////////////////////////////////////////////////////////////////////////////////
PBXItem::PBXItem()
{
}

PBXItem::~PBXItem()
{
}

////////////////////////////////////////////////////////////////////////////////
PBXAssignment::PBXAssignment(const char* key, PBXValue* value)
{
    mKey = key;
    mValue = value;
}

PBXAssignment::~PBXAssignment()
{
    delete mValue;
}

const char* PBXAssignment::key() const
{
    return mKey.c_str();
}

const char* PBXAssignment::keyComment() const
{
    return mKeyComment.c_str();
}

void PBXAssignment::setKeyComment(const char* keyComment)
{
    mKeyComment = keyComment;
}

bool PBXAssignment::hasKeyComment() const
{
    return !mKeyComment.empty();
}

const PBXValue* PBXAssignment::value() const
{
    return mValue;
}

////////////////////////////////////////////////////////////////////////////////
PBXArray::PBXArray()
{
}
PBXArray::~PBXArray()
{
    PBXValueList::iterator iter = mValues.begin();
    PBXValueList::iterator end  = mValues.end();
    for (; iter != end; ++iter) {
        delete *iter;
    }
}
void PBXArray::addValue(PBXValue* value)
{
    mValues.push_back(value);
}
PBXValueList::const_iterator PBXArray::begin() const
{
    return mValues.begin();
}
PBXValueList::const_iterator PBXArray::end()   const
{
    return mValues.end();
}
size_t PBXArray::count() const
{
	return mValues.size();
}

const PBXArray* PBXArray::cast(const PBXValue* value)
{
    return dynamic_cast<const PBXArray*> (value);
}

////////////////////////////////////////////////////////////////////////////////
PBXCommentItem::PBXCommentItem(const std::string& comment)
{
    mComment = comment;
}

std::string PBXCommentItem::comment() const
{
    return mComment;
}

////////////////////////////////////////////////////////////////////////////////
PBXFile::PBXFile()
{
    mBlock = NULL;
}
PBXFile::~PBXFile()
{
    delete mBlock;
}

const char* PBXFile::preamble() const
{
    return mPreamble.c_str();
}

void PBXFile::setPreamble(const char* preamble)
{
    mPreamble = preamble;
}

const PBXBlock* PBXFile::block() const
{
    return mBlock;
}

void PBXFile::setBlock(PBXBlock* block)
{
    mBlock = block;
}

const PBXValue* PBXFile::valueForKey(const char* keyName) const
{
    return mBlock ? mBlock->valueForKey(keyName) : NULL;
}

const PBXValue* PBXFile::valueForKeyPath(const PBXBlock* block, const char* keyPath) const
{
    if (block == NULL)
        return NULL;
    
    const PBXValue* value = NULL;
    const PBXBlock* parent = block;
    std::istringstream iss(keyPath);
    std::string key;
    
    while (getline(iss, key, '.')) {
        value = parent->valueForKey(key.c_str());  
        const PBXValueRef* valueRef = PBXValueRef::cast(value);
        if (valueRef) {
            const PBXBlock* child = PBXBlock::cast(deref(valueRef));
            if (child) {
                parent = child;
            }
        }
    }
    
    return value;
}

const PBXValue* PBXFile::valueForKeyPath(const char* keyPath) const
{
    return valueForKeyPath(mBlock, keyPath);
}

const PBXValue* PBXFile::deref(const PBXValueRef* ref) const
{
    if (!ref) return NULL;
    
    const PBXBlock* objects = dynamic_cast<const PBXBlock*>(valueForKey("objects"));
    return objects ? objects->valueForKey(ref->id()) : NULL;
}


#include <sys/stat.h>
#include <sys/errno.h>


extern "C" bool loadDocument(const char* projectPath, PBXFile **pDoc);
extern "C" bool loadProject(const char* projectPath, PBXFile **pDoc)
{
    if (projectPath == NULL || pDoc == NULL)
        return false;
    
    //determine file exists.
    struct stat st;
    if (stat(projectPath, &st) == ENOENT)
        return false;
    
    std::string projectFile =  projectPath;
    if (st.st_mode & S_IFDIR)
    {
        projectFile += "/project.pbxproj";
    }
    
    return loadDocument(projectFile.c_str(), pDoc);
}

