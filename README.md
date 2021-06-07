# Aquaponics2021
 Dự án aquaponic 2021 Lab-B304 ĐH Lạc Hồng
Aquaponics
Mô tả hệ thống:

# I.	Linh kiện phần cứng:
# II.	Fontend
# III.	Nhúng
## a.	Sơ đồ mạch
## b.	Command: 
-	Lệnh: AT=<mã lệnh>
-	Mã lệnh:
o	“all”: Lấy tất cả dữ liệu bao gồm trạng thái nút nhấn và  thông số cảm biến
o	 “ctr”: Lấy tất cả trạng thái của nút nhấn
o	“data”: Lấy tất cả thông số của cảm biến
o	“temp”: Lấy nhiệt độ
o	“tds”: Lấy giá trị TDS
o	“ph”: Lấy giá trị PH
## c.	Dữ liệu trả về
-	Lệnh “all”: 
o	{"ctr":[<btn1>,<btn2>,<btn3>,<btn4>],"data":[<tds>,<Nhiệt độ>,<PH>,<độ mở của chửa cho thức ăn>]} 
-	Lệnh “data”
o	{"data":[":[<tds>,<Nhiệt độ>,<PH>,<độ mở của chửa cho thức ăn>]} 
-	Lệnh “ctr”:
o	{"ctr":[<btn1>,<btn2>,<btn3>,<btn4>]}
-	Lệnh “temp”:
o	{"temp":<Nhiệt độ>} 
-	Lệnh “tds”:
o	{"tds":<nồng độ chất tan>} 
-	Lệnh “ph”:
o	{"ph":<ph>}

