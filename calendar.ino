uint8_t month_arr[12][3] = {{9, 0, 13},{5, 4, 1},{12, 0, 17}, {0, 15, 17}, {12, 0, 24}, {9, 20, 13}, {9, 20, 11}, 
                    {0, 20, 6}, {18, 4, 15}, {14, 2, 19}, {13, 14, 21}, {3, 4, 2}};

uint32_t day_color[] = {0xff0000ff, 0xff00ccff, 0xffa14bfc, 0xff119311, 0xff1278e5,  0xffe58d34, 0xffb7095d};// อ จ อ พ พฤ ศ ส

//โหมดปฏิทิน
void display_calendar() {
  //อ่านค่าจากปุ่มกด
  if(!digitalRead(32)){
    calendar_mode = !calendar_mode;
    while (!digitalRead(32));
    EEPROM.write(2, calendar_mode);
    EEPROM.commit();
  }
  //เเสดงผล
  if(RTC.read(tm)){
    graphic.setBackground(calendar_mode? (uint32_t) 0x00000000 : day_color[tm.Wday+4]); //เปลี่ยนสีตามวันในสัปดาห์
    // Serial.println(tm.Wday);
    graphic.drawWithColor(number3x5_data[tm.Day/10],calendar_mode? day_color[tm.Wday+4] :0xffeaeaea ,5,3,2,1);
    graphic.drawWithColor(number3x5_data[tm.Day%10],calendar_mode? day_color[tm.Wday+4] :0xffeaeaea ,5,3,6,1);
    graphic.drawWithColor(alphabet3x5_data[month_arr[tm.Month - 1][0]],0xffeaeaea,5,3,2,8);
    graphic.drawWithColor(alphabet3x5_data[month_arr[tm.Month - 1][1]],0xffeaeaea,5,3,6,8);
    graphic.drawWithColor(alphabet3x5_data[month_arr[tm.Month - 1][2]],0xffeaeaea,5,3,10,8);
    delay(50);
    graphic.display();
  }
}
