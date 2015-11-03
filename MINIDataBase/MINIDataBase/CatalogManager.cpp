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
		char* index;
		sprintf(index,"%d",i);
		if ((attrType[i]>=10)&&(attrType[i]<20)){
			
			sprintf(temp,"%d",i);
			string pkPosition(temp);
			jsonContent.append("{\"AttrName\": \"" + attrName[i]+"\",\"AttrType\": \"" + newNumber+"\",\"PrimaryKey\": \"" + pkPosition+"\",\"index\": \"" + string(index)+"\"}");
			newIndex(tableName, attrName[i], attrName[i]);
		}else
			jsonContent.append("{\"AttrName\": \"" + attrName[i]+"\",\"AttrType\": \"" + newNumber+"\",\"index\": \"" + string(index)+"\"}");
	}
	save(id);
	return id;
}

int CatalogManager::newIndex(string tableName, string attrName, string indexName){
	int id = getTableID(tableName);
	page* thisPage = bufferManager->findPage(id);
	string content = thisPage->content.content;
	jsonReader.parse(content,jsonContent);
	int pageID = bufferManager->newPage();
	string content = "";
	char temp[64];
	sprintf(temp,"%d",pageID);
	string pageIDs(temp);
	for (int i = 0 ; i< jsonContent.size()-1; i++)
	if ((jsonContent[i]["AttrName"] == attrName)){
		jsonContent.append("{\"AttrName\": \"" + attrName+"\",\"IndexName\": \"" + indexName+"\",\"AttrType\": \"" + jsonContent[i]["AttrType"].asString()+"\",\"pageID\": \"" + pageIDs+"\"}");
	}

	save(pageID);
	return pageID;
}
void CatalogManager::deleteIndex(string tableName, string indexName){
	int id = getTableID(tableName);
	page* thisPage = bufferManager->findPage(id);
	string content = thisPage->content.content;
	jsonReader.parse(content,jsonContent);
	int pageID = bufferManager->newPage();
	string content = "";
	char temp[64];
	sprintf(temp,"%d",pageID);
	string pageIDs(temp);
	for (int i = 0 ; i< jsonContent.size()-1; i++)
	if ((jsonContent[i]["IndexName"] == indexName)){
		jsonContent[i]["IndexName"] = "";
		jsonContent[i]["AttrName"] = "";
	}

	save(pageID);
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
	newAttr.attrNum = 0;
	int zero = 0;
	newAttr.attrNum = jsonContent.size() - 1;
	for (int i = 0 ; i< jsonContent.size()-1; i++)
		if ((jsonContent[i]["AttrName"].asString()!= "")&&(jsonContent[i]["IndexName"].asString()== "")){
			newAttr.attrName[newAttr.attrNum] = jsonContent[i]["AttrName"].asString();
			newAttr.attrType[newAttr.attrNum] = jsonContent[i]["AttrType"].asInt();
			newAttr.attrNum++;
		}
	return newAttr;
}
AttrSaver CatalogManager::getIndexName(string tableName){
	AttrSaver tmpSaver;
	tmpSaver.attrNum = 0;
	int id = getTableID(tableName);
	page* thisPage = bufferManager->findPage(id);
	string content = thisPage->content.content;
	jsonReader.parse(content,jsonContent);
	for (int i = 0 ; i< jsonContent.size()-1; i++)
		if (jsonContent[i]["IndexName"].asString()!= ""){
			tmpSaver.attrName[tmpSaver.attrNum] = jsonContent[i]["IndexName"].asString();
			tmpSaver.attrType[tmpSaver.attrNum] = jsonContent[i]["AttrType"].asInt();
			tmpSaver.attrNum++;
		}
	//-1 means not exist
	return tmpSaver;
}
int CatalogManager::getTablePage(string tableName){
	int id = getTableID(tableName);
	page* thisPage = bufferManager->findPage(id);
	string content = thisPage->content.content;
	jsonReader.parse(content,jsonContent);
	for (int i = 0 ; i< jsonContent.size()-1; i++)
		if (jsonContent[i]["pageID"].asString()!= ""){
			return jsonContent[i]["pageID"].asInt();
		}
	//-1 means not exist
	return -1;
}
int CatalogManager::getPKNum(string tableName){
	int id = getTableID(tableName);
	page* thisPage = bufferManager->findPage(id);
	string content = thisPage->content.content;
	jsonReader.parse(content,jsonContent);
	for (int i = 0 ; i< jsonContent.size()-1; i++)
		if (jsonContent[i]["PrimaryKey"].asString()!= ""){
			return jsonContent[i]["PrimaryKey"].asInt();
		}
	//-1 means not exist
	return -1;
}
int CatalogManager::getPKType(string tableName, int index){
	int id = getTableID(tableName);
	page* thisPage = bufferManager->findPage(id);
	string content = thisPage->content.content;
	jsonReader.parse(content,jsonContent);
	for (int i = 0 ; i< jsonContent.size()-1; i++)
		if (jsonContent[i]["index"].asString()!= ""){
			if (jsonContent[i]["index"].asInt() == index)
				return jsonContent[i]["AttrType"].asInt();
		}
	//-1 means not exist
	return -1;
}
int CatalogManager::findIndexPlace(string tableName, string indexName){
	int id = getTableID(tableName);
	page* thisPage = bufferManager->findPage(id);
	string content = thisPage->content.content;
	jsonReader.parse(content,jsonContent);
	for (int i = 0 ; i< jsonContent.size()-1; i++)
		if (jsonContent[i]["IndexName"].asString() == indexName){
			return jsonContent[i]["pageID"].asInt();
		}
	//-1 means not exist
	return -1;
}
int CatalogManager::returnType(string tableName, string attrName){
	int id = getTableID(tableName);
	page* thisPage = bufferManager->findPage(id);
	string content = thisPage->content.content;
	jsonReader.parse(content,jsonContent);
	for (int i = 0 ; i< jsonContent.size()-1; i++)
		if (jsonContent[i]["AttrName"] == attrName){
			return jsonContent[i]["AttrType"].asInt();
		}
	//-1 means not exist
	return -1;
}

int main(){
	cout << "test" <<endl;
}