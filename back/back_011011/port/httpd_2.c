/*
 * Copyright (c) 2001, Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * httpd.c
 *
 * Author : Adam Dunkels <adam@sics.se>
 *
 * CHANGELOG: this file has been modified by Sergio Perez Alcañiz <serpeal@upvnet.upv.es>
 *            Departamento de Informática de Sistemas y Computadores
 *            Universidad Politécnica de Valencia
 *            Valencia (Spain)
 *            Date: March 2003
 *
 */

#include "httpd.h"
#include "lwip/tcp.h"
#include "fsdata.c"
#include "main.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


struct http_state
{
  char *file;
  u32_t left;
};

char 
*post_svs(char *str)
{
    char *pch;
    //char *result = NULL;
  char *pcs = NULL;
  pch = strtok (str,"\r\n");
 while (pch != NULL)
  {
	//memset( pcs, 0);
	strcpy(pcs, pch);
    //printf ("%s\n",pch);
   pch = strtok (NULL, "\r\n");
  }
  return pcs;
}

/*-----------------------------------------------------------------------------------*/
static void
conn_err(void *arg, err_t err)
{
  struct http_state *hs;
  hs = arg;
  mem_free(hs);
}
/*-----------------------------------------------------------------------------------*/
static void
close_conn(struct tcp_pcb *pcb, struct http_state *hs)
{

  tcp_arg(pcb, NULL);
  tcp_sent(pcb, NULL);
  tcp_recv(pcb, NULL);
  mem_free(hs);
  tcp_close(pcb);
}
/*-----------------------------------------------------------------------------------*/
static void
send_data(struct tcp_pcb *pcb, struct http_state *hs)
{
  err_t err;
  u16_t len;

  /* We cannot send more data than space avaliable in the send
     buffer. */
  if (tcp_sndbuf(pcb) < hs->left)
  {
    len = tcp_sndbuf(pcb);
  }
  else
  {
    len = hs->left;
  }

  err = tcp_write(pcb, hs->file, len, 0);

  if (err == ERR_OK)
  {
    hs->file += len;
    hs->left -= len;
  }
}

/*-----------------------------------------------------------------------------------*/
static err_t
http_poll(void *arg, struct tcp_pcb *pcb)
{
  if (arg == NULL)
  {
    /*    printf("Null, close\n");*/
    tcp_close(pcb);
  }
  else
  {
    send_data(pcb, (struct http_state *)arg);
  }

  return ERR_OK;
}
/*-----------------------------------------------------------------------------------*/
static err_t
http_sent(void *arg, struct tcp_pcb *pcb, u16_t len)
{
  struct http_state *hs;

  hs = arg;

  if (hs->left > 0)
  {
    send_data(pcb, hs);
  }
  else
  {
    close_conn(pcb, hs);
  }

  return ERR_OK;
}
/*-----------------------------------------------------------------------------------*/
static err_t
http_recv(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err)
{
char mydata[1024];
 int len;
      char *pc;
  int i, j;
  char *data;
  char *pdata;
  char fname[40];
  struct fs_file file = {0, 0};
  struct http_state *hs;

  hs = arg;

  if (err == ERR_OK && p != NULL)
  {

    /* Inform TCP that we have taken the data. */
    tcp_recved(pcb, p->tot_len);

    if (hs->file == NULL)
    {
      data = p->payload;
	  
        
      if ((strncmp(data, "GET /valt.css", 13) == 0) || (strncmp(data, "POST /valt.css", 14) == 0 ))
      {
        char Digit1=0, Digit2=0, Digit3=0; 
        int ADCVal = 0;        
        char uart_buf[40]; 
        char i;
        pbuf_free(p);
	
        // ADCVal = adc_p1;
        // ADCVal = ADCVal/8;
        // Digit1= ADCVal/100;
        // Digit2= (ADCVal-(Digit1*100))/10;
        // Digit3= ADCVal-(Digit1*100)-(Digit2*10);
        
        // *((data_control_html)+ 0xaa) = 0x30 + Digit1; /* ADC value 1st digit */
        // *((data_control_html)+ 0xab) = 0x30 + Digit2; /* ADC value 2nd digit */ 
        // *((data_control_html)+ 0xac) = 0x30 + Digit3; /* ADC value 3rd digit*/
        
        i=0;
        /* Update the ADC value in control.html */
        *((data_valt_css)+ 0x32) = tempc1[i++];
        *((data_valt_css)+ 0x33) = tempc1[i++]; 
        *((data_valt_css)+ 0x34) = tempc1[i++]; 
        *((data_valt_css)+ 0x35) = tempc1[i++]; 
        *((data_valt_css)+ 0x36) = tempc1[i++];
        
          i=0;
        /* Update the ADC value in control.html */
        *((data_valt_css)+ 0x57) = tempc2[i++];
        *((data_valt_css)+ 0x58) = tempc2[i++]; 
        *((data_valt_css)+ 0x59) = tempc2[i++]; 
        *((data_valt_css)+ 0x5a) = tempc2[i++]; 
        *((data_valt_css)+ 0x5b) = tempc2[i++];
        /*
        sprintf(uart_buf,"%s:%s\r\n",tempc1,tempc2);
        
        printUART2((unsigned int *)uart_buf);
        strcat(data_control_html,uart_buf);
        */
		sprintf(uart_buf,"%02d:%02d:%02d",delh[0],delm[0],dels[0]);
		*((data_valt_css)+ 0xa5) = uart_buf[0];
		*((data_valt_css)+ 0xa6) = uart_buf[1];
		*((data_valt_css)+ 0xa8) = uart_buf[3];
		*((data_valt_css)+ 0xa9) = uart_buf[4];
		*((data_valt_css)+ 0xab) = uart_buf[6];
		*((data_valt_css)+ 0xac) = uart_buf[7];
		
		sprintf(uart_buf,"%02d:%02d:%02d",delh[1],delm[1],dels[1]);
		*((data_valt_css)+ 0x20a) = uart_buf[0];
		*((data_valt_css)+ 0x20b) = uart_buf[1];
		*((data_valt_css)+ 0x20d) = uart_buf[3];
		*((data_valt_css)+ 0x20e) = uart_buf[4];
		*((data_valt_css)+ 0x210) = uart_buf[6];
		*((data_valt_css)+ 0x211) = uart_buf[7];
		
		sprintf(uart_buf,"%02d:%1d:%1d",modes,sdoor,ebuff[39]);
		*((data_valt_css)+ 0x7c) = uart_buf[0];
		*((data_valt_css)+ 0x7d) = uart_buf[1];
		*((data_valt_css)+ 0x7e) = uart_buf[2];
		*((data_valt_css)+ 0x7f) = uart_buf[3];
		*((data_valt_css)+ 0x80) = uart_buf[4];
		*((data_valt_css)+ 0x81) = uart_buf[5];
		
		
		if(start==1)
		{
			sprintf(uart_buf,"START");
			*((data_valt_css)+ 0x245) = 0x66;
		}
		
		else
		{
			sprintf(uart_buf,"STOP ");
			*((data_valt_css)+ 0x245) = 0x32;
		}
		
		*((data_valt_css)+ 0x233) = uart_buf[0];
		*((data_valt_css)+ 0x234) = uart_buf[1];
		*((data_valt_css)+ 0x235) = uart_buf[2];
		*((data_valt_css)+ 0x236) = uart_buf[3];
		*((data_valt_css)+ 0x237) = uart_buf[4];
		
		
	//	*((data_valt_css)+ 0x211) = uart_buf[7];
		
        uint16_t cp1;
        uint16_t cp2;
        uint16_t cp3;
        uint16_t cp4;
        uint16_t cp5;
        uint16_t cp6;
        
        cp1=0xd6;
        cp2=0x104;
        cp3=0x132;
		cp4=0x160;
        cp5=0x18e;
        cp6=0x1bc;

		if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_9)) *((data_valt_css)+ cp1) = 0x6e;//n
		else{ *((data_valt_css)+ cp1) = 0x66;//f
				//	if(leds[0]==3)leds[0] = 2;
				}
		
		if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_10)) *((data_valt_css)+cp2) = 0x6e;
		else{ *((data_valt_css)+ cp2) = 0x66; 
		//if(leds[1]==3)leds[1] = 2;
		 }
		
		if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_11)) *((data_valt_css)+ cp3) = 0x6e;
		else{ *((data_valt_css)+ cp3) = 0x66; 
		//if(leds[2]==3)leds[2] = 2;
		}
				
		if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_12)) *((data_valt_css)+cp4) = 0x6e;
		else{ *((data_valt_css)+ cp4) = 0x66;
		//if(leds[3]==3)leds[3] = 2;
		}
        
		if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_13)) *((data_valt_css)+ cp5) = 0x6e;
		else *((data_valt_css)+ cp5) = 0x66;
            
		if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_14)) *((data_valt_css)+ cp6) = 0x6e;
		else *((data_valt_css)+ cp6) = 0x66;
		
        fs_open("/valt.css", &file);
        
        hs->file = file.data;
        hs->left = file.len;

        send_data(pcb, hs);

        /* Tell TCP that we wish be to informed of data that has been
           successfully sent by a call to the http_sent() function. */
        tcp_sent(pcb, http_sent);
      }
	  else if ((strncmp(data, "GET /ind.html", 13) == 0) || (strncmp(data, "POST /ind.html", 14) == 0 ))
      {
        
        pbuf_free(p);
            
        fs_open("/ind.html", &file);

        hs->file = file.data;
        hs->left = file.len;
      //  test_i2c();
        send_data(pcb, hs);

        /* Tell TCP that we wish be to informed of data that has been
           successfully sent by a call to the http_sent() function. */
        tcp_sent(pcb, http_sent);
      }
	   else if (strncmp(data, "GET /uppu", 9) == 0)
      {
        
        pbuf_free(p);
            
        fs_open("/index.html", &file);

        hs->file = file.data;
        hs->left = file.len;
       
        send_data(pcb, hs);

        /* Tell TCP that we wish be to informed of data that has been
           successfully sent by a call to the http_sent() function. */
        tcp_sent(pcb, http_sent);
      }
	else if ((strncmp(data, "GET /val.js", 11) == 0)|| (strncmp(data, "POST /val.js", 12) == 0 ))
      {
		char i,ii;
		char _buf[3]; 
		char cl;
	 
	
	  for(i=0;i<30;i++)
	  {
		sprintf(_buf,"%i;",ebuff[i]);
		cl = strlen(_buf);
		for(ii=0;ii<cl;ii++)
		*((data_val_js)+ 0x42+(i*12)+ii) = _buf[ii];
	   
	   }
	     pbuf_free(p);
	   
	     fs_open("/val.js", &file);

        hs->file = file.data;
        hs->left = file.len;
       
        send_data(pcb, hs);

        /* Tell TCP that we wish be to informed of data that has been
           successfully sent by a call to the http_sent() function. */
        tcp_sent(pcb, http_sent);
        /*
        pbuf_free(p);
            
        fs_open("/foot.html", &file);

        hs->file = file.data;
        hs->left = file.len;
       
        send_data(pcb, hs);

        /* Tell TCP that we wish be to informed of data that has been
           successfully sent by a call to the http_sent() function. */
       // tcp_sent(pcb, http_sent);
      }
      else if (strncmp(data, "GET /method=get", 15) == 0)
      {
        i = 15;
		//ET-STM32F ARM KIT Hardware Board
		/*
		out_off(k0);
		out_off(k1);
		out_off(k2);
		out_off(k3);
		out_off(k4);
		out_off(k5);
		out_off(k6);
		*/
		// out_off(d7);
		//onmousedown
		//onmouseup
        while(data[i]!=0x20/* */)
        {
          i++; 
          if (data[i] == 0x6C /* l */)
          {
            i++;
            if (data[i] ==  0x65 /* e */)
            {
              i++;
              if (data[i] ==  0x64 /* d*/)
              {
                i++; 
				//ET-STM32F ARM KIT Hardware Board
                if(data[i]==0x31 /* 1 */)
                {
                   out_toggle(k0);
				   //leds[0] = 1;
                }
  
                if(data[i]==0x32 /* 2 */)
                {
                   out_toggle(k1);   
				   //leds[1] = 1;
                }
        
                if(data[i]==0x33 /* 3 */)
                {
                   out_toggle(k2); 
				   //leds[2] = 1;
                }
            
                if(data[i]==0x34 /* 4 */)
                {
                     out_toggle(k3);  
					 //leds[3] = 1;
                }		
                if(data[i]==0x35 /* 5 */)
                {
					out_toggle(k4);  
                }		
                if(data[i]==0x36 /* 6 */)
                {
                   out_toggle(k5);  
                }	
                if(data[i]==0x37 /* 7 */)
                {
					outpp(0x00);
                }	
				if(data[i]==0x38 /* 8 */)
                {
					if(start==1)
					{
						start = 3;
					}						
					else 
					{
						start = 5;
					
					}						
                }			
              }   
            }
					
          }
	
        }
		i=15;
	if (data[i] == 0x25/* % */)
          {
			  i++; 
          if (data[i] != 0x6C /* l */)
          {
			
			char delims[] = ":";
			char *result = NULL;
			
			unsigned char ci,cl,cf,bci;
			char bv;
			ci=0;
			bci=0;
			result = strtok( data, delims );
			while( result != NULL ) {
			if(bci!=0)
			{
			bv=0;
			cl = strlen(result);
				//printf( "result is \"%s\"\n", result );
				/*
					if(result[0]==0x2d)
					{
						if(cl==2)
						{
							bv = (result[1]-30);
						}
						else if(cl == 3)
						{
							bv = (result[1]-30)*10;
							bv += (result[2]-30);
						}
						else bv=0;
					}
					else
					{
						if(cl==1)
						{
							bv = (result[0]-30);
						}
						else if(cl==2)
						{
							bv = (result[0]-30)*10;
							bv += (result[1]-30);
						}
						else bv=0;
					}
					*/
					bv = atoi(result);
					//ebuff[ci] = bv;
					ci++;
				}
								
				result = strtok( NULL, delims );
				bci++;
			}
			eemodes=10;
			pbuf_free(p);
			fs_open("/ind.html", &file);
			}
		  }		  
		  else 
		  {
		   pbuf_free(p);
		  fs_open("/valt.css", &file);
		  }
      //  fs_open("/index.html", &file);

       hs->file = file.data;
       hs->left = file.len;
       
		send_data(pcb, hs);

        /* Tell TCP that we wish be to informed of data that has been
           successfully sent by a call to the http_sent() function. */
        tcp_sent(pcb, http_sent);
      }    
      else if (strncmp(data, "GET ", 4) == 0)
      {
        for (i = 0; i < 40; i++)
        {
          if (((char *)data + 4)[i] == ' ' ||
              ((char *)data + 4)[i] == '\r' ||
              ((char *)data + 4)[i] == '\n')
          {
            ((char *)data + 4)[i] = 0;
          }
        }
        
        i = 0;
        j = 0;
        
        do
        {
          fname[i] = ((char *)data + 4)[j];
          j++;
          i++;
        } while (fname[i - 1] != 0 && i < 40);
        
        pbuf_free(p);

        if (!fs_open(fname, &file))
        {
          fs_open("/index.html", &file);
        }
        hs->file = file.data;
        hs->left = file.len;

        send_data(pcb, hs);

        /* Tell TCP that we wish be to informed of data that has been
           successfully sent by a call to the http_sent() function. */
        tcp_sent(pcb, http_sent);
      }
	  else if(strncmp(data, "POST ", 5) == 0)
	  {
		pdata = data;
        for (i = 0; i < 40; i++)
        {
          if (((char *)data + 5)[i] == ' ' ||
              ((char *)data + 5)[i] == '\r' ||
              ((char *)data + 5)[i] == '\n')
          {
            ((char *)data + 5)[i] = 0;
          }
        }
        
        i = 0;
        j = 0;
        
        do
        {
          fname[i] = ((char *)data + 5)[j];
          j++;
          i++;
        } while (fname[i - 1] != 0 && i < 40 && fname[i - 1] != 20);
        
        pbuf_free(p);

        if (!fs_open(fname, &file))
        {
			// *((data_foot_html)+ 0xa2) = 0x30 + Digit3; /* ADC value 3rd digit*/
			 
			//post_svs(pdata);
			fs_open("/index.html", &file);
        }
		
		/*
		strcat(file.data, hs->file);
		file.data = file.data;
		file.len = strlen(file.data);
		*/
        hs->file = file.data;
        hs->left = file.len;

        send_data(pcb, hs);

        /* Tell TCP that we wish be to informed of data that has been
           successfully sent by a call to the http_sent() function. */
        tcp_sent(pcb, http_sent);
	  
	  }
      else
      {
        close_conn(pcb, hs);
      }
    }
    else
    {
      pbuf_free(p);
    }
  }

  if (err == ERR_OK && p == NULL)
  {

    close_conn(pcb, hs);
  }
  
  return ERR_OK;
}
/*-----------------------------------------------------------------------------------*/
static err_t
http_accept(void *arg, struct tcp_pcb *pcb, err_t err)
{
  struct http_state *hs;

  /* Allocate memory for the structure that holds the state of the
     connection. */
  hs = mem_malloc(sizeof(struct http_state));

  if (hs == NULL)
  {
    return ERR_MEM;
  }

  /* Initialize the structure. */
  hs->file = NULL;
  hs->left = 0;

  /* Tell TCP that this is the structure we wish to be passed for our
     callbacks. */
  tcp_arg(pcb, hs);

  /* Tell TCP that we wish to be informed of incoming data by a call
     to the http_recv() function. */
  tcp_recv(pcb, http_recv);
 
  tcp_err(pcb, conn_err);

  tcp_poll(pcb, http_poll, 10);
  return ERR_OK;
}
/*-----------------------------------------------------------------------------------*/
void
httpd_init(void)
{
  struct tcp_pcb *pcb;

  pcb = tcp_new();
  tcp_bind(pcb, IP_ADDR_ANY, 80);
  pcb = tcp_listen(pcb);
  tcp_accept(pcb, http_accept);
  //  printU("int");
}
/*-----------------------------------------------------------------------------------*/
int
fs_open(char *name, struct fs_file *file)
{
  struct fsdata_file_noconst *f;

  for (f = (struct fsdata_file_noconst *)FS_ROOT; f != NULL; f = (struct fsdata_file_noconst *)f->next)
  {
    if (!strcmp(name, f->name))
    {
      file->data = f->data;
      file->len = f->len;
      return 1;
    }
  }
  return 0;
}
/*-----------------------------------------------------------------------------------*/

