#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <readline/readline.h>


#define BUFFER_LEN 1024

#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_COLOR_BLUE    "\x1b[34m"


char *redirectout=">";
char *redirectin="<";
char *pipesymbol="|";

typedef struct aliasstruct{
	char aliasname[20];
	char refersto[20];

} alias;

alias aliasstruct[100];
int aliascount=0;
int wildcardcalled=0;

void handlewc(char *line){

	wildcardcalled=1;
	char *handletoken= strtok(line, " ");
	handletoken= strtok(NULL, " ");
	//printf("printing wildcard token : %s\n", handletoken);
	int c=0;
	char tomatch[20];
	int m=0;
	
	int i=0;
	//printf("length of handletoken %ld\n", strlen(handletoken));
	strncpy(tomatch, handletoken, strlen(handletoken)-1);
	tomatch[strlen(handletoken)-1]='\0';

	//printf("printing just the matching part : %s\n", tomatch);
	
	char buffer[1024];
	FILE *stream=popen("ls","r");
	while(fgets(buffer, 1024, stream)!= NULL){
		int n=0, matchflag=0;

		//printf("buffer : %s\n", buffer);
		char buffercopy[20];
		strcpy(buffercopy, buffer);
		for(n=0; n< strlen(tomatch); n++){
			//printf("tomatch[%d] = %c : buffercopy[%d] : %c\n", n, tomatch[n], n, buffercopy[n]);
			if(tomatch[n]!=buffercopy[n]){
				matchflag=1;
			}
		
		}

		if(matchflag==0){
			printf("%s", buffer);
		}


	}

	pclose(stream);


}

void registeralias(char *line){

	char *token;                  //split command into separate strings
    char outputhere[20]="";
    
    int i=0;
    int flag=1;
    char *argv[100];
    int argc;
  
    token = strtok(line," ");
    token=strtok(NULL, " ");
    strcpy(aliasstruct[aliascount].aliasname, token);
    printf("printing the first stuff : %s\n", token);
    token=strtok(NULL, " ");
    printf("printing the second stuff : %s\n", token);
    strcpy(aliasstruct[aliascount].refersto, token);
    aliascount++;
    return ;
   
}





void pipethings(char *line){

	char *token;                  //split command into separate strings
    char outputhere[20]="";
    
    int i=0,j=0;
    int flag=1;
    char *argv[100];
    char *argv1[100];
    int argc;
    char* path= "/bin/";    //set path at bin
    char progpath[20];      //full file path
    printf("just printing line : %s\n", line);
    token = strtok(line," ");
    
    printf("outside token %s\n", token);
    while(token!=NULL){

    	printf("printing token inside pipes while : %s\n", token);

    	if(strcmp(token,pipesymbol)!=0 && flag==1){

    		argv[i]=token;
    		printf("printing argv[%d] before getting |: %s\n", i, argv[i]);
    		token=strtok(NULL," ");
    		printf("printing next token before getting | : %s\n", token);
    		i++;
    	}

    	else if(strcmp(token,pipesymbol)!=0 && flag==0){
    		/*printf("comes inside the other if\n" );
    		strcat(outputhere,token);
    		printf("printing outputhere : %s\n", outputhere);
    		*/
    		argv1[j]=token;
    		printf("printing argv1[%d] after finding |: %s\n", i, argv1[j]);
    		token=strtok(NULL," ");
    		printf("printing next token after finding | %s\n", token);
    		j++;

    	}
    	else{
    		printf("inside else \n" );
    		flag=0;
    		token=strtok(NULL," ");
    		printf("token in the else part %s\n", token);
    		/*
    		token[strlen(token)]='\0';
    		strcat(outputhere,token);
    		printf("printing outputhere : %s\n", outputhere);
    		break;
    		*/


    	}

    	


	}

	argv[i]=NULL;
	argv1[j]=NULL;
	int a;
	printf("just before exiting : i : %d j : %d", i, j);
	for(a=0; a<i;a ++){
		printf("argv[%d] : %s\n ", a, argv[a]);
	}

	for(a=0; a<j;a ++){
		printf("argv1[%d] : %s\n ", a, argv1[a]);
	}
	printf("done here\n");

	int pipefd[2];
	int pid;
	pipe(pipefd);

	//pid=fork();
	
	int pid1=fork();


	//if(pid==0){
	if(pid1==0){

			close(1);
			dup(pipefd[1]);
			close(pipefd[0]);
			//dup2(pipefd[0], 0);
			//close(pipefd[1]);
			execvp(argv[0], argv);
		}

	else{
			wait(0);
			close(0);
			dup(pipefd[0]);
			close(pipefd[1]);
			//dup2(pipefd[1], 1);
			//close(pipefd[0]);
			execvp(argv1[0], argv1);
		}

	

	//}

	/*else{                    //Parent
        wait(NULL);
        printf("Child exited\n");
    }
	*/

}

//char **a[100] = {argv, argv1};
//a[i][0], a[1]

void redirectfunc(char *line){

	char *token;                  //split command into separate strings
    char outputhere[20]="";
    
    int i=0;
    int flag=1;
    char *argv[100];
    int argc;
    char* path= "/bin/";    //set path at bin
    char progpath[20];      //full file path
    //printf("just printing line : %s\n", line);
    token = strtok(line," ");
    
    //printf("outside token %s\n", token);
    while(token!=NULL){
    	//printf("printing token inside redirectfunc : %s\n", token);
    	if(strcmp(token,redirectout)!=0 && flag==1){

    		argv[i]=token;
    		//printf("printing argv[%d] inside redirectfunc : %s\n", i, argv[i]);
    		token=strtok(NULL," ");
    		//printf("printing token inside if : %s\n", token);
    		i++;
    	}
    	else{
    		//printf("comes inside else\n" );
    		flag=0;
    		token=strtok(NULL," ");
    		//printf("token in the else part %s\n", token);
    		token[strlen(token)]='\0';
    		strcat(outputhere,token);
    		//printf("printing outputhere : %s\n", outputhere);
    		break;


    	}

    	/*if(strcmp(token,redirectout)!=0 && flag==0){
    		printf("comes inside the other if\n" );
    		strcat(outputhere,token);
    		printf("printing outputhere : %s\n", outputhere);

    	}*/
    
    }
    argv[i]=NULL;                     //set last value to NULL for execvp
    argc=i; 	

    strcpy(progpath, path);           //copy /bin/ to file path
    strcat(progpath, argv[0]);            //add program to path
    
    for(i=0; i<strlen(progpath); i++){    //delete newline
        if(progpath[i]=='\n'){      
            progpath[i]='\0';
        }
	}

	//close(1);

	FILE *f=fopen(outputhere,"w");
	int fd=fileno(f);
	int pid=fork();

	if(pid==0){               //Child
        //execvp(progpath,argv);
        dup2(fd,1);
        execvp(argv[0],argv);

        fprintf(stderr, "Child process could not do execvp----Invalid----\n");
        //wait(NULL);
    }else{                    //Parent
        wait(NULL);
        printf("Child exited\n");
    }
    strcat(outputhere,"");
    token="";


}

void inpredirectfunc(char *line){

	char *token;                  //split command into separate strings
    char outputhere[20]="";
    
    int i=0;
    int flag=1;
    char *argv[100];
    int argc;
    char* path= "/bin/";    //set path at bin
    char progpath[20];      //full file path
    //printf("just printing line in inputredirect : %s\n", line);
    token = strtok(line," ");
    
    //printf("outside token %s\n", token);
    while(token!=NULL){
    	///printf("printing token inside redirectfunc : %s\n", token);
    	if(strcmp(token,redirectin)!=0 && flag==1){

    		argv[i]=token;
    		//printf("printing argv[%d] inside inpredirectfunc : %s\n", i, argv[i]);
    		token=strtok(NULL," ");
    		//printf("printing token inside if : %s\n", token);
    		i++;
    	}
    	else{
    		//printf("comes inside else\n" );
    		flag=0;
    		token=strtok(NULL," ");
    		//printf("token in the else part %s\n", token);
    		token[strlen(token)]='\0';
    		strcat(outputhere,token);
    		printf("printing outputhere : %s\n", outputhere);
    		break;


    	}

    	/*if(strcmp(token,redirectout)!=0 && flag==0){
    		printf("comes inside the other if\n" );
    		strcat(outputhere,token);
    		printf("printing outputhere : %s\n", outputhere);

    	}*/
    
    }
    argv[i]=NULL;                     //set last value to NULL for execvp
    argc=i; 	

    strcpy(progpath, path);           //copy /bin/ to file path
    strcat(progpath, argv[0]);            //add program to path
    
    for(i=0; i<strlen(progpath); i++){    //delete newline
        if(progpath[i]=='\n'){      
            progpath[i]='\0';
        }
	}

	//close(1);

	FILE *f=fopen(outputhere,"r");
	int fd=fileno(f);
	int pid=fork();

        
	if(pid==0){               //Child
        //execvp(progpath,argv);
        dup2(fd, 0);
        close(fd);
        execvp(argv[0],argv);

        fprintf(stderr, "Child process could not do execvp----Invalid----\n");
        //wait(NULL);
    }else{                    //Parent
        wait(NULL);
        printf("Child exited\n");
    }
    strcat(outputhere,"");
    token="";


}

void history(char *cmd)// adding history to the history.txt file
{
    int fd,wr;
    time_t t;
    time(&t);
    fd=open("/home/adj/usp_project/shell_project/history.txt",O_RDWR|O_CREAT|O_APPEND,0644);// full path must be given
    if(fd <0)
    {
        printf("file open error");
    }
    //dprintf(fd,"%s%s",ctime(&t),cmd);    
    //dprintf(fd,"%s",cmd);
	char time[100];
	strcpy(time,ctime(&t));
	char command[100];
	strcpy(command,cmd);
	strcat(command," ");
	strcat(command,time);
	char pid[100];
	sprintf(pid," %d",getpid());
	strcat(command,pid);
	strcat(command,"\n");
	dprintf(fd," %s",command);
} 

void read_history()
{
    int fd1;
    char buf[1];
    int bytes_read;
    fd1=open("/home/adj/usp_project/shell_project/history.txt",O_RDONLY,0644);
    if(fd1 <0)
    {
        printf("file open error");
    }
    do
    {
        bytes_read=read(fd1,buf,1*sizeof(char));
        printf("%s",buf);
    }while(bytes_read !=0);

}

//char ESC=27;
  
  

int main(){
    char *line;  //get command line
    char* argv[100];
    char cmd[1000];        //user command
    char* path= "/bin/";    //set path at bin
    char progpath[20];      //full file path
    int argc;               //arg count
    int length;
    int res,res1;
    char* input, shell_prompt[100];    
    rl_bind_key('\t', rl_complete);
    while(1){
    	char cwd[1024];
		char esc=27;
    	getcwd(cwd, sizeof(cwd));
    	//printf("shell:-%s$",cwd);                    //print shell prompt
   		//printf(ANSI_COLOR_MAGENTA "shell:-%s$" ANSI_COLOR_RESET, cwd);
   		//printf("%c[0m",ESC); /* turn off bold */
		snprintf(shell_prompt, sizeof(shell_prompt),ANSI_COLOR_MAGENTA "shell:-%s$" ANSI_COLOR_RESET,cwd);
     	printf("%c[1m",esc);  /*- turn on bold */
		//if(!fgets(line, BUFFER_LEN, stdin)){  //get command and put it in line
       	//	break;                                //if user hits CTRL+D break
   		//}
		line = readline(shell_prompt);
    //printf("%s",line);
   		history(line);
    	length = strlen(line);
    	if (line[length - 1] == '\n')
    	{
        	line[length - 1] = '\0';
    	}

    	if(strcmp(line, "exit")==0){            //check if command is exit
        	break; // if exit then break from loop
   		 }
   		char send[20];
   		char handleline[20];
   		strcat(handleline, line);
   		strcat(send, line);
   		send[strlen(send)]='\0';
   		//printf("printing send in the main function : %s\n", send);
    	char *token;                  //split command into separate strings
    	//printf("printing line in the main function : %s\n", line);
    	token = strtok(line," ");
    	//printf("printing line again in the main function : %s\n", line);
    	//printf("printing token in the main function : %s\n", token);
   		int i=0;
   		
    	while(token!=NULL){
        	argv[i]=token;   
        	if(strcmp(argv[i],redirectout)==0){
        		//printf("goes in!!\n");
        		//printf("printing send inside while : %s\n", send);
        		redirectfunc(send);
        	}

        	if(strcmp(argv[i],redirectin)==0){
        		//printf("goes inside input redirection!!\n");
        		//printf("printing send inside while : %s\n", send);
        		inpredirectfunc(send);
        	}

        	if(strcmp(argv[i],pipesymbol)==0){
        		//printf("for pipe!!\n");
        		//printf("printing send inside while : %s\n", send);
        		pipethings(send);
        	}
        	
        	if(strcmp(argv[i],"alias")==0){
        		//printf("alias command encountered\n");
        		registeralias(send);

        	}
        	
        	//eprintf("argv[%d] : %s\n", i, argv[i]);   
        	token = strtok(NULL," ");
        	char handlecheck='-';
        	if(strcmp(argv[i], "ls")==0){
        		//printf("comes here look!!\n");
        		int c=0, goflag=0;
        		//printf("line : %s\n", line);
        		for(c=0; c< strlen(handleline); c++){
        			//printf("line[%d] : %c\n", c, handleline[c]);
        			if(handleline[c]==handlecheck){
        				goflag=1;
        				break;
        			}

        		}
        		//printf("printing flag at this moment : %d\n", goflag);
        		if(goflag==0 && strlen(handleline)>2){
        			//printf("comes inside goflag stuff");
        			handlewc(handleline);
        		}
        	}
        	
        	
        	//printf("token : %s\n", token);
        	i++;
   			
   		}
    			
    	argv[i]=NULL;                     //set last value to NULL for execvp
    	argc=i;                           //get arg count
    	strcpy(progpath, path);           //copy /bin/ to file path
    	strcat(progpath, argv[0]);            //add program to path
    	//printf("%s\n",argv[argc-1]);
    	res = strcmp(argv[0],"cd");
   		res1 =strcmp(argv[0],"history");
   		int res2=strcmp(argv[0],"editor");
   		int res3=strcmp(argv[0],"sgown");
   		int res4=strcmp(argv[0],"cpown");
   		int res5=strcmp(argv[0],"patpos");
		int res6=strcmp(argv[0],"rmempty");
		int res7=strcmp(argv[0],"readrvs");




   		int k=0;
   		char *argss=(char *)malloc(20);
   		argss=argv[0];
   		for(k=0; k< aliascount; k++){
   			if(strcmp(aliasstruct[k].aliasname, argv[0])==0){
   				strcpy(argss, aliasstruct[k].refersto);
   				break;
   			}
   			else{
   				argss=argv[0];
   			}
   		}



    	if(res==0)
    	{
       	 //printf("cd is not implemented as an executable command\n");
        	chdir(argv[argc-1]);
        	//printf("%s$",argv[argc-1]);
    	}
    	else if(res1==0)
    	{
        	//printf("history called\n");
        	read_history();
    	}

    	else if(res2==0)
		{
			if (fork() == 0)
			{ //this part will only be executed by the child process
				char *argg[]={};
				//system("gcc editor.c -w -o editr");
				//execv("/root/Desktop/usp_class/usp_proj/project2/editor", argg);    
				system("/home/adj/usp_project/shell_project/./editor"); 
			}
			wait(NULL); // parent waits untill child execution is compleated.	
		}

		else if(res3==0)// sgown
		{//search for a given word in file/directory etc.
			char * word=argv[1];
			char* loc=argv[2];
			//system("./sgown.sh word loc");
			int argcount = 2;
			const char* args[] = {word,loc};
			const char* binary_name = "/home/adj/usp_project/shell_project/./sgown.sh";
			char myoutput_array[5000];

			sprintf(myoutput_array, "%s", binary_name);
			for(int i = 0; i < argcount; ++i)
			{
				strcat(myoutput_array, " ");
				strcat(myoutput_array, args[i]);
			}
			system(myoutput_array);
		
		}
		else if(res4==0)// cpown
		{//count the no of matches of a the pattern in a file/folder etc.
			char * word=argv[1];
			char* loc=argv[2];
			//system("./sgown.sh word loc");
			int argcount = 2;
			const char* args[] = {word,loc};
			const char* binary_name = "/home/adj/usp_project/shell_project/./cpown.sh";
			char myoutput_array[5000];

			sprintf(myoutput_array, "%s", binary_name);
			for(int i = 0; i < argcount; ++i)
			{
				strcat(myoutput_array, " ");
				strcat(myoutput_array, args[i]);
			}
			system(myoutput_array);
		
		}
		else if(res5==0)// patpos
		{//Show the position of match in the line
			char * word=argv[1];
			char* loc=argv[2];
			//system("./sgown.sh word loc");
			int argcount = 2;
			const char* args[] = {word,loc};
			const char* binary_name = "/home/adj/usp_project/shell_project/./patpos.sh";
			char myoutput_array[5000];

			sprintf(myoutput_array, "%s", binary_name);
			for(int i = 0; i < argcount; ++i)
			{
				strcat(myoutput_array, " ");
				strcat(myoutput_array, args[i]);
			}
			system(myoutput_array);
		
		}
		else if(res6==0)//rmempty
		{
			char * direc=argv[1];
			//system("./rmempty");
			int argcount = 1;
			const char* args[] = {direc};
			const char* binary_name = "/home/adj/usp_project/shell_project/./rmempty.sh";
			char myoutput_array[5000];

			sprintf(myoutput_array, "%s", binary_name);
			for(int i = 0; i < 1; ++i)
			{
				strcat(myoutput_array, " ");
				strcat(myoutput_array, args[i]);
			}
			system(myoutput_array);
		}
		else if(res7==0)//readrvs
		{
			char * word=argv[1];
			char* loc=argv[2];
			//system("./sgown.sh word loc");
			int argcount = 2;
			const char* args[] = {word,loc};
			const char* binary_name = "/home/adj/usp_project/shell_project/./read_rvs.sh";
			char myoutput_array[5000];

			sprintf(myoutput_array, "%s", binary_name);
			for(int i = 0; i < argcount; ++i)
			{
				strcat(myoutput_array, " ");
				strcat(myoutput_array, args[i]);
			}
			system(myoutput_array);
		} 
    	else
    	{
    		for(i=0; i<strlen(progpath); i++){    //delete newline
        		if(progpath[i]=='\n'){      
            		progpath[i]='\0';
       			}
    		}


    		int pid= fork();              //fork child

    		if(pid==0){               //Child
       	 //execvp(progpath,argv);
       			execvp(argss,argv);
        		fprintf(stderr, "Child process could not do execvp----Invalid----\n");
        	//wait(NULL);
    		}else{                    //Parent
        		wait(NULL);
        		printf("Child exited\n");
   			}
    	}
    	strcpy(line,"");
    	strcpy(send,"");
    	strcpy(handleline,"");

	}

}


