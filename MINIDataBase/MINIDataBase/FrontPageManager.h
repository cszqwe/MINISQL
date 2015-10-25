#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include "BufferManager.h"
#include "json\json.h"
using namespace std;
class FrontPageManager{
	private:
		int base;
		Json::Value jsonContent;
		Json::Reader jsonReader;
		page* content;
		int tableNumber;
		
	public:
		BufferManager* bufferManager;
		map<string,int> tableMap;
		FrontPageManager();
		int readJsonFromFile(const char* filename);
		int newTable(string tableName);
		int getTableID(string tableName);
		int save();
};
