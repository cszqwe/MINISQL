#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include "json\json.h"
#include "temp.h"
#include "FrontPageManager.h"
using namespace std;
class CatalogManager{
	private:
		static int base;
		Json::Value jsonContent;
		Json::Reader jsonReader;
		page* content;
		int tableNumber;
		BufferManager* bufferManager;
		map<string,int> tableMap;
		FrontPageManager* frontPageManager;
	public:
		CatalogManager();
		int getTableID(string tableName);
		int newIndex(string tableName, string attrName, string indexName);
		int getPrimaryKeyID(string tablePage);
		int creatTable(string tableName, int attrNum, string *attrName, int* attrType);
		AttrSaver getTable(string tableName);
		AttrSaver getIndexName(string tableName);
		int save(int pageID);
		int getTablePage(string tableName);
		int getPKNum(string tableName);
		int getPKType(string tableName, int index);
		int findIndexPlace(string tableName, string indexName);
		int returnType(string tableName, string attrName);
		void deleteIndex(string tableName, string indexName);
};
