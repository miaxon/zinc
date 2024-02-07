Старый проект, целью которого было сделать оконное приложение, взаимодействющее с Adobe Flash Conrol (shockwaveflash).
Сам контрол размещается в ресурсах приложения, таким образом не требуется его установка в систему.
Flash-ролик, который проигрывается контролом, так же размещен в ресурсах прилождения.

Реализована функциональность т.н. `Layered Window`, когда изображение ролика размещается в окне с полностью прозрачным фоном (пример win.zip в `samples`).

Реализована возможность взаимодействия кода оболочки (С++) и кода флеш-ролика (AS 3.0).

Как результат мы имеем автономное приложение, код которого можно писать на AS 3.0 (в т.ч. задействуя графические возможности flash-технологии), при этом имея доступ к функциям оболочки.

Приложение реализует только основные функции: загрузка ресурсов, инициализация и отображение. Набор функций, доступных из AS 3.0, по задумке можно расширять под конкретное приложение на свое усмотрение.

В директории `samples` есть примеры готовых приложений.

Сделано по мотивам когда-то существовавшего проекта `Zinc`, предоставлявшего аналогичные возможности.

C++ код написан в MSVS C++ 2008. Код флеш ролика разрабатывался в некогда популярном редакторе FlashDevelop.

