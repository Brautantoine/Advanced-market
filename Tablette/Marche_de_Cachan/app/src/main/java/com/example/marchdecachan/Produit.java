package com.example.marchdecachan;

import java.util.ArrayList;

class Produit {
    public String ID_Produit;
    public int QT_produit;
    public String nom_Produit;
    public String emplacement_Produit;
    public boolean produit_Valide;          // inutil ?

    public void init_Produit (String ID, int QT) {
        ID_Produit = ID;            // 6 char
        QT_produit = QT;            // 3 char

        String[] recup = tableCorrespondance(ID);
        if(recup != null) {
            // recup[0] => ID
            nom_Produit = recup[1];
            emplacement_Produit = recup[2];
            produit_Valide = false;
        }
    }

    public String[] tableCorrespondance (String ID) {
        String[] produitCorrespondant = null;
        String mArrayListTableCorrespondance[][] = {
                // ID, Nom Emplacement
                { "f65eb4", "Aston Martin de james Bond" , "D8"},
                { "cb091d", "Generateur d'alcool infini" , "C9"},
                { "aecd97", "Robe de Soiree" , "C3"},
                { "0e3120", "Saturne 5" , "E7"},
                { "db6793", "PC MSI" , "C1"}
        };

        for(int i = 0; i < mArrayListTableCorrespondance.length; i++) {
            if (ID.equals(mArrayListTableCorrespondance[i][0]));
                produitCorrespondant = mArrayListTableCorrespondance[i];
        }

        return produitCorrespondant;
    }



}
