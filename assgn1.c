
/*
 SE 3BB4, assignment 1
 student name: Jason Paolasini
 student number: 1162915
 file assgn1.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>

#include "log.h"

#define high    0
#define medium  1
#define low     2

#define hpq_times   2
#define mpq_times   4

int logindex = 0;
int* logi = &logindex;

pid_t create_job(int i);
void siga_handler();   // handler for signal SIGALRM
void sigc_handler();   // handler for signal SIGCHLD
sigset_t mymask1;      // normal signal process mask when all signals are free
// but SIGALRM and SIGCHLD are blocked
sigset_t mymask2;      // special signal process mask when all signals are free
sigset_t jobmask;      // job signal process mask blocking all signals, leaving
// only SIGUSR2 unblocked
struct sigaction sa_alarm;  // disposition for SIGALRM
struct sigaction sa_chld;   // disposition for SIGCHLD



struct jobAttr{
    int jobNumber;
    int priority;
    int PIDi;
    int number_of_runs;
};



int number_of_jobs;

//Queue to handle jobs Each queue has their own Struct of nodes

struct hiNode
{
    struct jobAttr hiData;
    struct hiNode* hinext;
    
}*hiback, *hifront;


void hideQueue()
{
    struct hiNode *hitemp, *hivar = hiback;
    
    if (hivar==hiback)
    {
        hiback = hiback->hinext;
        free(hivar);
        
    }
    else
        printf("\n The High-Queue is Empty");
}

//Push Node onto Queue

void hipush(struct jobAttr hiPIDVal)
{
    struct hiNode *hitemp;
    hitemp = (struct hiNode *)malloc(sizeof(struct hiNode));
    hitemp->hiData = hiPIDVal;
    
    if (hifront == NULL)
    {
        hifront = hitemp;
        hifront->hinext =NULL;
        hiback = hifront;
    }
    else
    {
        hifront->hinext = hitemp;
        hifront= hitemp;
        hifront->hinext=NULL;
    }
    
}

//Print all values of Queue

int hiprintQueue(){
    struct hiNode *hivar=hiback;
    if(hivar!=NULL)
    {
        printf("\n High Jobs in Queue are: ");
        while(hivar!=NULL)
        {
            printf("\n%d,%d,%d,%d",hivar->hiData.jobNumber,hivar->hiData.priority,hivar->hiData.PIDi,hivar->hiData.number_of_runs);
            hivar = hivar->hinext;
        }
        printf("\n");
        return 1;
        
    }
    else{
        printf("\n The High-Queue was empty");
    return 0;
    }
}

//Check if queue is Empty

int hiisEmpty(){
    struct hiNode *hivar=hiback;
    if(hivar!=NULL)
    {
        while(hivar!=NULL)
        {
            hivar = hivar->hinext;
        }
        return 1;
        
    }
    else{
        return 0;
    }
    
}


//Repeats for Lo and Med Queues

struct loNode
{
    struct jobAttr loData;
    struct loNode* lonext;
    
}*loback, *lofront;


void lodeQueue()
{
    struct loNode *lotemp, *lovar = loback;
    
    if (lovar==loback)
    {
        loback = loback->lonext;
        free(lovar);
        
    }
    else
        printf("\n The low-Queue is Empty");
}

void lopush(struct jobAttr loPIDVal)
{
    struct loNode *lotemp;
    lotemp = (struct loNode *)malloc(sizeof(struct loNode));
    lotemp->loData = loPIDVal;
    
    if (lofront == NULL)
    {
        lofront = lotemp;
        lofront->lonext =NULL;
        loback = lofront;
    }
    else
    {
        lofront->lonext = lotemp;
        lofront= lotemp;
        lofront->lonext=NULL;
    }
    
}
int loisEmpty(){
    struct loNode *lovar=loback;
    if(lovar!=NULL)
    {
        while(lovar!=NULL)
        {
            lovar = lovar->lonext;
        }
        return 1;
        
    }
    else{
        return 0;
    }
    
}

int loprintQueue(){
    struct loNode *lovar=loback;
    if(lovar!=NULL)
    {
        printf("\n low priority Jobs in Queue are: ");
        while(lovar!=NULL)
        {
            printf("\n%d,%d,%d,%d",lovar->loData.jobNumber,lovar->loData.priority,lovar->loData.PIDi,lovar->loData.number_of_runs);
            lovar = lovar->lonext;
        }
        printf("\n");
        return 1;
        
    }
    else{
        printf("\n The low-Queue was empty");
        return 0;
    }
}



struct medNode
{
    struct jobAttr medData;
    struct medNode* mednext;
    
}*medback, *medfront;


void meddeQueue()
{
    struct medNode *medtemp, *medvar = medback;
    
    if (medvar==medback)
    {
        medback = medback->mednext;
        free(medvar);
        
    }
    else
        printf("\n The medgh-Queue is Empty");
}

void medpush(struct jobAttr medPIDVal)
{
    struct medNode *medtemp;
    medtemp = (struct medNode *)malloc(sizeof(struct medNode));
    medtemp->medData = medPIDVal;
    
    if (medfront == NULL)
    {
        medfront = medtemp;
        medfront->mednext =NULL;
        medback = medfront;
    }
    else
    {
        medfront->mednext = medtemp;
        medfront= medtemp;
        medfront->mednext=NULL;
    }
    
}
int medisEmpty(){
    struct medNode *medvar=medback;
    if(medvar!=NULL)
    {
        while(medvar!=NULL)
        {
            medvar = medvar->mednext;
        }
        return 1;
        
    }
    else{
        return 0;
    }
    
}

int medprintQueue(){
    struct medNode *medvar=medback;
    if(medvar!=NULL)
    {
        printf("\n medium priority Jobs in Queue are: ");
        while(medvar!=NULL)
        {
            printf("\n%d,%d,%d,%d",medvar->medData.jobNumber,medvar->medData.priority,medvar->medData.PIDi,medvar->medData.number_of_runs);
            medvar = medvar->mednext;
        }
        printf("\n");
        return 1;
        
    }
    else{
        printf("\n The medgh-Queue was empty");
    return 0;
    }
}






// function main -------------------------------------------------
int main(int argc,char** argv) {
    int i, j;
    pid_t pid;
    
    
    // check the number of command line arguments, if not 2, terminate
    // the program with a proper error message on the screen.
 
    if(argc!=2){
        
        fprintf(stderr, "\nCommand Line arguments does not equal 2\n");
        exit(EXIT_FAILURE);
    }
    

    else if((argv[1][0]<'3' || argv[1][0]>'6'||argv[1][1]!=NULL)){
        
        fprintf(stderr, "%c is not a value between 3 and 6\n",argv[1][0]);
        exit(EXIT_FAILURE);
        
    }
        int number_of_jobs = argv[1][0]-'0';
        
        printf("\nYou are about to process %d Jobs\n--------------------------------\n",number_of_jobs);
        
    
    
    
    
   
    create_log("assgn1.log");
    
    // prepare mymask1 -- SIGCHLD and SIGALRM blocked, all other signals free
    
    sigemptyset(&mymask1);
   
    sigaddset(&mymask1,SIGCHLD);
    sigaddset(&mymask1,SIGALRM);
    sigprocmask(SIG_SETMASK, &mymask1, NULL);
    
    
    // prepare mymask2 -- all signals free
    
    sigemptyset(&mymask2);

  

    // prepare jobmask -- all signals blocked except SIGUSR2
    
    sigfillset(&jobmask);
    sigdelset(&jobmask,SIGUSR2);
    
    
    //Initializes Child and Alarm Handler Masks and Flags
    
    sa_alarm.sa_handler = siga_handler;
    sigemptyset(&sa_alarm.sa_mask);
    sa_alarm.sa_flags = SA_RESTART;
    sigaction(SIGALRM,&sa_alarm,NULL);
    
    
    //Initializes Child and Alarm Handler Masks and Flags
    sa_chld.sa_handler = sigc_handler;
    sigemptyset(&sa_chld.sa_mask);
    sa_chld.sa_flags = SA_RESTART;
    sigaction(SIGCHLD,&sa_chld,NULL);

    
    
    // create a data structure to keep information about jobs - PID, number of runs
    struct jobAttr jawbs[number_of_jobs];
    
    //Initialize Jons to do based on input from user and add to Hi Queue
    //Initializes Queue!
    
     for(i = 0; i < number_of_jobs; i++) {
         pid = create_job(i);
         jawbs[i].PIDi = pid;
         jawbs[i].number_of_runs = 0;
         jawbs[i].priority= 0;
         jawbs[i].jobNumber = i;
         hipush(jawbs[i]);
    }
    
   
    
    int allEmpty = 1;
    
    // put all jobs in the high-priority queue
    // END TO DO
    while (allEmpty!=0)
        
    {
   //Check if all Jobs are done
        if((loisEmpty()+medisEmpty()+hiisEmpty())==0){
            allEmpty = 0;
            Msg("\nAll jobs done\n");
            msg("\nAll jobs done\n");
            exit(1);
            break;
            
        }
        
        
        
        //Check if Jobs are to be done in Hi Queue
        
        if (hiisEmpty()==1){
        
            Msg("Switched on high-priority job %d\n",hiback->hiData.jobNumber);
            msg("Switched on high-priority job %d\n",hiback->hiData.jobNumber);
            
            //Call to SIGURS1
            kill(hiback->hiData.PIDi,SIGUSR1);
            alarm(1);
            
            if(sigsuspend(&mymask2)!=-1){
                perror("Bad!");
            }
            
            
           
            
        }
        
        
        //Check if Jobs are to be done in Med Queue
        
        else if(medisEmpty()==1){
            
            Msg("Switched on medium-priority job %d\n",medback->medData.jobNumber);
            msg("Switched on medium-priority job %d\n",medback->medData.jobNumber);
            kill(medback->medData.PIDi,SIGUSR1);
            alarm(1);
            
            if(sigsuspend(&mymask2)!=-1){
                perror("Bad!");
            }
            //meddeQueue();
            //sigprocmask(SIG_SETMASK,&mymask2,&mymask1);
            
            
            
        }
        
        
        //Check if Jobs are to be done in Low Queue
        
        else {
//            
//            if((loisEmpty()==0)){
//                
//                Msg("\nAll jobs done\n");
//                msg("\nAll jobs done\n");
//                allEmpty = 0;
//                exit(1);
//                break;
//            }
          
            Msg("Switched on low-priority job %d\n",loback->loData.jobNumber);
            msg("Switched on low-priority job %d\n",loback->loData.jobNumber);
            kill(loback->loData.PIDi,SIGUSR1);
            

            alarm(1);
            if(sigsuspend(&mymask2)!=-1){
                perror("Bad!");
            }
        
       
        
        }
    }
    
    return 0;
}// end function main


// function create_job --------------------------------------------
pid_t create_job(int i) {
    pid_t pid;
    char argv0[10];
    char argv1[10];
    
   sigprocmask(SIG_SETMASK, &jobmask, &mymask1);
    
    if ((pid = fork()) < 0)
        Sys_exit("fork error\n");
    if (pid == 0) { // child process
        strcpy(argv0,"job");
        sprintf(argv1,"%d",i);
        execl("job",argv0,argv1,NULL);
    }
    
     sigprocmask(SIG_SETMASK, &mymask1, &jobmask);
    
    return pid;
}// end function create_job




// function siga_handler ------------------------------------------
void siga_handler() {
 
   //Hi Priority Job has completed, push to Med queue or back onto high, Based on number of runs
    
    if (hiisEmpty()==1){
        
        //Increment Runs
        (hiback->hiData.number_of_runs)++;
        
        Msg("Switched off high-priority job %d\n",hiback->hiData.jobNumber);
        msg("Switched off high-priority job %d\n",hiback->hiData.jobNumber);
        
        if(hiback->hiData.number_of_runs>=2){
            medpush(hiback->hiData);
            hideQueue();
            kill(hifront->hiData.PIDi,SIGUSR2);
            
        }
        else{
            hipush(hiback->hiData);
            hideQueue();
            kill(hifront->hiData.PIDi,SIGUSR2);
            
        }
        
        
        
    }
    
    
      //Med Priority Job has completed, push to Low queue or back onto Med, Based on number of runs
    
    else if(medisEmpty()==1){
        
        //Increment runs
        (medback->medData.number_of_runs)++;
        Msg("Switched off medium-priority job %d\n",medback->medData.jobNumber);
        msg("Switched off medium-priority job %d\n",medback->medData.jobNumber);

        
        if(medback->medData.number_of_runs>=6){
            lopush(medback->medData);
            meddeQueue();
            kill(medfront->medData.PIDi,SIGUSR2);
            
            
        }
        else{
            medpush(medback->medData);
             meddeQueue();
            kill(medfront->medData.PIDi,SIGUSR2);
           
        }
       
        
    }
    
    
    //Same as Med and Hi Queue.
    
    else {
        (loback->loData.number_of_runs)++;
     
        
        if (loback->loData.number_of_runs>50) {
            msg("Job Time-out, Job %d has executed 50 times",loback->loData.jobNumber);
            Msg("Job Time-out, Job %d has executed 50 times",loback->loData.jobNumber);
            exit(1);
            lodeQueue();
        }
        
        Msg("Switched off low-priority job %d\n",loback->loData.jobNumber);
        msg("Switched off low-priority job %d\n",loback->loData.jobNumber);
        lopush(loback->loData);
        lodeQueue();
        kill(lofront->loData.PIDi,SIGUSR2);
        
        
       
    }
    
   
    return;
}// end function siga_handler


// function sigc_handler ------------------------------------------
void sigc_handler() {
    
    if (hiisEmpty()==1){
        
        Msg("job %d done\n",hiback->hiData.jobNumber);
        //kill(hiback->hiData.PIDi,SIGUSR2);
        hideQueue();
        
        
    }
    
    else if(medisEmpty()==1){
        
        Msg("job %d done\n",medback->medData.jobNumber);
        //kill(medback->medData.PIDi,SIGUSR2);
        meddeQueue();
        
        
    }
    else {
        
        Msg("job %d done\n",loback->loData.jobNumber);
        //kill(loback->loData.PIDi,SIGUSR2);
        lodeQueue();
        
        
       
    }

 }// end function sigc_handler

