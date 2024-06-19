# IoT Project: Realtime Temperature and Humidity Monitoring

## Deskripsi Proyek

Proyek ini adalah sistem monitoring suhu dan kelembaban secara real-time menggunakan NodeMCU ESP8266 dan sensor DHT11. Data yang diperoleh dari sensor ditampilkan di layar LCD dan dikirimkan ke server MQTT setiap 10 detik. Pengguna juga dapat mengontrol LED yang terhubung ke NodeMCU melalui antarmuka web.

## Fitur

### Monitoring Suhu dan Kelembaban
1. Menggunakan sensor DHT11 untuk membaca suhu dan kelembaban.
2. Menampilkan hasil pembacaan pada LCD.
3. Mengirimkan data ke server MQTT setiap 10 detik.

### Kontrol LED
1. Terdapat 3 LED yang terhubung ke pin D5, D6, dan D7.
2. LED dapat dinyalakan dan dimatikan melalui antarmuka web.

### Antarmuka Web
1. Menampilkan grafik suhu dan kelembaban secara real-time.
2. Menyediakan tombol untuk mengontrol LED.
3. Menggunakan WebSocket untuk komunikasi real-time dengan server Node.js.

## Struktur Proyek

```plaintext
IoT_Project_Akhir/
├── backend/
│   ├── node_modules/
│   ├── package.json
│   ├── package-lock.json
│   └── server.js
├── frontend/
│   ├── assets/
│   │   ├── css/
│   │   │   └── styles.css
│   │   ├── js/
│   │   │   └── script.js
│   └── index.html
├── nodemcu/
│   └── esp8266_suhu_kelembaban_led.ino
└── README.md
```

## Instalasi dan Penggunaan

### Persiapan Hardware
1. Hubungkan NodeMCU ESP8266 ke sensor DHT11.
2. Hubungkan LED ke pin D5, D6, dan D7 pada NodeMCU.
3. Hubungkan NodeMCU ke komputer melalui kabel USB.

### Persiapan Software

#### Backend
1. Navigasi ke direktori `backend`:
    ```sh
    cd backend
    ```
2. Instal dependensi dengan npm:
    ```sh
    npm install
    ```
3. Jalankan server:
    ```sh
    npm start
    ```

#### Frontend
1. Buka Laragon dan pastikan Apache/Nginx berjalan.
2. Navigasi ke direktori `frontend` dan buat konfigurasi virtual host di Laragon:
  1. Buka `Menu > Laragon > Nginx > sites-enabled`.
  2. Tambahkan file konfigurasi baru untuk proyek Anda (misalnya `iot_project.test.conf`):
      ```nginx
      server {
          listen 80;
          server_name iot_project.test;

          root C:/laragon/www/IoT_Project_Akhir/frontend;
          index index.html;

          location / {
              try_files $uri $uri/ /index.html;
          }

          location /ws/ {
              proxy_pass http://localhost:8080;
              proxy_http_version 1.1;
              proxy_set_header Upgrade $http_upgrade;
              proxy_set_header Connection "upgrade";
          }
      }
      ```
  3. Tambahkan entri ke file `hosts` di `C:\Windows\System32\drivers\etc\hosts`:
      ```plaintext
      127.0.0.1 iot_project.test
      ```

3. Restart Nginx melalui Laragon.

### Menggunakan Antarmuka Web
1. Buka browser dan akses `http://iot_project.test` untuk melihat data suhu dan kelembaban secara real-time.
2. Gunakan tombol untuk mengontrol LED.

## Kontribusi

1. Fork repositori ini.
2. Buat branch fitur Anda: `git checkout -b fitur-baru`.
3. Commit perubahan Anda: `git commit -am 'Tambah fitur baru'`.
4. Push ke branch: `git push origin fitur-baru`.
5. Buat Pull Request.

## Lisensi

Proyek ini dilisensikan di bawah [MIT License](LICENSE).
