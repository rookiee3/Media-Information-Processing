void LD_Init_Common()
{
  bMp3Play = 0;
  LD_ReadReg(0x06); 
  LD_WriteReg(0x17, 0x35); 
  delay(10);
LD_ReadReg(0x06); 
LD_WriteReg(0x89, 0x03); 
delay(5);
LD_WriteReg(0xCF, 0x43); 
delay(5);
LD_WriteReg(0xCB, 0x02);
/*PLL setting*/
LD_WriteReg(0x11, LD_PLL_11); 
if (nLD_Mode == LD_MODE_MP3)
{
LD_WriteReg(0x1E, 0x00); 
//!!注意，下面三个寄存器，会随晶振频率变化而设置不同
//!!注意,请根据使用的晶振频率修改参考程序中的 CLK_IN
LD_WriteReg(0x19, LD_PLL_MP3_19); 
LD_WriteReg(0x1B, LD_PLL_MP3_1B); 
LD_WriteReg(0x1D, LD_PLL_MP3_1D); }
else
{
//
LD_WriteReg(0x1E,0x00);
//!!注意，下面三个寄存器，会随晶振频率变化而设置不同
//!!注意,请根据使用的晶振频率修改参考程序中的 CLK_IN
LD_WriteReg(0x19, LD_PLL_ASR_19); 
LD_WriteReg(0x1B, LD_PLL_ASR_1B);
LD_WriteReg(0x1D, LD_PLL_ASR_1D);
}
LD_WriteReg(0xCD, 0x04);
LD_WriteReg(0x17, 0x4c); 
delay(5);
LD_WriteReg(0xB9, 0x00);
LD_WriteReg(0xCF, 0x4f); 
LD_WriteReg(0x6F, 0xFF);
}
