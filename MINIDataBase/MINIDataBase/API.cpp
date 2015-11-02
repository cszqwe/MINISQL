#include<string>
#include"DataManager.h"
#include"IndexManager.h"
#include"CatalogManager.h"
#include"FrontPageManager.h"
#include"API.h"
using namespace std;
DataManager dataManager;
IndexManager indexManager;
CatalogManager catalogManager;
Error::Error(){}
Error::Error(bool flag, string eR){
		isSucc = flag;
		errorReason = eR;
}

OpType::OpType(){}
OpType::OpType(int ope, string attrName, int attrPos, string value, int attrType){
		this->ope = ope;
		this->attrName = attrName;
		this->value = value;
		this->attrPos = attrPos;
		this->attrType = attrType;
	}


	Select::Select(string tableName, int opNum, OpType *op){
		this->tableName = tableName;
		this->opNum = opNum;
		for (int i = 0; i < opNum; i++)
			this->op[i] = op[i];
	}
	Error Select::Execute(){
		Error re;
		bool flag = 1;
		int page,attrType;

		if ((opNum == 1) && (op[0].ope!=5)&&(op[0].ope!=6)&&(page=catalogManager.FindIndexPlace(tableName, op[0].attrName)))
		{
			attrType = catalogManager.AttrType(tableName, op[0].attrName);//����table��attr������
			re = indexManager.Find(page,op[0]);//��index�н��в��Ҳ���
		}
		else{
			page = catalogManager.GetTablePage(tableName);//���ر���table��data��ʼλ�õ�page
			re = dataManager.Select(page, opNum, op);//��datamanager�н��в��Ҳ���
		}

		return re;
	}
	Insert::Insert(string tableName, int attrNum, string* attr){
		this->tableName = tableName;
		this->attrNum = attrNum;
		for (int i = 0; i < attrNum; i++)
			this->attr[i] = attr[i];
	}
	Error Insert::Execute(){
		int place;
		AttrSaver index,attrGet;
		Error re;
		SavePlace p;
		int page,PKplace,PKType;
		page=catalogManager.GetTablePage(tableName);//���ر���table��data��ʼλ�õ�page
		PKplace = catalogManager.GetPKnum(tableName);//����table��PK��λ�ã�������Ϊ-1
		PKType = catalogManager.GetType(tableName,PKplace);//��ȡ��place�����Ե�����
		p=dataManager.InsertData(page,PKplace, PKType,attrNum, attr);//��pk˳����뵽data�У����ز����page���Ӷ���index����
		if (p.first) catalogManager.ChangeFirstPage(tableName, p.page);//��tablename��������ҳ��Ϊpage
		p.page = place;
		index = catalogManager.GetIndexName(tableName);//��������tableName�е�indexAttrname
		attrGet= catalogManager.GetAttrName(tableName);//��������tableName�е�Attribute
		for (int i = 0; i < index.attrNum; i++)//�����е�indexά��
			for (int j = 0; j < attrGet.attrNum; i++)
				if (index.attrName[i] == attrGet.attrName[j])
					indexManager.Insert(catalogManager.FindIndexPlace(tableName,index.attrName[i]), attr[j], place);//�ҵ�index��λ�ò�ִ�в������
		re.isSucc = 1;
		re.errorReason = "Insert success!";
		return re;
	}

	CreatTable::CreatTable(string tableName, int attrNum, string *attrName, int* attrType){
		this->tableName = tableName;
		this->attrNum = attrNum;
		for (int i = 0; i < attrNum; i++)
		{
			this->attrName[i] = attrName[i];
			this->attrType[i] = attrType[i];
		}
	}
	Error CreatTable::Execute(){
		Error re;
		AttrSaver index;
		catalogManager.creatTable(tableName, attrNum, attrName, attrType);//�½�һ��table������
		index = catalogManager.GetIndexName(tableName);//��������tableName�е�index
		for (int i = 0; i < index.attrNum; i++)
		{
			indexManager.NewIndex(catalogManager.FindIndexPlace(tableName, index.attrName[i]), catalogManager.ReturnType(tableName, attrName));//��page�н����µ�index
		}
		re.isSucc = 1;
		re.errorReason = "Creat table success!";
		return re;
	}
	DropTable::DropTable(string tableName){
		this->tableName = tableName;
	}
	Error DropTable::Execute(){
		Error re;
		int page;
		AttrSaver index;
		page = catalogManager.GetTablePage(tableName);//���ر���table��data��ʼλ�õ�page
		dataManager.DeleteData(page);//ɾ��table������data
		index = catalogManager.GetIndexName(tableName);//��������tableName�е�indexname
		for (int i = 0; i < index.attrNum; i++)
		{
			indexManager.DeleteIndex(catalogManager.FindIndexPlace(tableName, index.attrName[i]));//ɾ��table������index
		}
		catalogManager.DeleteTable(tableName);//ɾ��catalog�д�����table��Ϣ
		re.isSucc = 1;
		re.errorReason = "Drop table success!";
		return re;
	}

	CreatIndex::CreatIndex(string indexName, string tableName, string attrName){
		this->tableName = indexName;
		this->indexName = tableName;
		this->attrName = attrName;
	}
	Error CreatIndex::Execute(){
		Error re;
		int page;
		page = catalogManager.NewIndex(tableName, attrName, indexName);//�½�index��page
		indexManager.NewIndex(page,catalogManager.ReturnType(tableName,attrName));//��page�н����µ�index,ReturnTypeΪ��ȡtable�и���attr����ȡattr��type
		re.isSucc = 1;
		re.errorReason = "Creat Index success";
		return re;
	}
	DropIndex::DropIndex(string indexName){
		this->indexName = indexName;
	}
	Error DropIndex::Execute(){
		Error re;
		int page;
		page=catalogManager.FindIndex(indexName);//����indexnameѰ��index��page
		indexManager.DeleteIndex(page);//ɾ��page�µ�index
		catalogManager.DeleteIndex(indexName);//ɾ��catalog��indexname����Ϣ
		re.isSucc = 1;
		re.errorReason = "Drop Index success!";
		return re;
	}

	Delete::Delete(string tableName, OpType op){
		this->tableName = tableName;
		haveOp = true;
		this->op = op;
	}
	Delete::Delete(string tableName){
		this->tableName = tableName;
		haveOp = false;
	}
	Error Delete::Execute(){
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
//ע�� FindIndexPlace����Ϊattributename ��FindIndexΪindexname