#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>

//données des numéros de voitures
int tab[20]={44,63,1,11,55,16,4,3,14,31,10,22,5,18,6,23,77,24,47,9};

//stockage des records
double bestS1=50.0;
double bestS2=50.0;
double bestS3=50.0;
double bestAllTime=150.0;

//créateur de temps
double randomTime(){
	int a=rand()%21000 +25000;
	double b=(double)a/1000.0;
	return b;
}

//structure voiture
typedef struct car{
	int number;
	double s1;
	double s2;
	double s3;
	double bestTime;
	int stands;
	int abandon;
} Voiture;

Voiture run[20];


//initie les numéros de voiture dans le tableau run
void fillRun(Voiture run[]){
	for(int i=0;i<20;i++){
		run[i].number=tab[i];	
	}
}

//trie les voitures par leur Meilleur temps
void tri(Voiture run[]){
	Voiture a;
	for (int i = 0; i < 20; ++i) {

            for (int j = i + 1; j < 20; ++j){
 
                if (run[i].bestTime > run[j].bestTime){
 
                    a =  run[i];
                    run[i] = run[j];
                    run[j] = a;
 
                }
 
            }
 
        }
}

// affichage
void show(){
    char *filename = "affichage.txt";
	FILE *fichier = fopen(filename, "w");
    if (fichier == NULL)
    {
        printf("Error opening the file %s", filename);
    }
 	fprintf(fichier, "Numéro |   S1   |   S2   |   S3   |   Best   |   Diff   |   Stand   |\n");   
	printf("Numéro |   S1   |   S2   |   S3   |   Best   |   Diff   |   Stand   |\n");
	double min=0.0;
	for(int i=0;i<20;i++){
	    if (run[i].stands == 0 || run[i].abandon == 0){
    		if(i>0){
    			min=run[i].bestTime-run[i-1].bestTime;
    		}
    		if(run[i].number<10){
                fprintf(fichier, " ");
    			printf( " ");
    		}
            fprintf(fichier, "  %d   | %2.3f | %2.3f | %2.3f |",run[i].number,run[i].s1,run[i].s2,run[i].s3);
    		printf( "  %d   | %2.3f | %2.3f | %2.3f |",run[i].number,run[i].s1,run[i].s2,run[i].s3);
    		if(run[i].bestTime<100.0){
                fprintf(fichier, " ");
    			printf(" ");
    		}
            fprintf(fichier, " %2.3f  | + %2.3f  |   -----   |\n",run[i].bestTime,min);
    		printf(" %2.3f  | + %2.3f  |   -----   |\n",run[i].bestTime,min);
	    }
	    else{
            fprintf(fichier, "  %d   |   --   |   --   |   --   |   ----   |   ----   |     P     |",run[i].number);
    	    printf("  %d   |   --   |   --   |   --   |   ----   |   ----   |     P     |",run[i].number);
    	}
		
	}
    fprintf(fichier, "-------------------------------------------------------------------------------\n");
	printf("-------------------------------------------------------------------------------\n");
    fprintf(fichier, "BestS1 : %2.3f || BestS2 : %2.3f || BestS3 : %2.3f || BestTime : %2.3f\n",bestS1,bestS2,bestS3,bestAllTime);
	printf("BestS1 : %2.3f || BestS2 : %2.3f || BestS3 : %2.3f || BestTime : %2.3f\n",bestS1,bestS2,bestS3,bestAllTime);
	fclose(fichier);
}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   

// second jet du projet. utilisation de structures, mis en place d'un premier affichage
int main(void){
    int shmid = shmget(32, 21*sizeof(Voiture),IPC_CREAT|0666);
    void *circuit =  shmat(shmid,0,0);
    
	fillRun(run);
	double seg1;
	double seg2;
	double seg3;
	double segt;
	
	for(int i=0;i<20;i++){
		for(int j=0;j<10;j++){
			seg1=randomTime();
			seg2=randomTime();
			seg3=randomTime();
			segt=seg1+seg2+seg3;
			
			run[i].s1=seg1;
			run[i].s2=seg2;
			run[i].s3=seg3;
			run[i].abandon = 0;
			run[i].stands = 0;
			
			if(j==0){
				run[i].bestTime=segt; 
			}
			else if(run[i].bestTime>segt){
					run[i].bestTime=segt;
			}
			if(seg1<bestS1){
				bestS1=seg1;
			}
			if(seg2<bestS2){
				bestS2=seg2;
			}
			if(seg3<bestS3){
				bestS3=seg3;
			}
			if(segt<bestAllTime){
				bestAllTime=segt;
			}
			if(run[i].abandon == 1){
			    run[i].s1=0;
			    run[i].s2=0;
			    run[i].s3=0;
			    bestAllTime=0;
			}
		}
	}
	tri(run);
	show();	
	
}