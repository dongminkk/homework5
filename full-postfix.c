#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>

#define MAX_STACK_SIZE 10  // 기호상수 정의 MAX_STACK_SIZE = 10
#define MAX_EXPRESSION_SIZE 20 // MAX_EXPRESSION_SIZE = 20

/* stack 내에서 우선순위는 내림차순, lparen = 0 가장 낮음 */
typedef enum{ //열거형
	lparen = 0,  /* ( 왼쪽 괄호 */ //우선 순위가 가장 낮음
	rparen = 9,  /* ) 오른쪽 괄호*/
	times = 7,   /* * 곱셈 */
	divide = 6,  /* / 나눗셈 */
	plus = 5,    /* + 덧셈 */
	minus = 4,   /* - 뺄셈 */
	operand = 1 /* 피연산자 */
} precedence;

char infixExp[MAX_EXPRESSION_SIZE]; // 전역변수 선언 char형 배열 선언 20 infixExp 입력한 식 저장
char postfixExp[MAX_EXPRESSION_SIZE]; //전역변수 선언 char형 배열 선언 20 postfixEx 로 바꾼 식 저장
char postfixStack[MAX_STACK_SIZE]; //전역변수 선언 char형 배열 선언 10  저장 postfix 스텍
int evalStack[MAX_STACK_SIZE]; // 전역변수 선언 int형 배열 선언 10  저장 답 스텍

int postfixStackTop = -1; //전역변수 선언 int 형 초기 : top = -1, 공백 스택
int evalStackTop = -1; //전역변수 선언 int 형 초기 : top = -1, 공백 스택

int evalResult = 0; //전역변수 선언 int 형 수식의 답

//함수 선언
void postfixPush(char x); // Push하는 함수 (삽입) 연산자
char postfixPop(); // Pop하는 함수 (삭제) 연산자
void evalPush(int x); // Push 하는 함수 (삽입) 숫자
int evalPop(); // Pop하는 함수 (삭제) 숫자
void getInfix(); //수식 입력 함수
precedence getToken(char symbol); // 연산자 우선 순위
precedence getPriority(char x); //우선 순위 
void charCat(char* c); //문자하나를 전달받아, postfixExp에 추가
void toPostfix(); //toPostfix로 바꾸는 함수
void debug(); // 출력하는 함수 디버그 
void reset(); //리셋 함수
void evaluation(); //계산 함수

int main()
{
	char command; // 문자형 변수 선언
	printf("[----- [kim dongmin] [2017038093] -----]\n");

	do{
		printf("----------------------------------------------------------------\n");
		printf("               Infix to Postfix, then Evaluation               \n");
		printf("----------------------------------------------------------------\n");
		printf(" Infix=i,   Postfix=p,  Eval=e,   Debug=d,   Reset=r,   Quit=q \n");
		printf("----------------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);

		switch(command) {
		case 'i': case 'I':
			getInfix(); //입력 함수
			break;
		case 'p': case 'P':
			toPostfix(); // Postfix로 바꾸는 함수
			break;
		case 'e': case 'E':
			evaluation(); //계산 함수
			break;
		case 'd': case 'D':
			debug(); // 디버그 함수 출력한는 함수
			break;
		case 'r': case 'R':
			reset(); //초기화 함수
			break;
		case 'q': case 'Q': // 종료
			break;
		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q');

	return 1;


}

void postfixPush(char x) // Push 함수 
{
    postfixStack[++postfixStackTop] = x; //스텍 쌓이고 값 넣음
}

char postfixPop() //Pop 함수 
{
	char x; //문자형 변수 선언
    if(postfixStackTop == -1) // 스텍포인터가 -1이면 스텍에 아무것도 없음
        return '\0'; //-1 이면 '\0' 리턴
    else { //-1이 아니면 
    	x = postfixStack[postfixStackTop--]; // postfixStack[postfixStackTop--]값을 x에 넣음 스텍 포인터 --한다.
    }
    return x; //x값 리턴
}

void evalPush(int x) // Push 함수 (답)
{
    evalStack[++evalStackTop] = x;//스텍 쌓이고 값 넣음
}

int evalPop() //Pop 함수 답
{
    if(evalStackTop == -1) // 스텍포인터가 -1이면 스텍에 아무것도 없음
        return -1; //-1 이면 -1 리턴
    else
        return evalStack[evalStackTop--]; //evalStack [evalStackTop--]리턴 evalStackTop -1
}

/**
 * infix expression을 입력받는다.
 * infixExp에는 입력된 값을 저장한다.
 */

void getInfix() //수식 입력 함수
{
    printf("Type the expression >>> ");
    scanf("%s",infixExp); //수식 입력
}

precedence getToken(char symbol) //연산자 연산순위 숫자리턴
{
	switch(symbol) {
	case '(' : return lparen; //0리턴
	case ')' : return rparen; //9
	case '+' : return plus; //7
	case '-' : return minus; //6
	case '/' : return divide; //5
	case '*' : return times;//4
	default : return operand; //1
	}
}

precedence getPriority(char x) //연산자를 숫자로 
{
	return getToken(x); //리턴 getToken(x)
}


void charCat(char* c) //문자하나를 전달받아, postfixExp에 추가
{
	if (postfixExp == '\0')
		strncpy(postfixExp, c, 1); //복사
	else
		strncat(postfixExp, c, 1); //뒤 쪽에 붙이기
}

/**
 * infixExp의 문자를 하나씩 읽어가면서 stack을 이용하여 postfix로 변경한다.
 * 변경된 postfix는 postFixExp에 저장된다.
 */
void toPostfix() //Postfix로 바꾸는 함수
{
	/* infixExp의 문자 하나씩을 읽기위한 포인터 */
	char *exp = infixExp;
	char x; /* 문자하나를 임시로 저장하기 위한 변수 */

	/* exp를 증가시켜가면서, 문자를 읽고 postfix로 변경 */
	while(*exp != '\0') //반복문 *exp != '\0'때까지
	{
		if(getPriority(*exp) == operand) //피연산자 일ㄸ때
		{
			x = *exp;
        	charCat(&x);  //문자하나를 전달받아, postfixExp에 추가
		}
        else if(getPriority(*exp) == lparen) { //왼쪽 괄호

        	postfixPush(*exp);
        }
        else if(getPriority(*exp) == rparen) //오른쪽 괄호
        {
        	while((x = postfixPop()) != '(') { // ( 없었을때
        		charCat(&x); //문자하나를 전달받아, postfixExp에 추가
        	}
        }
        else
        {
            while(getPriority(postfixStack[postfixStackTop]) >= getPriority(*exp))
            {
            	x = postfixPop(); //Pop 해서 x에 저장
            	charCat(&x); //문자하나를 전달받아, postfixExp에 추가
            }
            postfixPush(*exp); // postfix Push 
        }
        exp++; //exp +1
	}

    while(postfixStackTop != -1) //스텍 포인터가 -1이 아닐 때 
    {
    	x = postfixPop(); //Pop 해서 x에 저장
    	charCat(&x); //문자하나를 전달받아, postfixExp에 추가
    }

}

void debug() //출력함수 
{
	printf("\n---DEBUG\n");
	printf("infixExp =  %s\n", infixExp); //infixExp 출력 입력값
	printf("postExp =  %s\n", postfixExp); //postfixExp로 바꾼거 출력
	printf("eval result = %d\n", evalResult); //evalResult 답 출력

	printf("postfixStack : ");
	for(int i = 0; i < MAX_STACK_SIZE; i++) //MAX_STACK_SIZE 반복
		printf("%c  ", postfixStack[i]); //우선 순위 순서로 쌓은 연산자 출력

	printf("\n");

}

void reset() //초기화
{
	infixExp[0] = '\0'; 
	postfixExp[0] = '\0'; 

	for(int i = 0; i < MAX_STACK_SIZE; i++) //초기화
		postfixStack[i] = '\0'; 
         
	postfixStackTop = -1; // 초기화 -1
	evalStackTop = -1; // 초기화 -1
	evalResult = 0;// 초기화 0
}

void evaluation() //계산 하는 함수
{
	int opr1, opr2, i; //정수형 변수 선언

	int length = strlen(postfixExp); // 문자열 postfixExp 길이를 length에 넣음
	char symbol; // 문자형 변수symbol 선언
	evalStackTop = -1; // 초기화 -1

	for(i = 0; i < length; i++) //length만큼 반복
	{
		symbol = postfixExp[i]; //symbol에 postfixExp(postfix로 바꾼 식)저장
		if(getToken(symbol) == operand) { //조건 symbol의 값 피연산자
			evalPush(symbol - '0');
		}
		else {
			opr2 = evalPop(); //  Pop하여 opr2에 저장
			opr1 = evalPop(); // Pop하여 opr1에 저장
			switch(getToken(symbol)) {
			case plus: evalPush(opr1 + opr2); break; //더하기
			case minus: evalPush(opr1 - opr2); break; //빼기
			case times: evalPush(opr1 * opr2); break; //곱하기
			case divide: evalPush(opr1 / opr2); break; //나누기
			default: break;
			}
		}
	}
	evalResult = evalPop(); //답에 저장
}