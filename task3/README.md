# Задача 3. Сжатие изображений с помощью SVD

[Репозиторий](https://github.com/RoketFlame/imageSVD) с реализацией утилиты.

## Эксперимент

Код для проведения эксперимента: [main.py](./main.py)

Было реализовано 3 алгоритма получения SVD: numpy, power-iterations, block-power-iter.
Подробнее рассказано в репозитории утилиты.

Были взяты следующие значения для степени компрессии: 2, 5, 10, 20, 50, 100.
Однако не все изображения удалось сжать в 50 и 100 раз, из-за недостаточного размера самих изображений.

Ниже представлена таблица с полученными результатами для трех изображений.
Все полученные изображения находятся в директории decompressed.

| Method\Power     | 2                                               | 5                                               | 10                                               | 20                                               | 50                                               | 100                                               |
|------------------|-------------------------------------------------|-------------------------------------------------|--------------------------------------------------|--------------------------------------------------|--------------------------------------------------|---------------------------------------------------|
| Numpy            | ![](./decompressed/numpy/2-tree.bmp)            | ![](./decompressed/numpy/5-tree.bmp)            | ![](./decompressed/numpy/10-tree.bmp)            | ![](./decompressed/numpy/20-tree.bmp)            | ![](./decompressed/numpy/50-tree.bmp)            | ![](./decompressed/numpy/100-tree.bmp)            |
| Power-iterations | ![](./decompressed/power-iterations/2-tree.bmp) | ![](./decompressed/power-iterations/5-tree.bmp) | ![](./decompressed/power-iterations/10-tree.bmp) | ![](./decompressed/power-iterations/20-tree.bmp) | ![](./decompressed/power-iterations/50-tree.bmp) | ![](./decompressed/power-iterations/100-tree.bmp) |
| Block-power-iter | ![](./decompressed/block-power-iter/2-tree.bmp) | ![](./decompressed/block-power-iter/5-tree.bmp) | ![](./decompressed/block-power-iter/10-tree.bmp) | ![](./decompressed/block-power-iter/20-tree.bmp) | ![](./decompressed/block-power-iter/50-tree.bmp) | ![](./decompressed/block-power-iter/100-tree.bmp) |


| Method\Power     | 2                                                 | 5                                                 | 10                                                 | 20                                                 | 50                                                 | 100                                                 |
|------------------|---------------------------------------------------|---------------------------------------------------|----------------------------------------------------|----------------------------------------------------|----------------------------------------------------|-----------------------------------------------------|
| Numpy            | ![](./decompressed/numpy/2-nature.bmp)            | ![](./decompressed/numpy/5-nature.bmp)            | ![](./decompressed/numpy/10-nature.bmp)            | ![](./decompressed/numpy/20-nature.bmp)            | ![](./decompressed/numpy/50-nature.bmp)            | ![](./decompressed/numpy/100-nature.bmp)            |
| Power-iterations | ![](./decompressed/power-iterations/2-nature.bmp) | ![](./decompressed/power-iterations/5-nature.bmp) | ![](./decompressed/power-iterations/10-nature.bmp) | ![](./decompressed/power-iterations/20-nature.bmp) | ![](./decompressed/power-iterations/50-nature.bmp) | ![](./decompressed/power-iterations/100-nature.bmp) |
| Block-power-iter | ![](./decompressed/block-power-iter/2-nature.bmp) | ![](./decompressed/block-power-iter/5-nature.bmp) | ![](./decompressed/block-power-iter/10-nature.bmp) | ![](./decompressed/block-power-iter/20-nature.bmp) | ![](./decompressed/block-power-iter/50-nature.bmp) | ![](./decompressed/block-power-iter/100-nature.bmp) |


| Method\Power     | 2                                                  | 5                                                  | 10                                                  | 20                                                  | 50                                                  | 100                                                  |
|------------------|----------------------------------------------------|----------------------------------------------------|-----------------------------------------------------|-----------------------------------------------------|-----------------------------------------------------|------------------------------------------------------|
| Numpy            | ![](./decompressed/numpy/2-luntik3.bmp)            | ![](./decompressed/numpy/5-luntik3.bmp)            | ![](./decompressed/numpy/10-luntik3.bmp)            | ![](./decompressed/numpy/20-luntik3.bmp)            | ![](./decompressed/numpy/50-luntik3.bmp)            | ![](./decompressed/numpy/100-luntik3.bmp)            |
| Power-iterations | ![](./decompressed/power-iterations/2-luntik3.bmp) | ![](./decompressed/power-iterations/5-luntik3.bmp) | ![](./decompressed/power-iterations/10-luntik3.bmp) | ![](./decompressed/power-iterations/20-luntik3.bmp) | ![](./decompressed/power-iterations/50-luntik3.bmp) | ![](./decompressed/power-iterations/100-luntik3.bmp) |
| Block-power-iter | ![](./decompressed/block-power-iter/2-luntik3.bmp) | ![](./decompressed/block-power-iter/5-luntik3.bmp) | ![](./decompressed/block-power-iter/10-luntik3.bmp) | ![](./decompressed/block-power-iter/20-luntik3.bmp) | ![](./decompressed/block-power-iter/50-luntik3.bmp) | ![](./decompressed/block-power-iter/100-luntik3.bmp) |

## Выводы
Как видно из таблицы метод numpy и block-power-iter дают схожие результаты для всех изображений, 
однако на практике метод numpy оказался в несколько раз производительнее.
Метод power-iterations дает схожие результаты вплоть до силы компрессии в 20 раз, 
однако потом на нескольких изображениях начинают проявляться специфичные артефакты и искажение цветов. 
Также этот метод оказался самым неэффективным из всех.
