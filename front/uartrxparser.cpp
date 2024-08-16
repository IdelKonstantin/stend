#include "uartrxparser.h"

uartRxParser::uartRxParser() {}

QString uartRxParser::getDoubleAsString(const QString& answer, int digNumber) const {

    auto argStdString = answer.toStdString();
    auto arg = std::stod(argStdString.substr(8, argStdString.length()));
    arg = arg / 10.0;

    return QString::number(arg, 'f', digNumber);
}

QString uartRxParser::getIntAsString(const QString& answer) const {

    auto argStdString = answer.toStdString();
    auto arg = std::stoi(argStdString.substr(8, argStdString.length()));

    return QString::number(arg);
}
