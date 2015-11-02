#ifndef _DATAMANAGER_H
#define _DATAMANAGER_H
#include"API.h"
#include"DataManager.h"
#include<sstream>
#include<string>
#include<iostream>
#include"BufferManager.h"
using namespace std;
bool Compare(string a, string b, int comType, int attrType);
BufferManager bufferManager("data");
class DataManager{
public:
	SavePlace InsertData(int page, int PKplace, int PKType,int attrNum, string*attr){}
	int DeleteData(int page, OpType op){}
	int DeleteData(int page){}
	Error Select(int page, int opNum, OpType *op){}

};
#endif