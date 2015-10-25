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
		int getPrimaryKeyID(string tablePage);
		int creatTable(string tableName, int attrNum, string *attrName, int* attrType);
		AttrSaver getTable(string tableName);
		int save(int pageID);
};
