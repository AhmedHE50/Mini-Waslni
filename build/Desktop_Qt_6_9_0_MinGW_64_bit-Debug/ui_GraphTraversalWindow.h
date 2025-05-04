/********************************************************************************
** Form generated from reading UI file 'GraphTraversalWindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GRAPHTRAVERSALWINDOW_H
#define UI_GRAPHTRAVERSALWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_GraphTraversalWindow
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *traversalGroup;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QComboBox *startCityComboBox;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *btnBFS;
    QPushButton *btnDFS;
    QGroupBox *resultsGroup;
    QVBoxLayout *verticalLayout_3;
    QTextEdit *traversalResultsTextEdit;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *btnClose;

    void setupUi(QDialog *GraphTraversalWindow)
    {
        if (GraphTraversalWindow->objectName().isEmpty())
            GraphTraversalWindow->setObjectName("GraphTraversalWindow");
        GraphTraversalWindow->resize(600, 500);
        verticalLayout = new QVBoxLayout(GraphTraversalWindow);
        verticalLayout->setObjectName("verticalLayout");
        traversalGroup = new QGroupBox(GraphTraversalWindow);
        traversalGroup->setObjectName("traversalGroup");
        verticalLayout_2 = new QVBoxLayout(traversalGroup);
        verticalLayout_2->setObjectName("verticalLayout_2");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        label = new QLabel(traversalGroup);
        label->setObjectName("label");

        horizontalLayout->addWidget(label);

        startCityComboBox = new QComboBox(traversalGroup);
        startCityComboBox->setObjectName("startCityComboBox");

        horizontalLayout->addWidget(startCityComboBox);


        verticalLayout_2->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        btnBFS = new QPushButton(traversalGroup);
        btnBFS->setObjectName("btnBFS");

        horizontalLayout_2->addWidget(btnBFS);

        btnDFS = new QPushButton(traversalGroup);
        btnDFS->setObjectName("btnDFS");

        horizontalLayout_2->addWidget(btnDFS);


        verticalLayout_2->addLayout(horizontalLayout_2);


        verticalLayout->addWidget(traversalGroup);

        resultsGroup = new QGroupBox(GraphTraversalWindow);
        resultsGroup->setObjectName("resultsGroup");
        verticalLayout_3 = new QVBoxLayout(resultsGroup);
        verticalLayout_3->setObjectName("verticalLayout_3");
        traversalResultsTextEdit = new QTextEdit(resultsGroup);
        traversalResultsTextEdit->setObjectName("traversalResultsTextEdit");
        traversalResultsTextEdit->setReadOnly(true);

        verticalLayout_3->addWidget(traversalResultsTextEdit);


        verticalLayout->addWidget(resultsGroup);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_2);

        btnClose = new QPushButton(GraphTraversalWindow);
        btnClose->setObjectName("btnClose");

        horizontalLayout_4->addWidget(btnClose);


        verticalLayout->addLayout(horizontalLayout_4);


        retranslateUi(GraphTraversalWindow);
        QObject::connect(btnClose, &QPushButton::clicked, GraphTraversalWindow, qOverload<>(&QDialog::close));

        QMetaObject::connectSlotsByName(GraphTraversalWindow);
    } // setupUi

    void retranslateUi(QDialog *GraphTraversalWindow)
    {
        GraphTraversalWindow->setWindowTitle(QCoreApplication::translate("GraphTraversalWindow", "Graph Traversal", nullptr));
        traversalGroup->setTitle(QCoreApplication::translate("GraphTraversalWindow", "Graph Traversal", nullptr));
        label->setText(QCoreApplication::translate("GraphTraversalWindow", "Start City:", nullptr));
        btnBFS->setText(QCoreApplication::translate("GraphTraversalWindow", "Breadth-First Search (BFS)", nullptr));
        btnDFS->setText(QCoreApplication::translate("GraphTraversalWindow", "Depth-First Search (DFS)", nullptr));
        resultsGroup->setTitle(QCoreApplication::translate("GraphTraversalWindow", "Traversal Results", nullptr));
        btnClose->setText(QCoreApplication::translate("GraphTraversalWindow", "Close", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GraphTraversalWindow: public Ui_GraphTraversalWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GRAPHTRAVERSALWINDOW_H
