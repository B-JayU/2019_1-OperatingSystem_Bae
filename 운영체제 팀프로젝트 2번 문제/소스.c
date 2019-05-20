#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

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

int *cycle_p;
int count = 0;

void request_edge(node *, node *);
void allocation_edge(node *, node *);
void foundCycle(int);

int main() {

	FILE *fp;
	int p_num, r_num; // 프로세스 수와 자원의 수

	if ((fp = fopen("input.txt", "r")) == NULL) {
		printf("File NOT found");
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

	printf("Adjacency list of wait-for graph\n");
	for (int i = 1; i <= p_num; i++) {
		printf("[P%d] : ", i);
		for (search = request_p[i]; search; search = search->next) {
			printf("P%d ", allocation_r[search->num]->num);
		}
		printf("\n");
	}
	printf("\n");

	printf("Lists of the cycles founded\n");
	
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

void foundCycle(int n) {
	
}