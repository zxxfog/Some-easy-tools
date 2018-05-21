/*
*      
*          ┌─┐       ┌─┐
*       ┌──┘ ┴───────┘ ┴──┐
*       │                 │
*       │       ───       │
*       │  ─┬┘       └┬─  │
*       │                 │
*       │       ─┴─       │
*       │                 │
*       └───┐         ┌───┘
*           │         │
*           │         │
*           │         │
*           │         └──────────────┐
*           │                        │
*           │                        ├─┐
*           │                        ┌─┘    
*           │                        │
*           └─┐  ┐  ┌───────┬──┐  ┌──┘         
*             │ ─┤ ─┤       │ ─┤ ─┤         
*             └──┴──┘       └──┴──┘ 
*                 神兽保佑 
*                 代码无BUG! 
*/


#include <stdio.h>
#include <io.h>
#include <string.h>
#include <unistd.h>
#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

/* 介绍： 本文件是计算某个文件夹内的所有.c文件包含的函数个数（包括被注释掉的函数），
* 方法（不敢称为算法。。。）就是检测代码文件中{}左右花括号的匹配
*  其中有一些特别关注点就是结构体变量的初始化带来的花括号使用 */

#define STR_ARRAY_LEN	(128u)
static char folderHead[25][16] = {"B_FUNC/", "B_GEN/", "B_IF/", "D_CFG/", "D_DD/", "F_CI/", "F_HOV/", "F_MA/", "F_PF/", "F_SIEVE/", "F_TRACE/", "F_TSR/", "F_VOBC/", "I_ATS/", "I_CI/", "I_MNM/", "I_SYN/", "I_VOBC/", "I_ZC/", "M_FMAIN/", "M_IMAIN/", "M_MAIN/", "P_IF/", "S_RI/", "S_RII/"};

static int getFuncNums(char *fileName);
static void updateCharArray(char bfChar[], char newCh);
static int isLikeFuncFlag(char bfChar[]);

/* 存储检索到左花括号之前的字符 */
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

	/* 打印当前的工作目录 */
	memset(cwdBuffer, 0, 256);
	(void)getcwd(cwdBuffer, 256);
	printf("Doc folder: %s\n",cwdBuffer);

	/* 组源代码目录 */
	memset(cwdBuffer, 0, 256);
	sprintf(cwdBuffer, "%s%s", folderNew, folderHead[SRC_FOLDER_INDEX]);
	(void)chdir(cwdBuffer);
	memset(cwdBuffer, 0, 256);
	(void)getcwd(cwdBuffer, 256);
	printf("Src folder: %s\n",cwdBuffer);

	/* 查找本文件夹内的文件 */
	fileHand = _findfirst(filePathStr,&fileInfo);

	printf("\n\n");

	/* 文件查找失败 */
	if(-1 == fileHand)
	{
		printf("Err: Find first file failed. %s-%d\n",__FILE__,__LINE__);
	}
	else
	{
		do
		{
			/* 新打开一个文件，要清理缓存 */
			memset(beforeChar, 0, STR_ARRAY_LEN);
			fileNums++;
			funcNums += getFuncNums(fileInfo.name);
			printf("File Name: %s, funcNum: %d\n",fileInfo.name, getFuncNums(fileInfo.name));
		}
		/* 查找文件夹内的下一个文件 */
		while(_findnext(fileHand,&fileInfo) == 0);

		printf("\n\n*******************  Total Info  **********************\n");
		printf("File Nums : %d\n",fileNums);
		printf("Func Nums : %d\n",funcNums);
	}

	printf("\n\n");
	system("pause");
	return 0;
}

/* 获取文件内的函数个数 */
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
		/* 打开文件 */
		fp = fopen(fileName, "r");
		if(NULL != fp)
		{
			/* 逐字节读取字符 */
			keyC = fgetc(fp);
			while(keyC != EOF)
			{
				/* 关键字符匹配， 注：123是左花括号的ASCII值  */
				if(123 == keyC)
				{
					/* 当前已经进入函数内部的花括号匹配 */
					if(startFuncFlag == 1)
					{
						matchFlagL++;
					}
					else
					{
						/* 认为这个花括号是函数开始 */
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
				else if(125 == keyC)	/* 125是右花括号的ASCII值 */
				{
					/* 在匹配函数时，右括号个数总是少于左括号 */
					if (matchFlagR < matchFlagL)
					{
						matchFlagR++;
					}
					else	/* 这里就有可能是全局变量的右花括号 */
					{}
				}
				else
				{
					/* 更新匹配到花括号之前的其他字符 */
					updateCharArray(beforeChar, keyC);
				}

				/* 函数的左括号和右括号匹配完成 */
				if((matchFlagL == matchFlagR) && (matchFlagL != 0))
				{
					ret++;
					matchFlagL = 0;
					matchFlagR = 0;
					startFuncFlag = 0;
				}
				else
				{}

				/* 读取下一个字符 */
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

/* 数组的最后总是存入最新的字符 */
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

/* 判断当前这个左花括号是否有可能是函数开始 */
static int isLikeFuncFlag(char bfChar[])
{
	int ret = -2;
	int i = 0;
	char tmpChar = 0;

	if (bfChar != NULL)
	{
		/* 从后往前遍历 */
		for(i=(STR_ARRAY_LEN-1); i>=0; i--)
		{
			/* 找到第一个非空格、TAB或换行的字符 */
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
		/* 从左花括号往前查第一个可见字符是) */
		if (')' == tmpChar)
		{
			/* 这就有可能是函数 */
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
