#include  <LiquidCrystal.h> 
#define Wifi_ismi "WiFi_Ismi"
#define Wifi_sifresi "WiFi_Sifresi"
#define IP "184.106.153.149" //thingspeak.com IP adresi
int ToprakPin = 6;
int veri;
int trigPin = 7;                                        //Ultrasonik sensör trig pini değişkeni
int echoPin = 6;                                        //Ultrasonik sensör echo pini değişkeni
int sure;                                               //Ses dalgasının gidip gelme süresi değişkeni
int uzaklik;                                            //Ölçülen uzaklık değeri değişkeni
int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2; 
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
 
void setup() {
   pinMode(trigPin, OUTPUT);                             //trig pinini OUTPUT olarak ayarlıyoruz.
   pinMode(echoPin,INPUT);                               //echo pinini INPUT olarak ayarlıyoruz.
   lcd.begin(16, 2);                                     
  pinMode (ToprakPin, INPUT);
  Serial.begin(115200); //Seriport'u açıyoruz. Güncellediğimiz 
  //ESP modülünün baudRate değeri 115200 olduğu için bizde Seriport'u 115200 şeklinde seçiyoruz

  Serial.println("AT"); //ESP modülümüz ile bağlantı kurulup kurulmadığını kontrol ediyoruz.

  delay(3000); //EPSP ile iletişim için 3 saniye bekliyoruz.

  if (Serial.find("OK")) { //esp modülü ile bağlantıyı kurabilmişsek modül "AT" komutuna "OK" komutu ile geri dönüş yapıyor.
    Serial.println("AT+CWMODE=1"); //esp modülümüzün WiFi modunu STA şekline getiriyoruz. Bu mod ile modülümüz başka ağlara bağlanabilecek.
    delay(2000);
    String baglantiKomutu = String("AT+CWJAP=\"") + Wifi_ismi + "\",\"" + Wifi_sifresi + "\"";
    Serial.println(baglantiKomutu);

    delay(5000);
  }
}

void loop() {
  //=============Uzaklık degeri===============//
   digitalWrite(trigPin, LOW);                           //Ultrasonik sensör ile ölçüm sekansını başlatıyoruz.
   delayMicroseconds(5);
   digitalWrite(trigPin, HIGH);
   delayMicroseconds(10);
   digitalWrite(trigPin, LOW);
   sure = pulseIn(echoPin, HIGH, 1160);                 //Ses dalgasının gidip gelme süresini ölçüyoruz.
   uzaklik= sure*0.0345/2;                               //Ölçülen süre ile uzaklık hesabı yapıyoruz.
   lcd.clear();                                          //LCD'deki eski yazılar temizlenir.
   lcd.setCursor(0, 0);                                  //LCD'nin 1. satır 1. sütunundan yazmaya başlıyoruz.      
   lcd.print("Uzaklik: | Sıcaklık:");                                
   lcd.setCursor(0, 1);                                  //LCD'nin 2. satır 1. sütunundan yazmaya başlıyoruz.
   lcd.print(uzaklik);                                   //Uzaklık değerini LCD'ye yazdırıyoruz.
   lcd.print("cm");
  // ===============Toprak Sensoru============//
  veri = digitalRead(ToprakPin);
  
    float veri;
    Serial.println(veri);
    verii(veri);
    
  if (veri == false) {
    Serial.println("Veri Yok");
    return;
    }  
    //==================Twitter kismi===========//  
 
 
}
void verii(float veri) {
  Serial.println(String("AT+CIPSTART=\"TCP\",\"") + IP + "\",80"); //thingspeak sunucusuna bağlanmak için bu kodu kullanıyoruz. 
  //AT+CIPSTART komutu ile sunucuya bağlanmak için sunucudan izin istiyoruz. 
  //TCP burada yapacağımız bağlantı çeşidini gösteriyor. 80 ise bağlanacağımız portu gösteriyor
  delay(1500);
  String tweet = "POST /apps/thingtweet/1/statuses/update HTTP/1.1\n";
  tweet += "Host: api.thingspeak.com\n";
  tweet += "Connection: close\n";
  tweet += "Content-Type: application/x-www-form-urlencoded\n";
  tweet += "Content-Length:50\r\n\r\napi_key=";
  tweet += String("HZ61GK32N3FG0TLM"); //ThingSpeak'den aldığımız Twitter API KEY'ini buraya yazıyoruz.
  tweet += String("&status=");
  tweet += String("Toprakta nem var"); // Yollamak istediğimiz tweeti buraya yazıyoruz.

  Serial.print("AT+CIPSEND="); //veri yollayacağımız zaman bu komutu kullanıyoruz. Bu komut ile önce kaç tane karakter yollayacağımızı söylememiz gerekiyor.
  delay(100);
  Serial.println(tweet.length());

  if (Serial.find(">")) { //eğer sunucu ile iletişim sağlayıp komut uzunluğunu gönderebilmişsek ESP modülü bize ">" işareti ile geri dönüyor.
    // arduino da ">" işaretini gördüğü anda sıcaklık verisini esp modülü ile thingspeak sunucusuna yolluyor.
    Serial.println(tweet);
    Serial.println("AT+CIPCLOSE=0");
    delay(100);

  } else {
    Serial.println("AT+CIPCLOSE=0");
  }
}
