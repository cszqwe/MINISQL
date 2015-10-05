#ifndef _DATAMANAGER_H
#define _DATAMANAGER_H
#include<sstream>
#include<string>
using namespace std;
class DataManager{
private:
	bool compare(string a, string b, int type){}
public:
	SavePlace InsertData(int page, int PKplace, int PKType,int attrNum, string*attr){}
	int DeleteData(int page, OpType op){}
	int DeleteData(int page){}
	Error Select(int page, int opNum, OpType *op){}

};
#endif