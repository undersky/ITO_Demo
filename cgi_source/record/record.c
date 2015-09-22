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

void show_tables(void);
char buf[512];

int cgiMain() 
{
	
	FILE *fp;

    fp = fopen("../record.html","r");

    if(fp == NULL)
    {
        exit(1);
    }

    
    cgiHeaderContentType("text/html;charset=\"utf-8\"");
    while(fgets(buf,512,fp))
    {
        if(strstr(buf,"Insert"))
        {
            fprintf(cgiOut,"I found it");
          //show_tables();
        }
        else
        {
            fprintf(cgiOut,buf);    
        }
    }



    fclose(fp);
	return 0;
}

void show_tables(void)
{
    sqlite3 *db = NULL;
    char *errmsg = NULL;
    char sql[256];
    int ret = 0;
    char **dbresult;
    int nrow;
    int ncolumn;
    int i,j;

    ret = sqlite3_open("../db/record.db",&db);
    if(ret != SQLITE_OK)
    {
        perror("fail to open record.db!\n");
        exit(-2);
    }

    memset(sql,0,256);
    sprintf(sql,"select * from records");
    ret = sqlite3_get_table(db,sql,&dbresult,&nrow,&ncolumn,&errmsg);
    if(ret != SQLITE_OK)
    {
        perror("fail to exec:");
        exit(-1);
    }

    if(ncolumn !=0)
    {
        for(i=0;i<nrow;i++)
        {
            fprintf(cgiOut,"<tr>");
            for(j=0;j<ncolumn;j++)
            {
                fprintf(cgiOut,"<th>");

                fprintf(cgiOut,"%s",&dbresult[i][j]);
                fprintf(cgiOut,"</th>");
            }
            fprintf(cgiOut,"</tr>");
        }
    }

    sqlite3_close(db);
}



