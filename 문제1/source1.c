#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUMBER_OF_CUSTOMERS 5
#define NUMBER_OF_RESOURCES 3
#define LIMIT_OF_NEEDS 2
#define TRUE 1
#define FALSE 0

int need[NUMBER_OF_CUSTOMERS + 1][NUMBER_OF_RESOURCES + 1];
// 몇번 프로세스(첫번째 인덱스)가 몇번째 리소스(두번째 인덱스)를 몇 개(value)를 요구하는가? 
int allocation[NUMBER_OF_CUSTOMERS + 1][NUMBER_OF_RESOURCES + 1];
// 몇번 프로세스(첫번째 인덱스)에 몇번째 리소스(두번째 인덱스)가 몇 개(value)를 할당되었는가?
int max[NUMBER_OF_CUSTOMERS + 1][NUMBER_OF_RESOURCES + 1];
// 몇번 프로세스(첫번째 인덱스)가 프로세스 완전 종료를 위해 몇번째 리소스(두번째 인덱스)가 최대 몇 개(value)가 필요한가?
int available[NUMBER_OF_RESOURCES + 1];
// 자원 타입별 가용 가능한 자원의 개수

int main() {

	FILE *fp, *fp2;
	
	if ((fp = fopen("resources_max.txt", "r")) == NULL) {
		printf("File not found");
		exit(0);
	}

	if ((fp2 = fopen("resources_alloc.txt", "r")) == NULL) {
		printf("File not found");
		exit(0);
	}

	for (int i = 1; i <= NUMBER_OF_RESOURCES; i++)
		fscanf(fp2, "%d", &available[i]);

	for (int i = 1; i <= NUMBER_OF_CUSTOMERS; i++) {
		for (int j = 1; j <= NUMBER_OF_RESOURCES; j++) {
			fscanf(fp, "%d ", &max[i][j]);
			fscanf(fp2, "%d", &allocation[i][j]);
			available[j] -= allocation[i][j];
			need[i][j] = max[i][j] - allocation[i][j];
		}
	}
	
	printf("------------------------------------------\n");
	printf("      Alloc        Max        Need\n");
	for (int i = 1; i <= NUMBER_OF_CUSTOMERS; i++) {
		printf("P%d   ", i);
		for (int j = 1; j <= NUMBER_OF_RESOURCES; j++)
			printf("%2d ", allocation[i][j]);
		printf("   ");
		for (int j = 1; j <= NUMBER_OF_RESOURCES; j++)
			printf("%2d ", max[i][j]);
		printf("   ");
		for (int j = 1; j <= NUMBER_OF_RESOURCES; j++)
			printf("%2d ", need[i][j]);
		printf("\n");
	}

	int *work;
	work = (int*)malloc(sizeof(int)*(NUMBER_OF_RESOURCES + 1));
	
	printf("\nAvailable : ( ");
	for (int i = 1; i <= NUMBER_OF_RESOURCES; i++)
		printf("%d ", available[i]);
	printf(")\n");

	//자원 요청 알고리즘
	int request_c, *request_r;
	srand(time(NULL));
	request_c = rand() % NUMBER_OF_CUSTOMERS + 1;
	request_r = (int *)malloc(sizeof(int)*(NUMBER_OF_RESOURCES + 1));

	/*for (int i = 1; i <= NUMBER_OF_RESOURCES; i++) {
		request_r[i] = rand() % LIMIT_OF_NEEDS + 1;
	}*/
	request_c = 1;
	request_r[1] = 0;
	request_r[2] = 2;
	request_r[3] = 0;

	printf("\n-----------------자원요청-----------------\n");
	printf("P%d's request : ", request_c);
	printf("( ");
	for (int i = 1; i <= NUMBER_OF_RESOURCES; i++)
		printf("%d ", request_r[i]);
	printf(")\n");

	int request_accept = TRUE;
	for (int i = 1; i <= NUMBER_OF_RESOURCES; i++) {
		if (need[request_c][i] < request_r[i]) {
			printf("Resource-Request Error!!!\n");
			request_accept = FALSE;
			break;
		}
	}

	if (request_accept == TRUE) {
		for (int i = 1; i <= NUMBER_OF_RESOURCES; i++) {
			if (available[i] < request_r[i]) {
				request_accept = FALSE;
			}
		}
		if (request_accept == TRUE) {
			printf("Resoure-request accepted\n");
			for (int i = 1; i <= NUMBER_OF_RESOURCES; i++) {
				available[i] -= request_r[i];
				need[request_c][i] -= request_r[i];
				allocation[request_c][i] += request_r[i];
			}
		}
		else
			printf("Resoure-request not accepted\n");
	}

	printf("Work : ( ");
	for (int i = 1; i <= NUMBER_OF_RESOURCES; i++) {
		work[i] = available[i];
		printf("%d ", work[i]);
	}
	printf(")\n");

	printf("      Alloc        Max        Need\n");
	for (int i = 1; i <= NUMBER_OF_CUSTOMERS; i++) {
		printf("P%d   ", i);
		for (int j = 1; j <= NUMBER_OF_RESOURCES; j++)
			printf("%2d ", allocation[i][j]);
		printf("   ");
		for (int j = 1; j <= NUMBER_OF_RESOURCES; j++)
			printf("%2d ", max[i][j]);
		printf("   ");
		for (int j = 1; j <= NUMBER_OF_RESOURCES; j++)
			printf("%2d ", need[i][j]);
		printf("\n");
	}

	printf("\n----------------안전성 검증----------------\n");
	//안전성 알고리즘
	int *finish;

	finish = (int*)malloc(sizeof(int)*(NUMBER_OF_CUSTOMERS + 1));
	for (int i = 1; i <= NUMBER_OF_CUSTOMERS; i++)
		finish[i] = FALSE;
	//

	int check;
	int finished_thread = 0;
	int pre_finished = 0;
	int step = 0;

	do {
		printf("STEP %d------------------------\n", ++step);
		for (int i = 1; i <= NUMBER_OF_CUSTOMERS; i++) {

			if (finish[i] == TRUE) // finishe가 true이면 다음 스레드에 대한 자원반납을 시행
				continue;

			check = TRUE;
			for (int j = 1; j <= NUMBER_OF_RESOURCES; j++) { // 자원필요량이 현재 가용량보다 큰 경우... break를 통해서 반복문을 빠져나오고
				if (need[i][j] > work[j]) {
					check = FALSE;
					break;
				}
			}
			if (check == FALSE) // 해당 스레드가 현재 상황에서 작업을 종료하지 못하면 대기하도록 한다.
				continue;

			// 현재 가용량으로 자원 필요량을 충족할 수 있다면... 자원필요량만큼 제공해주고 스레드가 작업수행을 끝내게 되므로
			// 스레드에 현재 할당된 자원량을 가용량에 추가해준다.
			// 그리고 해당 스레드는 finish를 true로 바꿔주고 완료된 스레드를 1 증가시켜준다.

			printf("P%d 종료... Work: ( ", i);
			for (int j = 1; j <= NUMBER_OF_RESOURCES; j++) {
				work[j] += allocation[i][j];
				printf("%d ", work[j]);
			}
			printf(")\n");
			finished_thread++;
			finish[i] = TRUE;
		}
		printf("\n");
	} while (finished_thread != NUMBER_OF_CUSTOMERS && finished_thread != pre_finished);

	printf("\n----------Result of Safety Algorithm----------\n");
	if (finished_thread == NUMBER_OF_CUSTOMERS) {
		printf("SAFE\n");
	}
	else {
		printf("Not safe\n");
	}
}
