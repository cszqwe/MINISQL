#include <cstdio>
#include <cstring>
#include "BufferManager.h"
using namespace std;
/* 
Function: LOG
Type: VOID
Argus: (string message)
Description: LOG the error Message
*/
void LOG(string message){
	cout << "error " << message << endl;
}

BufferManager::BufferManager (const string& name){
	this->fileName = name;
	if (openFile(name) == 1){
		LOG("File cannot be opened or created");	
	}
	pages.clear();
}
string BufferManager::getFileName(){
	return this->fileName;
}
int BufferManager::openFile(const string& name){
	file = fopen(name.c_str(),"r+");
	if (file == NULL) return 1; else{
		fseek(file, 0, SEEK_END);
		pageSize = (ftell(file) / PAGE_SIZE);
		fseek(file, 0, SEEK_SET);
		return 0;
	}
}
int BufferManager::readPage(int pageID){
	if (file == NULL){
		LOG("File cannot be opened or created");
		return 1;
	}else{
		if (pageID >= pageSize) return FAIL;
		page newPage;
		frame bf_ds;
		fseek(file,pageID * PAGE_SIZE,SEEK_SET);
		fread(&bf_ds,sizeof(bf_ds),1,file);
		newPage.content = bf_ds;
		newPage.pageId = pageID;
		newPage.isModified = false; 
		pages.insert(pair<int,page>(pageID,newPage)); 
		return 0;
	}
}
page* BufferManager::findPage(int pageID){
	map<int,page>::iterator it = pages.find(pageID);
	if (it != pages.end()){
		return &it->second;
	}else{
		if (!readPage(pageID)){
			it = pages.find(pageID);
			return &it->second;
		}else{
			return NULL;
		}
	}
}
int BufferManager::writePage(int pageID){
	if (pageID >= pageSize) return FAIL;
	map<int,page>::iterator it = pages.find(pageID);
	page* changedPage = &it->second;
	fseek(file, pageID*PAGE_SIZE , SEEK_SET);
	int bytesw = (int)fwrite(&(changedPage->content.content),sizeof(changedPage->content.content),1,file);
	return SUCCESS;
}
int BufferManager::updatePage(int pageID,string newContent){
	map<int,page>::iterator it = pages.find(pageID);
	if (it == pages.end()) return FAIL;
	page* changedPage = &it->second;
	for (int i = 0; i < newContent.length(); i++){
		changedPage->content.content[i] = newContent[i];
	}
	changedPage->isModified = true;
	return SUCCESS;
}
int BufferManager::newPage(){
	fseek(file, 0 ,SEEK_END);
	frame newFrame;
	memset(newFrame.content,' ',sizeof(newFrame.content));
	fwrite(&(newFrame),sizeof(newFrame),0,file);
	pageSize++;
	return pageSize - 1;
}
int BufferManager::getPageSize(){
	return pageSize;
}
