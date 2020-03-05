#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
 
typedef struct node {
    char c;
    struct node* next;
}node;
 
node* list_new() {
    return (node*)malloc(sizeof(node));
}
 
void list_inserts(node** head, char c)
{
    node* n = list_new();
    n->c = c;
    n->next = *head;
    *head = n;
}
 
void list_inserte(node** head, char c)
{
    node* n = list_new();
    n->c = c;
    n->next = NULL;
 
    if (*head == NULL) {
        *head = n;
        return;
    }
 
    node* tmp = *head;
 
    while (tmp->next != NULL) {
        tmp = tmp->next;
    }
 
    tmp->next = n;
 
}
 
void list_print(node* head)
{
    while (head != NULL) {
        printf("%c", head->c);
        head = head->next;
    }
    printf("\n");
}
 
void list_free(node** head)
{
    while (*head != NULL)
    {
        node* tmp = *head;
        *head = (*head)->next;
        free(tmp);
    }
}
 
char* read_line(const char* msg)
{
    char buf[50];
    char* s = (char*)calloc(1, sizeof(char));
    int len = 0;
    printf(msg);
 
    while (fgets(buf, 50, stdin) != NULL)
    {
        len += strlen(buf);
        if ((len - 1) == 0) {
            free(s);
            return NULL;
        }
        s = (char*)realloc(s, len * sizeof(char) + 1);
        strcat(s, buf);
 
        if (s[len - 1] == '\n') {
            s[len - 1] = '\0';
            return s;
        }
    }
 
    free(s);
    return NULL;
}
 
char* trim(char* s)
{
    char* res = (char*)malloc(1*sizeof(char));
    res[0] = '\0';
    int len = 0,vim=0;
    char* p = strtok(s, " \t");
    if (!p) {
        free(res);
        return s;
    }
    else {
        while (p != NULL) {
            p[strlen(p)] = '\0';
            len += strlen(p)+1;
            res[vim] = '\0';
            res = (char*)realloc(res, len * sizeof(char));
            res[vim] = '\0';
            strcat(res, p);
            res[len-1] = ' ';
            vim = len;
            //res[vim] = '\0';
            p = strtok(NULL, " \t");
        }
    }
    res[len - 1] = '\0';
    free(s);
    return res;
}
 
int main()
{
    for (;;)
    {
        node* head = NULL;
        char* s = read_line("Введите строку: ");
        char* t = NULL;
        int i = 0, j = 0, k = 0, n = 0, m = 0;
        if (s) {
            t = trim(s);
            puts(t);
        }
        else
        {
            return NULL;
        }
        for (k = 0, i = 0; i < 80; i++)
        {
            if (t[i] == ' ')
                k = i;
            if (t[i] == '\0')
            {
                k = i;
                break;
            }
        }
        for (i = 0; i < k; i++)
        {
            list_inserte(&head, t[i]);
        }
        if ((80 - k) % 2 == 1)
        {
            float f = k / 2;
            k = 40 - round(f);
        }
        else if ((80 - k) % 2 == 0)
            k = (80 - k) / 2;
 
        for (i = 0; i < k; i++)
            list_inserts(&head, ' ');
        free(t);
        list_print(head);
        list_free(&head);
    }
    system("pause");
}