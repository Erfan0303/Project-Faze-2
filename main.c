#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct twit{
    int user_id;
    int check;
    char twit[500];
    int reply_counter;
};

struct reply{
    int user_id;
    int check;
    int twit_id;
    char reply[500];
};

struct user {
    char name[100];
    char password[100];
    int twit_id[100];
    int twit_counter;
    int user_id;
};

typedef struct user User;

typedef struct twit Twit;

typedef struct reply Reply;

int id=1;

int reply_id=1;

int id_of_last_user;

int all_users=1;

void new_user(char user_name[],char password[],User* user){
    strcpy((user+all_users)->name,user_name);
    strcpy((user+all_users)->password,password);
    (user+all_users)->twit_counter=0;
    (user+all_users)->user_id=all_users;
    id_of_last_user=all_users;
    FILE* userp = fopen("user.txt", "a");
    FILE* passp = fopen("pass.txt", "a");
    if(userp == 0)
        printf("Error! File could not be opened");
    if(passp == 0)
        printf("Error! File could not be opened");
    fprintf(userp, "%s\n", user_name);
    fprintf(passp, "%s\n", password);
    printf("your account has been created successfully\nyour id is:%d\n",all_users);
    all_users++;
    fclose(userp);
    fclose(passp);
};

void new_twit(Twit* twit,User* user){
    char* messages=malloc(500*sizeof(char));
    printf("please enter your message or if you want to go back to the main menu enter exit\n");
    scanf(" %[^\n]s",messages);
    int result=strcmp(messages,"exit");
        if(result==0){
            return;
        }else{
            printf("do you want to your username be hidden?\n1.yes\n2.no\n");
            int check;
            here_1234:
            scanf("%d",&check);
            strcpy((twit+id)->twit,messages);
            (twit+id)->user_id=id_of_last_user;
            (twit+id)->reply_counter=0;
            if(check==2){
                (twit+id)->check=1;
            }else if(check==1){
                (twit+id)->check=0;
            }else{
                printf("the number is not available please enter again\n");
                goto here_1234;
            }
            (user+id_of_last_user)->twit_id[(user+id_of_last_user)->twit_counter]=id;
            printf("\nyour twit's id is:%d\n",id);
            printf("\nyour message has been successfully sent\n");
            FILE *twitsp,*sendersp,*checkp,*reply_counterp;
            twitsp = fopen("twits.txt", "a");
            sendersp = fopen("senders.txt", "a");
            checkp = fopen("check.txt","a");
            reply_counterp = fopen("reply_counter.txt","a");

            if(twitsp == NULL || sendersp == NULL || checkp == NULL || reply_counterp == NULL)
                printf("Error! File could not be opened");
            fputs(messages,twitsp);
            fputs("\n",twitsp);
            fprintf(sendersp,"%d",id_of_last_user);
            fputs("\n",sendersp);
            fprintf(checkp,"%d\n",check-1);
            fprintf(reply_counterp,"%d\n",0);
            fclose(twitsp);
            fclose(sendersp);
            fclose(checkp);
            fclose(reply_counterp);
            id++;
            (user+id_of_last_user)->twit_counter++;
        }
}

void edit_message(Twit* twit){
    printf("please enter the id of message that you want to edit or if you want to go back to the main menu enter 0\n");
    int i;
    here_2:
    scanf("%d",&i);
    if(i){
        if(i>=id || i<1){
            printf("there is not any message with this id,please enter id again or if you want to go back to the main menu enter 0\n");
            goto here_2;
        }
        if((twit+i)->user_id==id_of_last_user){
            printf("this is your message:%s\nplease enter the message to replace with this message\n",(twit+i)->twit);
            char* messages=malloc(500*sizeof(char));
            scanf(" %[^\n]s",messages);
            strcpy((twit+i)->twit,messages);
            FILE *file,*temp;
            char buffer[100];
            file = fopen("twits.txt", "r");
                temp = fopen("temp_twits.txt", "w");
                if(file == NULL || temp == NULL)
                    printf("Error! File could not be opened");
                for(int j = 1; j<id; j++){
                    fgets(buffer,100,file);
                    if(j == i){
                        fputs(messages,temp);
                        fputs("\n",temp);
                    }
                    else
                        fputs(buffer,temp);
                }
                fclose(file);
                fclose(temp);
                remove("twits.txt");
                rename("temp_twits.txt","twits.txt");
            printf("\nyour message has been successfully edited\n");
        }else{
            printf("you are not allowed to edit this message,please enter the id of a new message\n");
            goto here_2;
        }
    }else{
        return;
    }
}

void view_all_twits(Twit* twit,User* user,Reply* reply){
    if(id==1){
        printf("there is no message");
    }else{
        for(int i=1;i<id;i++){
            if((twit+i)->check){
                printf("%s:\n",(user+((twit+i)->user_id)));
            }
            printf("twit's id:%d\ntwit:%s\n",i,(twit+i)->twit);
            if((twit+i)->reply_counter>=1){
                printf("replies to this message:\n");
                for(int j=1;j<reply_id;j++){
                    if((reply+j)->twit_id==i){
                        if((reply+j)->check){
                            printf("%s:\n",(user+((reply+j)->user_id)));
                        }
                        printf("reply's id:%d\nreply:%s\n*********\n",j,(reply+j)->reply);
                    }
                }
            }
            printf("________________\n");
        }
    }
}

void view_some_twits(Twit* twit,User* user,Reply* reply){
    printf("please enter the id of user that you want to see his/hers twits or if you want to go back to the main menu enter -1\n");
    int z;
    int counter=0;
    here_4:
    scanf("%d",&z);
    if(z>0 && z<all_users+1){
        for(int i=1;i<id;i++){
            if((twit+i)->user_id==z){
                if((twit+i)->check){
                printf("%s:\n",(user+((twit+i)->user_id)));
            }
            printf("twit's id:%d\ntwit:%s\n",i,(twit+i)->twit);
            counter++;
            if((twit+i)->reply_counter>=1){
                printf("replies to this message:\n");
                for(int j=1;j<reply_id;j++){
                    if((reply+j)->twit_id==i){
                        if((reply+j)->check){
                            printf("%s:\n",(user+((reply+j)->user_id)));
                        }
                        printf("reply's id:%d\nreply:%s\n*********\n",j,(reply+j)->reply);
                    }
                }
            }
            printf("________________\n");
            }
        }
    }else if(z<1 || z>=all_users+1){
        printf("there is not any account with that id,please enter the id again\n");
        goto here_4;
    }
    if(counter==0){
        printf("there is no message");
    }
}

void change_password(User* user){
    char password[100];
    printf("\nplease enter your password\n");
    here_3:
    scanf("%s",password);
    int result_1=strcmp(password,(user+id_of_last_user)->password);
        if(result_1==0){
            printf("what do you want to change?\n1.password\n2.username\n");
            int z;
            here_5:
            scanf("%d",&z);
            if(z==1){
                printf("\nplease enter the new paasword\n");
                char new_password[100];
                scanf("%s",new_password);
                strcpy((user+id_of_last_user)->password,new_password);
                FILE *file, *temp;
                char buffer[100];
                file = fopen("pass.txt", "r");
                temp = fopen("temp_pass.txt", "w");
                if(file == NULL || temp == NULL)
                    printf("Error! File could not be opened");
                for(int i = 1; i<all_users; i++){
                    fgets(buffer,100,file);
                    if(i == id_of_last_user){
                        fputs(new_password,temp);
                        fputs("\n",temp);
                    }
                    else
                        fputs(buffer,temp);
                }
                fclose(file);
                fclose(temp);
                remove("pass.txt");
                rename("temp_pass.txt","pass.txt");
                printf("you password has been changed successfully\n");
            }else if(z==2){
                printf("\nplease enter the new username\n");
                char new_username[100];
                here_6:
                scanf("%s",new_username);
                for(int i=1;i<all_users+1;i++){
                    int result=strcmp(new_username,(user+i)->name);
                    int result_2=strcmp(password,(user+i)->password);
                    if(result==0 && result_2!=0){
                        printf("this username has been chosen before by someone else,please enter another username\n");
                        goto here_6;
                    }
                }
                strcpy((user+id_of_last_user)->name,new_username);
                FILE *file, *temp;
                char buffer[100];
                file = fopen("user.txt", "r");
                temp = fopen("temp_user.txt", "w");
                if(file == NULL || temp == NULL)
                    printf("Error! File could not be opened");
                for(int i = 1; i<all_users; i++){
                    fgets(buffer,100,file);
                    if(i == id_of_last_user){
                        fputs(new_username,temp);
                        fputs("\n",temp);
                    }
                    else
                        fputs(buffer,temp);
                }
                fclose(file);
                fclose(temp);
                remove("user.txt");
                rename("temp_user.txt","user.txt");
                printf("you username has been changed successfully\n");
            }else{
                printf("the number that you enter is not available please enter the number again\n");
                goto here_5;
            }
        }else{
            printf("\nthe password is not correct,please enter a new password\n");
            goto here_3;
        }
}

void rep(Twit* twit,Reply* reply){
    printf("please enter the id of message that you want to reply or if for you want to go back to main menu please enter 0\n");
    int a;
    here_100:
    scanf("%d",&a);
    if(a==0){
        return;
    }else if(a<0 || a>=id){
        printf("there is not any message with this id,please enter id again or if you want to go back to the main menu enter 0\n");
        goto here_100;
    }else{
        printf("please enter your message\n");
        char text[500];
        scanf(" %[^\n]s",text);
        strcpy((reply+reply_id)->reply,text);
        (reply+reply_id)->twit_id=a;
        (reply+reply_id)->user_id=id_of_last_user;
        printf("do you want to your username be hidden?\n1.yes\n2.no\n");
        int check;
        here_111:
        scanf("%d",&check);
        if(check==2){
                (reply+reply_id)->check=1;
            }else if(check==1){
                (reply+reply_id)->check=0;
            }else{
                printf("the number is not available please enter again\n");
                goto here_111;
            }
        (twit+a)->reply_counter++;
        FILE *reply_counterp,*temp;
        reply_counterp = fopen ("reply_counter.txt","r");
        temp = fopen("temp_reply.txt","w");
        if(reply_counterp == NULL || temp == NULL)
            printf("Error! File could not be opened");
        int buffer;
        for(int i = 1; i<id; i++){
            fscanf(reply_counterp,"%d",&buffer);
            if(i == a){
                fprintf(temp,"%d\n",(twit+a)->reply_counter);
            }
            else
                fprintf(temp,"%d\n",buffer);
        }
        fclose(reply_counterp);
        fclose(temp);
        remove("reply_counter.txt");
        rename("temp_reply.txt","reply_counter.txt");

        FILE *reply_userp,*reply_checkp,*reply_twitp,*replyp;
        reply_userp = fopen("reply_user.txt","a");
        reply_checkp = fopen("reply_check.txt","a");
        reply_twitp = fopen("reply_twit.txt","a");
        replyp = fopen("reply.txt","a");
        if(reply_checkp == NULL || reply_userp == NULL || reply_twitp == NULL || replyp == NULL)
            printf("Error! File could not be opened");
        fprintf(reply_userp,"%d\n", id_of_last_user);
        fprintf(reply_checkp,"%d\n",check-1);
        fprintf(reply_twitp,"%d\n",a);
        fprintf(replyp,"%s\n", text);
        fclose(reply_userp);
        fclose(reply_checkp);
        fclose(reply_twitp);
        fclose(replyp);

        printf("\nyour message has been successfully sent\n");
        reply_id++;
    }
}

int first_menu(User* user){
    here10:
    printf("Please enter your username or for exit please type 'exit':\n");
    char user_name[100];
    scanf("%s",user_name);
    int result=strcmp(user_name,"exit");
        if(result==0){
            FILE *lastp,*user_twitsp;
            lastp = fopen("last_t_u.txt","r+");
            if(lastp == NULL)
                puts("Error! File could not be opened");
            fprintf(lastp, "%d\n%d\n%d", id, all_users,reply_id);
            user_twitsp = fopen("user_twits.txt","r+");
            for(int i = 1; i<all_users; i++){
                    fprintf(user_twitsp,"%d\n",(user+i)->twit_counter);
                for(int j = 0; j<(user+i)->twit_counter; j++){
                    fprintf(user_twitsp,"%d\n",(user+i)->twit_id[j]);
                }
            }
            fclose(lastp);
            fclose(user_twitsp);
            return 1;
        }
    printf("please enter your password:\n");
    char password[100];
    scanf("%s",password);
    printf("Please enter the number of action:\n1.Login\n2.Register\n");
    int caller;
    scanf("%d",&caller);
    int check=0;
    if(caller == 1){
        for(int i=1;i<all_users;i++){
            int result=strcmp(user_name,(user+i)->name);
            if(result==0){
                check=1;
                int result_1=strcmp(password,(user+i)->password);
                if(result_1==0){
                    id_of_last_user=i;
                    printf("Welcome back %s\n",(user+id_of_last_user)->name);
                }else{
                    printf("Username and password don't match!\n");
                    goto here10;
                }
                break;
            }
        }
        if(check == 0){
            printf("No username matches '%s'\n", user_name);
            goto here10;
    }
}
    printf("");
    if(caller == 2){
        for(int i=1;i<=all_users+1;i++){
            int result=strcmp(user_name,(user+i)->name);
            if(result==0){
                check=1;
            }
        }
        if(check == 1){
            printf("Username '%s' already exists!\n", user_name);
            goto here10;
        }else if(check == 0)
            new_user(user_name,password,user);
    }
    return 0;
 }

int control(User* user,Twit* twit,Reply* reply){
    printf("\nplease enter the number of menu:\n1.send message\n2.edit message\n3.view all messages\n4.search people\n5.change username/password\n6.reply messages\n7.logout\n");
    int caller;
    here_1:
    scanf("%d",&caller);
    if(caller==1){
        new_twit(twit,user);
        return 0;
    }else if(caller==2){
        edit_message(twit);
        return 0;
    }else if(caller==3){
        view_all_twits(twit,user,reply);
        return 0;
    }else if(caller==4){
        view_some_twits(twit,user,reply);
        return 0;
    }else if(caller==5){
        change_password(user);
        return 0;
    }else if(caller==6){
        rep(twit,reply);
        return 0;
    }else if(caller==7){
        int i=first_menu(user);
        return i;
    }else if(caller>7 || caller<1){
        printf("\nthe number of menu is not available,please enter the number again\n");
        goto here_1;
    }
 }

int main()
{
    printf("Welcome to Twitter!\n\n");
    User *user=malloc(100*sizeof(User));
    Twit *twit=malloc(100*sizeof(Twit));
    Reply *reply=malloc(100*sizeof(Reply));
    FILE *lastp,*passp,*userp,*sendersp,*twitsp,*user_twitsp,*checkp,*reply_counterp;
    lastp = fopen("last_t_u.txt", "r");
    passp = fopen("pass.txt", "r");
    userp = fopen("user.txt", "r");
    sendersp = fopen("senders.txt", "r");
    twitsp = fopen("twits.txt", "r");
    user_twitsp = fopen("user_twits.txt", "r");
    checkp = fopen("check.txt", "r");
    reply_counterp = fopen("reply_counter.txt","r");
    if(lastp == 0 || passp == 0 || userp == 0 || sendersp == 0 || twitsp == 0 || user_twitsp == 0)
        printf("Error! File could not be opened");
    fscanf(lastp,"%d",&id);
    fscanf(lastp,"%d",&all_users);
    fscanf(lastp,"%d",&reply_id);
    for(int i = 1; i<all_users; i++){
        fscanf(userp,"%s",(user+i)->name);
        fscanf(passp,"%s",(user+i)->password);
        (user+i)->user_id = i;
        fscanf(user_twitsp,"%d",&(user+i)->twit_counter);
        for(int j = 0; j<(user+i)->twit_counter; j++)
            fscanf(user_twitsp,"%d", &(user+i)->twit_id[j]);
    }
    for(int i = 1; i<id; i++){
        fgets((twit+i)->twit, 100, twitsp);
        fscanf(sendersp,"%d", &(twit+i)->user_id);
        fscanf(checkp,"%d",&(twit+i)->check);
        fscanf(reply_counterp,"%d", &(twit+i)->reply_counter);
    }
    fclose(userp);
    fclose(passp);
    fclose(twitsp);
    fclose(sendersp);
    fclose(user_twitsp);
    fclose(lastp);
    fclose(checkp);
    fclose(reply_counterp);

    FILE *reply_userp,*reply_checkp,*reply_twitp,*replyp;
        reply_userp = fopen("reply_user.txt","r");
        reply_checkp = fopen("reply_check.txt","r");
        reply_twitp = fopen("reply_twit.txt","r");
        replyp = fopen("reply.txt","r");
        if(reply_checkp == NULL || reply_userp == NULL || reply_twitp == NULL || replyp == NULL)
            printf("Error! File could not be opened");
        for(int i = 1; i<reply_id; i++){
            fscanf(reply_userp,"%d", &(reply+i)->user_id);
            fscanf(reply_checkp,"%d", &(reply+i)->check);
            fscanf(reply_twitp,"%d", &(reply+i)->twit_id);
            fgets((reply+i)->reply, 100, replyp);
        }
        fclose(reply_userp);
        fclose(reply_checkp);
        fclose(reply_twitp);
        fclose(replyp);
    first_menu(user);
    while(1){
        int end=control(user,twit,reply);
        if(end)
            break;
    }
    return 0;
}

