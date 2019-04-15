/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "../gui/CustomOpenGLWidget.h"
#include "../gui/custom_widgets/ColorChooser.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionNew;
    QAction *actionOpen;
    QAction *actionSave;
    QAction *actionSave_as;
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    CustomOpenGLWidget *mapWidget;
    QTabWidget *tabWidget;
    QWidget *tabPathfinder;
    QVBoxLayout *verticalLayout;
    QFrame *algorithm_frame;
    QGridLayout *gridLayout_5;
    QLabel *algorithm_label;
    QPushButton *search_button;
    QComboBox *algorithm_chooser;
    QCheckBox *bidirectional_checkbox;
    QCheckBox *parallel_checkbox;
    QFrame *result_frame;
    QGridLayout *gridLayout_6;
    QLabel *time_label;
    QLabel *time_result_label;
    QLabel *length_label;
    QLabel *length_result_label;
    QLabel *result_label;
    QPushButton *drop_result_button;
    QFrame *res_hl_frame;
    QGridLayout *gridLayout_7;
    QCheckBox *hlFSACheckBox;
    QCheckBox *hlFSPCheckBox;
    QLabel *res_highlight_label;
    QCheckBox *hlRSPCheckBox;
    ColorChooser *rsaColorChooser;
    QCheckBox *hlPathCheckBox;
    ColorChooser *fsaColorChooser;
    QCheckBox *hlRSACheckBox;
    ColorChooser *fspColorChooser;
    ColorChooser *rspColorChooser;
    ColorChooser *pathColorChooser;
    ColorChooser *caColorChooser;
    QSpacerItem *verticalSpacer;
    QWidget *tabEditor;
    QGridLayout *gridLayout;
    QFrame *edges_editor_frame;
    QGridLayout *gridLayout_3;
    QPushButton *rm_sel_e_button;
    QPushButton *drop_sel_e_button;
    QLabel *edges_label;
    ColorChooser *ch_sel_e_color_chooser;
    ColorChooser *ch_e_color_chooser;
    QSpacerItem *verticalSpacer_3;
    QFrame *common_frame;
    QGridLayout *gridLayout_4;
    QLabel *common_label;
    QPushButton *rm_sel_button;
    QPushButton *drop_sel_button;
    QFrame *vertices_editor_frame;
    QGridLayout *gridLayout_2;
    QRadioButton *hl_v_radio;
    QPushButton *link_seq_button;
    QLabel *vertices_label;
    QPushButton *drop_sel_v_button;
    QPushButton *rm_sel_v_button;
    QPushButton *link_all_button;
    ColorChooser *ch_sel_v_color_chooser;
    ColorChooser *ch_v_color_chooser;
    QWidget *tabSettings;
    QMenuBar *menubar;
    QMenu *menuHelp;
    QMenu *menuFile;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->setWindowModality(Qt::ApplicationModal);
        MainWindow->resize(1248, 713);
        MainWindow->setMaximumSize(QSize(1920, 1024));
        actionNew = new QAction(MainWindow);
        actionNew->setObjectName(QStringLiteral("actionNew"));
        actionOpen = new QAction(MainWindow);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        actionSave = new QAction(MainWindow);
        actionSave->setObjectName(QStringLiteral("actionSave"));
        actionSave_as = new QAction(MainWindow);
        actionSave_as->setObjectName(QStringLiteral("actionSave_as"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(centralwidget->sizePolicy().hasHeightForWidth());
        centralwidget->setSizePolicy(sizePolicy);
        centralwidget->setToolTipDuration(-1);
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        mapWidget = new CustomOpenGLWidget(centralwidget);
        mapWidget->setObjectName(QStringLiteral("mapWidget"));
        mapWidget->setEnabled(true);
        sizePolicy.setHeightForWidth(mapWidget->sizePolicy().hasHeightForWidth());
        mapWidget->setSizePolicy(sizePolicy);
        mapWidget->setBaseSize(QSize(0, 0));
        mapWidget->setFocusPolicy(Qt::StrongFocus);
        mapWidget->setContextMenuPolicy(Qt::DefaultContextMenu);
        mapWidget->setAcceptDrops(false);
        mapWidget->setToolTipDuration(-1);

        horizontalLayout->addWidget(mapWidget);

        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(tabWidget->sizePolicy().hasHeightForWidth());
        tabWidget->setSizePolicy(sizePolicy1);
        tabWidget->setFocusPolicy(Qt::NoFocus);
        tabPathfinder = new QWidget();
        tabPathfinder->setObjectName(QStringLiteral("tabPathfinder"));
        verticalLayout = new QVBoxLayout(tabPathfinder);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        algorithm_frame = new QFrame(tabPathfinder);
        algorithm_frame->setObjectName(QStringLiteral("algorithm_frame"));
        algorithm_frame->setFrameShape(QFrame::StyledPanel);
        algorithm_frame->setFrameShadow(QFrame::Plain);
        gridLayout_5 = new QGridLayout(algorithm_frame);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        algorithm_label = new QLabel(algorithm_frame);
        algorithm_label->setObjectName(QStringLiteral("algorithm_label"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(algorithm_label->sizePolicy().hasHeightForWidth());
        algorithm_label->setSizePolicy(sizePolicy2);
        QFont font;
        font.setPointSize(14);
        algorithm_label->setFont(font);
        algorithm_label->setAlignment(Qt::AlignCenter);

        gridLayout_5->addWidget(algorithm_label, 0, 0, 1, 2);

        search_button = new QPushButton(algorithm_frame);
        search_button->setObjectName(QStringLiteral("search_button"));
        QSizePolicy sizePolicy3(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(search_button->sizePolicy().hasHeightForWidth());
        search_button->setSizePolicy(sizePolicy3);
        search_button->setFocusPolicy(Qt::NoFocus);

        gridLayout_5->addWidget(search_button, 4, 0, 1, 1);

        algorithm_chooser = new QComboBox(algorithm_frame);
        algorithm_chooser->setObjectName(QStringLiteral("algorithm_chooser"));
        algorithm_chooser->setFocusPolicy(Qt::NoFocus);

        gridLayout_5->addWidget(algorithm_chooser, 1, 0, 1, 2);

        bidirectional_checkbox = new QCheckBox(algorithm_frame);
        bidirectional_checkbox->setObjectName(QStringLiteral("bidirectional_checkbox"));
        sizePolicy3.setHeightForWidth(bidirectional_checkbox->sizePolicy().hasHeightForWidth());
        bidirectional_checkbox->setSizePolicy(sizePolicy3);
        bidirectional_checkbox->setFocusPolicy(Qt::NoFocus);

        gridLayout_5->addWidget(bidirectional_checkbox, 2, 0, 1, 1);

        parallel_checkbox = new QCheckBox(algorithm_frame);
        parallel_checkbox->setObjectName(QStringLiteral("parallel_checkbox"));
        sizePolicy3.setHeightForWidth(parallel_checkbox->sizePolicy().hasHeightForWidth());
        parallel_checkbox->setSizePolicy(sizePolicy3);
        parallel_checkbox->setFocusPolicy(Qt::NoFocus);

        gridLayout_5->addWidget(parallel_checkbox, 3, 0, 1, 1);


        verticalLayout->addWidget(algorithm_frame);

        result_frame = new QFrame(tabPathfinder);
        result_frame->setObjectName(QStringLiteral("result_frame"));
        result_frame->setFrameShape(QFrame::StyledPanel);
        gridLayout_6 = new QGridLayout(result_frame);
        gridLayout_6->setObjectName(QStringLiteral("gridLayout_6"));
        time_label = new QLabel(result_frame);
        time_label->setObjectName(QStringLiteral("time_label"));

        gridLayout_6->addWidget(time_label, 1, 0, 1, 1);

        time_result_label = new QLabel(result_frame);
        time_result_label->setObjectName(QStringLiteral("time_result_label"));

        gridLayout_6->addWidget(time_result_label, 1, 1, 1, 1);

        length_label = new QLabel(result_frame);
        length_label->setObjectName(QStringLiteral("length_label"));

        gridLayout_6->addWidget(length_label, 2, 0, 1, 1);

        length_result_label = new QLabel(result_frame);
        length_result_label->setObjectName(QStringLiteral("length_result_label"));
        sizePolicy.setHeightForWidth(length_result_label->sizePolicy().hasHeightForWidth());
        length_result_label->setSizePolicy(sizePolicy);

        gridLayout_6->addWidget(length_result_label, 2, 1, 1, 1);

        result_label = new QLabel(result_frame);
        result_label->setObjectName(QStringLiteral("result_label"));
        result_label->setFont(font);
        result_label->setAlignment(Qt::AlignCenter);

        gridLayout_6->addWidget(result_label, 0, 0, 1, 2);

        drop_result_button = new QPushButton(result_frame);
        drop_result_button->setObjectName(QStringLiteral("drop_result_button"));
        drop_result_button->setFocusPolicy(Qt::NoFocus);

        gridLayout_6->addWidget(drop_result_button, 3, 0, 1, 2);


        verticalLayout->addWidget(result_frame);

        res_hl_frame = new QFrame(tabPathfinder);
        res_hl_frame->setObjectName(QStringLiteral("res_hl_frame"));
        res_hl_frame->setFrameShape(QFrame::StyledPanel);
        gridLayout_7 = new QGridLayout(res_hl_frame);
        gridLayout_7->setObjectName(QStringLiteral("gridLayout_7"));
        hlFSACheckBox = new QCheckBox(res_hl_frame);
        hlFSACheckBox->setObjectName(QStringLiteral("hlFSACheckBox"));
        hlFSACheckBox->setFocusPolicy(Qt::NoFocus);

        gridLayout_7->addWidget(hlFSACheckBox, 5, 0, 1, 1);

        hlFSPCheckBox = new QCheckBox(res_hl_frame);
        hlFSPCheckBox->setObjectName(QStringLiteral("hlFSPCheckBox"));
        sizePolicy2.setHeightForWidth(hlFSPCheckBox->sizePolicy().hasHeightForWidth());
        hlFSPCheckBox->setSizePolicy(sizePolicy2);
        hlFSPCheckBox->setFocusPolicy(Qt::NoFocus);

        gridLayout_7->addWidget(hlFSPCheckBox, 2, 0, 1, 1);

        res_highlight_label = new QLabel(res_hl_frame);
        res_highlight_label->setObjectName(QStringLiteral("res_highlight_label"));
        res_highlight_label->setFont(font);
        res_highlight_label->setAlignment(Qt::AlignCenter);

        gridLayout_7->addWidget(res_highlight_label, 0, 0, 1, 2);

        hlRSPCheckBox = new QCheckBox(res_hl_frame);
        hlRSPCheckBox->setObjectName(QStringLiteral("hlRSPCheckBox"));
        hlRSPCheckBox->setFocusPolicy(Qt::NoFocus);

        gridLayout_7->addWidget(hlRSPCheckBox, 4, 0, 1, 1);

        rsaColorChooser = new ColorChooser(res_hl_frame);
        rsaColorChooser->setObjectName(QStringLiteral("rsaColorChooser"));
        rsaColorChooser->setFrameShape(QFrame::NoFrame);
        rsaColorChooser->setFrameShadow(QFrame::Raised);

        gridLayout_7->addWidget(rsaColorChooser, 11, 0, 1, 2);

        hlPathCheckBox = new QCheckBox(res_hl_frame);
        hlPathCheckBox->setObjectName(QStringLiteral("hlPathCheckBox"));
        hlPathCheckBox->setFocusPolicy(Qt::NoFocus);

        gridLayout_7->addWidget(hlPathCheckBox, 1, 0, 1, 1);

        fsaColorChooser = new ColorChooser(res_hl_frame);
        fsaColorChooser->setObjectName(QStringLiteral("fsaColorChooser"));
        fsaColorChooser->setFrameShape(QFrame::NoFrame);
        fsaColorChooser->setFrameShadow(QFrame::Raised);

        gridLayout_7->addWidget(fsaColorChooser, 10, 0, 1, 2);

        hlRSACheckBox = new QCheckBox(res_hl_frame);
        hlRSACheckBox->setObjectName(QStringLiteral("hlRSACheckBox"));
        hlRSACheckBox->setFocusPolicy(Qt::NoFocus);

        gridLayout_7->addWidget(hlRSACheckBox, 6, 0, 1, 1);

        fspColorChooser = new ColorChooser(res_hl_frame);
        fspColorChooser->setObjectName(QStringLiteral("fspColorChooser"));
        fspColorChooser->setFrameShape(QFrame::NoFrame);
        fspColorChooser->setFrameShadow(QFrame::Raised);

        gridLayout_7->addWidget(fspColorChooser, 8, 0, 1, 2);

        rspColorChooser = new ColorChooser(res_hl_frame);
        rspColorChooser->setObjectName(QStringLiteral("rspColorChooser"));
        rspColorChooser->setFrameShape(QFrame::NoFrame);
        rspColorChooser->setFrameShadow(QFrame::Raised);

        gridLayout_7->addWidget(rspColorChooser, 9, 0, 1, 2);

        pathColorChooser = new ColorChooser(res_hl_frame);
        pathColorChooser->setObjectName(QStringLiteral("pathColorChooser"));
        pathColorChooser->setFrameShape(QFrame::NoFrame);
        pathColorChooser->setFrameShadow(QFrame::Raised);

        gridLayout_7->addWidget(pathColorChooser, 7, 0, 1, 2);

        caColorChooser = new ColorChooser(res_hl_frame);
        caColorChooser->setObjectName(QStringLiteral("caColorChooser"));
        caColorChooser->setFrameShape(QFrame::NoFrame);
        caColorChooser->setFrameShadow(QFrame::Raised);

        gridLayout_7->addWidget(caColorChooser, 12, 0, 1, 2);


        verticalLayout->addWidget(res_hl_frame);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        tabWidget->addTab(tabPathfinder, QString());
        tabEditor = new QWidget();
        tabEditor->setObjectName(QStringLiteral("tabEditor"));
        gridLayout = new QGridLayout(tabEditor);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        edges_editor_frame = new QFrame(tabEditor);
        edges_editor_frame->setObjectName(QStringLiteral("edges_editor_frame"));
        edges_editor_frame->setFrameShape(QFrame::StyledPanel);
        gridLayout_3 = new QGridLayout(edges_editor_frame);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        rm_sel_e_button = new QPushButton(edges_editor_frame);
        rm_sel_e_button->setObjectName(QStringLiteral("rm_sel_e_button"));
        QSizePolicy sizePolicy4(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(rm_sel_e_button->sizePolicy().hasHeightForWidth());
        rm_sel_e_button->setSizePolicy(sizePolicy4);
        rm_sel_e_button->setFocusPolicy(Qt::NoFocus);

        gridLayout_3->addWidget(rm_sel_e_button, 2, 0, 1, 1);

        drop_sel_e_button = new QPushButton(edges_editor_frame);
        drop_sel_e_button->setObjectName(QStringLiteral("drop_sel_e_button"));
        sizePolicy.setHeightForWidth(drop_sel_e_button->sizePolicy().hasHeightForWidth());
        drop_sel_e_button->setSizePolicy(sizePolicy);
        drop_sel_e_button->setFocusPolicy(Qt::NoFocus);

        gridLayout_3->addWidget(drop_sel_e_button, 2, 1, 1, 1);

        edges_label = new QLabel(edges_editor_frame);
        edges_label->setObjectName(QStringLiteral("edges_label"));
        edges_label->setEnabled(true);
        sizePolicy.setHeightForWidth(edges_label->sizePolicy().hasHeightForWidth());
        edges_label->setSizePolicy(sizePolicy);
        edges_label->setMaximumSize(QSize(16777215, 20));
        QFont font1;
        font1.setFamily(QStringLiteral("Ubuntu"));
        font1.setPointSize(14);
        font1.setBold(false);
        font1.setItalic(false);
        font1.setUnderline(false);
        font1.setWeight(50);
        font1.setStrikeOut(false);
        font1.setKerning(true);
        edges_label->setFont(font1);
        edges_label->setMouseTracking(false);
        edges_label->setLayoutDirection(Qt::LeftToRight);
        edges_label->setFrameShadow(QFrame::Plain);
        edges_label->setTextFormat(Qt::PlainText);
        edges_label->setScaledContents(false);
        edges_label->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(edges_label, 1, 0, 1, 2);

        ch_sel_e_color_chooser = new ColorChooser(edges_editor_frame);
        ch_sel_e_color_chooser->setObjectName(QStringLiteral("ch_sel_e_color_chooser"));
        QSizePolicy sizePolicy5(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(ch_sel_e_color_chooser->sizePolicy().hasHeightForWidth());
        ch_sel_e_color_chooser->setSizePolicy(sizePolicy5);
        ch_sel_e_color_chooser->setFrameShape(QFrame::NoFrame);
        ch_sel_e_color_chooser->setFrameShadow(QFrame::Plain);

        gridLayout_3->addWidget(ch_sel_e_color_chooser, 4, 0, 1, 2);

        ch_e_color_chooser = new ColorChooser(edges_editor_frame);
        ch_e_color_chooser->setObjectName(QStringLiteral("ch_e_color_chooser"));
        ch_e_color_chooser->setFrameShape(QFrame::NoFrame);
        ch_e_color_chooser->setFrameShadow(QFrame::Raised);

        gridLayout_3->addWidget(ch_e_color_chooser, 3, 0, 1, 2);


        gridLayout->addWidget(edges_editor_frame, 1, 4, 1, 1);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer_3, 3, 4, 1, 1);

        common_frame = new QFrame(tabEditor);
        common_frame->setObjectName(QStringLiteral("common_frame"));
        QSizePolicy sizePolicy6(QSizePolicy::Preferred, QSizePolicy::Minimum);
        sizePolicy6.setHorizontalStretch(0);
        sizePolicy6.setVerticalStretch(0);
        sizePolicy6.setHeightForWidth(common_frame->sizePolicy().hasHeightForWidth());
        common_frame->setSizePolicy(sizePolicy6);
        common_frame->setFrameShape(QFrame::StyledPanel);
        common_frame->setFrameShadow(QFrame::Plain);
        gridLayout_4 = new QGridLayout(common_frame);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        common_label = new QLabel(common_frame);
        common_label->setObjectName(QStringLiteral("common_label"));
        common_label->setEnabled(true);
        sizePolicy6.setHeightForWidth(common_label->sizePolicy().hasHeightForWidth());
        common_label->setSizePolicy(sizePolicy6);
        common_label->setMaximumSize(QSize(16777215, 20));
        common_label->setFont(font1);
        common_label->setMouseTracking(false);
        common_label->setLayoutDirection(Qt::LeftToRight);
        common_label->setFrameShadow(QFrame::Plain);
        common_label->setTextFormat(Qt::PlainText);
        common_label->setScaledContents(false);
        common_label->setAlignment(Qt::AlignCenter);

        gridLayout_4->addWidget(common_label, 0, 0, 1, 2);

        rm_sel_button = new QPushButton(common_frame);
        rm_sel_button->setObjectName(QStringLiteral("rm_sel_button"));
        rm_sel_button->setFocusPolicy(Qt::NoFocus);

        gridLayout_4->addWidget(rm_sel_button, 1, 0, 1, 1);

        drop_sel_button = new QPushButton(common_frame);
        drop_sel_button->setObjectName(QStringLiteral("drop_sel_button"));
        drop_sel_button->setFocusPolicy(Qt::NoFocus);

        gridLayout_4->addWidget(drop_sel_button, 1, 1, 1, 1);


        gridLayout->addWidget(common_frame, 2, 4, 1, 1);

        vertices_editor_frame = new QFrame(tabEditor);
        vertices_editor_frame->setObjectName(QStringLiteral("vertices_editor_frame"));
        sizePolicy6.setHeightForWidth(vertices_editor_frame->sizePolicy().hasHeightForWidth());
        vertices_editor_frame->setSizePolicy(sizePolicy6);
        vertices_editor_frame->setFrameShape(QFrame::StyledPanel);
        gridLayout_2 = new QGridLayout(vertices_editor_frame);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        hl_v_radio = new QRadioButton(vertices_editor_frame);
        hl_v_radio->setObjectName(QStringLiteral("hl_v_radio"));
        QSizePolicy sizePolicy7(QSizePolicy::Fixed, QSizePolicy::Maximum);
        sizePolicy7.setHorizontalStretch(0);
        sizePolicy7.setVerticalStretch(0);
        sizePolicy7.setHeightForWidth(hl_v_radio->sizePolicy().hasHeightForWidth());
        hl_v_radio->setSizePolicy(sizePolicy7);
        hl_v_radio->setFocusPolicy(Qt::NoFocus);
        hl_v_radio->setLayoutDirection(Qt::LeftToRight);
        hl_v_radio->setAutoFillBackground(false);

        gridLayout_2->addWidget(hl_v_radio, 1, 0, 1, 2);

        link_seq_button = new QPushButton(vertices_editor_frame);
        link_seq_button->setObjectName(QStringLiteral("link_seq_button"));
        sizePolicy3.setHeightForWidth(link_seq_button->sizePolicy().hasHeightForWidth());
        link_seq_button->setSizePolicy(sizePolicy3);
        link_seq_button->setFocusPolicy(Qt::NoFocus);
        link_seq_button->setToolTipDuration(-1);

        gridLayout_2->addWidget(link_seq_button, 2, 0, 1, 1);

        vertices_label = new QLabel(vertices_editor_frame);
        vertices_label->setObjectName(QStringLiteral("vertices_label"));
        vertices_label->setEnabled(true);
        QSizePolicy sizePolicy8(QSizePolicy::Expanding, QSizePolicy::Minimum);
        sizePolicy8.setHorizontalStretch(0);
        sizePolicy8.setVerticalStretch(0);
        sizePolicy8.setHeightForWidth(vertices_label->sizePolicy().hasHeightForWidth());
        vertices_label->setSizePolicy(sizePolicy8);
        vertices_label->setMaximumSize(QSize(16777215, 20));
        vertices_label->setFont(font1);
        vertices_label->setMouseTracking(false);
        vertices_label->setLayoutDirection(Qt::LeftToRight);
        vertices_label->setFrameShape(QFrame::NoFrame);
        vertices_label->setFrameShadow(QFrame::Plain);
        vertices_label->setTextFormat(Qt::PlainText);
        vertices_label->setScaledContents(false);
        vertices_label->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(vertices_label, 0, 0, 1, 2);

        drop_sel_v_button = new QPushButton(vertices_editor_frame);
        drop_sel_v_button->setObjectName(QStringLiteral("drop_sel_v_button"));
        QSizePolicy sizePolicy9(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy9.setHorizontalStretch(0);
        sizePolicy9.setVerticalStretch(0);
        sizePolicy9.setHeightForWidth(drop_sel_v_button->sizePolicy().hasHeightForWidth());
        drop_sel_v_button->setSizePolicy(sizePolicy9);
        drop_sel_v_button->setFocusPolicy(Qt::NoFocus);

        gridLayout_2->addWidget(drop_sel_v_button, 3, 1, 1, 1);

        rm_sel_v_button = new QPushButton(vertices_editor_frame);
        rm_sel_v_button->setObjectName(QStringLiteral("rm_sel_v_button"));
        sizePolicy3.setHeightForWidth(rm_sel_v_button->sizePolicy().hasHeightForWidth());
        rm_sel_v_button->setSizePolicy(sizePolicy3);
        rm_sel_v_button->setFocusPolicy(Qt::NoFocus);

        gridLayout_2->addWidget(rm_sel_v_button, 3, 0, 1, 1);

        link_all_button = new QPushButton(vertices_editor_frame);
        link_all_button->setObjectName(QStringLiteral("link_all_button"));
        sizePolicy3.setHeightForWidth(link_all_button->sizePolicy().hasHeightForWidth());
        link_all_button->setSizePolicy(sizePolicy3);
        link_all_button->setFocusPolicy(Qt::NoFocus);

        gridLayout_2->addWidget(link_all_button, 2, 1, 1, 1);

        ch_sel_v_color_chooser = new ColorChooser(vertices_editor_frame);
        ch_sel_v_color_chooser->setObjectName(QStringLiteral("ch_sel_v_color_chooser"));
        ch_sel_v_color_chooser->setFrameShape(QFrame::NoFrame);
        ch_sel_v_color_chooser->setFrameShadow(QFrame::Raised);

        gridLayout_2->addWidget(ch_sel_v_color_chooser, 5, 0, 1, 2);

        ch_v_color_chooser = new ColorChooser(vertices_editor_frame);
        ch_v_color_chooser->setObjectName(QStringLiteral("ch_v_color_chooser"));
        ch_v_color_chooser->setFrameShape(QFrame::NoFrame);
        ch_v_color_chooser->setFrameShadow(QFrame::Raised);

        gridLayout_2->addWidget(ch_v_color_chooser, 4, 0, 1, 2);


        gridLayout->addWidget(vertices_editor_frame, 0, 4, 1, 1);

        tabWidget->addTab(tabEditor, QString());
        tabSettings = new QWidget();
        tabSettings->setObjectName(QStringLiteral("tabSettings"));
        tabWidget->addTab(tabSettings, QString());

        horizontalLayout->addWidget(tabWidget);

        horizontalLayout->setStretch(0, 3);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 1248, 22));
        menuHelp = new QMenu(menubar);
        menuHelp->setObjectName(QStringLiteral("menuHelp"));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        MainWindow->setMenuBar(menubar);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionNew);
        menuFile->addAction(actionOpen);
        menuFile->addSeparator();
        menuFile->addAction(actionSave);
        menuFile->addAction(actionSave_as);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        actionNew->setText(QApplication::translate("MainWindow", "New...", Q_NULLPTR));
        actionOpen->setText(QApplication::translate("MainWindow", "Open...", Q_NULLPTR));
        actionSave->setText(QApplication::translate("MainWindow", "Save", Q_NULLPTR));
        actionSave_as->setText(QApplication::translate("MainWindow", "Save as...", Q_NULLPTR));
        algorithm_label->setText(QApplication::translate("MainWindow", "Algorithm", Q_NULLPTR));
        search_button->setText(QApplication::translate("MainWindow", "Search", Q_NULLPTR));
        algorithm_chooser->clear();
        algorithm_chooser->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Dijkstra", Q_NULLPTR)
         << QApplication::translate("MainWindow", "Dijkstra with stop", Q_NULLPTR)
         << QApplication::translate("MainWindow", "A* Algorithm", Q_NULLPTR)
        );
        bidirectional_checkbox->setText(QApplication::translate("MainWindow", "Bidirectional", Q_NULLPTR));
        parallel_checkbox->setText(QApplication::translate("MainWindow", "Parallel", Q_NULLPTR));
        time_label->setText(QApplication::translate("MainWindow", "Time:", Q_NULLPTR));
        time_result_label->setText(QApplication::translate("MainWindow", "-", Q_NULLPTR));
        length_label->setText(QApplication::translate("MainWindow", "Lenght:", Q_NULLPTR));
        length_result_label->setText(QApplication::translate("MainWindow", "-", Q_NULLPTR));
        result_label->setText(QApplication::translate("MainWindow", "Result", Q_NULLPTR));
        drop_result_button->setText(QApplication::translate("MainWindow", "Drop current result", Q_NULLPTR));
        hlFSACheckBox->setText(QApplication::translate("MainWindow", "Forward Search Area", Q_NULLPTR));
        hlFSPCheckBox->setText(QApplication::translate("MainWindow", "Forward Search Part of Path", Q_NULLPTR));
        res_highlight_label->setText(QApplication::translate("MainWindow", "Results' Highlight", Q_NULLPTR));
        hlRSPCheckBox->setText(QApplication::translate("MainWindow", "Reverse Search Part of Path", Q_NULLPTR));
        hlPathCheckBox->setText(QApplication::translate("MainWindow", "Path", Q_NULLPTR));
        hlRSACheckBox->setText(QApplication::translate("MainWindow", "Reverse Search Area", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tabPathfinder), QApplication::translate("MainWindow", "Pathfinder", Q_NULLPTR));
        rm_sel_e_button->setText(QApplication::translate("MainWindow", "Remove selected", Q_NULLPTR));
        drop_sel_e_button->setText(QApplication::translate("MainWindow", "Drop selections", Q_NULLPTR));
        edges_label->setText(QApplication::translate("MainWindow", "Edges", Q_NULLPTR));
        common_label->setText(QApplication::translate("MainWindow", "Common", Q_NULLPTR));
        rm_sel_button->setText(QApplication::translate("MainWindow", "Remove selected", Q_NULLPTR));
        drop_sel_button->setText(QApplication::translate("MainWindow", "Drop selections", Q_NULLPTR));
        hl_v_radio->setText(QApplication::translate("MainWindow", "Highlight (H)", Q_NULLPTR));
        link_seq_button->setText(QApplication::translate("MainWindow", "Link sequently", Q_NULLPTR));
        vertices_label->setText(QApplication::translate("MainWindow", "Vertices", Q_NULLPTR));
        drop_sel_v_button->setText(QApplication::translate("MainWindow", "Drop selections", Q_NULLPTR));
        rm_sel_v_button->setText(QApplication::translate("MainWindow", "Remove selected", Q_NULLPTR));
        link_all_button->setText(QApplication::translate("MainWindow", "Link all together", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tabEditor), QApplication::translate("MainWindow", "Editor", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tabSettings), QApplication::translate("MainWindow", "Settings", Q_NULLPTR));
        menuHelp->setTitle(QApplication::translate("MainWindow", "Help", Q_NULLPTR));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
