#ifndef type_memory_h
#define type_memory_h

#include<iostream>

using namespace std;

typedef unsigned char BYTE;

//16(MB) = 16777216(B)
#define MEMORY_BLOCK_DEFAULTSIZE 16777216

//When alloc+dispose > this RUN OPTIMIZE
#define MEMORY_POOL_OPTIMIZECOUNT 16

class MemoryBlock
{
public:
	int _size;
	BYTE* _data;
	MemoryBlock* _mb_prev_ptr;
	MemoryBlock* _mb_next_ptr;

	MemoryBlock(int size = MEMORY_BLOCK_DEFAULTSIZE);
	~MemoryBlock();
};

class MemoryNullBlock
{
public:
	int _size;
	BYTE* _data;
	MemoryNullBlock* _mnb_prev_ptr;
	MemoryNullBlock* _mnb_next_ptr;

	MemoryNullBlock(BYTE* ptrHead);
	MemoryNullBlock(int size,BYTE* ptrHead);
	~MemoryNullBlock();
};


class STMemoryPool
{
private:
	int _mb_count;
	int _oper_count;
	
	MemoryBlock* _mb_head_ptr;
	MemoryNullBlock* _mnb_head_ptr;

	MemoryNullBlock* ExpandMemory(int size = MEMORY_BLOCK_DEFAULTSIZE);
	bool ReleaseMemory(MemoryBlock* ptrMemoryBlock);
public:
	STMemoryPool();
	~STMemoryPool();
	void* Allocate(int count,int size);
	bool Dispose(void* object,int count,int size);
	bool Clear();
	bool Optimize();
};

#endif