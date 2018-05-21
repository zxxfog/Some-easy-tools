/*
*      
*          ������       ������
*       �������� �ة��������������� �ة�����
*       ��                 ��
*       ��       ������       ��
*       ��  ���Щ�       ���Щ�  ��
*       ��                 ��
*       ��       ���ة�       ��
*       ��                 ��
*       ����������         ����������
*           ��         ��
*           ��         ��
*           ��         ��
*           ��         ��������������������������������
*           ��                        ��
*           ��                        ������
*           ��                        ������    
*           ��                        ��
*           ������  ��  �����������������Щ�����  ��������         
*             �� ���� ����       �� ���� ����         
*             �������ة�����       �������ة����� 
*                 ���ޱ��� 
*                 ������BUG! 
*/


#include <stdio.h>
#include <io.h>
#include <string.h>
#include <unistd.h>
#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ���ܣ� ���ļ��Ǽ���ĳ���ļ����ڵ�����.c�ļ������ĺ���������������ע�͵��ĺ�������
* ���������ҳ�Ϊ�㷨�����������Ǽ������ļ���{}���һ����ŵ�ƥ��
*  ������һЩ�ر��ע����ǽṹ������ĳ�ʼ�������Ļ�����ʹ�� */

#define STR_ARRAY_LEN	(128u)
static char folderHead[25][16] = {"B_FUNC/", "B_GEN/", "B_IF/", "D_CFG/", "D_DD/", "F_CI/", "F_HOV/", "F_MA/", "F_PF/", "F_SIEVE/", "F_TRACE/", "F_TSR/", "F_VOBC/", "I_ATS/", "I_CI/", "I_MNM/", "I_SYN/", "I_VOBC/", "I_ZC/", "M_FMAIN/", "M_IMAIN/", "M_MAIN/", "P_IF/", "S_RI/", "S_RII/"};

static int getFuncNums(char *fileName);
static void updateCharArray(char bfChar[], char newCh);
static int isLikeFuncFlag(char bfChar[]);

/* �洢������������֮ǰ���ַ� */
static char beforeChar[STR_ARRAY_LEN] = {0};

const char *folderNew = "../../../04Implement/02Code/";
const char *filePathStr = "./*.c";

#define SRC_FOLDER_INDEX (24)


int main(void)
{
	struct _finddata_t fileInfo;
	long fileHand;
	int funcNums = 0, fileNums = 0;
	char cwdBuffer[256] = {0};

	/* ��ӡ��ǰ�Ĺ���Ŀ¼ */
	memset(cwdBuffer, 0, 256);
	(void)getcwd(cwdBuffer, 256);
	printf("Doc folder: %s\n",cwdBuffer);

	/* ��Դ����Ŀ¼ */
	memset(cwdBuffer, 0, 256);
	sprintf(cwdBuffer, "%s%s", folderNew, folderHead[SRC_FOLDER_INDEX]);
	(void)chdir(cwdBuffer);
	memset(cwdBuffer, 0, 256);
	(void)getcwd(cwdBuffer, 256);
	printf("Src folder: %s\n",cwdBuffer);

	/* ���ұ��ļ����ڵ��ļ� */
	fileHand = _findfirst(filePathStr,&fileInfo);

	printf("\n\n");

	/* �ļ�����ʧ�� */
	if(-1 == fileHand)
	{
		printf("Err: Find first file failed. %s-%d\n",__FILE__,__LINE__);
	}
	else
	{
		do
		{
			/* �´�һ���ļ���Ҫ������ */
			memset(beforeChar, 0, STR_ARRAY_LEN);
			fileNums++;
			funcNums += getFuncNums(fileInfo.name);
			printf("File Name: %s, funcNum: %d\n",fileInfo.name, getFuncNums(fileInfo.name));
		}
		/* �����ļ����ڵ���һ���ļ� */
		while(_findnext(fileHand,&fileInfo) == 0);

		printf("\n\n*******************  Total Info  **********************\n");
		printf("File Nums : %d\n",fileNums);
		printf("Func Nums : %d\n",funcNums);
	}

	printf("\n\n");
	system("pause");
	return 0;
}

/* ��ȡ�ļ��ڵĺ������� */
static int getFuncNums(char *fileName)
{
	int ret = 0;
	FILE *fp = NULL;
	int matchFlagL = 0, matchFlagR = 0;
	char keyC = 0;
	int startFuncFlag = 0, endFuncFlag = 0;
	int globalStructFlag = 1;

	if(NULL != fileName)
	{
		/* ���ļ� */
		fp = fopen(fileName, "r");
		if(NULL != fp)
		{
			/* ���ֽڶ�ȡ�ַ� */
			keyC = fgetc(fp);
			while(keyC != EOF)
			{
				/* �ؼ��ַ�ƥ�䣬 ע��123�������ŵ�ASCIIֵ  */
				if(123 == keyC)
				{
					/* ��ǰ�Ѿ����뺯���ڲ��Ļ�����ƥ�� */
					if(startFuncFlag == 1)
					{
						matchFlagL++;
					}
					else
					{
						/* ��Ϊ����������Ǻ�����ʼ */
						if(1 == isLikeFuncFlag(beforeChar))
						{
							startFuncFlag = 1;
							matchFlagL = 1;
						}
						else
						{
						}
					}
				}
				else if(125 == keyC)	/* 125���һ����ŵ�ASCIIֵ */
				{
					/* ��ƥ�亯��ʱ�������Ÿ����������������� */
					if (matchFlagR < matchFlagL)
					{
						matchFlagR++;
					}
					else	/* ������п�����ȫ�ֱ������һ����� */
					{}
				}
				else
				{
					/* ����ƥ�䵽������֮ǰ�������ַ� */
					updateCharArray(beforeChar, keyC);
				}

				/* �����������ź�������ƥ����� */
				if((matchFlagL == matchFlagR) && (matchFlagL != 0))
				{
					ret++;
					matchFlagL = 0;
					matchFlagR = 0;
					startFuncFlag = 0;
				}
				else
				{}

				/* ��ȡ��һ���ַ� */
				keyC = fgetc(fp);
			}
			fclose(fp);
		}
		else
		{
			printf("Err: Open File Failed. %s-%d\n",__FILE__,__LINE__);
		}
	}
	else
	{
		printf("Err: Null-Pointer File Name. %s-%d\n",__FILE__,__LINE__);
	}

	return ret;
}

/* �����������Ǵ������µ��ַ� */
static void updateCharArray(char bfChar[], char newCh)
{
	int i = 0;
	if(bfChar != NULL)
	{
		for(i=0; i<(STR_ARRAY_LEN-1); i++)
		{
			bfChar[i] = bfChar[i+1];
		}
		bfChar[STR_ARRAY_LEN-1] = newCh;
	}
	else
	{
		printf("Err: Null-pointer. %s:%d\n",__FILE__,__LINE__);
	}
}

/* �жϵ�ǰ����������Ƿ��п����Ǻ�����ʼ */
static int isLikeFuncFlag(char bfChar[])
{
	int ret = -2;
	int i = 0;
	char tmpChar = 0;

	if (bfChar != NULL)
	{
		/* �Ӻ���ǰ���� */
		for(i=(STR_ARRAY_LEN-1); i>=0; i--)
		{
			/* �ҵ���һ���ǿո�TAB���е��ַ� */
			if((' ' != bfChar[i]) && ('\t' != bfChar[i]) && ('\n' != bfChar[i]))
			{
				tmpChar = bfChar[i];
				break;
			}
			else
			{
				ret = 0	;
			}
		}
		/* ����������ǰ���һ���ɼ��ַ���) */
		if (')' == tmpChar)
		{
			/* ����п����Ǻ��� */
			ret = 1;
		}
		else
		{
			ret = 0;
		}
	}
	else
	{
		ret = -1;
		printf("Err: Null-pointer. %s:%d\n", __FILE__, __LINE__);
	}
	return ret;
}


#ifdef __cplusplus
}
#endif
