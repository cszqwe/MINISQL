#ifndef _API_H
#define _API_H

#include<string>
using namespace std;
class SavePlace{
public:
	int page;
	bool first;
};
class Error{
public:
	bool isSucc;
	string errorReason;
	Error(){}
	Error(bool flag, string eR){}
};
class AttrSaver{
public:
	int attrNum;
	string attrName[32];
	int attrType[32];
};
class OpType{
public:
	int ope;//表示判断符 1.< 2.> 3.<= 4.>= 5.== 6.!=
	int attrType;//该属性的类型 1.int 2.float 3.char* 加10表示PK
	string attrName;
	int attrPos;//该属性是这个table的第几个属性
	string value;
	OpType();
	OpType(int ope, string attrName,int attrPos, string value,int attrType){}

};
class  OrderType{
public:
	OrderType(){}
	virtual Error Execute();
	virtual ~OrderType(){}
};

	class Select :OrderType{
	private:
		string tableName;
		int opNum;
		OpType op[100];
	public:
		Select(string tableName, int opNum, OpType *op){}
		Error Execute(){}
	};
class Insert :OrderType{
private:
	string tableName;
	int attrNum;
	string attr[32];
public: 
	Insert(string tableName, int attrNum, string* attr){}
	Error Execute(){}
};
class CreatTable :OrderType{
private:
	string tableName;
	int attrNum;
	string attrName[32];
	int attrType[32];
public:
	CreatTable(string tableName, int attrNum, string *attrName, int* attrType){}
	Error Execute(){}
};
class DropTable :OrderType{
private:
	string tableName;
public:
	DropTable(string tableName){}
	Error Execute(){}
};
class CreatIndex :OrderType{
private:
	string tableName;
	string indexName;
	string attrName;
public:
	CreatIndex(string indexName, string tableName, string attrName){}
	Error Execute(){}
};
class DropIndex :OrderType{
private:
	string indexName;
public:
	DropIndex(string indexName){}
	Error Execute(){}
};
class Delete : OrderType{
private:
	string tableName;
	bool haveOp;
	OpType op;
public:
	Delete(string tableName, OpType op){
	}
	Delete(string tableName){}
	Error Execute(){}
};
#endif