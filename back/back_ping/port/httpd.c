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
	  
        
      if ((strncmp(data, "GET /control.html", 17) == 0) || (strncmp(data, "POST /control.html", 18) == 0 ))
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
        *((data_control_html)+ 0x6a) = tempc1[i++];
        *((data_control_html)+ 0x6b) = tempc1[i++]; 
        *((data_control_html)+ 0x6c) = tempc1[i++]; 
        *((data_control_html)+ 0x6d) = tempc1[i++]; 
        *((data_control_html)+ 0x6e) = tempc1[i++];
        
          i=0;
        /* Update the ADC value in control.html */
        *((data_control_html)+ 0x77) = tempc2[i++];
        *((data_control_html)+ 0x78) = tempc2[i++]; 
        *((data_control_html)+ 0x79) = tempc2[i++]; 
        *((data_control_html)+ 0x7a) = tempc2[i++]; 
        *((data_control_html)+ 0x7b) = tempc2[i++];
        /*
        sprintf(uart_buf,"%s:%s\r\n",tempc1,tempc2);
        
        printUART2((unsigned int *)uart_buf);
        strcat(data_control_html,uart_buf);
        */

		if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_9)) *((data_control_html)+ 0x14c) = 0x6e;//n
		else{ *((data_control_html)+ 0x14c) = 0x66;//f
					if(leds[0]==3)leds[0] = 2;
				}
		
		if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_10)) *((data_control_html)+0x228) = 0x6e;
		else{ *((data_control_html)+ 0x228) = 0x66; 
		if(leds[1]==3)leds[1] = 2;}
		
		if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_11)) *((data_control_html)+ 0x30a) = 0x6e;
		else{ *((data_control_html)+ 0x30a) = 0x66; 
		if(leds[2]==3)leds[2] = 2;}
				
		if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_12)) *((data_control_html)+0x3ea) = 0x6e;
		else{ *((data_control_html)+ 0x3ea) = 0x66;
		if(leds[3]==3)leds[3] = 2;}
		if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_13)) *((data_control_html)+ 0x4c8) = 0x6e;
		else *((data_control_html)+ 0x4c8) = 0x66;
		if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_14)) *((data_control_html)+ 0x5ac) = 0x6e;
		else *((data_control_html)+ 0x5ac) = 0x66;
		
		

		
        fs_open("/control.html", &file);
        
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
	else if (strncmp(data, "GET /foot.html", 14) == 0)
      {
        
        pbuf_free(p);
            
        fs_open("/foot.html", &file);

        hs->file = file.data;
        hs->left = file.len;
       
        send_data(pcb, hs);

        /* Tell TCP that we wish be to informed of data that has been
           successfully sent by a call to the http_sent() function. */
        tcp_sent(pcb, http_sent);
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
out_on(k0);

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
                   out_toggle(k1);
				   leds[0] = 1;
                }
  
                if(data[i]==0x32 /* 2 */)
                {
                   out_toggle(k2);   
				   leds[1] = 1;
                }
        
                if(data[i]==0x33 /* 3 */)
                {
                   out_toggle(k3); 
				   leds[2] = 1;
                }
            
                if(data[i]==0x34 /* 4 */)
                {
                     out_toggle(k4);  
					 leds[3] = 1;
                }		
                if(data[i]==0x35 /* 5 */)
                {
                     out_toggle(k5);  
                }		
                if(data[i]==0x36 /* 6 */)
                {
                     out_toggle(k6);  
                }	
                if(data[i]==0x37 /* 7 */)
                {
						out_off(k1);
						out_off(k2);
						out_off(k3);
						out_off(k4);
						out_off(k5);
						out_off(k6);
                }				
              }   
            }
          } 
        }

      pbuf_free(p);
            
        fs_open("/index.html", &file);

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

