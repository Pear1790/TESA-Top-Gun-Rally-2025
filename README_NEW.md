# TESA Military Drone Command Center 🚁

คู่มือการใช้งานระบบเชื่อมต่อระหว่าง **MATLAB (Offense)**, **Python YOLOv8 (Defense)** และ **React (Dashboard)** ผ่าน Backend กลางแบบเรียลไทม์

---

## 🏗️ สถาปัตยกรรมระบบ (Architecture)

ระบบประกอบด้วย 4 ส่วนสำคัญที่ทำงานประสานกันผ่าน Node.js Server:

1. **Tesa_Backend**: ตึกฐานทัพ (Command Center) คอยรับข้อมูล HTTP POST และกระจายต่อไปยังหน้าเว็บผ่าน WebSocket (ทำงานบนพอร์ต `3000`)
2. **Tesa_Dashboard**: เว็บไซต์แสดงผลแบบเรียลไทม์ที่สร้างด้วย React+Vite รับส่งข้อมูลผ่าน WebSocket (ทำงานบนพอร์ต `5173`)
3. **Tesa_Offense (MATLAB)**: ส่งข้อมูล Altitude (ระนาบ X,Y,Z) วิ่งเข้าสู่พอร์ต `3000/api/offense_telemetry`
4. **Tesa_Defense (Python)**: ส่งข้อมูลพิกัด GPS ข้าศึกที่ตรวจจับได้จาก YOLOv8 วิ่งเข้าสู่พอร์ต `3000/api/defense_telemetry`

---

## 🚀 ขั้นตอนการเปิดใช้งานระบบ (Step-by-Step)

เพื่อให้ระบบทำงานประสานกันได้อย่างสมบูรณ์ **ต้องทำการเปิดใช้งานตามลำดับ** ดังนี้

### ขั้นที่ 1: เปิดการทำงานของ Backend Server
เปิด Terminal ตัวที่ 1 รันคำสั่งต่อไปนี้:
```bash
cd Tesa_Backend
npm install    # (หากรันครั้งแรก)
node server.js
```
> 🎉 **สำเร็จ:** จะต้องขึ้นข้อความ `🚀 TESA Backend Command Center running on port 3000`

### ขั้นที่ 2: เปิดการทำงานของ Dashboard
เปิด Terminal ตัวที่ 2 รันคำสั่ง:
```bash
cd Tesa_Dashboard
npm install    # (หากรันครั้งแรก)
npm run dev
```
> 🎉 **สำเร็จ:** เข้าเบราว์เซอร์ไปที่ `http://localhost:5173` จะพบกับอินเทอร์เฟซ Dashboard สไตล์เรียบหรูทางการทหาร

### ขั้นที่ 3: เริ่มระบบตรวจจับข้าศึก (Sentinel Watch / Defense)
เปิด Terminal ตัวที่ 3 รันระบบ Python:
```bash
cd Tesa_Defense
# อย่าลืมเปลี่ยนชื่อไฟล์ Video ใน 4_gps_testing.py ให้ถูกต้อง
python 4_gps_testing.py
```
> **สิ่งที่เกิดขึ้น:** หาก Python ตรวจพบโดรน มันจะโชว์วิดีโอที่มี Bounding Box พร้อมกับยิง Latitude & Longitude เข้า Backend ทันที หากกลับมาดูหน้า Dashboard ในโหมดอักษรสีแดง ข้อมูลจะเด้งในตารางเตือนภัยโดยอัตโนมัติ

### ขั้นที่ 4: เริ่มการบิน (Operation Strike / Offense)
เปิด **MATLAB** จากนั้นเข้าไปที่โฟลเดอร์รัน:
- เปิด `Quadrotor_Drone_PD_Control.m` แล้วกด **Run**
> **สิ่งที่เกิดขึ้น:** สคริปต์นี้ถูกดัดแปลงให้มีคำสั่ง `webwrite` ในบรรทัดล่างสุด เมื่อโปรแกรมค่อยๆ คำนวณ Trajectory (เส้นทางการบิน) ค่าทุกอย่าง (X,Y,Z, Yaw, Pitch) จะเด้งมาโชว์ที่ Dashboard ในโหมดอักษรสีเขียวแบบเรียลไทม์

---

## 🖼️ ตัวอย่างการทำงาน

*(รูปตัวอย่าง สามารถวาง Screen capture ที่ได้จากการ Capture หน้าจอใส่ลงในโฟลเดอร์นี้ได้เลยจ้า)*

- **หน้าของ MATLAB (Offense) ** โหมด `Operation Strike` ขณะ MATLAB กำลังส่งค่าเข้ามา หน้าปัด Altitude กับแกนความเร็วต่างๆ จะแปรผันตามทันที
- **หน้าของ Python (Defense) ** โหมด `Sentinel Watch` จะมีคำเตือน "INTRUDER DETECTED" กะพริบขึ้นถ้า Python โยนข้อมูล JSON ของโดรนที่ตรวจจับได้เข้ามาในท่อ

---