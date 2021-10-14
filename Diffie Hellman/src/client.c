#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <limits.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <math.h>
#include <ctype.h>

#define SERV_PORT 9999
#define MAX_LEN 1024
#define MAXSIZE 1000000
#define CAESAR_NUM 41
#define M_ITER 15

typedef struct GlobalInfo {
	int prime;
	int generator;
} GlobalInfo;

/* tính (a ^ b) mod p */
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

/* Kiểm tra tính nguyên tố của các số được tạo ngẫu nhiên 
 * bằng cách sử dụng Miller-Rabin 
*/
int MillerRabinTest(int value, int iteration) {
	if (value < 2)
		return 0;
	int q = value - 1, k = 0;
	while (!(q % 2)) {
		q /= 2;
		k++;
	}
	for (int i = 0; i < iteration; i++) {
		int a = rand() % (value - 1) + 1;
		int current = q;
		int flag = 1;
		int mod_result = power(a, current, value);
		for (int i = 1; i <= k; i++) {
			if (mod_result == 1 || mod_result == value - 1) {
				flag = 0;
				break;
			}
			mod_result = (int)((long long)mod_result * mod_result % value);
		}
		if (flag)
			return 0;
	}
	return 1;
}

/* Tạo một số nguyên tố p sẽ được chia sẻ 
 * công khai giữa client và server
 */
int CreatePrime() {
	srand(time(NULL));
	while(1) {
		int current_value = rand() % INT_MAX;
		if (!(current_value % 2))
			current_value++;
		if (MillerRabinTest(current_value, M_ITER) == 1)
			return current_value;
	}
}

/* Tạo gốc nguyên thủy a bằng cách kiểm tra các số ngẫu nhiên */
int CreatePrimeA(int p) {
	int sieve[MAXSIZE];
	memset(sieve, 0, sizeof(sieve));
	sieve[0] = sieve[1] = 1;
	for (int i = 4; i < MAXSIZE; i += 2)
		sieve[i] = 1;
	for (int i = 3; i < MAXSIZE; i += 2) {
		if (!sieve[i]) {
			for (int j = 2 * i; j < MAXSIZE; j += i)
				sieve[j] = 1;
		}
	}
	while (1) {
		int a = rand() % (p - 2) + 2;
		int phi = p - 1, flag = 1, root = sqrt(phi);
		for (int i = 2; i <= root; i++) {
			if (!sieve[i] && !(phi % i)) {
				int mod_result = power(a, phi / i, p);
				if (mod_result == 1) {
					flag = 0;
					break;
				}
				if (MillerRabinTest(phi / i, M_ITER) && !(phi % (phi / i))) {
					int mod_result = power(a, phi / (phi / i), p);
					if (mod_result == 1) {
						flag = 0;
						break;
					}
				}
			}
		}
		if (flag) 
			return a;
	}
}

/* Chuyển đổi ký tự thành ký tự được mã hóa 
 * bằng phương pháp Caeser
 */
char caesar_encrypt(char c, int key) {
	char dict[] = {' ','A','B','C','D','E','F','G','H','I','J','K','L','M','N',
					'O','P','Q','R','S','T','U','V','W','X','Y','Z',',','.',
					'?','0','1','2','3','4','5','6','7','8','9','!'};
	for (int i = 0; i < CAESAR_NUM; i++) {
		if (dict[i] == c) 
			return dict[(i + key) % CAESAR_NUM];
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

/* Client */
int main(int argc, char *argv[]) {
	if (argc < 3) {
		printf("Vui lòng nhập [IP_ADDRESS] [FILENAME]\n");
		exit(-1);
	}
	printf("---------------------------------------------------------------\n");
	printf("CLIENT\n");
	printf("---------------------------------------------------------------\n");

	/* Tạo một số nguyên tố và gốc nguyên thủy của nó (được biết đến rộng rãi) */
	GlobalInfo g;	
	g.prime = CreatePrime();
	printf("** Số nguyên tố P - %d\n", g.prime);

	g.generator = CreatePrimeA(g.prime);
	printf("** Số nguyên tố A - %d\n\n", g.generator);

	/* Tạo khóa riêng và khóa công khai của client */
	int private_key = rand() % (g.prime - 1) + 1;
	int public_key = power(g.generator, private_key, g.prime);
	printf("*** Khóa bí mật của Client: %d\n", private_key);
	printf("*** Khóa công khai của Client: %d\n\n", public_key);

	/* Thiết lập kết nối socket với máy chủ */
	int sockfd;
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Error socket");
		exit(-1);
	}
	struct sockaddr_in serv_addr;
	memset(&serv_addr, 0, sizeof(serv_addr));
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(SERV_PORT);
	
	/* Kết nối đến server */
	if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		perror("Error connect");
		exit(-1);
	}

	/* Gửi khóa công khai, số nguyên tố p, a tới server */
	char message[MAX_LEN];
	memset(message, 0, sizeof(message));	 
	int n = sprintf(message, "%d\n%d\n%d\n", public_key, g.prime, g.generator);
	send_message(sockfd, message, n);

	/* Nhận khóa công khai từ server */ 
	n = recv_message(sockfd, message, sizeof(int) + sizeof(char));
	int public_key_server = atoi(message);
	printf("**** Khóa công khai của Server: %d\n\n", public_key_server);

	/* Tính khóa chung và caesar_key */
	int shared_key = power(public_key_server, private_key, g.prime);
	int caesar_key = shared_key % CAESAR_NUM;
	printf("***** Khóa Chung: %d\n", shared_key);
	printf("***** Khóa Caesar: %d\n\n", caesar_key);

	/* Gửi thông điệp tệp đến server sau khi mã hóa */
	FILE *input;
	if ((input = fopen(argv[2], "r")) == NULL) {
		perror("Error File");
		exit(-1);
	}
	printf("****** Gửi thông điệp đến Server ở định dạng được mã hóa...\n");
	while ((n = fread(message, sizeof(char), MAX_LEN, input)) > 0) {
		for (int i = 0; i < n; i++) 
			message[i] = caesar_encrypt(toupper(message[i]), caesar_key);
		send_message(sockfd, message, n);
	} 
	fclose(input);
	printf("****** Hoàn tất quá trình gửi tệp đến Server!\n\n"); 
	printf("---------------------------------------------------------------\n");
	close(sockfd);
	return 0;
}



