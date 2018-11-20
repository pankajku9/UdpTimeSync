
#ifndef SOCKET_COMMON
#define SOCKET_COMMON

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>
#include <math.h>

/* Error Codes */
typedef int err_code;

#define ERR_CODE_SUCCESS            0
#define ERR_CODE_FAILURE            -1
#define ERR_CODE_INSTANCE_INVALID   -2

//#define DEBUG

/** DEBUG and PRINT MACRO for extendablity*/
#ifdef DEBUG
#define DEBUG_PRINT(fmt, ...)    fprintf(stderr, fmt, ##__VA_ARGS__)
#else
#define DEBUG_PRINT(fmt, args...)
#endif

#define ERROR_PRINT(fmt,  ...)    fprintf(stderr, fmt, ##__VA_ARGS__ )

/** Boiler Plate Macros of NULL CHECK RETURN for code reuse and Reablity*/
#define RETURN_IF_NULL(arg, msg)   do{ if((arg) == NULL) { \
                ERROR_PRINT("%s: Caught null pointer for parameter %s\r\n", \
                __func__, (msg));\
                return ERR_CODE_FAILURE;\
                }} while(0)

#define RETURN_ON_FAILURE(arg, msg)   do{ if((arg) < ERR_CODE_SUCCESS) { \
                ERROR_PRINT(" In: %s Code: %d Message: %s \r\n", \
                __func__, arg, (msg));\
                return ERR_CODE_FAILURE;\
                }} while(0)

/* UDP Connection  */

#define SERVER_PORT 1234
#define CLIENT_PORT 5678
#define SERVER_IP "172.23.37.53"
#define BUFFER_LEN 512

err_code setup_socket(int *sockfd);
err_code setup_sockaddr_in(struct sockaddr_in *addr, uint16_t port, char *addr_string);
err_code bind_socket(int sockfd, struct sockaddr_in *addr);
err_code send_data_raw(int sockfd, char* buffer, unsigned int buffer_length, 
	struct sockaddr_in *addr, const socklen_t slen);

err_code recv_data_raw(int sockfd, char* buffer, int *recv_len, unsigned int buffer_length,
	struct sockaddr_in *addr, socklen_t slen);

err_code get_time_from_str(char *str1, struct timespec* ts);
err_code timespec_diff(struct timespec *first, struct timespec *second, struct timespec *res);
err_code timespec_divide(struct timespec *input, int d, struct timespec *res);

#endif // !SOCKET_COMMON