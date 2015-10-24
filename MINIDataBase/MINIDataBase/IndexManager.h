#ifndef _INDEXMANAGER_H
#define _INDEXMANAGER_H
class IndexManager{
private:
	int IndexFind(int page, string value);
	void ChangeFather(int page, int faPage);
	void DeleteMaintain(int page);
	void InsertMaintain(int page, string attr, int childPage);
public:
	Error Find(int page, OpType op);
	void NewIndex(int page,int indexType);
	void DeleteIndex(int page);
	void Maintain(int page);
	void Insert(int page, string attr, int place);
};
#endif