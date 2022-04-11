#include <stdio.h>
#include <stdlib.h>

#define MAX_QUEUE_SIZE 4 // 기호상수 정의 큐사이즈 4

typedef char element; // char을 element로 사용

typedef struct { //구조체 정의
	element queue[MAX_QUEUE_SIZE]; //문자형 배열 선언
	int front, rear; //정수형 변수 선언
}QueueType;


QueueType *createQueue(); //구조체포인터 선언

//함수 선언
int freeQueue(QueueType *cQ); // 초기화 함수
int isEmpty(QueueType *cQ); //큐가 비어있는지 확인한는 함수
int isFull(QueueType *cQ); //큐가 Ful인지 확인하는 함수
void enQueue(QueueType *cQ, element item); //큐에 element을 추가 하는 함수
void deQueue(QueueType *cQ, element* item); //큐에 element를 빼내는 함수
void printQ(QueueType *cQ); //큐를 출력하는 함수
void debugQ(QueueType *cQ); //큐가어떻게 저장되어있는지와 front,rear 를 출력하는 함수 
element getElement(); //값 입력 getElement


int main(void)
{
	QueueType *cQ = createQueue(); //MAX_QUEUE_SIZE인 공백 큐 생성
	element data; //문자형 변수 선언
	
	char command;
	printf("[----- [kim dongmin] [2017038093] -----]\n");
	do{
		
		printf("\n-----------------------------------------------------\n");
		printf("                     Circular Q                   \n");
		printf("------------------------------------------------------\n");
		printf(" Insert=i,  Delete=d,   PrintQ=p,   Debug=b,   Quit=q \n");
		printf("------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command); //커멘드 입력

		switch(command) { //커멘드에 맞는 기능 실행
		case 'i': case 'I':
			data = getElement(); //값입력 받고 data에 저장
			enQueue(cQ, data); // 데이터 저장
			break;
		case 'd': case 'D':
			deQueue(cQ,&data); //큐에 element를 빼냄
			break;
		case 'p': case 'P':
			printQ(cQ); //큐 프린트
			break;
		case 'b': case 'B':
			debugQ(cQ); //디버그 큐에 어떻게 저장 되어있는지 보여줌
			break;
		case 'q': case 'Q':
			break;// 종료
		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q');


	return 1;
}

QueueType *createQueue() //구조체포인터 선언
{
	QueueType *cQ; //포인터 선언
	cQ = (QueueType *)malloc(sizeof(QueueType)); //동적할당 QueueType 크기 만큼
	cQ->front = 0; //front =0 
	cQ->rear = 0; //rear =0
	return cQ; //리턴 cQ
}

int freeQueue(QueueType *cQ) //큐 초기화
{
    if(cQ == NULL) return 1;
    free(cQ); //동적 메모리 해제 하지 않으면 메모리의 누수가 발생한다
    return 1; //1리턴
}

element getElement() //값 입력
{
	element item; //문자형 변수 선언
	printf("Input element = ");
	scanf(" %c", &item); //item에 값 입력 받음
	return item; //item 값 리턴
}


int isEmpty(QueueType *cQ) //큐가 비어있는지 확인한는 함수
{
	if (cQ->front == cQ->rear){ //큐의 공백 조건 front == rear
		printf("Circular Queue is empty!");
		return 1; //리턴 1
	}
	else return 0; //있으면 0 리턴
}

int isFull(QueueType *cQ) //큐가 Full인지 확인하는 함수
{
	if (((cQ->rear+1)%MAX_QUEUE_SIZE) == cQ->front) { // front rear+1같는 지 확인
		printf(" Circular Queue is full!");
		return 1; //1리턴
	}
	else return 0; //아니면 0리턴
}

void enQueue(QueueType *cQ, element item) //큐에 element을 추가 하는 함수
{
	if(isFull(cQ)) return; //큐가 Full인지 확인 
	else {
		cQ->rear = (cQ->rear + 1) % MAX_QUEUE_SIZE; //rear+ 1에 저장
		cQ->queue[cQ->rear] = item;  //큐에 element 저장
	}
}

void deQueue(QueueType *cQ, element *item) //큐에 element를 빼내는 함수 
{
	if(isEmpty(cQ)) return; //비어있는 지 확인
	else {
		cQ->front = (cQ->front + 1)%MAX_QUEUE_SIZE; // front+1 빼냈으니까 뒤로 이동
		*item = cQ->queue[cQ->front]; //빼낸 데이터 item에 저장
		return;
	}
}


void printQ(QueueType *cQ) //큐를 출력하는 함수
{
	int i, first, last; //정수형 변수 선언

	first = (cQ->front + 1)%MAX_QUEUE_SIZE; //first front + 1 원형 큐 이동 
	last = (cQ->rear + 1)%MAX_QUEUE_SIZE; //rear + 1  원형 큐 이동 

	printf("Circular Queue : [");

	i = first; //first 값 i에 대입
	while(i != last){ // first last가 같지 않을때까지 반복 
		printf("%3c", cQ->queue[i]); //값 출력
		i = (i+1)%MAX_QUEUE_SIZE; //first +1  원형 큐 이동 

	}
	printf(" ]\n");
}


void debugQ(QueueType *cQ) //큐가어떻게 저장되어있는지와 front,rear 를 출력하는 함수 
{

	printf("\n---DEBUG\n");
	for(int i = 0; i < MAX_QUEUE_SIZE; i++) // 0부터 MAX_QUEUE_SIZE-1 까지 반복
	{
		if(i == cQ->front) {  //i 값이 front와 같으면 
			printf("  [%d] = front\n", i); //해당 i번째에 front출력
			continue;
		}
		printf("  [%d] = %c\n", i, cQ->queue[i]); //원형 큐에 저장된 순서와 값 출력

	}
	//printQ(cQ);
	printf("front = %d, rear = %d\n", cQ->front, cQ->rear); //front,rear 를 출력
}