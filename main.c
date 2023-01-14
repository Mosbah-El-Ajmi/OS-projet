#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <time.h>
#include <lib/sprint.h>

//données des numéros de voitures
int tab[20]={44,63,1,11,55,16,4,3,14,31,10,22,5,18,6,23,77,24,47,9};

//si course ou sprint
int isRace = 0;

//stockage des records
double bestS1=50.0;
double bestS2=50.0;
double bestS3=50.0;
double bestAllTime=150.0;
int bestnumero = 0;
int bestcarS1 = 0;
int bestcarS2 = 0;
int bestcarS3 = 0;
double TempsCourse = 0.0;
double tailleCircuit = 0.0;
double tailleSprint = 0.0;

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
    double bests1;
    double bests2;
    double bests3;
	double tempStand;
	double bestperso;
	int points;
    int abandon;
	int maxstand;
    double total_time;
} Voiture;

//initie les numéros de voiture dans le tableau run
void fillRun(Voiture run[]){
    for(int i=0;i<20;i++){
        run[i].number=tab[i];
		run[i].bests1 = bestS1;
		run[i].bests2 = bestS2; 
		run[i].bests3 = bestS3;   
		run[i].bestperso = bestAllTime;  
		run[i].maxstand = 0;
    }
}

void updateBestTimes(Voiture run[]){
    for(int i=0; i<20; i++){
        if (run[i].abandon == 0) {
            if(run[i].s1 < run[i].bests1){
                run[i].bests1 = run[i].s1;
				if(run[i].s1 < bestS1){
					bestS1 = run[i].s1;
					bestcarS1 = run[i].number;
				}
            }
            if(run[i].s2 < run[i].bests1){
                run[i].bests2 = run[i].s1;
				if(run[i].s2 < bestS2){
					bestS2 = run[i].s2;
					bestcarS2 = run[i].number;
				}
            }
            if(run[i].s3 < run[i].bests3){
                run[i].bests3 = run[i].s1;
				if(run[i].s3 < bestS3){
					bestS3 = run[i].s3;
					bestcarS3 = run[i].number;
				}
            }
            //mise à jour du temps total de chaque voiture
            run[i].bestTime = run[i].s1 + run[i].s2 + run[i].s3;
			//mise à jour du meilleur temps de la séance (record perso)
			if(run[i].bestTime < run[i].bestperso){
                run[i].bestperso = run[i].bestTime;
            }
            //mise à jour du meilleur temps de la séance
            if(run[i].bestTime < bestAllTime){
                bestAllTime = run[i].bestTime;
                bestnumero = run[i].number;
            }
        } 
    } 
}

//trie les voitures par leur Meilleur temps
void tri(Voiture run[]){
	Voiture a;
	for (int i = 0; i < 20; ++i) {

            for (int j = i + 1; j < 20; ++j){
 
                if (run[i].bestperso > run[j].bestperso){
 
                    a =  run[i];
                    run[i] = run[j];
                    run[j] = a;
 
                }
 
            }
 
        }
}

//ecris dans un fichier classement le classement final de la course
void classement(Voiture run[]){
    char *filename = "classement.txt";
	FILE *fichier = fopen(filename, "w");
    if (fichier == NULL)
    {
        printf("Error opening the file %s", filename);
    }
 	fprintf(fichier, "Numero |   S1   |   S2   |   S3   |   Total   |   Diff   |   Stands  \n");   
	double min=0.0;
	for(int i=0;i<20;i++){
	    
		if(i>0){
			min=run[i].bestperso-run[i-1].bestperso;
		}
		if(run[i].number<10){
            fprintf(fichier, " ");
		}
		
        fprintf(fichier, "  %d   | %2.3f | %2.3f | %2.3f |",run[i].number,run[i].bests1,run[i].bests2,run[i].bests3);
		if(run[i].bestperso<100.0){
            fprintf(fichier, " ");
		}
        if(run[i].tempStand > 0){
		    fprintf(fichier, " %2.3f  | + %2.3f  |    %2.3f    \n",run[i].bestperso,min,run[i].tempStand);
		} else {
	        fprintf(fichier, " %2.3f  | + %2.3f  |    ---    \n",run[i].bestperso,min);
		}
	
    }
	fprintf(fichier, "-------------------------------------------------------------------------------\n");
	fprintf(fichier, "Best time sector 1 : %d => %2.3f seconds\n",bestcarS1,bestS1);
	fprintf(fichier, "Best time sector 2 : %d => %2.3f seconds\n",bestcarS2,bestS2);
	fprintf(fichier, "Best time sector 3 : %d => %2.3f seconds\n",bestcarS3,bestS3);
	fprintf(fichier, "Best total time : %d => %2.3f seconds\n",bestnumero,bestAllTime);
	fclose(fichier);
}

// affichage dans la console
void show(Voiture run[]){
	printf("Numero |   S1   |   S2   |   S3   |   Best   |   Diff   |   Stands   \n");
	double min=0.0;
	for(int i=0;i<20;i++){
	    
		if(i>0){
			min=run[i].bestperso-run[i-1].bestperso;
		}
		if(run[i].number<10){
			printf( " ");
		}
		
		printf( "  %d   | %2.3f | %2.3f | %2.3f |",run[i].number,run[i].bests1,run[i].bests2,run[i].bests3);
		if(run[i].bestperso<100.0){
			printf(" ");
		}
		if(run[i].tempStand > 0){
		    printf( " %2.3f  | + %2.3f  |    %2.3f    \n",run[i].bestperso,min,run[i].tempStand);
		} else {
	        printf( " %2.3f  | + %2.3f  |    ---    \n",run[i].bestperso,min);
		}	
    }
    printf("-------------------------------------------------------------------------------\n");

}

void abandon(Voiture run[]) {
    // génération d'un nombre aléatoire entre 0 et 19
    int index = rand() % 20;
    // vérification de la chance (1 chance sur 20) de mettre en abandon
    if (rand() % 20 == 0) {
        run[index].abandon = 1; // mise en abandon de la voiture
    }
}

void stand(Voiture run[]) {
	// génération d'un nombre aléatoire entre 0 et 19
	int index = rand() % 20;
	if(run[index].maxstand < 3){
		// vérification de la chance (1 chance sur 4) de mettre au stand
		if (rand() % 4 == 0) {
			int tempStand = 25; // mise au stand de la voiture
			run[index].tempStand = run[index].tempStand + tempStand;
			run[index].bestTime = run[index].bestTime + tempStand;
			run[index].s3 = run[index].s3 + tempStand;
			run[index].total_time = run[index].total_time + tempStand;
			run[index].maxstand = run[index].maxstand + 1;
		}
	}
}

void standSprint(Voiture run[]) {
    // génération d'un nombre aléatoire entre 0 et 19
    int index = rand() % 20;
    // vérification de la chance (1 chance sur 4) de mettre au stand
    if (rand() % 4 == 0) {
        int tempStand = rand() % 3 + 2; // mise au stand de la voiture
		run[index].tempStand = run[index].tempStand + tempStand;
		run[index].bestTime = run[index].bestTime + tempStand;
		run[index].total_time = run[index].total_time + tempStand;
    }
}

//remplit les temps de chaque secteur pour chaque voiture dans le tableau run
void fillLapTimes(Voiture run[]){
    for(int i=0;i<20;i++){
        if(run[i].abandon == 0){
            run[i].s1=randomTime();
            run[i].s2=randomTime();
            run[i].s3=randomTime();
            run[i].total_time += run[i].s1 + run[i].s2 + run[i].s3;
            if(run[i].total_time >= TempsCourse) {
                run[i].abandon = 1;
            }
        } 
    }
}

//remplit les temps de chaque secteur pour chaque voiture dans le tableau run pour le sprint
void fillLapTimesSprint(Voiture run[]){
    for(int i=0;i<20;i++){
        if(run[i].abandon == 0){
            run[i].s1=randomTime();
            run[i].s2=randomTime();
            run[i].s3=randomTime();
        } 
    }
}

//appelle cette fonction pour simuler des essais
void essais(){
	TempsCourse = 3600.0;

    int shmid = shmget(32, 20*sizeof(Voiture),IPC_CREAT|0666);
    Voiture *run =  shmat(shmid,0,0);

    fillRun(run);
    
    
    bool finished = false;
    while(!finished){
        fillLapTimes(run);
		stand(run);
        updateBestTimes(run);
        abandon(run);
        tri(run);
        show(run);

        finished = true;
        for(int i=0;i<20;i++){
            if(run[i].abandon == 0){
                finished = false;
                break;
            }
        }
    }
  
    classement(run);
}

//appelle cette fonction pour simuler des qualifs (avec temps a determiner)
void qualifs(){
	int shmid = shmget(32, 20*sizeof(Voiture),IPC_CREAT|0666);
    Voiture *run =  shmat(shmid,0,0);
    
	for(int i=0;i<3;i++){
		if(i == 0){
			TempsCourse = 1080.0;
			fillRun(run);
    
			bool finished = false;
			while(!finished){
				fillLapTimes(run);
				stand(run);
				updateBestTimes(run);
				abandon(run);
				tri(run);
				show(run);

				finished = true;
				for(int i=0;i<20;i++){
					if(run[i].abandon == 0){
						finished = false;
						break;
					}
				}
			}
		
			classement(run);

		}
		if(i == 1){
			TempsCourse = 900.0;
			fillRun(run);
    
			bool finished = false;
			while(!finished){
				fillLapTimes(run);
				stand(run);
				updateBestTimes(run);
				abandon(run);
				tri(run);
				show(run);

				finished = true;
				for(int i=0;i<20;i++){
					if(run[i].abandon == 0){
						finished = false;
						break;
					}
				}
			}
		
			classement(run);

		}
		if(i == 2){
			TempsCourse = 720.0;
			fillRun(run);
    
			bool finished = false;
			while(!finished){
				fillLapTimes(run);
				stand(run);
				updateBestTimes(run);
				abandon(run);
				tri(run);
				show(run);

				finished = true;
				for(int i=0;i<20;i++){
					if(run[i].abandon == 0){
						finished = false;
						break;
					}
				}
			}
		
			classement(run);

		}
	}

}

//enregistre les points et les ecrits dans un fichier texte le tableau des points des pilotes
void lespoints(Voiture run[]){
	if(isRace == 0){
		run[0].points = 8;
		run[1].points = 7;
		run[2].points = 6;
		run[3].points = 5;
		run[4].points = 4;
		run[5].points = 3;
		run[6].points = 2;
		run[7].points = 1;
	} else {
		run[0].points = 25;
		run[1].points = 20;
		run[2].points = 15;
		run[3].points = 10;
		run[4].points = 8;
		run[5].points = 6;
		run[6].points = 5;
		run[7].points = 3;
		run[8].points = 2;
		run[9].points = 1;
	}
	char *filename = "tableauScore.txt";
	FILE *fichier = fopen(filename, "w");
    if (fichier == NULL)
    {
        printf("Error opening the file %s", filename);
    }
 	fprintf(fichier, "Numero |   Points  \n");   
	double min=0.0;
	for(int i=0;i<20;i++){	    
		if(run[i].number<10){
            fprintf(fichier, " ");
		}
        fprintf(fichier, "  %d   |   %d \n",run[i].number,run[i].points);		
    }
	fclose(fichier);
}

//appelle cette fonction pour le sprint final
void sprint(){
    double tailleCircuit = 30.0;
	double tailleSprint = 120.0;
	int tours = tailleSprint/tailleCircuit;
    int shmid = shmget(32, 20*sizeof(Voiture),IPC_CREAT|0666);
    Voiture *run =  shmat(shmid,0,0);

    fillRun(run);
    
    
    for(int i=0;i<tours;i++){
		fillLapTimesSprint(run);
		stand(run);
        updateBestTimes(run);
        abandon(run);
        tri(run);
        show(run);
	}
	
	lespoints(run);
    classement(run);
}

int main(){
    srand(time(NULL));
    essais();
	qualifs();
	sprint();
    return 0;
}
