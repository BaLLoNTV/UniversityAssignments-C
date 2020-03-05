include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
 
typedef struct node {
    char c;
    struct node* next;
}node;
 
node* list_new() {
    return (node*)malloc(sizeof(node));
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
 
void list_insert_word(node** head,
                      char* s,
                      int start,
                      int end)
{
    for (int i = start; i < end; i++) {
        list_insert_end(head, s[i]);
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
 
        if (s[len-1] == '\n') {
            s[len-1] = '\0';
            return s;
        }
    }
 
    free(s);
    return NULL;
}
 
int words_count(char* s)
{
    int count = 0;
    char* tmp = (char*)malloc(strlen(s) * sizeof(char) + 1);
    strcpy(tmp, s);
    char* p = strtok(tmp, " \t");
    if (!p) {
        free(tmp);
        return count;
    }
 
    while (p) {
        count++;
        p = strtok(NULL, " \t");
    }
 
    free(tmp);
    return count;
}
 
int spaces_count(char* s)
{
    int count = 0;
    while (*s) {
        if (*s == ' ' || *s == '\t') {
            count++;
        }
        *s++;
    }
 
    return count;
}
 
char* trim(char* s)
{
    if (!s) {
        return NULL;
    }
 
    int spaces = spaces_count(s);
    int words = words_count(s);
    if ((words - spaces) == 1) {
        return s;
    }
 
    int old_len = strlen(s) + 1;
    char* p = strtok(s, " \t");
    if (!p) {
        return s;
    }
 
    char* res = (char*)calloc(old_len, sizeof(char));
    int len = 0;
 
    while (p)
    {
        len += strlen(p) + 1;
        if (len > old_len) {
            char* p = (char*)realloc(res, len * sizeof(char));
            if (!p) {
                free(res);
                return NULL;
            }
 
            res = p;
        }
 
        strcat(res, p);
        strcat(res, " ");
        p = strtok(NULL, " \t");
    }
 
    if (len < old_len) {
        char* p = (char*)realloc(res, len * sizeof(char));
        if (!p) {
            free(res);
            return NULL;
        }
        res = p;
    }
 
    res[len-1] = '\0';
 
    free(s);
 
    return res;
}
 
int main()
{
    while (1)
    {
        char* s = trim(read_line("string: "));
        if (!s) {
            return 0;
        }
 
        node* head = NULL;
        int k = 0;
 
        int word_start = 0;
        int word_end = 0;
        int total_len = strlen(s);
        int curr_len = 0;
 
		  for (int i = word_start; i <= total_len; i++)
        {
            if ((s[i] == ' ' || s[i] == '\0') && i < 80)
            {
                word_end = i;
                curr_len += (word_end - word_start);
                list_insert_word(&head, s, word_start++, word_end);
                word_start = word_end++;
            }
        }
 
        if (curr_len < 80) {
            int spaces_amount = (80 - curr_len) / 2;
            for (int i = 0; i < spaces_amount; i++) {
                list_insert_beg(&head, ' ');
            }
        }
 
        list_print(head);
        list_free(&head);
        free(s);
    }
}