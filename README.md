# client-server chat application

Разработка терминального клиент-серверного приложения для обмена текстовыми сообщениями между ограниченной группой лиц.
Пользователь может выбирать, с кем он хочет общаться. Также пользователь может получать сообщения от разных клиентов (в одной сессии).
Возможность отправки сообщений не должна зависеть от статуса сессии партнера (подключен/не подключен к серверу).

## Условные обозначения:
- статус сессии - активна сессия передачи сообщений от клиента к серверу (и от сервера к клиенту) или нет.
- username - псевдоним клиента. Состоит из 1 слова, состоящего из 7 байт.
- команда - полноценная отправка клиента серверу или сервера клиенту.
- сообщение - часть команды, являющаяся пользовательским или системным.

## Оглавление:
1. [Протокол взаимодействия клиента и сервера](#протокол-взаимодействия-клиента-и-сервера)
    - [Соединение](#соединение)
    - [Структура команды](#cтруктура-команды)
    - [Отправка команды клиенту-получателю](#отправка-команды-клиенту-получателю)
    - [Оффлайн-сообщения](#оффлайн-сообщения)
    - [Ответ](#ответ)
    - [Len](#len)
    - [Type](#type)
    - [Message id](#messageid)
    - [Status](#status)
    - [Message](#message)
    - [Завершение работы клиента](#завершение-работы-клиента)
2. [Сервер](#сервер)
    - [Основной поток](#основной-поток)
    - [Поток обработки клиента](#поток-обработки-клиента)
3. [Клиент](#клиент)


1. ## Протокол взаимодействия клиента и сервера:

### Соединение
    Соединение происходит по протоколу TCP.

### Cтруктура команды
-----------------------------------------------------
| len(2 bytes) | type(2 bytes) | messageID(4 bytes) |
-----------------------------------------------------
|              SRC_username(8 bytes)                |
-----------------------------------------------------
|              DST_username(8 bytes)                |
-----------------------------------------------------
|              message(0-1000 bytes)                |
-----------------------------------------------------

Все команды передаются в бинарном виде. Каждая команда весит от 24 до 1024 байт.
Поле SRC_username зваполняется 1 раз и не может быть изменено (заполняется всегда одинаково).
Поле DST_username заполняется на основе последнего выбранного клиентом-отправителем клиента-получателя. 

### Отправка команды клиенту-получателю
    Размер сообщения не более 1024 байт, если сообщение более 1024, то оно не передается.
    При отправке команды, не подразумевающей смену клиента-получателя, поле type = 0.
    Если отправка команды назначена другому клиенту-получателю, то type = 2,
    в DST_username записывается новый username.
    Команда записывается в буффер неподтвержденных сообщений.
    Клиент-отправитель ожидает получить ответ на свое сообщение.

### Оффлайн-сообщения
    Непрочитанные (отправленные клиенту, пока тот был не подключен к серверу) сообщения сохраняются на сервере и выдаются получателю при подключении.
    type = 4, все остальные поля заполняются в соответствии с параметрами, сохраненными на сервере.

### Ответ
    Ответ нужен для подтверждения жизнеспособности подключения клиента-получателя к серверу.
    Клиент-получатель должен отвечать на команды, подразумевающие ответ.
    Клиент-получатель не отправивший ответ считается отсоединившимся.
    Команда-ответ содержит: type = 1,
        messageID - messageID команды, на которую дан ответ, в message - status, 
        DST_username - username клиента-отправителя.

### len
    Поле len используется для определения размера команды, является целочисленным значением.
    Может принимать значение от 24 до 1024.

### type
    Тип комманды. Используется для возможности определения способа обработки сообщения.
    Классификация сообщений:
    0 - команда для отправки сообщения получателю (ожидается ответ).
    1 - команда-ответ на команду с сообщением (ответ не ожидается).
    2 - команда для отправки сообщения со сменой получателя (ожидается ответ).
    3 - команда для отсоединения клиента от сервера (ответ не ожидается).
    4 - сообщения, полученные во время неактивности клиента (ответ не ожидается).

### MessageID
    Является целым положительным числом. Используется для возможности идентифиации сообщений.
    Изначально равен 0 и прибавляется по 1 при каждом отправлении message клиента серверу. 
    Является целочисленным значением от 1 до 4 294 967 295.
    MessageID уникален для каждого SRC_username при отправке команд.

### Status
    200 - команда доставлена и прочитана. Прочитанным сообщением считается сообщение, на которое ответил клиент-получатель серверу в соответствующем формате.
    300 - команда доставлена, но не прочитанна (клиент-получатель не ответил серверу).

### Message
    Сообщение клиента должно составлять не более 1000 байт.
    Конец message объявляется переводом каретки (0x0A).

### Завершение работы клиента
    Завершающая команда отправляется серверу, где в поле type будет записано 3.
    Такая команда отправляется клиенту-получателю, но не требует ответа от клиента-получателя.
    Клиент-отправитель, отправивший такое сообщение, отключается от сервера.


## Сервер:

### Основной поток
    2.1.1. Запуск с параметрами (порт).
    2.1.2. Сервер начинает слушать порт.
    2.1.3. При получении сигнала о завершении работы сервера, сервер уведомляет все активные потоки о завершении работы.
        2.1.3.1. Сервер переходит к шагу 2.1.6.
    2.1.4. Сервер получает событие подключения клиента.
    2.1.5. Работа с клиентом переводится в отдельный поток.
        2.1.5.1. Основной поток взвращется к шагу 2.1.3 для последующего принятия новых клиентов.
    2.1.6. Сервер ожидает завершения всех потоков.
    2.1.7. Сервер завершает работу.

### Поток обработки клиента
    2.2.1. Сервер получает первую команду.
        2.2.1.1. Сервер заносит username в список пользователей.
        2.2.1.2. Сервер находит партнера для пользователя (по DST_username).
        2.2.1.3. Отправляет все оффлайн-сообщения с type=4 (если есть).
        2.2.1.4. Выполняется пункт 2.2.3.
    2.2.2. Сервер получает команду.
    2.2.3. Сервер обрабатывает полученную команду.
        2.2.3.1. type = 0:
            2.2.3.1.1. Команда пересылается получателю.
            2.2.3.1.2. Включается таймер ожидания ответа.
        2.2.3.2. type = 1:
            2.2.3.2.1. Таймер для сообщения с таким messageID для этого клиента сбрасывается.
            2.2.3.2.2. Если клиент подключен к серверу, то команда отправляется клиенту.
        2.2.3.3. type = 2:
            2.2.3.3.1. Сервер сохраняет нового получателя и будет отправлять все сообщения ему до следующего type=2.
            2.2.3.3.2. Выполняется шаг 2.2.3.1.
        2.2.3.4. type = 3:
            2.2.3.4.1. Команда пересылается партнеру.
            2.2.3.4.2. Клиент отсоединяется от сервера.
            2.2.3.4.3. Буффер неподтвержденных сообщений очищается.
            2.2.3.4.4. Поток завершает работу.
    2.2.4. Если был включен таймер и завершился без ответа клиента, то все обрабатываемые команды сохраняются в специальном файле. Партнер считается отсоединившимся.
    2.2.5. При получении сигнала завершения работы сервера, сервер записывает в файл offline/{DST_username}.txt все обрабатываемые команды из буффера команд.
    2.2.6. Повторяется шаг 2.2.2.


## Клиент:

### Начало работы приложения
    3.1.  Запуск с указанием адреса и порта сервера, username пользователя и партнера.

### Подключение к серверу
    3.2. Подключение к серверу по протоколу TCP.

### Подготовка к обмену командами
    3.3. Запуск потоков на отправку и принятие команд.

### Отправка команд
    3.4.1. Клиент может отправить перед своим сообщением специальный тег, позволяющий изменить партнера (newUser:).
        В таком случае новый партнер сохранится и будет записываться в DST_username до следующего использования тега.
        3.4.1.1. Если клиент не отправляет такого тега, то считается, что собеседник остался прежним.
    3.4.2. Проверяется на превышение лимита на размер сообщения.
        3.4.2.1. Если лимит превышен, то пользователь получает ошибку, команда не отправляется.
    3.4.3. Сообщение форматируется до протокольного вида (len type messageID SRC_username DST_username message).
    3.4.4. Команда отправляется серверу.

### Получение команд
    3.5.1. Команда проверяется на тип сообщения.
    3.5.2. Если type равен 0 или 2, то отправляется ответ на полученную команду.
    3.5.3. Команда парсится до необходимого вида.
    3.5.4. Клиент выводит сообщение в консоль в формате:
        SRC_username: message0x0a

### Завершение работы приложения
    3.6. Для завершения приложения, отправляется символ '~' в любом месте сообщения.
        В поле message, отправляемой команды, будет записано введенное пользователем сообщение целиком(до символа 0x0a), включая '~'.