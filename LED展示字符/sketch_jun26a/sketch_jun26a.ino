// ASC码的"We will make China strong"
const byte message[] = {
5,7,6,5,2,0,7,7,6,9,6,'c',6,'c',2,0,6,'d',6,1,6,'b',6,5,2,0,4,3,6,8,6,9,6,'e',6,1,2,0,7,3,7,4,7,2,6,'f',6,'e',6,7
};

void setup() {
// 设置小灯引脚为输出模式
pinMode(LED0, OUTPUT);
pinMode(LED1, OUTPUT);
pinMode(LED2, OUTPUT);
pinMode(LED3, OUTPUT);
}

void loop() {
// 逐个显示ASC码中的字母
for (int i = 0; i < sizeof(message); i++) {
displayCharacter(message[i]); // 显示当前字母
delay(1000); // 延时1秒
turnOffLeds(); // 关闭所有小灯
delay(500); // 延时0.5秒
}
}

void displayCharacter(byte character) {
// 将字母的ASC码转换为二进制，并通过四个小灯显示
digitalWrite(LED0, bitRead(character, 0));
digitalWrite(LED1, bitRead(character, 1));
digitalWrite(LED2, bitRead(character, 2));
digitalWrite(LED3, bitRead(character, 3));
}

void turnOffLeds() {
// 关闭所有小灯
digitalWrite(LED0, LOW);
digitalWrite(LED1, LOW);
digitalWrite(LED2, LOW);
digitalWrite(LED3, LOW);
}
