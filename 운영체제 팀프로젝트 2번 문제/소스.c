#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#define FALSE 0
#define TRUE 1
#define MAX_SIZE 20

typedef struct item* itemPointer;
typedef struct item {
	int num;
	itemPointer next;
}item;

typedef struct node {
	char type;
	item info;
}node;

itemPointer *request_p; // 프로세스가 요청한 자원들을 저장하는 연결리스트
itemPointer *allocation_r; // 리소스가 할당된 프로세스를 대한 정보 저장하는 연결리스트
itemPointer *wait_for;

int *cycle_p;
int count = 0;
int *visited;
int p_num, r_num; // 프로세스 수와 자원의 수 
int cycle[MAX_SIZE];

void request_edge(node *, node *);
void allocation_edge(node *, node *);
void dfs(int, int);

int main() {

	FILE *fp;
	int p_num, r_num; // 프로세스 수와 자원의 수

	if ((fp = fopen("input.txt", "r")) == NULL) {
		printf("File nOT found");
		exit(0);
	}

	fscanf(fp, "%d %d\n", &p_num, &r_num);

	request_p = (itemPointer *)malloc(sizeof(itemPointer) * (p_num + 1));
	for (int i = 1; i <= p_num; i++)
		request_p[i] = NULL;
	allocation_r = (itemPointer *)malloc(sizeof(itemPointer) * (r_num + 1));
	for (int i = 1; i <= r_num; i++)
		allocation_r[i] = NULL;

	while (!feof(fp)) {
		node nA, nB;

		fscanf(fp, "%c%d ", &nA.type, &nA.info.num);
		nA.info.next = NULL;

		fscanf(fp, "%c%d\n", &nB.type, &nB.info.num);
		nB.info.next = NULL;

		if (nA.type == 'R') {// 할당간선이면
			allocation_edge(&nA, &nB);
		}
		else { // 요청간선이면
			request_edge(&nA, &nB);
		}
	}

	printf("Adjacency list of RAG\n");
	itemPointer search;
	for (int i = 1; i <= p_num; i++) {
		printf("[P%d] : ", i);
		for (search = request_p[i]; search; search = search->next)
			printf("R%d ", search->num);
		printf("\n");
	}

	for (int i = 1; i <= r_num; i++) {
		printf("[R%d] : ", i);
		for (search = allocation_r[i]; search; search = search->next)
			printf("P%d ", search->num);
		printf("\n");
	}
	printf("\n");

	wait_for = (itemPointer*)malloc(sizeof(itemPointer) * (p_num + 1));
	for (int i = 1; i <= p_num; i++)
		wait_for[i] = NULL;
	
	item *temp;
	printf("Adjacency list of wait-for graph\n");
	for (int i = 1; i <= p_num; i++) {
		printf("[P%d] : ", i);
		for (search = request_p[i]; search; search = search->next) {
			printf("P%d ", allocation_r[search->num]->num);

			//wait_for graph 생성하기
			temp = (item*)malloc(sizeof(item));
			temp->num = allocation_r[search->num]->num;
			temp->next = NULL;

			if (wait_for[i] == NULL)
				wait_for[i] = temp;
			else {
				temp->next = wait_for[i];
				wait_for[i] = temp;
			}
		}
		printf("\n");
	}
	printf("\n");

	printf("Lists of the cycles founded\n");
	visited = (int*)malloc(sizeof(int)*(p_num + 1));
	for (int i = 1; i <= p_num; i++)
		visited[i] = FALSE;
	
	dfs(1,1);

	return 0;
}

void allocation_edge(node *a, node *b) {
	item *e;

	e = (item *)malloc(sizeof(item));
	e->num = b->info.num;
	e->next = NULL;
	allocation_r[a->info.num] = e;

}

void request_edge(node *a, node *b) {
	item * e;

	e = (item *)malloc(sizeof(item));
	e->num = b->info.num;
	e->next = NULL;

	if (request_p[a->info.num] == NULL)
		request_p[a->info.num] = e;
	else {
		e->next = request_p[a->info.num];
		request_p[a->info.num] = e;
	}
}

void dfs(int start, int node) {
	if (visited[node]) {
		if (node == start) {
			for (int i = 0; i < count; i++) {
				printf("%d ", cycle[i]);
			}
		}
		printf("\n");
		return;
	}
	visited[node] = TRUE;
	cycle[count++] = node;
	for (item *search = wait_for[node]; search; search = search->next)
		dfs(start, search->num);

	visited[node] = FALSE;
	count--;
}
