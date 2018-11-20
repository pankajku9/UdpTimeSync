
#include "common.h"


int main(int argc, char **argv)
{
	int sockfd = 0;
	int ret;
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	const socklen_t slen = sizeof(struct sockaddr_in);

	char recv_buff[BUFFER_LEN] = { 0 };
	int recv_len = 0;
	char send_buff[BUFFER_LEN] = { 0 };
	struct timespec curr_time, recvd_time, t1;
	
	

	printf("SERVER\n");

	/// Setup socket
	ret = setup_socket(&sockfd);
	RETURN_ON_FAILURE(ret, "setup_socket failed");

	/// Setup server addr struct
	ret = setup_sockaddr_in(&server_addr, SERVER_PORT, SERVER_IP);
	RETURN_ON_FAILURE(ret, "setup_sockaddr_in failed");

	/// Bind SERVER_PORT to address structure on sockfd
	ret = bind_socket(sockfd, &server_addr);
	RETURN_ON_FAILURE(ret, "bind_socket failed");

	
	

	for (;;) {
		
		memset(send_buff, 0, sizeof(struct timespec));
		memset(&curr_time, 0, sizeof(struct timespec));
		
		recv_data_raw(sockfd, recv_buff, &recv_len, BUFFER_LEN, &client_addr, slen);
		DEBUG_PRINT("Received MSG: %s From: %s:%d \n", recv_buff, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

		clock_gettime(CLOCK_REALTIME, &curr_time);
		sprintf(send_buff, "%lld %ld ", (long long)curr_time.tv_sec, curr_time.tv_nsec);
		DEBUG_PRINT("Sending Data: %s\n", send_buff);
		send_data_raw(sockfd, send_buff, BUFFER_LEN, &client_addr, slen);


		recv_data_raw(sockfd, recv_buff, &recv_len, BUFFER_LEN, &client_addr, slen);
		DEBUG_PRINT("Received MSG: %s From: %s:%d \n", recv_buff, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

		clock_gettime(CLOCK_REALTIME, &curr_time);
		get_time_from_str(recv_buff, &recvd_time);
		timespec_diff(&recvd_time, &curr_time, &t1);
		sprintf(send_buff, "%lld %ld ", (long long)t1.tv_sec, t1.tv_nsec);

		DEBUG_PRINT("Sending Data: %s\n", send_buff);
		send_data_raw(sockfd, send_buff, BUFFER_LEN, &client_addr, slen);

	}

	return 0;
}
