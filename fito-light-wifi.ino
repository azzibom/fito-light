#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

#define REL_PIN D1

// -407775979
// 384756738
char ssid[] = "myrzik";
char password[] = "34634600";

#define BOTtoken "1437532775:AAEoDfqHjFpEo24gxznfEcc6P9MvYTpStKk"

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

String keyboardJson = "[[\"/вкл\", \"/выкл\", \"/статус\"]]";


void setup() {
  client.setInsecure();
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void loop() {
  pinMode(REL_PIN, OUTPUT);
  int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
  handleNewMessages(numNewMessages);
}

void handleNewMessages(int numNewMessages) {
  for (int i = 0; i < numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    String text = bot.messages[i].text;

    if (chat_id != "-407775979" /*|| chat_id != '384756738'*/) {
      return;
    }
    if (text == "/start") {
      bot.sendMessageWithReplyKeyboard(chat_id, "Выберите один из следующих вариантов", "", keyboardJson, true);
    } else {
      if (text == "/вкл") {
        digitalWrite(REL_PIN, HIGH);
        bot.sendMessageWithReplyKeyboard(chat_id, "Включен", "", keyboardJson, true); // отправляем ответ в чат
      }

      if (text == "/выкл") {
        digitalWrite(REL_PIN, LOW);
        bot.sendMessageWithReplyKeyboard(chat_id, "Выключен", "", keyboardJson, true); // отправляем ответ в чат
      }

      if (text == "/статус") {
        bool pinStatus = digitalRead(REL_PIN);
        if (pinStatus == 1) {
          bot.sendMessageWithReplyKeyboard(chat_id, "Включен", "", keyboardJson, true); // отправляем ответ в чат
        } else {
          bot.sendMessageWithReplyKeyboard(chat_id, "Выключен", "", keyboardJson, true); // отправляем ответ в чат
        }
      }
    }
  }
}
