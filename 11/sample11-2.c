#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 255

typedef struct dir_list{
  char            *name;
  long int         size;
  struct dir_list *prev;
  struct dir_list *next;
} DIR_LIST;

DIR_LIST *new_item(char *name, long int size, DIR_LIST *post_item, DIR_LIST *pre_item)
{
    DIR_LIST *new;

    /*** 新しいリスト要素の領域確保 ***/
    new=(DIR_LIST*)malloc(sizeof(DIR_LIST));

    if (!new) {
        perror("new_item");
        exit(1);
    }

    if (name) {
        new->name = strdup(name);
        if (!new->name) {
            perror("new_item");
            exit(1);
        }
    }
printf("%s\n",name);
    new->size = size;

    /*** new の次の要素の指定 ***/
    new->next=post_item;
 
    /*** post_item の前の要素の指定（post_item が NULL でなければ）***/
    if(post_item!=NULL)
      post_item->prev=new;
 
    /*** new の前の要素の指定 ***/
    new->prev=pre_item;

    /*** pre_item の次の要素の指定（pre_item が NULL でなければ） ***/
    if(pre_item!=NULL)
      pre_item->next=new;

    return new;
}

DIR_LIST *insert(DIR_LIST *root, char *name, long int size){

  DIR_LIST *p;

  /* root が NULL の場合の新規リスト要素の挿入処理 */
  if(root == NULL)
    return(new_item(name, size, NULL, NULL));

  /* root の前が挿入位置の場合の処理 */
  if(/*** root の前が挿入位置であることを表す条件式 ***/strcmp(name,root->name)<0)
    return(new_item(name, size, root, NULL));

  /* 挿入位置の検索 */
  p = root;
  while(/*** 挿入位置検索のための条件式 ***/strcmp(name,p->name)<0)
    p = p->next;


  /* 新規リスト要素の挿入 */
  p->next = new_item(name, size, p->next, p);

  return(root);
}

void print_list(DIR_LIST *item){

  /*** item が NULL なら戻る ***/
  if(item==NULL)
    return;

  /*** ファイル名とファイルサイズの出力 ***/
  printf("%ld %s\n",item->size,item->name);

  /*** 次の要素の出力 ***/
  print_list(item->next);
}

void clear_list(DIR_LIST *item){

  /*** item が NULL なら戻る ***/
  if(item==NULL)
    return;
  /*** clear_list の再帰呼出し ***/
  clear_list(item->next);

  /*** ファイル名の領域を解放 ***/
  free(item->name);

  /*** item の領域解放 ***/
  free(item);

}

int main(void)
{
  char file[MAX_LEN];
  int  size;
  int  cnt;
  DIR_LIST* root = NULL;
 
  while (scanf(" %d %s\n", &size, file) != EOF){
    /* リストに追加 */
    root = insert(root, file, size);
  }
  /* ファイルリストの表示 */
  print_list(root);

  /* ファイルリストの削除 */
  clear_list(root);

  exit(0);
}
