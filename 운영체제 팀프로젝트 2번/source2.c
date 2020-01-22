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

int p_num, r_num; // 프로세스 수와 자원의 수 
int *visited;
int cycle[MAX_SIZE];
int count = 0;
int root[MAX_SIZE];
int parent[MAX_SIZE];
int cycleCheck = 0;

void request_edge(node *, node *);
void allocation_edge(node *, node *);
void dfs(int, int);
int findRoot(int);
void doUnion(int, int);
void execute(FILE *fp);

int main() {
	FILE *fp;
	printf("CASE 1 : Cycle ---------------------------------------\n");
	if ((fp = fopen("input1.txt", "r")) == NULL) { // Cycle
		printf("File not found");
		exit(0);
	}
	execute(fp);

	printf("\n\nCASE 2 : No Cycle ---------------------------------\n");
	if ((fp = fopen("input2.txt", "r")) == NULL) { // No Cycle
		printf("File not found");
		exit(0);
	}
	execute(fp);
}

void execute(FILE *fp) {
	
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
	int x, y;

	for (int i = 1; i <= p_num; i++) // 초기화
		parent[i] = -1;

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

			x = findRoot(i);
			y = findRoot(allocation_r[search->num]->num);

			if (x != y)
				doUnion(x, y);
		}
		printf("\n");
	}
	printf("\n");


	printf("Lists of the cycles founded\n");
	visited = (int*)malloc(sizeof(int)*(p_num + 1));
	for (int i = 1; i <= p_num; i++)
		visited[i] = FALSE;

	int p;
	int j = 1;
	int variation = 0;
	for (int i = 1; i <= p_num; i++) {
		if (parent[i] == -1) {
			p = i;
			dfs(p, p);
			if (cycleCheck != 1)
				p = p % p_num;
			while (cycleCheck != 1 && j != p_num + 1) {
				for (j = 1; j <= p_num; j++) {
					if (parent[j] == i && cycleCheck != 1) {
						++p;

						dfs(p, p);
					}
				}
			}
			if (variation < cycleCheck)
				variation = cycleCheck;
			j = 1;
			cycleCheck = 0;
			if (variation == 0)
				printf("No cycle\n\n");
		}
	}
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
			cycleCheck = 1;
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

void doUnion(int x, int y)
{
	int xRoot = findRoot(x);
	int yRoot = findRoot(y);
	parent[yRoot] = xRoot;

	for (int i = 1; i <= p_num; i++) {
		if (parent[i] == yRoot)
			parent[i] = xRoot;
	}
}

int findRoot(int i)
{
	if (parent[i] == -1)
		return i;
	return findRoot(parent[i]);
}
