#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#define SERV_PORT 9999
#define MAX_LEN 1024
#define CAESAR_NUM 41	
#define LISTEN_QUES 5

typedef struct GlobalInfo {
	int prime;
	int generator;
} GlobalInfo;

int power(int a, int b, int p) {
	long long x = 1, y = a;
	while (b > 0) {
		if (b % 2 == 1)
			x = (x * y) % p;
		y = (y * y) % p;
		b /= 2;
	}
	return (int)(x % p);
}

/* Chuyển đổi ký tự được mã hóa thành ký tự rõ */
char caesar_decrypt(char c, int key) {
	char dict[] = {' ','A','B','C','D','E','F','G','H','I','J','K','L','M','N',
					'O','P','Q','R','S','T','U','V','W','X','Y','Z',',','.',
					'?','0','1','2','3','4','5','6','7','8','9','!'};
	for (int i = 0; i < CAESAR_NUM; i++) {
		if (dict[i] == c) 
			return dict[(CAESAR_NUM + i - key) % CAESAR_NUM];
	}
	return c;
}

void send_message(int sockfd, char message[MAX_LEN], int len) {
	int n_sent = 0;
	while (n_sent < len) {
		int temp;
		if ((temp = send(sockfd, message + n_sent, len - n_sent, 0)) <= 0) {
			perror("Error ");
			exit(-1);
		}
		n_sent += temp;
	}
}

int recv_message(int sockfd, char buffer[MAX_LEN], int recv_size) {
	int n_recv = 0;
	while (n_recv < recv_size) {
		int temp;
		if ((temp = recv(sockfd, buffer + n_recv, MAX_LEN - n_recv, 0)) <= 0) {
			if (temp == 0)
				break;
			perror("Error ");
			exit(-1);
		}
		n_recv += temp;
	}
	return n_recv;
}

/* Server */
int main(int argc, char *argv[]) {
	if (argc < 2) {
		printf("Vui lòng nhập [IP_ADDRESS]\n");
		exit(-1);
	}
	printf("----------------------------------------------------------------\n");
	printf("SERVER\n");	
	printf("----------------------------------------------------------------\n");
	printf("Chờ kết nối từ phía Client...\n\n");

	GlobalInfo g;

	/* Thiết lập kết nối với client */
	int server_sockfd, cli_sockfd;
	if ((server_sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Error ");
		exit(-1);
	}
	struct sockaddr_in serv_addr;
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(SERV_PORT);

	if (bind(server_sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		perror("Error ");
		exit(-1);
	}
	listen(server_sockfd, LISTEN_QUES);

	while (1) {
		if ((cli_sockfd = accept(server_sockfd, NULL, NULL)) < 0) {
			perror("Error ");
			exit(-1);
		}
		printf("* Client đã kết nối!...\n\n");
		
		char buffer[MAX_LEN];
		memset(buffer, 0, sizeof(buffer));
		
		/* Nhận khóa công khai, số nguyên tố và gốc nguyên thủy từ client */
		int recv_size = sizeof(int) * 3 + sizeof(char) * 3;
		int n = recv_message(cli_sockfd, buffer, recv_size);
		int public_key_client = atoi(buffer);
		int i = 0;
		while (buffer[i] != '\n') 
			i++;
		g.prime = atoi(buffer + ++i);
		while (buffer[i] != '\n')
			i++;
		g.generator = atoi(buffer + ++i);
		
		printf("** Số nguyên tố P: %d\n", g.prime);
		printf("** Số nguyên tố A: %d\n", g.generator);
		printf("** Khóa công khai của Client: %d\n\n", public_key_client);

		/* Tạo khóa riêng và khóa công khai cho server */
		int private_key = rand() % (g.prime - 1) + 1;
		int public_key = power(g.generator, private_key, g.prime);
		printf("*** Khóa bí mật của Server: %d\n", private_key);
		printf("*** Khóa công khai của Server: %d\n\n", public_key);

		/* Gửi khóa công khai tới client */
		n = sprintf(buffer, "%d\n", public_key);
		send_message(cli_sockfd, buffer, n); 

		/* Tính khóa chung và caesar_key */
		int shared_key = power(public_key_client, private_key, g.prime);
		int caesar_key = shared_key % CAESAR_NUM;
		printf("**** Khóa Chung: %d\n", shared_key);
		printf("**** Khóa Caesar: %d\n\n", caesar_key);

		/* Nhận thông điệp được mã hóa từ client và ghi vào tệp output.txt */
		printf("***** Nhận file từ phía Client...\n\n");
		FILE *output;
		if((output = fopen("msg/output.txt","w")) < 0) {
			perror("Error ");
			exit(-1);
		}
		while ((n = recv_message(cli_sockfd, buffer, MAX_LEN)) > 0) {
			printf("Thông điệp nhận được: \n");
			for (int i = 0; i < n; i++) {
				printf("%c", buffer[i]);
				buffer[i] = caesar_decrypt(buffer[i], caesar_key);
			}
			fwrite(buffer, sizeof(char), n, output);
		}
		fclose(output);
		printf("\n\n***** Hoàn tất quá trình nhận file từ Client!\n\n");
		printf("----------------------------------------------------------------\n");
		close(cli_sockfd);
	}
	return 0;
}

