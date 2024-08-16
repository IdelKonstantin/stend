#ifndef UART_RX_PARSER_H
#define UART_RX_PARSER_H

#include <QString>

class uartRxParser {

private:


public:

    static const int oneDigit{1};
    static const int twoDigits{2};

    uartRxParser();

    //TODO: Добавить метод для разбора ответного сообщения при стартапе!!!

    QString getDoubleAsString(const QString& answer, int digNumber = 0) const;
    QString getIntAsString(const QString& answer) const;

};

#endif
