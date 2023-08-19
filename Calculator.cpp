#include <QApplication>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QWidget>
#include <QStringList>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Create the main window
    QWidget window;
    window.setWindowTitle("Simple Calculator");

    // Create the display
    QLineEdit *display = new QLineEdit(&window);
    display->setReadOnly(true);
    display->setAlignment(Qt::AlignRight);
    display->setText("0");

    // Create a label to show user input
    QLabel *inputLabel = new QLabel(&window);
    inputLabel->setAlignment(Qt::AlignRight);
    inputLabel->setText("");

    // Create buttons for digits and operations
    QPushButton *buttons[12]; // 0 to 9, decimal point, and equal button
    for (int i = 0; i < 10; ++i) {
        buttons[i] = new QPushButton(QString::number(i), &window);
        buttons[i]->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    }
    buttons[10] = new QPushButton(".", &window);
    buttons[11] = new QPushButton("=", &window);
    QPushButton *addButton = new QPushButton("+", &window);
    QPushButton *subButton = new QPushButton("-", &window);
    QPushButton *mulButton = new QPushButton("*", &window);
    QPushButton *divButton = new QPushButton("/", &window);
    QPushButton *clearButton = new QPushButton("C", &window);

    // Set up the layout
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(display, 0, 0, 1, 4);
    layout->addWidget(inputLabel, 1, 0, 1, 4);
    for (int i = 1; i <= 9; ++i) {
        int row = (9 - i) / 3 + 2;
        int col = (i - 1) % 3;
        layout->addWidget(buttons[i], row, col);
    }
    layout->addWidget(buttons[0], 5, 0);
    layout->addWidget(buttons[10], 5, 1);
    layout->addWidget(buttons[11], 5, 2);
    layout->addWidget(addButton, 2, 3);
    layout->addWidget(subButton, 3, 3);
    layout->addWidget(mulButton, 4, 3);
    layout->addWidget(divButton, 5, 3);
    layout->addWidget(clearButton, 6, 0, 1, 4);

    // Set the layout for the window
    window.setLayout(layout);

    // Connect signals and slots for button clicks
    QString currentInput;
    bool decimalClicked = false;

    QObject::connect(buttons[0], &QPushButton::clicked, [&currentInput, inputLabel]() {
        currentInput.append("0");
        inputLabel->setText(currentInput);
    });

    for (int i = 1; i <= 9; ++i) {
        QObject::connect(buttons[i], &QPushButton::clicked, [&currentInput, i, inputLabel]() {
            currentInput.append(QString::number(i));
            inputLabel->setText(currentInput);
        });
    }

    QObject::connect(buttons[10], &QPushButton::clicked, [&currentInput, &decimalClicked, inputLabel]() {
        if (!decimalClicked) {
            currentInput.append(".");
            decimalClicked = true;
            inputLabel->setText(currentInput);
        }
    });

    QObject::connect(addButton, &QPushButton::clicked, [&currentInput, &decimalClicked, inputLabel]() {
        if (!currentInput.isEmpty() && currentInput != ".") {
            currentInput.append(" + ");
            decimalClicked = false;
            inputLabel->setText(currentInput);
        }
    });

    QObject::connect(subButton, &QPushButton::clicked, [&currentInput, &decimalClicked, inputLabel]() {
        if (!currentInput.isEmpty() && currentInput != ".") {
            currentInput.append(" - ");
            decimalClicked = false;
            inputLabel->setText(currentInput);
        }
    });

    QObject::connect(mulButton, &QPushButton::clicked, [&currentInput, &decimalClicked, inputLabel]() {
        if (!currentInput.isEmpty() && currentInput != ".") {
            currentInput.append(" * ");
            decimalClicked = false;
            inputLabel->setText(currentInput);
        }
    });

    QObject::connect(divButton, &QPushButton::clicked, [&currentInput, &decimalClicked, inputLabel]() {
        if (!currentInput.isEmpty() && currentInput != ".") {
            currentInput.append(" / ");
            decimalClicked = false;
            inputLabel->setText(currentInput);
        }
    });

    QObject::connect(buttons[11], &QPushButton::clicked, [&currentInput, &decimalClicked, &display, inputLabel]() {
        if (!currentInput.isEmpty() && currentInput != ".") {
            QStringList tokens = currentInput.split(' ', Qt::SkipEmptyParts);
            double result = tokens.first().toDouble();
            for (int i = 1; i < tokens.size(); i += 2) {
                QString op = tokens.at(i);
                double num = tokens.at(i + 1).toDouble();
                if (op == "+")
                    result += num;
                else if (op == "-")
                    result -= num;
                else if (op == "*")
                    result *= num;
                else if (op == "/") {
                    if (num != 0.0)
                        result /= num;
                    else {
                        display->setText("Error: Cannot divide by zero!");
                        inputLabel->setText("");
                        return;
                    }
                }
            }
            display->setText(QString::number(result));
            inputLabel->setText(currentInput + " =");
            currentInput.clear();
            decimalClicked = false;
        }
    });

    QObject::connect(clearButton, &QPushButton::clicked, [&currentInput, &decimalClicked, &display, inputLabel]() {
        currentInput.clear();
        decimalClicked = false;
        display->setText("0");
        inputLabel->setText("");
    });

    // Show the window
    window.show();

    return app.exec();
}

