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
// ��� ���μ���(ù��° �ε���)�� ���° ���ҽ�(�ι�° �ε���)�� �� ��(value)�� �䱸�ϴ°�? 
int allocation[NUMBER_OF_CUSTOMERS + 1][NUMBER_OF_RESOURCES + 1];
// ��� ���μ���(ù��° �ε���)�� ���° ���ҽ�(�ι�° �ε���)�� �� ��(value)�� �Ҵ�Ǿ��°�?
int max[NUMBER_OF_CUSTOMERS + 1][NUMBER_OF_RESOURCES + 1];
// ��� ���μ���(ù��° �ε���)�� ���μ��� ���� ���Ḧ ���� ���° ���ҽ�(�ι�° �ε���)�� �ִ� �� ��(value)�� �ʿ��Ѱ�?
int available[NUMBER_OF_RESOURCES + 1];
// �ڿ� Ÿ�Ժ� ���� ������ �ڿ��� ����

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

	//�ڿ� ��û �˰���
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

	printf("\n-----------------�ڿ���û-----------------\n");
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

	printf("\n----------------������ ����----------------\n");
	//������ �˰���
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

			if (finish[i] == TRUE) // finishe�� true�̸� ���� �����忡 ���� �ڿ��ݳ��� ����
				continue;

			check = TRUE;
			for (int j = 1; j <= NUMBER_OF_RESOURCES; j++) { // �ڿ��ʿ䷮�� ���� ���뷮���� ū ���... break�� ���ؼ� �ݺ����� ����������
				if (need[i][j] > work[j]) {
					check = FALSE;
					break;
				}
			}
			if (check == FALSE) // �ش� �����尡 ���� ��Ȳ���� �۾��� �������� ���ϸ� ����ϵ��� �Ѵ�.
				continue;

			// ���� ���뷮���� �ڿ� �ʿ䷮�� ������ �� �ִٸ�... �ڿ��ʿ䷮��ŭ �������ְ� �����尡 �۾������� ������ �ǹǷ�
			// �����忡 ���� �Ҵ�� �ڿ����� ���뷮�� �߰����ش�.
			// �׸��� �ش� ������� finish�� true�� �ٲ��ְ� �Ϸ�� �����带 1 ���������ش�.

			printf("P%d ����... Work: ( ", i);
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
