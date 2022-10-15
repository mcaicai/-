#ifndef __MAKEU_NET_H__
#define __MAKEU_NET_H__

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/select.h>
#include <unistd.h>
#include <sys/time.h>

#define SERV_PORT 5002
#define SERV_IP_ADDR "192.168.1.117"
#define BACKLOG 5
#define SERV_RESP_STR "SERVER:"

#define QUIT_STR "quit"
#endif
