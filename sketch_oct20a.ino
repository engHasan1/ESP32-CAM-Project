#include "esp_camera.h"
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include "FS.h"
#include "SPIFFS.h"

// ===== WiFi Ayarları =====
const char* ssid = "WİFİ-Adı";
const char* password = "WİFİ-Şifresi";

// ===== Telegram Bot Bilgileri =====
String BOTtoken = "Telegram Bot Token";
String CHAT_ID = "ChatID";

WiFiClientSecure clientTCP;
UniversalTelegramBot bot(BOTtoken, clientTCP);

// ===== Hareket Sensörü =====
#define PIR_PIN 13

// ===== Kamera Ayarları (AI Thinker ESP32-CAM) =====
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

// Genel değişkenler (fotoğraf için)
camera_fb_t *current_fb = NULL;
int photo_index = 0;

// Daha fazla veri olup olmadığını kontrol et
bool isMoreDataAvailable() {
  return (photo_index < current_fb->len);
}

// Sonraki baytı oku
uint8_t getNextByte() {
  return current_fb->buf[photo_index++];
}

// ===== Fotoğraf Çekme ve Gönderme Fonksiyonu =====
void captureAndSendPhoto() {
  current_fb = esp_camera_fb_get();
  if (!current_fb) {
    Serial.println("❌ Kamera görüntüsü alınamadı");
    return;
  }

  Serial.println("📤 Fotoğraf Telegram’a gönderiliyor...");
  
  photo_index = 0; // İndeksi sıfırla

  // Callback fonksiyonlarıyla gönder
  String sent = bot.sendPhotoByBinary(CHAT_ID, "image/jpeg", current_fb->len,
                                      isMoreDataAvailable, 
                                      getNextByte,
                                      nullptr,
                                      nullptr);
  
  esp_camera_fb_return(current_fb);
  current_fb = NULL;

  if (sent == "") {
    Serial.println("❌ Fotoğraf gönderimi başarısız");
  } else {
    Serial.println("✅ Fotoğraf Telegram’a başarıyla gönderildi");
  }
}

// ===== Başlatma Ayarları =====
void setup() {
  Serial.begin(115200);
  pinMode(PIR_PIN, INPUT);

  // WiFi ağına bağlan
  WiFi.begin(ssid, password);
  Serial.print("📡 WiFi ağına bağlanılıyor");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\n✅ Ağa başarıyla bağlandı!");
  Serial.print("📶 IP: ");
  Serial.println(WiFi.localIP());

  // Kamera ayarları
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  // Telegram’a gönderim için bellek optimizasyonu
  if(psramFound()){
    config.frame_size = FRAMESIZE_SVGA;  // 800x600
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_VGA;   // 640x480
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("❌ Kamera başlatılamadı, hata kodu 0x%x\n", err);
    return;
  }

  // Görüntü kalitesini düşür (gönderimi hızlandırmak için)
  sensor_t * s = esp_camera_sensor_get();
  s->set_brightness(s, 0);     // -2 ile 2 arasında
  s->set_contrast(s, 0);       // -2 ile 2 arasında
  s->set_saturation(s, 0);     // -2 ile 2 arasında

  clientTCP.setInsecure();
  Serial.println("📸 Kamera kullanıma hazır!");
}

// ===== Ana Döngü =====
void loop() {
  int motion = digitalRead(PIR_PIN);
  if (motion == HIGH) {
    Serial.println("🚨 Hareket algılandı!");
    captureAndSendPhoto();
    delay(10000); // 10 saniye bekle (tekrarı önlemek için)
  }
  delay(100); // İşlemci kullanımını azaltmak için kısa bekleme
}
