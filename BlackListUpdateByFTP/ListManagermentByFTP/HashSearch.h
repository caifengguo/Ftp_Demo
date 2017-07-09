#pragma once
/*********************************************************************************
Description: ������ʵ�ֹ�ϣ�㷨��һ�����ӣ�KEY���ַ����������ṩ��BKDRhash��������
             ��ϣ��ֵ�������������ʱ�ù�ϣ��ʵ�ֿ��ٲ��ҵȡ�����stl������hash_map
             ��� map�����ơ�����ʹ��18���֤����Ϊ��ֵ����Ϊ���ٲ������֤ʹ�ã�
			 �����������ַ���Ϊ��ֵ��hash����Բο��˳���
Author: LGY
Date:   2015/09/25
**********************************************************************************/

//��ϣ���ȣ������������Ĵ�С���ú���ĳ���
const int HashTableLen=100000;


struct HashNode 
{
	HashNode()
	{
		used=false;
		next=NULL;
	}
	HashNode(char* STR)
	{
		strcpy_s(str, STR);
	}
	bool           used; //��ǰ�ռ��Ƿ�ռ��
	char           str[19];
	HashNode*      next;
};

//�ؼ����ظ��ĳ�ͻ�����ǲ�������ʽ
class CHashSearchOperator
{
public:
	CHashSearchOperator(void);
	~CHashSearchOperator(void);
private:
	//��ϣ������Ч���ݵĳ���
	int        m_nHashTableVaildLen;
	HashNode   m_HashTable[HashTableLen];

	//��ȡ��ϣ������ֵ(BKDRHash�㷨)
	int       GetBKDRHash( const char* str);
	//����hashtable,��������ʽ�������ô�hashtableʱҪ���ô˷���
	void      ClearTable();

public:
	//����hashtable�еĽڵ�Ϊ����״̬
	void      ResetHashTble();
	//����һ�����д洢���ļ��������ļ��е����ݵ�hash����
	int       EstablishHashTable( const TCHAR * Filename);
	//����hashtable�е��Ƿ���ڴ��ַ���
	bool      FindStr(const char*  str);

	//��
	bool      InsertData(HashNode* pNode);
	//ɾ
	bool      DeleteData(const char* strKey);
	//��
	bool      UpdateData(const char* strKey,HashNode* pNode);
	//��
	HashNode* FindData(const char* strKey);

	inline int GetHashTableVaildLen(){return m_nHashTableVaildLen;}
};