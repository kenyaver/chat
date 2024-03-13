# client-server chat application

Разработка терминального клиент-серверного приложения для обмена текстовыми сообщениями между ограниченной группой лиц.
Пользователь может выбирать, с кем он хочет общаться. Также пользователь может получать сообщения от разных клиентов (в одной сессии).
Возможность отправки сообщений не должна зависеть от статуса сессии партнера (онлайн - подключен к серверу / оффлайн - отключен от сервера)

Оглавление:
1. Протокол взаимодействия клиента и сервера
    1.1 Соединение
    1.2 Структура команды
    1.3 Команда для отправки сообщения клиенту-получателю
    1.4 Оффлайн-команды
    1.5 Ответ
    1.6 len
    1.7 type
    1.8 message id
    1.9 Код ответа
2. Сервер
    2.1 Основной поток
    2.2 Поток обработки клиента
3. Клиент
    3.1 Начало работы приложения
    3.2 Подключение к серверу
    3.3 Запуск потока на принятие команд
    3.4 Отправка команд
    3.5 Завершение потока на принятие команд
    3.6 Завершение работы приложения 


## Протокол взаимодействия клиента и сервера:

### 1. Соединение.
    Соединение происходит по протоколу TCP.

### 2. Структура команды.
    -----------------------------------------------------
    | len(2 bytes) | type(2 bytes) | messageID(4 bytes) |
    -----------------------------------------------------
    |               SRC_username(8 bytes)               |
    -----------------------------------------------------
    |               DST_username(8 bytes)               |
    -----------------------------------------------------
    |               message(0-1000 bytes)               |
    -----------------------------------------------------
    Все команды передаются в бинарном виде. Каждая команда весит от 24 до 1024 байт.
    Поле SRC_username - username клиента-отправителя.
    Поле DST_username - username клиента-получателя.
    message - сообщение клиента-отправителя клиенту-получателю или код ответа.

### 3. Команда для отправки сообщения клиенту-получателю.
    Отправка обычных команд с текстом для клиента-получателя. type = 0.

### 4. Оффлайн-команды.
    Команды, полученные сервером, и не имеющие type=1, во время отсутствия клиента-получателя, получают type=2, накапливаются и сохраняются на сервере.
    При подключении клиента, для которого есть оффлайн-команды, сервер отправляет такие команды клиенту.

### 5. Ответ.
    Ответ нужен для подтверждения состояния подключения клиента-получателя к серверу.
    Клиент-получатель, получивший команду с type=0, должен ответить кодом ответа="200".
    Сервер, не получивший ответ от клиента-получателя в течение 3 секунд, считает клиента-получателя отсоединившимся и отправляет клиенту-отправителю код ответа="300".
    Команда-ответ содержит: type = 1,
        messageID - messageID команды, на которую предполагается ответ,
        message - "200", если клиент-получатель ответил, или "300", если клиент-получатель не ответил,
        DST_username - username клиента-отправителя.

### 6. len.
    Поле len используется для определения размера команды, является целочисленным значением.
    Может принимать значение от 24 до 1024.

### 7. type.
    Тип команды. Используется для возможности определения способа обработки команды.
    type может принимать значение:
    0 - команда для отправки сообщения клиенту-получателю (ответ ожидается).
    1 - ответ (ответ не ожидается).
    2 - оффлайн-команды (ответ не ожидается).

#### 8. messageID.
    Является целым положительным числом. Используется для возможности идентифиации команд.
    Изначально равен 0 и прибавляется по 1 при каждом отправлении команды серверу. 
    Является целочисленным значением от 0 до 4 294 967 295.
    MessageID уникален для каждого SRC_username при отправке команд.

### 9. Код ответа.
    Код ответа используется для информирования о статусе прочтения сообщения клиентом-получателем.
    Код ответа является строкой, которая может заполняться "200", если клиент ответил (заполняется клиентом, отправляющим ответ),
        или "300", если клиент-получатель не ответил в пределах установленного временного лимита(заполняется сервером).