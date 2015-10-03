#include<string>
#include"DataManager.h"
#include"IndexManager.h"
using namespace std;
DataManager dataManager;
IndexManager indexManager;
class SavePlace{
public:
	int page;
	int offset;
};
class Error{
public:
	bool isSucc;
	string errorReason;
	Error(){}
	Error(bool flag, string eR){
		isSucc = flag;
		errorReason = eR;
	}
};
class AttrSaver{
public:
	int attrNum;
	string attrName[32];
	int attrType[32];
};

class OpType{
public:
	int ope;//表示判断符
	string attrName;
	string value;
	OpType(){}
	OpType(int ope, string attrName, string value){
		this->ope = ope;
		this->attrName = attrName;
		this->value = value;
	}
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
	Select(string tableName, int opNum, OpType *op){
		this->tableName = tableName;
		this->opNum = opNum;
		for (int i = 0; i < opNum; i++)
			this->op[i] = op[i];
	}
	Error Execute(){
		Error re;
		bool flag = 1;
		int page,attrType;

		if ((opNum == 1) && (page=catalogManager.FindIndexPlace(tableName, op[0].attrName)))
		{
			attrType = catalogManager.AttrType(tableName, op[0].attrName);//返回table中attr的类型
			re = indexManager.Find(page, op, attrType);//在index中进行查找操作
		}
		else{
			page = catalogManager.GetTablePage(tableName);//返回保存table中data起始位置的page
			re = dataManager.Select(page, opNum, op);//在datamanager中进行查找操作
		}

		return re;
	}
};
class Insert :OrderType{
private:
	string tableName;
	int attrNum;
	string attr[32];
public:
	Insert(string tableName, int attrNum, string* attr){
		this->tableName = tableName;
		this->attrNum = attrNum;
		for (int i = 0; i < attrNum; i++)
			this->attr[i] = attr[i];
	}
	Error Execute(){
		SavePlace place;
		AttrSaver index,attrGet;
		Error re;
		int page,PKplace;
		page=catalogManager.GetTablePage(tableName);//返回保存table中data起始位置的page
		PKplace = catalogManager.GetPKnum(tableName);//返回table中PK的位置，若无则为-1
		place=dataManager.InsertData(page,PKplace, attrNum, attr);//按pk顺序插入到data中，返回插入的page，从而让index连接
		index = catalogManager.GetIndexName(tableName);//返回所有tableName中的indexAttrname
		attrGet= catalogManager.GetAttrName(tableName);//返回所有tableName中的Attribute
		for (int i = 0; i < index.attrNum; i++)//将所有的index维护
			for (int j = 0; j < attrGet.attrNum; i++)
				if (index.attrName[i] == attrGet.attrName[j])
					indexManager.Insert(tableName, catalogManager.FindIndexPlace(tableName,index.attrName[i]), attr[j], place);//找到index的位置并执行插入操作
		re.isSucc = 1;
		re.errorReason = "Insert success!";
		return re;
	}
};
class CreatTable :OrderType{	
private:
	string tableName;
	int attrNum;
	string attrName[32];
	int attrType[32];
public:
	CreatTable(string tableName, int attrNum, string *attrName, int* attrType){
		this->tableName = tableName;
		this->attrNum = attrNum;
		for (int i = 0; i < attrNum; i++)
		{
			this->attrName[i] = attrName[i];
			this->attrType[i] = attrType[i];
		}
	}
	Error Execute(){
		Error re;
		AttrSaver index;
		catalogManager.NewTable(tableName, attrNum, attrName, attrType);//新建一个table的属性
		index = catalogManager.GetIndexName(tableName);//返回所有tableName中的index
		for (int i = 0; i < index.attrNum; i++)
		{

			indexManager.NewIndex(catalogManager.FindIndexPlace(tableName,index.attrName[i]));//在page中建立新的index
		//	int p;
	//		p=indexManager.NewIndex(index.attrName[i]);//新建index，返回页数
	//		catalogManager.IndexPlace(index.attrName[i], p);//将新建的index位置返回给catalog保存	
		}
		re.isSucc = 1;
		re.errorReason = "Creat table success!";
		return re;
	}
};
class DropTable :OrderType{
private:
	string tableName;
public:
	DropTable(string tableName){
		this->tableName = tableName;
	}
	Error Execute(){
		Error re;
		int page;
		AttrSaver index;
		page = catalogManager.GetTablePage(tableName);//返回保存table中data起始位置的page
		dataManager.DeleteTable(page);//删除table中所有data
		index = catalogManager.GetIndexName(tableName);//返回所有tableName中的indexname
		for (int i = 0; i < index.attrNum; i++)
		{
			indexManager.DeleteIndex(catalogManager.FindIndexPlace(tableName,index.attrName[i]));//删除table中所有index
		}
		catalogManager.DeleteTable(tableName);//删除catalog中存留的table信息
		re.isSucc = 1;
		re.errorReason = "Drop table success!";
		return re;
	}
};
class CreatIndex :OrderType{
private:
	string tableName;
	string indexName;
	string attrName;
public:
	CreatIndex(string indexName, string tableName, string attrName){
		this->tableName = indexName;
		this->indexName = tableName;
		this->attrName = attrName;
	}
	Error Execute(){
		Error re;
		int page;
		page = catalogManager.NewIndex(tableName, attrName, indexName);//新建index的page
		indexManager.NewIndex(page);//在page中建立新的index
		re.isSucc = 1;
		re.errorReason = "Creat Index success";
		return re;
	}
};
class DropIndex :OrderType{
private:
	string indexName;
public:
	DropIndex(string indexName){
		this->indexName = indexName;
	}
	Error Execute(){
		Error re;
		int page;
		page=catalogManager.FindIndex(indexName);//根据indexname寻找index的page
		indexManager.DeleteIndex(page);//删除page下的index
		catalogManager.DeleteIndex(indexName);//删除catalog中indexname的信息
		re.isSucc = 1;
		re.errorReason = "Drop Index success!";
		return re;
	}
};
class Delete : OrderType{
private:
	string tableName;
	bool haveOp;
	OpType op;
public:
	Delete(string tableName, OpType op){
		this->tableName = tableName;
		haveOp = true;
		this->op = op;
	}
	Delete(string tableName){
		this->tableName = tableName;
		haveOp = false;
	}
	Error Execute(){
		Error re;
		AttrSaver index;
		int numbers, page;
		page = catalogManager.GetTablePage(tableName);//返回保存table中data起始位置的page
		if (haveOp) numbers=dataManager.DeleteData(page,op);//有判断的删除table中的data，返回删除个数
		else numbers=dataManager.DeleteData(page);//无判断的删除table中的data，返回删除个数
		index = catalogManager.GetIndexName(tableName);//返回所有tableName中的indexAttrname
		for (int i = 0; i < index.attrNum; i++){
			indexManager.Maintain(catalogManager.FindIndexPlace(tableName,index.attrName[i]));//维护掉page中index已被删除的项
		}
		re.isSucc = 1;
		re.errorReason = "Delete Data success!"+numbers;
		return re;
	}
};
//注意 FindIndexPlace参数为attributename 而FindIndex为indexname