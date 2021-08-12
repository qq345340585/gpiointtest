#include "gpioconfig.h"

extern void GpioReback(int gpionum,int value);
int keepgoing = 1;
// Callback called when SIGINT is sent to the process (Ctrl-C)
void signal_handler(int sig)
{
	printf( "Ctrl-C pressed, cleaning up and exiting..\n" );
	keepgoing = 0;
}

int Watching(int gpionum)
{
	/* initial GPIO*/ 
	 
	int value = 0,i=0;
	int nfds = 0;
	int gpio_fd, timeout;
	struct epoll_event evd;
	struct epoll_event * events;
	char buf[MAX_BUF];
	int len;
	int cnt = 0;
	unsigned int valueback = 0;

	signal(SIGINT, signal_handler);

   

	 /*GPIO-34, SODIMM-135, Input, Interrupt*/
	     gpio_export(gpionum);
		 gpio_set_dir(gpionum, "in");
		 gpio_set_edge(gpionum, "rising");
		 printf ("\nSucessfully Configure GPIO-%d as input\n", gpionum);
		 gpio_fd = gpio_fd_open(gpionum, O_RDONLY);

		 timeout = POLL_TIMEOUT;

		int epollfd = epoll_create(10);
		events =  calloc (10, sizeof(struct epoll_event));
		evd.data.fd = gpio_fd;
		evd.events = EPOLLPRI;
		epoll_ctl(epollfd,EPOLL_CTL_ADD,gpio_fd,&evd);

        while (keepgoing) {
		 		nfds = epoll_wait(epollfd,events,10,-1);
		        if (nfds < 0) {
				 			printf("\npoll() failed!\n");
				 			return -1;
				 	    }
        	    for (i = 0;i<nfds;i++ )
        	    {
        	    	if (events->events&EPOLLPRI)
        	    	{
        	    		memset(buf,0x00,sizeof(buf));
        	    		len = read(events->data.fd,buf,sizeof(buf));
        	    		lseek(events->data.fd,0,SEEK_SET);
        	    		if (cnt > 0)
        	    		{
        	    			usleep(1);
        	    			gpio_get_value(gpionum, &valueback);
							GpioReback(gpionum,valueback);
        	    		}
        	    		cnt++;
        	    	}
        	    }
		}
		gpio_fd_close(gpio_fd);
		return (0);
}
