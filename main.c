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

//initie les numéros de voiture dans le tableau run
void fillRun(Voiture run[]){
    for(int i=0;i<20;i++){
        run[i].number=tab[i];    
    }
}

//remplit les temps de chaque secteur pour chaque voiture dans le tableau run
void fillLapTimes(Voiture run[]){
    for(int i=0;i<20;i++){
        run[i].s1=randomTime();
        run[i].s2=randomTime();
        run[i].s3=randomTime();
    }
}

//mise à jour des meilleurs temps de chaque secteur et de chaque voiture
void updateBestTimes(Voiture run[]){
    for(int i=0; i<20; i++){
        //mise à jour du meilleur temps de chaque secteur
        if(run[i].s1 < bestS1){
            bestS1 = run[i].s1;
        }
        if(run[i].s2 < bestS2){
            bestS2 = run[i].s2;
        }
        if(run[i].s3 < bestS3){
            bestS3 = run[i].s3;
        }
        //mise à jour du meilleur temps de chaque voiture
        run[i].bestTime = run[i].s1 + run[i].s2 + run[i].s3;
        //mise à jour du meilleur temps de la séance
        if(run[i].bestTime < bestAllTime){
            bestAllTime = run[i].bestTime;
        }
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
void show(Voiture run[]){
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
            fprintf(fichier, " %2.3f  | + %2.3f  |",run[i].bestTime,min);
    		printf( " %2.3f  | + %2.3f  |",run[i].bestTime,min);
    		if(run[i].stands==1){
                fprintf(fichier, "   P   |\n");
    			printf( "   P   |\n");
    		}
    		if(run[i].abandon==1){
                fprintf(fichier, "   A   |\n");
    			printf( "   A   |\n");
    		}
    		if(run[i].stands==0 && run[i].abandon==0){
                fprintf(fichier, "        |\n");
    			printf( "        |\n");
    		}
    	}
    }
    fprintf(fichier, "-------------------------------------------------------------------------------\n");
	printf("-------------------------------------------------------------------------------\n");
    fprintf(fichier, "BestS1 : %2.3f || BestS2 : %2.3f || BestS3 : %2.3f || BestTime : %2.3f\n",bestS1,bestS2,bestS3,bestAllTime);
	printf("BestS1 : %2.3f || BestS2 : %2.3f || BestS3 : %2.3f || BestTime : %2.3f\n",bestS1,bestS2,bestS3,bestAllTime);
	fclose(fichier);
}

int main(int argc, char const *argv[]) {
    int i;
    //mémoire partagée
    int shmid = shmget(75,sizeof(int)*5,0666|IPC_CREAT);
    Voiture *run = (Voiture *)shmat(shmid, NULL, 0);
    
    fillRun(run);
    fillLapTimes(run);
    updateBestTimes(run);
    tri(run);
    show(run);
    
    return 0;
}
