#include "FrontPageManager.h"

FrontPageManager::FrontPageManager(){
	base = 0;
	bufferManager = new BufferManager("mini.data");
	tableMap.clear();
	if (bufferManager->getPageSize() == 0){
		
		bufferManager->newPage();
		content = bufferManager->findPage(0);
		content->modifyContent("[{\"TableNumber\": \"0\"}]");
		jsonReader.parse(content->content.content,jsonContent);
		bufferManager->updatePage(0,content->content.content);
	}else{
		jsonReader.parse(content->content.content,jsonContent);
		
		tableNumber = jsonContent[base]["TableNumber"].asInt();
		for (int i = 1; i< jsonContent.size(); i++){
			tableMap.insert(pair<string,int>(jsonContent[i]["tableName"].asString(),jsonContent[i]["pageID"].asInt()));
		}
	}

}/*
int FrontPageManager::getTableID(string tableName){
	return tableMap.find(tableName)->second;
}
int FrontPageManager::newTable(string tableName){
	tableNumber++;
	char* newNumber;
	sprintf(newNumber,"%d",tableNumber);
	jsonContent[base]["TableNumber"] = newNumber;
	int newID = bufferManager->newPage();
	sprintf(newNumber,"%d",newID);
	jsonContent.append("{\"TableName\":\""+tableName+"\",\"pageID\":"+ newNumber+ "}");
	return tableMap.find(tableName)->second;
}
int FrontPageManager::save(){
	bufferManager->updatePage(0,jsonContent.asString());
	bufferManager->writePage(0);
	return SUCCESS;
}
*/
int main(){
}