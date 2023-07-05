# test-project

Было принято, что в игре принимает участие 8 обычных символов и символ Wild. Комбинаций из символ Wild нет.

Для нахождения нужной конфигурации лент использовался случайный поиск. Генерировались случайные ленты длины 12, среди которых выбиралась лучшая по определённой метрике.
Так как множество состояний автомата не велико, при известных лентах можно их все перебрать, тем самым получить аналитические значения статистических показателей. 
Требуемое значение СКО недостижимо, для чего привожу следующее доказательство:
СКО ~= \sqrt(\sum(ПВ - E[ПВ])^2 / N) = \sqrt(\sum(ПВ - 0.92)^2 / N) > \sqrt(0.8*N*(0 - 0.92)^2 / N) = 0.92 * \sqrt(0.8) ~= 82%
В рассчёте число 0.8 появляется из требования Hit = 0.2 => в 80% случаев выйгрыш, а в месте с ним и ПВ, равен 0.

Помимо полного перебора вариантов, в данном репозитории реализована случайная генерация матриц 5x3 и симулятор, вычисляющий все показатели на основе ограниченного количества испытаний.
Программа позволяет настроить по средством файлов в папке input_files ленты барабанов, таблицу выйгрышей, выйгрышные линии.
