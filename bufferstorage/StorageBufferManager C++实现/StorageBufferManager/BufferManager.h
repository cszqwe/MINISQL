#pragma once

#include "BufferManager.h"
#include "DSManager.h"
#include "Operate.h"

#include <iostream>
#include <ctime>
#include <stdio.h>
#include <iomanip>
//struct Record {
//	int page_id;
//	int slot_num;
//}; 


class BufferManager
{
public:
	BufferManager();
	int FixPage(int page_id);
	NewPage FixNewPage(bFrame buf[]);//NewPage 
	int UnfixPage(int page_id);
	int NumFreeFrames();
	int SelectVictim();
	int Hash(int page_id);
	void RemoveBCB(BCB * ptr, int page_id);
	void RemoveLRUEle(int frid);
	void SetDirty(int frame_id);
	void UnsetDirty(int frame_id);
	void WriteDirtys();
	bool FindFrame(int page_id);
	void PrintFrame(int frame_id);

	void calcLRUList(BCB * ptr, int frid);
//private:	/* Hash Table */
	int ftop[DEFBUFSIZE];//frame to page

	BCB * ptof[DEFBUFSIZE];

	// BUFFER
	bFrame buf[DEFBUFSIZE];

};
	/************************************************************************************
	*These interface functions will provide an interface to the file and access manager above it.  The functions required are:
	*FixPage(int page_id, int prot)
	*The prototype for this function is FixPage(Page_id, protection) and it returns a frame_id.  The file and access manager will call this page with the page_id that is in the record_id of the record.  The function looks to see if the page is in the buffer already and returns the corresponding frame_id if it is.  If the page is not resident in the buffer yet, it selects a victim page, if needed, and loads in the requested page.  
	*FixNewPage()
	*The prototype for this function is FixNewPage() and it returns a page_id and a frame_id.  This function is used when a new page is needed on an insert, index split, or object creation.  The page_id is returned in order to assign to the record_id and metadata.  This function will find an empty page that the File and Access Manager can use to store some data.
 	*UnfixPage(int page_id)
	*The prototype for this function is UnfixPage(page_id) and it returns a frame_id.  This function is the compliment to a FixPage or FixNewPage call.  This function decrements the fix count on the frame.  If the count reduces to zero, then the latch on the page is removed and the frame can be removed if selected.  The page_id is translated to a frame_id and it may be unlatched so that it can be chosen as a victim page if the count on the page has been reduced to zero.
	*NumFreeFrames()
	*NumFreeFrames function looks at the buffer and returns the number of buffer pages that are free and able to be used.  This is especially useful for the N-way sort for the query processor.  The prototype for the function looks like NumFreeFrames() and returns an integer from 0 to BUFFERSIZE-1(1023).
	*SelectVictim()
	*SelectVictim function selects a frame to replace. If the dirty bit of the selected frame is set then the page needs to be written on to the disk.
	*Hash(int page_id)
	*Hash function takes the page_id as the parameter and  returns the frame id.
	*RemoveBCB(BCB* ptr, int page_id)
	*RemoveBCB function removes the Buffer Control Block for the page_id from the array.  This is only called if the SelectVictim()  function needs to replace a frame.
	*RemoveLRUEle(int frid)
	*RemoveLRUEle function removes the LRU element from the list.
	*SetDirty(int frame_id)
	*SetDirty function sets the dirty bit for the frame_id.  This dirty bit is used to know whether or not to write out the frame.  A frame must be written if the contents have been modified in any way.  This includes any directory pages and data pages.  If the bit is 1, it will be written.  If this bit is zero, it will not be written.
	*UnsetDirty(int frame_id)
	*UnsetDirty function assigns the dirty_bit for the corresponding frame_id to zero.  The main reason to call this function is when the setDirty() function has been called but the page is actually part of a temporary relation.  In this case, the page will not actually need to be written, because it will not want to be saved.
	*WriteDirtys()
	*WriteDirtys function must be called when the system is shut down.  The purpose of the function is to write out any pages that are still in the buffer that may need to be written.  It will only write pages out to the file if the dirty_bit is one.
	*PrintFrame(int frame_id)
	*PrintFrame function prints out the contents of the frame described by the frame_id.
	**************************************************************************************/