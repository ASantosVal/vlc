/********************************************************************************
** Form generated from reading UI file 'ModuleInterfaceProtopype.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef MODULEINTERFACEPROTOPYPE_H
#define MODULEINTERFACEPROTOPYPE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QScrollBar>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QGridLayout *gridLayout;
    QGroupBox *GroupBox_MetadataEditor;
    QHBoxLayout *horizontalLayout_2;
    QTableWidget *tableWidget;
    QScrollBar *verticalScrollBar;
    QGroupBox *GroupBox_FileSelection;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QGroupBox *GroupBox_SetButtons;
    QHBoxLayout *horizontalLayout_5;
    QPushButton *pushButton_6;
    QPushButton *pushButton_7;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton_8;
    QGroupBox *GroupBox_WhatIsThis;
    QVBoxLayout *verticalLayout_2;
    QPushButton *pushButton_4;
    QPushButton *pushButton_5;
    QSpacerItem *verticalSpacer;
    QGroupBox *GroupBox_Artwork;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label;
    QGroupBox *GroupBox_OnlineSearch;
    QGridLayout *gridLayout_2;
    QGroupBox *GroupBox_SearchMethods;
    QVBoxLayout *verticalLayout;
    QRadioButton *radioButton;
    QRadioButton *radioButton_4;
    QRadioButton *radioButton_3;
    QRadioButton *radioButton_2;
    QProgressBar *progressBar;
    QPushButton *pushButton_3;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QStringLiteral("Dialog"));
        Dialog->resize(1215, 765);
        gridLayout = new QGridLayout(Dialog);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        GroupBox_MetadataEditor = new QGroupBox(Dialog);
        GroupBox_MetadataEditor->setObjectName(QStringLiteral("GroupBox_MetadataEditor"));
        horizontalLayout_2 = new QHBoxLayout(GroupBox_MetadataEditor);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        tableWidget = new QTableWidget(GroupBox_MetadataEditor);
        if (tableWidget->columnCount() < 10)
            tableWidget->setColumnCount(10);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(6, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(7, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(8, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(9, __qtablewidgetitem9);
        tableWidget->setObjectName(QStringLiteral("tableWidget"));

        horizontalLayout_2->addWidget(tableWidget);

        verticalScrollBar = new QScrollBar(GroupBox_MetadataEditor);
        verticalScrollBar->setObjectName(QStringLiteral("verticalScrollBar"));
        verticalScrollBar->setOrientation(Qt::Vertical);

        horizontalLayout_2->addWidget(verticalScrollBar);


        gridLayout->addWidget(GroupBox_MetadataEditor, 2, 0, 1, 4);

        GroupBox_FileSelection = new QGroupBox(Dialog);
        GroupBox_FileSelection->setObjectName(QStringLiteral("GroupBox_FileSelection"));
        horizontalLayout = new QHBoxLayout(GroupBox_FileSelection);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        pushButton = new QPushButton(GroupBox_FileSelection);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        horizontalLayout->addWidget(pushButton);

        pushButton_2 = new QPushButton(GroupBox_FileSelection);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));

        horizontalLayout->addWidget(pushButton_2);


        gridLayout->addWidget(GroupBox_FileSelection, 0, 0, 1, 2);

        GroupBox_SetButtons = new QGroupBox(Dialog);
        GroupBox_SetButtons->setObjectName(QStringLiteral("GroupBox_SetButtons"));
        horizontalLayout_5 = new QHBoxLayout(GroupBox_SetButtons);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        pushButton_6 = new QPushButton(GroupBox_SetButtons);
        pushButton_6->setObjectName(QStringLiteral("pushButton_6"));

        horizontalLayout_5->addWidget(pushButton_6);

        pushButton_7 = new QPushButton(GroupBox_SetButtons);
        pushButton_7->setObjectName(QStringLiteral("pushButton_7"));

        horizontalLayout_5->addWidget(pushButton_7);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer);

        pushButton_8 = new QPushButton(GroupBox_SetButtons);
        pushButton_8->setObjectName(QStringLiteral("pushButton_8"));

        horizontalLayout_5->addWidget(pushButton_8);


        gridLayout->addWidget(GroupBox_SetButtons, 3, 0, 1, 4);

        GroupBox_WhatIsThis = new QGroupBox(Dialog);
        GroupBox_WhatIsThis->setObjectName(QStringLiteral("GroupBox_WhatIsThis"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(GroupBox_WhatIsThis->sizePolicy().hasHeightForWidth());
        GroupBox_WhatIsThis->setSizePolicy(sizePolicy);
        verticalLayout_2 = new QVBoxLayout(GroupBox_WhatIsThis);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        pushButton_4 = new QPushButton(GroupBox_WhatIsThis);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(pushButton_4->sizePolicy().hasHeightForWidth());
        pushButton_4->setSizePolicy(sizePolicy1);

        verticalLayout_2->addWidget(pushButton_4);

        pushButton_5 = new QPushButton(GroupBox_WhatIsThis);
        pushButton_5->setObjectName(QStringLiteral("pushButton_5"));
        sizePolicy1.setHeightForWidth(pushButton_5->sizePolicy().hasHeightForWidth());
        pushButton_5->setSizePolicy(sizePolicy1);

        verticalLayout_2->addWidget(pushButton_5);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);


        gridLayout->addWidget(GroupBox_WhatIsThis, 0, 3, 2, 1);

        GroupBox_Artwork = new QGroupBox(Dialog);
        GroupBox_Artwork->setObjectName(QStringLiteral("GroupBox_Artwork"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(GroupBox_Artwork->sizePolicy().hasHeightForWidth());
        GroupBox_Artwork->setSizePolicy(sizePolicy2);
        horizontalLayout_4 = new QHBoxLayout(GroupBox_Artwork);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        label = new QLabel(GroupBox_Artwork);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout_4->addWidget(label);


        gridLayout->addWidget(GroupBox_Artwork, 0, 2, 2, 1);

        GroupBox_OnlineSearch = new QGroupBox(Dialog);
        GroupBox_OnlineSearch->setObjectName(QStringLiteral("GroupBox_OnlineSearch"));
        gridLayout_2 = new QGridLayout(GroupBox_OnlineSearch);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        GroupBox_SearchMethods = new QGroupBox(GroupBox_OnlineSearch);
        GroupBox_SearchMethods->setObjectName(QStringLiteral("GroupBox_SearchMethods"));
        sizePolicy2.setHeightForWidth(GroupBox_SearchMethods->sizePolicy().hasHeightForWidth());
        GroupBox_SearchMethods->setSizePolicy(sizePolicy2);
        verticalLayout = new QVBoxLayout(GroupBox_SearchMethods);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        radioButton = new QRadioButton(GroupBox_SearchMethods);
        radioButton->setObjectName(QStringLiteral("radioButton"));

        verticalLayout->addWidget(radioButton);

        radioButton_4 = new QRadioButton(GroupBox_SearchMethods);
        radioButton_4->setObjectName(QStringLiteral("radioButton_4"));

        verticalLayout->addWidget(radioButton_4);

        radioButton_3 = new QRadioButton(GroupBox_SearchMethods);
        radioButton_3->setObjectName(QStringLiteral("radioButton_3"));

        verticalLayout->addWidget(radioButton_3);

        radioButton_2 = new QRadioButton(GroupBox_SearchMethods);
        radioButton_2->setObjectName(QStringLiteral("radioButton_2"));

        verticalLayout->addWidget(radioButton_2);


        gridLayout_2->addWidget(GroupBox_SearchMethods, 0, 1, 2, 1);

        progressBar = new QProgressBar(GroupBox_OnlineSearch);
        progressBar->setObjectName(QStringLiteral("progressBar"));
        QSizePolicy sizePolicy3(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(progressBar->sizePolicy().hasHeightForWidth());
        progressBar->setSizePolicy(sizePolicy3);
        progressBar->setValue(24);

        gridLayout_2->addWidget(progressBar, 1, 0, 1, 1);

        pushButton_3 = new QPushButton(GroupBox_OnlineSearch);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));

        gridLayout_2->addWidget(pushButton_3, 0, 0, 1, 1);


        gridLayout->addWidget(GroupBox_OnlineSearch, 1, 0, 1, 2);


        retranslateUi(Dialog);

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QApplication::translate("Dialog", "Dialog", Q_NULLPTR));
        GroupBox_MetadataEditor->setTitle(QApplication::translate("Dialog", "Metadata Editor", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem->setText(QApplication::translate("Dialog", "Title", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem1->setText(QApplication::translate("Dialog", "Artist", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget->horizontalHeaderItem(3);
        ___qtablewidgetitem2->setText(QApplication::translate("Dialog", "Album", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget->horizontalHeaderItem(4);
        ___qtablewidgetitem3->setText(QApplication::translate("Dialog", "Genre", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidget->horizontalHeaderItem(5);
        ___qtablewidgetitem4->setText(QApplication::translate("Dialog", "New Column", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem5 = tableWidget->horizontalHeaderItem(6);
        ___qtablewidgetitem5->setText(QApplication::translate("Dialog", "Track #", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem6 = tableWidget->horizontalHeaderItem(7);
        ___qtablewidgetitem6->setText(QApplication::translate("Dialog", "Publisher", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem7 = tableWidget->horizontalHeaderItem(8);
        ___qtablewidgetitem7->setText(QApplication::translate("Dialog", "Copyright", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem8 = tableWidget->horizontalHeaderItem(9);
        ___qtablewidgetitem8->setText(QApplication::translate("Dialog", "Artwork", Q_NULLPTR));
        GroupBox_FileSelection->setTitle(QApplication::translate("Dialog", "File Selection", Q_NULLPTR));
        pushButton->setText(QApplication::translate("Dialog", "Get From Playlist", Q_NULLPTR));
        pushButton_2->setText(QApplication::translate("Dialog", "Get From Folder", Q_NULLPTR));
        GroupBox_SetButtons->setTitle(QString());
        pushButton_6->setText(QApplication::translate("Dialog", "Save All", Q_NULLPTR));
        pushButton_7->setText(QApplication::translate("Dialog", "Restore All", Q_NULLPTR));
        pushButton_8->setText(QApplication::translate("Dialog", "Cancel", Q_NULLPTR));
        GroupBox_WhatIsThis->setTitle(QApplication::translate("Dialog", "What is this?", Q_NULLPTR));
        pushButton_4->setText(QApplication::translate("Dialog", "Help", Q_NULLPTR));
        pushButton_5->setText(QApplication::translate("Dialog", "About", Q_NULLPTR));
        GroupBox_Artwork->setTitle(QApplication::translate("Dialog", "Artwork", Q_NULLPTR));
        label->setText(QApplication::translate("Dialog", "PixMap", Q_NULLPTR));
        GroupBox_OnlineSearch->setTitle(QApplication::translate("Dialog", "Online Search", Q_NULLPTR));
        GroupBox_SearchMethods->setTitle(QApplication::translate("Dialog", "Choose search methods:", Q_NULLPTR));
        radioButton->setText(QApplication::translate("Dialog", "RadioButton", Q_NULLPTR));
        radioButton_4->setText(QApplication::translate("Dialog", "RadioButton", Q_NULLPTR));
        radioButton_3->setText(QApplication::translate("Dialog", "RadioButton", Q_NULLPTR));
        radioButton_2->setText(QApplication::translate("Dialog", "RadioButton", Q_NULLPTR));
        pushButton_3->setText(QApplication::translate("Dialog", "PushButton", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // MODULEINTERFACEPROTOPYPE_H
