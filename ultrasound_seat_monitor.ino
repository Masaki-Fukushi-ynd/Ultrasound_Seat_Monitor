#define TRIGGER_PIN 25
#define ECHO_PIN 26
#define V 340
#define CHECK_DISTANCE 50 // 空席と判断する距離の閾値 (50 cm以上)
#define CHECK_TIME 10000 // 状態確認の持続時間 (ミリ秒)

unsigned long time_started = 0; // 状態変更開始時間
bool seat_occupied = false;  // 初期状態を空席と仮定

void setup() {
  Serial.begin(115200);
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  digitalWrite(TRIGGER_PIN, LOW);
}

long measureDistance() {
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);
  long distance = V * duration / 20000;
  Serial.println(distance);
  return distance;
}

void checkSeatStatus() {
  long distance = measureDistance();

  if ((distance >= CHECK_DISTANCE && seat_occupied) || (distance < CHECK_DISTANCE && !seat_occupied)) {
    if (time_started == 0) {
      time_started = millis();
    } else if (millis() - time_started > CHECK_TIME) {
      seat_occupied = !seat_occupied;
      time_started = 0;
    }
  } else {
    time_started = 0;
  }
}

void loop(){
  checkSeatStatus();
  if(seat_occupied){
    Serial.println("full");
  }else{
    Serial.println("empty");
  }
  delay(1000);
}
