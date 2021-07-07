#include "main.h" //khai báo thư biện định nghĩa
void setup() {
  //khai báo chân của sonar
  pinMode(trigPin, OUTPUT); //chân trig
  pinMode(echoPin, INPUT); //chân echo
  //khai báo chân input
  pinMode(btn_Pin_Led1, INPUT); //nút nhấn led1
  pinMode(btn_Pin_Led2, INPUT); //nút nhấn led 2
  pinMode(btn_Pin_Bom, INPUT); //nút nhấn bơm
  pinMode(btn_Pin_Func, INPUT); //nút nhấn function
  pinMode(TdsSensorPin, INPUT); //cảm biến tds

  //khai báo output
  pinMode(pin_Led_Status, OUTPUT); //led blink trạng thái
  pinMode(pin_Led1, OUTPUT); //đèn 1
  pinMode(pin_Led2, OUTPUT); //đèn 2
  pinMode(pin_Bom, OUTPUT); //bơm
  pinMode(pin_Func, OUTPUT); // relay setup function tùy chọn

  //cài đặt trạng thái mặc định
  digitalWrite(pin_Led1, LOW);
  digitalWrite(pin_Led2, LOW);
  digitalWrite(pin_Bom, LOW);
  digitalWrite(pin_Func, LOW);

  Serial.begin(115200); //khởi tạo serial với máy tính
  inputString.reserve(255); //set kích thước chuỗi
  Serial.println("Start...");

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { //Kiểm tra khởi động của LCD
    //    Serial.println(F("SSD1306 allocation failed"));
    //    while (1); //debug màn hình
  } else {//khởi động thành công thông báo lời chào
    //    Serial.println(F("Screen stated! "));
    display.clearDisplay(); //Xóa màn hình
    display.setTextSize(1); //cỡ chữ
    display.setTextColor(WHITE); //màu chữ
    display.setCursor(24, 0);//vị trí con trỏ
    display.print(F("AQUAPONIC 2020"));//text in ra
    display.setCursor(0, 12);
    display.print(F("Hoang Long"));
    display.setCursor(0, 24);
    display.print(F("Hong Ngoc"));
    display.setCursor(40, 36);
    display.print(F("WELLCOME"));
    display.display();
    delay(3000);
  }
  ds18b20.begin(); //khởi tạo cảm biến nhiệt độ
  ds18b20.setWaitForConversion(false); //giảm độ trễ của ds18b20-- bí thuật
  //  Serial.println("start Done");
  // khởi động serial thành công...
  //  Serial.println("Kiểm tra trạng thái");
  //  Serial.println(btn_Status_Led1);
  //  Serial.println("Update muc nuoc...");
}
void ControlDevice() { //hàm điều khiển thiết bị

  //device 1
  if (dec1 != led1_Status) {//kiểm tra trạng thái cũ với trạng thái mới của thiết bị
    if (led1_Status == ON) { //kiểm tra trạng thái hiện tại
      digitalWrite(pin_Led1, HIGH); //điều khiển thiết bị
    } else {
      digitalWrite(pin_Led1, LOW);
    }
    dec1 = led1_Status; //update trạng thái biến event
    UpdateSTT(); //update trạng thái về server
  }

  //device 2
  if (dec2 != led2_Status) {
    if (led2_Status == ON) {
      digitalWrite(pin_Led2, HIGH);
    } else {
      digitalWrite(pin_Led2, LOW);
    }
    dec2 = led2_Status; //update trạng thái biến event
    UpdateSTT(); //update trạng thái về server
  }
  //device 3
  if (dec3 != bom_Status) {
    if (bom_Status == ON) {
      digitalWrite(pin_Bom, HIGH);
    } else {
      digitalWrite(pin_Bom, LOW);
    }
    dec3 = bom_Status; //update trạng thái biến event
    UpdateSTT();//update trạng thái về server
  }
}

//update data server
void serialEvent() { //event có data
  while (Serial.available()) {
    // lấy byte mới
    char inChar = (char)Serial.read();
    // cộng vào chuỗi
    inputString += inChar;
    if (inChar == '\n') { //phát hiện xuống dòng thì chuyển trạng thái chuỗi đã hoàn tất
      stringComplete = true;
    }
  }
}
//lắng nghe cmd và trả về dữ liệu
void CommandListen() {
  if (stringComplete) { //trạng thái đã nhận xong chuỗi
    int ind; //biến lấy vị trí của kí tự ngăn cách
    String AT = inputString.substring(0, inputString.indexOf("=")); //lấy chuỗi con từ đầu đến vị trí dấu =
    if (AT == "AT" || AT == "at") {//kiểm tra tiền tố lệnh có đúng là at hay không
      String resp = ""; //khởi tạo biến resp= rỗng
      String cmd = inputString.substring(inputString.indexOf("=") + 1, inputString.length() - 1);//nếu đúng at thì kiểm tra lệnh
      if (cmd == "all") { //lấy tất cả giá trị
        resp = "{\"ctr\":[";
        resp += led1_Status;
        resp += ",";
        resp += led2_Status;
        resp += ",";
        resp += bom_Status;
        resp += ",";
        resp += relay4_Status;
        resp += "],\"data\":[";
        resp += tdsValue; //data tds
        resp += ",";
        resp += temperature; //datta nhiệt độ
        resp += ",";
        resp += phValue; //data PH
        resp += ",";
        resp += 30; // độ mở của module cho cá ăn
        resp += "]}";
      } else if (cmd == "ctr") { //lấy giá trị điều khiển
        resp = "{\"ctr\":[";
        resp += led1_Status;
        resp += ",";
        resp += led2_Status;
        resp += ",";
        resp += bom_Status;
        resp += ",";
        resp += relay4_Status;
        resp += "]}";
      } else if (cmd == "data") { //lấy dữ liệu cảm biến
        resp += "{\"data\":[";
        resp += tdsValue; //data tds
        resp += ",";
        resp += temperature; //datta nhiệt độ
        resp += ",";
        resp += phValue; //data PH
        resp += ",";
        resp += 30; // độ mở của module cho cá ăn
        resp += "]}";
      } else if (cmd == "temp") { //lấy nhiệt độ
        resp = "{\"temp\":";
        resp += temperature;
        resp += "}";
      } else if (cmd == "ph") { //lấy ph
        resp = "{\"ph\":";
        resp += phValue;
        resp += "}";
      } else if (cmd == "tds") { //lấy nồng độ chất tan
        resp = "{\"tds\":";
        resp += tdsValue;
        resp += "}";
      } else {
        resp = "CMD fail";
      }
      Serial.println(resp);//trả về dữ liệu
    }
    inputString = ""; //set biến dữ liệu
    stringComplete = false; //reset trạng thái dữ liệ
  }
}
void UpdateSTT() {//cập nhật tất cả dữ liệu cần
  String resp = "";
  resp = "{\"ctr\":[";
  resp += led1_Status;
  resp += ",";
  resp += led2_Status;
  resp += ",";
  resp += bom_Status;
  resp += ",";
  resp += relay4_Status;
  resp += "],\"data\":[";
  resp += tdsValue; //data tds
  resp += ",";
  resp += temperature; //datta nhiệt độ
  resp += ",";
  resp += phValue; //data PH
  resp += ",";
  resp += 30; // độ mở của module cho cá ăn
  resp += "]}";
  Serial.println(resp);
}

void ReadStatusButtonInLoop() {//đọc trạng thái nút nhấn trong loop
  //đọc button 1
  if (digitalRead(btn_Pin_Led1) == HIGH) { //Kiểm tra trạng thái của nút đã được nhấn
    timePressBtn1 = (unsigned long)(millis() - pressTime); //Khởi tạo thời gian nhấn nút
    if (timePressBtn1 < 500 && timePressBtn1 > 50 ) { //Kiểm tra thời gian nhấn nút
      //nút được nhấn
      btn_Status_Led1 = PRESS; //gán biến trạng thái nút nhấn của đối tượng là đã được nhấn
    }
  } else { //khi nút được thả ra
    if (btn_Status_Led1 == PRESS) {  //kiểm tra nút đã được nhấn chưa
      if ( led1_Status == ON)  led1_Status = OFF; else led1_Status = ON; //nếu trạng thái thiết bị on thì off và ngược lại
    }
    btn_Status_Led1 = HAHA; //trả lại trạng thái chưa được nhấn của nút
    pressTime = millis();//reset thời gian nhấn nút
  }
  //đọc button 2
  if (digitalRead(btn_Pin_Led2) == HIGH) {
    timePressBtn2 = (unsigned long)(millis() - pressTime2);
    if (timePressBtn2 < 500 && timePressBtn2 > 50 ) {
      //nút được nhấn
      btn_Status_Led2 = PRESS;
    }
  } else {
    if (btn_Status_Led2 == PRESS) {
      if ( led2_Status == ON)  led2_Status = OFF; else led2_Status = ON;
    }
    btn_Status_Led2 = HAHA;
    pressTime2 = millis();
  }
  //đọc button 3
  if (digitalRead(btn_Pin_Bom) == HIGH) {
    timePressBtn3 = (unsigned long)(millis() - pressTime3);
    if (timePressBtn3 < 500 && timePressBtn3 > 50) {
      //nút được nhấn
      btn_Status_Bom = PRESS;
    }
  } else {
    if (btn_Status_Bom == PRESS) {
      if ( bom_Status == ON)  bom_Status = OFF; else bom_Status = ON;
    }
    btn_Status_Bom = HAHA;
    pressTime3 = millis();
  }
  //đọc button btn_Pin_Func
  if (digitalRead(btn_Pin_Func) == HIGH) {
    timePressBtn4 = (unsigned long)(millis() - pressTime4);
    if (timePressBtn4 < 500 && timePressBtn4 > 50) {
      //nút được nhấn
      btn_Status_Func = PRESS;
    }
  } else {
    if (btn_Status_Func == PRESS) {
      if ( relay4_Status == ON)  relay4_Status = OFF; else relay4_Status = ON;
    }
    btn_Status_Func = HAHA;
    pressTime4 = millis();
  }
}

void ReadTDSSensorLoop() {//đọc giá trị tds
  averageVoltage = analogRead(TdsSensorPin); //đọc tín hiệu điện của cảm biến tds
  averageVoltage = simpleKalmanFilter.updateEstimate(averageVoltage);//lọc nhiễu
  averageVoltage = averageVoltage * (float)VREF / 1024.0;//chuyển từ analog sang volt
  float compensationCoefficient = 1.0 + 0.02 * (temperature - 25.0); //tính sai số
  float compensationVolatge = averageVoltage / compensationCoefficient; //chuyển đổi sai số
  tdsValue = (133.42 * compensationVolatge * compensationVolatge * compensationVolatge - 255.86 * compensationVolatge * compensationVolatge + 857.39 * compensationVolatge) * 0.5;//tính giá trị tds
}
void ReadTempLoop() { //đọc nhiệt đọ
  ds18b20.requestTemperatures();//lấy giá trị nhiệt độ
  float temp = ds18b20.getTempCByIndex(0);//lấy giá trị ở cảm biến thứ 1
  if (!isnan(temp) && temp != -127) { //check giá trị
    temperature = temp;
  } else {
    temperature = 25;
  }
}
void UpdateScreen() {//cập nhật màn hình
  display.clearDisplay();
  display.setCursor(24, 0);
  display.print(F("AQUAPONIC 2020"));
  display.setCursor(0, 12);
  display.print(F("Nhiet do: ")); display.print(temperature);
  display.setCursor(0, 24);
  display.print(F("TDS     : ")); display.print(tdsValue);
  display.setCursor(0, 36);
  display.print(F("PH      : ---") );
  display.setCursor(0, 48);
  display.print(d);
  display.print(F(":"));
  display.print(h);
  display.print(F(":"));
  display.print(m);
  display.print(F(":"));
  display.print(s);
  display.display();
}

// function tự cài đặt
void CustomFunc() {
  digitalWrite(trigPin, LOW); tạo xung ở chân trig sonar
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);// bắt tần số
  distance = duration * 0.034 / 2;//tính khoảng cách
  distance = sonar.updateEstimate(distance);//lọc nhiễu
  if (distance > 20 && distance < 100) {
    if (takePhoto && (unsigned long)(millis() - current) > 3000) {
      digitalWrite(pin_Func, HIGH);
      if ((unsigned long)(millis() - current) > 3100) {
        digitalWrite(pin_Func, LOW);
        takePhoto = false;
      }
    }
  } else {
    current = millis();
    takePhoto = true;
  }
}
//tính thời gian
void CalTime() {
  d = millis() / 86400000; //ngày
  h = (millis() % 86400000) / 3600000;//giờ
  m = (millis() % 86400000) % 3600000;//phút
  s = millis() % 60000;//giây
}
unsigned long test = 0;
unsigned long loopPing = millis();
void loop() {
  CalTime();// tính thời gian hoạt động đổi ra phút gây
  //check ping của vòng loop
  //  Serial.print("Ping: "); Serial.println(millis() - loopPing);
  //  loopPing = millis();
  //end check
  if (millis() - test > 1000) {
    //    Serial.println(freeMemory());
    //    Serial.println(temperature);
    UpdateScreen();//update màn hình
    digitalWrite(13, !digitalRead(13));//blink trạng thái  led
    test = millis();
  }
  ControlDevice(); //check tín hiệu điều khiển
  ReadStatusButtonInLoop(); //đọc trạng thái nút nhấn
  ReadTDSSensorLoop();// đọc cảm biến tds
  ReadTempLoop(); //đọc nhiệt độ
  //    UpdateStatus();
}
