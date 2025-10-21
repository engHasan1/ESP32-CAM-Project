# ESP32-CAM Hareket Algılama ve Telegram Uyarı Sistemi

Bu proje, **ESP32-CAM** modülü ve bir **PIR hareket sensörü** kullanarak, hareket algılandığında otomatik olarak bir fotoğraf çekip **Telegram** üzerinden gönderen bir güvenlik sistemi sunar.

---

## 🚀 Özellikler

- 📷 **ESP32-CAM ile fotoğraf çekimi**
- 📡 **WiFi bağlantısı üzerinden Telegram'a gönderim**
- 🔔 **Hareket algılandığında otomatik uyarı**
- 💾 **PSRAM desteğiyle yüksek çözünürlükte görüntü**
- ⚙️ **Basit ve anlaşılır yapılandırma**

---

## 🧰 Gerekli Donanım

| Bileşen | Açıklama |
|----------|-----------|
| ESP32-CAM | Kamera modülü (AI Thinker modeli önerilir) |
| PIR Sensör (HC-SR501) | Hareket algılama sensörü |
| FTDI USB Programlayıcı | ESP32-CAM'e kod yüklemek için |
| Jumper Kablolar | Bağlantılar için |

---

## 🔌 Devre Bağlantıları

| PIR Sensör Pin | ESP32-CAM Pin |
|----------------|---------------|
| VCC | 5V |
| OUT | GPIO 13 |
| GND | GND |

---

## ⚙️ Kurulum Adımları

1. Arduino IDE’yi açın ve **ESP32 kartlarını** yükleyin:  
   `Dosya → Tercihler → Ek Kart Yöneticisi URL’leri` alanına şu adresi ekleyin:  
   ```
   https://dl.espressif.com/dl/package_esp32_index.json
   ```
2. **Araçlar → Kart** menüsünden `AI Thinker ESP32-CAM` seçin.
3. Gerekli kütüphaneleri kurun:
   ```cpp
   UniversalTelegramBot
   ArduinoJson
   WiFi
   WiFiClientSecure
   ```
4. Aşağıdaki değişkenleri kendi bilgilerinizle güncelleyin:
   ```cpp
   const char* ssid = "WiFi_ADINIZ";
   const char* password = "WiFi_SIFRENIZ";
   String BOTtoken = "TELEGRAM_BOT_TOKENINIZ";
   String CHAT_ID = "TELEGRAM_CHAT_ID";
   ```
5. Kodu yükledikten sonra, **Serial Monitor** üzerinden bağlantı durumunu kontrol edin.

---

## 📷 Nasıl Çalışır

1. PIR sensörü bir hareket algıladığında ESP32-CAM devreye girer.  
2. Anında bir fotoğraf çeker.  
3. Fotoğrafı Telegram üzerinden önceden belirlenmiş kullanıcıya gönderir.  
4. 10 saniyelik bekleme süresi sonrasında tekrar aktif hale gelir.

---

## 🧠 Önemli Notlar

- Telegram BOT’unuzu [@BotFather](https://t.me/BotFather) üzerinden oluşturun.
- CHAT_ID’nizi öğrenmek için bu bota mesaj gönderin: [@userinfobot](https://t.me/userinfobot)
- Güç kaynağınızın en az **5V - 2A** olmasına dikkat edin.
- Eğer görüntü gelmiyorsa, kameranın bağlantı pinlerini kontrol edin.

---

## 📸 Örnek Seri Monitör Çıktısı

```
📡 WiFi ağına bağlanılıyor...
✅ Ağa başarıyla bağlandı!
📶 IP: 192.168.1.100
📸 Kamera kullanıma hazır!
🚨 Hareket algılandı!
📤 Fotoğraf Telegram’a gönderiliyor...
✅ Fotoğraf Telegram’a başarıyla gönderildi
```

---

## 🧑‍💻 Geliştirici

**Hasan Elrecep**  
📧 [elrecephasan@gmail.com](mailto:elrecephasan@gmail.com)

---

## 🪪 Lisans

Bu proje açık kaynaklıdır ve kişisel veya eğitim amaçlı kullanım için serbesttir.