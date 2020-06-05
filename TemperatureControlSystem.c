////////////////LCD_CONF///////////////
sbit LCD_RS at RB0_bit;
sbit LCD_EN at RB1_bit;
sbit LCD_D7 at RB5_bit;
sbit LCD_D6 at RB4_bit;
sbit LCD_D5 at RB3_bit;
sbit LCD_D4 at RB2_bit;
sbit LCD_RS_Direction at TRISB0_bit;
sbit LCD_EN_Direction at TRISB1_bit;
sbit LCD_D7_Direction at TRISB5_bit;
sbit LCD_D6_Direction at TRISB4_bit;
sbit LCD_D5_Direction at TRISB3_bit;
sbit LCD_D4_Direction at TRISB2_bit;
//////////////keypad_CONF ////////////////
sbit f_row at RB6_bit;
sbit s_row at RB7_bit;
sbit t_row at RD0_bit;
sbit fo_row at RD1_bit;
sbit f_col at RD2_bit;
sbit s_col at RD3_bit;
sbit t_col at RD4_bit;
sbit fo_col at RD5_bit;
sbit f_row_Direction at TRISB6_bit;
sbit s_row_Direction at TRISB7_bit;
sbit t_row_Direction at TRISD0_bit;
sbit fo_row_Direction at TRISD1_bit;
sbit f_col_Direction at TRISD2_bit;
sbit s_col_Direction at TRISD3_bit;
sbit t_col_Direction at TRISD4_bit;
sbit fo_col_Direction at TRISD5_bit;
///////////////VARS/////////////////////////
int key=0,tem=27,hum=60,x=17,val=0,i;
char txt[6];
char lcd_r1[]="Temp= 00/00  c";
char lcd_r2[]="Humi= 00/00  %";
unsigned char Check, T_byte1, T_byte2,RH_byte1, RH_byte2, Ch ;
int Temp, RH, Sum ;
///////////////////Prototypes//////////////////////
int keypad_init();
void StartSignal();
void CheckResponse();
char ReadData();
int data_store();
void lcd();
void comp();
///////////////////////////////////////////////////////////////////////
void main() {
      trisc=0;
      for(i=0;i<5;i++){
        portc.f0=1;
        delay_ms(100);
        portc.f0=0;
        delay_ms(100);}
      portc.f0=1;
      Lcd_Init();
      lcd_cmd(_lcd_cursor_off);
      Lcd_Out(1,4,"WELLCOME");
      Lcd_Out(2,1,"Embedded System");
      delay_ms(3000);
      Lcd_Cmd(_LCD_CLEAR);
      Lcd_Out(1,1,"DR/MOHAMED MOWOD");
      Lcd_Out(2,1,"(: <3 <3");
      delay_ms(3000);
      Lcd_Cmd(_LCD_CLEAR);
  while (1){
        key=keypad_init();
        switch(key){
              case 10:
              Lcd_Cmd(_LCD_CLEAR);
              lcd_out(1,1,"Enter");
              lcd_out(2,1,"Temperature");
              delay_ms(2000);
              tem = data_store();
              break;
              case 11:
              Lcd_Cmd(_LCD_CLEAR);
              lcd_out(1,1,"Enter ");
              lcd_out(2,1,"humidity");
              delay_ms(2000);
              hum = data_store();
              break;
              default:
              break;}
        StartSignal();
        CheckResponse();
        if(Check == 1){
              RH_byte1 = ReadData();
              RH_byte2 = ReadData();
              T_byte1 = ReadData();
              T_byte2 = ReadData();
              Sum = ReadData();
              lcd_cmd(_LCD_CLEAR);
              if(Sum == ((RH_byte1+RH_byte2+T_byte1+T_byte2) & 0XFF)){
              Temp = T_byte1;
              RH = RH_byte1;
              lcd();
              lcd_out(1,1,lcd_r1);
              lcd_out(2,1,lcd_r2);
              delay_ms(200);
              }
              else{
              Lcd_Cmd(_LCD_CLEAR);
              Lcd_Out(1, 1, "Check sum error");}
              }
             comp();
}}


///////////////////////////////////////
int keypad_init(){
    x =17;
    f_col_Direction=1;
    f_row_Direction=0;
    s_col_Direction=1;
    s_row_Direction=0;
    t_col_Direction=1;
    t_row_Direction=0;
    fo_col_Direction=1;
    fo_row_Direction=0;
////////////////////////////
  f_row=1;
  s_row=0;
  t_row=0;
  fo_row=0;
  if(f_col){
  while(f_col){}
      x=1;
  }
  else if(s_col){
  while(s_col){}
     x=2;
  }
  else if(t_col){
  while(t_col){}
     x=3;
  }
  else if(fo_col){
  while(fo_col){}
     x=10;
  }
  ////////////////////////////////
  f_row=0;
  s_row=1;
  t_row=0;
  fo_row=0;
  if(f_col){
  while(f_col){}
     x=4;
  }
  else if(s_col){
  while(s_col){}
     x=5;
  }
  else if(t_col){
  while(t_col){}
     x=6;
  }
  else if(fo_col){
  while(fo_col){}
     x=11;
  }
    //////////////////////////////////////
  f_row=0;
  s_row=0;
  t_row=1;
  fo_row=0;
  if(f_col){
  while(f_col){}
     x=7;
  }
  else if(s_col){
  while(s_col){}
     x=8;
  }
  else if(t_col){
  while(t_col){}
     x=9;
  }
  else if(fo_col){
  while(fo_col){}
     x=12;
  }
  ////////////////////////////////
  f_row=0;
  s_row=0;
  t_row=0;
  fo_row=1;
  if(f_col){
  while(f_col){}
     x=15;
  }
  else if(s_col){
  while(s_col){}
     x=0;
  }
  else if(t_col){
  while(t_col){}
     x=14;
  }
  else if(fo_col){
  while(fo_col){}
     x=13;
  }
  return x;
  }
///////////////////////////////////////
void StartSignal(){
  TRISD.B6 = 0; //Configure RD0 as output
  PORTD.B6 = 0; //RD0 sends 0 to the sensor
  delay_ms(18);
  PORTD.B6 = 1; //RD0 sends 1 to the sensor
  delay_us(30);
  TRISD.B6 = 1; //Configure RD0 as input
}
///////////////////////////////////////
void CheckResponse(){
  Check = 0;
  delay_us(40);
  if (PORTD.B6 == 0){
  delay_us(80);
  if (PORTD.B6 == 1) Check = 1; delay_us(40);}
  }
///////////////////////////////////////
char ReadData(){
  char i, j;
  for(j = 0; j < 8; j++){
  while(!PORTD.B6); //Wait until PORTD.F0 goes HIGH
  delay_us(30);
  if(PORTD.B6 == 0)
  i&= ~(1<<(7 - j)); //Clear bit (7-b)
  else {i|= (1 << (7 - j)); //Set bit (7-b)
  while(PORTD.B6);} //Wait until PORTD.F0 goes LOW
  }
  return i;
  }
///////////////////////////////////////
int data_store(){
    Lcd_Cmd(_LCD_CLEAR);
    val=0;
    while(key!=12)
       {
        IntToStr(val,txt);
        delay_ms(100);
        lcd_out(1,1,"Value =");
        lcd_out(2,1,txt);
        key=keypad_init();
        if(key<17){
        if(key!=10 && key!=11 &&key!=12 &&key!=14 &&key!=15 &&key!=16){
            if(key==13){
            delay_ms(200);
            val=val/10;
            }
        else{
        delay_ms(200);
            val=val*10+key;
            } }}
        delay_ms(100);
        Lcd_Cmd(_LCD_CLEAR);
        }
        Lcd_Cmd(_LCD_CLEAR);
        return val;
    }
///////////////////////////////////////
void lcd(){
    lcd_r1[6]=tem/10+48;
    lcd_r1[7]=tem%10+48;
    lcd_r1[9]=Temp/10+48;
    lcd_r1[10]=Temp%10+48;
    lcd_r2[6]=hum/10+48;
    lcd_r2[7]=hum%10+48;
    lcd_r2[9]=RH/10+48;
    lcd_r2[10]=RH%10+48;
    }
void comp(){
    if(Temp>tem){
      portc.f1=1;
      portc.f2=0;
      }
    else if(Temp<tem){
      portc.f1=0;
      portc.f2=1;
      }
    }