package com.example.marchdecachan;

// Toast type color & Icon : https://www.codingdemos.com/android-toast-message-tutorial/

import android.animation.TypeConverter;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.graphics.Color;
import android.os.Handler;
import android.os.SystemClock;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.UnsupportedEncodingException;
import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.Set;
import java.util.UUID;

// Page Liste Produits : Connexion BLuetooth
public class FenProduits extends AppCompatActivity {

    // decla Bluetooth
    private Button mBtnConnexionBluetooth;
    private TextView mBluetoothStatus;
    private BluetoothAdapter mBTAdapter;
    private Set<BluetoothDevice> mPairedDevices;
    private ArrayAdapter<String> mBTArrayAdapter;
    private TextView mReadBuffer;

    private final String TAG = MainActivity.class.getSimpleName();
    private Handler mHandler; // Our  main handler that will receive callback notifications
    private ConnectedThread mConnectedThread; // bluetooth background worker thread to send and receive data
    private BluetoothSocket mBTSocket = null; // bi-directional client-to-client data path

    private boolean BTConnecte;
    private static final UUID BTMODULEUUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");       // "random" unique identifier

    // #defines for identifying shared types between calling functions
    private final static int REQUEST_ENABLE_BT = 1;     // used to identify adding bluetooth names
    private final static int MESSAGE_READ = 2;          // used in bluetooth handler to identify message update
    private final static int CONNECTING_STATUS = 3;     // used in bluetooth handler to identify message status

    private Thread threadCoBt;

    // Produits
    private TextView titreProduit;      // textView_produit
    private TextView QtProduit;         // textView_quantite
    private TextView produitEmpl;       // textView_emplacement
    private ListView listeProduits;     // Liste_Produit
    private ArrayAdapter<String> mArrayAdapterListProduitScannes;
    private ArrayList<Produit> mProduitsArrayList;            // panier

    public Button click_buttonValidCommand;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_fen_produits);

        // Gestion Bluetooth
        mBtnConnexionBluetooth = (Button) findViewById(R.id.button_Connexion);
        mBTAdapter = BluetoothAdapter.getDefaultAdapter(); // get a handle on the bluetooth radio
        mBTArrayAdapter = new ArrayAdapter<String>(this,android.R.layout.simple_list_item_1);
        mBluetoothStatus = (TextView)findViewById(R.id.ID_textView_BTStatus);

        mReadBuffer = (TextView)findViewById(R.id.ID_readBuffer);
        BTConnecte = false;

        // gestion produits
        titreProduit = (TextView)findViewById(R.id.textView_produit);
        QtProduit = (TextView)findViewById(R.id.textView_quantite);
        produitEmpl = (TextView)findViewById(R.id.textView_emplacement);
        listeProduits = (ListView)findViewById(R.id.Liste_Produit);
        mArrayAdapterListProduitScannes = new ArrayAdapter<String>(this,android.R.layout.simple_list_item_1);
        listeProduits.setAdapter(mArrayAdapterListProduitScannes);       // ajoute le tableau a la liste visuelle
        mArrayAdapterListProduitScannes.clear();


        // getion premiere fenetre
        final Button click_buttonValidCommand = (Button) findViewById(R.id.button_ValidCommande);
        final Intent monIntent2 = new Intent(this, MainActivity.class);         // lien avec la 2eme page

        final Intent intent1 = getIntent();
        String msgServeur = intent1.getStringExtra(MainActivity.panier);        // ex trame : "** 2b 02 4a9cc . 02 / 0e3120 . 01 / \0"


        // creation du panier
        mProduitsArrayList = new ArrayList<Produit>();

        int qtTot = Integer.parseInt(msgServeur.substring(4, 6));     // recup QT[2] total panier

        msgServeur = msgServeur.substring(6);       // suppr : ID_NPanier / ID_Panier / QT_Total

        // tri des produits
        for(int i = 0; i< qtTot; i++) {
            Produit prod = new Produit();
            String produit = msgServeur.substring(0 + (i*10), 6 + (i*10));
            String quantite = msgServeur.substring(7 + (i*10), 9 + (i*10));

            prod.init_Produit(produit, Integer.parseInt(quantite));
            mProduitsArrayList.add(prod);
        }

        // ouvre la Deuxieme fenetre : Liste des Produits
        click_buttonValidCommand.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                Produit prod;
                try {           // empeche la validation tant que la commande est incomplete
                    prod = mProduitsArrayList.get(0);
                    Toast toastValid = Toast.makeText(getApplicationContext(), "Commande Non Complete\nProchain Produit: " + prod.nom_Produit, Toast.LENGTH_SHORT);      // color + icon
                    toastValid.show();
                }
                catch (Exception e) {
                    Toast toastValid = Toast.makeText(getApplicationContext(), "Valide Commande", Toast.LENGTH_SHORT);      // color + icon
                    toastValid.show();

                    // deconnecte le Bluetooth
                    mConnectedThread.cancel();

                    // charge l'autre fenetre
                    startActivity(monIntent2);
                }
            }
        });

        //titreProduit.setText("Produit");
        //QtProduit.setText("10000");
        changementProduitAScanner();            // selectionne le premier produit de la liste

        // recep & aff code barre scanne
        mHandler = new Handler(){
            public void handleMessage(android.os.Message msg){
                if(msg.what == MESSAGE_READ){
                    // recup le produit scanne
                    String readMessage = null;          // => ID
                    try {
                        readMessage = new String((byte[]) msg.obj, "UTF-8");
                    } catch (UnsupportedEncodingException e) {
                        e.printStackTrace();
                    }
                    mReadBuffer.setText(readMessage);

                    Produit lastProd = mProduitsArrayList.get(0);           // recup du produit a scanne

                    // test du nom pour affichage du produit dans la liste des valides (+ passage par table de correspondance)
                    //if(titreProduit.getText().toString().trim().equals(readMessage.trim())) {
                    if(lastProd.ID_Produit.trim().equals(readMessage.trim())) {
                        if(mArrayAdapterListProduitScannes != null) {
                            mArrayAdapterListProduitScannes.add(lastProd.nom_Produit);      // ajout liste produits scannes et valides
                            mArrayAdapterListProduitScannes.notifyDataSetChanged();

                            // aff msg Quantite
                            verifQuantiteProduit(QtProduit.getText().toString().trim());

                            // validation produit
                            validationProduit(readMessage.trim());
                        }
                        //else ToastMsgErreur("ERREUR SCAN:\nAdapter List = Null");
                    }
                    else {
                        // creation d'un nouveau produit pour aff l'erreur (Mauvais produit)
                        //Produit scannedProd = new Produit();
                        //scannedProd.init_Produit(readMessage.trim(), 0);

                        ToastMsgErreur("ERREUR SCAN:\nID Produit attendu = " + lastProd.ID_Produit +
                                /*"\nProduit attendu = " + titreProduit.getText() + */
                                "\nID Produit scanne = " + readMessage.trim() /*+
                                "\nProduit scanne = " + scannedProd.nom_Produit*/);
                    }
                }

                if(msg.what == CONNECTING_STATUS){
                    if(msg.arg1 == 1)
                        mBluetoothStatus.setText("Connecté à: " + (String)(msg.obj));
                    else
                        mBluetoothStatus.setText("Echec de la Connection");
                }
            }
        };

        // lance la connexion a l'ouverture de la fenetre
        autoConnectScanner(null);

        // gestion BLuetooth
        mBtnConnexionBluetooth.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                autoConnectScanner(v);
            }
        });

        // recupere le panier
        //remplissage_Panier(msgServeur);

    } // Fin onCreate

    // msg de rappel de Quantite
    private void verifQuantiteProduit (String Qt) {
        if(!Qt.equals("1")) {
            Toast toastQt = Toast.makeText(getApplicationContext(), "Attention Quantité attendue: " + Qt, Toast.LENGTH_SHORT);      // color + icon
            toastQt.show();
        }
    }

    // modification de l'interface pour aff le prochain produit
    private boolean changementProduitAScanner () {
        Produit lastProd = null;

        try {
            lastProd = mProduitsArrayList.get(0);       // recupere le (nouveau) premier produit de la liste
        }
        catch (Exception e) {           // cas du panier vide
            Toast toast = Toast.makeText(getApplicationContext(), "Panier Vide", Toast.LENGTH_SHORT);      // color + icon
            toast.show();
            titreProduit.setText("Panier Vide");
            QtProduit.setText("0");
            produitEmpl.setText("..");
            return false;
        }

        titreProduit.setText(lastProd.nom_Produit);             // pas d'aff de l'ID
        QtProduit.setText(Integer.toString(lastProd.QT_produit));
        produitEmpl.setText(lastProd.emplacement_Produit);
        return true;
    }

    // valide le produit scanne et le retire de la liste si c'est celui attendu
    private void validationProduit (String produitScanne) {
        Toast toast;
        Produit lastProd = null;

        try {
            lastProd = mProduitsArrayList.get(0);
        }
        catch (Exception e) {
            toast = Toast.makeText(getApplicationContext(), "validationProduit: Panier Vide", Toast.LENGTH_SHORT);      // color + icon
            toast.show();
            return;
        }

        // (+ ajouter passage par table de correspondance)

        // verif produit scanne = premier produit de la liste
        if(lastProd.ID_Produit.trim().equals(produitScanne.trim())) {
            lastProd.produit_Valide = true;
            mProduitsArrayList.remove(lastProd);

            // changement : nouveau produit a scanner
            changementProduitAScanner();
        }
    }

    // recupere la liste de tous les produits et de leurs quantites issu du Serveur
    private void remplissage_Panier (String msgServ) {
        int start = 4;
        int position = 12;      // init apres premier item
        int qt=000;
        String id = "000000";

        for(int i=0; i<msgServ.length(); i++) {
            // produit
            if (msgServ.charAt(i) == '.') {
                position = i;

                String substr = msgServ.substring(start, position);
                id = substr;
                start = position + 1;
            }

            // quantite
            else if (msgServ.charAt(i) == '/') {
                position = i;

                String substr = msgServ.substring(start, position);
                qt = Integer.parseInt(substr);

                // creation du nouveau produit
                Produit produit= new Produit ();

                // recup nom_Produit depuis liste Correspondance

                produit.init_Produit(id, qt);       // nom produit

                mProduitsArrayList.add(produit);    // ajoute un produit au panier

                start = position + 1;
            }
        }
    }


    // rassemble les etapes de la connexion bluetooth automatique au Scanner
    private boolean autoConnectScanner (View v) {
        if(bluetoothOn(v)) {
            listPairedDevices(v);
            if (BTConnecte == false)
                discover(v);
        }

        if(BTConnecte)
            return true;
        else return false;
    }


    // creer un message
    private void ToastMsgErreur(String msg) {
        Toast toastErreur = Toast.makeText(getApplicationContext(),msg,Toast.LENGTH_SHORT);
        View view = toastErreur.getView();
        view.setBackgroundColor(Color.RED);
        toastErreur.show();
    }

    // Active le Bluetooth
    private boolean bluetoothOn(View view){
        try {
            if (!mBTAdapter.isEnabled()) {
                Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
                startActivityForResult(enableBtIntent, REQUEST_ENABLE_BT);
            }
            mBluetoothStatus.setText("Bluetooth activé");
            return true;
        }
        catch (Exception e) {
            ToastMsgErreur("ERREUR Bluetooth:\nConnexion Bluetooth");
            return false;
        }
    }

    // recupere les appareils deja appaires
    private void listPairedDevices(View view){
        mBTArrayAdapter.clear();
        mPairedDevices = mBTAdapter.getBondedDevices();
        if(mBTAdapter.isEnabled()) {
            // put it's one to the adapter
            for (BluetoothDevice device : mPairedDevices) {
                mBTArrayAdapter.add(device.getName() + "\n" + device.getAddress());

                if (device.getAddress().equals("00:1C:97:14:35:02")==true) {               // recherche l'appareil avec la meme addr MAC (= Wasp Barcode)
                    // Connexion à l'appareil
                    BluetoothConnectionDevice(device.getAddress());           // connecte le Scanner s'il est detecte
                    mBluetoothStatus.setText("Appairé à " + device.getName());
                    BTConnecte = true;
                }
                //else mBluetoothStatus.setText("Appareil trouvé: " + device.getName());
            }
        }
        else
            Toast.makeText(getApplicationContext(), "Bluetooth NON Activé", Toast.LENGTH_SHORT).show();
    }


    // Recherche tous les peripheriques existants (avec BLuetooth)
    private void discover(View view){
        // Check if the device is already discovering
        if(mBTAdapter.isDiscovering()){
            mBTAdapter.cancelDiscovery();
            Toast.makeText(getApplicationContext(),"Arret de la Recherche en cours",Toast.LENGTH_SHORT).show();
        }
        else{
            if(mBTAdapter.isEnabled()) {
                //mBTArrayAdapter.clear(); // clear items
                mBTAdapter.startDiscovery();
                //Toast.makeText(getApplicationContext(), "Nouvelle Recherche", Toast.LENGTH_SHORT).show();
                registerReceiver(blReceiver, new IntentFilter(BluetoothDevice.ACTION_FOUND));
            }
            else{
                Toast.makeText(getApplicationContext(), "Bluetooth NON Activé", Toast.LENGTH_SHORT).show();
            }
        }
    }

    // Recuperation des appareils Bluetooth dans une "liste"
    final BroadcastReceiver blReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();
            if(BluetoothDevice.ACTION_FOUND.equals(action)){
                if(BTConnecte == false) {
                    BluetoothDevice device = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
                    // add the name to the list
                    //mBTArrayAdapter.add(device.getName() + "\n" + device.getAddress());
                    //mBTArrayAdapter.notifyDataSetChanged();
                    if (device.getAddress().equals("00:1C:97:14:35:02")) {               // recherche l'appareil avec la meme addr MAC (= Wasp Barcode)
                        // Connexion à l'appareil
                        BluetoothConnectionDevice(device.getAddress());           // connecte le Scanner s'il est detecte
                        mBluetoothStatus.setText("Connecté à " + device.getName());
                        BTConnecte = true;
                    }
                }
            }
        }
    };


    // creation Communication Bluetooth
    private BluetoothSocket createBluetoothSocket(BluetoothDevice device) throws IOException {
        try {
            final Method m = device.getClass().getMethod("createInsecureRfcommSocketToServiceRecord", UUID.class);
            return (BluetoothSocket) m.invoke(device, BTMODULEUUID);
        } catch (Exception e) {
            Log.e(TAG, "Could not create Insecure RFComm Connection",e);
        }
        return  device.createRfcommSocketToServiceRecord(BTMODULEUUID);
    }

    // connexion de l'appareil en Bluetooth
    private void BluetoothConnectionDevice (final String addrBTDevice) {
        if(!mBTAdapter.isEnabled()) {
            Toast.makeText(getBaseContext(), "Bluetooth not on", Toast.LENGTH_SHORT).show();
            return;
        }

        mBluetoothStatus.setText("Connecting...");

        threadCoBt = new Thread()
        {
            public void run() {
                boolean fail = false;

                BluetoothDevice device = mBTAdapter.getRemoteDevice(addrBTDevice);

                try {
                    mBTSocket = createBluetoothSocket(device);
                } catch (IOException e) {
                    fail = true;
                    ToastMsgErreur("ERREUR Creation Socket");
                }

                // Establish the Bluetooth socket connection.
                try {
                    mBTSocket.connect();
                } catch (IOException e) {
                    try {
                        fail = true;
                        mBTSocket.close();
                        mHandler.obtainMessage(CONNECTING_STATUS, -1, -1).sendToTarget();
                    } catch (IOException e2) {
                        //insert code to deal with this
                        ToastMsgErreur("ERREUR Creation Socket");
                    }
                }
                if(fail == false) {
                    mConnectedThread = new ConnectedThread(mBTSocket);
                    mConnectedThread.start();

                    mHandler.obtainMessage(CONNECTING_STATUS, 1, -1, device.getName()).sendToTarget();
                }
            }
        };

        threadCoBt.start();
    }


    // Class Thread : Nouvelle connexion Bluetooth a un appareil
    private class ConnectedThread extends Thread {
        private final BluetoothSocket mmSocket;
        private final InputStream mmInStream;
        private final OutputStream mmOutStream;

        public ConnectedThread(BluetoothSocket socket) {
            mmSocket = socket;
            InputStream tmpIn = null;
            OutputStream tmpOut = null;

            // Get the input and output streams, using temp objects because member streams are final
            try {
                tmpIn = socket.getInputStream();
                tmpOut = socket.getOutputStream();
            } catch (IOException e) { }

            mmInStream = tmpIn;
            mmOutStream = tmpOut;
        }

        public void run() {
            byte[] buffer = new byte[1024];  // buffer store for the stream
            int bytes; // bytes returned from read()
            // Keep listening to the InputStream until an exception occurs
            while (true) {
                try {
                    // Read from the InputStream
                    bytes = mmInStream.available();
                    if(bytes != 0) {
                        buffer = new byte[1024];
                        SystemClock.sleep(1000); //pause and wait for rest of data. Adjust this depending on your sending speed.
                        bytes = mmInStream.available(); // how many bytes are ready to be read?
                        bytes = mmInStream.read(buffer, 0, bytes); // record how many bytes we actually read
                        mHandler.obtainMessage(MESSAGE_READ, bytes, -1, buffer)
                                .sendToTarget(); // Send the obtained bytes to the UI activity
                    }
                } catch (IOException e) {
                    e.printStackTrace();

                    break;
                }
            }
        }

        /* Call this from the main activity to send data to the remote device */
        public void write(String input) {
            byte[] bytes = input.getBytes();           //converts entered String into bytes
            try {
                mmOutStream.write(bytes);
            } catch (IOException e) { }
        }

        /* Call this from the main activity to shutdown the connection */
        public void cancel() {
            try {
                mmSocket.close();
            } catch (IOException e) { }
        }
    }


}
