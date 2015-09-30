#pragma once

#include "Operate.h"
#include "BufferManager.h"
#include "DSManager.h"

#include <iostream>
#include <ctime>
#include <stdio.h>
#include <iomanip>
using namespace std;

class DSManager
{
public:
	DSManager();
	int OpenFile(string filename);
	int CloseFile();
	bFrame ReadPage(int page_id);
	int WritePage(int page_id, bFrame frm);
	int Seek(int offset, int pos);
	FILE * GetFile();
	void IncNumPages();
	int GetNumPages();
	void SetUse(int index, int use_bit);
	int GetUse(int index);

private:
	FILE *file;
	int numPages;
	int pages[MAXPAGES];
	//fstream file;	C++²Ù×÷

};

