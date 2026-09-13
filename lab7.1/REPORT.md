# Лаба 7.1

Вариант 1: `a[0]=1; f=2.0; #pragma omp parallel for; a[i] = f*a[i-1];`

## Поиск ошибки

`clang++ -fopenmp -fsanitize=thread -g -O1 ...` + `TSAN_OPTIONS=ignore_noninstrumented_modules=1 ./a.out`:

```
WARNING: ThreadSanitizer: data race
  Read of size 8 ... geometric_sequence.cpp:11 (a[i-1])
  Previous write of size 8 ... geometric_sequence.cpp:13 (a[i] = f * a[i - 1];)
```

Один поток пишет `a[i]`, другой в это же время читает `a[i-1]` — гонка.

## Исправление

Вместо `a[i] = f*a[i-1]` (зависимость по циклу) считаем явную формулу:

```
a[i] = a[0] * pow(f, i);   // + default(none) shared(a,n) firstprivate(a0,f)
```

Зависимости между итерациями больше нет.

## Повторная проверка

```
$ TSAN_OPTIONS=... ./a.out   # fixed-версия
(без warning)
```
