#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

void main()
{

	int pid;	  /* プロセスＩＤ */
	int status;   /* 子プロセスの終了ステータス */



	
	/*
	 *  子プロセスを生成する
	 *
	 *  生成できたかを確認し、失敗ならばプログラムを終了する
	 */

	/******** Your Program ********/

	
	/*
	 *  子プロセスの場合には引数として与えられたものを実行する
	 *
	 *  引数の配列は以下を仮定している
	 *  ・第１引数には実行されるプログラムを示す文字列が格納されている
	 *  ・引数の配列はヌルポインタで終了している
	 */

	/******** Your Program ********/

	
	/*
	 *  コマンドの状態がバックグラウンドなら関数を抜ける
	 */

	/******** Your Program ********/

	/*
	 *  ここにくるのはコマンドの状態がフォアグラウンドの場合
	 *
	 *  親プロセスの場合に子プロセスの終了を待つ
	 */

	/******** Your Program ********/
	
	char *args[] = {
		"pwd",
		NULL //C99では末尾にカンマがあってもOK！
	};


	pid = fork();
	if (pid < 0) {
		perror("fork");
		exit(-1);
	}else if(pid == 0){
		// 子プロセス
		printf("child process start\n");
		sleep(10);
		printf("child process end\n");
		return; //子プロセスの終了コードはWEXITSTATUS()で取れる
	}

	// 親プロセス
	printf("parent process start\n");

	pid_t r = waitpid(pid, &status, 0); //子プロセスのプロセスIDを指定して、終了を待つ
	if (r < 0) {
		perror("waitpid");
		exit(-1);
	}
	if (WIFEXITED(status)) {
		// 子プロセスが正常終了の場合
		int exit_code = WEXITSTATUS(status); //子プロセスの終了コード
		printf("child exit-code=%d\n", exit_code);
	} else {
		printf("child status=%04x\n", status);
	}

	printf("parent process end\n");
	

	return;
	
}