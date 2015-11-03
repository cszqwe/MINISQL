#include "CatalogManager.h"

CatalogManager::CatalogManager(){
	frontPageManager = new FrontPageManager();
	bufferManager = frontPageManager->bufferManager;
}
int CatalogManager::getTableID(string tableName){
	return frontPageManager->tableMap.find(tableName)->second;
}

int CatalogManager::creatTable(string tableName, int attrNum, string *attrName, int* attrType){
	int id = bufferManager->newPage();
	page* thisPage = bufferManager->findPage(id);
	string content = "";
	char temp[64];
	sprintf(temp,"%d",id);
	string pageID(temp);

	content= "[{\"TableName\": \"" + tableName+"\",\"pageID\": \"" + pageID+"\"}]";
	jsonReader.parse(content,jsonContent);
	for (int i = 0; i < attrNum; i++){
		char* newNumber;
		sprintf(newNumber,"%d",attrType[i]);
		if ((attrType[i]>=10)&&(attrType[i]<20)){
			newIndex(tableName, attrName[i], attrName[i]);
		}else
			jsonContent.append("{\"AttrName\": \"" + attrName[i]+"\",\"AttrType\": \"" + newNumber+"\"}");
	}
	save(id);
	return id;
}

int CatalogManager::newIndex(string tableName, string attrName, string indexName){
	int id = bufferManager->newPage();
	string content = "";
	char temp[64];
	sprintf(temp,"%d",id);
	string pageID(temp);
	jsonContent.append("{\"AttrName\": \"" + attrName+"\",\"IndexName\": \"" + indexName+"\",\"pageID\": \"" + pageID+"\"}");
}


int CatalogManager::save(int pageID){
	bufferManager->updatePage(0,jsonContent.asString());
	bufferManager->writePage(pageID);
	return SUCCESS;
}
AttrSaver CatalogManager::getTable(string tableName){
	int id = getTableID(tableName);
	page* thisPage = bufferManager->findPage(id);
	string content = thisPage->content.content;
	jsonReader.parse(content,jsonContent);
	AttrSaver newAttr;
	int zero = 0;
	newAttr.attrNum = jsonContent.size() - 1;
	for (int i = 0 ; i< jsonContent.size()-1; i++){
		newAttr.attrName[i] = jsonContent[i]["AttrName"].asString();
		newAttr.attrType[i] = jsonContent[i]["AttrType"].asInt();
	}
	return newAttr;
}
string* CatalogManager::getIndexName(){
	
}