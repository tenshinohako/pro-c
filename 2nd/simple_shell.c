/*
 *  インクルードファイル
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <dirent.h>

/*
 *  定数の定義
 */

#define BUFLEN	1024	 /* コマンド用のバッファの大きさ */
#define MAXARGNUM  256	 /* 最大の引数の数 */

#define MAXHISTORY 32
#define PATHLEN 256
#define PROMPTLEN 32
#define COMMANDLEN 16

/*
 *	構造体の宣言
 */

typedef struct dirs_stack{
	char dirsname[PATHLEN];
	struct dirs_stack *next;
} DIRS_STACK;
	
typedef struct alias_binary{
	char alias_command[COMMANDLEN];
	char original_command[BUFLEN];
	struct alias_binary *left;
	struct alias_binary *right;
} ALIAS_BINARY;

/*
 *  ローカルプロトタイプ宣言
 */

int parse(char [], char *[]);
void execute_command(char *[], int);

int execute_alias(ALIAS_BINARY *, char *, char *);
void delete_alias(ALIAS_BINARY **, char *);
void show_alias(ALIAS_BINARY *);
void delete_binary(ALIAS_BINARY *);

/*----------------------------------------------------------------------------
 *
 *  関数名   : main
 *
 *  作業内容 : シェルのプロンプトを実現する
 *
 *  引数	 :
 *
 *  返り値   :
 *
 *  注意	 :
 *
 *--------------------------------------------------------------------------*/

int main(int argc, char *argv[])
{
	char command_buffer[BUFLEN]; /* コマンド用のバッファ */
	char *args[MAXARGNUM];	   /* 引数へのポインタの配列 */
	int command_status;		  /* コマンドの状態を表す
									command_status = 0 : フォアグラウンドで実行
									command_status = 1 : バックグラウンドで実行
									command_status = 2 : シェルの終了
									command_status = 3 : 何もしない */
	char history[MAXHISTORY][BUFLEN];
	int history_num[MAXHISTORY] = {};
	int num = 0;
	int prev =0;
	char pathname[PATHLEN];
	char *command_buffer_p;
	char prompt[PROMPTLEN] = "prompt";
	int i;
	int history_x;
	char catbuf[BUFLEN];
	
	FILE *fp;
	
	DIRS_STACK *head = NULL, *p, *access;
	ALIAS_BINARY *a_head = NULL, *a_p, *a_access, *a_temp;
	/*
	 *  無限にループする
	 */

	for(;;) {
		/*
		 *  プロンプトを表示する
		 */

		if(prev == 0){
			if(isatty(fileno(stdin)))
				printf("\n%s : ", prompt);

		/*
		 *  標準入力から１行を command_buffer へ読み込む
		 *  入力が何もなければ改行を出力してプロンプト表示へ戻る
		 */
			if(fgets(command_buffer, BUFLEN, stdin) == NULL) {
				if(isatty(fileno(stdin))){
					printf("\n");
					continue;
				}else{
					exit(EXIT_SUCCESS);
				}
			}
			if(command_buffer[strlen(command_buffer) - 1] == '\n'){
				command_buffer[strlen(command_buffer) - 1] = '\0';
			}
		}
		
		command_buffer_p = &command_buffer[0];
		
		while(*command_buffer_p == ' ' || *command_buffer_p == '\t') {
			command_buffer_p++;
		}
	
		if(strcmp(command_buffer_p, "") != 0){
			if(prev == 0)
				for(i = 0; i<MAXHISTORY-1; i++){
					strcpy(&history[i][0], &history[i+1][0]);
					history_num[i] = history_num[i+1];
				}
			strcpy(&history[MAXHISTORY-1][0], command_buffer);
			history_num[MAXHISTORY-1] = ++num;
		}
		
		prev = 0;

		/*
		 *  入力されたバッファ内のコマンドを解析する
		 *
		 *  返り値はコマンドの状態
		 */

		command_status = parse(command_buffer, args);

		/*
		 *  終了コマンドならばプログラムを終了
		 *  引数が何もなければプロンプト表示へ戻る
		 */

		if(command_status == 2) {
			access = head;					//スタック容量を開放↓
			while(access != NULL){
				access = access->next;
				free(head);
				head = access;
			}								//スタック容量を開放↑
			delete_binary(a_head);	//二分木開放
			printf("done.\n");
			exit(EXIT_SUCCESS);
		} else if(command_status == 3) {
			continue;
		}

		/*
		 *  コマンドを実行する
		 */

		if(!strcmp(args[0], "history")){
			for(i = 0; i<MAXHISTORY; i++)
				if(history_num[i] != 0)
					printf("%2d %s\n",history_num[i], &history[i][0]);
				
		}else if(!strncmp(args[0], "!", 1)){
			++args[0];
			if(!strcmp(args[0], "!")){
				printf("%s\n",&history[MAXHISTORY-2][0]);
				stpcpy(&command_buffer[0], &history[MAXHISTORY-2][0]);
				--num;
				prev = 1;
			}else{
				history_x = 0;
				for(i = 0; *(args[0]+i) != '\0'; i++)
					if(47 < *(args[0]+i) && *(args[0]+i) < 58){
						history_x *= 10;
						history_x += (int)*(args[0]+i) - 48;
					}
					else
						break;
				if(*(args[0]+i) == '\0'){
					for(i = 0; i<MAXHISTORY && prev == 0; i++)
						if(history_x == history_num[MAXHISTORY-1-i]){
							printf("%s\n",&history[MAXHISTORY-1-i][0]);
							strcpy(command_buffer, &history[MAXHISTORY-1-i][0]);
							--num;
							prev = 1;
						}
					if(prev == 0)
						printf("%s: event not found\n", args[0]-1);
				}else{
					for(i = 0; i<MAXHISTORY && prev == 0; i++)
						if(!strncmp(args[0], &history[MAXHISTORY-1-i][0], strlen(args[0]))){
							printf("%s\n",&history[MAXHISTORY-1-i][0]);
							strcpy(command_buffer, &history[MAXHISTORY-1-i][0]);
							--num;
							prev = 1;
						}
					if(prev == 0)
						printf("%s: event not found\n", args[0]-1);
				}
			}
			
		}else if(!strcmp(args[0], "cd")){
			if(chdir(args[1]))
				printf("%s: No such file or directory\n", args[1]);
			getcwd(pathname, PATHLEN);
			printf("%s\n", pathname);
			
		}else if(!strcmp(args[0], "pwd")){
			getcwd(pathname, PATHLEN);
			printf("%s\n", pathname);
			
		}else if(!strcmp(args[0], "pushd")){
			p = (DIRS_STACK*)malloc(sizeof(DIRS_STACK));
			if(args[1] == NULL){
				getcwd(p->dirsname, PATHLEN);
				printf("%s\n",p->dirsname);
				p->next = head;
				head = p;
			}else{
				if(chdir(args[1])){
					printf("%s: No such file or directory\n", args[1]);
					free(p);
				}else{
					getcwd(p->dirsname, PATHLEN);
					printf("%s\n",p->dirsname);
					p->next = head;
					head = p;
				}
			}
			
		}else if(!strcmp(args[0], "dirs")){
			if(head == NULL)
				printf("(null)\n");
			else{
				access = head;
				while(access != NULL){
					printf("%s\n",access->dirsname);
					access = access->next;
				}
			}

		}else if(!strcmp(args[0], "popd")){
			if(head == NULL)
				printf("(null)\n");
			else{
				chdir(head->dirsname);
				printf("%s\n",head->dirsname);
				p = head;
				head = p->next;
				free(p);
			}

		}else if(!strcmp(args[0], "prompt")){
			if(args[1] == NULL)
				strcpy(prompt, "prompt");
			else
				stpcpy(prompt, args[1]);
			
		}else if(!strcmp(args[0], "alias")){
			if(args[1] == NULL){
				show_alias(a_head);
			}
			else{
				a_access = a_head;
				while(a_access != NULL){
					a_temp = a_access;
					if(strcmp(a_access->alias_command, args[1]) > 0)
						a_access = a_access->left;
					else if(strcmp(a_access->alias_command, args[1]) < 0)
						a_access = a_access->right;
					else{
						strcpy(a_access->original_command, args[2]);
						i = 3;
						while(args[i] != NULL){
							strcat(a_access->original_command, " ");
							strcat(a_access->original_command, args[i]);
							++i;
						}
						break;
					}
				}
				if(a_access == NULL){
					a_p = (ALIAS_BINARY *)malloc(sizeof(ALIAS_BINARY));
					strcpy(a_p->alias_command, args[1]);
					strcpy(a_p->original_command, args[2]);
					i = 3;
					while(args[i] != NULL){
						strcat(a_access->original_command, " ");
						strcat(a_access->original_command, args[i]);
						++i;
					}
					a_p->left = NULL;
					a_p->right = NULL;
					if(a_head == NULL)
						a_head = a_p;
					else{
						if(strcmp(a_temp->alias_command, args[1]) > 0)
							a_temp->left = a_p;
						else if(strcmp(a_temp->alias_command, args[1]) < 0)
							a_temp->right = a_p;
					}
				}
			}
		}else if(!strcmp(args[0], "unalias")){
			delete_alias(&a_head, args[1]);
			
		}else if(!strcmp(args[0], "cat")){
			if((fp = fopen(args[1], "r")) == 0){
				printf("%s: No such file or directory\n", args[1]);
			}else{
				while(fgets(catbuf, BUFLEN, fp) != NULL)
					printf("%s",catbuf);
				fclose(fp);
			}
			printf("\n");
		}else{
			if(execute_alias(a_head, args[0], command_buffer)){
				--num;
				prev = 1;
			}else
				execute_command(args, command_status);
		}
	}

	return 0;
}

int execute_alias(ALIAS_BINARY *now, char *command, char *command_buffer)
{
	ALIAS_BINARY *access;
	
	access = now;
	while(access != NULL){
		if(strcmp(access->alias_command, command) > 0){
			access = access->left;
		}else if(strcmp(access->alias_command, command) < 0){
			access = access->right;
		}else{
			strcpy(command_buffer, access->original_command);
			return(1);
		}
	}
		return(0);
}

void delete_alias(ALIAS_BINARY **head, char *command)
{
	ALIAS_BINARY *access1, *access2, *temp1, *temp2 = NULL;
	
	access1 = *head;
	
	while(access1 != NULL){
		if(strcmp(access1->alias_command, command) > 0){
			temp1 = access1;
			access1 = access1->left;
		}
		else if(strcmp(access1->alias_command, command) < 0){
			temp1 = access1;
			access1 = access1->right;
		}
		else
			break;
	}
	
	if(access1 == NULL)
		return;
	
	if(access1->right == NULL){
		if(access1 == *head)
			*head = access1->left;
		else{
			if(strcmp(temp1->alias_command, command) > 0)
				temp1->left = access1->left;
			else if (strcmp(temp1->alias_command, command) < 0)
				temp1->right = access1->left;
		}
		free(access1);
	}else{
		access2 = access1->right;
		while(access2->left != NULL){
			temp2 = access2;
			access2 = access2->left;
		}
		if(temp2 != NULL)
			temp2->left = access2->right;
		access2->left = access1->left;
		access2->right = access1->right;
		if(access1 == *head)
			*head = access2;
		else{
			if(strcmp(temp1->alias_command, command) > 0)
				temp1->left = access2;
			else if (strcmp(temp1->alias_command, command) < 0)
				temp1->right = access2;
		}
		free(access1);
	}
}

void show_alias(ALIAS_BINARY *now)
{
	if(now->left != NULL)
		show_alias(now->left);
	printf("%s: %s\n",now->alias_command,now->original_command);
	if(now->right != NULL)
		show_alias(now->right);
}

void delete_binary(ALIAS_BINARY *now)
{
	if(now != NULL){
		delete_binary(now->left);
		delete_binary(now->right);
		free(now);
	}
}

/*----------------------------------------------------------------------------
 *
 *  関数名   : parse
 *
 *  作業内容 : バッファ内のコマンドと引数を解析する
 *
 *  引数	 :
 *
 *  返り値   : コマンドの状態を表す :
 *				0 : フォアグラウンドで実行
 *				1 : バックグラウンドで実行
 *				2 : シェルの終了
 *				3 : 何もしない
 *
 *  注意	 :
 *
 *--------------------------------------------------------------------------*/

int parse(char buffer[],		/* バッファ */
		  char *args[])			/* 引数へのポインタ配列 */
{
	int arg_index;   /* 引数用のインデックス */
	int status;   /* コマンドの状態を表す */
	
	DIR *dir;
	struct dirent *dp;
	/*
	 *  変数の初期化
	 */

	arg_index = 0;
	status = 0;

	/*
	 *  バッファ内の最後にある改行をヌル文字へ変更
	 */

	//*(buffer + (strlen(buffer) - 1)) = '\0';

	/*
	 *  バッファが終了を表すコマンド（"exit"）ならば
	 *  コマンドの状態を表す返り値を 2 に設定してリターンする
	 */

	if(strcmp(buffer, "exit") == 0) {
		status = 2;
		return status;
	}
	
	/*
	 *  バッファ内の文字がなくなるまで繰り返す
	 *  （ヌル文字が出てくるまで繰り返す）
	 */

	while(*buffer != '\0') {

		/*
		 *  空白類（空白とタブ）をヌル文字に置き換える
		 *  これによってバッファ内の各引数が分割される
		 */

		while(*buffer == ' ' || *buffer == '\t') {
			*(buffer++) = '\0';
		}

		/*
		 * 空白の後が終端文字であればループを抜ける
		 */

		if(*buffer == '\0') {
			break;
		}

		/*
		 *  空白部分は読み飛ばされたはず
		 *  buffer は現在は arg_index + 1 個めの引数の先頭を指している
		 *
		 *  引数の先頭へのポインタを引数へのポインタ配列に格納する
		 */

		args[arg_index] = buffer;
		++arg_index;

		/*
		 *  引数部分を読み飛ばす
		 *  （ヌル文字でも空白類でもない場合に読み進める）
		 */

		while((*buffer != '\0') && (*buffer != ' ') && (*buffer != '\t')) {
			++buffer;
		}
	}

	/*
	 *  最後の引数の次にはヌルへのポインタを格納する
	 */

	args[arg_index] = NULL;

	/*
	 *  最後の引数をチェックして "&" ならば
	 *
	 *  "&" を引数から削る
	 *  コマンドの状態を表す status に 1 を設定する
	 *
	 *  そうでなければ status に 0 を設定する
	 */

	if(arg_index > 0 && strcmp(args[arg_index - 1], "&") == 0) {
		--arg_index;
		args[arg_index] = '\0';
		status = 1;
	} else {
		status = 0;
	}

	/*
	 *  引数が何もなかった場合
	 */

	if(arg_index == 0) {
		status = 3;
	}
	
	if(arg_index > 0 && strcmp(args[arg_index - 1], "*") == 0){	//ワイルドカード機能
		arg_index--;
		dir = opendir(".");
		while ((dp = readdir(dir)) != NULL) {
			args[arg_index] = dp->d_name;
			arg_index++;
		}
		args[arg_index] = NULL;
		closedir(dir);
	}															//ワイルドカード機能
	
	/*
	 *  コマンドの状態を返す
	 */

	return status;
}

/*----------------------------------------------------------------------------
 *
 *  関数名   : execute_command
 *
 *  作業内容 : 引数として与えられたコマンドを実行する
 *			 コマンドの状態がフォアグラウンドならば、コマンドを
 *			 実行している子プロセスの終了を待つ
 *			 バックグラウンドならば子プロセスの終了を待たずに
 *			 main 関数に返る（プロンプト表示に戻る）
 *
 *  引数	 :
 *
 *  返り値   :
 *
 *  注意	 :
 *
 *--------------------------------------------------------------------------*/

void execute_command(char *args[],	/* 引数の配列 */
					 int command_status)	 /* コマンドの状態 */
{
	int pid;	  /* プロセスＩＤ */
	int status;   /* 子プロセスの終了ステータス */
	
	/*
	 *  子プロセスを生成する
	 *
	 *  生成できたかを確認し、失敗ならばプログラムを終了する
	 */

	/******** Your Program ********/
	pid = fork();
	if (pid < 0){
		printf("fork failure\n");
		exit(EXIT_FAILURE);
	}

	/*
	 *  子プロセスの場合には引数として与えられたものを実行する
	 *
	 *  引数の配列は以下を仮定している
	 *  ・第１引数には実行されるプログラムを示す文字列が格納されている
	 *  ・引数の配列はヌルポインタで終了している
	 */

	/******** Your Program ********/
	else if(pid == 0){
		execvp(args[0],args);
		exit(EXIT_SUCCESS); 
	}

	/*
	 *  コマンドの状態がバックグラウンドなら関数を抜ける
	 */

	/******** Your Program ********/
	else{
		if(command_status == 1)
			return;

	/*
	 *  ここにくるのはコマンドの状態がフォアグラウンドの場合
	 *
	 *  親プロセスの場合に子プロセスの終了を待つ
	 */

	/******** Your Program ********/
		int r = waitpid(pid, &status, 0); //子プロセスのプロセスIDを指定して、終了を待つ
		if(r < 0){
			printf("wait failure\n");
			exit(EXIT_FAILURE);
		}
	}
	
	return;
}
/*-- END OF FILE -----------------------------------------------------------*/