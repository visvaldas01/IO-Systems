# Лабораторная работа 2

**Название:** "Разработка драйверов блочных устройств"

**Цель работы:** получить знания и навыки разработки драйверов блочных
устройств для операционной системы Linux.

## Описание функциональности драйвера

Создаёт блочное устройство vramdisk и таблицу разделов внутри него.

## Инструкция по сборке

Сборка модуля:

```
make
```

Очистка:

```
make clean
```

## Changelog

```
diff main.c main5.c

331,334c331,332

<   //device.gd = alloc_disk(7); // gendisk allocation

<   //device.gd = blk_mq_alloc_disk_node(&(device.tag_set), &device);

<   //device.gd = blk_alloc_disk(7);

<   device.gd = __alloc_disk_node(device.queue, 7, &sr_bio_compl_lkclass);

\---

\>   device.gd = alloc_disk(7); // gendisk allocation

\> 

341d338

<   device.gd->minors = 1;
```

## Ход мыслей

Начиная с 5.15 версии ядра, ```genhd.h``` стали сильно изменять, а в 6 версии и вовсе убрали, поэтому программа не собиралась из-за отсутствия функции ```alloc_disk```. В новой версии ядра появилось несколько аналогов, но при их использовании программа выводила в ```dmesg``` предупреждение (at block/genhd.c:416 device_add_disk+0x19a/0x3d0), и при всех, кроме ```__alloc_disk_node```, система зависала намертво при попытке выгрузки модуля. Предупреждение удалось исправить путём добавления строки ```device.gd->minors = 1```. Теперь при загрузке модуля в ```dmesg``` выводится номер драйвера, размер, open и closed, он спокойно выгружается и отображается в средствах для просмотра разделов жёсткого диска, но одна их половина показывает внутренние разделы (```parted```, ```fdisk```), а вторая этого не делает (```lsblk```, ```blkid```, директория ```/dev```). Есть интуитивное предположение, что это может быть связано с тем, что на драйвере имеется таблица разделов DOS, но он не отформатирован в поддерживаемую Linux файловую систему и/или не смонтирован.

spinics.net/lists/linux-modules/msg03243.html

patchwork.kernel.org/project/linux-block/patch/20210816131910.615153-6-hch@lst.de

## Инструкция пользователя

Сборка модуля - ```make```

Загрузка модуля - ```sudo insmod main.ko```

Проверка логов кольцевого буфера - ```sudo dmesg```

Проверка присутствия в списке разделов жёсткого диска: ```sudo fdisk -l /dev/vramdisk```, ```sudo parted /dev/vramdisk print```, ```lsblk -f```, ```blkid -o list```, и ещё множество способов.

## Примеры использования

```ls /dev```
![изображение](https://user-images.githubusercontent.com/60853667/227528036-e866b6c7-dc38-4533-92b4-173d73d35272.png)

```lsblk```
![изображение](https://user-images.githubusercontent.com/60853667/227528845-1a9633c3-a564-434d-a8cc-408a60734b57.png)

```fdisk```
![изображение](https://user-images.githubusercontent.com/60853667/227524529-a2451307-132d-40fc-b1e1-5cad3ec21a0a.png)

```parted```
![изображение](https://user-images.githubusercontent.com/60853667/227524843-c11265e5-b67a-4153-ae8c-a95fb9841e0c.png)

```blkid```
![изображение](https://user-images.githubusercontent.com/60853667/227525661-3cca51ff-d4c6-4e86-9b5e-abcab0201854.png)
