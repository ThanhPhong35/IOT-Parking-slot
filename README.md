# IOT-Parking-slot
BTL IOT_Parking slot

Thành viên và phân chia công việc
Thành viên: Nguyễn Thị Quyên - B20DCAT152
		          
	    Trần Đăng Trọng - B20DCAT191
            
Phân công: Nguyễn Thị Quyên: lên ý tưởng, thiết kế mạch, mô hình mạch, lập trình webserver
           
	   Trần Đăng Trọng: triển khai phần cứng, lắp mạch, lập trình webserver, báo cáo

Ý tưởng
- Mô hình sử dụng web để quản lý bãi đỗ xe.
- Có thẻ giữ xe
- Trang web hiển thị vị trí để xe trống
- web có giao diện đăng nhập
- web có 2 chế độ wifi là accesspoint and station mode. web cho phép admin thay đổi chế độ phát hay kết nối Wifi.
- web cho phép admin thêm, sửa, xóa thẻ giữ xe.
  
Linh kiện sử dụng
- Vi điều khiển esp32     2
- Module thu phát hồng ngoại MHIR01      8
- Mạch RFID NFC    1
- Motor Servo    2
- Buzzer    1
- Nút ấn   1

Thư viện sử dụng
- NTPClient.h: Thư viện cho phép kết nối và truy vấn thời gian từ một máy chủ
- NTP (Network Time Protocol).
- WiFiUdp.h: Thư viện cho phép sử dụng giao thức UDP (User Datagram Protocol) để gửi và nhận dữ liệu qua mạng Wi-Fi.
- ArduinoJson.h: Thư viện cho phép phân tích và tạo các đối tượng JSON.
- EEPROM.h: Thư viện cho phép ghi và đọc dữ liệu trực tiếp vào và từ bộ nhớ EEPROM (Electrically Erasable Programmable Read-Only Memory).
- FS.h và SPIFFS.h: Thư viện cho phép đọc và ghi dữ liệu vào bộ nhớ flash bên trong của nó.
- WiFi.h: Thư viện cho phép kết nối và giao tiếp với mạng Wi-Fi.
- WebServer.h: Thư viện cho phép tạo và quản lý một máy chủ web.
- WebSocketsServer.h: Thư viện cho phép tạo và quản lý một máy chủ WebSocket.
- webServer: Đối tượng WebServer để quản lý máy chủ web.
- webSocket: Đối tượng WebSocketsServer để quản lý máy chủ WebSocket
- SS_PIN và RST_PIN: Các chân kết nối với module đọc thẻ RFID MFRC522
- mfrc522: Đối tượng MFRC522 để quản lý module đọc thẻ RFID MFRC522
- Buzzer: Chân kết nối với còi chip.
- Servo.h: Thư viện cho phép Arduino điều khiển các động cơ servo
- Wire.h: Thư viện cho phép sử dụng giao thức I2C để giao tiếp với các thiết bị khác.
- LiquidCrystal_I2C.h: Thư viện cho phép giao tiếp với một module LCD 20x4 thông qua giao thức I2C.

Link báo cáo: https://docs.google.com/document/d/1GeTb_3yTSkK9aaes8IF583ZFMiAY2S8L/edit?usp=drivesdk&ouid=106224551968194405189&rtpof=true&sd=true

Link youtube: 
