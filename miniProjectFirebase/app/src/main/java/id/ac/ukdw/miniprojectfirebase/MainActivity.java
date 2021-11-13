package id.ac.ukdw.miniprojectfirebase;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import android.os.Bundle;

import android.widget.CompoundButton;
import android.widget.TextView;
import android.widget.ToggleButton;


import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

public class MainActivity extends AppCompatActivity {

    ToggleButton buttonPower;

    TextView status;
    TextView suhu;

    String valueStatus;
    String valueSuhu;

    DatabaseReference dref;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        buttonPower = (ToggleButton) findViewById(R.id.button);

        status = (TextView) findViewById(R.id.txtView_valueStatus);
        suhu = (TextView) findViewById(R.id.txtView_valueSuhu);

        dref = FirebaseDatabase.getInstance("https://fir-mini-3e80b-default-rtdb.asia-southeast1.firebasedatabase.app/").getReference();
        dref.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot snapshot) {
                valueSuhu = snapshot.child("Node1/suhu").getValue().toString();
                suhu.setText(valueSuhu);

                valueStatus = snapshot.child("Node1/status").getValue().toString();
                status.setText(valueStatus);
            }

            @Override
            public void onCancelled(@NonNull DatabaseError error) {

            }
        });
        buttonPower.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if(isChecked){
                    DatabaseReference dhtRef = FirebaseDatabase.getInstance("https://fir-mini-3e80b-default-rtdb.asia-southeast1.firebasedatabase.app/").getReference("Node1/dht");
                    dhtRef.setValue(1);
                }
                else
                {
                    DatabaseReference dhtRef = FirebaseDatabase.getInstance("https://fir-mini-3e80b-default-rtdb.asia-southeast1.firebasedatabase.app/").getReference("Node1/dht");
                    dhtRef.setValue(0);
                }
            }
        });
    }
}