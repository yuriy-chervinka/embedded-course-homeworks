All homeworks are going to be included in single repo.  
In order to switch between different programs select proper env (mentioned in each program `env` section) and *Upload* it to board.

---

# Homework 1:
Створити програму для ESP32, яка керує двома світлодіодами (червоний і синій) та імітує роботу поліцейської мигалки.

## Program 1:
**Task:**  
- Використати два GPIO-виводи ESP32.  
- Під’єднати червоний та синій світлодіоди через резистори.  
- Реалізувати послідовність миготіння, у якій червоний і синій LED блимають по черзі.  
- Програма має працювати циклічно.  

**Scheme:**  

    ESP32-S3 DevKit  
    ┌─────────────┐  
    │             │  
    │      GPIO 1 ├──────[ Resistor 220 Ohm ]──────(A) LED_RED (C)──────┐  
    │             │                                                     │  
    │      GPIO 2 ├──────[ Resistor 220 Ohm ]──────(A) LED_BLUE (C)─────┤  
    │             │                                                     │  
    │        GND  ├─────────────────────────────────────────────────────┘  
    │             │  
    └─────────────┘  

  * LED: (A) = Anode (long lead, `+`), (C) = Cathode (short lead, `-`)  

**Notes**  
Simple single loop for both LEDs blinking.  

**env:**  
`env:hw1_program1`  


## Program 2:
**Task:**  
- ускладнити патерн миготіння  

**Scheme:**  

    ESP32-S3 DevKit  
    ┌─────────────┐  
    │             │  
    │      GPIO 1 ├──────[ Resistor 220 Ohm ]──────(A) LED_RED (C)──────┐  
    │             │                                                     │  
    │      GPIO 2 ├──────[ Resistor 220 Ohm ]──────(A) LED_BLUE (C)─────┤  
    │             │                                                     │  
    │        GND  ├─────────────────────────────────────────────────────┘  
    │             │  
    └─────────────┘  

  * LED: (A) = Anode (long lead, `+`), (C) = Cathode (short lead, `-`)  

**Notes**  
Async tasks for independent LEDs control.  

**env:**  
`env:hw1_program2`  


## Program 3:
**Task:**  
- додати третій світлодіод  

**Scheme:**  

    ESP32-S3 DevKit  
    ┌─────────────┐  
    │             │  
    │      GPIO 4 ├──────[ Resistor 220 Ohm ]──────(A) LED_RED (C)──────┐  
    │             │                                                     │  
    │      GPIO 5 ├──────[ Resistor 220 Ohm ]──────(A) LED_BLUE (C)─────┤  
    │             │                                                     │  
    │      GPIO 6 ├──────[ Resistor 220 Ohm ]──────(A) LED_GREEN (C)────┤  
    │             │                                                     │  
    │        GND  ├─────────────────────────────────────────────────────┘  
    │             │  
    └─────────────┘  

  * LED: (A) = Anode (long lead, `+`), (C) = Cathode (short lead, `-`)  

**Notes**  
Simple loop but with LL approach.  

**env:**  
`env:hw1_program3`  


## Program 4:
**Task:**  
- змінювати швидкість миготіння  

**Scheme:**  

    ESP32-S3 DevKit  
    ┌─────────────┐  
    │             │  
    │      GPIO 4 ├──────[ Resistor 220 Ohm ]──────(A) LED_RED (C)──────┐  
    │             │                                                     │  
    │        GND  ├─────────────────────────────────────────────────────┘  
    │             │  
    └─────────────┘  

  * LED: (A) = Anode (long lead, `+`), (C) = Cathode (short lead, `-`)  

**Notes**  
Simple loop but using registers and interruptions.  

**env:**  
`env:hw1_program4`  


## Program 5:
**Task:**  
- зменшити кількість задіяних пінів: змусити обидва світлодіоди блимати по черзі за допомогою лише одного GPIO-виводу.  

**Scheme:**  

    ESP32-S3 DevKit  
    ┌─────────────┐  
    │             │  
    │        3V3  ├──────[ Resistor 220 Ohm ]──────(A) LED_RED (C)──────┐  
    │             │                                                     │
    │      GPIO 4 ├─────────────────────────────────────────────────────┤  
    │             │                                                     │  
    │        GND  ├──────[ Resistor 220 Ohm ]──────(C) LED_BLUE (A)─────┘  
    │             │  
    └─────────────┘  

  * LED: (A) = Anode (long lead, `+`), (C) = Cathode (short lead, `-`)  

**Notes**  
Simple loop. Which LED On is controlled by current direction (level on GPIO).  

**env:**  
`env:hw1_program5`  

---

# Homework 2:
Мета роботи: Навчитися конфігурувати цифрові виводи мікроконтролера в режими входу та виходу, освоїти роботу з вбудованою підтяжкою (INPUT_PULLUP).

**Task:**  
1. Апаратні вимоги (Схемотехніка):
Світлодіоди: Під’єднати два світлодіоди до будь-яких вільних цифрових пінів плати ESP32-S3 (наприклад, GPIO 15 та 16) обов'язково через обмежувальні резистори номіналом 220 Ом.
Зовнішня кнопка: Під’єднати тактову кнопку до одного з вільних GPIO (наприклад, GPIO 21).
Вбудована кнопка: Як другу кнопку використати розпаяну на платі кнопку BOOT (GPIO 0). Додаткового монтажу для неї не потрібно.
2. Опис режимів роботи
Програма повинна циклічно виконувати один із двох режимів, перемикання між якими фіксується в пам'яті (стан за замовчуванням при старті - Режим №1):
Режим №1 (Швидкий / Синхронний): активується натисканням зовнішньої кнопки.
Візуальний ефект: Обидва світлодіоди працюють синхронно - одночасно вмикаються і одночасно вимикаються з короткою затримкою delay(200).
Режим №2 (Повільний / По черзі):  активується натисканням вбудованої кнопки BOOT (GPIO 0).
Візуальний ефект: Світлодіоди миготять по черзі (ефект залізничного переїзду) - коли перший горить, другий згашений, і навпаки, з довгою затримкою delay(1000).
3. Програмні вимоги (Алгоритм)
Ініціалізація пінів:
Налаштувати піни світлодіодів як OUTPUT.
Налаштувати зовнішню кнопку в режим INPUT_PULLUP.
Налаштувати кнопку BOOT у режим INPUT (вона вже має апаратну зовнішню pull-up підтяжку на платі).
Фіксація стану: Після одноразового натискання кнопки вибраний темп і тип миготіння мають зберігатися, поки не буде натиснуто іншу кнопку.

**Scheme:**  

    ESP32-S3 DevKit  
    ┌─────────────┐  
    │ ┌┬┐ *       │  
    │ ─/─  GPIO 0 │  
    │             │  
    │      GPIO 4 ├──────[ Resistor 220 Ohm ]──────(A) LED_RED (C)──────┐  
    │             │                                                     │  
    │      GPIO 5 ├──────[ Resistor 220 Ohm ]──────(A) LED_BLUE (C)─────┤  
    │             │      ┌┬┐                                            │  
    │      GPIO 6 ├───────/─────────────────────────────────────────────┤  
    │             │                                                     │  
    │        GND  ├─────────────────────────────────────────────────────┘  
    │             │  
    └─────────────┘  

  * LED: (A) = Anode (long lead, `+`), (C) = Cathode (short lead, `-`)  
  * built-in 'BOOT' push-button (interconnected with GPIO 0)

**Notes**  
Simple loop. Which LED On is controlled by current direction (level on GPIO).  

**env:**  
`env:hw1_program5`  

---
