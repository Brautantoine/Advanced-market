package com.example.marchdecachan;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

// page d'acceuil : Connextion WIFI
public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        final Button click_buttonNvCommand = (Button) findViewById(R.id.button_nvCommand);

        final Intent monIntent = new Intent(this, FenProduits.class);         // lien avec la 2eme page

        // ouvre la Deuxieme fenetre : Liste des Produits
        click_buttonNvCommand.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                startActivity(monIntent);
            }
        });
    }
}
