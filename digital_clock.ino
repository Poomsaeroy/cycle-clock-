
unsigned long lasttime;
unsigned long pausetime;
bool blink = true;
uint32_t color[] = {0x00000000, 0xffeaeaea, 0xff0000bf, 0xffbf5f00, 0xff62b217, 0xff007fff};

// โหมดนาฬิกาดิจิตอล
void display_clock()
{
  // อ่านค่าปุ่มกดเพื่อเปลี่ยนสีพื้นหลังเเละตัวเลข
  read_sw_digital_clock();
  // ตรวจสอบว่าสามารถอ่านค่าจาก RTC DS1307 ได้หรือไม่
  if (RTC.read(tm))
  {
    // เเสดงผล
    graphic.setBackground(color[bg_clock_color]);
    graphic.drawWithColor(number3x5_data[tm.Hour / 10], color[digit_clock_color], 5, 3, 4, 1);
    graphic.drawWithColor(number3x5_data[tm.Hour % 10], color[digit_clock_color], 5, 3, 8, 1);

    if ((millis() - lasttime) > 1000)
    {
      lasttime = millis();
      blink = !blink;
    }
    graphic.draw(blink ? color[digit_clock_color] : color[bg_clock_color], 6, 7);
    graphic.draw(blink ? color[digit_clock_color] : color[bg_clock_color], 8, 7);
    graphic.drawWithColor(number3x5_data[tm.Minute / 10], color[digit_clock_color], 5, 3, 4, 9);
    graphic.drawWithColor(number3x5_data[tm.Minute % 10], color[digit_clock_color], 5, 3, 8, 9);
    delay(50);
    graphic.display();

  //   if (millis() - pausetime > 900000)
  // {
  //   pausetime = millis();

  //   graphic.draw(p_data[0], 0, 0);
  //   graphic.display();
  //   delay(500);

  //   graphic.draw(p_data[1], 0, 0);
  //   graphic.display();
  //   delay(500);

  //   graphic.draw(p_data[0], 0, 0);
  //   graphic.display();
  //   delay(500);

  //   graphic.draw(p_data[1], 0, 0);
  //   graphic.display();
  //   delay(500);
  //   graphic.draw(p_data[0], 0, 0);
  //   graphic.display();
  //   delay(500);

  //   graphic.draw(p_data[1], 0, 0);
  //   graphic.display();
  //   delay(500);

  //   graphic.draw(p_data[0], 0, 0);
  //   graphic.display();
  //   delay(500);

  //   graphic.draw(p_data[1], 0, 0);
  //   graphic.display();
  //   delay(500);


  // }
  }

  
}

// ฟังก์ชันอ่านค่าจากปุ่มกดเพื่อเปลี่ยนสีพื้นหลังเเละตัวเลข
void read_sw_digital_clock()
{
  // เปลี่ยนสีตัวเลข
  if (!digitalRead(32))
  {
    if ((millis() - last_time) > 100)
    {
      last_time = millis();
      while (!digitalRead(32))
        ;
      digit_clock_color < 6 ? digit_clock_color++ : digit_clock_color = 0;
      digit_clock_color != bg_clock_color ?: digit_clock_color < 6 ? digit_clock_color++
                                                                   : digit_clock_color = 0;
      uint8_t cur_data = EEPROM.read(1);
      cur_data &= 0xF0;
      EEPROM.write(1, digit_clock_color | cur_data);
      EEPROM.commit();
    }
  }
  // เปลี่ยนสีฉากหลัง
  if (!digitalRead(33))
  {
    if ((millis() - last_time) > 100)
    {
      last_time = millis();
      while (!digitalRead(33))
        ;
      bg_clock_color < 6 ? bg_clock_color++ : bg_clock_color = 0;
      digit_clock_color != bg_clock_color ?: bg_clock_color < 6 ? bg_clock_color++
                                                                : bg_clock_color = 0;
      uint8_t cur_data = EEPROM.read(1);
      cur_data &= 0x0F;
      EEPROM.write(1, (bg_clock_color << 4) | cur_data);
      EEPROM.commit();
    }
  }
}
