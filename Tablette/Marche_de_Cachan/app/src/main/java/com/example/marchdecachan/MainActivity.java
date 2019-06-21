package com.example.marchdecachan;

import android.annotation.SuppressLint;
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
import java.util.concurrent.ExecutionException;

// page d'acceuil : Connextion WIFI
public class MainActivity extends AppCompatActivity {

    // wifi
    private TextView mWIFIStatus;
    private TextView mWifiEcho;
    private Button mWIFICo;
    private Button bpNvCommand;
    private TextView mSelectIP;

    public TcpClient mTcpClient;
    private ArrayList<String> arrayList;

    static public String recepServ = "";
    static public String panier = "";

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

        arrayList = new ArrayList<String>();

        final Intent monIntent = new Intent(this, FenProduits.class);         // lien avec la 2eme page

        // Connection au Serveur WIFI
        mWIFICo.setOnClickListener(new View.OnClickListener() {
            @SuppressLint("SetTextI18n")
            @Override
            public void onClick(View v) {
                try{
                    wifiOn(v);          // etape de connexion
                }
                catch (Exception e) {
                    ToastMsgErreur("ERREUR WIFI:\nConnexion WIFI Serveur");
                }
            }
        });

        // ouvre la Deuxieme fenetre : Liste des Produits
        bpNvCommand.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                while(recepServ == "") {
                    Log.e("send. ", "send 1.1: recepServ = " + recepServ);
                    try {
                        Thread.sleep(500);
                    }
                    catch (Exception ex) {
                        Log.e("Sleep ", "ERREUR Thread.sleep");
                    }
                }

                // demande Nv Panier au serveur :
                Log.e("send ", "send 1: Debut");

                // envoi d'une requete de New Panier au serv
                requeteNvPanier(v);

                // bloque en attendant la reponse du serveur
                while(recepServ == "") {
                    Log.e("send ", "send 1.2: recepServ = " + recepServ);
                    try {
                        Thread.sleep(500);
                    }
                    catch (Exception ex) {
                        Log.e("Sleep ", "ERREUR Thread.sleep");
                    }
                }

                Log.e("co ", "recepServ 2 : " + recepServ);

                String ID_Panier = null;
                String QT_Panier = null;

                for(int i = 0; i < recepServ.length(); i++) {
                    if (recepServ.charAt(i) == ((char)0x10)) {
                        //if (recepServ.charAt(i+1) == ((char)0x01))
                        recepServ = recepServ.substring(i);
                        break;
                    }
                }

                panier = recepServ;

                ID_Panier = recepServ.substring(2, 4);          // == 2b ?
                Log.e("send ", "send 2.1. ID: " + ID_Panier);
                QT_Panier = recepServ.substring(4, 6);          // == 02 ?
                Log.e("send ", "send 2.2. QT: " + QT_Panier);

                // envoie du msg de reception du panier
                requeteRecepNvPanier(recepServ);

                // attente de la validation du serveur
                while(recepServ == "") {
                    Log.e("send ", "send 2.1: recepServ = " + recepServ);
                    try {
                        Thread.sleep(500);
                    }
                    catch (Exception ex) {
                        Log.e("Sleep ", "ERREUR Thread.sleep");
                    }
                }
                Log.e("co ", "recepServ 4 : " + recepServ);

                if(recepServ.substring(2, 4).equals(ID_Panier)) {     // verifi que la validation correspond au panier actuel
                    Log.e("send ", "send 3: " + recepServ.substring(2, 4));
                }
                else Log.e("send ", "ERREUR send 3: ID: " + recepServ.substring(2, 4) + " != " + ID_Panier);

                Log.e("send ", "send 4: Fin");
                recepServ = "";           // RAZ de la variable de "reponse du serveur"

                try {
                    wifiOff(v);     // coupe la liaison WIFI
                }
                catch (Exception e) {
                    ToastMsgErreur("ERREUR WIFI:\nDéconnexion WIFI");
                }

                monIntent.putExtra(panier, panier);
                startActivity(monIntent);
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
                    if(!message.equals(""))
                        mWifiEcho.setText(/*"TCP - messageReceived: " +*/ message);
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
            Log.e("onProgressUpdate ", "response: " + values[0]);       // affichage a la fin de l'echange !
        }
    }

    // connecte le Wifi + envoie un msg
    private void wifiOn(View view){
        // start connection to the server
        mWIFIStatus.setText("Nouvelle Connexion Wifi ...");
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

        recepServ = "";           // RAZ de la variable de "reponse du serveur"

        // envoie de la requete au serveur
        wifiSend(v, messagetx);        // View = null, requete = 0x1001

        Log.e("requeteNvPanier", "requeteNvPanier: " + messagetx);

        // attente pendant la connexion (sur un autre Thread)
        try {
            Thread.sleep(1000);
        }
        catch (Exception ex) {
            mWIFIStatus.setText("ERREUR recep requeteNvPanier");
        }
    }

    // envoie une trame demandant un nouveau panier
    private void requeteRecepNvPanier (String reponseServ) {
        String messagetx = new String(new byte[] {0x10, 0x01});

        //messagetx += reponseServ.substring(2, 2);     // recup ID Panier
        messagetx += reponseServ.substring(5, 6);     // recup QT Produit dans Panier

        // envoie de la requete au serveur
        wifiSend(null, messagetx);        // View = null, requete = 0x1001

        Log.e("requeteRecepNvPanier", "requeteRecepNvPanier");

        // attente pendant la connexion (sur un autre Thread)
        try {
            Thread.sleep(1000);
        }
        catch (Exception ex) {
            mWIFIStatus.setText("ERREUR recep requeteRecepNvPanier");
        }
    }

}
