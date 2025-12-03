#include <stdio.h>
#include <string.h>

#define MAX_RES 100
#define MAX_SALLE 10
typedef enum {PENDING, CONFIRMED, CANCELLED} Status;
typedef struct {
    int id;
    char nom_client[50];
    char salle[50];
    char date[20];   
    int heure_debut;  
    int heure_fin;    
    int nombre_personnes;
    float tarif;
    Status statut;
} Reservation;

typedef struct {
    char nom[50];
    int capacite;
    float tarif_horaire;
    char equipements[200];
} Salle;

typedef struct {
    int id;
    char nom[50];
    char email[50];
    char téléphone[20];
    int nombre_reservations;  
} Client;


int Disponibilite(char *nomSalle, char *date, int hDebut, int hFin, Reservation reservations[], int nb_salles) {
    for(int i = 0; i < nb_salles; i++) {
        if(strcmp(reservations[i].salle, nomSalle) == 0 && strcmp(reservations[i].date, date) == 0) {

            if(hDebut < reservations[i].heure_fin || reservations[i].heure_debut < hFin) {
                return 1; 
            }
        }
    }
    return 0; 
}

int verifCapacite(int nombre_personnes, Salle salles[], int nb_salles, char *nomSalle) {
    for(int i = 0; i < nb_salles; i++) {
        if(strcmp(salles[i].nom, nomSalle) == 0) {
            if(nombre_personnes <= salles[i].capacite) {
                return 0; 
            } else {
                return 1; 
            }
        }
    }
    return 0; 
}

int creerReservation(Reservation r, Reservation reservations[], int *nb_res, Salle salles[], int nb_salles) {

    if(!verifCapacite(r.nombre_personnes, salles, nb_salles, r.salle)) {
        printf("Erreur : Salle insuffisante pour la capacité %s\n", r.salle);
        return 1;
    }

    if(!Disponibilite(r.salle, r.date, r.heure_debut, r.heure_fin, reservations, *nb_res)) {
        printf("Erreur : La salle %s est déjà réservée à cet horaire\n", r.salle);
        return 1;
    }

    float tarif_horaire = 0;
    for(int i = 0; i < nb_salles; i++) {
        if(strcmp(salles[i].nom, r.salle) == 0) {
            tarif_horaire = salles[i].tarif_horaire;
            break;
        }
    }
    r.tarif = tarif_horaire * (r.heure_fin - r.heure_debut);

    reservations[*nb_res] = r;
    (*nb_res)++;

    printf("Réservation créée avec succès ! Tarif = %.2f DT\n", r.tarif);
    return 0;
    }


float calculerTarifTotal(Reservation *r, Salle salles[], int nb_salles) {
    float tarif_horaire = 0;

    
    for(int i = 0; i < nb_salles; i++) {
        if(strcmp(salles[i].nom, r->salle) == 0) {
            tarif_horaire = salles[i].tarif_horaire;
            break;
        }
    }

    r->tarif = tarif_horaire * (r->heure_fin - r->heure_debut);
    return r->tarif;
}

void sauvegardetarifs_salle(Salle salles[], int nb_salles, const char *nomFichier) {
    FILE *f = fopen(nomFichier, "w"); 
    if(!f) {
        printf("Erreur : impossible d'écrire dans %s\n", nomFichier);
        return;
    }

    for(int i = 0; i < nb_salles; i++) {
        fprintf(f, "%s %.2f\n", salles[i].nom, salles[i].tarif_horaire);
    }

    fclose(f); 
}
void sauvegardetarifs_reservation(Reservation reservations[], int nb_reservations, Salle salles[], int nb_salles, const char *nomFichier){
    FILE *f = fopen(nomFichier, "w");
    if(!f) {
        printf("Erreur : impossible d'écrire dans %s\n", nomFichier);
        return;
    }

    for(int i = 0; i < nb_reservations; i++) {
        calculerTarifTotal(&reservations[i], salles, nb_salles);
        fprintf(f, "%d %s %s %s %d %d %d %.2f\n",
                reservations[i].id,
                reservations[i].nom_client,
                reservations[i].salle,
                reservations[i].date,
                reservations[i].heure_debut,
                reservations[i].heure_fin,
                reservations[i].nombre_personnes,
                reservations[i].tarif);
    }

    fclose(f);
}

void genererFacture(Reservation r, int id) {
    FILE *f = fopen(nomFichier, "w");
    if(!f) {
        printf("Erreur : impossible de créer %s\n", nomFichier);
        return;
    }
    int duree = r.heure_fin - r.heure_debut;
    fprintf(f, "Facture ID : %d\n", r.id);
    fprintf(f, "Client : %s\n", r.nom_client);
    fprintf(f, "Salle : %s\n", r.salle);
    fprintf(f, "Date : %s\n", r.date);
    fprintf(f, "Durée : %d heures\n", duree);
    fprintf(f, "Montant : %.2f DT\n", r.tarif);
    fclose(f);
}

void chiffredaffairesparsalle(Reservation reservations[],int nb_reservations, int nb_salles){
    printf("Chiffre d'affaire par salle");
    
    float ca[10]={0};    //tableau des chiffres daffaires

    for(int i = 0; i < nb_reservations; i++){
        int exist = -1;

        for(int j = 0; j < nb_salles; j++) {
            if(strcmp(salles[j], reservations[i].salle) == 0) {
                exist = j;
                break;
            }
        if(!exist = -1){
           ca[exist]+=reservations[i].tarif;
        }
    }
    for(int i = 0; i < nb_salles; i++) {
        printf("Salle %s : %.2f DT\n", salles[i], ca[i]);
    }
}
void reservationsParMois(Reservation reservations[], int nb_reservations) {
    int mois_count[12] = {0};  // compteur pour chaque mois (0 = janvier, 11 = décembre)

    for(int i = 0; i < nb_reservations; i++) {
        int mois;
        sscanf(reservations[i].date, "%*d-%d-%*d", &mois);
        mois_count[mois - 1]++;
    }

    printf("\n=== Nombre de réservations par mois ===\n");
    for(int i = 0; i < 12; i++) {
        printf("Mois %02d : %d réservation(s)\n", i + 1, mois_count[i]);
    }
}
void sallesPopulaires(Reservation reservations[], int nb_reservations, salle salles[] , int nb_salles) {

    int compte[10] = {0};

    for(int i = 0; i < nb_reservations; i++) {
        int index = -1;
        for(int j = 0; j < nb_salles; j++) {
            if(strcmp(salles[j], reservations[i].salle) == 0) {
                index = j;
                break;
            }
        }
        compte[index]++;
    }
 int max_res = 0;
    for(int i = 0; i < nb_salles; i++) {
        if(compte[i] > max_res) {
            max_res = compte[i];
        }
    }
    printf("\n=== Salle(s) la/les plus populaire(s) ===\n");
    for(int i = 0; i < nb_salles; i++) {
        if(compte[i] == max_res) {
            printf("Salle %s : %d réservation(s)\n", salles[i], compte[i]);
        }
    }
}

float prixAvecFidelite(Client c, float prix_base) {
    float reduction = 0.0f;

    if (c.total_reservations > 10) {
        reduction = 0.20f;  // 20 %
    } 
    else if (c.total_reservations > 5) {
        reduction = 0.10f;  // 10 %
    }
    float prix_final = prix_base * (1.0f - reduction);
    return prix_final;
}







