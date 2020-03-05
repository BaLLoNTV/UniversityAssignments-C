
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>



typedef struct node
{
	int key;
	char* data;
	struct node* r;
	struct node* l;
	struct node* pr;
}node;



typedef struct {
	struct node* it;
	struct node* father;
	int busy;
} temp;

void delete_tree(node **root) {
	for (; *root != NULL;)
	{
		node* m = *root;
		(*root)->data = NULL;
		*root = (*root)->pr;
		free(m);
	}
}

node* list_new() {
	return (node*)malloc(sizeof(node));
}

temp* find(node** items, int key)
{
	node* idx = *items;
	node* idx2 = NULL;
	temp* hf = (temp*)malloc(sizeof(temp));
	if (*items == NULL)
	{
		hf->it = idx;
		hf->busy = -1;
		return hf;
	}
	for (;;)
	{
		if (idx->key == key)
		{
			hf->it = idx;
			hf->father = idx2;
			hf->busy = 1;
			return hf;
		}
		else if (idx->key > key && idx->l != NULL) {
			idx2 = idx;
			idx = idx->l;
		}
		else if (idx->key < key && idx->r != NULL) {
			idx2 = idx;
			idx = idx->r;
		}
		else
		{
			hf->it = idx;
			hf->busy = 0;
			return hf;
		}
	}
}





int table_insert(node** items, int key, char* value, node** last)
{
	temp* hf = NULL;
	hf = find(items, key);
	if (hf->busy == 1)
	{
		free(hf);
		return -1;
	}
	else if (hf->busy == -1)
	{
		*items = list_new();
		(*items)->data = value;
		(*items)->r = NULL;
		(*items)->l = NULL;
		(*items)->pr = NULL;
		(*items)->key = key;
		*last = *items;
	}
	else if (hf->busy == 0)
	{
		node* m = list_new();
		m->data = value;
		m->key = key;
		m->l = NULL;
		m->r = NULL;
		m->pr = *last;
		if (key > hf->it->key) {
			hf->it->r = m;
			*last = m;
		}
		else {
			hf->it->l = m;
			*last = m;
		}
	}
	free(hf);
}

void read_int(int* num)
{
	int res = 0;

	while ((res = scanf("%d", num)) <= 0)
	{
		printf("Neverniy vvod\n");
		setbuf(stdin, NULL);
	}
	setbuf(stdin, NULL);
}

char* read_line(const char* msg)
{
	char buf[50] = { 0 };
	char* s = (char*)calloc(50, sizeof(char));
	int len = 0;
	printf(msg);

	while (fgets(buf, 50, stdin) != NULL)
	{
		len += strlen(buf);
		if ((len - 1) == 0) {
			return NULL;
		}
		s = (char*)realloc(s, len * sizeof(char)+1);
		strcat(s, buf);

		if (s[len - 1] == '\n') {
			s[len - 1] = '\0';
			return s;
		}
	}

	return NULL;
}

int D_Add(node **items, node** last)
{
	int key = 0;
	int idx = 0;
	char* data = NULL;
	printf("Kluch: ");
	read_int(&key);
	data = read_line("Stroka: ");
	idx = table_insert(items, key, data, last);
	if (idx != -1) {
		printf("\n--Uspeshno\n\n");
	}
	else
	{
		printf("Element uje sushestvuet \n");
	}
	return 1;
}

int rad_last(node** last, node* check1, node* hf, node* start)
{
	node* check;
	for (check = start; check->pr!=hf; check = check->pr)
	{}
	check->pr = check->pr->pr;
	for (check = start; check->pr != check1; check = check->pr)
	{}
	check->pr = hf;
	hf->pr = check1->pr;
	*last = start->pr;

}
int rad_last1(node** last, node* check1, node* hf, node* start)
{
	node* check;
	node* check2;
	for (check = start; check->pr != hf; check = check->pr)
	{
	}
	check->pr = check1;
	check2 = check1->pr;
	check1->pr = hf->pr;
	for (check = start; check->pr != check1; check = check->pr)
	{
	}
	check->pr = hf;
	hf->pr = check2;
	*last = start->pr;
}





int table_delete(node** items, int key, node** last, int rec)
{
	node* check = list_new();
	node* check1 ;
	node* check2 ;
	node* start = list_new();
	start->pr = *last;
	check = NULL;
	temp* hf = NULL;
	hf = find(items, key);
	int god = 0;
	if (hf->busy == -1 || hf->busy == 0)
	{
		return hf->busy;
	}
	else if (hf->busy == 1)
	{
		if (hf->it->l == NULL && hf->it->r == NULL)
		if (hf->father == NULL)
		{
			*items = NULL;
			*last = NULL;
			free(start);
			return 1;
		}
		else
		{
			if (hf->father->r == hf->it)
			{
				hf->father->r = NULL;
			}
			else
			{
				hf->father->l = NULL;
			}
			for (check = start;; check = check->pr)
			{
				if (*last == hf->it) {
					*last = (*last)->pr;
					free(hf->it);
					free(start);
					return 1;
				}
				else if (check->pr == hf->it) {
					check->pr = check->pr->pr;
					free(hf->it);
					free(start);
					return 1;
				}
			}
		}
		else if ((hf->it->r != NULL && hf->it->l == NULL) || (hf->it->r == NULL && hf->it->l != NULL))
		{
			if (hf->it->r != NULL)
			{
				check1 = hf->it->r;
				hf->it->data = hf->it->r->data;
				god = hf->it->r->key;
				hf->it->l = hf->it->r->l;
				hf->it->r = hf->it->r->r;
				rad_last(last, check1, hf->it, start);
				hf->it->key = god;
				return 1;

			}
			else
			{
				check1 = hf->it->l;
				hf->it->data = hf->it->l->data;
				god = hf->it->l->key;
				hf->it->r = hf->it->l->r;
				hf->it->l = hf->it->l->l;
				rad_last(last, check1, hf->it, start);
				hf->it->key = god;
				return 1;
			}
		}
		else if (hf->it->r != NULL && hf->it->l != NULL)
		{
			if (hf->it->r->l == NULL)
			{
				check1 = hf->it->r;
				hf->it->data = hf->it->r->data;
				god = hf->it->r->key;
				hf->it->r = hf->it->r->r;
				rad_last(last, check1, hf->it, start);
				hf->it->key = god;
				return 1;

			}
			else
			{
				for (check = hf->it->r->l; check->l != NULL; check = check->l)
				{
				}
				check1 = check;
				hf->it->data = check->data;
				god = check->key;
				rad_last1(last, check1, hf->it, start);
				table_delete(items, check->key, last, 1);
				hf->it->key = god;
				return 1;
			}
		}
	}
	free(start);
}
int D_Delete(node **items, node** last)
{
	int key = 0;
	int del = 0;
	printf("Kluch: ");
	read_int(&key);
	del = table_delete(items, key, last, 0);
	if (del == 0) {
		printf("\n--Oshibka pri udalenii\n\n");
	}
	else if (del == -1) {
		printf("\n--Drevo pusto\n\n");
	}
	else {
		printf("\n--Uspeshno\n\n");
	}
	return 1;
}

void inorderTraversal(node* x)
{
	if (x != NULL)
	{
		inorderTraversal(x->l);
		printf(" %d", x->key);
		inorderTraversal(x->r);
	}
}





int D_Timing()
{
	node* root = list_new();
	node* last1 = list_new();
	temp* hf = NULL;
	root = NULL;
	last1 = NULL;
	int n = 10, key[10000], k, cnt = 400000, i, m;
	clock_t first, last;
	srand(time(NULL));
	while (n-- > 0) {
		for (i = 0; i < 10000; ++i)
			key[i] = rand() * rand();
		for (i = 0; i < cnt;) {
			k = rand() * rand();
			if (table_insert(&root, k, "", &last1))
				++i;
		}
		m = 0;
		first = clock();
		for (i = 0; i < 10000; ++i)
		{
			hf = find(&root, key[i]);
			if (hf->busy)
				++m;
		}
		last = clock();
		printf("%d items was found\n", m);
		printf("test #%d, number of nodes = %d, time = %d\n", 10 - n, (10 - n)*cnt, last - first);
	}
	delete_tree(&last1);
	return 1;
}


void vivod_nit(node* last)
{
	node* check;
	for (check = last; check != NULL; check = check->pr)
	{
		printf("%d ", check->key);
	}
	printf("\n");
}

void treeprint(node *p, int level) {
	int k;
	if (p == NULL) return;
	treeprint(p->r, level + 3);
	for (int i = 0; i<level; ++i)
		printf(" ");
	if (p->pr)
	printf("(%03d)(%03d)\n", p->key, p->pr->key);
	else
	printf("(%03d)(   )\n", p->key);
	treeprint(p->l, level + 3);

}



int main()
{
	node* items = list_new();
	node* last = list_new();
	int i = 0, ch = 0;
	items = NULL;
	last = NULL;
	const char *msgs[] = { "0. Quit", "1. Add", "2. Delete", "3. Show as tree", "4. Show ", "5. Read file" };
	for (i = 0; i < 6; i++)
	{
		puts(msgs[i]);
	}
	while (1)
	{


		puts("Command >");
		read_int(&ch);
		switch (ch)
		{
		case 0:
			return 0;
			break;
		case 1:
			D_Add(&items, &last);
			break;
		case 2:
			D_Delete(&items, &last);
			break;
		case 3:
			treeprint(items, 0);
			printf("\n");
			vivod_nit(last);
			break;
		case 4:
				  inorderTraversal(items);
				  printf("\n");
				  break;
		case 5:
			D_Timing();
			break;
			//default:
			//return 0 ;
			break;
		}
	}
}

/* 6 9 8 16 1 10 27 9 6 8 */
