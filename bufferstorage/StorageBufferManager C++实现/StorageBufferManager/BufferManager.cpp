#pragma once

#include "BufferManager.h"
#include "DSManager.h"
#include <iostream>
#include <ctime>
#include <stdio.h>
#include <iomanip>
using namespace std;

DSManager ds;
BufferManager bm;
// LRU��ָ��// 
LRUEle * lru;
LRUEle * mru;



BufferManager::BufferManager()
{
	int i = 0;
	for(i = 0; i < DEFBUFSIZE; i++)
	{
		ptof[i] = NULL;// ��ʼ��BCB����
		ftop[i] = -1;
	}
	ds.OpenFile("data.dbf");
}


/**************************************************************************
* ����Ӧpage_id��page���뵽buffer�С����buffer����������Ҫѡ�񻻳���frame
***************************************************************************/
int BufferManager::FixPage(int page_id)
{
	//bFrame ReadPage(int page_id);
	//int Hash(int page_id);
	int fid = -1;
	int frame_id=Hash(page_id);
	//int offs=0;
	BCB * bcb =ptof[frame_id];

	/*if(((*bcb[frame_id]).dirty=0))
		ds->ReadPage(frame_id,bcb);
	else if(((*bcb[frame_id]).dirty=0))//ƫ������//
	{
		frame_id++;
		//offs++;
		ds->ReadPage(frame_id,bcb);
	}
	else
		{
			int fid=bm->SelectVictim(bcb);
			bm->RemoveLRUEle(fid,bcb);
			frame_id=fid				;
			ds->ReadPage(frame_id,bcb);

	    }
	fr[frame_id]=frame_id;
	*bcb[frame_id]=offs;*/

	
	while(bcb != NULL)
	{
		if(bcb->page_id == page_id)
		{
			break;
		}
		bcb = bcb->next;
	}
  
	if(bcb != NULL)
	{
		if(bcb->stime != -1)
		{
			bcb->ftime=bcb->stime;
		}
		calcLRUList(bcb, bcb->frame_id);
		return bcb->frame_id;
	}
 
	else
	{
		bcb = ptof[frame_id];
		fid = SelectVictim();
		buf[fid] = ds.ReadPage(page_id);
		//cout<<(char)(buf[fid].field)<<endl;
		ftop[fid] = page_id;
		if(bcb != NULL)
		{
			while(bcb->next != NULL)
			{
				bcb = bcb->next;
			}
			bcb->next = new BCB();
			bcb = bcb->next;
		}
		else
		{
			bcb = new BCB();
			ptof[frame_id] = bcb; 
		}
		//��������bcb//
		bcb->next = NULL;
		bcb->page_id = page_id;
		bcb->frame_id = fid;
		bcb->latch = 0;
		bcb->count = 0;
		bcb->ftime = static_cast<unsigned>(time(0));
		//cout<<bcb->ftime<<endl;
		bcb->stime = -1;
		//ͬ��ά��LRUlist//
		calcLRUList(bcb, fid);
	}
	return fid;
}


//�ڲ�������ʱ����һ����page���������buffer��//
//NewPage BufferManager::FixNewPage(bFrame buf[])
//{
//}

/*******************************************************
*This function decrements the fix count on the frame.  
*If the count reduces to zero, then the latch on the page 
*is removed and the frame can be removed if selected.  
*The page_id is translated to a frame_id and it may be 
*unlatched so that it can be chosen as a victim page 
*if the count on the page has been reduced to zero.
********************************************************/
int BufferManager::UnfixPage(int page_id)
{
  return 0;
}


/*********************************************************
* buffer����ѡ�񻻳���Ԫ�ص�frame_id
**********************************************************/
int BufferManager::SelectVictim()
{
	int vFrame = 0,pid = 0,fid=0;
	BCB * bcb = NULL;
	bool found = false;
	LRUEle * temp = lru;
	if(temp == NULL)
	{
		return 0;
	}
	for(int i = 0; i < DEFBUFSIZE; i++)
	{
		if(ftop[i] == -1)
		{
			return i;
		}
	}
	vFrame = temp->fid;
	pid = ftop[vFrame];
	fid = Hash(pid);
	bcb = ptof[fid];
	while(found != true)
	{
		if(bcb == NULL)
		{
			break;
		}
    while(bcb->frame_id != vFrame)
    {
      bcb = bcb->next;
    }
    if(bcb->count == 0 && bcb->latch == 0)
    {
		found = true;
	}
    else
    {
		temp = temp->more_recent;
		if(temp == NULL)
		{
			break;
		}
		vFrame = temp->fid;
		pid = ftop[vFrame];
		fid = Hash(pid);
		bcb = ptof[fid];
	}
	}
	pid = ftop[vFrame];
	fid = Hash(pid);
	bcb = ptof[fid];
	if(bcb != NULL)
	{
		if(bcb->next != NULL)
		{
			while(bcb != NULL && bcb->page_id != pid)
			{
				bcb = bcb->next;
			}
			if(bcb != NULL && bcb->dirty == 1)
			{
				ds.WritePage(pid,buf[vFrame]);
			}
			if(bcb != NULL)
			{
				RemoveLRUEle(vFrame);
				RemoveBCB(bcb,pid);
			}
		}
	}
	return vFrame;
}

//���Ҹ�����page�Ƿ��Ѿ���ĳ��frame����//
bool BufferManager::FindFrame(int page_id)
{
	int f_id = NULL;
	//cout<<page_id<<endl;
	for (int i = 0;i<DEFBUFSIZE;i++)
	{
		if (ptof[i]->page_id == page_id)
		{
			f_id = ptof[i]->frame_id;
		}

	}
	//cout<<f_id<<endl;
	if (f_id != NULL)
		return true;
	else
		return false;
}

//HASH�任//
int BufferManager::Hash(int page_id)
{
	//H(k)  =  (page_id)%buffer_size,hashת������//
	int fid;
	int bufsize = DEFBUFSIZE;
	fid = page_id %bufsize;//������ƫ������//

	return fid;
}

//ɾ��һ��BCB//
void BufferManager::RemoveBCB(BCB *ptr, int page_id)
{
	BCB * bcb = ptof[Hash(page_id)];
	BCB * tmp = NULL;
	if(bcb == ptr)
	{
		if(bcb->next != NULL)
		{
			bcb = bcb->next;
			delete ptr;
			ptof[Hash(page_id)] = bcb;
			ptr = bcb = NULL;
		}
		else
		{
			delete ptr;
			ptof[Hash(page_id)] = NULL;
			ptr = bcb = NULL;
		}
	}
	else
	{
		while(bcb->next != ptr)
		{
			bcb = bcb->next;
		}
		bcb->next = ptr->next;
		delete ptr;
		ptr = NULL;
	}
}

//ɾ��LRUԪ��//
void BufferManager::RemoveLRUEle(int frid)
{
	LRUEle * ptr = mru;
	LRUEle * temp = NULL;
	if(ptr == NULL)
	{
		return;
	}
	while(ptr != NULL && ptr->fid != frid)
	{
		ptr = ptr->less_recent;
	}
	if(ptr == NULL)
	{
		return;
	}
	ptr->less_recent->less_recent->more_recent = ptr;
	temp = ptr->less_recent->less_recent;
	delete ptr->less_recent;

	ptr->less_recent = temp;
}

//LRUListά��//
void BufferManager::calcLRUList(BCB * ptr, int frid)
{
	if(mru == NULL)//Ϊ���Ǵ���һ���µ�//
	{
		mru = new LRUEle();
		mru->fid = frid;
		mru->less_recent = NULL;
		mru->more_recent = NULL;
		if(ptr->stime != -1)
		{
			mru->b2dtime = (ptr->stime-ptr->ftime);
		}
		else
		{
			mru->b2dtime = -1;
		}
		lru = mru;
	}
	else
	{
		double t = 0.0;
		if(ptr->stime != -1)
		{
			t = ptr->stime - ptr->ftime;
		}
		else
		{
			t = -1;
		}
		LRUEle * pt = mru;
		if(t == -1)
		{
			while(pt->less_recent != NULL && pt->less_recent->b2dtime != -1)
			{
				pt = pt->less_recent;
			}
			if(pt->less_recent == NULL)
			{
				pt->less_recent = new LRUEle();
				pt->less_recent->more_recent = pt;
				pt = pt->less_recent;
				pt->fid = frid;
				pt->less_recent = NULL;
				pt->b2dtime = t;
				lru = pt;
			}
			else
			{
				LRUEle * temp = new LRUEle();
				temp->fid = frid;
				temp->b2dtime = t;
				temp->less_recent = pt->less_recent;
				temp->more_recent = pt;
				pt->less_recent->more_recent = temp;
				pt->less_recent = temp;
			}
		}
		else
		{
			while(pt->less_recent != NULL && t > pt->less_recent->b2dtime)
			{
				pt = pt->less_recent;
			}
			if(pt->less_recent == NULL)
			{
				pt->less_recent = new LRUEle();
				pt->less_recent->more_recent = pt;
				pt = pt->less_recent;
				pt->b2dtime = t;
				pt->fid = frid;
				pt->less_recent = NULL;
				lru = pt;
			}
			
			else// ��Ҫ���м�����µ�Ԫ��//
			{
				LRUEle * temp = new LRUEle();
				temp->b2dtime = t;
				temp->fid = frid;
				temp->less_recent = pt->less_recent;
				temp->more_recent = pt;
				pt->less_recent->more_recent = temp;
				pt->less_recent = temp;
			}
		}
	}
}


//�ҳ����е�frame//
int BufferManager::NumFreeFrames()
{
	int i = 0,j = 0,num = 0,Free = 0;
	BCB * bcb = NULL;
	for(i = 0; i < DEFBUFSIZE; i++)
	{
		j = ftop[i];
		if(j == -1)
		{
			Free++;
		}
		else
		{
			num = Hash(j);
			int frame_id = num;
			bcb = ptof[num];
			while(bcb != NULL && bcb->frame_id != j)
			{
				bcb = bcb->next;
			}
			if(bcb != NULL && bcb->frame_id == j)
			{
				if(bcb->count == 0 && bcb->latch == 0)
				{
					Free++;
				}
			}
		}
	}
	return Free;
}

//�����ض�bcb�е�dirtyֵΪ1//
void BufferManager::SetDirty(int frame_id)
{
	int pid = ftop[frame_id];
	int fid = Hash(pid);
	BCB * bcb = ptof[fid];
	while(bcb != NULL && bcb->page_id != pid)
	{
		bcb = bcb->next;
	}
	if(bcb != NULL)
	{
		bcb->dirty = 1;
	}
}


//�����ض�bcb�е�dirtyֵΪ0//
void BufferManager::UnsetDirty(int frame_id)
{
	int pid = ftop[frame_id];
	int fid = Hash(pid);
	BCB * bcb = ptof[fid];
	while(bcb != NULL && bcb->page_id != pid)
	{
		bcb = bcb->next;
	}
	if(bcb != NULL)
	{
		bcb->dirty = 0;
	}
}

//��������bcb[]�е�dirtyֵΪ1//
void BufferManager::WriteDirtys()
{
	int i = 0;
	BCB * bcb = NULL;
	for(i = 0; i < DEFBUFSIZE; i++)
	{
		bcb = ptof[i];
		while(bcb != NULL)
		{
			if(bcb->dirty == 1)
			{
				ds.WritePage(bcb->page_id, buf[bcb->frame_id]);
			}
			bcb = bcb->next;
		}
	}
}

void BufferManager::PrintFrame(int index)
{
}
