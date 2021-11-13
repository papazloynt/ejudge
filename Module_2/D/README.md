# Автокоррекция
Реализуйте программу, которая предлагает варианты замены слова, в котором допущена одна ошибка.

Для решения этой задачи реализуйте префиксное дерево (возможно сжатое).

Регистр букв для программы коррекции не имеет значения (слова в словаре хранятся в нижнем регистре).

Варианты ошибок - как в алгоритме Дамерау-Левенштейна: вставка лишнего символа, удаление символа, замена символа или транспозиция соседних символов.

Реализация алгоритма должна быть инкапсулирована. В комментариях напишите сложность ключевых алгоритмов с пояснением.

# Формат входных данных
Данные подаются на стандартный поток ввода. Пустые строки игнорируются.

Первая строка содержит число N - количество слов в словаре.

Последующие N строк содержат слова из словаря, по одному в строке.

Остальные строки - слова, которые надо проверять.

# Формат результата
Каждая строка выхода содержит предложение для исправления слова. Слова обрабатываются в порядке их появления.

Если слово не содержит ошибок, то выводится "%слово% - ok".

Если слово содержит одну ошибку, то выводится "%слово% -> %слово_в_словаре%". Если вариантов несколько, то они сортируются лексикографически и разделяются запятой с пробелом.

Если слово содержит более одной ошибки, то выводится "%слово% -?"

Результат работы программы выводится в стандартный поток вывода.