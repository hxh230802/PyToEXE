#ifndef CPPSYNTAXHIGHLIGHTER_H
#define CPPSYNTAXHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QFont>
#include <QString>
#include <QColor>

class CppSyntaxHighlighter : public QSyntaxHighlighter {
public:
    CppSyntaxHighlighter(QTextDocument* parent = nullptr) : QSyntaxHighlighter(parent) {
        // 定义关键字的格式
        QTextCharFormat keywordFormat;
        keywordFormat.setForeground(QColor("#da7510")); // 使用自定义的橙色
        QStringList keywords = {
            "int", "double", "float", "char", "bool", // 关键字列表
            "if", "else", "for", "while", "do", "switch",
            "PyInstaller", "Python", "Platform", "Windows", "Extending", "wrote", "checking", "Bootloader"
        };
        for (const QString& keyword : keywords) {
            HighlightingRule rule;
            rule.pattern = QRegExp("\\b" + keyword + "\\b");
            rule.format = keywordFormat;
            highlightingRules.append(rule);
        }

        // 定义数字和INFO格式
        QTextCharFormat numberInfoFormat;
        numberInfoFormat.setForeground(QColor(Qt::darkGreen));
        HighlightingRule numberRule;
        numberRule.pattern = QRegExp("\\b\\d+\\b|INFO");
        numberRule.format = numberInfoFormat;
        highlightingRules.append(numberRule);

        // 定义错误的格式
        QTextCharFormat errorFormat;
        errorFormat.setForeground(QColor(Qt::red));
        HighlightingRule errorRule;
        errorRule.pattern = QRegExp("Error");
        errorRule.format = errorFormat;
        highlightingRules.append(errorRule);
    }

protected:
    void highlightBlock(const QString& text) override {
        for (const HighlightingRule& rule : highlightingRules) {
            QRegExp expression(rule.pattern);
            int index = expression.indexIn(text);
            while (index >= 0) {
                int length = expression.matchedLength();
                setFormat(index, length, rule.format);
                index = expression.indexIn(text, index + length);
            }
        }
    }

private:
    struct HighlightingRule {
        QRegExp pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;
};

#endif // CPPSYNTAXHIGHLIGHTER_H
