# client-server chat application

разработка терминального клиент-серверного приложения для обмена текстовыми сообщениями попарно между ограниченной группой лиц.

при запуске клиента выбирается партнер. можно запустить несколько клиентов с разными партнерами.
каждая сессия несет username - уникальное имя.

возможность отправки сообщений не должна зависеть от статуса партнера. количество отправляемых команд: не более 4 неподтвержденных партнером.  
размер сообщения не более 1024 байт, если сообщение более 1024, то оно не передается.  
неподтвержденныеые команды сохраняются на сервере и выдаются получателю при подключении.  
буффер неподтвержденных сообщений 4096 байт.

если пользователь не отправляет команд в течении 3 минут, то сервер отправляет клиенту предупреждение  
    если пользователь не отправляет команд в течении 5 минут, то сервер отключает пользователя.

## условные обозначения:
- Статус партнера - состояние искомого собеседника (подключен к серверу ил не подключен)
- username - псевдоним клиента. состоит из 1 слова, состоящего из 8 ангийских букв и цифр
- <> - пользовательское сообщение (набранное пользователем)
- [] - системное сообщение (сформированное системой)
- () - ссылка на пункт
- команда - полноценная отправка клиента серверу или сервера клиенту или в файл
- сообщение - часть команды, являющаяся пользовательским или системным

## Оглавление:
1. [Протокол](#протокол-взаимодействия-клиента-и-сервера)
    - [соединение](#соединение)
    - [первая команда](#первая-команда)
    - [отправка сообщений](#отправка-сообщения-клиенту-получателю)
    - [ответ](#ответ)
    - [message id](#message-id)
    - [status](#status)
    - [message](#message)
    - [завершение сессии общения](#завершение-сессии-общения)
2. [Сервер](#сервер)
3. [Клиент](#клиент)

## Протокол взаимодействия клиента и сервера:

1. ### Соединение 
    Соединение происходит по протоколу TCP

2. ### Первая команда
    первая команда клиента серверу отправляются без форматирований
        \<username partner>\n
    Это сообщение не нумеруется, не записывается в буффер и нигде не сохраняется.
    данную команду отправляет клиент. команда должна быть первой в сессии

3. ### Отправка сообщения клиенту-получателю
    - онлайн:  
    формат:  
    > [message id](#message-id) [\<message>](#message)  

    Конец [\<message>](#message) объявляется переводом каретки (0x0A из ASCII таблицы).  
    отправляются клиентом серверу и сервером клиенту-получателю.  
    клиент-отправитель ожидает получить ответ на свое сообщение.  

    - оффлайн:  
    происходит если клиент-получатель не ответил серверу на полученную им команду в течение 10 сек.  
    формат:  
    > \<client-username: message>

        Конец [\<message>](#message) объявляется переводом каретки (0x0A из ASCII таблицы).  
        Ктправляются клиентом серверу и сервером в файл.  
        Команда записывается в файл \<partner>.txt и выводится клиенту-получателю, когда тот подключается к серверу.  
        Для данных комманд не требуется подтверждение. сервер просто отправляет эти сообщения клиенту-получателю, когда тот подключается к серверу  

4. ### Ответ:
    Клиент отправляет команду серверу:  
    > [[message id]](#message-id) [\<message>](#message)  

    Сервер записывает этот [\<message>](#message) в буффер неподтвержденных команд и отправляет его клиенту-получателю  
    Клиент-получатель отправляет серверу ответ-подтверждение [[id message]](#message-id)  
    Если клиент-получатель не отправляет ответ-подтверждение в течение 10 сек, то сообщение записывается в файл  
    Сервер удаляет команду из буффера неподтвержденных команд  
    Сервер возвращает клиенту-отправителю команду \<message id: status>  
    [[message id]](#message-id) и [\<message>](#message) разделяются пробелом => клиент-получатель будет выводить полученную строку после первого пробела.  
    
    
5. ### Message id:
    является целым положительным числом. Используется для возможности идентифиации сообщений.  
    Изначально равен 0 и прибавляется по 1 при каждом отправлении \<message> клиента серверу. является строкой в десятичной записи числа от 0 до 65 535  
    уникален для клиента-отправителя.  

6. ### Status:
    200 - команда доставлена и прочитана. Прочитанным сообщением счиитается сообщение, на идентификатор которого ответил клиент-получатель серверу в формате:  
    > [[message id]](#message-id)  

    300 - команда доставлена, но не прочитанна (клиент-получатель не ответил серверу). в таком случае команда записывается сервером в файл <partner>.txt
        в формате:  
    > \<username: message>  

    301 - буффер переполнен, поток общения с данным пользователем завершается. 
        отправляется после 4 отправленной и непрочитанной команды (клиент-получатель оффлайн)  

7. ### Message:
    сообщение клиента должно составлять не более 1024 байт на английском или русском языке.  
        Сообщение на русском языке не должно превышать 512 символов русского языка.  
        Сообщение на английском языке не должно превышать 1024 символов английского языка.  
    Конец [\<message>](#message) объявляется переводом каретки (0x0A из ASCII таблицы).  
    Отправляются клиентом серверу и сервером клиенту или в файл.  
    Символ '~' в любом месте [\<message>](#message) сигнализирует о звершеннии сессии общения  

8. ### Завершение сессии общения:
    Если завершение сессии происходит по инициативе 1 из клиентов, то последняя команда отправляется клиенту-получателю.  
        После этого сервер отправляет команду клиенту-получателю о завершении общения "the end session" и программа завершается.  
    Если завершение сессии происходит по инициативе сервера, то сервер отправляет команду клиентам ["server died=("].  

## Сервер:
0. Запуск с параметрами (порт).  

1. Сервер начинает слушать порт.  

2. Сервер получает событие подключения клиента.  

3. Сервер получает username пользователя и никнейм клиента-получателя.  
    - Проверяет никнейм клиента-получателя на доступность собеседника(онлайн).  
    Если username занят, то сервер отправляет сообщение "choose other username". иначе сообщения не будет не будет.  
    Сервер уведомляет клиента-отправителя о состоянии клиента-получателя.  

4. Если клиент-получатель онлайн, то сервер создает поток общения и переводит тудв клиентов.  
    Если клиент-получатель оффлайн, то все полученные команды записывает в файл в пределах установленных выше лимитов.  

5. Сервер получает команду:  
    - Записывает в буффер неподтвержденных команд, отправляет клиенту-получателю.  
    - Ждет от клиента-получателя обратную команду с идентификатором команды [[message id]](#message-id) в течение 15 секунд.  
    - При получении команды от клиента-получателя, сервер возвращает клиенту-отправителю [[message id]](#message-id) [\<status>](#status)  
    - При отсутствии команды-ответа в течение указанного времени, сервер закрывает неактивную сессию, записывает полученную команду в файл
        и возвращает клиенту-отправителю [[message id]](#message-id) [\<status>](#status)  

6. Повторяется шаг 5 до завершения работы.  

7. При получении '~' в [\<message>](#message) (в любом месте) клиента, сервер завершает сессию общения, клиенты отключаются от сервера.  

8. При получении сигнала завершения работы сервера, сервер записывает в файл все обрабатываемые команды из буффера команд в формате \<username: message>,
    завершает все сессии с сообщением ["server died=("].  



## Клиент:
0. Запуск с указанием адрес порт сервера + никнейм + никнейм партнера.  

1. Подключение к серверу.  

3. Передача серверу своего username и username собеседника.  

4. Ожидается любого из событий (5 или 6).  

5. Отправка команд с проверкой на превышение лимитов команды.  
    - Если лимит не превышен, то клиент отправляет серверу [\<message>](#message)
      
    - Если лимит превышен, то клиент получает ошибку, команда не отправляется.  

6. При получении команды, клиент отвечает серверу  
    [id message]  

7. Повторяются шаг 4 до завершения работы.  

8. Для завершения сессии общения, отправляется символ '~' в любом месте [\<message>](#message)