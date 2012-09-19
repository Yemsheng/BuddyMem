
#include"BuddyMem.h"

bool BuddyMem::BuddyMemCreate()
{
	if(m_maxMemorySize<m_blockSize)
	{
		m_createFlag = false;
		return m_createFlag;
	}

	m_startPos = new char[m_maxMemorySize];
	if(m_startPos==NULL)
	{
		m_createFlag = false;
		return m_createFlag;
	}

	CaculateTableLinkListSize();

	m_createFlag = true;
	return m_createFlag;
}

void BuddyMem::BuddyMemDestroy()
{
	if(!m_createFlag)
	{
		return;
	}

	delete []m_startPos;
	m_createFlag = false;
}

char* BuddyMem::BuddyMemMalloc(const unsigned int size)
{
	unsigned int needBlockAmount = size/m_blockSize;
	if(size%m_blockSize!=0)
	{
		needBlockAmount++;
	}
	//´ýÐø


	return NULL;
}

void BuddyMem::CaculateTableLinkListSize()
{
	unsigned int maxBlock = m_maxMemorySize/m_blockSize;
	int i = 0;
	for(i=i;;i++)
	{
		if((maxBlock>>i)==1)
		{
			m_tableLinkListSize = i;
			break;
		}
	}
}