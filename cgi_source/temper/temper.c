/* Change this if the SERVER_NAME environment variable does not report
	the true name of your web server. */
#if 1
#define SERVER_NAME cgiServerName
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

#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/msg.h>
#include <stdio.h>
#include "cgic.h"
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>



char buf[512];
unsigned char temp[2];

float get_temp(void)
{

    float rnt=0;

    int fd = -1;
    fd = open("/dev/TEM0",O_RDWR|O_NDELAY|O_NOCTTY);
    if(fd < 0)
    {
        exit(1);
    }
    
    read(fd,buf,sizeof(buf));

}

int cgiMain()
{	
    FILE *fp;
    float result=0;
    int fd;

    fp = fopen("../temper.html","r");
    if(fp == NULL)
    {
        exit(1);
    }

    fd = open("/dev/TEM0",O_RDWR|O_NDELAY|O_NOCTTY);
    if(fd < 0)
    {
        exit(1);
    }
    
    read(fd,buf,sizeof(buf));

    cgiHeaderContentType("text/html;charset=\"utf-8\"");
	
    while(fgets(buf,512,fp))
    {
        if(strstr(buf,"$CurTemp$"))
        {

            fprintf(cgiOut,"%d.%d ",buf[0],buf[1]);
        }
        else if(strstr(buf,"$CurHumidity$"))
        {
            fprintf(cgiOut,"23 %");
        }
        else
        {
            fprintf(cgiOut,buf);

        }
    }
    fclose(fp);
    close(fd);
    return 0;

}

