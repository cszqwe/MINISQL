//index数据设计为 叶子标记+AttrType+父节点page+个数+每个的page和值
#include"API.h"
#include"DataManager.h"
#include<sstream>
#include<string>
#include<iostream>
#include"IndexManager.h"
#include "BufferManager.h"
using namespace std;
#define MAX 200
	int IndexManager::IndexFind(int page, string value){
		string text,temp;
		stringstream stream;
	    
		text = bufferManager.GetText(page);
		int firstLeaf, isLeaf, attrType, faPage, num, nextPage;
		stream << text;
		stream >> isLeaf >> attrType >> faPage >> num ;
		if (isLeaf) return page;
		for (int i = 0; i < num; i++){
			stream >> nextPage >> temp;
			if (Compare(value, temp, 1, attrType)) return IndexFind(nextPage, value);
		}
		stream >> nextPage;
		return IndexFind(nextPage, value);
	}
	void  IndexManager::ChangeFather(int page, int faPage){
		string text;
		stringstream stream;
		stringstream stream1;
		string temp;
		int firstLeaf, isLeaf, attrType, fatherPage, num, nextPage;
		text = bufferManager.GetText(page);
		stream << text;
		stream >> isLeaf >> attrType >> fatherPage;
		stream1 << isLeaf << ' ' << attrType << ' ' << faPage;
		while (stream >> temp) stream1 << ' ' << temp;
		getline(stream1, temp);
		bufferManager.SendText(page, temp);
	}
	void  IndexManager::DeleteMaintain(int page){


	}
	void  IndexManager::InsertMaintain(int page, string attr, int childPage){

		stringstream stream;
		string text;
		string value, temp;
		int nextPage,firstPage;
		text = bufferManager.GetText(page);
		int firstLeaf, isLeaf, attrType, faPage, num;
		stream << text;
		stream >> isLeaf >> attrType >> faPage >> num>>firstPage;

		if (num < MAX)
		{
			stringstream stream1;
			bool flag=true;
			stream1 << isLeaf << ' ' << attrType << ' ' << faPage << ' ' << num + 1<<' '<<firstPage;
			for (int i = 0; i < num; i++)
			{
				stream >> value >> nextPage;
				if ((flag)&&(Compare(attr, value, 1, attrType))){
					stream1 <<' ' << attr << ' '<< childPage;
					flag = false;
				}
				stream1 << ' ' << value<< ' ' << nextPage  ;
			}
			if (flag) stream1 << ' ' << attr << ' ' << childPage;
			getline(stream1, temp);
			bufferManager.SendText(page, temp);
		}
		else{
			stringstream stream1;
			stringstream stream2;
			int newPage = bufferManager.NewPage();
			string values[MAX];
			int nextPages[MAX];
			int num1, num2, insertPlace = -1;
			stream1 << isLeaf << ' ' << attrType << ' ' << faPage << ' ';
			stream2 << isLeaf << ' ' << attrType << ' ' << faPage << ' ';
			for (int i = 0; i < num; i++)
			{
				stream >> values[i] >> nextPages[i];
				if ((insertPlace == -1) && (Compare(attr, values[i], 1, attrType))) insertPlace = i;
			}
			if (insertPlace <= MAX / 2){
				num1 = MAX / 2 + 1;
				num2 = num - MAX / 2;
			}
			else {
				num1 = MAX / 2;
				num2 = num - MAX / 2 + 1;
			}
			stream1 << num1;
			stream2 << num2;
			bool flag = true;
			string attribute;
			for (int i = 0; i < num; i++)
			{
				if (i == insertPlace){
					if (insertPlace < MAX / 2){
						stream1 << ' ' << attr << ' ' << childPage;
					}
					else {
						stream2 << ' ' << attr << ' ' << childPage;
						ChangeFather(childPage, newPage);
						if (flag){
							attribute = values[i];
							flag = false;
						}
					}
				}
				if (i < MAX / 2) {
					stream1 << ' ' << values[i] << ' ' << nextPages[i];
				}
				else {
					stream2 << ' ' << values[i] << ' ' << nextPages[i];
					ChangeFather(nextPages[i], newPage);
					if (flag){
						attribute = values[i];
						flag = false;
					}
				}
			}
			if (-1 == faPage){
				int newPage1 = bufferManager.NewPage();
				
				getline(stream1, temp);
				bufferManager.SendText(newPage1, temp);
				stream1 >> isLeaf >> attrType >> faPage >> num >> firstPage;
				ChangeFather(firstPage, newPage1);
				for (int i = 0; i < num; i++){
					stream1 >> value >> firstPage;
					ChangeFather(firstPage, newPage1);
				}
				getline(stream2, temp);
				bufferManager.SendText(newPage, temp);
				ChangeFather(newPage, page);
				ChangeFather(newPage1, page);
				return;
			}
			getline(stream1, temp);
			bufferManager.SendText(page, temp);
			getline(stream2, temp);

			bufferManager.SendText(newPage, temp);

			InsertMaintain(faPage, attribute, newPage);
		}
	}
	Error  IndexManager::Find(int page, OpType op){
		Error re;
		int aimPage = IndexFind(page, op.value);
		if ((1 == op.ope) || (3 == op.ope)){
			stringstream stream1;
			string text;
			string value;
			text = bufferManager.GetText(page);
			int firstLeaf, isLeaf, attrType, faPage, num, nextPage;
			stream1 << text;
			stream1 >> isLeaf >> attrType >> faPage >> num >> nextPage;
			while (!(isLeaf)){
				stream1.clear();
				stream1.str("");
				text = bufferManger.GetText(nextPage);
				firstLeaf = nextPage;
				stream1 << text;
				stream1 >> isLeaf >> attrType >> faPage >> num >> nextPage;
			}
			int nowLeaf = firstLeaf;
			string attrs[32];
			int front, next, attrNumber;
			while (nowLeaf != aimPage){
				for (int i = 0; i < num; i++){
					text = bufferManager.GetText(nextPage);
					stringstream stream;
					stream << text;
					stream >> front >> next;
					attrNumber = 0;
					while (stream >> attrs[attrNumber])attrNumber++;
					for (int i = 0; i < attrNumber; i++)
						re.errorReason = re.errorReason + attrs[i] + " ";
					re.errorReason = re.errorReason + "\n";
					stream1 >> value >> nextPage;
				}
				nowLeaf = nextPage;
				stream1.clear();
				stream1.str("");
				text = bufferManager.GetText(nowLeaf);
				stream1 << text;
				stream1 >> isLeaf >> attrType >> faPage >> num >> nextPage;
			}
			int nextValue, nowValue = nextPage;
			for (int i = 0; i < num; i++){
				stream1 >> value >> nextValue;
				if (Compare(value, op.value, op.ope, attrType)){
						text = bufferManager.GetText(nowValue);
						stringstream stream;
						stream << text;
						stream >> front >> next;
						attrNumber = 0;
						while (stream >> attrs[attrNumber])attrNumber++;
						for (int i = 0; i < attrNumber; i++)
							re.errorReason = re.errorReason + attrs[i] + " ";
						re.errorReason = re.errorReason + "\n";
				}
				nowValue = nextValue;
			}
		}		
		if ((2== op.ope) || (4 == op.ope)){//>= ,>
			stringstream stream1;
			string text;
			string value;
			string attrs[32];
			int front, next, attrNumber,nowLeaf;
			text = bufferManager.GetText(aimPage);
			int firstLeaf, isLeaf, attrType, faPage, num, nextPage;
			stream1 << text;
			stream1 >> isLeaf >> attrType >> faPage >> num >> nextPage;
			int nextValue, nowValue = nextPage;
			for (int i = 0; i < num; i++){
				stream1 >> value >> nextValue;
				if (Compare(value, op.value, op.ope, attrType)){
					text = bufferManager.GetText(nowValue);
					stringstream stream;
					stream << text;
					stream >> front >> next;
					attrNumber = 0;
					while (stream >> attrs[attrNumber])attrNumber++;
					for (int i = 0; i < attrNumber; i++)
						re.errorReason = re.errorReason + attrs[i] + " ";
					re.errorReason = re.errorReason + "\n";
				}
				nowValue = nextValue;
			}

			nowLeaf = nowValue;//输出后面的
			text = bufferManager.GetText(nowLeaf);
			stream1 << text;
			stream1 >> isLeaf >> attrType >> faPage >> num >> nextPage;
			while (nowLeaf != -1){
				for (int i = 0; i < num; i++){
					text = bufferManager.GetText(nextPage);
					stringstream stream;
					stream << text;
					stream >> front >> next;
					attrNumber = 0;
					while (stream >> attrs[attrNumber])attrNumber++;
					for (int i = 0; i < attrNumber; i++)
						re.errorReason = re.errorReason + attrs[i] + " ";
					re.errorReason = re.errorReason + "\n";
					stream1 >> value >> nextPage;
				}
				nowLeaf = nextPage;
				stream1.clear();
				stream1.str("");
				text = bufferManager.GetText(nowLeaf);
				stream1 << text;
				stream1 >> isLeaf >> attrType >> faPage >> num >> nextPage;
			}
		}
	}
	void  IndexManager::NewIndex(int page, int indexType){
		stringstream stream;
		string text;
		stream << 1<<' '<<indexType<<' '<<-1<<' '<<0<<' '<<-1;
		getline(stream, text);
		bufferManager.SendText(page, text);
	}
	void  IndexManager::DeleteIndex(int page){
		stringstream stream;
		string text,content;
		int isLeaf, attrType,faPage,num,nextPage;
		text = bufferManager.GetText(page);
		stream << text;
		stream >> isLeaf >> attrType >> faPage >> num>>nextPage;
		if (!(isLeaf))
		{
			DeleteIndex(nextPage);
			for (int i = 0; i < num; i++){
				stream >> content >> nextPage;
				DeleteIndex(nextPage);
			}
		}
		bufferManager.SendText(page, "");
		bufferManager.DeletePage(page);
	}
	void  IndexManager::Maintain(int page){
		stringstream stream1;
		string text;
		string value;
		text=bufferManager.GetText(page);
		int firstLeaf,isLeaf, attrType, faPage, num, nextPage;
		stream1 << text;
		stream1 >> isLeaf >> attrType >> faPage >> num >> nextPage;
		while (!(isLeaf)){
			stream1.clear();
			stream1.str("");
			text = bufferManger.GetText(nextPage);
			firstLeaf = nextPage;
			stream1 << text;
			stream1 >> isLeaf >> attrType >> faPage >> num >> nextPage;
		}
		int nowLeaf = firstLeaf;
		while (-1!=nowLeaf)//删掉不存在的data
		{
			int Pages[MAX];
			string values[MAX];
			int newNum = num;
			stringstream stream2;
			string temp;
			stream2 << isLeaf << ' ' << attrType << ' ' << faPage << ' ';
			for (int i = 0; i < num; i++){
				Pages[i] = nextPage;
				if (!bufferManager.GetText(nextPage)) newNum--;
				stream1 >> value[i] >> nextPage;
			}
			stream2 << newNum;
			for (int i = 0; i < num; i++){
				if (bufferManager.GetText(Pages[i]))
					stream2 << ' ' << Pages[i] << ' ' << values[i];
			}
			stream2 << ' ' << nextPage;
			getline(stream2, temp);
			bufferManager.SendText(nowLeaf, temp);
			nowLeaf = nextPage;
			if (-1 != nowLeaf)
			{
				stream1.clear();
				stream1.str("");
				text = bufferManager.GetText(nowLeaf);
				stream1 << text;
				stream1 >> isLeaf >> attrType >> faPage >> num >> nextPage;
			}
		}
		nowLeaf = firstLeaf;
		while (-1 != nowLeaf){//维护已经删除后的叶节点
			DeleteMaintain(nowLeaf);
			string temp = bufferManager.GetText(nowLeaf);
			stringstream stream2;
			stream2 << temp;
			stream2 >> isLeaf >> attrType >> faPage >> num >> nextPage;
			for (int i = 0; i < num; i++) stream2 >> value >> nextPage;
			nowLeaf = nextPage;
		}
	}
	void  IndexManager::Insert(int page, string attr, int place){

		int aimPage = IndexFind(page, attr);
		stringstream stream;
		int text, isLeaf, attrType, faPage, num, nextPage;
		int Pages[MAX];
		bool flag = true;
		string temp;
		string values[MAX];
		text = bufferManager.GetText(page);
		stream << text;
		stream>> isLeaf >> attrType >> faPage >> num;
		for (int i = 0; i < num; i++)
			stream >> Pages[i] >> values[i];
		if (num < MAX){
			stringstream stream2;
			stream2 << isLeaf << ' ' << attrType << ' ' << faPage << ' ' << num + 1 ;
			for (int i = 0; i < num - 1; i++){
				stream2 << ' ' << Pages[i] << ' ' << values[i];
				if ((Compare(values[i], attr, 3, attrType)) && (Compare(values[i + 1], attr, 2, attrType)))
				{
					flag = false;
					stream2 << ' ' << place << ' ' << attr;
				}	
			}
			if (flag) stream2 << ' ' << place << ' ' << attr;
			stream >> nextPage;
			stream2 << ' ' << nextPage;
			getline(stream2, temp);
			bufferManager.SendText(aimPage, temp);
		}
		else {
			int newPage = bufferManager.NewPage();
			stringstream stream1, stream2;
			int num1 = 0, num2 = 0, insertPlace=-1;
			stream1 << isLeaf << ' ' << attrType << ' ' << faPage << ' ';
			stream2 << isLeaf << ' ' << attrType << ' ' << faPage << ' ';
			for (int i = 0; i < num-1; i++){
				if ((Compare(values[i], attr, 3, attrType)) && (Compare(values[i + 1], attr, 2, attrType))) insertPlace = i;
			}
			if (-1 == insertPlace) insertPlace = num - 1;
			if (insertPlace <= MAX / 2) {
				num1 = MAX / 2 + 1;
				num2 = num - MAX / 2;
			}
			else {
				num1 = MAX / 2;
				num2 = num - MAX / 2 + 1;
			}
			stream1 << num1;
			stream2 << num2;
			bool attrFlag = true;
			string attribute;
			for (int i = 0; i < num; i++){
				if (i < MAX/2) stream1 << ' ' << Pages[i] << ' ' << values[i];
				else {
					stream2 << ' ' << Pages[i] << ' ' << values[i];
					if (attrFlag){
						attrFlag = false;
						attribute = values[i];
					}
				}
				if (i == insertPlace){
					if (i < MAX / 2) stream1 << ' ' << place << ' ' << attr;
					else stream2 << ' ' << place << ' ' << attr;
				}
			}
			stream1 << ' ' << newPage;
			stream >> nextPage;
			stream2 << ' ' << nextPage;
			getline(stream1, temp);
			bufferManager.SendText(aimPage, temp);
			getline(stream2, temp);
			bufferManager.SendText(newPage, temp);
			InsertMaintain(faPage, attribute, newPage);
		}
	}