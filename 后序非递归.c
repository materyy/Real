#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct TreeNode
{
    int val;
    struct TreeNode* left;
    struct TreeNode* right;
}TreeNode;


/**栈数据结构引入**/
#define MAXSIZE 10000
#define OVERFLOW 0
#define error -65530

/**栈的数据结构定义**/
typedef struct Sq_stack
{
    struct TreeNode data[MAXSIZE];
    int top;
}Sq_stack;

/**栈的创建--初始化**/
void initStack(Sq_stack *S)
{
    S = (Sq_stack*)malloc(sizeof(Sq_stack));
    if(!S)
        exit(OVERFLOW);//栈空间分配失败
    S->top = 0; //栈顶元素从0开始算起
}

/**插入栈顶元素e**/
bool Push(Sq_stack *S, struct TreeNode* e)
{
    /**插入栈顶元素：判断栈是否已满**/
    if( S->top == MAXSIZE-1 )
        return false;
    S->top++;
    S->data[S->top] = *e;
    return true;
}

/**删除栈顶元素,并用节点承接**/
struct TreeNode* Pop(Sq_stack *S)
{
    /**删除栈顶元素：判断栈是否为空**/
    if(S->top == 0)
        return NULL;
    struct TreeNode* e = (struct TreeNode*)malloc(sizeof(struct TreeNode));
    *e = S->data[S->top];
    S->top--;
    return e;
}

bool isEmptyStack( Sq_stack *S )
{
    return S->top == 0?true:false;
}


int* postorderTraversal(struct TreeNode* root, int* returnSize)
{
    if( root == NULL ) return NULL;

    /**自定义结果数组并初始化**/
    int* result = (int*)malloc(1000*sizeof(int));
    int len = 0;

    /**定义栈并初始化**/
    Sq_stack* stk_result = (Sq_stack*)malloc(sizeof(Sq_stack));
    initStack(stk_result);
    Sq_stack* temp_stk = (Sq_stack*)malloc(sizeof(Sq_stack));
    initStack(temp_stk);

    while( root || !isEmptyStack(temp_stk) )
    {
        /**将当前结点同时入临时栈和结果栈【根】，并遍历右子树----【右】**/
        while( root )
        {
            Push(temp_stk,root);
            Push(stk_result,root);
            root = root->right;
        }
        /**当右子树遍历结束后，弹出临时栈栈顶结点，并遍历其左子树----【左】，继续while**/
        if( !isEmptyStack(temp_stk) )
        {
            root = Pop(temp_stk);
            root = root->left;
        }
    }
    /**
        当所有结点都访问完，即最后访问的树节点为空且临时栈也为空时，
        主算法结束，依次pop出结果栈结点
    **/
    struct TreeNode* e = (struct TreeNode*)malloc(sizeof(struct TreeNode));
    while( !isEmptyStack(stk_result) )
    {
        e = Pop(stk_result);
        result[len++] = e->val;
    }
    free(e);
    *returnSize = len;
    return result;
}

