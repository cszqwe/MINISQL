#include"API.h"
#include"DataManager.h"
#include<sstream>
#include<string>
#include<iostream>
#include"BufferManager.h"
using namespace std;
bool Compare(string a, string b, int comType, int attrType){
	stringstream stream1;
	stringstream stream2;
	stream1 << a;
	stream2 << b;
	if (attrType == 1) {
		int a1, b1;
		stream1 >> a1;
		stream2 >> b1;
		switch (comType){
		case 1: if (a1 < b1) return true;
				else return false;
				break;
		case 2:if (a1 > b1) return true;
			   else return false;
			   break;
		case 3:if (a1 <= b1) return true;
			   else return false;
			   break;
		case 4:if (a1 >= b1) return true;
			   else return false;
			   break;
		case 5:if (a1 == b1) return true;
			   else return false;
			   break;
		case 6:if (a1 != b1) return true;
			   else return false;
			   break;
		}
	}
	if (attrType == 2){
		double a1, b1;
		stream1 >> a1;
		stream2 >> b1;
		switch (comType){
		case 1: if (a1 < b1) return true;
				else return false;
				break;
		case 2:if (a1 > b1) return true;
			   else return false;
			   break;
		case 3:if (a1 <= b1) return true;
			   else return false;
			   break;
		case 4:if (a1 >= b1) return true;
			   else return false;
			   break;
		case 5:if (a1 == b1) return true;
			   else return false;
			   break;
		case 6:if (a1 != b1) return true;
			   else return false;
			   break;
		}
	}
	if (attrType == 3){
		string a1 = a, b1 = b;
		switch (comType){
		case 1: if (a1 < b1) return true;
				else return false;
				break;
		case 2:if (a1 > b1) return true;
			   else return false;
			   break;
		case 3:if (a1 <= b1) return true;
			   else return false;
			   break;
		case 4:if (a1 >= b1) return true;
			   else return false;
			   break;
		case 5:if (a1 == b1) return true;
			   else return false;
			   break;
		case 6:if (a1 != b1) return true;
			   else return false;
			   break;
		}
	}
	return false;
}
	SavePlace DataManager::InsertData(int page, int PKplace, int PKType, int attrNum, string*attr){
		SavePlace re;
		if ((page==-1)||(PKplace == -1)){//放在文件头
			stringstream stream;
			string text;
			stream << -1 << ' ' << page;
			for (int i = 0; i < attrNum; i++) stream << ' ' << attr[i];
			getline(stream, text);
			re.first = true;
			re.page = bufferManager.newPage();//新建page
			bufferManager.updatePage(re.page, text);
			return re;
		}
		else {
			int p = page;
			int front, next;
			int k;
			string text;
			bool flag = true;
			while (p != -1){
				text = bufferManager.readPage(p);
				stringstream stream;
				stream << text;
				stream >> front >> next;
				int k =PKplace;
				string attr1;
				stream >> attr1;
				while (k > 0){
					stream >> attr1;
					k--;
				}
				if (Compare(attr[PKplace], attr1, 3, PKType)) {
					if (next == -1){
						stringstream stream;
						string text;
						stream << p << ' ' << next;
						for (int i = 0; i < attrNum; i++) stream << ' ' << attr[i];
						getline(stream, text);
						if (flag)re.first = true;
						else re.first = false;
						re.page = bufferManager.newPage();//新建page
						bufferManager.updatePage(re.page, text);
						return re;
					}
					p = next;
				}
				else {
					stringstream stream;
					string text;
					stream << front << ' ' << p;
					for (int i = 0; i < attrNum; i++) stream << ' ' << attr[i];
					getline(stream, text);
					if (flag)re.first = true;
					else re.first = false;
					re.page = bufferManager.newPage();//新建page
					bufferManager.updatePage(re.page, text);
					return re;
				}
				flag = false;
			}

		}
	}
	int DataManager::DeleteData(int page, OpType op){
		string text;
		int p = page;
		int front, next;
		while (p != -1){
			text = bufferManager.readPage(p);
			stringstream stream;
			stream << text;
			stream >> front >> next;
			int k = op.attrPos;
			string attr;
			stream >> attr;
			while (k > 0){
				stream >> attr;
				k--;
			}
			if (Compare(attr, op.value, op.ope,op.attrType)){
				if (front != -1){
					string textf,ne;
					textf = bufferManager.readPage(front);
					stringstream stream1;
					stream1 << next;
					stream1 >> ne;
					int pos1, pos2;
					pos1 = textf.find(' ', 0);
					pos2 = textf.find(' ', pos1+1);
					textf.replace(pos1, pos2 - pos1, ne);
					bufferManager.updatePage(front, textf);
				}
				if (next != -1){
					string textn, fr;
					textn = bufferManager.readPage(next);
					stringstream stream1;
					stream1 << front;
					stream1 >> fr;
					int pos1, pos2;
					pos1 = textn.find(' ', 0);
					textn.replace(0, pos1, fr);
					bufferManager.updatePage(next, textn);
				}
				text = "";
				bufferManager.updatePage(p, text);
			}
			p = next;
		}
	}
	int DataManager::DeleteData(int page){
		string text;
		int p = page;
		int front, next;
		while (p != -1){
			text = bufferManager.readPage(p);
			stringstream stream;
			stream << text;
			stream >> front >> next;
			text = "";
			bufferManager.updatePage(p, text);
			p = next;
		}
	}
	Error DataManager::Select(int page, int opNum, OpType *op){
		Error re;
		int p = page;
		bool flag = true, f ;
		string text;
		int front, next;
		int attrNumber;
		string attrs[32];
		while (p != -1){
			text = bufferManager.readPage(p);
			bufferManager.updatePage(p, text);
			stringstream stream;
			stream << text;
			stream >> front >> next;
			attrNumber = 0;
			while (stream >> attrs[attrNumber])attrNumber++;
			f = true;
			for (int i = 0; i < opNum; i++){
				if (!Compare(attrs[op[i].attrPos], op[i].value, op[i].ope, op[i].attrType))
				{
					f = false;
					break;
				}
			}
			if (f){
				flag = false;
				for (int i = 0; i < attrNumber; i++)
					re.errorReason = re.errorReason + attrs[i] + " ";
				re.errorReason = re.errorReason + "\n";
			}
			p = next;
		}
		if (flag){
			re.isSucc = false;
			re.errorReason = "No exist data.";
			return re;
		}
		re.isSucc = true;
		return re;
	}
