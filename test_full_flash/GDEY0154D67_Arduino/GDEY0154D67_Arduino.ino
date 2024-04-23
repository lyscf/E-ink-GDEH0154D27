#include <SPI.h>
#include"Ap_29demo.h"
//IO settings

static const uint8_t D0   = 16;
static const uint8_t D1   = 5;
static const uint8_t D2   = 4;
static const uint8_t D3   = 0;
static const uint8_t D4   = 2;
static const uint8_t D5   = 14;
static const uint8_t D6   = 12;
static const uint8_t D7   = 13;
static const uint8_t D8   = 15;
static const uint8_t D9   = 3;
static const uint8_t D10  = 1;


int BUSY_Pin = D1; 
int RES_Pin = D4; 
int DC_Pin = D2; 
int CS_Pin = D8; 
//HSCLK---D5
//HMOSI--D7
#define EPD_W21_CS_0 digitalWrite(CS_Pin,LOW)
#define EPD_W21_CS_1 digitalWrite(CS_Pin,HIGH)
#define EPD_W21_DC_0  digitalWrite(DC_Pin,LOW)
#define EPD_W21_DC_1  digitalWrite(DC_Pin,HIGH)
#define EPD_W21_RST_0 digitalWrite(RES_Pin,LOW)
#define EPD_W21_RST_1 digitalWrite(RES_Pin,HIGH)
#define isEPD_W21_BUSY digitalRead(BUSY_Pin) 

#define ALLSCREEN_GRAGHBYTES  5000

////////FUNCTION//////
   
void SPI_Write(unsigned char value);
void Epaper_Write_Command(unsigned char command);
void Epaper_Write_Data(unsigned char data);
//EPD
void Epaper_READBUSY(void);
void EPD_HW_Init(void); //Electronic paper initialization
void EPD_Part_Init(void); //Local refresh initialization
void EPD_Part_Update(void); 
void EPD_Update(void);
void EPD_WhiteScreen_Black(void);
void EPD_WhiteScreen_White(void);
void EPD_DeepSleep(void);
//Display 
void EPD_WhiteScreen_ALL(const unsigned char * datas);
void EPD_SetRAMValue_BaseMap(const unsigned char * datas);
void EPD_Dis_Part(unsigned int x_start,unsigned int y_start,const unsigned char * datas,unsigned int PART_COLUMN,unsigned int PART_LINE);


void Sys_run(void)
{
   ESP.wdtFeed(); //Feed dog to prevent system reset
}
void LED_run(void)
{
  digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on (Note that LOW is the voltage level
  delay(500);
  digitalWrite(LED_BUILTIN, HIGH);   // Turn the LED on (Note that LOW is the voltage level
  delay(500);
  }



void setup() 
{
   pinMode(BUSY_Pin, INPUT); 
   pinMode(RES_Pin, OUTPUT);  
   pinMode(DC_Pin, OUTPUT);    
   pinMode(CS_Pin, OUTPUT);    
   //SPI
   SPI.beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE0)); 
   SPI.begin ();  
}
////////Partial refresh schematic////////////////

/////Y/// (0,0)               /---/(x,y)
          //                 /---/
          //                /---/  
          //x
          //
          //
//Tips//
/*When the electronic paper is refreshed in full screen, the picture flicker is a normal phenomenon, and the main function is to clear the display afterimage in the previous picture.
  When the local refresh is performed, the screen does not flash.*/
/*When you need to transplant the driver, you only need to change the corresponding IO. The BUSY pin is the input mode and the others are the output mode. */







// void loop() {
//     unsigned char fen_L,fen_H,miao_L,miao_H; 

//     /************Fast picture display(1.5s)*******************/
//     EPD_HW_Init_Fast(); //EPD init Fast
//     EPD_WhiteScreen_ALL_Fast(gImage_1);//EPD_picture1
//     EPD_DeepSleep();//EPD_DeepSleep,Sleep instruction is necessary, please do not delete!!!
//     delay(2000); //2s  
  
//     EPD_HW_Init_Fast(); //EPD init Fast
//     EPD_WhiteScreen_ALL_Fast(gImage_2);//EPD_picture1
//     EPD_DeepSleep();//EPD_DeepSleep,Sleep instruction is necessary, please do not delete!!!
//     delay(2000); //2s  
    
// //////////////////////Partial refresh digital presentation//////////////////////////////////////  
//     EPD_HW_Init(); //Electronic paper initialization
//     EPD_SetRAMValue_BaseMap(gImage_basemap);  //Partial refresh background color,Brush map is a must, please do not delete
//     for(fen_H=0;fen_H<6;fen_H++)
//     {
//     for(fen_L=0;fen_L<10;fen_L++)
//     {
//     for(miao_H=0;miao_H<6;miao_H++)   
//     {
//     for(miao_L=0;miao_L<10;miao_L++)
//     {
//         EPD_Dis_Part_myself(64,40,Num[miao_L],         //x-A,y-A,DATA-A
//                             64,72,Num[miao_H],         //x-B,y-B,DATA-B
//                             64,112,gImage_numdot,       //x-C,y-C,DATA-C
//                             64,154,Num[fen_L],         //x-D,y-D,DATA-D
//                             64,186,Num[fen_H],32,64);  //x-E,y-E,DATA-E,Resolution 32*64
                            
//                             if((fen_L==0)&&(miao_H==0)&&(miao_L==5))
//                             goto Clear;
//       }
//     }
//     }
    
//     }  
//   ////////////////////////////////////////////////////////////////////////  
//       //Clean
//      Clear:
//     delay(2000); //2s
//     EPD_HW_Init();//Electronic paper initialization
//     EPD_WhiteScreen_White();  //Show all white
//     EPD_DeepSleep();  //Enter deep sleep,Sleep instruction is necessary, please do not delete!!!
//   while(1) 
//     {
//      Sys_run();//System run
//      LED_run();//Breathing lamp
//     }
// }


void loop() 
{
    unsigned char fen_L,fen_H,miao_L,miao_H; 

    EPD_HW_Init(); //Electronic paper initialization
    EPD_WhiteScreen_ALL(gImage_1); //Full screen refresh picture 1
    // delay(4000);
    
//////////////////////Partial refresh digital presentation//////////////////////////////////////  

    // EPD_SetRAMValue_BaseMap(gImage_basemap);  //Partial refresh background color
    // EPD_Part_Init();//Local refresh initialization
   //  delay(100);
   //  EPD_Dis_Part(0,32,gImage_num1,32,32); //x,y,DATA,resolution 32*32
   //  EPD_Dis_Part(0,32,gImage_num2,32,32); //x,y,DATA,resolution 32*32
   //  EPD_Dis_Part(0,32,gImage_num3,32,32); //x,y,DATA,resolution 32*32
   //  EPD_Dis_Part(0,32,gImage_num4,32,32); //x,y,DATA,resolution 32*32
   //  EPD_Dis_Part(0,32,gImage_num5,32,32); //x,y,DATA,resolution 32*32
   //  EPD_Dis_Part(0,32,gImage_num6,32,32); //x,y,DATA,resolution 32*32
   //  EPD_Dis_Part(0,32,gImage_num7,32,32); //x,y,DATA,resolution 32*32
   //  EPD_Dis_Part(0,32,gImage_num8,32,32); //x,y,DATA,resolution 32*32
   //  EPD_Dis_Part(0,32,gImage_num9,32,32); //x,y,DATA,resolution 32*32
   // delay(1000);
  ////////////////////////////////////////////////////////////////////////  
      //Clean

    // EPD_HW_Init();//Electronic paper initialization
    // EPD_WhiteScreen_White();  //Show all white
    EPD_DeepSleep();  //Enter deep sleep,Sleep instruction is necessary, please do not delete!!!
    while(1)
    {
      Sys_run();//System run
      LED_run();//Breathing lamp
     }
}


















































///////////////////EXTERNAL FUNCTION////////////////////////////////////////////////////////////////////////
/////////////////////delay//////////////////////////////////////
// void driver_delay_us(unsigned int xus)  //1us
// {
//   for(;xus>1;xus--);
// }
// void driver_delay_xms(unsigned long xms) //1ms
// {  
//     unsigned long i = 0 , j=0;

//     for(j=0;j<xms;j++)
//   {
//         for(i=0; i<256; i++);
//     }
// }
// void DELAY_S(unsigned int delaytime)     
// {
//   int i,j,k;
//   for(i=0;i<delaytime;i++)
//   {
//     for(j=0;j<4000;j++)           
//     {
//       for(k=0;k<222;k++);
                
//     }
//   }
// }
//////////////////////SPI///////////////////////////////////

void SPI_Write(unsigned char value)                                    
{                                                           
  SPI.transfer(value);
}

void Epaper_Write_Command(unsigned char command)
{
  EPD_W21_CS_0;                   
  EPD_W21_DC_0;   // command write
  SPI_Write(command);
  EPD_W21_CS_1;
}
void Epaper_Write_Data(unsigned char data)
{
  EPD_W21_CS_0;                   
  EPD_W21_DC_1;   // data write
  SPI_Write(data);
  EPD_W21_CS_1;
}

/////////////////EPD settings Functions/////////////////////

/////////////////////////////////////LUT//////////////////////////////////////////////
const unsigned char LUT_DATA[] PROGMEM= 
{    //30 bytes
0x66,0x66,0x44,0x66,0xAA,0x11,
0x80,0x08,0x11,0x18,0x81,0x18,
0x11,0x88,0x11,0x88,0x11,0x88,
0x00,0x00,0xFF,0xFF,0xFF,0xFF,
0x5F,0xAF,0xFF,0xFF,0x2F,0x00
}; 
const unsigned char LUT_DATA_part[] PROGMEM=
{  //30 bytes
0x10,0x18,0x18,0x28,0x18,0x18,
0x18,0x18,0x08,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x13,0x11,0x22,0x63,
0x11,0x00,0x00,0x00,0x00,0x00
};        
void EPD_select_LUT(const unsigned char * wave_data)
{        
     unsigned char count;
     Epaper_Write_Command(0x32);
   for(count=0;count<30;count++)
  Epaper_Write_Data(pgm_read_byte(&wave_data[count]));

} 




//////////////////////////////////////////////////////////////////////////////////////////////////
// void EPD_HW_Init(void)
// {
//   EPD_W21_RST_0;  // Module reset      
//   delay(10); //At least 10ms delay 
//   EPD_W21_RST_1; 
//   delay(10); //At least 10ms delay 

//   Epaper_READBUSY();   
//   Epaper_Write_Command(0x12);  //SWRESET
//   Epaper_READBUSY();   
    
//   Epaper_Write_Command(0x01); //Driver output control      
//   Epaper_Write_Data(0xC7);
//   Epaper_Write_Data(0x00);
//   Epaper_Write_Data(0x00);

//   Epaper_Write_Command(0x11); //data entry mode       
//   Epaper_Write_Data(0x01);

//   Epaper_Write_Command(0x44); //set Ram-X address start/end position   
//   Epaper_Write_Data(0x00);
//   Epaper_Write_Data(0x18);    //0x0C-->(18+1)*8=200

//   Epaper_Write_Command(0x45); //set Ram-Y address start/end position          
//   Epaper_Write_Data(0xC7);   //0xC7-->(199+1)=200
//   Epaper_Write_Data(0x00);
//   Epaper_Write_Data(0x00);
//   Epaper_Write_Data(0x00); 

//   Epaper_Write_Command(0x3C); //BorderWavefrom
//   Epaper_Write_Data(0x05);  
      
//   Epaper_Write_Command(0x18); //Reading temperature sensor
//   Epaper_Write_Data(0x80);  

//   Epaper_Write_Command(0x4E);   // set RAM x address count to 0;
//   Epaper_Write_Data(0x00);
//   Epaper_Write_Command(0x4F);   // set RAM y address count to 0X199;    
//   Epaper_Write_Data(0xC7);
//   Epaper_Write_Data(0x00);
//   Epaper_READBUSY();  
// }

void EPD_HW_Init(void)
{
  EPD_W21_RST_0;  // Module reset      
  delay(10); //At least 10ms delay 
  EPD_W21_RST_1; 
  delay(10); //At least 10ms delay 

  Epaper_Write_Command(0x01); //Driver output control      
  Epaper_Write_Data(0xC7);  //Y
  Epaper_Write_Data(0x00);
  Epaper_Write_Data(0x00);
  
  Epaper_Write_Command(0x0C); //softstart  
  Epaper_Write_Data(0xD7);   //is necessary
  Epaper_Write_Data(0xD6);   //is necessary
  Epaper_Write_Data(0x9D);  //is necessary

  Epaper_Write_Command(0x2C);     //VCOM Voltage
  Epaper_Write_Data(0x77);    

  Epaper_Write_Command(0x3A);     //Dummy Line   
  Epaper_Write_Data(0x1A);    
  Epaper_Write_Command(0x3B);     //Gate time 
  Epaper_Write_Data(0X08);  
  
  Epaper_Write_Command(0x11); //data entry mode       
  Epaper_Write_Data(0x01);

  Epaper_Write_Command(0x3C); //BorderWavefrom
  Epaper_Write_Data(0x33);  

  Epaper_Write_Command(0x44); //set Ram-X address start/end position   
  Epaper_Write_Data(0x00);
  Epaper_Write_Data(0x18);    //0x18-->(24+1)*8=200

  Epaper_Write_Command(0x45); //set Ram-Y address start/end position          
  Epaper_Write_Data(0xC7);   //0xC7-->(199+1)=200
  Epaper_Write_Data(0x00);
  Epaper_Write_Data(0x00);
  Epaper_Write_Data(0x00); 

  Epaper_Write_Command(0x4E);   // set RAM x address count to 0;
  Epaper_Write_Data(0x00);
  Epaper_Write_Command(0x4F);   // set RAM y address count to 0X127;    
  Epaper_Write_Data(0xC7);  //Y
  Epaper_Write_Data(0x00);
  Epaper_READBUSY();
  EPD_select_LUT(LUT_DATA); //LUT
  
}

//////////////////////////////All screen update////////////////////////////////////////////
void EPD_WhiteScreen_ALL(const unsigned char * datas)
{
   unsigned int i;
    Epaper_Write_Command(0x24);   //write RAM for black(0)/white (1)
   for(i=0;i<5000;i++)
   {               
     Epaper_Write_Data(pgm_read_byte(&datas[i]));
   }
   EPD_Update();   
}

/////////////////////////////////////////////////////////////////////////////////////////
void EPD_Update(void)
{   
  Epaper_Write_Command(0x22); //Display Update Control 
  Epaper_Write_Data(0xC7);   
  Epaper_Write_Command(0x20); //Activate Display Update Sequence
  Epaper_READBUSY();  
}


void EPD_DeepSleep(void)
{  
  Epaper_Write_Command(0x10); //enter deep sleep
  Epaper_Write_Data(0x01); 
  delay(100);
}
void Epaper_READBUSY(void)
{ 
  while(1)
  {   //=1 BUSY
     if(isEPD_W21_BUSY==0) break;
      ESP.wdtFeed(); //Feed dog to prevent system reset
  }  
}



///////////////////////////Part update//////////////////////////////////////////////
void EPD_SetRAMValue_BaseMap( const unsigned char * datas)
{
  unsigned int i;   
  const unsigned char  *datas_flag; 
  unsigned int x_end,y_start1,y_start2,y_end1,y_end2;
  unsigned int x_start=0,y_start=200;
  unsigned int PART_COLUMN=200,PART_LINE=200;  
  datas_flag=datas;
  //FULL update
  EPD_HW_Init(); 
  Epaper_Write_Command(0x24);   //Write Black and White image to RAM
   for(i=0;i<ALLSCREEN_GRAGHBYTES;i++)
   {               
     Epaper_Write_Data(pgm_read_byte(&datas[i]));
   } 
  EPD_Update();  
  delay(100);  
  //PART update
  EPD_Part_Init();//Local initialization (the top left corner of the screen is the origin of the coordinates)
  datas=datas_flag;//Record array first address
  x_start=x_start/8;//Convert to byte
  x_end=(x_start+PART_LINE)/8-1; 
  
  y_start1=0;
  y_start2=y_start-1;
  if(y_start>=256)
  {
    y_start1=y_start2/256;
    y_start2=y_start2%256;
  }
  y_end1=0;
  y_end2=0;
  if(y_end2>=256)
  {
    y_end1=y_end2/256;
    y_end2=y_end2%256;    
  }   

  Epaper_Write_Command(0x44);       // set RAM x address start/end, in page 35
  Epaper_Write_Data(x_start);    // RAM x address start at 00h;
  Epaper_Write_Data(x_end);    // RAM x address end at 0fh(15+1)*8->128 
  Epaper_Write_Command(0x45);       // set RAM y address start/end, in page 35
  Epaper_Write_Data(y_start2);    // RAM y address start at 0127h;
  Epaper_Write_Data(y_start1);    // RAM y address start at 0127h;
  Epaper_Write_Data(y_end2);    // RAM y address end at 00h;
  Epaper_Write_Data(y_end1);    // ????=0 


  Epaper_Write_Command(0x4E);   // set RAM x address count to 0;
  Epaper_Write_Data(x_start); 
  Epaper_Write_Command(0x4F);   // set RAM y address count to 0X127;    
  Epaper_Write_Data(y_start2);
  Epaper_Write_Data(y_start1);
  

   Epaper_Write_Command(0x24);   //Write Black and White image to RAM
   for(i=0;i<PART_COLUMN*PART_LINE/8;i++)
   {                         
     Epaper_Write_Data(pgm_read_byte(&datas[i]));
   } 
   EPD_Part_Update();
   delay(100); 
     
  datas=datas_flag;
  Epaper_Write_Command(0x44);       // set RAM x address start/end, in page 35
  Epaper_Write_Data(x_start);    // RAM x address start at 00h;
  Epaper_Write_Data(x_end);    // RAM x address end at 0fh(15+1)*8->128 
  Epaper_Write_Command(0x45);       // set RAM y address start/end, in page 35
  Epaper_Write_Data(y_start2);    // RAM y address start at 0127h;
  Epaper_Write_Data(y_start1);    // RAM y address start at 0127h;
  Epaper_Write_Data(y_end2);    // RAM y address end at 00h;
  Epaper_Write_Data(y_end1);    // ????=0 


  Epaper_Write_Command(0x4E);   // set RAM x address count to 0;
  Epaper_Write_Data(x_start); 
  Epaper_Write_Command(0x4F);   // set RAM y address count to 0X127;    
  Epaper_Write_Data(y_start2);
  Epaper_Write_Data(y_start1);
  
   Epaper_Write_Command(0x24);   //Write Black and White image to RAM
   for(i=0;i<PART_COLUMN*PART_LINE/8;i++)
   {                         
     Epaper_Write_Data(pgm_read_byte(&datas[i]));
   }   
   
}

void EPD_Part_Init(void)
{
  EPD_HW_Init(); //Electronic paper initialization
  EPD_select_LUT(LUT_DATA_part);

  //POWER ON
  Epaper_Write_Command(0x22); //Display Update Control
  Epaper_Write_Data(0xC0);   
  Epaper_Write_Command(0x20); //Activate Display Update Sequence
  Epaper_READBUSY();  
  
  Epaper_Write_Command(0x3C); //BorderWavefrom
  Epaper_Write_Data(0x01);

}



void EPD_Dis_Part(unsigned int x_start,unsigned int y_start,const unsigned char * datas,unsigned int PART_COLUMN,unsigned int PART_LINE)
{
  const unsigned char  *datas_flag; 
  unsigned int i;  
  unsigned int x_end,y_start1,y_start2,y_end1,y_end2;
  datas_flag=datas;//Record array first address
  x_start=x_start/8;//Convert to byte
  x_end=x_start+PART_LINE/8-1; 
  
  y_start1=0;
  y_start2=y_start-1;
  if(y_start>=256)
  {
    y_start1=y_start2/256;
    y_start2=y_start2%256;
  }
  y_end1=0;
  y_end2=y_start+PART_COLUMN-1;
  if(y_end2>=256)
  {
    y_end1=y_end2/256;
    y_end2=y_end2%256;    
  }   
  
  Epaper_Write_Command(0x44);       // set RAM x address start/end, in page 35
  Epaper_Write_Data(x_start);    // RAM x address start at 00h;
  Epaper_Write_Data(x_end);    // RAM x address end at 0fh(15+1)*8->128 
  Epaper_Write_Command(0x45);       // set RAM y address start/end, in page 35
  Epaper_Write_Data(y_start2);    // RAM y address start at 0127h;
  Epaper_Write_Data(y_start1);    // RAM y address start at 0127h;
  Epaper_Write_Data(y_end2);    // RAM y address end at 00h;
  Epaper_Write_Data(y_end1);    // ????=0 


  Epaper_Write_Command(0x4E);   // set RAM x address count to 0;
  Epaper_Write_Data(x_start); 
  Epaper_Write_Command(0x4F);   // set RAM y address count to 0X127;    
  Epaper_Write_Data(y_start2);
  Epaper_Write_Data(y_start1);
  
  
   Epaper_Write_Command(0x24);   //Write Black and White image to RAM
   for(i=0;i<PART_COLUMN*PART_LINE/8;i++)
   {                         
     Epaper_Write_Data(pgm_read_byte(&datas[i]));
   } 
   EPD_Part_Update(); 
   
   
   datas=datas_flag;
  Epaper_Write_Command(0x44);       // set RAM x address start/end, in page 35
  Epaper_Write_Data(x_start);    // RAM x address start at 00h;
  Epaper_Write_Data(x_end);    // RAM x address end at 0fh(15+1)*8->128 
  Epaper_Write_Command(0x45);       // set RAM y address start/end, in page 35
  Epaper_Write_Data(y_start2);    // RAM y address start at 0127h;
  Epaper_Write_Data(y_start1);    // RAM y address start at 0127h;
  Epaper_Write_Data(y_end2);    // RAM y address end at 00h;
  Epaper_Write_Data(y_end1);    // ????=0 


  Epaper_Write_Command(0x4E);   // set RAM x address count to 0;
  Epaper_Write_Data(x_start); 
  Epaper_Write_Command(0x4F);   // set RAM y address count to 0X127;    
  Epaper_Write_Data(y_start2);
  Epaper_Write_Data(y_start1);
  
  
   Epaper_Write_Command(0x24);   //Write Black and White image to RAM
   for(i=0;i<PART_COLUMN*PART_LINE/8;i++)
   {                         
     Epaper_Write_Data(pgm_read_byte(&datas[i]));
   } 

}

void EPD_Part_Update(void)
{
  Epaper_Write_Command(0x22); 
  Epaper_Write_Data(0x04);   //different
  Epaper_Write_Command(0x20); 
  Epaper_READBUSY();    
}


/////////////////////////////////Single display////////////////////////////////////////////////

void EPD_WhiteScreen_Black(void)

{
   unsigned int i,k;
    Epaper_Write_Command(0x24);   //write RAM for black(0)/white (1)
  for(k=0;k<200;k++)
  {
    for(i=0;i<25;i++)
    {
      Epaper_Write_Data(0x00);
      }
  }
    EPD_Update();
}

void EPD_WhiteScreen_White(void)

{
   unsigned int i,k;
    Epaper_Write_Command(0x24);   //write RAM for black(0)/white (1)
  for(k=0;k<200;k++)
  {
    for(i=0;i<25;i++)
    {
      Epaper_Write_Data(0xff);
      }
  }
  EPD_Update();
}


//////////////////////////////////END//////////////////////////////////////////////////
