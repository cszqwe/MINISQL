#pragma once
#define DEFBUFSIZE 2048
#define FRAMESIZE  4096
#define MAXPAGES   100000
//��ʵ��COUNT=MAXPAGES=50,000
#define COUNT 500000

//ʵ�ʴ洢page�ĵط�//
struct bFrame {
	char field[FRAMESIZE];
};

//�µ�һҳ//
struct NewPage {
	int page_id;
	int frame_id;
};

//BCB�ṹ��//
struct BCB {
	BCB();
	int page_id;
	int frame_id;
	int latch;
	int count;
	int ftime;
	int stime;
	int dirty;
	BCB * next;
};

//LRU�б�Ԫ��//
struct LRUEle {
	LRUEle();
	int fid;
	double b2dtime;
	LRUEle * less_recent;
	LRUEle * more_recent;
};


class Operate
{
public:
	Operate(void);
	~Operate(void);
};

