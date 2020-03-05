#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
 
 
typedef struct node {
    char c;
    struct node* next;
}node;
 
node* list_new() {
    return (node*)malloc(sizeof(node));
}
 
void listdel(node** head)
{
    node* n = *head;
    *head = (*head)->next;
    free(n);
}
 
struct node * deletelem(node *head1,node *head2)
{
    struct node *temp=head2;
        while (temp->next != head1)
        {
            temp = temp->next;
        }
        temp->next = head1->next;
        free(head1);
    return(temp);
}
 
 
 
 
void list_insert_end(node** head, char c)
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
 
void list_insert_beg(node** head, char c)
{
    node *n = list_new();
    n->c = c;
    n->next = *head;
    *head = n;
}
 
void list_insert_words(node** head, char* s)
{
    while (*s) {
        list_insert_end(head, *s);
        *s++;
    }
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
    //char *s = NULL;
    char *s = (char*)calloc(1, sizeof(char));
    int len = 0;
    printf(msg);
 
    while (fgets(buf, 50, stdin))
    {
        len += strlen(buf);
        if (len == 1) {
            free(s);
            return NULL;
        }
        char* p = (char*)realloc(s, len + 1);
        if (!p) {
            free(s);
            return NULL;
        }
        s = p;
        strcat(s, buf);
        if (s[len - 1] == '\n') {
            s[len - 1] = '\0';
            return s;
        }
    }
 
    free(s);
    return NULL;
}
 
void insrtfs(node** head,char* s)
{
    int k = 0;
    node* n = *head;
    for (k=0; k < strlen(s); k++)
    {
        if (s[k] == '\t')
            s[k] = ' ';
    }
    for (k=0; k < strlen(s); k++)
    {
        if((s[k]!=' ')||((s[k] == ' ')&&(s[k+1]!=' ')))
        list_insert_end(head, s[k]);
    }
    if ((*head)->c == ' ')
        listdel(head);
}
 
void del0f(node** head)
{
    node* n = *head;
    int i=0;
    for (; n->next->c =='0';)
        if (n->c == '0')
        {
            listdel(head);
            n = *head;
        }
    if((n->next->c!=' ')&&(n->c=='0'))
        listdel(head);
    node* h = *head;
 
    for (n = *head; n->next->next != NULL;)
    {
        if (n->next->c == '0')
        {
            h = deletelem(n->next, h);
            n = *head;
        }
        else
            n = n->next;
        while ((n->c != ' '))
        {
            n = n->next;
            if (n == NULL)
                break;
        }
       
    }
}
 
node* revers(node** head)
{
    int i = 0;
    node* head1 = NULL;
    node* head2 = NULL;
    node* k =NULL;
    node* n = *head;
    for (i=0;n!=NULL;i++)
    {
        for (; (n!=NULL)&&(n->c != ' '); n = n->next)
        {
            list_insert_beg(&head1, (n)->c);
        }
        if (i == 0)
        {
            head2 = head1;
            k = head2;
            head1 = NULL;
        }
        if (i > 0)
        {
            while (k->next != NULL)
                k = k ->next;
            k->next = head1;
            while (head1 != NULL)
                head1 = head1->next;
        }
        if(n!=NULL)
            if (n->c == ' ')
            {
                n = n->next;
                list_insert_end(&head2, ' ');
            }
    }
    return head2;
}
int main()
{
    while (1)
    {
        node* head = NULL;
        node* head1 = NULL;
        char* s = read_line("string: ");
        if (!s) {
            return 0;
        }
        insrtfs(&head, s);
        list_print(head);
        del0f(&head);
        list_print(head);
        head1 = revers(&head);
        del0f(&head1);
        list_print(head1);
        list_free(&head);
        free(s);
    }
}