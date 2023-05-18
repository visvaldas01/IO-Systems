# Лабораторная работа 3

**Название:** "Разработка драйверов сетевых устройств"

**Цель работы:** получить знания и навыки разработки драйверов сетевых устройств для операционной системы Linux.

## Описание функциональности драйвера

Драйвер создаёт виртуальный сетевой интерфейс в ОС Linux, который перехватывает пакеты родительского интерфейса enp0s3.

## Инструкция по сборке

Сборка модуля:

```
make
```

Очистка:

```
make clean
```

## Инструкция пользователя (работа через терминал)

Отправка UDP-пакета:

```
echo abcd > /dev/udp/localhost/8000
```

Вывод разбора состояния пакетов (интерфейс ```proc```):

```
cat /proc/var5
```

Вывод длин перехваченных пакетов:

```
dmesg
```

## Примеры использования

Пример №1:

```
echo abcd > /dev/udp/localhost/8000
cat /proc/var5
```

Вывод:

```
rx_packets : 1
rx_dropped : 0
rx_bytes   : 33
tx_packets : 0
tx_dropped : 0
tx_bytes   : 0
```

Пример №2:

```
echo abcd > /dev/udp/localhost/8000
dmesg
```

Вывод:

```
[  552.102450] Module virt_net_if loaded
[  552.102469] virt_net_if: create link vni0
[  552.102476] virt_net_if: registered rx handler for lo
[  552.669947] vni0: device opened
[  569.129777] Captured UDP datagram.
[  569.129789] Data length: 5.
```

## Дополнительное задание

Обеспечить динамическое изменение максимального размера перехватываемого пакета.

Пример:

```
echo abcd > /dev/udp/localhost/8000
echo "Set max data length: 15" > /dev/udp/localhost/8000"
dmesg
```

Вывод:

```
[  552.102450] Module virt_net_if loaded
[  552.102469] virt_net_if: create link vni0
[  552.102476] virt_net_if: registered rx handler for lo
[  552.669947] vni0: device opened
[  569.129777] Captured UDP datagram.
[  569.129789] Data length: 5; max data length: 10.
[  573.846346] Max data length updated: 15.
```
