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



char mode[4][64];

void get_mode()
{
	cgiFormStringNoNewlines("led1_radio",&mode[0][0], 64);
	cgiFormStringNoNewlines("led2_radio",&mode[1][0], 64);
	cgiFormStringNoNewlines("led3_radio",&mode[2][0], 64);
	cgiFormStringNoNewlines("led4_radio",&mode[3][0], 64);


}



int cgiMain()
{	
    int fd=0;

    fd = open("/dev/leds",0);
    if(fd < 0)
    {
        exit(1);
    }

    get_mode();

    cgiHeaderContentType("text/html;charset=\"utf-8\"");
    fprintf(cgiOut,"<html>\n");
    fprintf(cgiOut,"<body>\n");

    if(strncmp(&mode[0][0],"led1_on",7) == 0)
    {
        ioctl(fd,1,0);
        fprintf(cgiOut,"LED1 opened!!\n");
    }
    else if(strncmp(&mode[0][0],"led1_off",7) == 0)
    {
        ioctl(fd,0,0);
        fprintf(cgiOut,"LED1 closeed!!\n");
    }
    else
    {
        
    
    }
    
    if(strncmp(&mode[1][0],"led2_on",7) == 0)
    {
        ioctl(fd,1,1);
        fprintf(cgiOut,"LED2 opened!!\n");
    }
    else if(strncmp(&mode[1][0],"led2_off",7) == 0)
    {
        ioctl(fd,0,1);
        fprintf(cgiOut,"LED2 closeed!!\n");
    }
    else
    {
        
    
    }

    if(strncmp(&mode[2][0],"led3_on",7) == 0)
    {
        ioctl(fd,1,2);
        fprintf(cgiOut,"LED3 opened!!\n");
    }
    else if(strncmp(&mode[2][0],"led3_off",7) == 0)
    {
        ioctl(fd,0,2);
        fprintf(cgiOut,"LED3 closeed!!\n");
    }
    else
    {
        
    
    }
   
    if(strncmp(&mode[3][0],"led4_on",7) == 0)
    {
        ioctl(fd,1,3);
        fprintf(cgiOut,"LED4 opened!!\n");
    }
    else if(strncmp(&mode[3][0],"led4_off",7) == 0)
    {
        ioctl(fd,0,3);
        fprintf(cgiOut,"LED4 closeed!!\n");
    }
    else
    {
        
    
    }
    fprintf(cgiOut,"<a href=\"../main.html\">..返回之前页面</a>\n");
    fprintf(cgiOut,"</body>\n");
    fprintf(cgiOut,"</html>\n");
    close(fd);
	return 0;

}

