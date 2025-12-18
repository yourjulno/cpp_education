
int main()
{
    // каждый вызов методов может вызвать исключение
    // person.name()
    // person.grade()
    // person.salary()
    // person.id()

    // если person.grade() выбросит исключение → выход из функции
    // если grade() == 10, то salary() не вызывается
    //
    // если grade() != 10, то salary() может выбросить исключение

    // Вызов save(...)
    // save(Status::success, person.id());
    // save(Status::failure, person.id());
    // person.id()
    // сама функция save(...) может выбросить исключения

    // 

}