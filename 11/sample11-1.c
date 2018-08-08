#include <stdio.h>
#include <stdlib.h>

int main(void)
{
  int  n_data;
  int* data;
  int  sign;
  int  i, j, swap;

  printf("Input the number of data:");
  scanf("%d", &n_data);

  /* メモリ確保 */
  data = (int*)malloc(sizeof(int)*n_data);
  if (data==NULL){
    printf("Fail to allocate memory.\n ");
    return (-1);
  }

  /* データの入力 */
  for (i = 0; i < n_data; i++){
    printf("Input %d-th data:", i);
    scanf("%d", &data[i]);
  }
  
  /* 昇順・降順の確認 */
  printf("Input sorting order (1: ascending, -1: descending):");
  scanf("%d", &sign);
  
  if (abs(sign) != 1){
    printf("Invalid sorting order (%d).\n ", sign);
    return (-1);
  }
   
  /* ソート */
  for (i = 0; i < n_data - 1; i++){
    for (j = i + 1; j < n_data; j++){
      if (sign * (data[i] - data[j]) > 0){
        swap = data[i];
        data[i] = data[j];
        data[j] = swap;
      }
    }
  }

  /* 表示 */
  printf("Sorted data:\n ");
  for (i = 0; i < n_data; i++){
    printf("%d\n", data[i]);
  }

  /* メモリ解放 */
  free(data);

  return (0);
}

