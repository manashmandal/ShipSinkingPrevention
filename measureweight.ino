#define THRESHOLD 300

bool is_ready = false;

const byte ir = A0;
const byte pressure_sensor = A1;

int weight = 0;

int fsrReading;
int fsrVoltage;
unsigned long fsrResistance;
unsigned long fsrConductance;
long fsrForce;


void setup() {
  // put your setup code here, to run once:
  pinMode(ir, INPUT);
  pinMode(pressure_sensor, INPUT);
  Serial.begin(9600);
}


int weightReading()
{
  return analogRead(pressure_sensor);
}

bool personDetected()
{
  if (analogRead(ir) > THRESHOLD) return true;
  else return false;
}


void calculateFSRProperties()
{
  fsrReading = analogRead(pressure_sensor);
  Serial.println("Analog Reading = ");
  Serial.println(fsrReading);

  //analog reading to 5V measurement
  fsrVoltage = map(fsrReading, 0, 1023, 0, 5000);
  Serial.println("Voltage reading in mv = ");
  Serial.println(fsrVoltage);

   if (fsrVoltage == 0) Serial.println("No pressure");
   else {
    fsrResistance = 5000 - fsrVoltage;
    fsrResistance *= 10000;
    fsrResistance /= fsrVoltage;
    Serial.println("FSR resistance in ohms = ");
    Serial.println(fsrResistance);

    fsrConductance = 1000000;
    fsrConductance /= fsrResistance;
    Serial.print("Conductance in microOhms: ");
    Serial.println(fsrConductance);

    if (fsrConductance <= 1000){
      fsrForce = fsrConductance / 80;
      Serial.println("Force in Newtons: ");
      Serial.println(fsrForce);
    } else {
      fsrForce = fsrConductance - 1000;
      fsrForce /= 30;
      Serial.println("Force in Newtons: ");
      Serial.println(fsrForce);
    }
   }
   Serial.println("---------------");
   delay(250);
}

void loop() {
  calculateFSRProperties();
  if (personDetected()) {Serial.println("A person "); is_ready = true;}
  if (is_ready && fsrForce > 1){
    delay(1000);
    weight += fsrForce;    
    is_ready = false;
  }

  Serial.print("Weight :");
  Serial.println(weight);

  delay(1000);
}
