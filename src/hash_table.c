#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>


// author: Felipe Pegoraro
// data..: 29/06/2023


#include "./hash_table.h"

HashTable *hs_create(size_t key_size, size_t value_size, size_t max_size)
{
  HashTable *hs  = (HashTable *)malloc(sizeof(HashTable));
  hs->size       = max_size;
  hs->length     = 0;
  hs->buckets    = (Entry **)calloc(max_size, sizeof(Entry *));
  hs->items_size.key_s   = key_size;
  hs->items_size.value_s = value_size;
  return hs;
}


static void hs_free_item(Entry *en){
  free(en->key);
  free(en->value);
  free(en);
}


void hs_free(HashTable *hs){
  for (size_t i = 0; i < hs->size; ++i) {
    Entry *current = hs->buckets[i];
    while (current != NULL)
    {
      Entry *aux = current;
      current = current->next;
      hs_free_item(aux);
    }
  }
  free(hs->buckets);
  free(hs);
}



static Entry *hs_create_item(
  size_t key_size, void *key, 
  size_t value_size, void *value
){
  Entry *ret = (Entry *) malloc(sizeof(Entry));

  ret->key = malloc(key_size);
  memcpy(ret->key, key, key_size);

  ret->value = malloc(value_size);
  memcpy(ret->value, value, value_size);

  ret->next = NULL;

  return ret;
}


static unsigned char *hs_hash(const void *key, size_t size)
{
  unsigned char *hash = (unsigned char*) malloc(SHA256_DIGEST_LENGTH);
  SHA256((unsigned const char*) key, size, hash);
  return hash;
}


static void hs_free_hash(unsigned char* hash)
{
  free(hash);
}


// TODO: colisão


void hs_insert(HashTable *hs, void *key, void *value)
{
  unsigned char *hash = hs_hash(key, hs->items_size.key_s);
  size_t index = hash[0] % hs->size;

  Entry *new = hs_create_item(
    hs->items_size.key_s, key,
    hs->items_size.value_s, value
  );

  if (hs->buckets[index] == NULL) {
    hs->buckets[index] = new;
  } else {
    Entry *current = hs->buckets[index];
    while (current->next != NULL) {
      current = current->next;
    }
    current->next = new;
  }

  hs_free_hash(hash);
  hs->length++;
}


void *hs_search(HashTable *hs, const void *key)
{
  unsigned char *hash = hs_hash(key, hs->items_size.key_s);
  
  size_t index = hash[0] % hs->size;

  Entry *current = hs->buckets[index];
  while (current != NULL)
  {
    if (memcmp(current->key, key, hs->items_size.key_s) == 0)
    {
      hs_free_hash(hash);
      return current->value;
    }

    current = current->next;
  }

  hs_free_hash(hash);
  return NULL;
}


// TODO: delete
void  hs_delete(HashTable *hs, void *key);
