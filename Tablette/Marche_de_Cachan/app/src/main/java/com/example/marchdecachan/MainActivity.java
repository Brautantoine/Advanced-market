package com.example.marchdecachan;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.Button;

// page d'acceuil : Connextion WIFI
public class MainActivity extends AppCompatActivity {

    static String recepServeur;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        final Button click_buttonNvCommand = (Button) findViewById(R.id.button_nvCommand);

        final Intent monIntent = new Intent(this, FenProduits.class);         // lien avec la 2eme page

        // test
        recepServeur = "1122a3333b.444/c5555d.666 \0 ";      // id_Npanier = 11, id_panier = 22, panier = {id_produit = a3333b, ".",  qt = 444 "/"}, 0xFF

        // ouvre la Deuxieme fenetre : Liste des Produits
        click_buttonNvCommand.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                monIntent.putExtra(recepServeur, recepServeur);
                startActivity(monIntent);
            }
        });
    }


}
