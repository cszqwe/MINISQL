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
			attrType = catalogManager.AttrType(tableName, op[0].attrName);//返回table中attr的类型
			re = indexManager.Find(page,op[0]);//在index中进行查找操作
		}
		else{
			page = catalogManager.GetTablePage(tableName);//返回保存table中data起始位置的page
			re = dataManager.Select(page, opNum, op);//在datamanager中进行查找操作
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
		page=catalogManager.GetTablePage(tableName);//返回保存table中data起始位置的page
		PKplace = catalogManager.GetPKnum(tableName);//返回table中PK的位置，若无则为-1
		PKType = catalogManager.GetType(tableName,PKplace);//获取第place个属性的类型
		p=dataManager.InsertData(page,PKplace, PKType,attrNum, attr);//按pk顺序插入到data中，返回插入的page，从而让index连接
		if (p.first) catalogManager.ChangeFirstPage(tableName, p.page);//将tablename的数据首页改为page
		p.page = place;
		index = catalogManager.GetIndexName(tableName);//返回所有tableName中的indexAttrname
		attrGet= catalogManager.GetAttrName(tableName);//返回所有tableName中的Attribute
		for (int i = 0; i < index.attrNum; i++)//将所有的index维护
			for (int j = 0; j < attrGet.attrNum; i++)
				if (index.attrName[i] == attrGet.attrName[j])
					indexManager.Insert(catalogManager.FindIndexPlace(tableName,index.attrName[i]), attr[j], place);//找到index的位置并执行插入操作
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
		catalogManager.creatTable(tableName, attrNum, attrName, attrType);//新建一个table的属性
		index = catalogManager.GetIndexName(tableName);//返回所有tableName中的index
		for (int i = 0; i < index.attrNum; i++)
		{
			indexManager.NewIndex(catalogManager.FindIndexPlace(tableName, index.attrName[i]), catalogManager.ReturnType(tableName, attrName));//在page中建立新的index
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
		page = catalogManager.GetTablePage(tableName);//返回保存table中data起始位置的page
		dataManager.DeleteData(page);//删除table中所有data
		index = catalogManager.GetIndexName(tableName);//返回所有tableName中的indexname
		for (int i = 0; i < index.attrNum; i++)
		{
			indexManager.DeleteIndex(catalogManager.FindIndexPlace(tableName, index.attrName[i]));//删除table中所有index
		}
		catalogManager.DeleteTable(tableName);//删除catalog中存留的table信息
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
		page = catalogManager.NewIndex(tableName, attrName, indexName);//新建index的page
		indexManager.NewIndex(page,catalogManager.ReturnType(tableName,attrName));//在page中建立新的index,ReturnType为提取table中根据attr名提取attr的type
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
		page=catalogManager.FindIndex(indexName);//根据indexname寻找index的page
		indexManager.DeleteIndex(page);//删除page下的index
		catalogManager.DeleteIndex(indexName);//删除catalog中indexname的信息
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
//注意 FindIndexPlace参数为attributename 而FindIndex为indexname