package com.example.marchdecachan;

class Produit {
    public int ID_Produit;
    public int QT_produit;
    String nom_Produit;
    public boolean produit_Valide;

    public void init_Produit (int ID, int QT) {
        ID_Produit = ID;            // 3 char
        QT_produit = QT;
        produit_Valide = false;
    }
}
