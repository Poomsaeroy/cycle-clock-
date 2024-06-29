unsigned long patt;
unsigned long lastt;
unsigned long alarmt;
extern int buzzerPin;
bool Wblink = true;
uint32_t work_color[] = {0x00000000, 0xfff6f6fb, 0xff0000bf, 0xffbf5f00, 0xfff6f6fb, 0xff007fff};

// โหมดนาฬิกาดิจิตอล
void display_work()
{
    // ตรวจสอบว่าสามารถอ่านค่าจาก RTC DS1307 ได้หรือไม่
    if (RTC.read(tm))
    {
        // เเสดงผล
        graphic.setBackground(work_color[1]);
        graphic.drawWithColor(number3x5_data[tm.Hour / 10], work_color[0], 5, 3, 4, 1);
        graphic.drawWithColor(number3x5_data[tm.Hour % 10], work_color[0], 5, 3, 8, 1);

        if ((millis() - lastt) > 1000)
        {
            lastt = millis();
            Wblink = !Wblink;
        }
        graphic.draw(Wblink ? work_color[0] : work_color[3], 6, 7);
        graphic.draw(Wblink ? work_color[0] : work_color[3], 8, 7);
        graphic.drawWithColor(number3x5_data[tm.Minute / 10], work_color[0], 5, 3, 4, 9);
        graphic.drawWithColor(number3x5_data[tm.Minute % 10], work_color[0], 5, 3, 8, 9);
        delay(50);
        graphic.display();

        if (millis() - patt > 1500000) //1500000
        {
            patt = millis();

            graphic.draw(p_data[4], 0, 0);
            graphic.display();
            delay(200);

            graphic.draw(p_data[5], 0, 0);
            graphic.display();
            delay(200);

            graphic.draw(p_data[6], 0, 0);
            graphic.display();
            delay(200);

            graphic.draw(p_data[4], 0, 0);
            graphic.display();
            delay(200);
            graphic.draw(p_data[5], 0, 0);
            graphic.display();
            delay(200);

            graphic.draw(p_data[6], 0, 0);
            graphic.display();
            delay(200);

            graphic.draw(p_data[4], 0, 0);
            graphic.display();
            delay(200);

            graphic.draw(p_data[5], 0, 0);
            graphic.display();
            delay(200);
        }
    }

    if ((millis() - alarmt) > 3600000) //3600000
    { // จับทุก 1 ชั่วโมง (1 ชม = 3600000 มิลลิวินาที)
        alarmt = millis();

        // เพิ่มเวลา 1 ชั่วโมง
        tm.Hour++;
        if (tm.Hour >= 24)
        {
            tm.Hour = 0;
        }

        graphic.clear();
        graphic.draw(water_data[0], 0, 0);
        graphic.display();
        digitalWrite(buzzerPin, 1);
        delay(600);
        graphic.draw(water_data[1], 0, 0);
        graphic.display();
        digitalWrite(buzzerPin, 0);
        delay(600);
        graphic.draw(water_data[0], 0, 0);
        graphic.display();
        digitalWrite(buzzerPin, 1);
        delay(600);
        graphic.draw(water_data[1], 0, 0);
        graphic.display();
        digitalWrite(buzzerPin, 0);
        delay(600);
        graphic.draw(water_data[0], 0, 0);
        graphic.display();
        digitalWrite(buzzerPin, 1);
        delay(600);
        graphic.draw(water_data[1], 0, 0);
        graphic.display();
        digitalWrite(buzzerPin, 0);
        delay(600);
    }
}