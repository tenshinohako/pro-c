/*
 *  �C���N���[�h�t�@�C��
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <dirent.h>

/*
 *  �萔�̒�`
 */

#define BUFLEN	1024	 /* �R�}���h�p�̃o�b�t�@�̑傫�� */
#define MAXARGNUM  256	 /* �ő�̈����̐� */

#define MAXHISTORY 32
#define PATHLEN 256
#define PROMPTLEN 32
#define COMMANDLEN 16

/*
 *	�\���̂̐錾
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
 *  ���[�J���v���g�^�C�v�錾
 */

int parse(char [], char *[]);
void execute_command(char *[], int);

int execute_alias(ALIAS_BINARY *, char *, char *);
void delete_alias(ALIAS_BINARY **, char *);
void show_alias(ALIAS_BINARY *);
void delete_binary(ALIAS_BINARY *);

/*----------------------------------------------------------------------------
 *
 *  �֐���   : main
 *
 *  ��Ɠ��e : �V�F���̃v�����v�g����������
 *
 *  ����	 :
 *
 *  �Ԃ�l   :
 *
 *  ����	 :
 *
 *--------------------------------------------------------------------------*/

int main(int argc, char *argv[])
{
	char command_buffer[BUFLEN]; /* �R�}���h�p�̃o�b�t�@ */
	char *args[MAXARGNUM];	   /* �����ւ̃|�C���^�̔z�� */
	int command_status;		  /* �R�}���h�̏�Ԃ�\��
									command_status = 0 : �t�H�A�O���E���h�Ŏ��s
									command_status = 1 : �o�b�N�O���E���h�Ŏ��s
									command_status = 2 : �V�F���̏I��
									command_status = 3 : �������Ȃ� */
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
	 *  �����Ƀ��[�v����
	 */

	for(;;) {
		/*
		 *  �v�����v�g��\������
		 */

		if(prev == 0){
			if(isatty(fileno(stdin)))
				printf("\n%s : ", prompt);

		/*
		 *  �W�����͂���P�s�� command_buffer �֓ǂݍ���
		 *  ���͂������Ȃ���Ή��s���o�͂��ăv�����v�g�\���֖߂�
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
		 *  ���͂��ꂽ�o�b�t�@���̃R�}���h����͂���
		 *
		 *  �Ԃ�l�̓R�}���h�̏��
		 */

		command_status = parse(command_buffer, args);

		/*
		 *  �I���R�}���h�Ȃ�΃v���O�������I��
		 *  �����������Ȃ���΃v�����v�g�\���֖߂�
		 */

		if(command_status == 2) {
			access = head;					//�X�^�b�N�e�ʂ��J����
			while(access != NULL){
				access = access->next;
				free(head);
				head = access;
			}								//�X�^�b�N�e�ʂ��J����
			delete_binary(a_head);	//�񕪖؊J��
			printf("done.\n");
			exit(EXIT_SUCCESS);
		} else if(command_status == 3) {
			continue;
		}

		/*
		 *  �R�}���h�����s����
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
 *  �֐���   : parse
 *
 *  ��Ɠ��e : �o�b�t�@���̃R�}���h�ƈ�������͂���
 *
 *  ����	 :
 *
 *  �Ԃ�l   : �R�}���h�̏�Ԃ�\�� :
 *				0 : �t�H�A�O���E���h�Ŏ��s
 *				1 : �o�b�N�O���E���h�Ŏ��s
 *				2 : �V�F���̏I��
 *				3 : �������Ȃ�
 *
 *  ����	 :
 *
 *--------------------------------------------------------------------------*/

int parse(char buffer[],		/* �o�b�t�@ */
		  char *args[])			/* �����ւ̃|�C���^�z�� */
{
	int arg_index;   /* �����p�̃C���f�b�N�X */
	int status;   /* �R�}���h�̏�Ԃ�\�� */
	
	DIR *dir;
	struct dirent *dp;
	/*
	 *  �ϐ��̏�����
	 */

	arg_index = 0;
	status = 0;

	/*
	 *  �o�b�t�@���̍Ō�ɂ�����s���k�������֕ύX
	 */

	//*(buffer + (strlen(buffer) - 1)) = '\0';

	/*
	 *  �o�b�t�@���I����\���R�}���h�i"exit"�j�Ȃ��
	 *  �R�}���h�̏�Ԃ�\���Ԃ�l�� 2 �ɐݒ肵�ă��^�[������
	 */

	if(strcmp(buffer, "exit") == 0) {
		status = 2;
		return status;
	}
	
	/*
	 *  �o�b�t�@���̕������Ȃ��Ȃ�܂ŌJ��Ԃ�
	 *  �i�k���������o�Ă���܂ŌJ��Ԃ��j
	 */

	while(*buffer != '\0') {

		/*
		 *  �󔒗ށi�󔒂ƃ^�u�j���k�������ɒu��������
		 *  ����ɂ���ăo�b�t�@���̊e���������������
		 */

		while(*buffer == ' ' || *buffer == '\t') {
			*(buffer++) = '\0';
		}

		/*
		 * �󔒂̌オ�I�[�����ł���΃��[�v�𔲂���
		 */

		if(*buffer == '\0') {
			break;
		}

		/*
		 *  �󔒕����͓ǂݔ�΂��ꂽ�͂�
		 *  buffer �͌��݂� arg_index + 1 �߂̈����̐擪���w���Ă���
		 *
		 *  �����̐擪�ւ̃|�C���^�������ւ̃|�C���^�z��Ɋi�[����
		 */

		args[arg_index] = buffer;
		++arg_index;

		/*
		 *  ����������ǂݔ�΂�
		 *  �i�k�������ł��󔒗ނł��Ȃ��ꍇ�ɓǂݐi�߂�j
		 */

		while((*buffer != '\0') && (*buffer != ' ') && (*buffer != '\t')) {
			++buffer;
		}
	}

	/*
	 *  �Ō�̈����̎��ɂ̓k���ւ̃|�C���^���i�[����
	 */

	args[arg_index] = NULL;

	/*
	 *  �Ō�̈������`�F�b�N���� "&" �Ȃ��
	 *
	 *  "&" ������������
	 *  �R�}���h�̏�Ԃ�\�� status �� 1 ��ݒ肷��
	 *
	 *  �����łȂ���� status �� 0 ��ݒ肷��
	 */

	if(arg_index > 0 && strcmp(args[arg_index - 1], "&") == 0) {
		--arg_index;
		args[arg_index] = '\0';
		status = 1;
	} else {
		status = 0;
	}

	/*
	 *  �����������Ȃ������ꍇ
	 */

	if(arg_index == 0) {
		status = 3;
	}
	
	if(arg_index > 0 && strcmp(args[arg_index - 1], "*") == 0){	//���C���h�J�[�h�@�\
		arg_index--;
		dir = opendir(".");
		while ((dp = readdir(dir)) != NULL) {
			args[arg_index] = dp->d_name;
			arg_index++;
		}
		args[arg_index] = NULL;
		closedir(dir);
	}															//���C���h�J�[�h�@�\
	
	/*
	 *  �R�}���h�̏�Ԃ�Ԃ�
	 */

	return status;
}

/*----------------------------------------------------------------------------
 *
 *  �֐���   : execute_command
 *
 *  ��Ɠ��e : �����Ƃ��ė^����ꂽ�R�}���h�����s����
 *			 �R�}���h�̏�Ԃ��t�H�A�O���E���h�Ȃ�΁A�R�}���h��
 *			 ���s���Ă���q�v���Z�X�̏I����҂�
 *			 �o�b�N�O���E���h�Ȃ�Ύq�v���Z�X�̏I����҂�����
 *			 main �֐��ɕԂ�i�v�����v�g�\���ɖ߂�j
 *
 *  ����	 :
 *
 *  �Ԃ�l   :
 *
 *  ����	 :
 *
 *--------------------------------------------------------------------------*/

void execute_command(char *args[],	/* �����̔z�� */
					 int command_status)	 /* �R�}���h�̏�� */
{
	int pid;	  /* �v���Z�X�h�c */
	int status;   /* �q�v���Z�X�̏I���X�e�[�^�X */
	
	/*
	 *  �q�v���Z�X�𐶐�����
	 *
	 *  �����ł��������m�F���A���s�Ȃ�΃v���O�������I������
	 */

	/******** Your Program ********/
	pid = fork();
	if (pid < 0){
		printf("fork failure\n");
		exit(EXIT_FAILURE);
	}

	/*
	 *  �q�v���Z�X�̏ꍇ�ɂ͈����Ƃ��ė^����ꂽ���̂����s����
	 *
	 *  �����̔z��͈ȉ������肵�Ă���
	 *  �E��P�����ɂ͎��s�����v���O���������������񂪊i�[����Ă���
	 *  �E�����̔z��̓k���|�C���^�ŏI�����Ă���
	 */

	/******** Your Program ********/
	else if(pid == 0){
		execvp(args[0],args);
		exit(EXIT_SUCCESS); 
	}

	/*
	 *  �R�}���h�̏�Ԃ��o�b�N�O���E���h�Ȃ�֐��𔲂���
	 */

	/******** Your Program ********/
	else{
		if(command_status == 1)
			return;

	/*
	 *  �����ɂ���̂̓R�}���h�̏�Ԃ��t�H�A�O���E���h�̏ꍇ
	 *
	 *  �e�v���Z�X�̏ꍇ�Ɏq�v���Z�X�̏I����҂�
	 */

	/******** Your Program ********/
		int r = waitpid(pid, &status, 0); //�q�v���Z�X�̃v���Z�XID���w�肵�āA�I����҂�
		if(r < 0){
			printf("wait failure\n");
			exit(EXIT_FAILURE);
		}
	}
	
	return;
}
/*-- END OF FILE -----------------------------------------------------------*/