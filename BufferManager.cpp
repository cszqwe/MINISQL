#include <cstdio>
#include <cstring>
#include "BufferManager.h"
using namespace std;
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
	if (file == NULL) return 1; else return 0;
}
int BufferManager::readPage(int pageID){
	if (file == NULL){
		LOG("File cannot be opened or created");
		return 1;
	}else{
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




int main(){
	cout << "Hello World";
}
