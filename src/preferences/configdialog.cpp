/************************************************************************
**
** Authors:   Ulf Hermann <ulfonk_mennhar@gmx.de> (Alve),
**            Marek Krejza <krejza@gmail.com> (Caligor),
**            Nils Schimmelmann <nschimme@gmail.com> (Jahara)
**
** This file is part of the MMapper project.
** Maintained by Nils Schimmelmann <nschimme@gmail.com>
**
** This program is free software; you can redistribute it and/or
** modify it under the terms of the GNU General Public License
** as published by the Free Software Foundation; either version 2
** of the License, or (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the:
** Free Software Foundation, Inc.
** 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
**
************************************************************************/

#include "configdialog.h"
#include "ui_configdialog.h"
#include "generalpage.h"
#include "parserpage.h"
#include "pathmachinepage.h"
#include "groupmanagerpage.h"
#include "clientpage.h"
#include "mumeprotocolpage.h"

#include <QListWidget>
#include <QStackedWidget>
#include <QScrollBar>

ConfigDialog::ConfigDialog(Mmapper2Group *gm, QWidget *parent)
    : QDialog(parent),
      ui(new Ui::ConfigDialog),
      m_groupManager(gm)
{
    ui->setupUi(this);

    setWindowTitle(tr("Config Dialog"));

    ui->contentsWidget->setViewMode(QListView::IconMode);
    ui->contentsWidget->setIconSize(QSize(70, 70));
    ui->contentsWidget->setMovement(QListView::Static);
    ui->contentsWidget->setMaximumWidth(110);
    ui->contentsWidget->setMinimumWidth(110);
    ui->contentsWidget->setSpacing(9);

    createIcons();

    pagesWidget = new QStackedWidget(this);
    pagesWidget->setMinimumWidth(500);
    pagesWidget->setMinimumHeight(500);
    pagesWidget->addWidget(new GeneralPage(this));
    pagesWidget->setCurrentIndex(0);

    ui->pagesScrollArea->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    ui->pagesScrollArea->setMinimumWidth(520);
    ui->pagesScrollArea->setWidget(pagesWidget);

    connect(ui->closeButton, SIGNAL(clicked()), this, SLOT(close()));
}

ConfigDialog::~ConfigDialog()
{
    delete ui;
}

void ConfigDialog::createIcons()
{
    QListWidgetItem *configButton = new QListWidgetItem(ui->contentsWidget);
    configButton->setIcon(QIcon(":/icons/generalcfg.png"));
    configButton->setText(tr("General"));
    configButton->setTextAlignment(Qt::AlignHCenter);
    configButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QListWidgetItem *updateButton = new QListWidgetItem(ui->contentsWidget);
    updateButton->setIcon(QIcon(":/icons/parsercfg.png"));
    updateButton->setText(tr("Parser"));
    updateButton->setTextAlignment(Qt::AlignHCenter);
    updateButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QListWidgetItem *clientButton = new QListWidgetItem(ui->contentsWidget);
    clientButton->setIcon(QIcon(":/icons/terminal.png"));
    clientButton->setText(tr("Integrated\nMud Client"));
    clientButton->setTextAlignment(Qt::AlignHCenter);
    clientButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QListWidgetItem *groupButton = new QListWidgetItem(ui->contentsWidget);
    groupButton->setIcon(QIcon(":/icons/groupcfg.png"));
    groupButton->setText(tr("Group\nManager"));
    groupButton->setTextAlignment(Qt::AlignHCenter);
    groupButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QListWidgetItem *mpiButton = new QListWidgetItem(ui->contentsWidget);
    mpiButton->setIcon(QIcon(":/icons/mumeprotocolcfg.png"));
    mpiButton->setText(tr("Mume\nProtocol"));
    mpiButton->setTextAlignment(Qt::AlignHCenter);
    mpiButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QListWidgetItem *pathButton = new QListWidgetItem(ui->contentsWidget);
    pathButton->setIcon(QIcon(":/icons/pathmachinecfg.png"));
    pathButton->setText(tr("Path\nMachine"));
    pathButton->setTextAlignment(Qt::AlignHCenter);
    pathButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    connect(ui->contentsWidget,
            SIGNAL(currentItemChanged(QListWidgetItem *, QListWidgetItem *)),
            this, SLOT(changePage(QListWidgetItem *, QListWidgetItem *)));
}

void ConfigDialog::changePage(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (!current)
        current = previous;

    if (pagesWidget->count() <= 1) {
        pagesWidget->addWidget(new ParserPage(this));
        pagesWidget->addWidget(new ClientPage(this));
        pagesWidget->addWidget(new GroupManagerPage(m_groupManager, this));
        pagesWidget->addWidget(new MumeProtocolPage(this));
        pagesWidget->addWidget(new PathmachinePage(this));
    }

    ui->pagesScrollArea->verticalScrollBar()->setSliderPosition(0);

    pagesWidget->setCurrentIndex(ui->contentsWidget->row(current));
}

