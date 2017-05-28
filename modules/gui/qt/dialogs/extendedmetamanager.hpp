/*****************************************************************************
 * extendedmetamanager.hpp : Extended Metadata Manager dialogs
 ****************************************************************************
 * Copyright (C) 2017 Asier Santos Valcárcel
 * Authors: Asier Santos Valcárcel
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *****************************************************************************/

#ifndef QVLC_EXTMETAMANAGER_DIALOG_H_
#define QVLC_EXTMETAMANAGER_DIALOG_H_ 1

#include "util/qvlcframe.hpp"
#include "util/singleton.hpp"

#include "ui/extmetamanager.h" //include de compiled version of extmetamanager.ui

class ExtMetaManagerDialog : public QVLCDialog, public Singleton<ExtMetaManagerDialog>
{
    Q_OBJECT
private:
    ExtMetaManagerDialog( intf_thread_t * );
    virtual ~ExtMetaManagerDialog();

    Ui::ExtMetaManagerWidget ui; //add de ui
private slots:
    void close() Q_DECL_OVERRIDE;
    void cancel() Q_DECL_OVERRIDE;

    void getFromPlaylist();
    void getFromFolder();
    void searchNow();
    void saveAll();
    void restoreAll();
    void help();
    void about();
    void clearTable();
    void addTableEntry(QString url);
    void updateArtwork(int row, int column);
    void showArtwork();

    friend class    Singleton<ExtMetaManagerDialog>;
public:
    void toggleVisible();
};

#endif
