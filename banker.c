#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define PROCESS_NUM 5
#define RESOURCE_NUM 3

//각각 최대 instance 갯수 10,5,7로 일단 지정

int p_allocation[5][3] = { {0,1,0 },{ 3, 0, 2 }, { 3, 0, 2 }, { 2, 1, 1 }, { 0, 0, 2 } };
// 초기 각 프로세스의 최대 요구 자원
int p_max[5][3] = { {7,5,3}, {3,2,2}, {9,0,2},{2,2,2} ,{4,3,3} };
int p_available[3] = { 2,3,0 };  // 사용 가능한 자원의 수
int p_need[5][3], p_work[3], p_finish[5], p_request[5][3];


void print_status();
int resource_request(int processname);

int main()
{
	srand(time(NULL));
	int random;
	int exit_key = 0;
	
	
	int i, j, k, processname = 0, false = 0, true = 1;
	int request_process;

	printf("-----	초기 할당된 상태	-----\n\n");
	for (i = 0; i < PROCESS_NUM; i++) {
		for (j = 0; j < RESOURCE_NUM; j++) {
			p_need[i][j] = p_max[i][j] - p_allocation[i][j];
		}
	}

	print_status();

	do {
		for (i = 0; i < PROCESS_NUM; i++) {
			for (j = 0; j < RESOURCE_NUM; j++) {
				p_need[i][j] = p_max[i][j] - p_allocation[i][j];
			}
		}
	

		// 자원이 요청(resource-request algorithm)

		request_process =resource_request(processname);//request_process는 프로세스의 number
			//P[0]의 A B C request => 0,2,0 은 unsafe

			puts("");
			
			printf("P[%d]의 자원요청\n", request_process);
			//for (i = 0; i < RESOURCE_NUM; i++) {
				if (p_request[request_process][0] <= p_need[request_process][0] && p_request[request_process][1] <= p_need[request_process][1] && p_request[request_process][2] <= p_need[request_process][2]  ) {
				
					printf("\nstep 1 :\nrequest <= Need\n" );
					printf("(%d %d %d) <= (%d %d %d)\n", p_request[request_process][0], p_request[request_process][1], p_request[request_process][2], p_need[request_process][0], p_need[request_process][1], p_need[request_process][2]);
				
					if (p_request[request_process][0] <= p_available[0] && p_request[request_process][1] <= p_available[1] && p_request[request_process][2] <= p_available[2])  {
						printf("\nstep 2 : \nrequest <= available\n" );
						printf("(%d %d %d) <= (%d %d %d)\n", p_request[request_process][0], p_request[request_process][1], p_request[request_process][2], p_available[0], p_available[1], p_available[2]);

					for (int j = 0; j < RESOURCE_NUM; j++) {
						p_available[j] -= p_request[request_process][j];
						p_allocation[request_process][j] += p_request[request_process][j];
						p_need[request_process][j] -= p_request[request_process][j];
					}
					printf("\n");
					print_status();
						printf("\n");
					
					}
					else {
						// 요청한  자원이 활용 가능한 자원보다 많으면 자원이 부족하므로 자원을 더 요구한 프로세스는 대기한다.
						printf("Resurce isn't to be enough. Wait P[%d]...\n\n", processname);
						exit(1);
						
					}
				}
				else {  // 프로세스가 최대 요구값을 초과하기 때문에 오류 상태가 된다.
					printf("System is error state\n\n");
					exit(1);
					
				}
		//	}
			// 자원을 요청한 후 상태

			//printf("Process가 자원을 allocated한 후 상태\n\n");
			//print_status();
		////

		for (i = 0; i < RESOURCE_NUM; i++) {
			p_work[i] = p_available[i];
		}

		for (i = 0; i < PROCESS_NUM; i++) { 
			p_finish[i] = false;
		}

		printf("\n");
		for (i = 0; i < PROCESS_NUM; i++) { 
			for (j = 0; j < PROCESS_NUM; j++) {  
				if (p_finish[j] == false) {  
					if (p_need[j][0] <= p_work[0] && p_need[j][1] <= p_work[1] && p_need[j][2] <= p_work[2]) { 
						for (k = 0; k < 3; k++) {
							
							p_work[k] = p_work[k] + p_allocation[j][k];
						}
						p_finish[j] = true; 
						printf("P[%d] ", j);  // 프로세스의 sequence
					}
				}
			}
		}

		for (i = 0; i < PROCESS_NUM; i++) {
			if (p_finish[i] != true) { 
				printf("unsafe state\n"); 
				for (j = 0; j < RESOURCE_NUM; j++) {  // 이전의 자원 할당 상태로 복원
					p_available[i] += p_request[i][j];
					p_allocation[processname][i] -= p_request[i][j];
					p_need[processname][i] += p_request[i][j];
				}
				exit(1);
			}
		}

		printf("\nsafe state\n");  // 모든 프로세스의 상태가 true이면 안정 상태
		//printf("\nRequest resource 0 or End 1 : ");  
		//scanf("%d", &select);
		printf("계속하시려면 1을 종료하시려면 0을 입력하세요 ");
		scanf("%d", &exit_key);

		if (exit_key == 0)exit(1);
	} while (1);
}



void print_status()
{
	int i, j;
	printf("       Allocation       Max         Available       Need     \n");
	printf("     -------------- ------------ ---------------- --------\n");
	printf("       A  B  C       A  B  C       A  B  C       A  B  C    \n");
	for (i = 0; i < PROCESS_NUM; i++) {
		printf("P[%d]  ", i);
		for (j = 0; j < RESOURCE_NUM; j++) printf(" %d ", p_allocation[i][j]);
		printf("     ");
		for (j = 0; j < RESOURCE_NUM; j++) printf(" %d ", p_max[i][j]);
		printf("     ");
		if (i == 0) for (j = 0; j < RESOURCE_NUM; j++) printf(" %d ", p_available[j]);
		else printf("         ");
		printf("     ");
		for (j = 0; j < RESOURCE_NUM; j++) printf(" %d ", p_need[i][j]);
		printf("\n");
	}
}

int resource_request(int processname)
{
	int i;
	printf("\n자원요청할 process를 0번째부터 4번째까지 중에 선택해주세요 : ");
	//printf("select the number of Process 0 to 4 : ");
	scanf("%d", &processname);
	//random = rand() % 5;
	//processname = random;
	//printf("%d\n", processname);

	//0번째 process에다가 A :0, B:2 ,C : 0 으로 자원 요청하면 unsafe 상태가 됨

	printf("p[%d]에게 자원요청 할 resource 개수\n", processname);
	for (i = 0; i < RESOURCE_NUM; i++) {
		printf("%c : ", ('A' + i));
		//random = rand() % 5;
		scanf("%d", &p_request[processname][i]);
		//p_request[i] = random;
		//printf("%d\n", p_request[i]);

	}
	return processname;
}
