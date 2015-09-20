/* Change this if the SERVER_NAME environment variable does not report
	the true name of your web server. */
#if 1
#define SERVER_NAME cgiServerName
#endif
#if 0
#define SERVER_NAME "www.boutell.com"
#endif

/* You may need to change this, particularly under Windows;
	it is a reasonable guess as to an acceptable place to
	store a saved environment in order to test that feature. 
	If that feature is not important to you, you needn't
	concern yourself with this. */

#ifdef WIN32
#define SAVED_ENVIRONMENT "c:\\cgicsave.env"
#else
#define SAVED_ENVIRONMENT "/tmp/cgicsave.env"
#endif /* WIN32 */

#include <stdio.h>
#include "cgic.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "sqlite3.h"

#define MAX_LEN 20

int check_user();
void HandleSubmit();
void ShowForm();
void getUsername();
void getPasswd();

char username[MAX_LEN];
char passwd[MAX_LEN];

int cgiMain() {
	
	int ret;
	HandleSubmit();
	if((ret = check_user(username,passwd)) == 0)
	{
		cgiHeaderContentType("text/html");
		fprintf(cgiOut, "<HTML>\n");
		fprintf(cgiOut, "<BODY bgcolor=\"ffffffff\">\n");
	    ShowForm();
		fprintf(cgiOut, "登录成功\n ");
	//	fprintf(cgiOut, "username:%s\n ",username);
	//	fprintf(cgiOut, "passwd:%s\n ",passwd);			
		fprintf(cgiOut, "</BODY></HTML>\n");	
			
		return 1;
	}
	//此处进行用户登录验证，如果密码错误则提示密码输入错误，并推出，否则继续下一步

	else
	{
		cgiHeaderContentType("text/html");
		fprintf(cgiOut, "<HTML>\n");
		fprintf(cgiOut, "<BODY bgcolor=\"#ffffffff\">\n");
	//	ShowForm();
	//		fprintf(cgiOut, "username:%s\n ",username);
	//	fprintf(cgiOut, "passwd:%s\n ",passwd);			
		fprintf(cgiOut,"<a href=\"../index.html\">..返回登录页面</a>\n");
		if(ret == -1 )
		{
			fprintf(cgiOut, "<H3 align=\"center\">登录失败，没有此账号</H3>\n ");
		}
		if(ret == 1 )
		{
			fprintf(cgiOut, "<H3 align=\"center\">登录失败，密码错误</H3>\n ");
		}
		if(ret == -2 )
		{
			fprintf(cgiOut, "<H3 align=\"center\">数据库打开失败</H3>\n ");
		}
		if(ret == -3 )
		{
			fprintf(cgiOut, "<H3 align=\"center\">exec fail</H3>\n ");
		}
		fprintf(cgiOut, "</BODY></HTML>\n");

	}

	return 0;
}

void HandleSubmit()
{
	 getUsername();
	 getPasswd();
}
//获取用户名
void getUsername() {
//	char username[81];
	memset(username,0,sizeof(username));
	cgiFormStringNoNewlines("inputEmail", username, MAX_LEN);
	fprintf(cgiOut, "username:%s\n ",username);
}
//获取登陆密码
void getPasswd() {
//	char passwd[81];
    memset(passwd,0,sizeof(passwd));
	cgiFormStringNoNewlines("inputPassword", passwd, MAX_LEN);
	fprintf(cgiOut, "passwd:%s\n ",passwd);
}
//跳转到登录成功界面
void ShowForm()
{
	fprintf(cgiOut, "<!-- 2.0: multipart/form-data is required for file uploads. -->");
	fprintf(cgiOut, "<meta http-equiv=\"refresh\" content=\"0;url=../main.html\">");
}

#if 0
int check_user(char *name,char *password)
{
	int rst = 0;
	if(strncmp(name,"ming",4) == 0)
		rst = 1;
	else
		rst = 0;
	if(strncmp(password,"1234",4) == 0)
		rst = 1;
	else
		rst = 0;
	return rst;
	
}
#endif
#if 1
int check_user(char *name ,char *password)
{
	sqlite3 *db = NULL;
	char *errmsg = NULL;
	char sql[256];
	int ret = 0;
	char **dbresult;
	int nrow;
	int ncolumn;
#ifdef DEBUG
	int index;
	int i ,j;
#endif
	ret = sqlite3_open("../db/user.db",&db);
	if(ret != SQLITE_OK)
	{
		perror("fail to open user.db:\n");
		return -2;
	}
	memset(sql,0,256);
	sprintf(sql,"select passwd from users where email='%s'",username);
	ret = sqlite3_get_table(db,sql,&dbresult,&nrow,&ncolumn,&errmsg);
	if(ret != SQLITE_OK)
	{
		perror("fail to exec:");
		return -3;
	}
	if(ncolumn != 0)
	{
#ifdef DEBUG
		fprintf(cgiOut, "nrow %d ncolumn %d\n",nrow,ncolumn);
		index = ncolumn;
		for(i = 0;i <nrow;i ++)
		{
			for(j = 0;j < ncolumn; j++)
			{
			fprintf(cgiOut,"name %s value %s\n",dbresult[i],(dbresult[index]));
				++ index;
			}
			printf("****************\n");
		}
#endif
		if(strcmp(dbresult[ncolumn*nrow],password) == 0)
		{
			ret = 0;
			goto end;
		}
		else
		{
			ret = 1;
			goto end;
		}
	}	
	else 
	{
		ret = -1;
		goto end;
	}
end:
	sqlite3_close(db);
	return ret;
}
#endif
