// RTOSとは?
// http://www.kumikomi.net/archives/2008/12/32rtos.php?page=7
#include <STM32FreeRTOS.h>
#include <Arduino.h>
#include <LiquidCrystal.h>

/*****************************************************************************/
/** タスク1 Lチカ用 ***********************************************************/
/*****************************************************************************/
TaskHandle_t handle1;
static void Task1(void *arg) {
  // LEDのピンをOUTに設定
  pinMode(LED_BUILTIN, OUTPUT);

  // 思う存分チカチカ
  while (true) {
    digitalWrite(LED_BUILTIN, HIGH);
    vTaskDelay(500);//500ms遅延
    digitalWrite(LED_BUILTIN, LOW);
    vTaskDelay(500);//500ms遅延
  }
}
/*****************************************************************************/

/*****************************************************************************/
/** タスク2 シリアル確認用 *****************************************************/
/*****************************************************************************/
TaskHandle_t handle2;
static void Task2(void *arg) {
  // 「for(;;)」も「while(true)」も「while(1)」も同じ
  // 延々と繰り返すという意味
  for (;;) {
    vTaskDelay(500);//500ms遅延
    Serial.println(F("HI SHARLOCK"));
    vTaskDelay(500);//500ms遅延
    Serial.println(F("MISS ME?"));
  }
}
/*****************************************************************************/

/*****************************************************************************/
/** タスク3 LCD確認用 *********************************************************/
/*****************************************************************************/
TaskHandle_t handle3;
// LCD仕様は以下URL参照
// https://wiki.dfrobot.com/Arduino_LCD_KeyPad_Shield__SKU__DFR0009_
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
static void Task3(void *arg) {
  // LCD初期化コード 16文字2行
  lcd.begin( 16, 2 );
  lcd.clear();
  for (;;) {
    vTaskDelay(2000);//1sec遅延
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("What do you mean");
    lcd.setCursor(0, 1);
    lcd.print(", Doc?          ");

    vTaskDelay(2000);//1sec遅延
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("All best stuff a");
    lcd.setCursor(0, 1);
    lcd.print("re made in Japan");
    
    vTaskDelay(2000);//1sec遅延
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Great scott ...!");
    lcd.setCursor(0, 1);
    lcd.print("      from BTTF3");
  }
}
/*****************************************************************************/


/*****************************************************************************/
/** Arduino 初期化コード*******************************************************/
/*****************************************************************************/
void setup() {
  // タスクができたかどうか確認するための変数です
  portBASE_TYPE tsk1, tsk2, tsk3;
  // シリアル通信を有効にしましし
  Serial.begin(9600);
  while(!Serial);
  // 有効にできたら。おはよう世界
  Serial.println(F("HelloWorld!!"));
  // 優先度1 でタスクを作製します
  tsk1 = xTaskCreate(Task1, NULL, configMINIMAL_STACK_SIZE, NULL, 1, &handle1);
  // 優先度2 でタスクを作製します
  tsk2 = xTaskCreate(Task2, NULL, configMINIMAL_STACK_SIZE, NULL, 2, &handle2);
  // 優先度3 でタスクを作製します
  tsk3 = xTaskCreate(Task3, NULL, configMINIMAL_STACK_SIZE, NULL, 3, &handle2);
  // タスクが作成されたか確認します
  if (tsk1 != pdPASS || tsk2 != pdPASS || tsk3 != pdPASS ) {
    Serial.println(F("ERR:タスクが作成できませんでした."));
    for(;;);
  }
  // RealTimeOSを起動する
  Serial.println(F("OSを起動します"));
  vTaskStartScheduler();
  // 注意:OSが起動したはずなので下のコードは実行されてはいけない.
  Serial.println(F("ERR:メモリ不足で起動しなかったものと思われます."));
  for(;;);
}
/*****************************************************************************/

// このloopは使用しません
// それでも消してはいけません
void loop() {}