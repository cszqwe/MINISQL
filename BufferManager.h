#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include <map> 
#include "BufferStruct.h"
#define MAX_PAGE_NUMBER 10
using namespace std;
class BufferManager{
	private:
		string fileName;
		int pageNumber;
		int pageSize; 
		map<int,page> pages;
		int ids[MAX_PAGE_NUMBER];
		FILE *file;
		
	public:
		//For return value, 1 means fail and 0 means succeed
		BufferManager(const string& name);
		string getFileName();
		void setFileName();
		int openFile(const string& name);
		int readPage(int pageID);
		int writePage(int pageID); 
		page findPage(int pageID); 
};

