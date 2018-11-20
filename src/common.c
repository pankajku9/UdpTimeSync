#include "common.h"


/* */

static err_code is_valid_ipaddr(char *ipaddr_str)
{
	struct sockaddr_in sa;
	int result = inet_pton(AF_INET, ipaddr_str, &(sa.sin_addr));
	return result != 0;
}

err_code setup_socket(int *sockfd)
{
	
	RETURN_IF_NULL(sockfd, "sockfd");

	int fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	RETURN_ON_FAILURE(fd, "socket setup failed");

	*sockfd = fd;
	return ERR_CODE_SUCCESS;
}



err_code setup_sockaddr_in(struct sockaddr_in *addr, uint16_t port, char *addr_string)
{
	RETURN_IF_NULL(addr, "addr");
	int ret;

	memset((char *)addr, 0, sizeof(struct sockaddr_in));

	addr->sin_family = AF_INET;

	if (port >= 0) 
		addr->sin_port = htons(port);
	else
		return ERR_CODE_FAILURE;

	if (addr_string == NULL) 
		addr->sin_addr.s_addr = htonl(INADDR_ANY);
	else 
	{
		ret = inet_aton(addr_string, &addr->sin_addr);
		RETURN_ON_FAILURE(ret, "inet_aton failed");
		
	}
	return ERR_CODE_SUCCESS;
}

err_code bind_socket(int sockfd, struct sockaddr_in *addr)
{

	RETURN_IF_NULL(addr, "addr");
	int ret;

	if (sockfd <= 0)
		return ERR_CODE_FAILURE;

	ret = bind(sockfd, (struct sockaddr *)addr, sizeof(struct sockaddr));
	RETURN_ON_FAILURE(ret, "bind socket failed");
	
	return ERR_CODE_SUCCESS;
}

err_code send_data_raw(int sockfd, char* buffer, unsigned int buffer_length, 
	struct sockaddr_in *addr, const socklen_t slen)
{
	RETURN_IF_NULL(addr, "addr");
	RETURN_IF_NULL(buffer, "buffer");
	int ret;

	if (sockfd <= 0)
		return ERR_CODE_FAILURE;

	ret = sendto(sockfd, buffer, buffer_length, 0, (struct sockaddr *)addr, slen);
	RETURN_ON_FAILURE(ret, "sendto failed");
	
	return ERR_CODE_SUCCESS;
}

err_code recv_data_raw(int sockfd, char* buffer, int *recv_len, unsigned int buffer_length,
		  struct sockaddr_in *addr, socklen_t slen)
{
	RETURN_IF_NULL(addr, "addr");
	RETURN_IF_NULL(buffer, "buffer");
	int ret;

	memset(buffer, 0, buffer_length);

	ret = recvfrom(sockfd, buffer, buffer_length, 0, (struct sockaddr *)addr, &slen);
	RETURN_ON_FAILURE(ret, "recvfrom failed");
	
	*recv_len = ret;
	return ERR_CODE_SUCCESS;
}

err_code get_time_from_str(char *str1, struct timespec* ts)
{

	RETURN_IF_NULL(str1, "str1");
	RETURN_IF_NULL(ts, "ts");

	char str[BUFFER_LEN];
	char *token;

	strcpy(str, str1);

	token = strtok(str, " ");
	ts->tv_sec = (time_t)atoll(token) ;

	token = strtok(NULL, " ");
	ts->tv_nsec = atol(token);

	return ERR_CODE_SUCCESS;

}

err_code timespec_diff(struct timespec *first, struct timespec *second, struct timespec *res)
{
	
	RETURN_IF_NULL(first, "first");
	RETURN_IF_NULL(second, "second");

	struct timespec *ts1, *ts2;
	
	// swap to avoid negative diff
	if (first->tv_sec > second->tv_sec)
	{
		ts2 = first;
		ts1 = second;
	}
	else
	{
		ts2 = second;
		ts1 = first;
	}

	if ((ts2->tv_nsec - ts1->tv_nsec) < 0) {
		res->tv_nsec = ts2->tv_nsec - ts1->tv_nsec + 1000000000;
		res->tv_sec = ts2->tv_sec - ts1->tv_sec - 1;
	}
	else {
		res->tv_sec = ts2->tv_sec - ts1->tv_sec;
		res->tv_nsec = ts2->tv_nsec - ts1->tv_nsec;
	}

	return ERR_CODE_SUCCESS;
}

err_code timespec_divide(struct timespec *input, int d, struct timespec *res)
{

	RETURN_IF_NULL(input, "input");
	RETURN_IF_NULL(res, "res");

	res->tv_sec = input->tv_sec / d;
	res->tv_nsec = ((input->tv_sec % d * 1000000000) / d) + input->tv_nsec / d;

	return ERR_CODE_SUCCESS;
}


