# 🎯 TESA Offense - Drone Control & Dynamics (ระบบควบคุมและพลศาสตร์ของโดรน)

โฟลเดอร์นี้รวบรวมไฟล์สำหรับการจำลองและควบคุมการบินของโดรน (Quadrotor) ด้วยโปรแกรม **MATLAB** และ **Simulink** เพื่อใช้ในการคำนวณแบบจำลองทางฟิสิกส์และการควบคุม (PD Control) ในภารกิจฝั่งรุก (Offense)

## 📂 โครงสร้างและไฟล์สำคัญ (Key Files)
- `Quadrotor_Drone_PD_Control.m`: สคริปต์หลักภาษา MATLAB สำหรับรันระบบควบคุมแบบ PD Control ของโดรน
- ไฟลโมเดลจำลอง (`TESAOffense50Day2.slx`, `TESAOffense50Day3.slx`, `quadrotorsmodel2.slx`): ไฟล์ Simulink (.slx) สำหรับการจำลองพฤติกรรมแบบเสมือนจริง
- โฟลเดอร์โจทย์ (`Offense-Day1-Problem`, `Offense-Day2-Problem`): โจทย์ปัญหา ไฟล์ภารกิจ และสถานการณ์จำลองแต่ละวัน
- ไฟล์ฟังก์ชันและข้อมูล Workspace (`MATLAB Function...txt`, `TESA_Workspace...txt`): โค้ดสูตรคณิตศาสตร์และค่าตัวแปรเริ่มต้นที่อาจต้องใช้โหลดเข้าโปรแกรม
- ไฟล์จดบันทึก (`โจทย์ สถานการณ์ 3.txt`, `โน้ต rasberry PI.txt`): ข้อมูลเพิ่มเติมที่เกี่ยวข้องกับการนำไปปรับใช้จริงบนบอร์ด Raspberry Pi

## 🛠️ ข้อกำหนดเบื้องต้น (Prerequisites)
- **โปรแกรมหลัก:** ต้องติดตั้งโปรแกรม **MATLAB** พร้อมส่วนเสริม **Simulink**
- **Toolboxes (หากจำเป็น):** ส่วนเสริมเช่น Control System Toolbox, Simulink 3D Animation เป็นต้น

## 🚀 วิธีการใช้งาน (Usage)
1. เปิดโปรแกรม **MATLAB** และตั้งค่า (Set Path) โฟลเดอร์ปัจจุบันให้มาอยู่ที่โฟลเดอร์ `Tesa_Offense`
2. **รันสคริปต์ตั้งค่า:** เปิดและรันไฟล์ `Quadrotor_Drone_PD_Control.m` เพื่อคำนวณหรือโหลดค่าพารามิเตอร์เริ่มต้นต่างๆ เข้าสู่ Workspace ของ MATLAB
3. **เปิดโมเดลจำลอง:** ดับเบิลคลิกเปิดไฟล์ `.slx` ตัวที่ต้องการ เช่น `quadrotorsmodel2.slx` เพื่อแสดงแผนภาพ Block ของระบบใน Simulink
4. กดปุ่ม **Run** ในหน้าต่าง Simulink เพื่อเริ่มการจำลองผลการบิน (Simulation) ของโดรน
