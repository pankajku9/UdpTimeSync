#include "common.h"
#include <signal.h>
#include <sys/time.h>
#include <time.h>


#define INTERVAL 1
static volatile int usr_interrupt = 1;

void alarm_wakeup(int i)
{
	struct itimerval tout_val;
	if(i > 0)
		signal(SIGALRM, alarm_wakeup);

	usr_interrupt = 1;

	tout_val.it_interval.tv_sec = 0;
	tout_val.it_interval.tv_usec = 0;
	tout_val.it_value.tv_sec = INTERVAL; /* 10 seconds timer */
	tout_val.it_value.tv_usec = 0;

	setitimer(ITIMER_REAL, &tout_val, 0);
}

void exit_func(int i)
{
	signal(SIGINT, exit_func);
	printf("\nBye Bye!!!\n");
	exit(0);
}


int main(int argc, char **argv)
{
	int sockfd = 0;
	int ret;
	struct sockaddr_in client_addr;
	struct sockaddr_in server_addr;
	const socklen_t slen = sizeof(struct sockaddr_in);

	char recv_buff[BUFFER_LEN] = { 0 };
	int recv_len = 0;
	char send_buff[BUFFER_LEN] = { 0 };
	struct timespec curr_time, recvd_time, t1, t2, t3, t4;

	printf("CLIENT\n");

	/// Setup socket
	ret = setup_socket(&sockfd);
	RETURN_ON_FAILURE(ret, "setup_socket failed");
	
	/// Setup server addr struct
	ret = setup_sockaddr_in(&server_addr, SERVER_PORT, SERVER_IP);
	RETURN_ON_FAILURE(ret, "inet_aton failed");
	
	/// Setup client addr struct
	ret = setup_sockaddr_in(&client_addr, CLIENT_PORT, NULL);
	RETURN_ON_FAILURE(ret, "setup_sockaddr_in failed");

	/// Bind CLIENT_PORT to address structure on sockfd
	ret = bind_socket(sockfd, &client_addr);
	RETURN_ON_FAILURE(ret, "bind_socket failed");

	alarm_wakeup(-1);
	signal(SIGALRM, alarm_wakeup); /* set the Alarm signal capture */
	signal(SIGINT, exit_func);

	int req_count = 0;
	
	for (;;) {

		usr_interrupt = 0;

		sprintf(send_buff, "TIME_REQUEST_%d ", req_count);
		DEBUG_PRINT("Requesting time from Server: %s\n", send_buff);
		send_data_raw(sockfd, send_buff, BUFFER_LEN, &server_addr, slen);

		recv_data_raw(sockfd, recv_buff, &recv_len, BUFFER_LEN, &server_addr, slen);
		clock_gettime(CLOCK_REALTIME, &curr_time);
		get_time_from_str(recv_buff, &recvd_time);
		timespec_diff(&recvd_time, &curr_time, &t1);
		DEBUG_PRINT("Receved Time: %s Diff from CurrTime: %lld  %ld\n", recv_buff,  t1.tv_sec, t1.tv_nsec);


		clock_gettime(CLOCK_REALTIME, &curr_time);
		sprintf(send_buff, "%lld %ld ", (long long)curr_time.tv_sec, curr_time.tv_nsec);
		DEBUG_PRINT("Sending client to Server: %s\n", send_buff);
		send_data_raw(sockfd, send_buff, BUFFER_LEN, &server_addr, slen);


		recv_data_raw(sockfd, recv_buff, &recv_len, BUFFER_LEN, &server_addr, slen);
		DEBUG_PRINT("Received diff at Server: %s\n", recv_buff);
		get_time_from_str(recv_buff, &t2);
		timespec_diff(&t2, &t1, &t3);
		timespec_divide(&t3, 2, &t4);

		time_t t = time(NULL);
		printf("%s Time diff: %lld sec %ld nsec\n", ctime(&t), t4.tv_sec, t4.tv_nsec);

		if (!usr_interrupt)
			pause();

	}


	return 0;
}
