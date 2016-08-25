#include "type_memory.h"

MemoryBlock::MemoryBlock(int size)
{
	this->_size = size;
	this->_data = new BYTE[size];
	this->_mb_next_ptr = NULL;
	this->_mb_prev_ptr = NULL;
	cout<<"New MemoryBlock("<<size<<")"<<endl;
}

MemoryBlock::~MemoryBlock()
{
	this->_size=0;
	delete[] this->_data;
	cout<<"Delete MemoryBlock"<<endl;
}

MemoryNullBlock::MemoryNullBlock(BYTE* ptrHead)
{
	this->_size = MEMORY_BLOCK_DEFAULTSIZE;
	this->_data = ptrHead;
	this->_mnb_next_ptr = NULL;
	this->_mnb_prev_ptr = NULL;
}

MemoryNullBlock::MemoryNullBlock(int size,BYTE* ptrHead)
{
	this->_size = size;
	this->_data = ptrHead;
	this->_mnb_next_ptr = NULL;
	this->_mnb_prev_ptr = NULL;
}

MemoryNullBlock::~MemoryNullBlock()
{
	this->_size = 0;
	this->_data = NULL;
	this->_mnb_next_ptr = NULL;
	this->_mnb_prev_ptr = NULL;
}



STMemoryPool::STMemoryPool()
{
	cout<<"New MemoryPool"<<endl;
	this->_mb_count = 1;
	this->_oper_count = 0;
	this->_mb_head_ptr = new MemoryBlock();
	this->_mnb_head_ptr = new MemoryNullBlock(this->_mb_head_ptr->_data);
}

STMemoryPool::~STMemoryPool()
{
	this->_mb_count = 0;
	this->_oper_count = 0;
	this->Clear();
	this->_mb_head_ptr = NULL;
	this->_mnb_head_ptr = NULL;
}


MemoryNullBlock* STMemoryPool::ExpandMemory(int size)
{
	MemoryBlock* tmpMB1 = new MemoryBlock();
	MemoryBlock* tmpMB2 = this->_mb_head_ptr;
	MemoryNullBlock* tmpMNB1 = new MemoryNullBlock(tmpMB1->_data);
	MemoryNullBlock* tmpMNB2 = this->_mnb_head_ptr;
	
	this->_mb_count++;

	while(tmpMB2->_mb_next_ptr != NULL)	tmpMB2 = tmpMB2->_mb_next_ptr;
	tmpMB2->_mb_next_ptr = tmpMB1;
	tmpMB1->_mb_prev_ptr = tmpMB2;

	while(tmpMNB2->_mnb_next_ptr != NULL)	tmpMNB2 = tmpMNB2->_mnb_next_ptr;
	tmpMNB2->_mnb_next_ptr = tmpMNB1;
	tmpMNB1->_mnb_prev_ptr = tmpMNB2;

	return tmpMNB1;
}

//Consider if dispose MNB
bool STMemoryPool::ReleaseMemory(MemoryBlock* ptrMemoryBlock)
{
	if(ptrMemoryBlock->_mb_prev_ptr==NULL)
	{
		if(ptrMemoryBlock->_mb_next_ptr==NULL)
		{
			cout<<"Cannot release the only memory block."<<endl;
			return false;
		}
		else
		{
			this->_mb_head_ptr = ptrMemoryBlock->_mb_next_ptr;
			ptrMemoryBlock->_mb_next_ptr->_mb_prev_ptr = NULL;
		}
	}
	else
	{
		ptrMemoryBlock->_mb_prev_ptr->_mb_next_ptr = ptrMemoryBlock->_mb_next_ptr;
		if(ptrMemoryBlock->_mb_next_ptr!=NULL)
		{
			ptrMemoryBlock->_mb_next_ptr->_mb_prev_ptr = ptrMemoryBlock->_mb_prev_ptr;
		}
	}
	delete ptrMemoryBlock;
	return true;
}


void* STMemoryPool::Allocate(int count,int size)
{
	void* tmpVar = NULL;
	int totalsize = count*size;

	MemoryNullBlock* tmpMNB = this->_mnb_head_ptr;
	while(tmpMNB!=NULL && tmpMNB->_size<totalsize) tmpMNB = tmpMNB->_mnb_next_ptr;

	if(tmpMNB!=NULL) //MemoryBlock available
	{
		tmpVar = tmpMNB->_data;
		tmpMNB->_data += totalsize;
		tmpMNB->_size -= totalsize;
	}
	else //MemoryBlock unavailable
	{
		if(this->_oper_count>0)
		{
			this->Optimize();
			tmpMNB = this->_mnb_head_ptr;
			while(tmpMNB!=NULL && tmpMNB->_size<totalsize) tmpMNB = tmpMNB->_mnb_next_ptr;
			
			if(tmpMNB!=NULL) //MemoryBlock available
			{
				tmpVar = tmpMNB->_data;
				tmpMNB->_data += totalsize;
				tmpMNB->_size -= totalsize;
			}
		}
		if(tmpMNB==NULL)
		{
			if((tmpMNB = this->ExpandMemory(totalsize))!=NULL)
			{
				tmpVar = tmpMNB->_data;
				tmpMNB->_data += totalsize;
				tmpMNB->_size -= totalsize;
			}
			else
			{
				return NULL;
			}
		}
	}
	this->_oper_count++;
	if(this->_oper_count>MEMORY_POOL_OPTIMIZECOUNT) this->Optimize();
	return tmpVar;
}

bool STMemoryPool::Dispose(void* object,int count,int size)
{
	int totalsize = count*size;
	BYTE* obj = (BYTE*)object;
	bool result = false;

	MemoryNullBlock* tmpMNB = this->_mnb_head_ptr;
	while(tmpMNB!=NULL)
	{
		if(tmpMNB->_data+tmpMNB->_size == obj) //Near Right
		{
			tmpMNB->_size += totalsize;
			result = true;
			break;
		}
		else if(tmpMNB->_data == obj+totalsize) //Near Left
		{
			tmpMNB->_data -= totalsize;
			tmpMNB->_size += totalsize;
			result = true;
			break;
		}
		else if(tmpMNB->_mnb_next_ptr == NULL && obj>tmpMNB->_data+tmpMNB->_size) //Right
		{
			MemoryNullBlock* tmpMNB2 = new MemoryNullBlock(totalsize,obj);
			tmpMNB2->_mnb_prev_ptr = tmpMNB;
			tmpMNB->_mnb_next_ptr = tmpMNB2;
			result = true;
			break;
		}
		else if(tmpMNB->_mnb_prev_ptr == NULL && obj+totalsize<tmpMNB->_data) //Left
		{
			MemoryNullBlock* tmpMNB2 = new MemoryNullBlock(totalsize,obj);
			tmpMNB2->_mnb_next_ptr = tmpMNB;
			tmpMNB->_mnb_prev_ptr = tmpMNB2;
			this->_mnb_head_ptr = tmpMNB2;
			result = true;
			break;
		}
		else if(tmpMNB->_data+tmpMNB->_size<obj && obj+totalsize<tmpMNB->_mnb_next_ptr->_data)
		{
			MemoryNullBlock* tmpMNB2 = new MemoryNullBlock(totalsize,obj);
			tmpMNB2->_mnb_prev_ptr = tmpMNB;
			tmpMNB2->_mnb_next_ptr = tmpMNB->_mnb_next_ptr;
			tmpMNB2->_mnb_next_ptr->_mnb_prev_ptr = tmpMNB2;
			tmpMNB->_mnb_next_ptr = tmpMNB2;
			result = true;
			break;
		}
		tmpMNB=tmpMNB->_mnb_next_ptr;
	}
	if(result)
	{
		this->_oper_count++;
		if(this->_oper_count>MEMORY_POOL_OPTIMIZECOUNT) this->Optimize();
	}
	return false;
}

bool STMemoryPool::Clear()
{
	MemoryBlock* tmpMB = this->_mb_head_ptr;
	while(tmpMB != NULL)
	{
		this->_mb_head_ptr = tmpMB->_mb_next_ptr;
		delete tmpMB;
		tmpMB = this->_mb_head_ptr;
	}
	MemoryNullBlock* tmpMNB = this->_mnb_head_ptr;
	while(tmpMNB != NULL)
	{
		this->_mnb_head_ptr = tmpMNB->_mnb_next_ptr;
		delete tmpMNB;
		tmpMNB = this->_mnb_head_ptr;
	}
	return true;
}

bool STMemoryPool::Optimize()
{
	MemoryNullBlock* tmpMNB = this->_mnb_head_ptr;
	MemoryNullBlock* tmpMNB2 = NULL;
	while(tmpMNB->_mnb_next_ptr!=NULL) tmpMNB = tmpMNB->_mnb_next_ptr;
	while(tmpMNB->_mnb_prev_ptr!=NULL)
	{
		tmpMNB2 = tmpMNB->_mnb_prev_ptr;
		if(tmpMNB->_size==0)
		{
			tmpMNB2->_mnb_next_ptr = tmpMNB->_mnb_next_ptr;
			if(tmpMNB->_mnb_next_ptr!=NULL)
			{
				tmpMNB->_mnb_next_ptr->_mnb_prev_ptr = tmpMNB->_mnb_prev_ptr;
			}
			delete tmpMNB;
		}
		tmpMNB = tmpMNB2;
	}
	tmpMNB = this->_mnb_head_ptr;
	while(tmpMNB->_mnb_next_ptr!=NULL) tmpMNB = tmpMNB->_mnb_next_ptr;
	while(tmpMNB->_mnb_prev_ptr!=NULL)
	{
		tmpMNB2 = tmpMNB->_mnb_prev_ptr;
		if(tmpMNB2->_data+tmpMNB2->_size==tmpMNB->_data)
		{
			tmpMNB2->_size += tmpMNB->_size;
			tmpMNB2->_mnb_next_ptr = tmpMNB->_mnb_next_ptr;
			if(tmpMNB->_mnb_next_ptr!=NULL)
			{
				tmpMNB->_mnb_next_ptr->_mnb_prev_ptr = tmpMNB->_mnb_prev_ptr;
			}
			delete tmpMNB;
		}
		tmpMNB = tmpMNB2;
	}
	this->_oper_count = 0;
	return true;
}
