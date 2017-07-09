#include "stdafx.h"
#include "HashSearch.h"

CHashSearchOperator::CHashSearchOperator(void)
: m_nHashTableVaildLen(0)
{

}

CHashSearchOperator::~CHashSearchOperator(void)
{
	ClearTable();
}


int CHashSearchOperator::GetBKDRHash(const char *str)
{
	//BKDRhash����
	unsigned int seed = 131;       //��ȡֵ 1313   13131   131313  ��
	unsigned int hash = 0;
	while(*str != '\n' &&  *str != 0)
		hash = hash * seed + (*str++);

	return hash % HashTableLen;
}


void CHashSearchOperator::ResetHashTble()
{
	for(int i= 0; i< HashTableLen; i++)
	{
		HashNode *p = &m_HashTable[i];
		m_HashTable[i].used= false;
		while (p->next != NULL)
		{
			p= p->next;
			p->used= false;
		}
	}
	return;
}


void CHashSearchOperator::ClearTable()
{
	for(int i= 0; i< HashTableLen; i++)
	{
		HashNode *p = &m_HashTable[i];
		p= p->next;
		while (p != NULL)
		{
			HashNode *q = p;          //��ֵ��q
			p = p->next;
			delete  q;
			q = NULL;
		}
	}
	return;
}

int  CHashSearchOperator::EstablishHashTable(const TCHAR * chFilename)
{
	HashNode  *phashNode;
	int key;
	string  Linestr;
	ifstream ifs(chFilename,ios_base::binary|ios_base::in);
	//ifs.seekg(3,ios::beg);                // utf8�ļ��ճ�ǰ3���ֽ�
	while( ifs>>Linestr)
	{
		if (Linestr.length() != 18)
		{
			continue;
		}
		key=GetBKDRHash(Linestr.c_str());
		if(m_HashTable[key].used==false)
		{
			strcpy_s(m_HashTable[key].str, Linestr.c_str());
			m_HashTable[key].used=true;
			m_nHashTableVaildLen++;  
		}
		else//��ͻ����
		{
			phashNode=&m_HashTable[key];
			while(phashNode->next!=NULL)
			{
				if(phashNode->next->used == true)
					phashNode = phashNode->next;
				else
				{
					phashNode->next->used=true;
					strcpy_s(phashNode->next->str, Linestr.c_str());
					m_nHashTableVaildLen++;
					break;
				}
			}
			if(phashNode->next == NULL )
			{
				phashNode->next = new HashNode();
				phashNode->next->used=true;
				strcpy_s(phashNode->next->str, Linestr.c_str());
				m_nHashTableVaildLen++;
			}
		}
	}
	return m_nHashTableVaildLen;        // ���ض�������ݸ���
}

bool CHashSearchOperator::FindStr(const char*  str)
{
	HashNode * phashNode;
	int key = 0;

	key=GetBKDRHash(str);
	if(m_HashTable[key].used==false)
	{
		return false;
	}
	else
	{
		phashNode = &m_HashTable[key];
		while(phashNode != NULL)
		{
			if(!strcmp(phashNode->str,str)) 
			{
				return true;
			}
			else
			{
				phashNode =phashNode->next;
			}
			if(phashNode == NULL || phashNode->used == false) 
				return false;
		}
	}
	return false;
}

//��
bool CHashSearchOperator::InsertData(HashNode* pNode)
{
	HashNode * phashNode;
	int key = GetBKDRHash(pNode->str);

	phashNode=&m_HashTable[key];
	if(phashNode->used == false)
	{
		phashNode->used=true;
		strcpy_s(phashNode->str, pNode->str);
		m_nHashTableVaildLen ++;
		return true;
	}

	while(phashNode->next!=NULL)
	{
		if(phashNode->next->used == true)
			phashNode = phashNode->next;
		else
		{
			phashNode->next->used=true;
			strcpy_s(phashNode->next->str, pNode->str);
			m_nHashTableVaildLen++;
			return true;
		}
	}
	if(phashNode->next == NULL )
	{
		phashNode->next=new HashNode();
		phashNode->next->used=true;
		strcpy_s(phashNode->next->str, pNode->str);
		m_nHashTableVaildLen++;
		return true;
	}
	return false;
}

//ɾ
bool CHashSearchOperator::DeleteData(const char* strKey)
{
	HashNode * phashNode;
	int key = GetBKDRHash(strKey);

	phashNode=&m_HashTable[key];
	
	if(phashNode->used == true && strcmp(phashNode->str,strKey)==0)
	{
		phashNode->used=false;
		m_nHashTableVaildLen --;
		return true;
	}

	while(phashNode->next!=NULL)
	{
		if(phashNode->next->used == true && strcmp(phashNode->next->str,strKey)==0)
		{
			phashNode->next->used = false;
			m_nHashTableVaildLen --;
			return true;
		}
		else
		{
			phashNode = phashNode->next;
		}
	}

	if(phashNode->next == NULL )
	{
		return false;
	}
	return false;
}

//��
bool CHashSearchOperator::UpdateData(const char* strKey,HashNode* pNode)
{
	//������û����ݵ��������ݣ���û�о���ʵ�ָ�
	HashNode * phashNode;
	int key = GetBKDRHash(strKey);

	phashNode=&m_HashTable[key];

	if(phashNode->used == true && strcmp(phashNode->str,strKey)==0)
	{
		//�޸���������
		return true;
	}

	while(phashNode->next!=NULL)
	{
		if(phashNode->next->used == true && strcmp(phashNode->next->str,strKey)==0)
		{
			//�޸���������
			return true;
		}
		else
		{
			phashNode = phashNode->next;
		}
	}

	if(phashNode->next == NULL )
	{
		return false;
	}
	return false;
}

//��
HashNode* CHashSearchOperator::FindData(const char* strKey)
{
	HashNode * phashNode;
	int key = GetBKDRHash(strKey);

	phashNode=&m_HashTable[key];

	if(phashNode->used == true && strcmp(phashNode->str,strKey)==0)
	{
		return phashNode;
	}

	while(phashNode->next!=NULL)
	{
		if(phashNode->next->used == true && strcmp(phashNode->next->str,strKey)==0)
		{
			return phashNode->next;
		}
		else
		{
			phashNode = phashNode->next;
		}
	}

	if(phashNode->next == NULL )
	{
		return NULL;
	}
	return NULL;
}