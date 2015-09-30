#pragma once
#define DEFBUFSIZE 2048
#define FRAMESIZE  4096
#define MAXPAGES   100000
//本实验COUNT=MAXPAGES=50,000
#define COUNT 500000

//实际存储page的地方//
struct bFrame {
	char field[FRAMESIZE];
};

//新的一页//
struct NewPage {
	int page_id;
	int frame_id;
};

//BCB结构体//
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

//LRU列表元素//
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

