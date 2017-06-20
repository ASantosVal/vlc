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

#include <QSignalMapper> /* for the Help and About popups */

class CoverArtLabelExt;
class Chromaprint;

class ExtMetaManagerDialog : public QVLCDialog, public Singleton<ExtMetaManagerDialog>
{
    Q_OBJECT
private:
    ExtMetaManagerDialog( intf_thread_t * );
    virtual ~ExtMetaManagerDialog();

    /* Help and About windows' text (HTML) */
    const char *help_text = "This window is designed to help you <b>manage "
    "your music's metadata/tags</b>."
    "<br><br>"
    "You can put the mouse over an item to see tips and aditional help info."
    "<br><br>"
    "First you have to <b>load your files</b>. Yo can choose to load them from "
    "the current playlist or from a folder. Note that <b>only audio files</b> "
    "will be loaded (other files will be ignored)."
    "<br><br>"
    "The data will automatically be added to the table. Now you can <b>edit it "
    "directly on the table or use the automatic search</b> function. You can "
    "also resize the table as you wish."
    "<br><br>"
    "When <b>searching</b>, the best option is automatically selected (but not "
    "saved, yet). If you want to be able to choose between the found options, "
    "activate de advanced search. Note that only the selected entries will be "
    "searched (the ones with the checkbox activated)."
    "<br><br>"
    "Once you hace edited/searched all your information, you can <b>save "
    "it</b> definitely with the <i>Save button</i>. If you want to discard "
    "the changes made on the table, click the <i>Reload</i> button. To "
    "empty the table, hit the <i>Clear</i> button."
    "<br><br>"
    "To <b>exit</b>, click the <i>Cancel</i> button or just close the window.";

    const char *about_text = "<big>Extended Metadata Manager</big> <br><br>"
    "This module has been created by <b>Asier Santos Valcárcel</b> as the final project "
    "for his Computer Science Engineering degree at <i>University of the "
    "Basque Country (UPV/EHU)</i>."
    "<br><br>"
    "It was my first time using QT, C++ and in general managing a big project. "
    "I did my best, but there may still be bugs and improvements to be made on "
    "the code. Feel free to contact me if you have a suggestion. "
    "<br><br>"
    "<b>Contact information:</b><br>"
    "<b>Email:</b> asiersantosval@gmail.com<br>"
    "<b>LinkedIn:</b> "
    "<a href=\"https://www.linkedin.com/in/asier-santos-valcarcel\">Profile</a></a><br>"
    "<b>GitHub for the project:</b> "
    "<a href=\"https://github.com/ASantosVal/vlc-extension-trials\">Repository</a></a><br>";

    /* Text for the "tips" */
    const char *getFromPlaylist_tip = "Load files into the table from the current playlist.ONLY THE AUDIO FILES.";
    const char *getFromFolder_tip = "Load files into the table from a file/folder. ONLY THE AUDIO FILES.";
    const char *help_tip = "Launch the help window.";
    const char *about_tip = "Learn more about this window and it's creator.";
    const char *searchNow_tip = "Start the automatic search of the selected item's information.";
    const char *saveAll_tip = "Save all the changes you have made permanently (only selected rows).";
    const char *restoreAll_tip = "Discard your changes and reaload the original information.";
    const char *clearTable_tip = "Clear the table.";
    const char *cancel_tip = "Go back to VLC media player.";
    const char *progressBar_tip = "Progress made by the search function.";
    const char *checkbox_tip = "When this checkbox is NOT selected, it will be "
    "IGNORED by search and save functions.";
    const char *artwork_tip = "This is the selected item's current artwork/cover. "
    "Use the buttons on the table to change it.";
    const char *artworkButton_tip = "Change this item's artwork/cover.";
    const char *acoustid_tip = "With this option selected, your files "
    "will be searched in acoustID database, which can identify files even "
    "when guess information is given, by analyzing the file's content.";
    const char *disableFastSearch_tip = "With this option selected, instead of "
    "choosing automatically the \"best\" option, a window will show asking you "
    "to choose which entry is the correct.";

    /* Declarations for the fingerprinter */
    Chromaprint *t;
    fingerprint_request_t *p_r;

    /* The widget used to show the artwork */
    CoverArtLabelExt *art_cover;

    /* An array with the items the window is working at a the moment */
    vlc_array_t *workspace;

    /* The User interface (UI) made in QT */
    Ui::ExtMetaManagerWidget ui;

    /* Mapper used on the buttons on the table to know from which row is the
    call being made */
    QSignalMapper ButtonSignalMapper;

private slots:
    void close() Q_DECL_OVERRIDE;
    void closeEvent(QCloseEvent *event);

    void getFromPlaylist();
    void getFromFolder();
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
    void changeArtwork(int row);
    input_item_t* getItemFromRow(int row);
    input_item_t* getItemFromURI(const char* uri);
    bool isAudioFile(const char* uri);

    friend class    Singleton<ExtMetaManagerDialog>;
public:
    void toggleVisible();
};

#endif
