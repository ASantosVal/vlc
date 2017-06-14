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

//aliases for the table's columns' name
#define COL_CHECKBOX 0
#define COL_TITLE 1
#define COL_ARTIST 2
#define COL_ALBUM 3
#define COL_GENRE 4
#define COL_TRACKNUM 5
#define COL_PUBLISHER 6
#define COL_COPYRIGHT 7
#define COL_ARTWORK 8
#define COL_PATH 9

#include "util/qvlcframe.hpp"
#include "util/singleton.hpp"

#include "vlc_fingerprinter.h" //VLC's fingerprinting api

#include "ui/extmetamanager.h" // Include the precompiled version of extmetamanager.ui

class CoverArtLabelExt;
class Chromaprint;

class ExtMetaManagerDialog : public QVLCDialog, public Singleton<ExtMetaManagerDialog>
{
    Q_OBJECT
private:
    ExtMetaManagerDialog( intf_thread_t * );
    virtual ~ExtMetaManagerDialog();

    const char *help_text = "This window is designed to help you <b>manage "
    "your music's metadata/tags</b>.<br><br>"
    "First you have to <b>load your files</b>. Yo can choose to load them from "
    "the current playlist or from a folder.<br><br>"
    "The data will automatically be added to the table. Now you can <b>edit it "
    "directly on the table or use the automatic search</b> function.<br><br>"
    "When searching, the best option is automatically selected (but not saved, "
    "yet). If you want to be able to choose between the found options, "
    "activate de advanced search. Note that onlye the selected entries will be "
    "searched (the ones with the checkbox activated).<br><br>"
    "Once you hace edited/searched all your information, you can <b>save "
    "it</b> definitively with the <i>Save button</i>. If you want to discard "
    "the changes made on the table, you can click the <i>Reload</i> button. To "
    "empty the table, hit the <i>Clear</i> button.<br><br>"
    "To <b>exit</b>, click the <i>Cancel</i> button or just close the window.";

    const char *about_text = "<big>Extended Metadata Manager</big> <br><br>"
    "This module has been created by <b>Asier Santos Valcárcel</b> as the final project "
    "for his Computer Science Engineering degree at <i>University of the "
    "Basque Country (UPV/EHU)</i>.<br><br>"
    "It was my first time using QT, C++ and in general managing a big project. "
    "I did my best, but there may still be bugs and improvements to be made on "
    "the code. Contact me if you have a suggestion. <br><br>"
    "<b>Contact information:</b><br>"
    "<b>Email:</b> asiersantosval@gmail.com<br>"
    "<b>LinkedIn:</b> "
    "<a href=\"https://www.linkedin.com/in/asier-santos-valcarcel\">Profile</a></a><br>"
    "<b>GitHub for the project:</b> "
    "<a href=\"https://github.com/ASantosVal/vlc-extension-trials\">Repository</a></a><br>";

    /* Declarations for the fingerprinter */
    Chromaprint *t;
    fingerprint_request_t *p_r;

    /* The widget used to show the artwork */
    CoverArtLabelExt *art_cover;

    /* An array with the items the window is working at a the moment */
    vlc_array_t *workspace;

    /*Boolean which means if the current items are from the PL or from a file.
     This is needed because the only way I found to preparse (load their
     metadata) the items got form a file is by adding them to the PL, which
     creates a great mess. Using this variable the program will adapt to the
     sitution and delete the items we have added to the PL (clean it).*/
    bool playlistLoaded;

    //The UI
    Ui::ExtMetaManagerWidget ui;

private slots:
    void close() Q_DECL_OVERRIDE;
    void closeEvent(QCloseEvent *event);

    void getFromPlaylist();
    void getFromFolder();
    void clearPlaylist();
    void searchNow();
    void saveAll();
    void fingerprintTable(bool fast);
    void fingerprint(input_item_t *p_item, bool fast);
    void restoreAll();
    void cleanUp();
    void helpDialog();
    void aboutDialog();
    void clearTable();
    void multipleItemsChanged( QTableWidgetItem *item );
    void addTableEntry(input_item_t *p_item);
    void updateTableEntry(input_item_t *p_item, int row);
    bool rowIsSelected(int row);
    void updateArtwork(int row, int column);
    void changeArtwork();
    input_item_t* getItemFromRow(int row);
    input_item_t* getItemFromURI(const char* uri);

    friend class    Singleton<ExtMetaManagerDialog>;
public:
    void toggleVisible();
};

#endif
