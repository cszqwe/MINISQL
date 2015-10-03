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
	int ope;//��ʾ�жϷ�
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
			attrType = catalogManager.AttrType(tableName, op[0].attrName);//����table��attr������
			re = indexManager.Find(page, op, attrType);//��index�н��в��Ҳ���
		}
		else{
			page = catalogManager.GetTablePage(tableName);//���ر���table��data��ʼλ�õ�page
			re = dataManager.Select(page, opNum, op);//��datamanager�н��в��Ҳ���
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
		page=catalogManager.GetTablePage(tableName);//���ر���table��data��ʼλ�õ�page
		PKplace = catalogManager.GetPKnum(tableName);//����table��PK��λ�ã�������Ϊ-1
		place=dataManager.InsertData(page,PKplace, attrNum, attr);//��pk˳����뵽data�У����ز����page���Ӷ���index����
		index = catalogManager.GetIndexName(tableName);//��������tableName�е�indexAttrname
		attrGet= catalogManager.GetAttrName(tableName);//��������tableName�е�Attribute
		for (int i = 0; i < index.attrNum; i++)//�����е�indexά��
			for (int j = 0; j < attrGet.attrNum; i++)
				if (index.attrName[i] == attrGet.attrName[j])
					indexManager.Insert(tableName, catalogManager.FindIndexPlace(tableName,index.attrName[i]), attr[j], place);//�ҵ�index��λ�ò�ִ�в������
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
		catalogManager.NewTable(tableName, attrNum, attrName, attrType);//�½�һ��table������
		index = catalogManager.GetIndexName(tableName);//��������tableName�е�index
		for (int i = 0; i < index.attrNum; i++)
		{

			indexManager.NewIndex(catalogManager.FindIndexPlace(tableName,index.attrName[i]));//��page�н����µ�index
		//	int p;
	//		p=indexManager.NewIndex(index.attrName[i]);//�½�index������ҳ��
	//		catalogManager.IndexPlace(index.attrName[i], p);//���½���indexλ�÷��ظ�catalog����	
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
		page = catalogManager.GetTablePage(tableName);//���ر���table��data��ʼλ�õ�page
		dataManager.DeleteTable(page);//ɾ��table������data
		index = catalogManager.GetIndexName(tableName);//��������tableName�е�indexname
		for (int i = 0; i < index.attrNum; i++)
		{
			indexManager.DeleteIndex(catalogManager.FindIndexPlace(tableName,index.attrName[i]));//ɾ��table������index
		}
		catalogManager.DeleteTable(tableName);//ɾ��catalog�д�����table��Ϣ
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
		page = catalogManager.NewIndex(tableName, attrName, indexName);//�½�index��page
		indexManager.NewIndex(page);//��page�н����µ�index
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
		page=catalogManager.FindIndex(indexName);//����indexnameѰ��index��page
		indexManager.DeleteIndex(page);//ɾ��page�µ�index
		catalogManager.DeleteIndex(indexName);//ɾ��catalog��indexname����Ϣ
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
		page = catalogManager.GetTablePage(tableName);//���ر���table��data��ʼλ�õ�page
		if (haveOp) numbers=dataManager.DeleteData(page,op);//���жϵ�ɾ��table�е�data������ɾ������
		else numbers=dataManager.DeleteData(page);//���жϵ�ɾ��table�е�data������ɾ������
		index = catalogManager.GetIndexName(tableName);//��������tableName�е�indexAttrname
		for (int i = 0; i < index.attrNum; i++){
			indexManager.Maintain(catalogManager.FindIndexPlace(tableName,index.attrName[i]));//ά����page��index�ѱ�ɾ������
		}
		re.isSucc = 1;
		re.errorReason = "Delete Data success!"+numbers;
		return re;
	}
};
//ע�� FindIndexPlace����Ϊattributename ��FindIndexΪindexname