unsigned long slepBlinkTime_m;
unsigned long slepBlinkTime_h;
unsigned long lat;
unsigned long previousMillis = 0;
bool hourBlink = true;
bool minuteBlink = true;
bool Onsensor = false;
extern int buzzerPin;
extern int pirSensorPin;
extern int soundSensorPin;
const int threshold = 3072;
bool soundDetected = false;
int startsensor = 0; // 1800000;
int on_sensor = 0;
int count_sound = 0;
int movingCount = 0;
int prevInput = LOW;
const long interval = 2000;
bool ack_time = false;

boolean printedMessage = false;

uint32_t Scolor[] = {0x00000000, 0xffeaeaea, 0xff0000bf, 0xffbf5f00, 0xff62b217, 0xff007fff};

bool b32_state = false;
bool time_set = false;
bool act_sleep = false;

int time_save_hour = 0;
int time_save_minute = 0;
int alarmsleep_hour = 0;
int alarmsleep_minute = 0;
int sleep_hour;
int sleep_minute;

int count_Line = 0;

bool sendline = false;
bool alarm_on = false;

void calculate_sleep_time()
{
    // กำหนดเวลาปลุกก่อนเวลาตั้งปลุก 8 ชั่วโมง
    sleep_hour = alarmsleep_hour - 8;
    sleep_minute = alarmsleep_minute;

    // หากรายการเวลานอนต่ำกว่า 0 (ตั้งเวลาปลุกก่อนเวลา 8 ชั่วโมง)
    if (sleep_hour < 0)
    {
        // ปรับค่าชั่วโมงให้เป็นค่าที่ถูกต้อง
        sleep_hour += 24;
    }
    if (ack_time)
    {
        if ((alarmsleep_hour - tm.Hour) > 0 && (alarmsleep_hour - tm.Hour) < 8)
        {
            //Serial.println("dddddd");
            LINE.notify("นอนไม่ครบ 8 ชั่วโมงอาจจะทำให้ง่วงนอนและอ่อนเพลีย");
        }
        Serial.println("123");
        LINE.notify("เราจะทำการแจ้งเตือนเมื่อถึงเวลานอน <3 และจะตรวจจับการนอนของคุณเมื่อเวลาผ่านไป 1 ชั่วโมง");
        LINE.notifySticker(6325, 10979907);
        ack_time = false;
    }
}

// โหมดนาฬิกาดิจิตอล
void display_sleeptime()
{

    if (!time_set)
    {

        if (RTC.read(tm))
        {
            readpin32();
            if (b32_state)
            {
                if (minuteBlink)
                {
                    graphic.setBackground(Scolor[0]);
                    graphic.drawWithColor(number3x5_data[time_save_minute / 10], Scolor[5], 5, 3, 4, 9);
                    graphic.drawWithColor(number3x5_data[time_save_minute % 10], Scolor[5], 5, 3, 8, 9);
                }
                // Display minute digits without blinking
                graphic.drawWithColor(number3x5_data[time_save_hour / 10], Scolor[5], 5, 3, 4, 1);
                graphic.drawWithColor(number3x5_data[time_save_hour % 10], Scolor[5], 5, 3, 8, 1);

                if (digitalRead(33) == LOW) // key 33 3 +
                {
                    if ((millis() - lat) > 100)
                    {
                        lat = millis();
                        while (!digitalRead(33))
                            ;
                        time_save_minute++;
                        if (time_save_minute > 59)
                        {
                            time_save_minute = 0;
                        }
                    }
                }
                if (digitalRead(34) == LOW)
                {
                    if ((millis() - lat) > 100)
                    {
                        lat = millis();
                        while (!digitalRead(34))
                            ;
                        time_save_minute--;
                        if (time_save_minute < 0)
                        {
                            time_save_minute = 59;
                        }
                    }
                }
            }

            else
            {
                if (hourBlink)
                {
                    graphic.setBackground(Scolor[0]);
                    graphic.drawWithColor(number3x5_data[time_save_hour / 10], Scolor[5], 5, 3, 4, 1);
                    graphic.drawWithColor(number3x5_data[time_save_hour % 10], Scolor[5], 5, 3, 8, 1);
                }
                // Display minute digits without blinking
                graphic.drawWithColor(number3x5_data[time_save_minute / 10], Scolor[5], 5, 3, 4, 9);
                graphic.drawWithColor(number3x5_data[time_save_minute % 10], Scolor[5], 5, 3, 8, 9);

                if (digitalRead(33) == LOW) // key 33 3
                {
                    if ((millis() - lat) > 100)
                    {
                        lat = millis();
                        while (!digitalRead(33))
                            ;
                        time_save_hour++;
                        if (time_save_hour > 23)
                        {
                            time_save_hour = 0;
                        }
                    }
                }
                if (digitalRead(34) == LOW)
                {
                    if ((millis() - lat) > 100)
                    {
                        lat = millis();
                        while (!digitalRead(34))
                            ;
                        time_save_hour--;
                        if (time_save_hour < 0)
                        {
                            time_save_hour = 23;
                        }
                    }
                }
            }
            // กระพริบเครื่องหมายโคลนทุกๆ วินาที
            if ((millis() - slepBlinkTime_m) > 1000)
            {
                slepBlinkTime_m = millis();
                minuteBlink = !minuteBlink;
            }
            if ((millis() - slepBlinkTime_h) > 1000)
            {
                slepBlinkTime_h = millis();
                hourBlink = !hourBlink;
            }

            // แสดงเครื่องหมาย .. (กระพริบ)
            graphic.draw(hourBlink ? Scolor[2] : Scolor[2], 6, 7);
            graphic.draw(hourBlink ? Scolor[2] : Scolor[2], 8, 7);

            // อัพเดทการแสดงผล
            delay(50); // ความล่าช้าเพื่อความเสถียร
            graphic.display();

            act_sleep = false;
            read_sw_timeset();


            if (sendline)
            {
                if (count_Line == 0)
                {
                    graphic.draw(p_data[2], 0, 0);
                    graphic.display();
                    motioncheck();
                    soundcheck();
                    movingCount = 0;
                    count_sound = 0;
                    count_Line++;
                    digitalWrite(19, LOW);
                }
            }
        }
    }
    if (time_set)
    {
        if (alarm_on)
        {
            digitalWrite(18, LOW);
            graphic.clear();
            digitalWrite(buzzerPin, HIGH);
            graphic.draw(bed_data[4], 0, 0);
            graphic.display();
            delay(250);

            digitalWrite(buzzerPin, LOW);
            graphic.draw(bed_data[5], 0, 0);
            graphic.display();
            delay(250);

            digitalWrite(buzzerPin, HIGH);
            graphic.draw(bed_data[4], 0, 0);
            graphic.display();
            delay(250);

            digitalWrite(buzzerPin, LOW);
            graphic.draw(bed_data[5], 0, 0);
            graphic.display();
            delay(250);
            Onsensor = false;

            if (digitalRead(12) == LOW)
            {
                if ((millis() - lat) > 100)
                {
                    lat = millis();
                    digitalWrite(19, HIGH);
                    while (!digitalRead(12))
                        ;
                    alarm_on = false;
                    sendline = true;
                    startsensor = millis();
                    time_set = false;
                }
            }
        }
        display_clock();

        calculate_sleep_time();

        checkedit();

        if (sleep_hour == tm.Hour && sleep_minute == tm.Minute && tm.Second < 10) // tell time to sleep
        {
            Onsensor = true;
            graphic.setBackground(Scolor[0]);
            digitalWrite(buzzerPin, HIGH);
            graphic.clear();
            graphic.draw(bed_data[7], 0, 0);
            graphic.display();
            delay(500);
            digitalWrite(buzzerPin, LOW);
            graphic.draw(bed_data[8], 0, 0);
            graphic.display();
            delay(500);
            digitalWrite(buzzerPin, HIGH);
            graphic.draw(bed_data[7], 0, 0);
            graphic.display();
            delay(500);
            graphic.draw(bed_data[8], 0, 0);
            graphic.display();
            digitalWrite(buzzerPin, LOW);
            delay(500);
        }
        if (Onsensor)
        {
            if (millis() - startsensor > 3600000) // 3600000
            {
                // Serial.println("start");
                digitalWrite(18, HIGH);
                unsigned long currentMillis = millis();
                // Check if it's time to read PIR sensor
                if (currentMillis - previousMillis >= interval)
                {
                    previousMillis = currentMillis;
                    // Read the PIR sensor input
                    int pirInputState = digitalRead(pirSensorPin);
                    // If motion is detected by the PIR sensor
                    if (pirInputState == HIGH)
                    {
                        // Increment motion count
                        movingCount++;
                        Serial.print("Motion detected! Moving count: ");
                        Serial.println(movingCount);
                    }
                }
                // Read analog value from the sound sensor
                int sensorValue = analogRead(soundSensorPin);
                // If sound level exceeds threshold, increment count and send message
                if (sensorValue < threshold)
                {
                    soundDetected = true;
                    if (count_sound == 0)
                    {
                        count_sound = 1;
                    }
                    else
                    {
                        count_sound++;
                        Serial.println("Sound Count: " + String(count_sound)); // Print count to serial monitor
                    }
                }
                else
                {
                    soundDetected = false;
                }
                delay(100); // Add a small delay for stability
            }
        }

        if (alarmsleep_hour == tm.Hour && alarmsleep_minute == tm.Minute)
        {
            alarm_on = true;
        }
    }
}

void checkedit()
{
    if (digitalRead(27) == LOW)
    {
        if ((millis() - lat) > 100) // edit time
        {
            lat = millis();
            while (!digitalRead(27))
                ;
            graphic.clear();
            graphic.draw(bed_data[2], 0, 0);
            graphic.display();
            delay(500);
            graphic.draw(bed_data[3], 0, 0);
            graphic.display();
            delay(500);
            graphic.draw(bed_data[2], 0, 0);
            graphic.display();
            delay(500);
            graphic.draw(bed_data[3], 0, 0);
            graphic.display();
            delay(500);
            Onsensor = false;
            time_set = false;
            digitalWrite(18, LOW);
            display_sleeptime();
        }
    }
}

void readpin32()
{
    if (digitalRead(32) == LOW)
    {
        if ((millis() - lat) > 100)
        {
            lat = millis();
            while (!digitalRead(32))
                ;
            b32_state = !b32_state;
        }
    }
}

void read_sw_timeset()
{
    if (digitalRead(2) == HIGH)
    {
        if ((millis() - lat) > 100)
        {
            lat = millis();
            while (!digitalRead(2))
                ;
            alarmsleep_hour = time_save_hour;
            alarmsleep_minute = time_save_minute;
            
            graphic.clear();
            graphic.draw(bed_data[0], 0, 0);
            graphic.display();
            delay(500);
            graphic.draw(bed_data[1], 0, 0);
            graphic.display();
            delay(500);
            graphic.draw(bed_data[0], 0, 0);
            graphic.display();
            delay(500);
            graphic.draw(bed_data[1], 0, 0);
            graphic.display();
            delay(500);
            digitalWrite(19, LOW);
            count_sound = 0;
            movingCount = 0;
            count_Line = 0;
            ack_time = true;
            if (alarmsleep_hour - tm.Hour < 8)
            {
                Onsensor = true;
            }
            sendline = false;
            time_set = true;
            startsensor = millis();
        }
    }
}

void motioncheck()
{
    if (movingCount >= 0 && movingCount <= 10)
    {
        LINE.notify("ในคืนที่ผ่านมาสามารถตรวจจับการเคลื่อนไหวได้ " + String(movingCount) + " ครั้ง ซึ่งอยู่ในเกณฑ์ปกติ");
        LINE.notify("HAVE A GOOD DAY :) ");
    }
    else if (movingCount >= 11 && movingCount <= 25)
    {
        LINE.notify("ในคืนที่ผ่านมาสามารถตรวจจับการเคลื่อนไหวได้ " + String(movingCount) + " ครั้ง ซึ่งอยู่ในเกณฑ์ค่อนข้างบ่อย");
        LINE.notify("* การเคลื่อนไหวที่ตรวจจับได้อาจเกิดจากหลายปัจจัย เช่น การเปลี่ยนท่านอน, การหมุนของพัดลม, การเคลื่อนไหวของสัตว์เลี้ยงภายในบ้าน, ภาวะสุขภาพ, อาการไม่สบายใจหรือภาวะความเครียด หรือโรคพาร์กินสัน *");
        LINE.notify("หากไม่มีปัจจัยข้างต้น การเคลื่อนไหวอาจเกิดจากการดิ้นหรือนอนกระตุกของผู้ใช้งาน");
        LINE.notifyPicture("https://scontent.fbkk5-3.fna.fbcdn.net/v/t1.15752-9/430869739_374899145434245_2191517697991245436_n.png?_nc_cat=105&ccb=1-7&_nc_sid=5f2048&_nc_eui2=AeH8oKsCs8R2LKTlCVPjyuCpBd7o0JQ5134F3ujQlDnXfvH-sSHeSiNuXvgZkoKegnItZ6EEnBMh0nYkt56Mi-Xi&_nc_ohc=XsDTaMwrYlcAX8RRFH6&_nc_ht=scontent.fbkk5-3.fna&oh=03_AdQHawNuV2tnb2g0xCRCPHe4p90gCYvjdeisT-s9Mrb_Ww&oe=661539C1");
    }
    else if (movingCount >= 26 && movingCount <= 40)
    {
        LINE.notify("ในคืนที่ผ่านมาสามารถตรวจจับการเคลื่อนไหวได้ " + String(movingCount) + " ครั้ง ซึ่งอยู่ในเกณฑ์บ่อย");
        LINE.notify("* การเคลื่อนไหวที่ตรวจจับได้อาจเกิดจากหลายปัจจัย เช่น การเปลี่ยนท่านอน, การหมุนของพัดลม, การเคลื่อนไหวของสัตว์เลี้ยงภายในบ้าน, ภาวะสุขภาพ, อาการไม่สบายใจหรือภาวะความเครียด หรือโรคพาร์กินสัน *");
        LINE.notify("หากไม่มีปัจจัยข้างต้น การเคลื่อนไหวอาจเกิดจากการดิ้นหรือนอนกระตุกของผู้ใช้งาน");
        LINE.notifyPicture("https://scontent.fbkk5-3.fna.fbcdn.net/v/t1.15752-9/430869739_374899145434245_2191517697991245436_n.png?_nc_cat=105&ccb=1-7&_nc_sid=5f2048&_nc_eui2=AeH8oKsCs8R2LKTlCVPjyuCpBd7o0JQ5134F3ujQlDnXfvH-sSHeSiNuXvgZkoKegnItZ6EEnBMh0nYkt56Mi-Xi&_nc_ohc=XsDTaMwrYlcAX8RRFH6&_nc_ht=scontent.fbkk5-3.fna&oh=03_AdQHawNuV2tnb2g0xCRCPHe4p90gCYvjdeisT-s9Mrb_Ww&oe=661539C1");
    }
    else if (movingCount > 40)
    {
        LINE.notify("ในคืนที่ผ่านมาสามารถตรวจจับการเคลื่อนไหวได้ " + String(movingCount) + " ครั้ง ซึ่งอยู่ในเกณฑ์บ่อย");
        LINE.notify("* การเคลื่อนไหวที่ตรวจจับได้อาจเกิดจากหลายปัจจัย เช่น การเปลี่ยนท่านอน, การหมุนของพัดลม, การเคลื่อนไหวของสัตว์เลี้ยงภายในบ้าน, ภาวะสุขภาพ, อาการไม่สบายใจหรือภาวะความเครียด หรือโรคพาร์กินสัน *");
        LINE.notify("หากไม่มีปัจจัยข้างต้น การเคลื่อนไหวอาจเกิดจากการดิ้นหรือนอนกระตุกของผู้ใช้งาน");
        LINE.notifyPicture("https://scontent.fbkk5-6.fna.fbcdn.net/v/t1.15752-9/430904806_2726132534360838_8104318621251663684_n.png?_nc_cat=102&ccb=1-7&_nc_sid=5f2048&_nc_eui2=AeG33rg5e1HefyFNCfDofU3y3gqNNHclWCreCo00dyVYKvHagWeTF_GLfsA7NimzfTMdvrWBaHEdAvur6EMY-H2C&_nc_ohc=6K_CNML-qM0AX_mH0Zc&_nc_ht=scontent.fbkk5-6.fna&oh=03_AdQw229bO4M8crk6IP9Zemlxxnsa4QsxR5Kwf99uHUL97A&oe=66152D32");
    }
}

void soundcheck()
{
    if (count_sound >= 0 && count_sound <= 10)
    {
        LINE.notify("ในคืนที่ผ่านมาสามารถตรวจจับเสียงได้ " + String(count_sound) + " ครั้ง ซึ่งอยู่ในเกณฑ์ปกติ");
        LINE.notify("ขอให้วันนี้เป็นวันที่ดีครับ");
    }
    else if (count_sound >= 11 && count_sound <= 25)
    {
        LINE.notify("ในคืนที่ผ่านมาสามารถตรวจจับเสียงได้ " + String(count_sound) + " ครั้ง ซึ่งอยู่ในเกณฑ์การเกิดเสียงค่อนข้างบ่อย");
        LINE.notify("* เสียงที่ตรวจจับได้อาจเกิดจากปัจจัยภายนอก เช่น เสียงรถเดินทางผ่านหน้าบ้าน, เสียงเครื่องปรับอากาศหรือพัดลม, เสียงของสัตว์เลี้ยงภายในบ้าน หรือเสียงจากการทำกิจกรรมของบุคคลอื่น *");
        LINE.notify(" หากไม่มีปัจจัยข้างต้น เสียงที่เกิดขึ้นอาจเกิดจากการนอนกรนของผู้ใช้งาน");
        LINE.notifyPicture("https://scontent.fbkk5-6.fna.fbcdn.net/v/t1.15752-9/431030564_1110628016917110_2054650376944761815_n.png?_nc_cat=101&ccb=1-7&_nc_sid=5f2048&_nc_eui2=AeH4fHEIdeZ59VOQhj82vqXRZ_ABdYDipW5n8AF1gOKlbmb1etLPG-dpzekKbbOheQdglG_c1DT_Tle70OJTUQE0&_nc_ohc=o1R2OnbBIP8AX9fo5IR&_nc_ht=scontent.fbkk5-6.fna&oh=03_AdSwefuuGIK4O4XM12nxLh-oEHHFNDw-sM3B5lBo2kmsqQ&oe=661516D9");
    }
    else if (count_sound >= 26 && count_sound <= 40)
    {
        LINE.notify("ในคืนที่ผ่านมาสามารถตรวจจับเสียงได้ " + String(count_sound) + " ครั้ง ซึ่งอยู่ในเกณฑ์การเกิดเสียงบ่อย");
        LINE.notify("* เสียงที่ตรวจจับได้อาจเกิดจากปัจจัยภายนอก เช่น เสียงรถเดินทางผ่านหน้าบ้าน, เสียงเครื่องปรับอากาศหรือพัดลม, เสียงของสัตว์เลี้ยงภายในบ้าน หรือเสียงจากการทำกิจกรรมของบุคคลอื่น *");
        LINE.notify("หากไม่มีปัจจัยข้างต้น เสียงที่เกิดขึ้นอาจเกิดจากการนอนกรนของผู้ใช้งาน");
        LINE.notifyPicture("https://scontent.fbkk5-6.fna.fbcdn.net/v/t1.15752-9/431030564_1110628016917110_2054650376944761815_n.png?_nc_cat=101&ccb=1-7&_nc_sid=5f2048&_nc_eui2=AeH4fHEIdeZ59VOQhj82vqXRZ_ABdYDipW5n8AF1gOKlbmb1etLPG-dpzekKbbOheQdglG_c1DT_Tle70OJTUQE0&_nc_ohc=o1R2OnbBIP8AX9fo5IR&_nc_ht=scontent.fbkk5-6.fna&oh=03_AdSwefuuGIK4O4XM12nxLh-oEHHFNDw-sM3B5lBo2kmsqQ&oe=661516D9");
    }
    else if (count_sound > 40)
    {
        LINE.notify("ในคืนที่ผ่านมาสามารถตรวจจับเสียงได้ " + String(count_sound) + " ครั้ง ซึ่งอยู่ในเกณฑ์การเกิดเสียงบ่อยมาก");
        LINE.notify("* เสียงที่ตรวจจับได้อาจเกิดจากปัจจัยภายนอก เช่น เสียงรถเดินทางผ่านหน้าบ้าน, เสียงเครื่องปรับอากาศหรือพัดลม, เสียงของสัตว์เลี้ยงภายในบ้าน หรือเสียงจากการทำกิจกรรมของบุคคลอื่น *");
        LINE.notify("หากไม่มีปัจจัยข้างต้น เสียงที่เกิดขึ้นอาจเกิดจากการนอนกรนของผู้ใช้งาน");
        LINE.notifyPicture("https://scontent.fbkk10-1.fna.fbcdn.net/v/t1.15752-9/431749121_1563409497773916_3095282645925287271_n.png?_nc_cat=103&ccb=1-7&_nc_sid=5f2048&_nc_eui2=AeFOeBWni3TY9WVmgI79RPdH_xhlep5CIdb_GGV6nkIh1lfBxPn4P6lFL_iuY-nIvpFvoZi2w_3aiohd_wiyvBXL&_nc_ohc=v5wMB2MJqB0AX-8YbLe&_nc_ht=scontent.fbkk10-1.fna&oh=03_AdSeGzRtAoBLRTAHcRNVs-72U_05z7fVCPVJPRdW3YP_8Q&oe=661510A5");
    }
}