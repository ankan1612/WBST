/* 
---------------------------------------------------------------------------------------------------------
Solution to the 1st Project of Advanced Data Structures I9600
City College of New York, CUNY 

AUTHOR : ANKAN KUMAR GIRI

Email : agiri000@citymail.cuny.edu / ankangiri@gmail.com
---------------------------------------------------------------------------------------------------------
*/

#include<iostream>
#include<stdlib.h>
#include<stdio.h>
#include<cstring>
#define ALPHA 0.288
#define EPSILON 0.005
using namespace std;

class Text 
{
    public:
        int weight;
        Text *left;
        Text *right;
};

class Stack 
{
    public:
        Text *text_t;
        Stack *next;
};

Stack *create_stack() 
{
    Stack *stack = new Stack();
    stack->next = NULL;
    return stack;
}

void push(Text *&txt, Stack *&stack) 
{
    Stack *tmp_stack = new Stack();
    tmp_stack->text_t = txt;
    tmp_stack->next = stack->next;
    stack->next = tmp_stack;
}

bool stack_empty(Stack *&stack) 
{
    if (stack->next == NULL) 
    {
        return true;
    }
    return false;
}

Text *pop(Stack *&stack) 
{
    Stack *tmp_stack = stack->next;
    Text *txt = tmp_stack->text_t;
    stack->next = tmp_stack->next;
    delete tmp_stack;
    return txt;
}

void remove_stack(Stack *&stack) 
{
    Stack *temp_stack;
    do 
    {
        temp_stack = stack->next;
        delete stack;
        stack = temp_stack;
    } while (temp_stack != NULL);
}

Text *create_text() 
{
    Text *txt = new Text();
    txt->left = NULL;
    txt->right = NULL;
    txt->weight = 0;
    return txt;
}

int length_text(Text *&txt) 
{
    return txt->weight-1;
}

void left_rotation(Text *&txt) 
{
    Text *temp_txt = txt->left;
    txt->left = txt->right;
    txt->right = txt->left->right;
    txt->left->right = txt->left->left;
    txt->left->left = temp_txt;
}

void right_rotation(Text *&txt) 
{
    Text *temp_txt = txt->right;
    txt->right = txt->left;
    txt->left = txt->right->left;
    txt->right->left = txt->right->right;
    txt->right->right = temp_txt;
}

char* get_line(Text *&txt, int index)
{
    if(index<1 || index>length_text(txt))
    {
        return NULL;
    }
    Text *temp_txt = txt;
    while(temp_txt->right!=NULL)
    {
        if(index <= temp_txt->left->weight)
        {
            temp_txt = temp_txt->left;
        }
        else
        {
            index = index - temp_txt->left->weight;
            temp_txt = temp_txt->right;
        }
    }
    if(temp_txt->weight == index)
    {
        return (char *)temp_txt->left;
    }
    return NULL;
}

char* set_line(Text *&txt, int index, char *new_line)
{
    if(index<1 || index>length_text(txt))
    {
        return NULL;
    }
    Text *temp_txt = txt;
    while(temp_txt->right!=NULL)
    {
        if(index <= temp_txt->left->weight)
        {
            temp_txt = temp_txt->left;
        }
        else
        {
            index = index - temp_txt->left->weight;
            temp_txt = temp_txt->right;
        }
    }
    if(temp_txt->weight == index)
    {
        char *tmp_ln = (char *)temp_txt->left;
        temp_txt->left = (Text *)new_line;
        return tmp_ln;
    }
    return NULL;
}

void append_line(Text *&txt, char *new_line)
{
    if (length_text(txt)==0) 
    {
        Text *eof = new Text();
        Text *temp_txt = new Text();
        eof->left = (Text *)(char *)EOF; 
        temp_txt->left = (Text *) new_line;
        eof->right = NULL;
        temp_txt->right = NULL;
        eof->weight = 1;
        temp_txt->weight = 1;
        txt->left = temp_txt;
        txt->right = eof;
        txt->weight = 2;
    } 
    else 
    {
        Stack *stk = create_stack();
        Text *temp_txt = txt;
        while (temp_txt->right != NULL) 
        {
            push(temp_txt, stk);
            temp_txt = temp_txt->right;
        }
        Text *old_leaf = new Text();
        Text *new_leaf = new Text();
        old_leaf->left = temp_txt->left;
        old_leaf->right = NULL;
        old_leaf->weight = 1;
        new_leaf->left = (Text *) new_line;
        new_leaf->right = NULL;
        new_leaf->weight = 1;
        temp_txt->right = old_leaf;
        temp_txt->left = new_leaf;
        temp_txt->weight = 2;
        while (!stack_empty(stk)) 
        {
            temp_txt = pop(stk);
            temp_txt->weight = temp_txt->left->weight + temp_txt->right->weight;
            if (temp_txt->right->weight < (ALPHA * temp_txt->weight)) 
            {
                if (temp_txt->left->left->weight > ((ALPHA + EPSILON) * temp_txt->weight)) 
                {
                    right_rotation(temp_txt);
                    temp_txt->right->weight = temp_txt->right->left->weight + temp_txt->right->left->weight;
                } 
                else 
                {
                    left_rotation(temp_txt->left);
                    right_rotation(temp_txt);
                    temp_txt->right->weight = temp_txt->right->left->weight + temp_txt->right->right->weight;
                    temp_txt->left->weight = temp_txt->left->left->weight + temp_txt->left->right->weight;
                }
            } 
            else if (temp_txt->left->weight < (ALPHA * temp_txt->weight)) 
            {
                if (temp_txt->right->right->weight > ((ALPHA + EPSILON) * temp_txt->weight)) 
                {
                    left_rotation(temp_txt);
                    temp_txt->left->weight = temp_txt->left->left->weight + temp_txt->left->right->weight;
                } 
                else 
                {
                    right_rotation(temp_txt->right);
                    left_rotation(temp_txt);
                    temp_txt->right->weight = temp_txt->right->left->weight + temp_txt->right->right->weight;
                    temp_txt->left->weight = temp_txt->left->left->weight + temp_txt->left->right->weight;
                }
            }
        }
        remove_stack(stk);
    }
}

void inorder(Text *&txt)
{
    if(txt->right == NULL)
    {
        printf("%d->",txt->weight);
        return;
    }
    inorder(txt->left);
    printf("%d->",txt->weight);
    inorder(txt->right);
}

char* delete_line(Text *&txt, int index)
{
    if(length_text(txt)==0)
    {
        return NULL;
    }
    else if(index==1 && length_text(txt)==1)
    {
        char *del_ln = (char *)txt->left->left;
        delete txt->left;
        delete txt->right;
        txt->left = txt->right = NULL;
        txt->weight = 0;
        return del_ln;
    }
    else if(index>=1 && index<=length_text(txt))
    {
        char *del_ln = NULL;
        Stack *stk = create_stack();
        Text *temp_txt = txt;
        Text *upper, *other;
        while (temp_txt->right != NULL) 
        {
            upper = temp_txt;
            if(index <= temp_txt->left->weight)
            {
                if(temp_txt->left->weight>1)
                {
                    push(temp_txt,stk);
                }
                temp_txt = upper->left;
                other = upper->right;
            }
            else
            {
                if(temp_txt->right->weight>1)
                {
                    push(temp_txt,stk);
                }
                index = index - upper->left->weight;
                temp_txt = upper->right;
                other = upper->left;
            }
        }
        if(temp_txt->weight == index)
        {
            upper->weight = other->weight;
            upper->left = other->left;
            upper->right = other->right;
            del_ln = (char *)temp_txt->left;
            delete temp_txt;
            delete other;
        }
        while (!stack_empty(stk)) 
        {
            temp_txt = pop(stk);
            temp_txt->weight = temp_txt->left->weight + temp_txt->right->weight;
            if (temp_txt->right->weight < (ALPHA * temp_txt->weight)) 
            {
                if (temp_txt->left->left->weight > ((ALPHA + EPSILON) * temp_txt->weight)) 
                {
                    right_rotation(temp_txt);
                    temp_txt->right->weight = temp_txt->right->left->weight + temp_txt->right->left->weight;
                }
                else 
                {
                    left_rotation(temp_txt->left);
                    right_rotation(temp_txt);
                    temp_txt->right->weight = temp_txt->right->left->weight + temp_txt->right->right->weight;
                    temp_txt->left->weight = temp_txt->left->left->weight + temp_txt->left->right->weight;
                }
            } 
            else if (temp_txt->left->weight < (ALPHA * temp_txt->weight)) 
            {
                if (temp_txt->right->right->weight > ((ALPHA + EPSILON) * temp_txt->weight)) 
                {
                    left_rotation(temp_txt);
                    temp_txt->left->weight = temp_txt->left->left->weight + temp_txt->left->right->weight;
                } 
                else 
                {
                    right_rotation(temp_txt->right);
                    left_rotation(temp_txt);
                    temp_txt->right->weight = temp_txt->right->left->weight + temp_txt->right->right->weight;
                    temp_txt->left->weight = temp_txt->left->left->weight + temp_txt->left->right->weight;
                }
            }
        }
        remove_stack(stk);
        return del_ln;
    }
    return NULL;
}

void insert_line(Text *&txt, int index, char *new_line)
{ 
    if( length_text(txt)==0 || index<1 || index>length_text(txt))
    { 
        append_line(txt, new_line);
    }
    else
    { 
        Stack *stk = create_stack();
        Text *temp_txt = txt;
        while( temp_txt->right != NULL )
        { 
            push(temp_txt,stk);
            if(index <= temp_txt->left->weight)
            {
                push(temp_txt,stk);
                temp_txt = temp_txt->left;
            }
            else
            {
                index = index - temp_txt->left->weight;
                temp_txt = temp_txt->right;
            }
        }
        if( temp_txt->weight == index )
        { 
            Text *old_leaf = new Text();
            Text *new_leaf = new Text();
            old_leaf->left = temp_txt->left;
            old_leaf->right = NULL;
            old_leaf->weight = 1;
            new_leaf->left = (Text *)new_line;
            new_leaf->right = NULL;
            new_leaf->weight = 1;
            temp_txt->left = new_leaf;
            temp_txt->right = old_leaf;
            temp_txt->weight = 2;
            while (!stack_empty(stk)) 
            {
                temp_txt = pop(stk);
                temp_txt->weight = temp_txt->left->weight + temp_txt->right->weight;
                if (temp_txt->right->weight < (ALPHA * temp_txt->weight)) 
                {
                    if (temp_txt->left->left->weight > ((ALPHA + EPSILON) * temp_txt->weight)) 
                    {
                        right_rotation(temp_txt);
                        temp_txt->right->weight = temp_txt->right->left->weight + temp_txt->right->left->weight;
                    }
                    else 
                    {
                        left_rotation(temp_txt->left);
                        right_rotation(temp_txt);
                        temp_txt->right->weight = temp_txt->right->left->weight + temp_txt->right->right->weight;
                        temp_txt->left->weight = temp_txt->left->left->weight + temp_txt->left->right->weight;
                    }
                } 
                else if (temp_txt->left->weight < (ALPHA * temp_txt->weight)) 
                {
                    if (temp_txt->right->right->weight > ((ALPHA + EPSILON) * temp_txt->weight)) 
                    {
                        left_rotation(temp_txt);
                        temp_txt->left->weight = temp_txt->left->left->weight + temp_txt->left->right->weight;
                    } 
                    else 
                    {
                        right_rotation(temp_txt->right);
                        left_rotation(temp_txt);
                        temp_txt->right->weight = temp_txt->right->left->weight + temp_txt->right->right->weight;
                        temp_txt->left->weight = temp_txt->left->left->weight + temp_txt->left->right->weight;
                    }
                }
            }
            remove_stack(stk);
        }
    }
}

#include "test_file.cpp"