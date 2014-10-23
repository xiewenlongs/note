#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <glib.h>

void print_key_value(gpointer key, gpointer value, gpointer user_data);
void display_hash_table(GHashTable *table);
void free_key(gpointer data);
void free_value(gpointer value);

void print_key_value(gpointer key, gpointer value, gpointer user_data)
{
    printf ("%s ---> %s\n", key, value);
}

void display_hash_table(GHashTable *table)
{
    g_hash_table_foreach(table, print_key_value, NULL);
}

void free_key(gpointer data)
{
    printf("We free key: %s \n", data);
    free(data);
}

void free_value(gpointer data)
{
    printf("We free value: %s \n", data);
    free(data);
}

int main(int argc, char *argv[])
{
    GHashTable *table = NULL;

    table = g_hash_table_new(g_str_hash, g_str_equal);

    g_hash_table_insert(table, "1", "one");
    g_hash_table_insert(table, "2", "two");
    g_hash_table_insert(table, "3", "three");
    g_hash_table_insert(table, "4", "four");
    g_hash_table_insert(table, "5", "five");
    display_hash_table(table);
    printf("Size of hash table: %d \n", g_hash_table_size(table));

    gpointer p = g_hash_table_lookup (table, "7");
    if (p == NULL) {
        printf ("=========\n");
    }
    printf ("lookup 7 : %s\n", p);

    //printf("Before replace: 3 ---> %s /n", g_hash_table_lookup(table, "3"));
    //g_hash_table_replace(table, "3", "third");
    //printf("After replace: 3 ---> %s /n", g_hash_table_lookup(table, "3"));

    g_hash_table_remove(table, "2");
    display_hash_table(table);
    printf("Now size of hash table: %d \n", g_hash_table_size(table));

    g_hash_table_destroy(table);

    //table = g_hash_table_new_full(g_str_hash, g_str_equal, free_key, free_value);
    //g_hash_table_insert(table, strdup("one"), strdup("first"));
    //g_hash_table_insert(table, strdup("two"), strdup("second"));
    //g_hash_table_insert(table, strdup("three"), strdup("third"));

    //printf("Remove an item from hash table: /n");
    //g_hash_table_remove(table, "two");

    //printf("Destroy hash table: /n");
    //g_hash_table_destroy(table);

    return 0;
}
