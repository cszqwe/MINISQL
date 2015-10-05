#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include <map> 
#include "BufferStruct.h"
#define MAX_PAGE_NUMBER 10
using namespace std;
/*Class: BufferManager

Description:
	Move the page from disk into the memory.
	Update the pages modified in memory to disk.
*/
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
		//open the page file according to the given name
		int openFile(const string& name);
		//Get the page from the disk into pages(which is a map)
		int readPage(int pageID);
		//Update the page information to the disk
		int writePage(int pageID); 
		//Try to find a page in pages map first, if not exists, then read from file.
		page* findPage(int pageID); 
		//Update page
		int updatePage(int pageID, string newContent); 
		//New page, return the new page's ID
		int newPage();
};

