/*
	HTTP CLIENT FOR RAW LWIP
	(c) 2008-2009 Noyens Kenneth
	PUBLIC VERSION V0.2 16/05/2009
 
	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU LESSER GENERAL PUBLIC LICENSE Version 2.1 as published by
	the Free Software Foundation.
 
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.
 
	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the
	Free Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 
*/
#include <stdlib.h>
#include <string.h>
#include "httpclient.h"

//#include "utils/ustdlib.h"
 
// Close a PCB(connection)
void hc_clearpcb(struct tcp_pcb *pcb)
{
	if(pcb != NULL)
	{
		// Close the TCP connection
    		tcp_close(pcb);
	}
}
 
// Function that lwip calls for handling recv'd data
err_t hc_recv(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err)
{
	struct hc_state *hcstate = arg;
	char * page = NULL;
	struct pbuf * temp_p;
	hc_errormsg errormsg = GEN_ERROR;
	int i;
 
    if((err == ERR_OK) && (p != NULL))
    {
		tcp_recved(pcb, p->tot_len);
 
		// Add payload (p) to hcstate
		temp_p = p;
		while(temp_p != NULL)
		{
			hcstate->RecvData = realloc(hcstate->RecvData, temp_p->len + hcstate->Len + 1);
 
			// CHECK 'OUT OF MEM'
			if(hcstate->RecvData == NULL)
			{
				// OUT OF MEMORY
				(*hcstate->ReturnPage)(hcstate->Num, OUT_MEM, NULL, 0);	
				return(ERR_OK);
			}
 
			strncpy(hcstate->RecvData + hcstate->Len, temp_p->payload, temp_p->len);
			hcstate->RecvData[temp_p->len + hcstate->Len] = '\0';			
			hcstate->Len += temp_p->len;
 
			temp_p = temp_p->next;
		}
 
		// Removing payloads
 
		while(p != NULL)
		{
			temp_p = p->next;
			pbuf_free(p);
			p = temp_p;
		}
 
    }
 
    // NULL packet == CONNECTION IS CLOSED(by remote host)
    else if((err == ERR_OK) && (p == NULL))
    {	
		// Simple code for checking 200 OK
		for(i=0; i < hcstate->Len; i++)
		{
			if(errormsg == GEN_ERROR)
			{
				// Check for 200 OK 
				if((*(hcstate->RecvData+i) == '2') && (*(hcstate->RecvData+ ++i) == '0') && (*(hcstate->RecvData+ ++i) == '0')) errormsg = OK;
				if(*(hcstate->RecvData+i) == '\n') errormsg = NOT_FOUND;
			}
			else
			{
				// Remove headers
				if((*(hcstate->RecvData+i) == '\r') && (*(hcstate->RecvData+ ++i) == '\n') && (*(hcstate->RecvData+ ++i) == '\r') && (*(hcstate->RecvData + ++i) == '\n'))
				{
					i++;
					page = malloc(strlen(hcstate->RecvData+i));
					strcpy(page, hcstate->RecvData+i);
					break;
				}
			}
		}
 
		if(errormsg == OK)
		{
			// Put recv data to ---> p->ReturnPage
			(*hcstate->ReturnPage)(hcstate->Num, OK, page, hcstate->Len);
		}
		else
		{
			// 200 OK not found Return NOT_FOUND (WARNING: NOT_FOUND COULD ALSO BE 5xx SERVER ERROR, ...) 
			(*hcstate->ReturnPage)(hcstate->Num, errormsg, NULL, 0);
		}
 
        // Clear the PCB
        hc_clearpcb(pcb);
 
		// free the memory containing hcstate
		free(hcstate->RecvData);
		free(hcstate);
    }
 
    return(ERR_OK);
}
 
// Function that lwip calls when there is an error
static void hc_error(void *arg, err_t err)
{
    struct hc_state *hcstate = arg;
    // pcb already deallocated
 
    // Call return function
    // TO-DO: Check err_t err for out_mem, ...
    (*hcstate->ReturnPage)(hcstate->Num, GEN_ERROR, NULL, 0);
 
    free(hcstate->RecvData);
    free(hcstate->PostVars);
    free(hcstate->Page);
    free(hcstate);
}
 
// Function that lwip calls when the connection is idle
// Here we can kill connections that have stayed idle for too long
static err_t hc_poll(void *arg, struct tcp_pcb *pcb)
{
    struct hc_state *hcstate = arg;
 
    hcstate->ConnectionTimeout++;
    if(hcstate->ConnectionTimeout > 20)
    {
        // Close the connection
        tcp_abort(pcb);
 
		// Give err msg to callback function
		// Call return function
		(*hcstate->ReturnPage)(hcstate->Num, TIMEOUT, NULL, 0);
    }
 
    return(ERR_OK);
}
 
// lwip calls this function when the remote host has successfully received data (ack)
static err_t hc_sent(void *arg, struct tcp_pcb *pcb, u16_t len)
{
    struct hc_state *hcstate = arg;
 
    // Reset connection timeout
    hcstate->ConnectionTimeout = 0;
 
    return(ERR_OK);
}
 
// lwip calls this function when the connection is established
static err_t hc_connected(void *arg, struct tcp_pcb *pcb, err_t err)
{
    struct hc_state *hcstate = arg;
    char  * headers;
 
    // error?
    if(err != ERR_OK)
    {
        hc_clearpcb(pcb);
 
        // Call return function
        (*hcstate->ReturnPage)(hcstate->Num, GEN_ERROR, NULL, 0);
 
        // Free wc hcstate
        free(hcstate->RecvData);
        free(hcstate);
 
        return(ERR_OK);
    }
 
    // Define Headers
    if(hcstate->PostVars == NULL)
    {	
        // GET headers (without page)(+ \0) = 19
        headers = malloc(19 + strlen(hcstate->Page));
        sprintf(headers,"GET /%s HTTP/1.0\r\n\r\n", hcstate->Page);
    }
    else
    {
        // POST headers (without PostVars or Page)(+ \0) = 91
        // Content-length: %d <== 						   ??? (max 10)
        headers = malloc(91 + strlen(hcstate->PostVars) + strlen(hcstate->Page) + 10);
        sprintf(headers, "POST /%s HTTP/1.0\r\nContent-type: application/x-www-form-urlencoded\r\nContent-length: %d\r\n\r\n%s\r\n\r\n", hcstate->Page, strlen(hcstate->PostVars), hcstate->PostVars);
    }
 
    // Check if we are nut running out of memory
    if(headers == NULL)
    {
        hc_clearpcb(pcb);
 
        // Call return function
        (*hcstate->ReturnPage)(hcstate->Num, OUT_MEM, NULL, 0);
 
        // Free wc hcstate
        free(hcstate->RecvData);
        free(hcstate);
 
        return(ERR_OK);
    }
 

 
    // Send data
    tcp_write(pcb, headers, strlen(headers), 1);
    tcp_output(pcb);
 
    // remove headers
    free(headers);
    free(hcstate->PostVars);			// postvars are send, so we don't need them anymore
    free(hcstate->Page);		    	        // page is requested, so we don't need it anymore
 
    return(ERR_OK);
}
 
 
// Public function for request a webpage (REMOTEIP, ...
int hc_open(struct ip_addr remoteIP, char *Page, char *PostVars, void (* returnpage)(u8_t, hc_errormsg, char *, u16_t))
{
	char  * headers;
	struct tcp_pcb *pcb = NULL;
	struct hc_state *hcstate;
	static u8_t num = 0;
	// local port
	u16_t port= 61000; 	
 
	// Get a place for a new webclient hcstate in the memory
	hcstate = malloc(sizeof(struct hc_state));
 
	// Create a new PCB (PROTOCOL CONTROL BLOCK)
	pcb = tcp_new();
	if(pcb == NULL || hcstate == NULL)
	{
		//UARTprintf("hc_open: Not enough memory for pcb or hcstate\n");	
		//Not enough memory
		return 0;
	}
 
	// Define webclient hcstate vars
	num++;
	hcstate->Num = num;
	hcstate->RecvData = NULL;
	hcstate->ConnectionTimeout = 0;
	hcstate->Len = 0;
	hcstate->ReturnPage = returnpage;
 
	// Make place for PostVars & Page
	if(PostVars != NULL) hcstate->PostVars = malloc(strlen(PostVars) +1);
	hcstate->Page = malloc(strlen(Page) +1);
 
	// Check for "out of memory"
	
	if(hcstate->Page == NULL || (hcstate->PostVars == NULL && PostVars != NULL))
	{
		free(hcstate->Page);
		free(hcstate->PostVars);
		free(hcstate);
		tcp_close(pcb);
		return 0;
	}
	
	// Place allocated copy data 
	strcpy(hcstate->Page, Page);
	if(PostVars != NULL) strcpy(hcstate->PostVars, PostVars);
 
	// Bind to local IP & local port
	/*
	while(tcp_bind(pcb, IP_ADDR_ANY, port) != ERR_OK)
	{
		// Local port in use, use port+1
		port++;
	}
	

 	if(tcp_bind(pcb, IP_ADDR_ANY, 0) != ERR_OK)
	{
		// Local port in use, use port+1
		free(hcstate->Page);
		free(hcstate->PostVars);
		free(hcstate);
		//tcp_close(pcb);
		return 0;
	}*/
	tcp_bind(pcb, IP_ADDR_ANY, 0);
	// Use conn -> argument(s)
	tcp_arg(pcb, hcstate);
	
	 
    // Setup the TCP error function
    tcp_err(pcb, hc_error);
	
	    // Setup the TCP receive function
    tcp_recv(pcb, hc_recv);
 
    // Setup the TCP polling function/interval	 //TCP_POLL IS NOT CORRECT DEFINED @ DOC!!!
    tcp_poll(pcb, hc_poll, 10);					 	
 
    // Setup the TCP sent callback function
    tcp_sent(pcb, hc_sent);
 
	// Open connect (SEND SYN) 
	tcp_connect(pcb, &remoteIP, 80, hc_connected);
	/*tcp_connect(pcb, &remoteIP, 80,NULL);
	 headers = malloc(19 + strlen(hcstate->Page));
     sprintf(headers,"GET /%s HTTP/1.0\r\n\r\n", hcstate->Page);
	 
	  // Send data
    tcp_write(pcb, headers, strlen(headers), 1);
    tcp_output(pcb);
	*/
	return num;
}
