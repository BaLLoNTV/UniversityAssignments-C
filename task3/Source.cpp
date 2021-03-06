#include <stdlib.h>
#include <stdio.h>
#include <string.h>
 
 
#define T_SIZE 10
char* data_file_name = NULL;
 
typedef struct {
    int busy;
    int key;
    int ver;
    int offset;
    char* data;
} t_item;
 
typedef struct {
    int busy;
    int key;
    int ver;
    int delver;
} t_info;
 
void table_init(t_item* items) {
    for (int i = 0; i < T_SIZE; i++) {
        items[i].busy = 0;
        items[i].key = NULL;
        items[i].ver = 0;
        items[i].data = NULL;
    }
}
 
t_info table_get_free_index(t_item* items, int key, int ver)
{
    t_info k;
    k.busy = -1;
    k.key = 0;
    k.ver = 0;
    k.delver = -1;
    int m = 0, h = key;
    for (int n = 0, i = h % T_SIZE; n <= T_SIZE; i = (h++) % T_SIZE, n++)
    {
        if (items[i].busy <= 0 && k.key == 0)
        {
            k.key = i;
            k.busy = 0;
            if (items[i].busy == 0)
                return k;
        }
        if (items[i].busy == 1 && items[i].key == key)
        {
            if (k.ver <= items[i].ver)
                k.ver = items[i].ver + 1;
            if (ver == items[i].ver || ver == -1)
            {
                k.busy = 1;
                k.delver = i;
                return k;
            }
        }
 
    }
    return k;
}
 
int table_insert(t_item* items, int key, char* value)
{
    t_info idx = table_get_free_index(items, key, -2);
    FILE* f = fopen(data_file_name, "r+b");
    if (idx.busy == -1) {
        return -1;
    }
    items[idx.key].busy = 1;
    items[idx.key].key = key;
    items[idx.key].ver = idx.ver;
    items[idx.key].data = value;
    int len = strlen(items[idx.key].data);
    fseek(f, 0, SEEK_END);
    items[idx.key].offset = ftell(f);
    fwrite(&len, sizeof(int), 1, f);
    fwrite(items[idx.key].data, sizeof(char), len, f);
    fclose(f);
    return 0;
}
 
void table_delete_item(t_item* item)
{
    item->busy = -1;
    if (item->data != NULL) {
        free(item->data);
        item->data = NULL;
    }
}
 
int table_delete(t_item* items, int key, int ver)
{
    int del_count = 0;
    t_info k;
    k.busy = 0;
    k.key = 0;
    k.ver = 0;
    k.delver = 0;
    for (k = table_get_free_index(items, key, ver); k.delver != -1; k = table_get_free_index(items, key, ver))
    {
        table_delete_item(&items[k.delver]);
        del_count++;
        if (ver != -1)
            break;
    }
    return del_count;
}
 
void table_print(t_item* items) {
    printf("------------------------------\n");
 
    printf("key\tver\tvalue\n");
    for (int i = 0; i < T_SIZE; i++) {
        if (items[i].busy == 1) {
            printf("%d\t%d\t%s\n", items[i].key, items[i].ver, items[i].data);
        }
    }
    printf("------------------------------\n");
 
}
 
void table_clear(t_item* items) {
    for (int i = 0; i < T_SIZE; i++) {
        if (items[i].busy == 0) {
            continue;
        }
 
        table_delete_item(&items[i]);
    }
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
        s = (char*)realloc(s, len * sizeof(char) + 1);
        strcat(s, buf);
 
        if (s[len - 1] == '\n') {
            s[len - 1] = '\0';
            return s;
        }
    }
 
    return NULL;
}
 
int D_Add(t_item *items)
{
    int key = 0;
    int idx = 0;
    char* data = NULL;
    printf("Kluch: ");
    read_int(&key);
    data = read_line("Stroka: ");
    idx = table_insert(items, key, data);
    if (idx != -1) {
        printf("\n--Uspeshno\n\n");
    }
    else {
        printf("\n--Spisok perepolnen\n\n");
    }
    return 1;
}
int D_Delete(t_item *items)
{
    int del = 0;
    int key = 0;
    int ver = 0;
    printf("Kluch: ");
    read_int(&key);
    printf("Versiya (-1, udalit vse): ");
    read_int(&ver);
    del = table_delete(items, key, ver);
    if (del == 0) {
        printf("\n--Oshibka pri udalenii\n\n");
    }
    else {
        printf("\n--Uspeshno\n\n");
    }
    return 1;
}
int D_Show(t_item *items)
{
    table_print(items);
    return 1;
}
 
t_info table_get_free_index_for_find(t_item* items, int key, int ver, int start)
{
    t_info k;
    k.busy = -1;
    k.key = 0;
    k.ver = 0;
    k.delver = -1;
    int m = 0, h = start;
    int n = start, i = h % T_SIZE;
    for (; n <= T_SIZE + key % T_SIZE; i = (h++) % T_SIZE, n++)
    {
        if (items[i].busy <= 0 && k.key == 0)
        {
            k.key = i;
            k.busy = 0;
            if (items[i].busy == 0)
                return k;
        }
        if (items[i].busy == 1 && items[i].key == key)
        {
            if (k.ver <= items[i].ver)
                k.ver = items[i].ver + 1;
            if (ver == items[i].ver || ver == -1)
            {
                k.busy = 1;
                k.delver = i;
                return k;
            }
        }
 
    }
    return k;
}
 
int table_find(t_item* items, t_item* items1, int key, int ver)
{
    int find_count = 0, i = key % T_SIZE;
    t_info k;
    k.busy = 0;
    k.key = 0;
    k.ver = 0;
    k.delver = key % T_SIZE;
    for (k = table_get_free_index_for_find(items, key, ver, k.delver); k.delver != -1; k = table_get_free_index_for_find(items, key, ver, k.delver + 1))
    {
        table_insert(items1, key, items[k.delver].data);
        find_count++;
        if ((ver != -1) || k.delver + 1 == i)
            break;
    }
    return find_count;
}
 
int D_Find(t_item *items)
{
    t_item items1[T_SIZE];
    int find = 0;
    int key = 0;
    int ver = 0;
    printf("Kluch: ");
    read_int(&key);
    printf("Versiya (-1, udalit vse): ");
    read_int(&ver);
    find = table_find(items, items1, key, ver);
    if (find == 0) {
        printf("\n--Oshibka pri poiske\n\n");
    }
    else {
        table_print(items1);
    }
    return 1;
}
 
int dialog(const char *msgs[], int N)
{
    char *err = "";
    int ans = -1, i, n = 0;
    do
    {
        puts(err);
        err = "Input error. Repeat input.\n";
        for (i = 0; i<N; i++)
            puts(msgs[i]);
        puts("Command >");
        read_int(&ans);
 
    } while ((ans<0) || (ans >= N));
    return ans;
}
 
void init_names(const char* name)
{
    data_file_name = _strdup(name);
    data_file_name = (char*)realloc(data_file_name, strlen(data_file_name) + 5);
    strcat(data_file_name, ".dat");
}
 
int file_exists() {
    FILE* f = fopen(data_file_name, "rb");
    if (!f) {
        return 0;
    }
 
    fclose(f);
    return 1;
}
 
void create_files() {
    int i = 1111;
    FILE* df = fopen(data_file_name, "w+b");
    fseek(df, (T_SIZE * 4 ) * sizeof(int), SEEK_SET);
    fwrite(&i, sizeof(int), 1 , df);
    fclose(df);
}
 
 
 
 
 
int insert(t_item* items, int busy, int key, int ver,int offset, int pos, char* value)
{
    items[pos].busy = busy;
    items[pos].key = key;
    items[pos].ver = ver;
    items[pos].data = value;
    items[pos].offset = offset;
    return 0;
}
 
void read_meta(t_item* items)
{
    int i;
    int res;
    int busy;
    int key;
    int ver;
    int offset;
    int len;
    FILE* f = fopen(data_file_name, "rb");
 
    for (i = 0, fseek(f, 0, SEEK_SET); i<T_SIZE;  i++, fseek(f, (4*i)*sizeof(int), SEEK_SET))
    {
        fread(&busy, sizeof(int), 1, f);
        if (busy < 1)
        {
            insert(items, busy, NULL, NULL,NULL, i, NULL);
            continue;
        }
        fread(&key, sizeof(int), 1, f);
        fread(&ver, sizeof(int), 1, f);
        fread(&offset, sizeof(int), 1, f);
        fseek(f, offset, SEEK_SET);
        fread(&len, sizeof(int), 1, f);
        char* s = (char*)calloc(len + 1, sizeof(char));
        fread(s, sizeof(char), len, f);
        insert(items, busy, key, ver,offset, i, s);
    }
 
    fclose(f);
}
 
void rewrite_meta(t_item* items)
{
    FILE* df = fopen(data_file_name, "r+b");
    int i, n, h = 0;
 
    for (i = 0, fseek(df, 0, SEEK_SET); i < T_SIZE; i++, fseek(df, (4 * i) * sizeof(int), SEEK_SET))
    {
 
            fwrite(&items[i].busy, sizeof(int), 1, df);
            fwrite(&items[i].key, sizeof(int), 1, df);
            fwrite(&items[i].ver, sizeof(int), 1, df);
            fwrite(&items[i].offset, sizeof(int), 1, df);
 
    }
    fclose(df);
}
 
int main()
{
    t_item items[T_SIZE];
    const char *msgs[] = { "0. Quit", "1. Add", "2. Find", "3. Delete", "4. Show" };
    int(*fptr[])(t_item *) = { NULL, D_Add, D_Find, D_Delete, D_Show };
 
    char* filename = read_line("filename: ");
    if (filename == NULL) {
        return 0;
    }
 
    init_names(filename);
    if (!file_exists()) {
        create_files();
        table_init(items);
    }
    else {
        read_meta(items);
    }
    int rc;
    while (rc = dialog(msgs, 5))
        if (!fptr[rc](items))
            break;
    printf("That's all. Bye!\n");
    rewrite_meta(items);
 
}