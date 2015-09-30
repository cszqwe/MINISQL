#pragma once
#include "Operate.h"
#include "BufferManager.h"
#include "DSManager.h"
#include <iostream>
#include <fstream>
#include <ctime>

using namespace std;

Operate::Operate(void)
{
}
Operate::~Operate(void)
{
}

LRUEle::LRUEle()
{
	fid = -1;
	b2dtime = 0.0;
	less_recent = NULL;
	more_recent = NULL;
}

BCB::BCB()
{
	next = NULL;
	latch = -1;
	count = -1;
	page_id = -1;
	frame_id = -1;
	stime =-1;
	ftime=-1;
	dirty=-1;

}

int main()
{
	//第一次运行，创建全为1，data.dbf文件,之后屏蔽*****************//
	FILE *fp=fopen("data.dbf","w+");
	char bu[100000];
	memset(bu,1,sizeof(bu));//清1//
	for (int i = 0;i<4096;i++)
	{
		fwrite(bu,sizeof(char),sizeof(bu),fp);
	}
	fclose(fp);

	/*************************************************************
	* data-5w-50w-zipf.txt第一个数表示读或者写，第二个为page_id
	**************************************************************/
	ifstream infile("data-5w-50w-zipf.txt");
	if(!infile)
	{
		cerr<<"open data-5w-50w-zipf.txt error"<<endl;
		exit(1);
	}
	//(int,int\n);
	int temp1,temp2;
	int ct = -1;
	int ICount = 0;int OCount = 0;
	char c1;

	/*********************************************************
	* 函数声明
	**********************************************************/
	BufferManager *bm=new BufferManager;
	DSManager *ds=new DSManager;
	//bFrame ReadPage(int page_id);
	//int WritePage(int frame_id, bFrame frm);
	//int Hash(int page_id);
	bFrame bf_1;
	bFrame *bf_2=new bFrame[DEFBUFSIZE];
	ds->OpenFile("data.dbf");
	
	/***************按frame_id填满buf*************************/
	for (int i2 = 0;i2<DEFBUFSIZE;i2++)
	{
		bf_2[i2] = ds->ReadPage(i2);
	}

	for (int i3 = 0;i3<DEFBUFSIZE;i3++)
	{
		bm->FixPage(i3);
	}
	//cout<<bm->ptof[358]->frame_id<<endl;
	//cout<<(char)bm->buf[358].field<<endl;

	//计时开始//
	double beginTime = (double)clock()/CLK_TCK;
	/*******************************************************
	* Each trace record has the format as "x, ###",where x is 
	* 0(read) or 1(write) and ### is the referenced page number.
	* 一边读.txt文件，一边处理
	********************************************************/
	while(infile!=NULL)
	{
		infile>>temp1;//记录是读还是写操作//
		//cout<<temp1;
		infile>>c1;
		//cout<<c1;
		infile>>temp2;
		//cout<<temp2;
		//cout<<endl;
		ct++;
		if (temp1 == 0)//read
		{
			int p_id_1 = temp2;
			//int f_id_1=bm->Hash(p_id_1);
			/*	int f_id_1=bm->Hash(p_id_1);
			if (p_id_1==bm->ptof[f_id_1]->page_id)
			{
				bf_1 = bm->buf[f_id_1];
				//cout<<(char)(bm->buf[f_id_1].field)<<endl;
			}
			else
			{
				bf_1 = ds->ReadPage(p_id_1);
				int f_id_2=bm->Hash(p_id_1);
				bm->buf[f_id_2] = bf_1;
				bm->ptof[f_id_2]->page_id=p_id_1;
				//bm->FixNewPage(bf_1);
				//fr[p_id_1]=bm->FixPage(p_id_1,bcb);
				//bcb[p_id_1]->dirty=0;
				ICount++;
			}
			*/
			if (bm->FindFrame(p_id_1))//缓存中有，从缓存读取//
			{
				//读取bm->buf[f_id_1];内容
			} 
			else
			{
				/*将对应page_id的page读入到buffer中。如果buffer已满，则需要选择换出的frame*/
				bm->FixPage(p_id_1);
				ICount++;
				//读取bm->buf[f_id_1];内容
				
			}
		}
		else			//write
		{
			//cout<<bm->ptof[0]->page_id;
			int p_id_2 = temp2;
			if(bm->FindFrame(p_id_2))//缓存有相应页，先写入缓存
			{				
				//bm->buf[i]=bf_1;
			}
			else
			{
				//直接写入文件，修改缓存//
				//ds->WritePage(p_id_2,bf_1);
				/*估计：读取时间=写入时间，省略直接写入文件部分,
				  所以直接用一个FixPage代替一个I/O并且设施缓存*/
				bm->FixPage(p_id_2);
				
				OCount++;
			}
			/*int f_id_2 = bm->Hash(p_id_2);
			if (p_id_2==bm->ptof[f_id_2]->page_id)
			{
				//int f_id_2 = bm->ptof[f_id_2]->frame_id;
				//bm->buf[f_id_2].field = b_2;
			}
			else
			{
				//bf_op=buf[p_id_2];
				//ds->WritePage(p_id_2,bf_op,bcb);
				//fr[p_id_2]=bm->FixPage(p_id_2,bcb);
				//bcb[p_id_2]->dirty=0;
				OCount++;
			}*/

			
			//bm->FixNewPage(bf_2);
		}

	}
	//计时结束//
	double finishTime = (double)clock()/CLK_TCK;

	double runTime = finishTime - beginTime;

	int totle = ICount+OCount;
	double l = (double(ct-totle)/ct)*100;
	cout<<"DEFBUFSIZE大小"<<DEFBUFSIZE<<endl;
	cout<<"操作总页数"<<ct<<endl;
	cout<<"总的I/O："<<totle<<endl;
	cout<<"命中率"<<l<<"%"<<endl;
	cout<<"程序总运行时间为："<<runTime<<endl;

	//关闭程序的时候，dirty的buf写回文件//
	for (int i = 0;i < DEFBUFSIZE;i++)
	{
		while (bm->ptof[i]->dirty=1)
		{
			bFrame bf = bm->buf[bm->ptof[i]->page_id];
			ds->WritePage(bm->ptof[i]->page_id,bf);
		}
	}

	infile.close();
	return 0;
}