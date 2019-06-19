package com.example.marchdecachan;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.graphics.Color;
import android.os.AsyncTask;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import java.util.ArrayList;

// page d'acceuil : Connextion WIFI
public class MainActivity extends AppCompatActivity {

    // wifi
    private TextView mWIFIStatus;
    private TextView mWifiEcho;
    private Button mWIFICo;
    private Button bpNvCommand;
    private TextView mSelectIP;

    private Button mBPDebug;

    public TcpClient mTcpClient;
    private ArrayList<String> arrayList;
    private Button mWIFISend;

    static public String recepServ = null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // WIFI
        mWIFIStatus = (TextView) findViewById(R.id.textView_statusWifi);
        mWifiEcho = (TextView) findViewById(R.id.ID_TextView_WifiEcho);
        mWIFICo = (Button) findViewById(R.id.ID_button_WIFICo);
        bpNvCommand = (Button) findViewById(R.id.button_nvCommand);
        mSelectIP = (TextView) findViewById(R.id.ID_IP);
        mWIFISend = (Button) findViewById(R.id.ID_button_WIFISend);

        mBPDebug = (Button) findViewById(R.id.button_debug);

        arrayList = new ArrayList<String>();

        final Intent monIntent = new Intent(this, FenProduits.class);         // lien avec la 2eme page

        // Connection au Serveur WIFI
        mWIFICo.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                try{
                    wifiOn(v);          // etape de connexion
                }
                catch (Exception e) {
                    ToastMsgErreur("ERREUR WIFI:\nConnexion WIFI Serveur");
                }

                // attente pendant la connexion (sur un autre Thread)
                try {
                    Thread.sleep(1000);
                }
                catch (Exception ex) {
                    mWIFIStatus.setText("ERREUR Connexion Wifi ...");
                }

                // envoi d'un premier msg (".") pour recevoir le msg de Bienvenue du serveur
                wifiSend(v, ".");
            }
        });

        // envoi d'une trame au serveur
        mWIFISend.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                // demande Nv Panier au serveur
                //new CommServ().execute("");
                requeteNvPanier(v);
                //wifiSend(v, "un panier !");

                //new CommServ().execute("");



            }
        });

        // ouvre la Deuxieme fenetre : Liste des Produits
        bpNvCommand.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                try {
                    wifiOff(v);     // coupe la liaison WIFI
                }
                catch (Exception e) {
                    ToastMsgErreur("ERREUR WIFI:\nDéconnexion WIFI");
                }

                monIntent.putExtra(recepServ, recepServ);
                startActivity(monIntent);
            }
        });


        // permet de debug le serveur si commande non terminée depuis client
        mBPDebug.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                try {
                    String messagetx = new String(new byte[] {0x10, 0x01});

                    messagetx += "2";     // recup QT Produit dans Panier

                    // envoie de la requete au serveur
                    wifiSend(null, messagetx);        // View = null, requete = 0x1001
                }
                catch (Exception e) {
                    ToastMsgErreur("ERREUR WIFI:\nDEBUG WIFI");
                }
            }
        });
    }

    // creer un message ERREUR
    private void ToastMsgErreur(String msg) {
        Toast toastErreur = Toast.makeText(getApplicationContext(),msg,Toast.LENGTH_SHORT);
        View view = toastErreur.getView();
        view.setBackgroundColor(Color.RED);
        toastErreur.show();
    }

    // WIFI
    public class ConnectTask extends AsyncTask<String, String, TcpClient> {

        @Override
        protected TcpClient doInBackground(String... message) {
            Log.e("doInBackground", "TCP: " + message.toString());

            //we create a TCPClient object
            mTcpClient = new TcpClient(new TcpClient.OnMessageReceived() {
                @Override
                //here the messageReceived method is implemented
                public void messageReceived(String message) {
                    mWifiEcho.setText("TCP - messageReceived: " + message);
                    Log.e("messageReceived", "messageReceived: " + message);
                    //this method calls the onProgressUpdate
                    publishProgress(message);       // appel onProgressUpdate()
                    recepServ = message;            // svg en global
                }
            }, mSelectIP.getText().toString());

            mTcpClient.run();
            return null;
        }

        @Override
        protected void onProgressUpdate(String... values) {         // MAJ IU ici
            super.onProgressUpdate(values);
            //response received from server
            //mWIFIStatus.setText("onProgressUpdate:  " + values[0]);
            Log.e("onProgressUpdate ", "response: " + values[0]);
        }
    }

    // connecte le Wifi + envoie un msg
    private void wifiOn(View view){
        // start connection to the server
        mWIFIStatus.setText("New Connexion Wifi ...");
        new ConnectTask().execute("");
    }

    // envoi un msg au serveur Wifi (et recup la reponse: Echo)     -> erreur car mTcpClient = null (different de celui init)
    private void wifiSend(View view, final String msg){
        if (mTcpClient != null) {
            mTcpClient.sendMessage(msg);
            mWIFIStatus.setText("Msg sent: " + msg);
        }
        else mWIFIStatus.setText("Disconnected");
    }

    // Deconnecte le Wifi
    private void wifiOff(View view){
        try {
            if (mTcpClient != null) {
                mTcpClient.stopClient();
                mWIFIStatus.setText("Wifi Disconnected");
            }
            else mWIFIStatus.setText("Wifi already Disconnected");
        }
        catch  (Exception ex) {
            Toast.makeText(getApplicationContext(), "TCP - Disconnection Failed\n" + ex.toString(), Toast.LENGTH_SHORT).show();
        }
    }

    // envoie une trame demandant un nouveau panier
    private void requeteNvPanier (View v) {
        String messagetx = new String(new byte[] {0x10, 0x01});

        // envoie de la requete au serveur
        wifiSend(v, messagetx);        // View = null, requete = 0x1001
    }

    // envoie une trame demandant un nouveau panier
    private void requeteRecepNvPanier (String reponseServ) {
        String messagetx = new String(new byte[] {0x10, 0x01});

        //messagetx += reponseServ.substring(2, 2);     // recup ID Panier
        messagetx += reponseServ.substring(3, 4);     // recup QT Produit dans Panier

        // envoie de la requete au serveur
        wifiSend(null, messagetx);        // View = null, requete = 0x1001
    }







    // permet de gerer la communication avec le serveur en parallele du reste du programme
    public class CommServ extends AsyncTask<String, String, Void> {

        @Override
        protected Void doInBackground(String... message) {
            String ID_Panier = null;

            Log.e("doInBackground ", "doInBackground 1: " + message[0]);

            // envoi d'une requete de New Panier au serv
            requeteNvPanier(null);

            // bloque ne attendant la reponse du serveur
            while(recepServ == null);

            ID_Panier = recepServ.substring(2, 2);
            Log.e("doInBackground ", "doInBackground 2: " + ID_Panier);
            // envoie du msg de reception du panier
            requeteRecepNvPanier(recepServ);
            recepServ = null;           // RAZ de la variable de "reponse du serveur"

            // attente de la validation du serveur
            while(recepServ == null);

            if(recepServ.substring(2,2).equals(ID_Panier)) {     // verifi que la validation correspond au panier actuel
                recepServ = null;           // RAZ de la variable de "reponse du serveur"
                Log.e("doInBackground ", "doInBackground 3: " + recepServ.substring(2,2));
            }
            else Log.e("doInBackground ", "ERREUR doInBackground 3: " + recepServ.substring(2,2));
            return null;
        }

        @Override
        protected void onProgressUpdate(String... values) {         // MAJ IU ici
            super.onProgressUpdate(values);
            //response received from server
            Log.e("onProgressUpdate ", "response: " + values[0]);
        }
    }



}
