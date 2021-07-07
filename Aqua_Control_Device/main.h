#include <SimpleKalmanFilter.h> //thư viện lọc nhiễu 
SimpleKalmanFilter simpleKalmanFilter(2, 2, 0.01);
SimpleKalmanFilter sonar(2, 2, 0.01);
#define TdsSensorPin A1 //khai báo chân TDS sensor
#define VREF 5.0 //điện áp cấp cho tds;


#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
Adafruit_SSD1306 display(128, 64, &Wire, 12);


#include <OneWire.h>// thư viện đọc wire nhiệt độ
#include <DallasTemperature.h> // thư viện ds18b20
#define ONE_WIRE_BUS A0 //chân cảm biến
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature ds18b20(&oneWire);
float averageVoltage = 0, tdsValue = 0, temperature = 25, phValue = 7;

//sonar
#define echoPin 10
#define trigPin 11
long duration; //xung
float distance = 0.0; //khoảng cách

#define minKc 20  //Khoảng cách nước đầy so với cảm biến cm
#define maxKc 70  //Khoảng cách nước cạn so với cảm biến cm

//tính time
long d, h, m, s;
//TaskHandle_t taskNotificationHandler;//khai báo task noti/ bỏ vì lỗi

//SoftwareSerial myServer(10, 11); // RX, TX //xung đột với màn hình

#define btn_Pin_Led1 6 // định nghĩa chân điều khiển led 1 kích mức high
#define btn_Pin_Led2 7 // định nghĩa chân điều khiển led 2
#define btn_Pin_Bom 8 // định nghĩa chân điều khiển Bơm
#define btn_Pin_Func 12 //nút nhấn điều khiển chụp ảnh

#define pin_Led1 5
#define pin_Led2 4
#define pin_Bom 3
#define pin_Func 2

#define pin_Led_Status 13
//hàm chụp ảnh
int blinkCount = 0;
bool takePhoto = false;

// hàm đọc serial
String inputString = "";
bool stringComplete = false;
//

unsigned long timePressBtn1 = 0, timePressBtn2 = 0, timePressBtn3 = 0,timePressBtn4 = 0;
unsigned long pressTime = millis(), pressTime2 = millis(), pressTime3 = millis(),pressTime4 = millis(), current;
//khai báo trạng thái của nút nhấn tổng quát
enum BTN_STATUS {
  HAHA,
  PRESS,
  HOLD_3,
  HOLD_5
} ;
//khai báo trạng thái của thiết bị
enum DEVICE_STATUS {
  OFF,
  ON
};
//trạng thái device
DEVICE_STATUS led1_Status = OFF;
DEVICE_STATUS led2_Status = OFF;
DEVICE_STATUS bom_Status = OFF;
DEVICE_STATUS relay4_Status = OFF;
//lưu trạng thái cũ của device in event
DEVICE_STATUS dec1;
DEVICE_STATUS dec2;
DEVICE_STATUS dec3;

//khai báo trạng của từng nút nhấn
BTN_STATUS btn_Status_Led1 = HAHA;
BTN_STATUS btn_Status_Led2 = HAHA;
BTN_STATUS btn_Status_Bom = HAHA;
BTN_STATUS btn_Status_Func = HAHA;


void ReadStatusButtonInLoop();//hàm đọc trạng thái nút nhấn liên tục trong loop

void ControlDevice(); //hàm điều khiển trạng thái của thiết bị

void UpdateSTT(); //cập nhật trạng thái của thiết bị lên server
void CommandListen(); //lắng nghe sự kiện
void serialEvent(); // sự kiện rewrite của Serial

void ReadTempLoop(); //update nhiệt độ

void ReadTDSSensorLoop(); //update tds

void CalTime();//tính thời gian

void CustomFunc(); //kích hoạt chân chụp ảnh
